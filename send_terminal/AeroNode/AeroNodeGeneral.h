/*********************************************************************
**文件夹名：AeroNode
**Copyright (c) 2012-2013 南京航空航天 航空通信网络研究室
**创建人：李科艳、张雅静
**日期：2013.10
**描述：AN-1型半实物测试节点界面AeroNode是基于QT编程与Linux系统的界面，
        该界面简洁、清晰，代码结构清晰明了，具有很强的可读性与可修改性。
		该界面提供了四个模块，分别为：
		1、系统（AeroNodeSystem）模块
		2、功能（AeroNodeFunction）模块，功能模块还包含高效并发、拓扑
		   透明和弹机协同三个子模块。
		   高效并发界面在AeroNodeCCSMA文件中定义；
		   拓扑透明的骨干节点在AeroNodeESTDMA_Backbone文件中定义；
		   拓扑透明的子网网关节点在AeroNodeESTDMAZZ_SubnetGatway文件中定义；
		   弹机协同的载机节点在AeroNodeDTDMA_Aircraft文件中定义；
		   弹机协同的导弹节点在AeroNodeDTDMA_Missile文件中定义。
		3、同步（AeroNodeSynchronization）模块
		4、帮助（AeroNodeHelp）模块
		AeroNodeConfigDialogall文件设置了整体界面的布局。
		AeroNodePictureViewer文件中设置了图片浏览器的界面。
		AeroNodeVirtualKeyboard文件中设置了虚拟键盘。
**版本：AN-1型AeroNode半实物测试节点界面
**文件名：AeroNodeGeneral.h
**功能：AeroNodeGeneral.h定义了AeroNodeGeneral高效并发节点的收发界面类，
        同时还定义了与之相应的定时器线程MyThread_General类和拍照线程
		GetPictures类。 
*********************************************************************/


#include<QDialog>
#include<QSystemTrayIcon>
#include <QtCore/QTextCodec>
#include <QtGui>
#include <QWidget>
#include <QDate>
#include <QMainWindow>
#include <QtNetwork>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include<QLCDNumber>
#include<QTimer>
#include<QProcess>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <getopt.h>           
//#include <fcntl.h>            
//#include <unistd.h>
//#include <errno.h>
//#include <malloc.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <sys/time.h>
//#include <sys/mman.h>
//#include <sys/ioctl.h>
//#include <asm/types.h>        
//#include <linux/videodev2.h>
//#include <alloc.h>
//#include <fstream.h>
class QTimer;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QLabel;
class QTimeEdit;
class QLCDNumber;
class QVariant;
class QTime;
class QAction;
class QTextBrowser;
class QCheckBox;
class WidgetKeyboard;
using namespace std;
/*********************************************************************
// 类  名：MyThread_General
// 功  能：定时器线程
*********************************************************************/  
class MyThread_General : public QThread
{
     Q_OBJECT
public:
     void run();
public	slots:
	 void timerStrDisplay();
public:
     QLCDNumber  *m_timerLCD;
     int m_second;
     int m_minute;
     int m_hour;
	 int m_msecond;
	 int all_msecond;
	 int all_second;
     QTimer *m_timer;
signals:
     void changeText(QString str);
	  
};

/*********************************************************************
// 类  名：GetPictures
// 功  能：拍照线程
*********************************************************************/
class GetPictures_General : public QThread
{
    Q_OBJECT
public:
    void run();
	//void send(QString path);
	//int v4l2_camera();
	//int read_frame (FILE *k);
	QUdpSocket *udpSocket_sendpicture; 
	quint16 port_sendpicture;
	QHostAddress *hostaddr_picture;
signals:
    void Textchange(QString output);
};

/*********************************************************************
// 类  名：AeroNodeGeneral
// 功  能：高效并发节点收发界面类
*********************************************************************/
class AeroNodeGeneral: public QDialog 
{
    Q_OBJECT
public:
    AeroNodeGeneral( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~AeroNodeGeneral();	
public:
    QUdpSocket *udpSocket;
	QUdpSocket *udpSocket_config;
	QUdpSocket *udpSocket_repicture;//zyj
	QHostAddress *hostaddr1;
    quint16 port_send;
	quint16 port_rece;
	quint16 port_serve;//zyj
	quint16 port_config;	
    bool isStarted;
	double num;
	QLineEdit *LineEditHostIP;
	QPushButton *StartButton;
	QPushButton *ResetButton;
	QPushButton *moreButton;
	QPushButton *OPenkeyboard;
	QLabel *Title;
    QWidget *extension;
    QWidget *extension_1;
    //**************************************************
    //接收界面
    //**************************************************
	int send_pcket_1;
	double Length;
    QTextEdit *TextEditReceive; 
	int Num_rece_packets;
	QTime frTime;
	QTime last_ReceTime; 
	double timetofirst;
	double throughtput;
	int Index;
	QLineEdit *thruputE;
	QLineEdit *gaptimeE;
	QLineEdit *LineEditPacketnumber2;
	QLineEdit *LineEditPacketnumber;
	QLineEdit *LineEditPacketsize2;
	QLineEdit *LineEditPacketsize;
    QPushButton *PushButtonClose_R;
	QPushButton *PictureButton;	
	QPushButton *receive_PictureButton;
	QPushButton *receive_VideoButton;
    WidgetKeyboard *virtualKeyBoard;//软键盘
private slots:
    void timerStrDisplay(QString str);
	void Deal_guideinfo();
	void timeout();
	void PushButtonClose_clicked() ;
	void recv_slot();
	void runClicked();
	void moreButton_state();
	void Reseting();
	void config();
	void receiveVideos();
	//软键盘
	void on_btnShow_clicked();
    void on_btnHide_clicked();
	//提取图片	
    void readOutput(QString output);
    void pictures();
	void picture_Preventjitter();//打开图片防抖
	void getpictures_Preventjitter();
	void picturereceive();
	void Saverouteseting1();
	void Saverouteseting2();
	void Saverouteseting3();
	void videos();
private: 
    struct receivebuf *fristbuf;////存储接收图片的结构体 
    QString receive_output;	//用于输出接收图片信息
	QString specified_timer_span;
    QLineEdit* outputEdit;
    QTextEdit* receive_outputEdit;
    QLCDNumber  *m_timerLCD;//定时器
	QProcess* routecmd1; 
	QProcess* routecmd2; 
	QProcess* routecmd3; 
    int m_second;
    int m_minute;
    int m_hour;
	int m_msecond;
	int all_msecond;
	int all_second;	  
	int simulationtime;	  
    int time_span;
	int isStarted1;
	long receivephotonum;//收图片数目
	QTimer *m_timer;
	QComboBox *formatComboBox;
	QComboBox *IPComboBox;
    QComboBox *timer_spanComboBox;
	QComboBox *guideinformationComboBox;
	//QComboBox *OrangenNode1Edit;
	//QComboBox *OrangenNode2Edit;
	//QComboBox *OrangenNode3Edit;
	QComboBox *DestNode1Edit;
	QComboBox *DestNode2Edit;
	QComboBox *DestNode3Edit;
	QComboBox *NextNode1Edit;
	QComboBox *NextNode2Edit;
	QComboBox *NextNode3Edit;
    QPushButton *Saveseting1;
    QPushButton *Saveseting2;
    QPushButton *Saveseting3;	
	QPushButton *HandPictureButton;
	QPushButton *VideoSendButton;
    //创建线程
    MyThread_General *mythread;
    GetPictures_General *getpictures;
	void pictureclear();
};


/*********************************************************************
// 类  名：GetPictures
// 功  能：视频线程
*********************************************************************/
class GetVideos_General : public QThread
{
    Q_OBJECT
public:
    void run();
signals:
private slots:
};
