#ifndef RTPSENDMYLIB_H
#define RTPSENDMYLIB_H
#include "rtpsendmylib.h"
#endif

static RTPSession                   sess;
static RTPUDPv4TransmissionParams   transparams;
static RTPSessionParams             sessparams;
static uint                         portbase,destport;
static uint                         destip;
static std::string                  ipstr = "";
static int                          status,i,num;
static int                          timeStampInc = 90000 / 10;
static long long                    currentTime;
static int                          sendFrameCount = 0;

static unsigned char* nalBuffer;
static unsigned char* sendBuffer;



int rtp_params_init()
{   
	printf("debug\n");
    ipstr = "192.168.2.3";
    destport = 9000;
    portbase = 1234;
    num = 0;
    destip = inet_addr(ipstr.c_str());
    if (destip == INADDR_NONE)
    {
        std::cerr << "Bad IP address specified" << std::endl;
        return -1;
    }

       destip = ntohl(destip);

       sessparams.SetOwnTimestampUnit((1.0f/20.0f));
       sessparams.SetAcceptOwnPackets(true);
       sessparams.SetMaximumPacketSize(MAX_RTP_PKT_LENGTH+20);
       transparams.SetPortbase(portbase);
       fprintf(stderr,"SetPortbass\n");
       status = sess.Create(sessparams,&transparams);
       fprintf(stderr, "Creat session\n");
       checkerror(status);

       RTPIPv4Address addr(destip,destport);


      status = sess.AddDestination(addr);

       fprintf(stderr,"Add to Destination\n");
       checkerror(status);
       currentTime = clock();
	   
		sendBuffer = (unsigned char*)malloc(1000*1000);
		nalBuffer = (unsigned char*)malloc(1000*1000);
}

void rtp_send(unsigned char *buf, int len)
{


    int n    = len / MAX_RTP_PKT_LENGTH;
    int last = len % MAX_RTP_PKT_LENGTH;
    if(last > 0) n++;

    int timeInc ;

    //currentTime += timeStampInc;
    //fprintf(stderr,"buf len = \n");
    if(len>0)
    	fprintf(stderr,"send count2:\n");
    char nalHead = buf[0];
    if(len < MAX_RTP_PKT_LENGTH)
    {    fprintf(stderr,"send count74:\n");
        status = sess.SendPacket((void*)buf,len,H264,true,timeStampInc);

    }
    else
    {
       for(int i = 0; i < n;i++)
       {

            sendBuffer[0] = (nalHead & 0x60)|28;//fu——a分包的fu indicator的前三位为原来nalhead的前三位
            sendBuffer[1] = (nalHead & 0x1f);//fu_a分包的fu headr的后五位为原来nalhead的后五位
            
                if(0 == i)
                {
                       timeInc = timeStampInc;
                       sendBuffer[1] |= 0x80;
                       memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH+1],MAX_RTP_PKT_LENGTH);
                       status = sess.SendPacket((void*)sendBuffer,MAX_RTP_PKT_LENGTH+2,H264,false,timeInc);
                }

            else if(i == n - 1) //send the last subpacket
            {
               timeInc = 0;
               sendBuffer[1] |= 0x40;
               memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH+1],last-1);
                fprintf(stderr,"send count99:\n");
               status = sess.SendPacket((void*)sendBuffer,last +1 ,H264,true,timeInc);
            }
            else
            {
                timeInc = 0;

               memcpy(&sendBuffer[2],&buf[i*MAX_RTP_PKT_LENGTH+1],MAX_RTP_PKT_LENGTH);
               status = sess.SendPacket((void*)sendBuffer,MAX_RTP_PKT_LENGTH+2 ,H264,false,timeInc);

            }

            checkerror(status);
                //sess.OnPollThreadStart();

    //        free(tempBuf);
     
            status = sess.Poll();
    //        checkerror(status);
       }
    }

}

void checkerror(int err)
{
  if (err < 0) {
  const  char* errstr = RTPGetErrorString(err).c_str();
    printf("Error:%s/n", errstr);
    exit(-1);
  }
}


void nal_rtp_send(unsigned char* buf, int len)
{
    int pos = 0;
    fprintf(stderr,"send count1:\n");
    while(pos < len)
    {
        int nalLen = get_next_nalu(&buf[pos],len - pos,nalBuffer);
        rtp_send(nalBuffer + 4,nalLen - 4);                         //drop the start code 00 00 00 01
        pos += nalLen;
    }

}


int get_next_nalu(unsigned char* buf,int buf_len,unsigned char* outbuf)
{
    int pos = 0;
    int findStartcode;
    if(buf_len > 4)
    {
        findStartcode = find_start_code(&buf[pos]);
    }
    else return 0 ;
    if(findStartcode)
    {

        do
        {
                    outbuf[pos] = buf[pos];

                    pos++;
                    findStartcode = find_start_code(&buf[pos]);
        }while(!findStartcode && (pos + 4 < buf_len));
    }

    if(findStartcode)
    {

        return pos ;
    }

    else if(pos + 4 >= buf_len)
    {
        do{outbuf[pos] = buf[pos];pos++;}while(pos < buf_len);
        return buf_len;
    }
    else return 0;
}

int find_start_code(unsigned char*buf)
{
  if(  (buf[0] == 0)&&
      (buf[1] == 0)&&
      (buf[2] == 0)&&
      (buf[3] == 1)
          )
  {
    return 1;
  }

  else
      return 0;
}

int rtp_params_stop()
{   
	// printf("destip is %s! \n", ipstr);
		// fflush(stdout);
	printf("destport is %d! \n", destport);
		fflush(stdout);
	
       sess.BYEDestroy(RTPTime(10,0),0,0); 
		printf("sess.BYEDestroy end! \n");
		fflush(stdout);
       fprintf(stderr,"Delete Destination\n");
       checkerror(status);
       return 0;
}