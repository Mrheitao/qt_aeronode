#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <sys/procfs.h>
#include <pthread.h>
#include <poll.h>
#include <semaphore.h>
#include <dlfcn.h>
#include <sys/ioctl.h>
#include "mfc_interface.h"
using namespace std;

#ifndef RTPSENDMYLIB_H
#define RTPSENDMYLIB_H
#include "rtpsendmylib.h"
#endif

#define MAX_STREAM_SIZE (2*1024*1024)
#define POLL_ENC_WAIT_TIMEOUT 25
#define TimeOut 5 
#define CapNum 10

#define CapWidth 640
#define CapHeight 480

#define ReqButNum 3

#define IsRearCamera 0

#define  FPS 20

#define PIXELFMT V4L2_PIX_FMT_YUYV

#define CapDelay 100*1000

// #define SOFILE "mylib.so"


#define CLEAR(x)    memset(&(x), 0, sizeof(x))

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define CLAMP(x,l,h) ((x) < (l) ? (l) : ((x) > (h) ? (h) : (x)))
#define ERRSTR strerror(errno)

#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define ERR(...) __info("Error", __FILE__, __LINE__, __VA_ARGS__)
#define ERR_ON(cond, ...) ((cond) ? ERR(__VA_ARGS__) : 0)

#define CRIT(...) \
	do { \
		__info("Critical", __FILE__, __LINE__, __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while(0)
#define CRIT_ON(cond, ...) do { if (cond) CRIT(__VA_ARGS__); } while(0)


typedef struct
{
	void *start;
	int length;
	int bytesused;
}BUFTYPE;

extern int processRunning;

extern FILE *fp_nv12;
extern FILE *fp_h264;
extern char lcd_path[];
extern char fimc0_path[];
extern char cam_path[];
extern char mfc_path[];

//extern static sem_t lcd_sem;
//extern struct send_dev_buffers *bufs;
extern SSBSIP_MFC_ENC_H264_PARAM *param;
extern SSBSIP_MFC_ENC_INPUT_INFO iinfo;
extern SSBSIP_MFC_ENC_OUTPUT_INFO oinfo;
extern SSBSIP_MFC_ERROR_CODE err;
extern  _MFCLIB *pCTX;
extern BUFTYPE *fimc0_out_buf;
extern BUFTYPE *cam_buffers;
extern BUFTYPE *fimc0_cap_buf;
//extern static int n_buffer;
extern int frmcnt;
extern void *fimc_in;
extern void *fimc_out;

extern void *handle;

extern int fimc0_out_buf_length;
extern int fimc0_cap_buf_length;
//void *fimc0_out[16];
//void *fimc0_cap[16];

//extern static struct fb_var_screeninfo send_vinfo;
//extern static struct fb_fix_screeninfo finfo;
//extern static int lcd_buf_size;
/* extern static char *fb_buf;
extern static int tsp_fd;
extern static int fimc0_fd;
extern static pthread_t save_tid; */
//static pthread_t display_tiddisplay_tid; 
//extern static pthread_t rtp_tid;
//extern static pthread_mutex_t mut;
extern int lcd_fd;
extern int cam_fd;
extern int display_x;
extern int display_y;
//extern static int fimc0_cap_index;
extern unsigned char *temp_buf;

struct send_dev_buffers {
	int count; /* number of buffers */
	int nplanes; /* number of planes per buffer */
	int *lengths; /* array of plane lengths */
	 /* array of plane addresses, address of plane p in buffer b
	    is at addr[nplanes * b + p] */
	char **addr;
	 /* array of bytes used by plane, bytesused of plane p in buffer b
	    is at bytesused[nplanes * b + p] */
	int *bytesused;
};

extern struct send_dev_buffers *bufs;

int sendBegin();
long read_frame(unsigned char **);
int convert_yuv_to_rgb_buffer(unsigned char *, unsigned char *, unsigned int , unsigned int );
int stop_dev();
void endOfSend();
void parametersSetting(string, string, string);
//void setFormat();