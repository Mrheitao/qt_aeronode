#include "videodisplay.h"

#include <QMessageBox>
#include <QScrollArea>
#include <QProcess>
#include <stdio.h>

void SendVideosThread::run()
 {
	sendBegin();
 }

VideoDisplay::VideoDisplay(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(sendVideos()));  
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(closeWidget()));  
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	this->setWindowIcon(QIcon(" "));//去掉标题栏图标
	
	//pd.fd = -1;
	videos_Thread = new SendVideosThread;
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
  
void VideoDisplay::flushBuff()  
{  
	//QMessageBox::information(this,tr("Tip"),tr("Flush buff.")); 
    unsigned char *buffer = NULL;
	
    long temp = read_frame (&buffer);
	
	if(temp == 0 || buffer == NULL){
		return;
	}
	
    bufrgb = (unsigned char *)malloc(640 * 480 * 3);
	
    memset(bufrgb, 0, 640 * 480* 3);
	
	//QMessageBox::information(this,tr("Tip"),tr("Begin convert."));  
    convert_yuv_to_rgb_buffer((unsigned char *)buffer,bufrgb,640,480);  
    QImage image(bufrgb,640,480,QImage::Format_RGB888);  
    ui.displayLabel->setPixmap(QPixmap::fromImage(image));
	free(bufrgb);
}  

void VideoDisplay::sendVideos()  
{  
	//parametersSetting(ui.runTime, ui.desAddress, ui.compressionRatio);
	videos_Thread->start();
	timer->start();
    ui.sendButton->setDisabled(TRUE);
	/* ui.runTime->setEditable(true);
	ui.desAddress->setEditable(true);
	ui.compressionRatio->setEditable(true); */
}

void VideoDisplay::closeWidget()
{
	endOfSend();
	videos_Thread->exit();
	timer->stop();
	this->close();
}