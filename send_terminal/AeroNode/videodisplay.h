#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <cstddef>
#include <QtGui/QDialog>
#include <QTimer>
#include <QThread>
#include "ui_videodisplay.h"

#include "mfcenc.h"

class SendVideosThread : public QThread
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
    void flushBuff();  
    void sendVideos();
	void closeWidget();
private:
	Ui::VideoDisplayClass ui;
	//pass_data pd;  
    QTimer *timer;
	SendVideosThread *videos_Thread;
    unsigned char *bufrgb;  
};
#endif // VIDEODISPLAY_H
