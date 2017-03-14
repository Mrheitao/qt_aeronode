/*********************************************************************
**�ļ�������AeroNode
**Copyright (c) 2012-2013 �Ͼ����պ��� ����ͨ�������о���
**�����ˣ�����ޡ����ž�
**���ڣ�2013.10
**������AN-1�Ͱ�ʵ����Խڵ����AeroNode�ǻ���QT�����Linuxϵͳ�Ľ��棬
        �ý����ࡢ����������ṹ�������ˣ����к�ǿ�Ŀɶ�������޸��ԡ�
		�ý����ṩ���ĸ�ģ�飬�ֱ�Ϊ��
		1��ϵͳ��AeroNodeSystem��ģ��
		2�����ܣ�AeroNodeFunction��ģ�飬����ģ�黹������Ч����������
		   ͸���͵���Эͬ������ģ�顣
		   ��Ч����������AeroNodeCCSMA�ļ��ж��壻
		   ����͸���ĹǸɽڵ���AeroNodeESTDMA_Backbone�ļ��ж��壻
		   ����͸�����������ؽڵ���AeroNodeESTDMAZZ_SubnetGatway�ļ��ж��壻
		   ����Эͬ���ػ��ڵ���AeroNodeDTDMA_Aircraft�ļ��ж��壻
		   ����Эͬ�ĵ����ڵ���AeroNodeDTDMA_Missile�ļ��ж��塣
		3��ͬ����AeroNodeSynchronization��ģ��
		4��������AeroNodeHelp��ģ��
		AeroNodeConfigDialogall�ļ��������������Ĳ��֡�
		AeroNodePictureViewer�ļ���������ͼƬ������Ľ��档
		AeroNodeVirtualKeyboard�ļ���������������̡�
**�汾��AN-1��AeroNode��ʵ����Խڵ����
**�ļ�����AeroNodeGeneral.h
**���ܣ�AeroNodeGeneral.h������AeroNodeGeneral��Ч�����ڵ���շ������࣬
        ͬʱ����������֮��Ӧ�Ķ�ʱ���߳�MyThread_General��������߳�
		GetPictures�ࡣ 
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
// ��  ����MyThread_General
// ��  �ܣ���ʱ���߳�
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
// ��  ����GetPictures
// ��  �ܣ������߳�
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
// ��  ����AeroNodeGeneral
// ��  �ܣ���Ч�����ڵ��շ�������
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
    //���ս���
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
    WidgetKeyboard *virtualKeyBoard;//�����
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
	//�����
	void on_btnShow_clicked();
    void on_btnHide_clicked();
	//��ȡͼƬ	
    void readOutput(QString output);
    void pictures();
	void picture_Preventjitter();//��ͼƬ����
	void getpictures_Preventjitter();
	void picturereceive();
	void Saverouteseting1();
	void Saverouteseting2();
	void Saverouteseting3();
	void videos();
private: 
    struct receivebuf *fristbuf;////�洢����ͼƬ�Ľṹ�� 
    QString receive_output;	//�����������ͼƬ��Ϣ
	QString specified_timer_span;
    QLineEdit* outputEdit;
    QTextEdit* receive_outputEdit;
    QLCDNumber  *m_timerLCD;//��ʱ��
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
	long receivephotonum;//��ͼƬ��Ŀ
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
    //�����߳�
    MyThread_General *mythread;
    GetPictures_General *getpictures;
	void pictureclear();
};


/*********************************************************************
// ��  ����GetPictures
// ��  �ܣ���Ƶ�߳�
*********************************************************************/
class GetVideos_General : public QThread
{
    Q_OBJECT
public:
    void run();
signals:
private slots:
};
