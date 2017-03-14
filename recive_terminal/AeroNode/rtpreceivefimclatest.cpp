#include "rtpreceivefimclatest.h"

BUFTYPE *fimc0_out_buf;

int fimc0_fd;
static int n_buffer = 0;
void *fimc0_cap[16];
unsigned char * temp_buf = NULL;

long fimc0_cap_buf_length;

struct pollfd fds;
char format[21]={0x00,
                 0x00,
                 0x00,
                 0x01,
                 0x67,
                 0x4D,
                 0x00,
                 0x28,
                 0xE9,
                 0x01,
                 0x40,
                 0x7B, 
                 0x20,  
                 0x00,  
                 0x00,  
                 0x00,  
                 0x01,  
                 0x68,  
                 0xCE,  
                 0x04,  
                 0x72};  
int idx=0;

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
int lcd_fd;
int lcd_buf_size;
char *fb_buf = NULL;
struct dev_buffers {
	long count; /* number of buffers */
	long nplanes; /* number of planes per buffer */
	long *lengths; /* array of plane lengths */
	 /* array of plane addresses, address of plane p in buffer b
	    is at addr[nplanes * b + p] */
	char **addr;
	 /* array of bytes used by plane, bytesused of plane p in buffer b
	    is at bytesused[nplanes * b + p] */
	long *bytesused;
};
struct dev_buffers *bufs;
FILE *fpo;
void  * pBufOut1;
long    pOutLen1;
void * pBufOut2;
long   pOutLen2;
void *openHandle;
void *virInBuf;
void *phyInBuf;
int count=0;
char headbuf[50];
int headlen;
int cnt = 0;
SSBSIP_MFC_DEC_OUTBUF_STATUS status = MFC_GETOUTBUF_STATUS_NULL;
SSBSIP_MFC_ERROR_CODE err;
SSBSIP_MFC_DEC_OUTPUT_INFO oinfo;
 char * ylin=NULL, * clin=NULL;
static pthread_t receive_tid;
static pthread_t display_tid; 
static sem_t lcd_sem;


static inline int __info(const char *prefix, const char *file, int line,
	const char *fmt, ...)
{
	int errsv = errno;
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "%s(%s:%d): ", prefix, file, line);
	vfprintf(stderr, fmt, va);
	va_end(va);
	errno = errsv;

	return 1;
}
 
void csc_tiled_to_linear(char *yuv420_dest, char *nv12t_src, int yuv420_width, int yuv420_height)
{
    unsigned int i, j;
    unsigned int tiled_x_index = 0, tiled_y_index = 0;
    unsigned int aligned_x_size = 0;
    unsigned int tiled_offset = 0, tiled_offset1 = 0, tiled_offset2 = 0, tiled_offset3 = 0;
    unsigned int temp1 = 0, temp2 = 0;
 
    if (yuv420_width >= 1024) {
        for (i=0; i<yuv420_height; i=i+1) {
            tiled_offset = 0;
            tiled_y_index = i>>5;
            if (tiled_y_index & 0x1) {
                /* odd fomula: 2+x_block_num*(y-1) */
                tiled_offset = tiled_y_index-1;
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_offset*(temp1>>6);
                tiled_offset = tiled_offset+2;
                tiled_offset = tiled_offset<<11;
                tiled_offset1 = tiled_offset+2048*1;
                tiled_offset2 = tiled_offset+2048*2;
                tiled_offset3 = tiled_offset+2048*3;
                temp2 = 8;
            } else {
                temp2 = ((yuv420_height+31)>>5)<<5;
                /* even fomula: x_block_num*y */
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_y_index*(temp1>>6);
                tiled_offset = tiled_offset<<11;
                if ((i+32)<temp2) {
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*6;
                    tiled_offset3 = tiled_offset+2048*7;
                    temp2 = 8;
                } else {
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*2;
                    tiled_offset3 = tiled_offset+2048*3;
                    temp2 = 4;
                }
            }
            temp1 = i&0x1F;
            memcpy(yuv420_dest+yuv420_width*(i), nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*1, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*2, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*3, nv12t_src+tiled_offset3+64*(temp1), 64);
 
            tiled_offset = tiled_offset+temp2*2048;
            tiled_offset1 = tiled_offset1+temp2*2048;
            tiled_offset2 = tiled_offset2+temp2*2048;
            tiled_offset3 = tiled_offset3+temp2*2048;
            memcpy(yuv420_dest+yuv420_width*(i)+64*4, nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*5, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*6, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*7, nv12t_src+tiled_offset3+64*(temp1), 64);
 
            tiled_offset = tiled_offset+temp2*2048;
            tiled_offset1 = tiled_offset1+temp2*2048;
            tiled_offset2 = tiled_offset2+temp2*2048;
            tiled_offset3 = tiled_offset3+temp2*2048;
            memcpy(yuv420_dest+yuv420_width*(i)+64*8, nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*9, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*10, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*11, nv12t_src+tiled_offset3+64*(temp1), 64);
 
            tiled_offset = tiled_offset+temp2*2048;
            tiled_offset1 = tiled_offset1+temp2*2048;
            tiled_offset2 = tiled_offset2+temp2*2048;
            tiled_offset3 = tiled_offset3+temp2*2048;
            memcpy(yuv420_dest+yuv420_width*(i)+64*12, nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*13, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*14, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+yuv420_width*(i)+64*15, nv12t_src+tiled_offset3+64*(temp1), 64);
        }
        aligned_x_size = 1024;
    }
 
    if ((yuv420_width-aligned_x_size) >= 512) {
        for (i=0; i<yuv420_height; i=i+1) {
            tiled_offset = 0;
            tiled_y_index = i>>5;
            if (tiled_y_index & 0x1) {
                /* odd fomula: 2+x_block_num*(y-1) */
                tiled_offset = tiled_y_index-1;
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_offset*(temp1>>6);
                tiled_offset = tiled_offset+2;
                temp1 = aligned_x_size>>5;
                tiled_offset = tiled_offset+temp1;
                tiled_offset = tiled_offset<<11;
                tiled_offset1 = tiled_offset+2048*1;
                tiled_offset2 = tiled_offset+2048*2;
                tiled_offset3 = tiled_offset+2048*3;
                temp2 = 8;
            } else {
                temp2 = ((yuv420_height+31)>>5)<<5;
                /* even fomula: x_block_num*y */
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_y_index*(temp1>>6);
                tiled_offset = tiled_offset<<11;
                if ((i+32)<temp2) {
                    temp1 = aligned_x_size>>5;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*6;
                    tiled_offset3 = tiled_offset+2048*7;
                    temp2 = 8;
                } else {
                    temp1 = aligned_x_size>>6;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*2;
                    tiled_offset3 = tiled_offset+2048*3;
                    temp2 = 4;
                }
            }
            temp1 = i&0x1F;
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i), nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*1, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*2, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*3, nv12t_src+tiled_offset3+64*(temp1), 64);

            tiled_offset = tiled_offset+temp2*2048;
            tiled_offset1 = tiled_offset1+temp2*2048;
            tiled_offset2 = tiled_offset2+temp2*2048;
            tiled_offset3 = tiled_offset3+temp2*2048;
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*4, nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*5, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*6, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*7, nv12t_src+tiled_offset3+64*(temp1), 64);
        }
        aligned_x_size = aligned_x_size+512;
    }

    if ((yuv420_width-aligned_x_size) >= 256) {
        for (i=0; i<yuv420_height; i=i+1) {
            tiled_offset = 0;
            tiled_y_index = i>>5;
            if (tiled_y_index & 0x1) {
                /* odd fomula: 2+x_block_num*(y-1) */
                tiled_offset = tiled_y_index-1;
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_offset*(temp1>>6);
                tiled_offset = tiled_offset+2;
                temp1 = aligned_x_size>>5;
                tiled_offset = tiled_offset+temp1;
                tiled_offset = tiled_offset<<11;
                tiled_offset1 = tiled_offset+2048*1;
                tiled_offset2 = tiled_offset+2048*2;
                tiled_offset3 = tiled_offset+2048*3;
            } else {
                temp2 = ((yuv420_height+31)>>5)<<5;
                /* even fomula: x_block_num*y */
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_y_index*(temp1>>6);
                tiled_offset = tiled_offset<<11;
                if ((i+32)<temp2) {
                    temp1 = aligned_x_size>>5;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*6;
                    tiled_offset3 = tiled_offset+2048*7;
                } else {
                    temp1 = aligned_x_size>>6;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                    tiled_offset2 = tiled_offset+2048*2;
                    tiled_offset3 = tiled_offset+2048*3;
                }
            }
            temp1 = i&0x1F;
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i), nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*1, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*2, nv12t_src+tiled_offset2+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64*3, nv12t_src+tiled_offset3+64*(temp1), 64);
        }
        aligned_x_size = aligned_x_size+256;
    }

    if ((yuv420_width-aligned_x_size) >= 128) {
        for (i=0; i<yuv420_height; i=i+2) {
            tiled_offset = 0;
            tiled_y_index = i>>5;
            if (tiled_y_index & 0x1) {
                /* odd fomula: 2+x_block_num*(y-1) */
                tiled_offset = tiled_y_index-1;
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_offset*(temp1>>6);
                tiled_offset = tiled_offset+2;
                temp1 = aligned_x_size>>5;
                tiled_offset = tiled_offset+temp1;
                tiled_offset = tiled_offset<<11;
                tiled_offset1 = tiled_offset+2048*1;
            } else {
                temp2 = ((yuv420_height+31)>>5)<<5;
                /* even fomula: x_block_num*y */
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_y_index*(temp1>>6);
                tiled_offset = tiled_offset<<11;
                if ((i+32)<temp2) {
                    temp1 = aligned_x_size>>5;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                } else {
                    temp1 = aligned_x_size>>6;
                    tiled_offset = tiled_offset+(temp1<<11);
                    tiled_offset1 = tiled_offset+2048*1;
                }
            }
            temp1 = i&0x1F;
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i), nv12t_src+tiled_offset+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i)+64, nv12t_src+tiled_offset1+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i+1), nv12t_src+tiled_offset+64*(temp1+1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i+1)+64, nv12t_src+tiled_offset1+64*(temp1+1), 64);
        }
        aligned_x_size = aligned_x_size+128;
    }

    if ((yuv420_width-aligned_x_size) >= 64) {
        for (i=0; i<yuv420_height; i=i+4) {
            tiled_offset = 0;
            tiled_x_index = aligned_x_size>>6;
            tiled_y_index = i>>5;
            if (tiled_y_index & 0x1) {
                /* odd fomula: 2+x+(x>>2)<<2+x_block_num*(y-1) */
                tiled_offset = tiled_y_index-1;
                temp1 = ((yuv420_width+127)>>7)<<7;
                tiled_offset = tiled_offset*(temp1>>6);
                tiled_offset = tiled_offset+tiled_x_index;
                tiled_offset = tiled_offset+2;
                temp1 = (tiled_x_index>>2)<<2;
                tiled_offset = tiled_offset+temp1;
                tiled_offset = tiled_offset<<11;
            } else {
                temp2 = ((yuv420_height+31)>>5)<<5;
                if ((i+32)<temp2) {
                    /* even1 fomula: x+((x+2)>>2)<<2+x_block_num*y */
                    temp1 = tiled_x_index+2;
                    temp1 = (temp1>>2)<<2;
                    tiled_offset = tiled_x_index+temp1;
                    temp1 = ((yuv420_width+127)>>7)<<7;
                    tiled_offset = tiled_offset+tiled_y_index*(temp1>>6);
                    tiled_offset = tiled_offset<<11;
                } else {
                    /* even2 fomula: x+x_block_num*y */
                    temp1 = ((yuv420_width+127)>>7)<<7;
                    tiled_offset = tiled_y_index*(temp1>>6);
                    tiled_offset = tiled_offset+tiled_x_index;
                    tiled_offset = tiled_offset<<11;
                }
            }

            temp1 = i&0x1F;
            temp2 = aligned_x_size&0x3F;
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i), nv12t_src+tiled_offset+temp2+64*(temp1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i+1), nv12t_src+tiled_offset+temp2+64*(temp1+1), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i+2), nv12t_src+tiled_offset+temp2+64*(temp1+2), 64);
            memcpy(yuv420_dest+aligned_x_size+yuv420_width*(i+3), nv12t_src+tiled_offset+temp2+64*(temp1+3), 64);
        }
        aligned_x_size = aligned_x_size+64;
    }

    if (yuv420_width != aligned_x_size) {
        for (i=0; i<yuv420_height; i=i+4) {
            for (j=aligned_x_size; j<yuv420_width; j=j+4) {
                tiled_offset = 0;
                tiled_x_index = j>>6;
                tiled_y_index = i>>5;
                if (tiled_y_index & 0x1) {
                    /* odd fomula: 2+x+(x>>2)<<2+x_block_num*(y-1) */
                    tiled_offset = tiled_y_index-1;
                    temp1 = ((yuv420_width+127)>>7)<<7;
                    tiled_offset = tiled_offset*(temp1>>6);
                    tiled_offset = tiled_offset+tiled_x_index;
                    tiled_offset = tiled_offset+2;
                    temp1 = (tiled_x_index>>2)<<2;
                    tiled_offset = tiled_offset+temp1;
                    tiled_offset = tiled_offset<<11;
                } else {
                    temp2 = ((yuv420_height+31)>>5)<<5;
                    if ((i+32)<temp2) {
                        /* even1 fomula: x+((x+2)>>2)<<2+x_block_num*y */
                        temp1 = tiled_x_index+2;
                        temp1 = (temp1>>2)<<2;
                        tiled_offset = tiled_x_index+temp1;
                        temp1 = ((yuv420_width+127)>>7)<<7;
                        tiled_offset = tiled_offset+tiled_y_index*(temp1>>6);
                        tiled_offset = tiled_offset<<11;
                    } else {
                        /* even2 fomula: x+x_block_num*y */
                        temp1 = ((yuv420_width+127)>>7)<<7;
                        tiled_offset = tiled_y_index*(temp1>>6);
                        tiled_offset = tiled_offset+tiled_x_index;
                        tiled_offset = tiled_offset<<11;
                    }
                }

                temp1 = i&0x1F;
                temp2 = j&0x3F;
                memcpy(yuv420_dest+j+yuv420_width*(i), nv12t_src+tiled_offset+temp2+64*(temp1), 4);
                memcpy(yuv420_dest+j+yuv420_width*(i+1), nv12t_src+tiled_offset+temp2+64*(temp1+1), 4);
                memcpy(yuv420_dest+j+yuv420_width*(i+2), nv12t_src+tiled_offset+temp2+64*(temp1+2), 4);
                memcpy(yuv420_dest+j+yuv420_width*(i+3), nv12t_src+tiled_offset+temp2+64*(temp1+3), 4);
            }
        }
    }
} 

int fimc0_out_dbuf()
{
	
	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b, buf;
	struct v4l2_plane plane[2];
	
	/* enqueue buffer to fimc0 output */
	CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;

	b.m.planes = plane;
	b.length = 2;
	ret = ioctl(fimc0_fd, VIDIOC_DQBUF, &b);	
	 //printf("the index is %d=============================================================\n", b.index);
	/*idx++;
	 if(idx>2)
	 	idx=0;*/
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_DQBUF: %s\n", ERRSTR))
		return -errno;	

	
	}

int fimc0_out_qbuf()
{

	int ret;
	struct v4l2_buffer b;
	struct v4l2_plane plane[2];
	
	CLEAR(*plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = plane;
	b.length = 2;
	b.index = idx;
	ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
		return -errno;

}
int fimc0_cap_qbuf ()
{
	int ret;
	struct v4l2_buffer b;
	struct v4l2_plane plane;
	
	
		CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = &plane;
	b.length = 1;
	/*idx--;
	if(idx<0)
		idx=2;
		*/
	b.index =idx ;
	
	ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
		return -errno;
	
	}
int fimc0_cap_dbuf()
{

	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b, buf;
	struct v4l2_plane plane;
	static int count = 0;
	/* enqueue buffer to fimc0 output */
	CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = &plane;
	b.length = 1;
	ret = ioctl(fimc0_fd, VIDIOC_DQBUF, &b);
	 idx = b.index;
	 //printf("the index is %d=============================================================\n", b.index);	 	   
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_DQBUF: %s\n", ERRSTR))
		return -errno;	
	
	// 20170113
	//memcpy(fb_buf,fimc0_cap[b.index],fimc0_cap_buf_length);
	
	/* int j;
    for (j=0; j<480; j++) {
            memcpy(fb_buf+j*800*4 ,fimc0_cap[b.index]+j*640*4,640*4);
    } */
	
	temp_buf = (unsigned char *)fimc0_cap[b.index];
	
	
	
//	printf("%s,%d\n",__func__, count);
	return 0;
}
 
 
int UnpackRTPH264( void   *  bufIn,  long  len,   void **  pBufOut,   long *  pOutLen)
 {   
     * pOutLen  =   0 ;
     if  (len  <  RTP_HEADLEN)
     {
         return   1 ;
    } 

    unsigned  char *  src  =  (unsigned  char * )bufIn  +RTP_HEADLEN ;
    unsigned  char  head1  =   * src; // 获取第一个字节 
     unsigned  char  head2  =   * (src + 1 ); // 获取第二个字节 
     unsigned  char  nal  =  head1  &   0x1f ; // 获取FU indicator的类型域， 
     unsigned  char  flag  =  head2  &   0xe0 ; // 获取FU header的前三位，判断当前是分包的开始、中间或结束 
     unsigned  char  nal_fua  =  (head1  &   0xe0 )  |  (head2  &   0x1f ); // FU_A nal nuheader
      int  bFinishFrame  =   0 ;
      
     if  (nal == 0x1c ) // 判断NAL的类型为0x1c=28，说明是FU-A分片 
      { // fu-a 
      
          if  (flag == 0x80 ) // 开始 
          { 
             * pBufOut  =  src - 3 ;
             * (( int * )( * pBufOut))  =   0x01000000  ; // zyf:大模式会有问题 
              * (( char * )( * pBufOut) + 4 )  =  nal_fua;
             *  pOutLen  =  len  -  RTP_HEADLEN  +   3 ;
          
        } 
         else   if (flag == 0x40 ) // 结束 
          { 
             * pBufOut  =  src + 2 ;
             *  pOutLen  =  len  -  RTP_HEADLEN  -   2 ;
        } 
         else // 中间 
          {  
             * pBufOut  =  src + 2 ;
             *  pOutLen  =  len  -  RTP_HEADLEN  -   2 ;
        } 
    } 
     else // 单包数据 
      {     
         * pBufOut  =  src - 4 ;
         * (( int * )( * pBufOut))  =   0x01000000 ; // zyf:大模式会有问题 
          
          *pOutLen  =  len  -  RTP_HEADLEN  +   4 ;
          	
    }  
    
  /*  unsigned  char *  bufTmp  =  (unsigned  char * )bufIn;
     if  (bufTmp[ 1 ]  &   0x80 )
     {  printf("111111111111111\n");
        bFinishFrame  =   1 ; // rtp mark 
     } 
     else 
      {
        bFinishFrame  =   0 ;
    } 
     return  bFinishFrame;*/
}   
class CVideoData{
public:
    unsigned char * m_pBuffer;
    long m_lLength;
    long m_lMaxLength;
    uint64_t m_pts;
 
 
    CVideoData(){
    	          m_pBuffer = new unsigned char[BUFFER_SIZE];
    	          m_lLength = 0; 
    	          m_pts = 0;
    	          m_lMaxLength = BUFFER_SIZE;
    	}
    CVideoData(long lLength){m_lLength = lLength;m_pts = 0;m_pBuffer = new unsigned char[BUFFER_SIZE];m_lMaxLength = BUFFER_SIZE;}
    ~CVideoData(){
        if(m_pBuffer != NULL)
        {
            delete []m_pBuffer;
            m_pBuffer = NULL;
        }
        m_lLength = 0;
    }
 
 
};
void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

//
// The new class routine
// The new class routine

class CRTPReceiver : public RTPSession
{
protected:
    void OnPollThreadStep();
    void ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack);
    void OnRTCPCompoundPacket(RTCPCompoundPacket *pack,const RTPTime &receivetime,const RTPAddress *senderaddress);
     
public:
    //CThreadSafeArray m_ReceiveArray;
    void InitBufferSize();
 
private:
    CVideoData* m_pVideoData;
    unsigned char m_buffer[BUFFER_SIZE];
    long m_current_size;
    
};
 
void CRTPReceiver::InitBufferSize()
{
    //m_ReceiveArray.SetMemberSize(BUFFER_SIZE);
    m_pVideoData = new CVideoData();
    memset(m_buffer,0,BUFFER_SIZE);
    m_current_size = 0;
    
}
 
void CRTPReceiver::OnRTCPCompoundPacket(RTCPCompoundPacket *pack,const RTPTime &receivetime,const RTPAddress *senderaddress)
{
    //std::cout<<"Got RTCP packet from: "<<senderaddress<<std::endl;
}
 
 
void CRTPReceiver::OnPollThreadStep()
{   
    BeginDataAccess();
    
    // check incoming packets
    if (GotoFirstSourceWithData())
    {    
        do
        {
            RTPPacket *pack;
            RTPSourceData *srcdat;
             
            srcdat = GetCurrentSourceInfo();
             
            while ((pack = GetNextPacket()) != NULL)
            {
                ProcessRTPPacket(*srcdat,*pack);
                 
                DeletePacket(pack);
            }
        } while (GotoNextSourceWithData());
    }
         
    EndDataAccess();
	//RTPTime::Wait(RTPTime(0.1,0));
}
 
 void CRTPReceiver::ProcessRTPPacket(const RTPSourceData &srcdat,const RTPPacket &rtppack)
{
    // You can inspect the packet and the source's info here
    //std::cout<<"Packet type: "<<rtppack.GetPayloadType()<<std::endl;
    //std::cout<<"Packet says: "<<(char *)rtppack.GetPayloadData()<<std::endl;
    //test RTCP packet
    /*int status = this->SendRTCPAPPPacket(0,(uint8_t*)&("123"),(void*)&("hel"),4);
    checkerror(status);*/
    fds.events |= POLLIN | POLLPRI | POLLOUT;
    fds.fd = fimc0_fd;
    int r ;      
    if(rtppack.GetPayloadType() == 96)
    {
        std::cout<<"Got H264 packet：êo " << rtppack.GetExtendedSequenceNumber() << " from SSRC " << srcdat.GetSSRC() <<std::endl;
        if(rtppack.HasMarker())//如果是最后一包则进行组包
        {  //printf("begin to the last packet\n");
        	 unsigned char* p2 = rtppack.GetPacketData();
        	 long length2=rtppack.GetPacketLength();
			std::cout<<"Got H264 2 " << length2 <<std::endl;

			
        	UnpackRTPH264(p2,length2, &pBufOut2, &pOutLen2); 
            m_pVideoData->m_lLength = m_current_size + (pOutLen2);//得到数据包总的长度
            printf("the total length is %d\n ,m_length=%d,outlen2 = %d.",m_pVideoData->m_lLength,m_current_size, pOutLen2);
            memcpy(m_pVideoData->m_pBuffer,m_buffer,m_current_size);
            memcpy(m_pVideoData->m_pBuffer + m_current_size ,pBufOut2,pOutLen2);
            /* count++;          
            //printf("the count is %d\n",count);   */
            do{ 
            		memcpy(virInBuf,m_pVideoData->m_pBuffer,m_pVideoData->m_lLength);
                err=SsbSipMfcDecExe(openHandle, m_pVideoData->m_lLength);
				if(err<0) 
                fprintf(stderr,"Error: SsbSipMfcDecExe. Code %d\n",err);
            	memset(&oinfo,0,sizeof(oinfo));
            	status = SsbSipMfcDecGetOutBuf(openHandle,&oinfo);
            	
            if(status==MFC_GETOUTBUF_DISPLAY_DECODING || status==MFC_GETOUTBUF_DISPLAY_ONLY) 
			{
            		
            if( ylin  == NULL)
                ylin=(char*)malloc(oinfo.img_width*oinfo.img_height);
            if(!ylin) 
                fprintf(stderr,"Out of memory.\n");    
            // converted tiled to linear nv12 format - Y plane
             csc_tiled_to_linear(ylin,(char *)oinfo.YVirAddr,oinfo.img_width,oinfo.img_height);
             memcpy(bufs->addr[idx*bufs->nplanes + 0],ylin,oinfo.img_width*oinfo.img_height);
            //fwrite(ylin,1,oinfo.img_width*oinfo.img_height,fpo);
            if(clin == NULL)
                clin=(char *)malloc(oinfo.img_width*oinfo.img_height/2);
            if(!clin) 
                fprintf(stderr,"Out of memory.\n");
                
              
            // converted tiled to linear nv12 format - C plane
             csc_tiled_to_linear(clin,(char *)oinfo.CVirAddr,oinfo.img_width,oinfo.img_height/2);
             memcpy(bufs->addr[idx* bufs->nplanes + 1],clin,oinfo.img_width*oinfo.img_height/2);
       
             fimc0_out_qbuf();

            r = poll(&fds, 1, 5);
            if(fds.revents & POLLIN)
            {
			 fimc0_cap_dbuf();
             fimc0_out_dbuf();	
             fimc0_cap_qbuf();         
			}						
            //fwrite(clin,1,oinfo.img_width*oinfo.img_height/2,fpo);
            
            cnt++;
            //printf("the display frame count is %d\n",cnt);
            }	
            		//printf("the status is %d\n",status);
            }while(status==MFC_GETOUTBUF_DECODING_ONLY);
            	   

            //fwrite(m_pVideoData->m_pBuffer,m_pVideoData->m_lLength,1,fp);
             //printf("got one frame\n");
           
 
            memset(m_buffer,0,m_current_size);//清空缓存，为下次做准备
            m_current_size = 0;
            if(clin)
            	{
               free(clin);
               clin=NULL;
              }
            if(ylin)
               {free(ylin);
               	ylin=NULL;
               }
            
          //printf("end to the last packet\n");
        } 
        else//放入缓冲区，在此必须确保有序
        {
            unsigned char* p1 = rtppack.GetPacketData();
            long length1=rtppack.GetPacketLength();
           
            UnpackRTPH264(p1,length1, &pBufOut1,&pOutLen1); 	
			     std::cout<<"Got H264 1 " << length1 <<std::endl;
            memcpy(m_buffer + m_current_size,pBufOut1,pOutLen1);
            m_current_size += pOutLen1;     
        }
    } 
}
//void *receive_thread(void *pVoid)
  void StartReceive()
{
    /*CRTPReceiver sess;*/
    CRTPReceiver sess;
    sess.InitBufferSize();
    std::string ipstr;
    uint16_t portbase;
    int status;
    int num;
 
    // Now, we'll create a RTP session, set the destination
    // and poll for incoming data.
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;
     
	 portbase = 9000;
	 num = 1000;
     
    // IMPORTANT: The local timestamp unit MUST be set, otherwise
    //            RTCP Sender Report info will be calculated wrong
    // In this case, we'll be just use 9000 samples per second.
    sessparams.SetOwnTimestampUnit(1.0/9000.0);     
     
    transparams.SetPortbase(portbase);
    
    status = sess.Create(sessparams,&transparams);  
        
    checkerror(status);
    RTPTime::Wait(RTPTime(num,0));
   
    sess.BYEDestroy(RTPTime(10,0),0,0);
   
}

int fimc0_setfmt()
{
	int err;
	int ret;
	int i;
	struct v4l2_fmtdesc fmt;
	struct v4l2_capability cap;
	struct v4l2_format stream_fmt;
	struct v4l2_input input;
	struct v4l2_control ctrl;
	struct v4l2_streamparm stream;
	struct v4l2_plane_pix_format planes[2];

	//printf("%s: +\n", __func__);

	CLEAR(stream_fmt);
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	struct v4l2_pix_format_mplane *pix_mp =&stream_fmt.fmt.pix_mp;
  
	pix_mp->width =640 ;
	pix_mp->height = 480;
	pix_mp->pixelformat = V4L2_PIX_FMT_NV12M;
	pix_mp->num_planes = 2;
	for(i=0;i<2;i++)
	{ pix_mp->plane_fmt[i].sizeimage=(pix_mp->width*pix_mp->height)/(i+1);
		pix_mp->plane_fmt[i].bytesperline=pix_mp->width;
		}
   //printf("827line %d,%d\n",pix_mp->plane_fmt[0].sizeimage,pix_mp->plane_fmt[1].sizeimage);
	//dump_format("fimc0-output", &stream_fmt);
	ret = ioctl(fimc0_fd, VIDIOC_S_FMT, &stream_fmt);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_S_FMT: %s\n", ERRSTR))
		return -errno;
	//dump_format("fimc0-output", &stream_fmt);
	
	
	
	  CLEAR(stream_fmt);
		/* set format on fimc0 capture */
	  stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	/* try cmdline format, or use fimc0-output instead */

		struct v4l2_pix_format_mplane *pix_mp_f = &stream_fmt.fmt.pix_mp;
	  pix_mp_f->pixelformat = V4L2_PIX_FMT_RGB32;
	  pix_mp_f->width =640;		//800
	  pix_mp_f->height =480;	//600
		pix_mp_f->num_planes = 1;
		pix_mp_f->plane_fmt[0].bytesperline = 0;
		pix_mp_f->plane_fmt[0].sizeimage = pix_mp_f->width*pix_mp_f->height*4;
	//dump_format("pre-fimc0-capture", &stream_fmt);
	ret = ioctl(fimc0_fd, VIDIOC_S_FMT, &stream_fmt);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_S_FMT: %s\n", ERRSTR))
		return -errno;

	//printf("%s -\n", __func__);
	
	
	
	}
	
int  fimc0_reqbufs()
{
	int i ;
	int n;
	int err;
	int ret;
	struct v4l2_control ctrl;
	struct v4l2_requestbuffers reqbuf;
	struct v4l2_requestbuffers rb;
	CLEAR(rb);
	/* enqueue the dmabuf to vivi */
	struct v4l2_buffer b;
	CLEAR(b);


	//printf("%s: +\n", __func__);
		/* request buffers for FIMC0 */
	rb.count = ReqButNum;
	rb.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	rb.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fimc0_fd, VIDIOC_REQBUFS, &rb);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_REQBUFS: %s\n", ERRSTR))
		return -errno;
	//printf("fimc0 output_buf_num:%d\n",rb.count);
	

	n_buffer = rb.count;

	//fimc0_out_buf = calloc(rb.count,sizeof(*fimc0_out_buf));
	/*if(fimc0_out_buf == NULL){
		fprintf(stderr,"Out of memory\n");
		exit(EXIT_FAILURE);
	}*/

	
		/* mmap DMABUF */
	struct v4l2_plane plane[2];
  bufs = (struct dev_buffers *)malloc(sizeof(struct dev_buffers));
	CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.index = 0;
	b.m.planes = plane;
	b.length = 2;
 
		for (n = 0; n <  ReqButNum; ++n) {    
		
		  b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	    b.memory = V4L2_MEMORY_MMAP;
	    b.m.planes = plane;
	    b.length = 2;                 
			b.index = n;                                 
			ret = ioctl(fimc0_fd, VIDIOC_QUERYBUF, &b);
			
			if (ret != 0) 
	  	{                                 
				printf("QUERYBUF failed");                       
				return -1;                                    
			}    
			if (n == 0) 
		 { 
			 for (i = 0; i <b.length; ++i)
				if (b.m.planes[i].length == 0)
					  break;
					  
		  
			bufs->nplanes = i;
			//printf("debug2\n");		
			//printf("the planes is %d\n",bufs->nplanes);
			bufs->lengths =(long *) malloc(bufs->nplanes* sizeof(*bufs->lengths));
			for (i = 0; i < bufs->nplanes; ++i)
				{bufs->lengths[i] = b.m.planes[i].length;
				//printf("the plane[%d]is %d\n",i,bufs->lengths[i]);
        }
			bufs->bytesused = (long*)malloc(ReqButNum * bufs->nplanes
						* sizeof(*bufs->bytesused));
			bufs->addr = (char**)malloc(ReqButNum * bufs->nplanes
						* sizeof(*bufs->addr));
		}
			for (i = 0; i < bufs->nplanes; ++i) {
			bufs->addr[n * bufs->nplanes + i] =(char*) mmap(NULL,
						b.m.planes[i].length,
						PROT_READ | PROT_WRITE,
						MAP_SHARED, fimc0_fd,
						b.m.planes[i].m.mem_offset);
			if (bufs->addr[n * bufs->nplanes + i] == MAP_FAILED) {
				printf("Failed mmap buffer %d for %d", n,fimc0_fd);
				return -1;
			}
		}
	}	
	//	printf("fimc0 output:plane.length:%d\n",fimc0_out_buf_length);
	

	CLEAR(plane);
	CLEAR(b);

	rb.count = ReqButNum;
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	rb.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fimc0_fd, VIDIOC_REQBUFS, &rb);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_REQBUFS: %s\n", ERRSTR))
		return -errno;

	for (n = 0; n < ReqButNum; ++n) {
	
		b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		b.memory = V4L2_MEMORY_MMAP;
		b.index = n;
		b.m.planes = plane;
		b.length = 1;

		b.index = n;
		ret = ioctl(fimc0_fd, VIDIOC_QUERYBUF, &b);
		if (ERR_ON(ret < 0, "fimc0: VIDIOC_QUERYBUF: %s\n", ERRSTR))
			return -errno;

		fimc0_cap[n] = mmap(NULL,
						b.m.planes[0].length,
						PROT_READ | PROT_WRITE,
						MAP_SHARED, fimc0_fd,
						b.m.planes[0].m.mem_offset);
			if (fimc0_cap[n] == MAP_FAILED) {
				printf("Failed mmap buffer %d for %d\n", n,
							fimc0_fd);
				return -1;
			}

		fimc0_cap_buf_length = b.m.planes[0].length;
		printf("fimc0 capture:plane.length:%d\n",fimc0_cap_buf_length);	
	}

	printf("%s -\n", __func__);
	
	
	}	
	
int initfimc()
{
	fimc0_setfmt();
	fimc0_reqbufs();
	
	}
int start_capturing()
{
	unsigned int i;
	enum v4l2_buf_type type;
		int ret;
	
	struct v4l2_plane plane;
	printf("%s +\n", __func__);

	CLEAR(plane);

	for(i = 0;i < n_buffer;i ++)
	{
		struct v4l2_buffer b;
		CLEAR(b);
		b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		b.memory = V4L2_MEMORY_MMAP;
		b.index = 0;
		b.m.planes = &plane;
		b.length = 1;
		b.index = i;
		ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
		if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
			return -errno;	

	}

	
	
			/* start processing */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fimc0_fd, VIDIOC_STREAMON, &type);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_STREAMON: %s\n", ERRSTR))
		return -errno;

	type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fimc0_fd, VIDIOC_STREAMON, &type);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_STREAMON: %s\n", ERRSTR))
		return -errno;
	printf("%s -\n", __func__);
	return 0;
}
int open_lcd_device()
{
	int fd;
	int err;
	int ret;
	if((fd = open("/dev/fb0", O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("Fail to open");
		exit(EXIT_FAILURE);
	} 
	//printf("open lcd success %d\n",fd);

	if(-1 == ioctl(fd, FBIOGET_FSCREENINFO,&finfo))
	{
		perror("Fail to ioctl:FBIOGET_FSCREENINFO\n");
		exit(EXIT_FAILURE);
	}
	if (-1==ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)) 
	{
		perror("Fail to ioctl:FBIOGET_VSCREENINFO\n");
		exit(EXIT_FAILURE);
	}
    lcd_buf_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;	
	printf("vinfo.xres:%d, vinfo.yres:%d, vinfo.bits_per_pixel:%d, lcd_buf_size:%d, finfo.line_length:%d\n",vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, lcd_buf_size, finfo.line_length); 


	lcd_fd = fd;
	
	vinfo.activate = FB_ACTIVATE_FORCE;
	vinfo.yres_virtual = vinfo.yres;
	ret = ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo );
	if( ret < 0 )
	{
		printf( "ioctl FBIOPUT_VSCREENINFO failed\n");
		return -1;
	}
	
    //mmap framebuffer    
    fb_buf = (char *)mmap(
	    NULL,
	    lcd_buf_size,
	    PROT_READ | PROT_WRITE,MAP_SHARED ,
	    lcd_fd, 
	    0);    
	if(NULL == fb_buf)
	{
		perror("Fail to mmap fb_buf");
		exit(EXIT_FAILURE);
	}
	ret = ioctl( lcd_fd, FBIOBLANK, FB_BLANK_UNBLANK );
	if( ret < 0 )
	{
			printf( "ioctl FBIOBLANK failed\n");
			return -1;
	}
	
	return fd;
}
int receiveBegin()
{
	 
	   sem_init(&lcd_sem, 0, 0);
	   fimc0_fd=open("/dev/video0",O_RDWR );
	   if (fimc0_fd<0)
	   printf("err to open fimc\n");
	   initfimc();
	   start_capturing();
     open_lcd_device();
	   openHandle=SsbSipMfcDecOpen();
	   if(!openHandle) 
     fprintf(stderr,"Error: SsbSipMfcDecOpen.\n");
     printf("SsbSipMfcDecOpen succeeded.\n");
     virInBuf = SsbSipMfcDecGetInBuf(openHandle, &phyInBuf, MAX_DECODER_INPUT_BUFFER_SIZE);
     memcpy(virInBuf,format,21); 
     err=SsbSipMfcDecInit(openHandle,H264_DEC,21);	
      if(err<0) 
      	{
           fprintf(stderr,"Error: SsbSipMfcDecInit. Code %d\n",err);
        }
    //WSADATA dat;
    //WSAStartup(MAKEWORD(2,2),&dat);
    //fpo=fopen("test.nv12","wba+");
    //pthread_create(&receive_tid,NULL,receive_thread,(void *)NULL);  
	  //pthread_create(&display_tid,NULL,display_thread,(void *)NULL); 
     StartReceive();
  /*  while(1)
	{
		sleep(10);
	}
    return 0;*/
}

 int read_frame(unsigned char **buffer)
{
	int index = 0;
	*buffer = temp_buf;
	return fimc0_cap_buf_length;
}

void convert_rgbLCD_to_rgb24_buffer(unsigned char* rgbLCD, unsigned char* rgb, int width, int height)
{
	for(int i=0;i<width*height;i++)
	{
				*rgb = *(rgbLCD+2);
				*(rgb+1)= *(rgbLCD+1);
				*(rgb+2) = *rgbLCD;
				rgb =rgb + 3;
				rgbLCD = rgbLCD +4;
	}
}