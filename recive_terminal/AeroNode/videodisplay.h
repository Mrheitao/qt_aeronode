#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <cstddef>
#include <QtGui/QDialog>
#include <QThread>
#include <QTimer>
#include "ui_videodisplay.h"
#include "rtpreceivefimclatest.h"

class ReceiveVideosThread : public QThread
 {
     Q_OBJECT
 private:
     void run();
 };

class VideoDisplay : public QDialog
{
	Q_OBJECT

public:
	VideoDisplay(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoDisplay();
private slots:  
    void receiveVideos();
	void flushBuff();
	void closeWidget();
private:
	Ui::VideoDisplayClass ui;
	//pass_data pd;
	ReceiveVideosThread *videos_Thread;
    QTimer *timer;
    unsigned char *bufrgb;  
};

#endif // VIDEODISPLAY_H
