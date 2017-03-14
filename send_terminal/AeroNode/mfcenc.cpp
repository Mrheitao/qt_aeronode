#include "mfcenc.h"

int processRunning = 0; //用来控制进程停止，processRunning == 1时，进程正在运行。

FILE *fp_nv12;
FILE *fp_h264;
char lcd_path[] = "/dev/fb0";
char fimc0_path[] = "/dev/video0";
char cam_path[] = "/dev/video15";
char mfc_path[]="/dev/video9";
static int first_click_button = 0;
static sem_t lcd_sem;
struct send_dev_buffers *bufs = NULL;
SSBSIP_MFC_ENC_H264_PARAM *param;
SSBSIP_MFC_ENC_INPUT_INFO iinfo;
SSBSIP_MFC_ENC_OUTPUT_INFO oinfo;
SSBSIP_MFC_ERROR_CODE err;
 _MFCLIB *pCTX;
BUFTYPE *fimc0_out_buf;
BUFTYPE *cam_buffers;
BUFTYPE *fimc0_cap_buf;
static int n_buffer = 0;
int frmcnt = 0;
void *fimc_in = NULL;
void *fimc_out = NULL;

int fimc0_out_buf_length;
int fimc0_cap_buf_length;
//void *fimc0_out[16];
//void *fimc0_cap[16];

static struct fb_var_screeninfo send_vinfo;
static struct fb_fix_screeninfo finfo;
static int lcd_buf_size;
static char *fb_buf = NULL;
static int tsp_fd;
static int fimc0_fd;
static pthread_t save_tid;
//static pthread_t display_tiddisplay_tid; 
static pthread_t rtp_tid;
static pthread_mutex_t mut;
int lcd_fd;
int cam_fd;
int display_x = 0;
int display_y = 0;
static int fimc0_cap_index = 0;
unsigned char *temp_buf=NULL;
static int align(int x, int a)
{
	return ((x + a - 1) / a) * a;
}

int display_format(int pixelformat)
{
			printf("{pixelformat = %c%c%c%c}\n",
				pixelformat & 0xff,(pixelformat >> 8)&0xff,
				(pixelformat >> 16) & 0xff,(pixelformat >> 24)&0xff
				);
}
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


struct format {
	unsigned long fourcc;
	unsigned long width;
	unsigned long height;
};
void dump_format(char *str, struct v4l2_format *fmt)
{
	if (V4L2_TYPE_IS_MULTIPLANAR(fmt->type)) {
		struct v4l2_pix_format_mplane *pix = &fmt->fmt.pix_mp;
		LOG("%s: width=%u height=%u format=%.4s bpl=%u\n", str,
			pix->width, pix->height, (char*)&pix->pixelformat,
			pix->plane_fmt[0].bytesperline);
	} else {
		struct v4l2_pix_format *pix = &fmt->fmt.pix;
		LOG("%s: width=%u height=%u format=%.4s bpl=%u\n", str,
			pix->width, pix->height, (char*)&pix->pixelformat,
			pix->bytesperline);
	}
}
int open_camera_device(void)
{
	int fd;
  int hMFCopen;
  pCTX=(_MFCLIB*)malloc(sizeof(_MFCLIB));
  if (pCTX == NULL) 
  	{
        printf("[%s] malloc failed.",__func__);
        return 0;
    }
	if((fd = open(cam_path,O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("Fail to open");
		exit(EXIT_FAILURE);
	} 
	cam_fd = fd;
	if((fimc0_fd = open(fimc0_path,O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("Fail to open");
		exit(EXIT_FAILURE);
	} 
	if((hMFCopen = open(mfc_path, O_RDWR | O_NONBLOCK))<0)
		{
			perror("Fail to open");
		  exit(EXIT_FAILURE);
		}
  pCTX->hMFC = hMFCopen;
	printf("open cam success %d\n",hMFCopen);
	return fd;
}

//′ò?aé???í·éè±?
/* int open_lcd_device(void)
{
	int fd;
	int err;
	int ret;
	if((fd = open(lcd_path, O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("Fail to open");
		exit(EXIT_FAILURE);
	} 
	printf("open lcd success %d\n",fd);

	if(-1 == ioctl(fd, FBIOGET_FSCREENINFO,&finfo))
	{
		perror("Fail to ioctl:FBIOGET_FSCREENINFO\n");
		exit(EXIT_FAILURE);
	}
	if (-1==ioctl(fd, FBIOGET_VSCREENINFO, &send_vinfo)) 
	{
		perror("Fail to ioctl:FBIOGET_VSCREENINFO\n");
		exit(EXIT_FAILURE);
	}
    lcd_buf_size = send_vinfo.xres * send_vinfo.yres * send_vinfo.bits_per_pixel / 8;	
	printf("send_vinfo.xres:%d, send_vinfo.yres:%d, send_vinfo.bits_per_pixel:%d, lcd_buf_size:%d, finfo.line_length:%d\n",send_vinfo.xres, send_vinfo.yres, send_vinfo.bits_per_pixel, lcd_buf_size, finfo.line_length); 


	lcd_fd = fd;
	
	send_vinfo.activate = FB_ACTIVATE_FORCE;
	send_vinfo.yres_virtual = send_vinfo.yres;
	ret = ioctl(fd, FBIOPUT_VSCREENINFO, &send_vinfo );
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
	ret = ioctl( lcd_fd, FBIOBLANK, FB_BLANK_UNBLANK );//清屏
	if( ret < 0 )
	{
			printf( "ioctl FBIOBLANK failed\n");
			return -1;
	}
	
	return fd;
} */

int fb_wait_for_vsync(int lcd_fd)
{
	int ret;
	unsigned long temp;

	ret = ioctl(lcd_fd, FBIO_WAITFORVSYNC, &temp);
	if (ret < 0) {
		perror("Wait for vsync failed");
		return -1;
	}
	return 0;
}
int cam_reqbufs(void)
{
	struct v4l2_requestbuffers req;
	int i;
	printf("%s: +\n", __func__);
	int n_buffers = 0;
	CLEAR(req);

	req.count  = ReqButNum;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
//	req.memory = V4L2_MEMORY_USERPTR;

	if (-1 == ioctl(cam_fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support "
				 "user pointer i/o\n", "campture");
			exit(EXIT_FAILURE);
		} else {
			printf("VIDIOC_REQBUFS");
			exit(EXIT_FAILURE);
		}
	}

	cam_buffers = (BUFTYPE*)calloc(ReqButNum, sizeof(*cam_buffers));

	if (!cam_buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < ReqButNum; ++i) {
	
		struct v4l2_buffer buf;
		
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(-1 == ioctl(cam_fd, VIDIOC_QUERYBUF, &buf))
		{
			perror("Fail to ioctl : VIDIOC_QUERYBUF");
			exit(EXIT_FAILURE);
		}

		cam_buffers[i].length = buf.length;
		cam_buffers[i].start = 
			mmap(
					NULL,/*start anywhere*/
					buf.length,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					cam_fd,buf.m.offset
				);
		
		if(MAP_FAILED == cam_buffers[i].start)
		{
			perror("Fail to mmap\n");
			printf("%d\n",i);
			exit(EXIT_FAILURE);
		}
		printf("cam_capture rebuf::08%lx\n", cam_buffers[i].start);
	}	

	printf("%s: -\n", __func__);
}

int fimc0_reqbufs(void)
{
	int i=0,n=0;
	int err;
	int ret;
	struct v4l2_control ctrl;
	//struct v4l2_requestbuffers reqbuf;
	struct v4l2_requestbuffers rb;
	struct v4l2_plane plane[2];
	CLEAR(rb);
	/* enqueue the dmabuf to vivi */
	struct v4l2_buffer b;
	CLEAR(b);


	printf("%s: +\n", __func__);
		/* request buffers for FIMC0 */
	rb.count = ReqButNum;
	rb.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	rb.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fimc0_fd, VIDIOC_REQBUFS, &rb);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_REQBUFS: %s\n", ERRSTR))
		return -errno;
	printf("fimc0 output_buf_num:%d\n",rb.count);

	n_buffer = rb.count;

	fimc0_out_buf = (BUFTYPE*)calloc(rb.count,sizeof(BUFTYPE));
	if(fimc0_out_buf == NULL){
		fprintf(stderr,"Out of memory\n");
		exit(EXIT_FAILURE);
	}
	printf("%s, fimc0_out_buf request successfully\n",__func__);
	
		/* mmap DMABUF */


	for (n = 0; n < ReqButNum; ++n) {
		b.index = n;
		b.length = 1;
		b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		b.memory = V4L2_MEMORY_MMAP;
		b.m.planes = plane;
		ret = ioctl(fimc0_fd, VIDIOC_QUERYBUF, &b);
		
		if (ERR_ON(ret < 0, "fimc0: VIDIOC_REQBUFS: %s\n", ERRSTR))
		exit(EXIT_FAILURE);
		
		//	printf("fimc0 querybuf:%d,%d\n", b.m.planes[0].length, b.m.planes[0].m.mem_offset);
			fimc0_out_buf[n].start = mmap(NULL,
						b.m.planes->length,
						PROT_READ | PROT_WRITE,
						MAP_SHARED, fimc0_fd,
						b.m.planes->m.mem_offset);


		
		//	fimc0_out_buf[n].start = fimc0_out[n];
			fimc0_out_buf[n].length = b.m.planes->length;
				if (fimc0_out_buf[n].start == MAP_FAILED) {
				printf("Failed mmap buffer %d for %d\n", n,
							fimc0_fd);
				return -1;
			}

		fimc0_out_buf_length = b.m.planes->length;
		printf("fimc0 querybuf:0x%08lx,%d,%d\n", fimc0_out_buf[n], fimc0_out_buf_length, b.m.planes->m.mem_offset);
		
	//	printf("fimc0 output:plane.length:%d\n",fimc0_out_buf_length);
	}
		/*fimc0_cap_buf = calloc(ReqButNum, sizeof(BUFTYPE));
		if (ERR_ON(fimc0_cap_buf == NULL, "fimc0_cap_buf: VIDIOC_QUERYBUF: %s\n", ERRSTR))
			return -errno;*/

  bufs = (struct send_dev_buffers*)malloc(sizeof(struct send_dev_buffers));
	CLEAR(*plane);
	CLEAR(b);
	CLEAR(rb);
	
	rb.count = ReqButNum;
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	rb.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fimc0_fd, VIDIOC_REQBUFS, &rb);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_REQBUFS: %s\n", ERRSTR))
		return -errno;

	
	for (n = 0; n <  ReqButNum; ++n) {    
		
		
		  b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
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
			if (n == 0) {
			 for (i = 0; i <b.length; ++i)
				if (b.m.planes[i].length == 0)
					  break;
					 
			bufs->nplanes = i;
			printf("the planes is %d",bufs->nplanes);
			bufs->lengths = (int*)malloc(bufs->nplanes
						* sizeof(*bufs->lengths));
			for (i = 0; i < bufs->nplanes; ++i)
				{bufs->lengths[i] = b.m.planes[i].length;
				printf("the plane[%d]is %d",i,bufs->lengths[i]);
}
			bufs->bytesused = (int*)malloc(ReqButNum * bufs->nplanes
						* sizeof(*bufs->bytesused));
			bufs->addr = (char**)malloc(ReqButNum * bufs->nplanes
						* sizeof(*bufs->addr));
		}
			for (i = 0; i < bufs->nplanes; ++i) {
			bufs->addr[n * bufs->nplanes + i] = (char*)mmap(NULL,
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
			       
		
	//	printf("fimc0 capture:plane.length:%d\n",fimc0_cap_buf_length);	
	printf("%s -\n", __func__);
	return 0;
}
int cam_setfmt(void)
{
	int err;
	int ret;
	struct v4l2_fmtdesc fmt;
	struct v4l2_capability cap;
	struct v4l2_format stream_fmt;
	struct v4l2_input input;
	struct v4l2_control ctrl;
	struct v4l2_streamparm stream;
	
	memset(&fmt,0,sizeof(fmt));
	fmt.index = 0;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	while((ret = ioctl(cam_fd,VIDIOC_ENUM_FMT,&fmt)) == 0)
	{
		fmt.index ++ ;
		printf("{pixelformat = %c%c%c%c},description = '%s'\n",
				fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
				(fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
				fmt.description);
	}
	ret = ioctl(cam_fd,VIDIOC_QUERYCAP,&cap);
	if(ret < 0){
		perror("FAIL to ioctl VIDIOC_QUERYCAP");
		exit(EXIT_FAILURE);
	}
	printf("capabilities:%08lx\n",cap.capabilities);

	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))//0x00000001
	{
		printf("The Current device is not a video capture device\n");
		exit(EXIT_FAILURE);
	
	}

	if(!(cap.capabilities & V4L2_CAP_STREAMING))//0x04000000
	{
		printf("The Current device does not support streaming i/o\n");
		exit(EXIT_FAILURE);
	}

	CLEAR(stream_fmt);
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	stream_fmt.fmt.pix.width = CapWidth;  //640
	stream_fmt.fmt.pix.height = CapHeight;    //480
	stream_fmt.fmt.pix.pixelformat = PIXELFMT;
	stream_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if(-1 == ioctl(cam_fd,VIDIOC_S_FMT,&stream_fmt))
	{
		printf("Can't set the fmt\n");
		perror("Fail to ioctl\n");
		exit(EXIT_FAILURE);
	}
	printf("VIDIOC_S_FMT successfully\n");
	
	printf("%s: -\n", __func__);
	return 0;
}
int cam_setrate(void)
{
	int err;
	int ret;

	struct v4l2_streamparm stream;

	CLEAR(stream);
    stream.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stream.parm.capture.capturemode = 0;
    stream.parm.capture.timeperframe.numerator = 1;
    stream.parm.capture.timeperframe.denominator = 15;

    err = ioctl(cam_fd, VIDIOC_S_PARM, &stream);
	if(err < 0)
    printf("FimcV4l2 start: error %d, VIDIOC_S_PARM", err);
	err = ioctl(cam_fd, VIDIOC_S_PARM, &stream);
	if(err < 0)
    printf("FimcV4l2 start: error %d, VIDIOC_S_PARM", err);
	return 0;

}
int fimc0_setfmt(void)
{
	int err;
	int ret;
	struct v4l2_fmtdesc fmt;
	struct v4l2_capability cap;
	struct v4l2_format stream_fmt;
	struct v4l2_input input;
	struct v4l2_control ctrl;
	struct v4l2_streamparm stream;

	printf("%s: +\n", __func__);
	CLEAR(stream_fmt);
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	stream_fmt.fmt.pix.width = CapWidth;
	stream_fmt.fmt.pix.height = CapHeight;
	stream_fmt.fmt.pix.pixelformat = PIXELFMT;
	stream_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
		/* get format from VIVI */
	ret = ioctl(cam_fd, VIDIOC_G_FMT, &stream_fmt);
	if (ERR_ON(ret < 0, "vivi: VIDIOC_G_FMT: %s\n", ERRSTR))
		return -errno;
	dump_format("cam_fd-capture", &stream_fmt);


		/* setup format for FIMC 0 */
	/* keep copy of format for to-mplane conversion */
	
	struct v4l2_pix_format pix = stream_fmt.fmt.pix;

	CLEAR(stream_fmt);
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	struct v4l2_pix_format_mplane *pix_mp =&stream_fmt.fmt.pix_mp;
  
	pix_mp->width = pix.width;
	pix_mp->height = pix.height;
	pix_mp->pixelformat = pix.pixelformat;
	pix_mp->num_planes = 1;
	pix_mp->plane_fmt[0].bytesperline = pix.bytesperline;

	dump_format("fimc0-output", &stream_fmt);
	ret = ioctl(fimc0_fd, VIDIOC_S_FMT, &stream_fmt);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_S_FMT: %s\n", ERRSTR))
		return -errno;
	dump_format("fimc0-output", &stream_fmt);
	
	
	
	  CLEAR(stream_fmt);
		/* set format on fimc0 capture */
	  stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	/* try cmdline format, or use fimc0-output instead */

		struct v4l2_pix_format_mplane *pix_mp_f = &stream_fmt.fmt.pix_mp;
	  pix_mp_f->pixelformat = V4L2_PIX_FMT_NV12M;
	  pix_mp_f->width =CapWidth;
	  pix_mp_f->height =CapHeight;
		
		pix_mp_f->num_planes = 2;
		pix_mp_f->plane_fmt[0].bytesperline = align(CapWidth, 128);
		pix_mp_f->plane_fmt[0].sizeimage = align(CapWidth * CapHeight, 2048);
		pix_mp_f->plane_fmt[1].bytesperline = align(CapWidth, 128);
		pix_mp_f->plane_fmt[1].sizeimage = align(CapWidth * (CapHeight / 2),2048);
		


	//dump_format("pre-fimc0-capture", &stream_fmt);
	ret = ioctl(fimc0_fd, VIDIOC_S_FMT, &stream_fmt);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_S_FMT: %s\n", ERRSTR))
		return -errno;

	printf("%s -\n", __func__);

}
void* mfc_setfmt(void)
{  
	struct v4l2_capability cap;
	enum v4l2_buf_type type;
  struct v4l2_format fmt;
  struct v4l2_plane planes[MFC_ENC_NUM_PLANES];
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers reqbuf;
  struct v4l2_control ctrl;
  struct pollfd poll_events;
  int poll_state;
  struct v4l2_ext_control ext_ctrl_mpeg4[23];
  struct v4l2_ext_control ext_ctrl_h263[17];
  struct v4l2_ext_control ext_ctrl[38];
  struct v4l2_ext_controls ext_ctrls;
  int pad_value = 0;
  SSBSIP_MFC_ENC_H264_PARAM *h264_arg;
	int ret,i, j,index;   
	
	
	printf("mfc_setfmt init. \n");
	fflush(stdout);
    memset(&cap, 0, sizeof(cap));
	
	printf("mfc_setfmt memset. \n");
	fflush(stdout);
    ret = ioctl(pCTX->hMFC, VIDIOC_QUERYCAP, &cap);
	
	printf("mfc_setfmt ioctl. \n");
	fflush(stdout);
	if (ret != 0)
		{
        printf("[%s] VIDIOC_QUERYCAP failed",__func__);
        close(pCTX->hMFC);
        free(pCTX);
        return NULL;
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) 
    {
        printf("[%s] Device does not support capture",__func__);
        close(pCTX->hMFC);
        free(pCTX);
        return NULL;
    }

	
	printf("mfc_setfmt 2 if. \n");
	fflush(stdout);
    if (!(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT_MPLANE)) 
    {
        printf("[%s] Device does not support output",__func__);
        close(pCTX->hMFC);
        free(pCTX);
        return NULL;
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        printf("[%s] Device does not support streaming",__func__);
        close(pCTX->hMFC);
        free(pCTX);
        return NULL;
    }
	 printf("pCTX 1\n");
	  pCTX->v4l2_enc.bRunning = 0;
    /* physical address is used for Input source */
    pCTX->v4l2_enc.bInputPhyVir = 0;
    pCTX->cacheablebuffer = NO_CACHE;
    param= (SSBSIP_MFC_ENC_H264_PARAM *)malloc(sizeof(*param));
    if(param==NULL) 
    {
       fprintf(stderr,"Error: malloc param\n");
       return NULL;
    }
    memset(param,0,sizeof(*param));
       param->codecType = H264_ENC;
	     param->SourceWidth  = 640;
       param->SourceHeight = 480;
       param->IDRPeriod    = 1;
       param->SliceMode    = 0;
       param->RandomIntraMBRefresh = 0;
       param->Bitrate      = 128000;//64000; //pSECOutputPort->portDefinition.format.video.nBitrate;
       param->QSCodeMax    = 51;
       param->QSCodeMin    = 10;
       param->PadControlOn = 0;             // 0: disable, 1: enable
       param->LumaPadVal   = 0;
       param->CbPadVal     = 0;
       param->CrPadVal     = 0;

       param->ProfileIDC   = 0;//OMXAVCProfileToProfileIDC(pH264Enc->AVCComponent[OUTPUT_PORT_INDEX].eProfile); //0;  //(OMX_VIDEO_AVCProfileMain)
       param->LevelIDC     = 11;//OMXAVCLevelToLevelIDC(pH264Enc->AVCComponent[OUTPUT_PORT_INDEX].eLevel);       //40; //(OMX_VIDEO_AVCLevel4)
       param->FrameRate    = 15; //pMfcH264Enc->videoIn.FrameRate;  //15
       param->SliceArgument = 0;          // Slice mb/byte size number
       param->NumberBFrames = 0;            // 0 ~ 2
       param->NumberReferenceFrames = 1;
       param->NumberRefForPframes   = 1;
       param->LoopFilterDisable     = 1;    // 1: Loop Filter Disable, 0: Filter Enable
       param->LoopFilterAlphaC0Offset = 0;
       param->LoopFilterBetaOffset    = 0;
       param->SymbolMode       = 0;         // 0: CAVLC, 1: CABAC
       param->PictureInterlace = 0;
       param->Transform8x8Mode = 1; //default:0;         // 0: 4x4, 1: allow 8x8
       param->DarkDisable     = 1;
       param->SmoothDisable   = 1;
       param->StaticDisable   = 1;
       param->ActivityDisable = 1;
       param->FrameQp      = 30;//30; //0;//pVideoEnc->quantization.nQpI;
       param->FrameQp_P    = 20;//pVideoEnc->quantization.nQpP;
       param->FrameQp_B    = 20;//pVideoEnc->quantization.nQpB;
       param->EnableFRMRateControl = 0;        // 0: Disable, 1: Frame level RC
       param->EnableMBRateControl  = 0;        // 0: Disable, 1:MB level RC
       param->CBRPeriodRf  = 100;
       param->FrameMap = NV12_LINEAR;
       
    h264_arg = (SSBSIP_MFC_ENC_H264_PARAM*)param;
    pCTX->codecType = H264_ENC;
    pCTX->width = h264_arg->SourceWidth;
    pCTX->height = h264_arg->SourceHeight;
    pCTX->framemap = (SSBSIP_MFC_INSTRM_MODE_TYPE)h264_arg->FrameMap;
    ext_ctrl[0].id = V4L2_CID_MPEG_VIDEO_H264_PROFILE;
    ext_ctrl[0].value = h264_arg->ProfileIDC;//0
    ext_ctrl[1].id = V4L2_CID_MPEG_VIDEO_H264_LEVEL;
    ext_ctrl[1].value = h264_arg->LevelIDC;
    ext_ctrl[2].id = V4L2_CID_MPEG_VIDEO_GOP_SIZE;
    ext_ctrl[2].value = h264_arg->IDRPeriod;
    ext_ctrl[3].id = V4L2_CID_MPEG_MFC51_VIDEO_H264_NUM_REF_PIC_FOR_P;
    ext_ctrl[3].value = h264_arg->NumberRefForPframes;
    ext_ctrl[4].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE;
    ext_ctrl[4].value = h264_arg->SliceMode;  /* 0: one, 1: fixed #mb, 3: fixed #bytes */
    if (h264_arg->SliceMode == 0){
            ext_ctrl[5].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_MB;
            ext_ctrl[5].value = 1;  /* default */
            ext_ctrl[6].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES;
            ext_ctrl[6].value = 1900; /* default */
        } else if (h264_arg->SliceMode == 1) {
            ext_ctrl[5].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_MB;
            ext_ctrl[5].value = h264_arg->SliceArgument;
            ext_ctrl[6].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES;
            ext_ctrl[6].value = 1900; /* default */
        } else if (h264_arg->SliceMode == 3) {
            ext_ctrl[5].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_MB;
            ext_ctrl[5].value = 1; /* default */
            ext_ctrl[6].id = V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES;
            ext_ctrl[6].value = h264_arg->SliceArgument;
        }
        /*
        It should be set using h264_arg->NumberBFrames after being handled by appl.
         */
        ext_ctrl[7].id =  V4L2_CID_MPEG_VIDEO_B_FRAMES;
        ext_ctrl[7].value = h264_arg->NumberBFrames;
        ext_ctrl[8].id = V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_MODE;
        ext_ctrl[8].value = h264_arg->LoopFilterDisable;
        ext_ctrl[9].id = V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_ALPHA;
        ext_ctrl[9].value = h264_arg->LoopFilterAlphaC0Offset;
        ext_ctrl[10].id = V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_BETA;
        ext_ctrl[10].value = h264_arg->LoopFilterBetaOffset;
        ext_ctrl[11].id = V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE;
        ext_ctrl[11].value = h264_arg->SymbolMode;

        ext_ctrl[12].id = V4L2_CID_MPEG_VIDEO_H264_8X8_TRANSFORM;
        ext_ctrl[12].value = h264_arg->Transform8x8Mode;
        ext_ctrl[13].id = V4L2_CID_MPEG_VIDEO_CYCLIC_INTRA_REFRESH_MB;
        ext_ctrl[13].value = h264_arg->RandomIntraMBRefresh;

        ext_ctrl[14].id = V4L2_CID_MPEG_MFC51_VIDEO_PADDING;
        ext_ctrl[14].value = h264_arg->PadControlOn;

	      pad_value |= h264_arg->CrPadVal;
        pad_value |= h264_arg->CbPadVal << 8;
        pad_value |= h264_arg->LumaPadVal << 16;
        ext_ctrl[15].id = V4L2_CID_MPEG_MFC51_VIDEO_PADDING_YUV;
        ext_ctrl[15].value = pad_value;

        ext_ctrl[16].id = V4L2_CID_MPEG_VIDEO_FRAME_RC_ENABLE;
        ext_ctrl[16].value = h264_arg->EnableFRMRateControl;
        ext_ctrl[17].id = V4L2_CID_MPEG_VIDEO_MB_RC_ENABLE;
        ext_ctrl[17].value = h264_arg->EnableMBRateControl;

        ext_ctrl[18].id = V4L2_CID_MPEG_VIDEO_BITRATE;
        /* FIXME temporary fix */
        if (h264_arg->Bitrate)
            ext_ctrl[18].value = h264_arg->Bitrate;
        else
            ext_ctrl[18].value = 1; /* just for testing Movi studio */

        ext_ctrl[19].id = V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP;
        ext_ctrl[19].value = h264_arg->FrameQp;
        ext_ctrl[20].id =  V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP;
        ext_ctrl[20].value = h264_arg->FrameQp_P;
        ext_ctrl[21].id =  V4L2_CID_MPEG_VIDEO_H264_B_FRAME_QP;
        ext_ctrl[21].value = h264_arg->FrameQp_B;
        ext_ctrl[22].id =  V4L2_CID_MPEG_VIDEO_H264_MAX_QP;
        ext_ctrl[22].value = h264_arg->QSCodeMax;
        ext_ctrl[23].id = V4L2_CID_MPEG_VIDEO_H264_MIN_QP;
        ext_ctrl[23].value = h264_arg->QSCodeMin;
        ext_ctrl[24].id = V4L2_CID_MPEG_MFC51_VIDEO_RC_REACTION_COEFF;
        ext_ctrl[24].value = h264_arg->CBRPeriodRf;
        ext_ctrl[25].id =  V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_DARK;
        ext_ctrl[25].value = h264_arg->DarkDisable;
        ext_ctrl[26].id =  V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_SMOOTH;
        ext_ctrl[26].value = h264_arg->SmoothDisable;
        ext_ctrl[27].id =  V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_STATIC;
        ext_ctrl[27].value = h264_arg->StaticDisable;
        ext_ctrl[28].id =  V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_ACTIVITY;
        ext_ctrl[28].value = h264_arg->ActivityDisable;

        /* doesn't have to be set */
        ext_ctrl[29].id = V4L2_CID_MPEG_VIDEO_GOP_CLOSURE;
        ext_ctrl[29].value = 0;
        ext_ctrl[30].id = V4L2_CID_MPEG_VIDEO_H264_I_PERIOD;
        ext_ctrl[30].value = 10;

        if (V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_LEVEL_LIMIT == pCTX->enc_frameskip) {
            ext_ctrl[31].id = V4L2_CID_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE;
            ext_ctrl[31].value = V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_LEVEL_LIMIT;
        } else if(V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_BUF_LIMIT == pCTX->enc_frameskip) {
            ext_ctrl[31].id = V4L2_CID_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE;
            ext_ctrl[31].value = V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_BUF_LIMIT;
        } else { /* ENC_FRAME_SKIP_MODE_DISABLE (default) */
            ext_ctrl[31].id = V4L2_CID_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE;
            ext_ctrl[31].value = V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_DISABLED;
        }

        ext_ctrl[32].id = V4L2_CID_MPEG_VIDEO_HEADER_MODE;
        ext_ctrl[32].value = 0; /* 0: seperated header
                                              1: header + first frame */

        ext_ctrl[33].id = V4L2_CID_MPEG_MFC51_VIDEO_RC_FIXED_TARGET_BIT;
        ext_ctrl[33].value = 1;

        ext_ctrl[34].id =  V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_ENABLE;
        ext_ctrl[34].value = 0;
        ext_ctrl[35].id = V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_IDC;
        ext_ctrl[35].value = 0;

        ext_ctrl[36].id = V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_WIDTH;
        ext_ctrl[36].value = 0;
        ext_ctrl[37].id =  V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_HEIGHT;
        ext_ctrl[37].value = 0;
        
      ext_ctrls.ctrl_class = V4L2_CTRL_CLASS_MPEG;
      ext_ctrls.count = 38;
      ext_ctrls.controls = ext_ctrl;
	   printf("pCTX 4\n");
    ret = ioctl(pCTX->hMFC, VIDIOC_S_EXT_CTRLS, &ext_ctrls);   
    if (ret != 0) {
     printf("[%s] Failed to set extended controls",__func__);
     ret = MFC_RET_ENC_INIT_FAIL;
     return NULL;
    }
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    fmt.fmt.pix_mp.width = pCTX->width;
    fmt.fmt.pix_mp.height = pCTX->height;
    fmt.fmt.pix_mp.num_planes = 2;
    fmt.fmt.pix_mp.plane_fmt[0].bytesperline = align(fmt.fmt.pix_mp.width, 128);
    fmt.fmt.pix_mp.plane_fmt[1].bytesperline = align(fmt.fmt.pix_mp.width, 128);
	   printf("pCTX 5\n");
    if (NV12_TILE == pCTX->framemap) {
        fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12MT; /* 4:2:0, 2 Planes, 64x32 Tiles */
        fmt.fmt.pix_mp.plane_fmt[0].sizeimage =align(align(fmt.fmt.pix_mp.width, 128) * align(fmt.fmt.pix_mp.height, 32), 8192); /* tiled mode */
        fmt.fmt.pix_mp.plane_fmt[1].sizeimage =align(align(fmt.fmt.pix_mp.width, 128) * align(fmt.fmt.pix_mp.height >> 1, 32), 8192); /* tiled mode */
    } else { /* NV12_LINEAR (default) */
        fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12M; /* 4:2:0, 2 Planes, linear */
        fmt.fmt.pix_mp.plane_fmt[0].sizeimage =align((fmt.fmt.pix_mp.width * fmt.fmt.pix_mp.height), 2048); /* linear mode, 2K align */
        fmt.fmt.pix_mp.plane_fmt[1].sizeimage =align((fmt.fmt.pix_mp.width * (fmt.fmt.pix_mp.height >> 1)), 2048); /* linear mode, 2K align */
    }
   ret = ioctl(pCTX->hMFC, VIDIOC_S_FMT, &fmt);
    if (ret != 0) {
        printf("[%s] S_FMT failed on MFC output stream",__func__);
        ret = MFC_RET_ENC_INIT_FAIL;
        return NULL;
     }
    memset(&fmt, 0, sizeof(fmt));
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_H264;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.plane_fmt[0].sizeimage = MAX_STREAM_SIZE; 
    ret = ioctl(pCTX->hMFC, VIDIOC_S_FMT, &fmt);
    if (ret != 0) {
        printf("[%s] S_FMT failed on MFC output stream",__func__);
        ret = MFC_RET_ENC_INIT_FAIL;
        return NULL;
      }
    reqbuf.count  = MFC_ENC_NUM_SRC_BUFS;
    reqbuf.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    if (pCTX->v4l2_enc.bInputPhyVir)
        reqbuf.memory = V4L2_MEMORY_USERPTR;
    else
        reqbuf.memory = V4L2_MEMORY_MMAP;//this one

    ret = ioctl(pCTX->hMFC, VIDIOC_REQBUFS, &reqbuf);
    if (ret != 0) {
        printf("[%s] Reqbufs src ioctl failed",__func__);
        ret = MFC_RET_ENC_INIT_FAIL;
        return NULL;
      }
   
   pCTX->v4l2_enc.mfc_num_src_bufs  = reqbuf.count;
    if (!pCTX->v4l2_enc.bInputPhyVir) {
        /* Then the buffers have to be queried and mmaped */
        for (i = 0;  i < pCTX->v4l2_enc.mfc_num_src_bufs; ++i) {
            memset(&buf, 0, sizeof(buf));
            buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;
            buf.m.planes = planes;
            buf.length = 2;

            ret = ioctl(pCTX->hMFC, VIDIOC_QUERYBUF, &buf);
            if (ret != 0) {
                printf("[%s] Querybuf src ioctl failed",__func__);
                ret = MFC_RET_ENC_INIT_FAIL;
                return NULL;
              }	
   	
   	    pCTX->v4l2_enc.mfc_src_bufs_len[0] = buf.m.planes[0].length;
        pCTX->v4l2_enc.mfc_src_bufs_len[1] = buf.m.planes[1].length;
        
        pCTX->v4l2_enc.mfc_src_bufs[i][0] =(char *)mmap(NULL, buf.m.planes[0].length, PROT_READ | PROT_WRITE,
                MAP_SHARED, pCTX->hMFC, buf.m.planes[0].m.mem_offset);
            if (pCTX->v4l2_enc.mfc_src_bufs[i][0] == MAP_FAILED) {
                printf("[%s] Mmap on src buffer (0) failed",__func__);
                ret = MFC_RET_ENC_INIT_FAIL;
                return NULL;
              }
              pCTX->v4l2_enc.mfc_src_bufs[i][1] =(char *)mmap(NULL, buf.m.planes[1].length, PROT_READ | PROT_WRITE,
                MAP_SHARED, pCTX->hMFC, buf.m.planes[1].m.mem_offset);
            if (pCTX->v4l2_enc.mfc_src_bufs[i][1] == MAP_FAILED) {
                munmap(pCTX->v4l2_enc.mfc_src_bufs[i][0], pCTX->v4l2_enc.mfc_src_bufs_len[0]);
                printf("[%s] Mmap on src buffer (1) failed",__func__);
                ret = MFC_RET_ENC_INIT_FAIL;
                return NULL;
              }
            }  
          } else
          printf("[%s] Camera Phys src buf %d",__func__,reqbuf.count);   
          for (i = 0; i<pCTX->v4l2_enc.mfc_num_src_bufs; i++)
           pCTX->v4l2_enc.mfc_src_buf_flags[i] = BUF_DEQUEUED;
           pCTX->sizeFrmBuf.luma = (unsigned int)(pCTX->width * pCTX->height);
           pCTX->sizeFrmBuf.chroma = (unsigned int)((pCTX->width * pCTX->height) >> 1);
           pCTX->inter_buff_status |= MFC_USE_YUV_BUFF;  
           
           
    memset(&reqbuf, 0, sizeof(reqbuf));
    reqbuf.count  = MFC_ENC_MAX_DST_BUFS;
	printf("pCTX 11\n");
    reqbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
	printf("pCTX 10\n");
    ret = ioctl(pCTX->hMFC, VIDIOC_REQBUFS, &reqbuf);
	printf("pCTX 20\n");
    if (ret != 0) {
        printf("[%s] Reqbufs dst ioctl failed",__func__);
        ret = MFC_RET_ENC_INIT_FAIL; 
        return NULL;
      }
     printf("pCTX 12\n");
     pCTX->v4l2_enc.mfc_num_dst_bufs   = reqbuf.count;
	printf("pCTX 13\n");
    for (i = 0; i<3; ++i) {
			printf("pCTX 14\n");
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
			printf("pCTX 15\n");
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.m.planes = planes;
        buf.length = 1;

        ret = ioctl(pCTX->hMFC, VIDIOC_QUERYBUF, &buf);
        if (ret != 0) {
            printf("[%s] Querybuf dst ioctl failed",__func__);
            ret = MFC_RET_ENC_INIT_FAIL;
            return NULL;
          }
        pCTX->v4l2_enc.mfc_dst_bufs_len = buf.m.planes[0].length;
        pCTX->v4l2_enc.mfc_dst_bufs[i] = (char *)mmap(NULL, buf.m.planes[0].length, PROT_READ | PROT_WRITE,
                MAP_SHARED, pCTX->hMFC, buf.m.planes[0].m.mem_offset);
        if (pCTX->v4l2_enc.mfc_dst_bufs[i] == MAP_FAILED) {
            printf("[%s] Mmap on dst buffer failed",__func__);
            ret = MFC_RET_ENC_INIT_FAIL;
            return NULL;
          }
          ret = ioctl(pCTX->hMFC, VIDIOC_QBUF, &buf);
        if (ret != 0) {
            printf("[%s] VIDIOC_QBUF failed, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE",__func__);
            ret = MFC_RET_ENC_INIT_FAIL;
            return NULL;
          }
        }
    pCTX->sizeStrmBuf = MAX_ENCODER_OUTPUT_BUFFER_SIZE;
    pCTX->inter_buff_status |= MFC_USE_STRM_BUFF;

   type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
   ret = ioctl(pCTX->hMFC, VIDIOC_STREAMON, &type);
    if (ret != 0) {
        printf("[%s] V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE, VIDIOC_STREAMON failed",__func__);
        ret = MFC_RET_ENC_INIT_FAIL; 
        return NULL;
      }
      
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.m.planes = planes;
    buf.length = 1;
    /* note: #define POLLOUT 0x0004 */
    poll_events.fd = pCTX->hMFC;
    poll_events.events = POLLIN | POLLERR;
    poll_events.revents = 0;  
     do {
        poll_state = poll((struct pollfd*)&poll_events, 1, POLL_ENC_WAIT_TIMEOUT);
        if (0 < poll_state) {
            if (poll_events.revents & POLLIN) { /* POLLIN */
                ret = ioctl(pCTX->hMFC, VIDIOC_DQBUF, &buf);
                if (ret == 0)
                    break;
            } else if(poll_events.revents & POLLERR) { /*POLLERR */
                printf("[%s] POLLERR\n",__func__);
                ret = MFC_RET_ENC_INIT_FAIL;
               
            } else {
                printf("[%s] poll() returns 0x%x\n",__func__, poll_events.revents);
                ret = MFC_RET_ENC_INIT_FAIL;
          
            }
        } else if(0 > poll_state) {
            ret = MFC_RET_ENC_INIT_FAIL;
            return NULL;
          }
        }while (0 == poll_state);
    pCTX->v4l2_enc.mfc_dst_bufs_bytes_used_len = buf.m.planes[0].bytesused;
    pCTX->virStrmBuf =(int) pCTX->v4l2_enc.mfc_dst_bufs[buf.index];
    index = buf.index;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;
    buf.m.planes = planes;
    buf.length = 1;

    ret = ioctl(pCTX->hMFC, VIDIOC_QBUF, &buf);
    if (ret != 0) {
        printf("[%s] VIDIOC_QBUF failed, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE",__func__);
        ret = MFC_RET_ENC_INIT_FAIL;
        return NULL;
      }
      printf("[%s] Strm out idx %d",__func__,index);
      
    return   pCTX;     
	}
	
SSBSIP_MFC_ERROR_CODE mfc_getoutbuf(SSBSIP_MFC_ENC_OUTPUT_INFO *output_info)
{
	if (pCTX->v4l2_enc.bRunning == 0) {
        pCTX->encodedHeaderSize = pCTX->v4l2_enc.mfc_dst_bufs_bytes_used_len;
        output_info->dataSize = 0;
    } else {
        output_info->dataSize = pCTX->v4l2_enc.mfc_dst_bufs_bytes_used_len;
    }
	printf("successed into mfc_getoutbuf 1 ");
    output_info->headerSize = pCTX->encodedHeaderSize;
    output_info->frameType = pCTX->encodedframeType;
    output_info->StrmPhyAddr = (void *)0;
    output_info->StrmVirAddr = (void *)pCTX->virStrmBuf;
    output_info->encodedYPhyAddr = (void*)0;
    output_info->encodedCPhyAddr = (void*)0;
	printf("successed into mfc_getoutbuf 2");
    return MFC_RET_OK;
	
	}

SSBSIP_MFC_ERROR_CODE  mfc_getinbuf(SSBSIP_MFC_ENC_INPUT_INFO *input_info)
{
	  int i;
    if (pCTX->v4l2_enc.bInputPhyVir) {
        input_info->YPhyAddr = (void*)0;
        input_info->CPhyAddr = (void*)0;
        input_info->YVirAddr = (void*)0;
        input_info->CVirAddr = (void*)0;

        if (NV12_TILE == pCTX->framemap) {
            /* 4:2:0, 2 Planes, 64x32 Tiles */
            input_info->YSize = align(align(pCTX->width, 128) * align(pCTX->height, 32), 8192); /* tiled mode */
            input_info->CSize = align(align(pCTX->width, 128) * align(pCTX->height >> 1, 32), 8192); /* tiled mode */
        } else { /* NV12_LINEAR (default) */
            /* 4:2:0, 2 Planes, linear */
            input_info->YSize = align(align(pCTX->width, 16) * align(pCTX->height, 16), 2048); /* width = 16B, height = 16B align */
            input_info->CSize = align(align(pCTX->width, 16) * align(pCTX->height >> 1, 8), 2048); /* width = 16B, height = 8B align */
        }
    } else {
        for (i = 0; i < pCTX->v4l2_enc.mfc_num_src_bufs; i++)
            if (BUF_DEQUEUED == pCTX->v4l2_enc.mfc_src_buf_flags[i])
                break;
        if (i == pCTX->v4l2_enc.mfc_num_src_bufs) {
            printf("[%s] No buffer is available.",__func__);
            return MFC_RET_ENC_GET_INBUF_FAIL;
        } else {
            /* FIXME check this for correct physical address */
           // input_info->YPhyAddr = (void*)pCTX->v4l2_enc.mfc_src_phys[i][0];
            //input_info->CPhyAddr = (void*)pCTX->v4l2_enc.mfc_src_phys[i][1];
            input_info->YVirAddr = (void*)pCTX->v4l2_enc.mfc_src_bufs[i][0];
            input_info->CVirAddr = (void*)pCTX->v4l2_enc.mfc_src_bufs[i][1];
            input_info->YSize = (int)pCTX->v4l2_enc.mfc_src_bufs_len[0];
            input_info->CSize = (int)pCTX->v4l2_enc.mfc_src_bufs_len[1];
            printf("line 1191 ysize is %d,csize is %d",input_info->YSize, input_info->CSize);
            pCTX->v4l2_enc.mfc_src_buf_flags[i] = BUF_ENQUEUED;
        }
    }
    printf("[%s] Input Buffer idx %d",__func__,i);
    return MFC_RET_OK;
	
	}




int init_device(void)
{  
	rtp_params_init();
	printf("rtp  init success\n");
	//dlclose(handle);
	cam_setfmt();
	fimc0_setfmt();
	mfc_setfmt();//  SsbSipMfcEncOpen  and  SsbSipMfcEncInit
	first_click_button = 1;
	err=mfc_getoutbuf(&oinfo);//SsbSipMfcEncGetOutBuf
	 if(err<0) {
        fprintf(stderr,"Error: SsbSipMfcEncGetOutBuf. Code %d\n",err);
    }else {
        printf("SsbSipMfcEncGetOutBuf suceeded\n");
       // fwrite(oinfo.StrmVirAddr,1,oinfo.headerSize,fp_h264);
        printf("oinfo.StrmVirAddr=0x%x, oinfo.headerSize=%d.\n",(unsigned)oinfo.StrmVirAddr,oinfo.headerSize);
    }
	err=mfc_getinbuf(&iinfo);//SsbSipMfcEncGetInBuf
	if(err<0) {
        fprintf(stderr,"Error: SsbSipMfcEncGetInBuf. Code %d\n",err);
    }else {
        printf("SsbSipMfcEncGetInBuf succeeded\n");
    }
	
	fimc0_reqbufs();
	cam_reqbufs();
	cam_setrate();
	
	printf("%s -\n", __func__);
	return 0;
}

int start_capturing(int cam_fd)
{
	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b;
	struct v4l2_plane plane[2];
	printf("%s +\n", __func__);
	
	for(i = 0;i < n_buffer;i ++)
	{
		struct v4l2_buffer buf;

		CLEAR(buf);	

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(-1 == ioctl(cam_fd,VIDIOC_QBUF,&buf))
		{
			perror("cam Fail to ioctl 'VIDIOC_QBUF'");
			exit(EXIT_FAILURE);
		}
	}


	for(i = 0; i < n_buffer; i++)
	{
		CLEAR(*plane);
		CLEAR(b);
		b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		b.memory = V4L2_MEMORY_MMAP;
		b.index = i;
		b.m.planes = plane;
		b.length = 2;
	/*	if(b.memory == V4L2_MEMORY_MMAP)
		{
			plane.m.userptr = (unsigned long)fimc0_cap_buf[i].start;
			plane.length = (unsigned long)fimc0_cap_buf_length;
			plane.bytesused = fimc0_cap_buf_length;
		}*/
		ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
		if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
			return -errno;	
		
	}


	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(cam_fd,VIDIOC_STREAMON,&type))
	{
		printf("i = %d.\n",i);
		perror("cam_fd Fail to ioctl 'VIDIOC_STREAMON'");
		exit(EXIT_FAILURE);
	}
	
			/*start processing */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fimc0_fd, VIDIOC_STREAMON, &type);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_STREAMON: %s\n", ERRSTR))
		return -errno;
/*
	type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fimc0_fd, VIDIOC_STREAMON, &type);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_STREAMON: %s\n", ERRSTR))
		return -errno;
*/
 
		
	type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fimc0_fd, VIDIOC_STREAMON, &type);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_STREAMON: %s\n", ERRSTR))
		return -errno;
	printf("%s - \n", __func__);
	return 0;
}
int cam_cap_dbuf(int *index)
{
	
	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer buf;

	bzero(&buf,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bzero(&buf,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if(-1 == ioctl(cam_fd,VIDIOC_DQBUF,&buf))
	{
		perror("cam Fail to ioctl 'VIDIOC_DQBUF'");
		exit(EXIT_FAILURE);
	}

	cam_buffers[buf.index].bytesused = buf.bytesused;//playload
//	printf("%s,Line:%d,bytesused:%d\n",__func__, __LINE__, buf.bytesused);
	*index = buf.index;

//	printf("%s -\n", __func__);
	return 0;

}
int cam_cap_qbuf(int index)
{

	struct v4l2_buffer buf;

		bzero(&buf,sizeof(buf));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = index;
		if(-1 == ioctl(cam_fd,VIDIOC_QBUF,&buf))
		{
			perror("Fail to ioctl 'VIDIOC_QBUF'");
			exit(EXIT_FAILURE);
		}

//		printf("%s -\n", __func__);
		return 0;
}

int fimc0_out_qbuf(int index)
{
	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b, buf;
	struct v4l2_plane plane;
	
	/* enqueue buffer to fimc0 output */
	CLEAR(plane);
	CLEAR(b);

	b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.index = index;
	b.m.planes = &plane;
	b.length = 1;

	plane.m.userptr = (unsigned long)fimc0_out_buf[index].start;
	plane.length = (unsigned long)fimc0_out_buf[index].length;
	plane.bytesused = fimc0_out_buf[index].length;

//	printf("fimc0_out_buf:0x%08lx,length:%d,byteused:%d\n",fimc0_out_buf[index].start, 	fimc0_out_buf[index].length, fimc0_out_buf[index].bytesused);
	//process_image(fimc0_out_buf[index].start,0);	
	ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
	
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
		return -errno;	
//	printf("%s -\n", __func__);
}

int fimc0_out_dbuf(int *index)
{

	unsigned int i;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b, buf;
	struct v4l2_plane plane[3];
	
	/* enqueue buffer to fimc0 output */
	CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	b.memory = V4L2_MEMORY_USERPTR;
//	b.index = index;
	b.m.planes = plane;
	b.length = 1;
//	plane[0].m.userptr = (unsigned long)fimc0_out_buf[index].start;
//	plane[0].length = (unsigned long)fimc0_out_buf[index].length;
//	planes[0].bytesused = fimc0_out_buf[buf.index].length;
	ret = ioctl(fimc0_fd, VIDIOC_DQBUF, &b);
	*index = b.index;
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_DQBUF: %s\n", ERRSTR))
		return -errno;	
}

int fimc0_cap_dbuf(int *index)
{

	unsigned int i=0,n=0;
	enum v4l2_buf_type type;
	int ret;
	struct v4l2_buffer b, buf;
	struct v4l2_plane plane[2];
	static int count = 0;
	/* enqueue buffer to fimc0 output */
	CLEAR(*plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = plane;
	b.length = 2;
	ret = ioctl(fimc0_fd, VIDIOC_DQBUF, &b);
	*index = b.index;
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_DQBUF: %s\n", ERRSTR))
		return -errno;	
	fimc0_cap_index = b.index;
	memcpy(iinfo.YVirAddr, bufs->addr[fimc0_cap_index* bufs->nplanes + 0], bufs->lengths[0]);
	memcpy(iinfo.CVirAddr, bufs->addr[fimc0_cap_index* bufs->nplanes + 1], bufs->lengths[1]);
	printf("memcpy success\n");
	//for(n=0;n<2;n++)
	 //fwrite(bufs->addr[fimc0_cap_index* bufs->nplanes + n],bufs->lengths[n],1,fp_nv12);
	count ++;
	return 0;
}

int fimc0_cap_qbuf(int index)
{
//	int *pdata = (int *)addr;
	int ret;
	struct v4l2_buffer b;
	struct v4l2_plane plane[2];
	static unsigned int count = 0;
	//sleep(0);
//	printf("%s +\n", __func__);
	CLEAR(*plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = plane;
	b.length = 2;
	b.index = index;
//	plane.m.userptr = (unsigned long)fimc0_cap_buf[index].start;
	//plane.length = (unsigned long)fimc0_cap_buf_length;
//	plane.bytesused = fimc0_cap_buf_length;

	ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
		return -errno;
//	printf("%s -\n", __func__);
}
void process_cam_to_fimc0(void)
{
	int index;
	printf("%s +\n", __func__);
	cam_cap_dbuf(&index);
	memcpy(fimc0_out_buf[index].start, cam_buffers[index].start, fimc0_out_buf_length);
	temp_buf = (unsigned char *)cam_buffers[index].start;
	fimc0_out_qbuf(index);
	printf("%s -,index:%d\n",__func__, index);
}
void process_fimc0_to_cam(void)
{
	 int index;
//	printf("%s +\n", __func__);
	fimc0_out_dbuf(&index);
	cam_cap_qbuf(index);
//	printf("%s -,index:%d\n",__func__, index);
}

int process_fimc0_capture(void)
{
//	int *pdata = (int *)addr;
	int ret;
	struct v4l2_buffer b;
	struct v4l2_plane plane;
	static unsigned int count = 0;
	//sleep(0);
//	printf("%s +\n", __func__);
		CLEAR(plane);
	CLEAR(b);
	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	b.memory = V4L2_MEMORY_MMAP;
	b.m.planes = &plane;
	b.length = 1;

	/* grab processed buffers */
	ret = ioctl(fimc0_fd, VIDIOC_DQBUF, &b);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_DQBUF: %s\n", ERRSTR))
		return -errno;
	count ++;
	printf("%s,%d\n",__func__, count);

	//memcpy((void *)fb_buf, (void *)fimc0_cap[b.index], 800*480*4);
	//process_rgb32(fimc0_cap[b.index]);	//temp_buf = (char *)fimc0_cap[b.index];
		
	/* enqueue buffer to fimc0 capture */
//	CLEAR(plane);
//	CLEAR(b);
//	b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
//	b.memory = V4L2_MEMORY_MMAP;
//	b.index = 0;
//	b.m.planes = &plane;
//	b.length = 1;

	ret = ioctl(fimc0_fd, VIDIOC_QBUF, &b);
	if (ERR_ON(ret < 0, "fimc0: VIDIOC_QBUF: %s\n", ERRSTR))
		return -errno;
//	printf("%s -\n", __func__);
	//memcpy(fb_buf, fimc_out, 640)	

}
SSBSIP_MFC_ERROR_CODE mfc_out_qbuf(SSBSIP_MFC_ENC_INPUT_INFO *input_info)
{
	 struct v4l2_buffer qbuf;
   struct v4l2_plane planes[MFC_ENC_NUM_PLANES];
   int ret,i;
	 memset(&qbuf, 0, sizeof(qbuf));
	if (pCTX->v4l2_enc.bInputPhyVir) {
        qbuf.memory = V4L2_MEMORY_USERPTR;
        qbuf.index = pCTX->v4l2_enc.beingUsedIndex;
        planes[0].m.userptr = (unsigned long)input_info->YPhyAddr;
        planes[0].length = input_info->YSize;
        planes[0].bytesused = input_info->YSize;
        planes[1].m.userptr = (unsigned long)input_info->CPhyAddr;
        planes[1].length = input_info->CSize;
        planes[1].bytesused = input_info->CSize;

        /* FIXME, this is only for case of not using B frame,
        Camera side should know which buffer is queued() refering to index of
        MFC dqbuf() */
        pCTX->v4l2_enc.beingUsedIndex++;
        pCTX->v4l2_enc.beingUsedIndex %= MFC_ENC_NUM_SRC_BUFS;
        printf("[%s] Phy Input Buffer idx Queued %d",__func__,pCTX->v4l2_enc.beingUsedIndex);
    } else {
        for (i = 0; i < pCTX->v4l2_enc.mfc_num_src_bufs; i++)
            if (pCTX->v4l2_enc.mfc_src_bufs[i][0] == input_info->YVirAddr)
                break;

        if (i == pCTX->v4l2_enc.mfc_num_src_bufs) {
            printf("[%s] Can not use the buffer",__func__);
            return MFC_RET_INVALID_PARAM;
        } else {
            pCTX->v4l2_enc.beingUsedIndex = i;
            //pCTX->v4l2_enc.mfc_src_buf_flags[i] = BUF_ENQUEUED;
        }
        qbuf.memory = V4L2_MEMORY_MMAP;
        qbuf.index = pCTX->v4l2_enc.beingUsedIndex;
        planes[0].bytesused = pCTX->width * pCTX->height;
        planes[1].bytesused = (pCTX->width * pCTX->height) >> 1;
        printf("[%s] Input Buffer idx Queued %d",__func__,pCTX->v4l2_enc.beingUsedIndex);
    }

    qbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    qbuf.m.planes = planes;
    qbuf.length = 2;

    ret = ioctl(pCTX->hMFC, VIDIOC_QBUF, &qbuf);
    if (ret != 0) {
        printf("[%s] VIDIOC_QBUF failed, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE",__func__);
        return MFC_RET_ENC_SET_INBUF_FAIL;
    }

    return MFC_RET_OK;
	
	}
SSBSIP_MFC_ERROR_CODE mfc_enc_exe(void)
	{
		struct v4l2_buffer qbuf;
    struct v4l2_plane planes[MFC_ENC_NUM_PLANES];
    enum v4l2_buf_type type;
    struct v4l2_control ctrl;
    int ret;
    int dequeued_index;
    int loopcnt = 0;
    struct pollfd poll_events;
    int poll_state;
    
    ctrl.id = V4L2_CID_CODEC_FRAME_TAG;
    ctrl.value = pCTX->inframetag;
    ret = ioctl(pCTX->hMFC, VIDIOC_S_CTRL, &ctrl);
    if (ret != 0) {
        printf("[%s] VIDIOC_S_CTRL failed, V4L2_CID_CODEC_FRAME_TAG",__func__);
        return MFC_RET_ENC_EXE_ERR;
    }

    if (pCTX->v4l2_enc.bRunning == 0) {
			printf("pCTX 14\n");
        type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		printf("pCTX 15\n");
        ret = ioctl(pCTX->hMFC, VIDIOC_STREAMON, &type);
        if (ret != 0) {
            printf("[%s] VIDIOC_STREAMON failed, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE",__func__);
            return MFC_RET_ENC_EXE_ERR;
        }

        pCTX->v4l2_enc.bRunning = 1;
    }
		printf("pCTX 16\n");
    memset(&qbuf, 0, sizeof(qbuf));
    qbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	printf("pCTX 17\n");
    qbuf.memory = V4L2_MEMORY_MMAP;
    qbuf.m.planes = planes;
    qbuf.length = 1;
      poll_events.fd = pCTX->hMFC;
    poll_events.events = POLLIN | POLLERR;
    poll_events.revents = 0;
    /* wait for encoding */
    do {
        poll_state = poll((struct pollfd*)&poll_events, 1, POLL_ENC_WAIT_TIMEOUT);
        if (0 < poll_state) {
            if (poll_events.revents & POLLIN) { /* POLLIN */
                ret = ioctl(pCTX->hMFC, VIDIOC_DQBUF, &qbuf);
                if (ret == 0)
                    break;
            } else if (poll_events.revents & POLLERR) { /* POLLERR */
                printf("[%s] POLLERR\n",__func__);
                return MFC_RET_ENC_EXE_ERR;
            } else {
                printf("[%s] poll() returns 0x%x\n",__func__, poll_events.revents);
                return MFC_RET_ENC_EXE_ERR;
            }
        } else if (0 > poll_state) {
            printf("[%s] poll() Encoder POLL Timeout 0x%x\n",__func__, poll_events.revents);
            return MFC_RET_ENC_EXE_ERR;
        }
        loopcnt++;
    } while ((0 == poll_state) && (loopcnt < 5));

      if (pCTX->v4l2_enc.bRunning != 0) {
        pCTX->encodedframeType = (qbuf.flags & 0x18) >> 3; /* encoded frame type */

        printf("[%s] encoded frame type = %d\n",__func__, pCTX->encodedframeType);
        switch (pCTX->encodedframeType) {
        case 1:
            pCTX->encodedframeType = MFC_FRAME_TYPE_I_FRAME;
            break;
        case 2:
            pCTX->encodedframeType = MFC_FRAME_TYPE_P_FRAME;
            break;
        case 4:
            pCTX->encodedframeType = MFC_FRAME_TYPE_B_FRAME;
            break;
        default:
             printf("[%s] VIDIOC_DQBUF failed, encoded frame type is wrong",__func__);
        }
    }

     dequeued_index = qbuf.index;
    if (qbuf.m.planes[0].bytesused > 0) { /* FIXME later */
        pCTX->v4l2_enc.mfc_dst_bufs_bytes_used_len = qbuf.m.planes[0].bytesused;
    }

    ctrl.id = V4L2_CID_CODEC_FRAME_TAG;
    ctrl.value = 0;

    ret = ioctl(pCTX->hMFC, VIDIOC_G_CTRL, &ctrl);
    if (ret != 0) {
        printf("[%s] VIDIOC_G_CTRL failed, V4L2_CID_CODEC_FRAME_TAG",__func__);
        return MFC_RET_ENC_EXE_ERR;
    }

    pCTX->outframetagtop = ctrl.value;
    memset(&qbuf, 0, sizeof(qbuf));
    qbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    qbuf.memory = V4L2_MEMORY_MMAP;
    qbuf.index = dequeued_index;
    qbuf.m.planes = planes;
    qbuf.length = 1;

    ret = ioctl(pCTX->hMFC, VIDIOC_QBUF, &qbuf);
    if (ret != 0) {
        printf("[%s] VIDIOC_QBUF failed, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE",__func__);
        return MFC_RET_ENC_EXE_ERR;
    }
    if (pCTX->v4l2_enc.bRunning != 0) {
        memset(&qbuf, 0, sizeof(qbuf));
        qbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;

        if (pCTX->v4l2_enc.bInputPhyVir)
            qbuf.memory = V4L2_MEMORY_USERPTR;
        else
            qbuf.memory = V4L2_MEMORY_MMAP;

        ret = ioctl(pCTX->hMFC, VIDIOC_DQBUF, &qbuf);
        if (ret != 0) {
            printf("[%s] VIDIOC_DQBUF failed, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE",__func__);
            return MFC_RET_ENC_EXE_ERR;
        }
    }
    pCTX->v4l2_enc.mfc_src_buf_flags[qbuf.index] = BUF_DEQUEUED;

    /* Update context stream buffer address */
    pCTX->virStrmBuf = (int)pCTX->v4l2_enc.mfc_dst_bufs[dequeued_index];
    printf("[%s] Strm out idx %d",__func__,dequeued_index);

    return MFC_RET_OK; 
		}

int mainloop(int cam_fd)
{   
	int count = 1;//CapNum;
	clock_t startTime, finishTime;
	double selectTime, frameTime;
	struct pollfd fds[2];//
	int nfds = 0;
	while(processRunning)
	{		
			struct timeval tv;
			int r,ret;
			struct timeval start;
			struct timeval end;
			int time_use=0;
			
			gettimeofday(&start,NULL);
			
			fds[0].events |= POLLIN | POLLPRI;//关心可读和可读高优先级事件
			fds[0].fd = cam_fd;

			fds[1].events |= POLLIN | POLLPRI | POLLOUT;//关心可读，可读高优先级事件，可写事件
			fds[1].fd = fimc0_fd;
			//fds[2].events |=POLLIN |POLLERR;
		//	fds[2].fd =pCTX->hMFC;
			//++nfds;
			
			r = poll(fds, 2, -1);
			if(-1 == r)
			{
				if(EINTR == errno)//阻塞的情况
					continue;
				
				perror("Fail to select");
				exit(EXIT_FAILURE);
			}
			if(0 == r)
			{
				fprintf(stderr,"select Timeout\n");
				exit(EXIT_FAILURE);
			}

			if (fds[0].revents & POLLIN)//
			{
				process_cam_to_fimc0();
			//	printf("debug\n");
				//gettimeofday(&end,NULL);
				//time_use=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
			//printf("time_use is %dms\n",time_use/1000);
			 
			}
			if (fds[1].revents & POLLIN)
			{ 
				printf("fimc0 has data to read\n");
				int index;
				fimc0_cap_dbuf(&index);
				mfc_out_qbuf(&iinfo);//SsbSipMfcEncSetInBuf
				mfc_enc_exe();//SsbSipMfcEncExe
				err=mfc_getoutbuf(&oinfo);//SsbSipMfcEncGetOutBuf
	       if(err<0) {
                      fprintf(stderr,"Error: SsbSipMfcEncGetOutBuf. Code %d\n",err);
                     }else {
                            printf("SsbSipMfcEncGetOutBuf suceeded\n");
						               }
					
			 // fwrite(oinfo.StrmVirAddr,1,oinfo.dataSize,fp_h264);
			  printf("oinfo.StrmVirAddr=0x%x, oinfo.dataSize=%d.\n",(unsigned)oinfo.StrmVirAddr,oinfo.dataSize);
			  printf("Frame # %d encoded\n", frmcnt++);
			  nal_rtp_send((unsigned char*)oinfo.StrmVirAddr,oinfo.dataSize); 
				fimc0_cap_qbuf(index);
			}
			if (fds[1].revents & POLLOUT)
			{
				int index;
				process_fimc0_to_cam();
			}		
		// nal_rtp_send(oinfo.StrmVirAddr,oinfo.dataSize);
	  
	   	
		}
		rtp_params_stop();
		printf("rtp_param_stop succeeded!\n ");
		fflush(stdout);
	return 0;
}

void stop_capturing(int cam_fd)
{
	enum v4l2_buf_type type;
	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(cam_fd,VIDIOC_STREAMOFF,&type))
	{
		perror("Fail to ioctl 'VIDIOC_STREAMOFF'");
		exit(EXIT_FAILURE);
	}
	return;
}

void uninit_camer_device(void)
{
	unsigned int i,n;

//fimc的output端解除
	for(i = 0;i < n_buffer;i ++)
	{
		if(-1 == munmap(fimc0_out_buf[i].start, fimc0_out_buf[i].length))  
		{
			exit(EXIT_FAILURE);
		}
	}
	
//fimc的capture端解除	
	for(i=0;i<n_buffer;i++)
	{  for (n=0;n<2;n++)
		if(-1==munmap(bufs->addr[i* bufs->nplanes + n],bufs->lengths[n])) 
		{ 
			exit(EXIT_FAILURE);
			
			}
		}

//cam的capture端解除
 for(i = 0;i < ReqButNum;i ++)
	{
		if(-1 == munmap(cam_buffers[i].start, cam_buffers[i].length))  
		{
			exit(EXIT_FAILURE);
		}
	}
 
//mfc的output端一层解除
	for(i = 0;i < MFC_ENC_NUM_SRC_BUFS;i ++)
	{
		if(-1 == munmap(pCTX->v4l2_enc.mfc_src_bufs[i][0], pCTX->v4l2_enc.mfc_src_bufs_len[0]))  
		{
			exit(EXIT_FAILURE);
		}
	}
	
//mfc的output端二层解除	
	 for(i = 0;i < MFC_ENC_NUM_SRC_BUFS;i ++)
	{
		if(-1 == munmap(pCTX->v4l2_enc.mfc_src_bufs[i][1], pCTX->v4l2_enc.mfc_src_bufs_len[1]))
		{
			exit(EXIT_FAILURE);
		}
	}

//mfc的capture端解除
	 for(i = 0;i < MFC_ENC_MAX_DST_BUFS;i ++)
	{
		if(-1 == munmap(pCTX->v4l2_enc.mfc_dst_bufs[i], pCTX->v4l2_enc.mfc_dst_bufs_len))
		{
			exit(EXIT_FAILURE);
		}
	}
	

	/*if (-1 == munmap(fb_buf, lcd_buf_size)) 
	{          
		perror(" Error: framebuffer device munmap() failed.\n");          
		exit (EXIT_FAILURE) ;       
	}   
	free(fimc0_out_buf);*/


	return;
}

void close_camer_device(int lcd_fd, int cam_fd)
{
	if(-1 == close(lcd_fd))
	{
		perror("Fail to close lcd_fd");
		exit(EXIT_FAILURE);
	}
	if(-1 == close(cam_fd))
	{
		perror("Fail to close cam_fd");
		exit(EXIT_FAILURE);
	}

	return;
}
static void *enc_thread(void *pVoid)
{ 
	if(processRunning == 1)
	{
		mainloop(cam_fd);
		return (void *)NULL;
	}
}
static void *display_thread(void *pVoid)
{
	static unsigned int count = 0;
	printf("display_thread start\n");

	int num = 800*480*4;
	while(1)
	{
		sem_wait(&lcd_sem);
		count ++;
		memcpy((void *)fb_buf, (void *)fimc0_cap_buf[fimc0_cap_index].start, num);
		//fb_wait_for_vsync(lcd_fd);
		fimc0_cap_qbuf(fimc0_cap_index);
	}
}


int sendBegin(void)
{  
	processRunning = 1;
	printf("begin. \n");
	fflush(stdout);
	
	fp_h264=fopen("test.264","wba+");
	open_camera_device();
	init_device();
	start_capturing(cam_fd);
	mainloop(cam_fd);
	stop_dev();
	if (param) free(param);
	if (pCTX) free(pCTX);
	if (bufs) free(bufs);
	
	return 0;
} 

long read_frame(unsigned char **buffer)
{
	int index = 0;
	*buffer = temp_buf;
	return fimc0_out_buf_length;
}
static int convert_yuv_to_rgb_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    pixel[0] = r * 220 / 256;
    pixel[1] = g * 220 / 256;
    pixel[2] = b * 220 / 256;
    
	return pixel32;
}

int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
	/* unsigned char* in = yuv;
	int istride=1280;
	int x,y,j;
	int y0,u,y1,v,r,g,b;
	long location=0;

	for ( x= 0; x< height; ++x) {
		for (j = 0, y=0; j < width *2 ; j += 4,y+=2) {
			
			// location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
				// (y+vinfo.yoffset) * finfo.line_length;

			y0 = in[j];
			u = in[j + 1] - 128;                
			y1 = in[j + 2];        
			v = in[j + 3] - 128;        

			r = y1 + (1.370705 * (v-128));
			g = y1 - (0.698001 * (v-128)) - (0.337633 * (u-128));
			b = y1 + (1.732446 * (u-128));

			rgb[ location++] = r<0?0:(r>255?255:r);
			rgb[ location++] = g<0?0:(g>255?255:g);
			rgb[ location++] = b<0?0:(b>255?255:b);  

			r = y0 + (1.370705 * (v-128));
			g = y0 - (0.698001 * (v-128)) - (0.337633 * (u-128));
			b = y0 + (1.732446 * (u-128));

			rgb[ location++ ]= r<0?0:(r>255?255:r);
			rgb[ location++] = g<0?0:(g>255?255:g);
			rgb[ location++] = b<0?0:(b>255?255:b);  
		}
		in +=istride;
	} */

	
    unsigned int in, out = 0;
    unsigned int pixel_16;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0, u, y1, v;
    for(in = 0; in < width * height * 2; in += 4) {
        pixel_16 = yuv[in + 3] << 24 |
                   yuv[in + 2] << 16 |
                   yuv[in + 1] <<  8 |
                   yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >>  8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
		
			
		// printf("The %dth pixel convert finished\n", in);
		// fflush(stdout);
    }
    return 0;
}
int stop_dev()
{  
	if(cam_fd != 0){
		processRunning = 0;

		stop_capturing(cam_fd);
	 	uninit_camer_device();
		close_camer_device(lcd_fd, cam_fd);
		
		printf("close camera device succeed! \n");
		fflush(stdout);
		
	}
	else
	{
		printf("There's no camera fid! \n ");
		fflush(stdout);
	}
	return 0;
}

void endOfSend()
{
	processRunning = 0;
}

void parametersSetting(string time, string ip, string ratio)
{
	return;
}

/* void setFormat()
{
	cam_setfmt();
	fimc0_setfmt();
	mfc_setfmt();//  SsbSipMfcEncOpen  and  SsbSipMfcEncInit
} */