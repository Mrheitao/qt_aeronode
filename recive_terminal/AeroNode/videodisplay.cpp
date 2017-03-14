#include "videodisplay.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QProcess>
#include <stdio.h>


void ReceiveVideosThread::run()
 {
	receiveBegin();
 }

VideoDisplay::VideoDisplay(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
    connect(ui.receiveButton, SIGNAL(clicked()), this, SLOT(receiveVideos()));  
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(closeWidget()));  
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	this->setWindowIcon(QIcon(" "));//去掉标题栏图标
	
	//pd.fd = -1;
	videos_Thread = new ReceiveVideosThread;
    timer = new QTimer(this);  
    timer->setInterval(50); 
    connect(timer, SIGNAL(timeout()), this, SLOT(flushBuff()));  
  
    //pd.dev_name = "/dev/video15";  
}

VideoDisplay::~VideoDisplay()
{
	if (timer->isActive()) {  
        timer->stop();  
    }
}

void VideoDisplay::receiveVideos()  
{  
	videos_Thread->start();
    timer->start();
    ui.receiveButton->setDisabled(TRUE);
}

void VideoDisplay::flushBuff()  
{  
	long width = 640, height = 480;
	//QMessageBox::information(this,tr("Tip"),tr("Flush buff.")); 
   unsigned char *buffer = NULL;
	unsigned char *praes_buffer = NULL;
	int j = 0 ;
    long  temp = read_frame (&buffer);
	if(temp == 0 || buffer == NULL){
		return;
	}
	praes_buffer = (unsigned char *)malloc(width * height * 4);
	if (praes_buffer == NULL )
	{
		printf("not enough memory");
		return;
	}
	for (j=0; j<height; j++) {
            memcpy(praes_buffer+j*width*4 ,buffer+j*width*4,width*4);
    } 
	printf("This frame's length is %d\n",temp);
    bufrgb = (unsigned char *)malloc(width * height * 3+1);
    memset(bufrgb, 0, width * height* 3+1);
	memset(bufrgb, 0, width * height* 3);
    convert_rgbLCD_to_rgb24_buffer((unsigned char *)praes_buffer,bufrgb,width,height);  
    QImage image(bufrgb,width,height,QImage::Format_RGB888);  
    ui.displayLabel->setPixmap(QPixmap::fromImage(image));
	
	free(bufrgb);
	if (praes_buffer)  free(praes_buffer);
}  

void VideoDisplay::closeWidget()
{
	videos_Thread->exit();
	timer->stop();
	//stop_dev(&pd);
	this->close();
}