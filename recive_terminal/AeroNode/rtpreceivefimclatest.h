/*
   This IPv4 example uses the background thread itself to process all packets.
   You can use example one to send data to the session that's created in this
   example.
*/
#include "mfc_interface.h"


//#include "color_space_convertor.h"
#include "rtpsession.h"
#include "rtppacket.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include "rtpsourcedata.h"
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#define  BUFFER_SIZE   120000
#define  RTP_HEADLEN 12 
#define ERRSTR strerror(errno)
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define ERR(...) __info("Error", __FILE__, __LINE__, __VA_ARGS__)
#define ERR_ON(cond, ...) ((cond) ? ERR(__VA_ARGS__) : 0)
#define CLEAR(x)    memset(&(x), 0, sizeof(x))	
#define ReqButNum 1

typedef struct
{
	void *start;
	int length;
	int bytesused;
}BUFTYPE;

extern BUFTYPE *fimc0_out_buf;

extern int fimc0_fd;
//extern static int n_buffer;
extern void *fimc0_cap[16];
extern unsigned char * temp_buf;

extern long fimc0_cap_buf_length;
extern struct pollfd fds;
extern char format[21];
extern int idx;

//extern static struct fb_var_screeninfo vinfo;
//extern static struct fb_fix_screeninfo finfo;
extern int lcd_fd;
extern int lcd_buf_size;
extern char *fb_buf;

int receiveBegin();
int read_frame(unsigned char **buffer);
void convert_rgbLCD_to_rgb24_buffer(unsigned char* rgbLCD, unsigned char* rgb, int width, int height);