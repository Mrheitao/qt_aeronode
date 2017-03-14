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
**�ļ�����AeroNodeGeneral.cpp
**���ܣ�AeroNodeGeneral�ඨ���˸�Ч�����ڵ�ķ�������ս��棬ͬʱ������
        ����֮���׵Ķ�ʱ���߳��������̡߳� 
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QTranslator>
#include <QtGui>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDesktopWidget>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeGeneral.h"
#include <stdio.h>
#include "AeroNodeVirtualKeyboard.h"
#include "AeroNodePictureViewer.h"
#include "AeroNodeConfig.h"

#include "videodisplay.h"

//#include <netdb.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <iostream.h>
//#include <fstream.h>
//using namespace std;
static QString paramter=NULL;
static QString str_IP=NULL;
//static char *dev_name = "/dev/video15";//����ͷ�豸��
//static int   fd = -1;   //�ļ�������fd
//static struct buffer *buffers = NULL;
//static unsigned int  n_buffers = 0;
/*********************************************************************
// ��������AeroNodeGeneral
// ��  �ܣ����캯��
// ��  �أ���
*********************************************************************/
AeroNodeGeneral::AeroNodeGeneral( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//�����������
    //*****************************************************************
    //���ü�ʱ���߳��������߳�	
    //*****************************************************************
	mythread = new MyThread_General;
	getpictures=new GetPictures_General;
	connect(getpictures,SIGNAL(Textchange(QString)),this,SLOT(readOutput(QString)),Qt::QueuedConnection);
	connect(mythread,SIGNAL(changeText(QString)),this,SLOT(timerStrDisplay(QString)),Qt::QueuedConnection);
    //*****************************************************************
    //���������շ�����ײ�	
    //*****************************************************************
	QVBoxLayout *VSRbMain = new QVBoxLayout( this );	
	extension = new QWidget;
	extension_1 = new QWidget;
    //*****************************************************************
    //��ʼ������	
    //*****************************************************************
	Num_rece_packets=1;//ͳ�ƽ������ݰ���Ŀ
	timetofirst=0;//��ʱͳ����
	throughtput=0;//ͳ��������
    send_pcket_1=0;//�����հ���b�ַ�ʱ��һֱ����������send_pcket_1=1ʱ������ʱ��
	Length=0;//���յ������ݰ��ĳ�ʼ����
	Index=1;
    num=1;
	isStarted=false;
	isStarted1=0;
	receivephotonum=0;
	fristbuf=NULL;//�洢����ͼƬ�Ľṹ��
    //*****************************************************************
    //���÷���ʱ��
    //*****************************************************************
	QLabel *LCD=new QLabel(this);
	LCD->setText(tr("����ʱ��:"));
	m_timerLCD=new QLCDNumber;
  	m_timerLCD->setFrameStyle ( QLCDNumber::NoFrame);
	m_timerLCD->setFixedSize(280,60);
    m_timerLCD->setNumDigits(8);
    m_timerLCD->display("00:00:00");
	m_timerLCD->setSegmentStyle(QLCDNumber::Flat);
    //*****************************************************************
    //�������á�����\���͡��رհ�ť	
    //*****************************************************************	
	StartButton = new QPushButton( this);
    StartButton->setText( tr( "��ʼ" ) ); 
	StartButton ->setFixedSize(120,60);
	connect(StartButton,SIGNAL(clicked()),this,SLOT(Deal_guideinfo()));
	ResetButton = new QPushButton( this);
    ResetButton->setText( tr( "����" ) ); 
	ResetButton ->setFixedSize(120,60);
	connect(ResetButton,SIGNAL(clicked()),this,SLOT(Reseting()));
	moreButton = new QPushButton( QIcon(":/images/interface.png"),tr("����"),this);
	moreButton->setFixedSize( 160,60);
    PushButtonClose_R = new QPushButton( this);
    PushButtonClose_R->setText( tr( "�ر�" ) ); 
	PushButtonClose_R ->setFixedSize(120,60);
	connect(PushButtonClose_R,SIGNAL(clicked()),this,SLOT(PushButtonClose_clicked()));
    //*****************************************************************
    //����̧ͷ	
    //*****************************************************************	
	Title=new QLabel(this);
	Title->setText(tr("                    ���ͽ���                         "));
	Title->setAlignment(Qt::AlignHCenter);
    //*****************************************************************
    //���ֵ�һ��	
    //***************************************************************** 	
	QHBoxLayout *first_line = new QHBoxLayout;
	first_line->addWidget(LCD);
	first_line->addWidget(m_timerLCD);	
	first_line->addWidget(StartButton);
	first_line->addWidget(Title);
	first_line->addWidget(ResetButton);
	first_line->addWidget(moreButton);
	first_line->addWidget(PushButtonClose_R);
	VSRbMain->addLayout(first_line);
    //*****************************************************************
    //���÷��ͽ���	
    //*****************************************************************
	QVBoxLayout *vbMain = new QVBoxLayout;
	extension->setLayout(vbMain);
	 //*****************************************************************
    //����ҳ�沼��ҵ���������
    //***************************************************************** 	
	QGroupBox *count_send1= new QGroupBox(tr("ҵ�����"));
    //*****************************************************************
    //���÷���ʱ�������
    //***************************************************************** 	
    QLabel *simulation=new QLabel(this);
	simulation->setText(tr("����ʱ��:"));
    QLineEdit *lineEdit1 = new QLineEdit;//����
    lineEdit1->setAlignment(Qt::AlignCenter);	
    formatComboBox = new QComboBox;
	formatComboBox->addItem(tr("80"));
    formatComboBox->addItem(tr("120"));
	formatComboBox->setEditable(true);//�ɱ༭
	formatComboBox->setMaxCount(2);//�������ѡ��
	formatComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	formatComboBox->setLineEdit(lineEdit1); 
	simulation->setBuddy(formatComboBox);
	//*****************************************************************
    //���÷������������
    //***************************************************************** 
	QLabel *guideinformation=new QLabel(this);	
	guideinformation->setText(tr("��������:"));	
    QLineEdit *lineEdit2 = new QLineEdit;//����
    lineEdit2->setAlignment(Qt::AlignCenter);
	guideinformationComboBox = new QComboBox;
    guideinformationComboBox->addItem(tr("01010"));
	guideinformationComboBox->setEditable(true);
	guideinformationComboBox->setMaxCount(1);
	guideinformationComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	guideinformationComboBox->setLineEdit(lineEdit2);
	guideinformation->setBuddy(guideinformationComboBox);
	//*****************************************************************
    //���������
    //***************************************************************** 
	virtualKeyBoard = new WidgetKeyboard(this);//���������
	OPenkeyboard = new QPushButton( QIcon(":/images/keyboard.png"),tr(""),this);
	OPenkeyboard->setFixedSize(200,120);	
	OPenkeyboard->setStyleSheet("QPushButton {border-style:outset;border-color: beige;font: bold 12px;min-width: 2.6em;padding: 6px;}");
    connect(OPenkeyboard, SIGNAL(clicked()), this, SLOT(on_btnShow_clicked()));	//����
	//*****************************************************************
    //���÷�����������
    //*****************************************************************
	QLabel *Timegap=new QLabel(this);
	Timegap->setText(tr("�������:"));
    QLineEdit *lineEdit3 = new QLineEdit;//����
    lineEdit3->setAlignment(Qt::AlignCenter);
	timer_spanComboBox = new QComboBox;
	timer_spanComboBox->addItem(tr("100"));
    timer_spanComboBox->addItem(tr("150"));
	timer_spanComboBox->addItem(tr("200"));
    timer_spanComboBox->setEditable(true);
	timer_spanComboBox->setMaxCount(3);
	timer_spanComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	timer_spanComboBox->setLineEdit(lineEdit3);
	Timegap->setBuddy(timer_spanComboBox);
	//*****************************************************************
    //����Ŀ���ַ
    //***************************************************************** 	
	QLabel *LabelPeerIP=new QLabel(this);
	LabelPeerIP->setText(tr("Ŀ���ַ:"));
    QLineEdit *lineEdit4 = new QLineEdit;//����
    lineEdit4->setAlignment(Qt::AlignCenter);
	IPComboBox = new QComboBox;
	IPComboBox->addItem(tr("192.168.2.1"));
    IPComboBox->addItem(tr("192.168.2.2"));
	IPComboBox->addItem(tr("192.168.2.3"));
	IPComboBox->addItem(tr("192.168.2.4"));
	IPComboBox->addItem(tr("192.168.2.5"));
	IPComboBox->addItem(tr("192.168.2.6"));
	IPComboBox->setMaxCount(7);
    IPComboBox->setEditable(true);
	IPComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	IPComboBox->setLineEdit(lineEdit4);
	LabelPeerIP->setBuddy(IPComboBox);
	//*****************************************************************
    //���÷�����Ŀ
    //***************************************************************** 	
    QLabel *Packetnumber= new QLabel(this);
	Packetnumber->setText(tr("������Ŀ:"));
	LineEditPacketnumber= new QLineEdit(this);
	LineEditPacketnumber->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //���÷�����С
    //*****************************************************************
	QLabel *Packetsize=new QLabel(this);
	Packetsize->setText(tr("������С:"));
    LineEditPacketsize= new QLineEdit(this);
	LineEditPacketsize->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //����ͼ���ȡ��Ϣ	
    //*****************************************************************	  	
    QLabel *status= new QLabel(this);
	status->setText(tr("������Ϣ:"));
	outputEdit= new QLineEdit(this);
	outputEdit->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //���ò鿴ͼƬ��ť	
    //*****************************************************************	
	PictureButton = new QPushButton( this);//�鿴ͼ�������ͼ��ť
    PictureButton->setText( tr( "�鿴ͼ��" ) );
    PictureButton ->setFixedSize(160,60);
	connect(PictureButton, SIGNAL(clicked()), this, SLOT(pictures()));	//���ô�����ȡͼƬ
	//*****************************************************************
    //��������ͼƬ��ť	
    //*****************************************************************	
	HandPictureButton = new QPushButton( this);
    HandPictureButton->setText( tr( "����ͼƬ" ) );
    HandPictureButton->setFixedSize(160,60);
	connect(HandPictureButton, SIGNAL(clicked()), this, SLOT(getpictures_Preventjitter()));	
	//*****************************************************************
    //���ý�����Ƶ��ť
    //*****************************************************************	
	VideoSendButton = new QPushButton( this);//������Ƶ��ť
    VideoSendButton->setText( tr( "������Ƶ" ) );
    VideoSendButton ->setFixedSize(160,60);
	connect(VideoSendButton, SIGNAL(clicked()), this, SLOT(receiveVideos()));	//���ô�����ʼ����
	//*****************************************************************
    //���ֻ������Ͳ�����
    //***************************************************************** 
	QGridLayout *mainLayout1 = new QGridLayout;//����
    mainLayout1->addWidget(simulation, 0, 0,1,1);
    mainLayout1->addWidget(formatComboBox, 0, 1,1,1);
    mainLayout1->addWidget(guideinformation, 0, 2,1,1);
    mainLayout1->addWidget(guideinformationComboBox, 0, 3,1,1);
	mainLayout1->addWidget(OPenkeyboard, 0, 4,1,1);
	mainLayout1->addWidget(Timegap, 0, 5,1,1);
	mainLayout1->addWidget(timer_spanComboBox, 0, 6,1,2);
	
	mainLayout1->addWidget(LabelPeerIP, 1, 0,1,1);
	mainLayout1->addWidget(IPComboBox, 1, 1,1,1);
	mainLayout1->addWidget(Packetnumber,1,2,1,1);
	mainLayout1->addWidget(LineEditPacketnumber, 1, 3,1,2);
    mainLayout1->addWidget(Packetsize, 1, 5,1,1);
	mainLayout1->addWidget(LineEditPacketsize, 1, 6,1,2);
	
    mainLayout1->addWidget(status, 2, 0,1,1);
	mainLayout1->addWidget(outputEdit, 2, 1,1,5);
	mainLayout1->addWidget(PictureButton, 2, 6,1,1);
	mainLayout1->addWidget(HandPictureButton, 2, 7,1,1);
	mainLayout1->addWidget(VideoSendButton, 2, 8,1,1);
	
	count_send1->setLayout(mainLayout1);
	vbMain->addWidget(count_send1);
	//*****************************************************************
    //����ҳ�沼��·�ɲ�������
    //***************************************************************** 	
	QGroupBox *count_send2= new QGroupBox(tr("·������"));

	//QLabel *OrangenNode1= new QLabel(this);
	//OrangenNode1->setText(tr("Դ�ڵ�:"));
    //QLineEdit *lineEdit5 = new QLineEdit;//����
    //lineEdit5->setAlignment(Qt::AlignCenter);
	//OrangenNode1Edit = new QComboBox;
	//OrangenNode1Edit->addItem(tr("1"));
    //OrangenNode1Edit->addItem(tr("2"));
	//OrangenNode1Edit->addItem(tr("3"));
	//OrangenNode1Edit->addItem(tr("4"));
	//OrangenNode1Edit->setMaxCount(4);
    //OrangenNode1Edit->setEditable(true);
	//OrangenNode1Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	//OrangenNode1Edit->setLineEdit(lineEdit5);
	//OrangenNode1->setBuddy(OrangenNode1Edit);
	
	QLabel *DestNode1= new QLabel(this);
	DestNode1->setText(tr("Ŀ�Ľڵ�:"));
    QLineEdit *lineEdit6 = new QLineEdit;//����
    lineEdit6->setAlignment(Qt::AlignCenter);
	DestNode1Edit = new QComboBox;
	DestNode1Edit->addItem(tr("192.168.2.1"));
    DestNode1Edit->addItem(tr("192.168.2.2"));
	DestNode1Edit->addItem(tr("192.168.2.3"));
	DestNode1Edit->addItem(tr("192.168.2.4"));
	DestNode1Edit->setMaxCount(4);
    DestNode1Edit->setEditable(true);
	DestNode1Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	DestNode1Edit->setLineEdit(lineEdit6);
	DestNode1->setBuddy(DestNode1Edit);
	
	QLabel *NextNode1= new QLabel(this);
	NextNode1->setText(tr("��һ���ڵ�:"));
    QLineEdit *lineEdit7 = new QLineEdit;//����
    lineEdit7->setAlignment(Qt::AlignCenter);
	NextNode1Edit = new QComboBox;
	NextNode1Edit->addItem(tr("192.168.2.1"));
    NextNode1Edit->addItem(tr("192.168.2.2"));
	NextNode1Edit->addItem(tr("192.168.2.3"));
	NextNode1Edit->addItem(tr("192.168.2.4"));
	NextNode1Edit->setMaxCount(4);
    NextNode1Edit->setEditable(true);
	NextNode1Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	NextNode1Edit->setLineEdit(lineEdit7);
	NextNode1->setBuddy(NextNode1Edit);
	
	Saveseting1 = new QPushButton( this);//����·�����ð�ť
    Saveseting1->setText( tr( "����" ) );
    Saveseting1 ->setFixedSize(120,60);
	connect(Saveseting1, SIGNAL(clicked()), this, SLOT(Saverouteseting1()));	//����·������
	
	//QLabel *OrangenNode2= new QLabel(this);
	//OrangenNode2->setText(tr("Դ�ڵ�:"));
    //QLineEdit *lineEdit8 = new QLineEdit;//����
   // lineEdit8->setAlignment(Qt::AlignCenter);
	//OrangenNode2Edit = new QComboBox;
	//OrangenNode2Edit->addItem(tr("1"));
    //OrangenNode2Edit->addItem(tr("2"));
	//OrangenNode2Edit->addItem(tr("3"));
	//OrangenNode2Edit->addItem(tr("4"));
	//OrangenNode2Edit->setMaxCount(4);
    //OrangenNode2Edit->setEditable(true);
	//OrangenNode2Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	//OrangenNode2Edit->setLineEdit(lineEdit8);
	//OrangenNode2->setBuddy(OrangenNode2Edit);
	
	QLabel *DestNode2= new QLabel(this);
	DestNode2->setText(tr("Ŀ�Ľڵ�:"));
    QLineEdit *lineEdit9 = new QLineEdit;//����
    lineEdit9->setAlignment(Qt::AlignCenter);
	DestNode2Edit = new QComboBox;
	DestNode2Edit->addItem(tr("192.168.2.1"));
    DestNode2Edit->addItem(tr("192.168.2.2"));
	DestNode2Edit->addItem(tr("192.168.2.3"));
	DestNode2Edit->addItem(tr("192.168.2.4"));
	DestNode2Edit->setMaxCount(4);
    DestNode2Edit->setEditable(true);
	DestNode2Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	DestNode2Edit->setLineEdit(lineEdit9);
	DestNode2->setBuddy(DestNode2Edit);
	
	QLabel *NextNode2= new QLabel(this);
	NextNode2->setText(tr("��һ���ڵ�:"));
    QLineEdit *lineEdit10 = new QLineEdit;//����
    lineEdit10->setAlignment(Qt::AlignCenter);
	NextNode2Edit = new QComboBox;
	NextNode2Edit->addItem(tr("192.168.2.1"));
    NextNode2Edit->addItem(tr("192.168.2.2"));
	NextNode2Edit->addItem(tr("192.168.2.3"));
	NextNode2Edit->addItem(tr("192.168.2.4"));
	NextNode2Edit->setMaxCount(4);
    NextNode2Edit->setEditable(true);
	NextNode2Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	NextNode2Edit->setLineEdit(lineEdit10);
	NextNode2->setBuddy(NextNode2Edit);
	
	Saveseting2 = new QPushButton( this);//����·�����ð�ť
    Saveseting2->setText( tr( "����" ) );
    Saveseting2 ->setFixedSize(120,60);
	connect(Saveseting2, SIGNAL(clicked()), this, SLOT(Saverouteseting2()));	//����·������
	
	//QLabel *OrangenNode3= new QLabel(this);
	//OrangenNode3->setText(tr("Դ�ڵ�:"));
    //QLineEdit *lineEdit11 = new QLineEdit;//����
    //lineEdit11->setAlignment(Qt::AlignCenter);
	//OrangenNode3Edit = new QComboBox;
	//OrangenNode3Edit->addItem(tr("1"));
    //OrangenNode3Edit->addItem(tr("2"));
	//OrangenNode3Edit->addItem(tr("3"));
	//OrangenNode3Edit->addItem(tr("4"));
	//OrangenNode3Edit->setMaxCount(4);
    //OrangenNode3Edit->setEditable(true);
	//OrangenNode3Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	//OrangenNode3Edit->setLineEdit(lineEdit11);
	//OrangenNode3->setBuddy(OrangenNode3Edit);
	
	QLabel *DestNode3= new QLabel(this);
	DestNode3->setText(tr("Ŀ�Ľڵ�:"));
    QLineEdit *lineEdit12 = new QLineEdit;//����
    lineEdit12->setAlignment(Qt::AlignCenter);
	DestNode3Edit = new QComboBox;
	DestNode3Edit->addItem(tr("192.168.2.1"));
    DestNode3Edit->addItem(tr("192.168.2.2"));
	DestNode3Edit->addItem(tr("192.168.2.3"));
	DestNode3Edit->addItem(tr("192.168.2.4"));
	DestNode3Edit->setMaxCount(4);
    DestNode3Edit->setEditable(true);
	DestNode3Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	DestNode3Edit->setLineEdit(lineEdit12);
	DestNode3->setBuddy(DestNode3Edit);
	
	QLabel *NextNode3= new QLabel(this);
	NextNode3->setText(tr("��һ���ڵ�:"));
    QLineEdit *lineEdit13 = new QLineEdit;//����
    lineEdit13->setAlignment(Qt::AlignCenter);
	NextNode3Edit = new QComboBox;
	NextNode3Edit->addItem(tr("192.168.2.1"));
    NextNode3Edit->addItem(tr("192.168.2.2"));
	NextNode3Edit->addItem(tr("192.168.2.3"));
	NextNode3Edit->addItem(tr("192.168.2.4"));
	NextNode3Edit->setMaxCount(4);
    NextNode3Edit->setEditable(true);
	NextNode3Edit->setInsertPolicy(QComboBox::InsertAfterCurrent);
	NextNode3Edit->setLineEdit(lineEdit13);
	NextNode3->setBuddy(NextNode3Edit);
	
	Saveseting3 = new QPushButton(this);//����·�����ð�ť
    Saveseting3->setText( tr( "����" ) );
    Saveseting3 ->setFixedSize(120,60);
	connect(Saveseting3, SIGNAL(clicked()), this, SLOT(Saverouteseting3()));	//����·������
	//*****************************************************************
    //����·�����ÿ�
    //***************************************************************** 	
    QGridLayout *mainLayout2 = new QGridLayout;//����
    //mainLayout2->addWidget(OrangenNode1, 0, 0,1,1);
	//mainLayout2->addWidget(OrangenNode1Edit, 0, 1,1,1);
	mainLayout2->addWidget(DestNode1, 0, 0,1,1);
	mainLayout2->addWidget(DestNode1Edit, 0, 1,1,2);
	mainLayout2->addWidget(NextNode1, 0, 3,1,1);
	mainLayout2->addWidget(NextNode1Edit, 0, 4,1,2);
	mainLayout2->addWidget(Saveseting1, 0, 6,1,1);
	//mainLayout2->addWidget(OrangenNode2, 1, 0,1,1);
	//mainLayout2->addWidget(OrangenNode2Edit, 1, 1,1,1);
	mainLayout2->addWidget(DestNode2, 1, 0,1,1);
	mainLayout2->addWidget(DestNode2Edit, 1, 1,1,2);
	mainLayout2->addWidget(NextNode2, 1, 3,1,1);
	mainLayout2->addWidget(NextNode2Edit, 1, 4,1,2);
	mainLayout2->addWidget(Saveseting2, 1, 6,1,1);
	//mainLayout2->addWidget(OrangenNode3, 2, 0,1,1);
	//mainLayout2->addWidget(OrangenNode3Edit, 2, 1,1,1);
	mainLayout2->addWidget(DestNode3, 2, 0,1,1);
	mainLayout2->addWidget(DestNode3Edit, 2, 1,1,2);
	mainLayout2->addWidget(NextNode3, 2, 3,1,1);
	mainLayout2->addWidget(NextNode3Edit, 2, 4,1,2);
	mainLayout2->addWidget(Saveseting3, 2, 6,1,1);
	count_send2->setLayout(mainLayout2);
	vbMain->addWidget(count_send2);
    //�������ڼ����Ӵ���	
	VSRbMain->addWidget(extension);
	//extension->setLayout(vbMain);
    //*****************************************************************
    //���ý��ս���
    //*****************************************************************
	//���沼��
	QVBoxLayout *vbrMain = new QVBoxLayout;
	extension_1->setLayout(vbrMain);
	VSRbMain->addWidget(extension_1);//�򴰿ڼ����Ӵ���extension_1
    //*****************************************************************
    //ʵ�ִ����л�����
    //*****************************************************************		
	extension_1->hide();
	connect(moreButton, SIGNAL(clicked()), this,SLOT(moreButton_state()));
    //*****************************************************************
    //�����հ���ʾ
    //*****************************************************************
	QLabel *receivepacket= new QLabel(this);
	receivepacket->setText(tr("�հ���ʾ:"));
	TextEditReceive = new QTextEdit( this);
	QVBoxLayout *receivemessage= new QVBoxLayout;
	receivemessage->addWidget(receivepacket );
	receivemessage->addWidget(TextEditReceive ); 
	vbrMain->addLayout(receivemessage);
    //*****************************************************************
    //�����հ���С
    //*****************************************************************
	QLabel *Packetsize2=new QLabel(this);
    Packetsize2->setText(tr("�հ���С:"));
    LineEditPacketsize2= new QLineEdit(this);
	LineEditPacketsize2->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //�����հ���Ŀ
    //*****************************************************************	
    QLabel *Packetnumber2= new QLabel(this);
    Packetnumber2->setText(tr("�հ���Ŀ:"));
	LineEditPacketnumber2= new QLineEdit(this);
	LineEditPacketnumber2->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //����������
    //*****************************************************************	
	QLabel *thruput=new QLabel(this);
    thruput->setText(tr("��  ��  ��:"));
	thruputE=new QLineEdit(this);
	thruputE->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //����ˢ������
    //*****************************************************************	
	QLabel *time=new QLabel(this);
	time->setText(tr("ˢ������:"));	
	gaptimeE=new QLineEdit(this);
    gaptimeE->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //��ȡ��ʾԴ��ַ
    //*****************************************************************  
    QLabel *LabelHostIP=new QLabel(this);
	LabelHostIP->setText(tr("Դ  ��  ַ:"));
    LineEditHostIP= new QLineEdit(this);
	LineEditHostIP->setAlignment(Qt::AlignHCenter);
    //*****************************************************************
    //���񲼾�
    //*****************************************************************		
	QGridLayout *mainLayouter = new QGridLayout;//����
	mainLayouter->addWidget(Packetsize2, 0, 0,1,1);
	mainLayouter->addWidget(LineEditPacketsize2, 0, 1,1,1); 
	mainLayouter->addWidget(Packetnumber2, 0, 2,1,1);
	mainLayouter->addWidget(LineEditPacketnumber2, 0, 3,1,1);
	mainLayouter->addWidget(thruput, 1, 0,1,1);
	mainLayouter->addWidget(thruputE, 1, 1,1,1);
    mainLayouter->addWidget(time, 1, 2,1,1);
    mainLayouter->addWidget(gaptimeE, 1, 3,1,1);
	mainLayouter->addWidget(LabelHostIP, 2, 0,1,1);
	mainLayouter->addWidget(LineEditHostIP, 2, 1,1,3);

	vbrMain->addLayout(mainLayouter);
    //*****************************************************************
    //�������ͼ�������Ϣ
    //*****************************************************************	
	QLabel *receive_status= new QLabel(this);
	receive_status->setText(tr("ͼ�������Ϣ:"));
    receive_outputEdit = new QTextEdit;   
    //*****************************************************************
    //���ò鿴ͼ��ť
    //*****************************************************************
	receive_PictureButton = new QPushButton( this);
    receive_PictureButton->setText( tr( "�鿴ͼ��" ) );  
	receive_PictureButton ->setFixedSize(160,60);
	connect(receive_PictureButton, SIGNAL(clicked()), this, SLOT(pictures()));	//���ô�����ȡͼƬ
	 //*****************************************************************
    //���ý�����Ƶ��ť
    //*****************************************************************
	receive_VideoButton = new QPushButton( this);
    receive_VideoButton->setText( tr( "������Ƶ" ) );  
	receive_VideoButton ->setFixedSize(160,60);
	connect(receive_VideoButton, SIGNAL(clicked()), this, SLOT(receiveVideos()));	//���ô���������Ƶ
    //*****************************************************************
    //����һ��
    //*****************************************************************
	QHBoxLayout* pictureandstatus = new QHBoxLayout;
	pictureandstatus->addWidget(receive_status); 
	pictureandstatus->addWidget(receive_PictureButton);
	pictureandstatus->addWidget(receive_VideoButton); 	
	QVBoxLayout* layout = new QVBoxLayout;      
	layout->addLayout(pictureandstatus);
	layout->addWidget( receive_outputEdit);
	vbrMain->addLayout( layout );
    //*****************************************************************
    //���ò��ִ��ڵײ�
    //*****************************************************************   
	setLayout(VSRbMain);
    //*****************************************************************
    //���÷��ͺͼ����˿�	
    //*****************************************************************		 
    port_send=5555;
	port_rece=5555;
	port_config=4000;
	port_serve=6800;
    //*****************************************************************
    //�����׽���	
    //*****************************************************************		
	udpSocket = new QUdpSocket(this); 
	udpSocket_config=new QUdpSocket(this);
	udpSocket_repicture=new QUdpSocket(this);
    //*****************************************************************
    //�󶨶˿�
    //*****************************************************************		
	udpSocket->bind(port_rece);
	udpSocket_config->bind(port_config);
	udpSocket_repicture->bind(port_serve);
    //*****************************************************************
    //ʵ�ָ���socket���ܲ�	
    //*****************************************************************		
	connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recv_slot()));
	connect(udpSocket_config,SIGNAL(readyRead()),this,SLOT(config()));
	connect(udpSocket_repicture,SIGNAL(readyRead()),this,SLOT(picturereceive()));
	pictureclear(); //�������ɾ������ͼƬ
    //*****************************************************************
    //��ȡ����IP	
    //*****************************************************************	
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach(QHostAddress address, list)
    {
     if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
          if(address.toString().contains("127.0."))
           {	 
		 continue;
           }
       	  str_IP=address.toString();
	    }
    }
}

/*********************************************************************
// ��������picture_Preventjitter()
// ��  �ܣ���Ϻ���pictures()��ɽ���鿴��ť����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::picture_Preventjitter()
{
	 Index=1;
}

/*********************************************************************
// ��������pictures()
// ��  �ܣ���ͼ�������
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::pictures()
{  
   if(Index==1)
   {
    Index=0;
   	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    MainWindow *udpclient2 = new MainWindow();	
    udpclient2->show();
	QTimer::singleShot(2*1000, this, SLOT(picture_Preventjitter()));//�������
	}
}

/*********************************************************************
// ��������~AeroNodeGeneral()
// ��  �ܣ�ȡ���׽���
// ��  �أ���
*********************************************************************/
AeroNodeGeneral::~AeroNodeGeneral()
{
  delete udpSocket;
  delete udpSocket_config;
  delete udpSocket_repicture;
} 

/*********************************************************************
// ��������Reseting()
// ��  �ܣ�����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::Reseting()
{
	udpSocket->close();
	udpSocket_config->close();
	udpSocket_repicture->close();
	//routecmd1->close();
	//routecmd2->close();
	//routecmd3->close();
	getpictures->exit();
	mythread->exit();
	on_btnHide_clicked();
	udpSocket->bind(port_rece);
	udpSocket_config->bind(port_config);
	udpSocket_repicture->bind(port_serve);
	pictureclear();
    //*****************************************************************
    //�����ؿռ�
    //*****************************************************************
    m_timerLCD->display("00:00:00");
	formatComboBox->setCurrentIndex(0);
	guideinformationComboBox->setCurrentIndex(0);
	timer_spanComboBox->setCurrentIndex(0);
	IPComboBox->setCurrentIndex(0);
	LineEditPacketnumber->clear();
	LineEditPacketsize->clear();
	outputEdit->clear();
	//OrangenNode1Edit->setCurrentIndex(0);
	//OrangenNode2Edit->setCurrentIndex(0);
	//OrangenNode3Edit->setCurrentIndex(0);
	DestNode1Edit->setCurrentIndex(0);
	DestNode2Edit->setCurrentIndex(0);
	DestNode3Edit->setCurrentIndex(0);
	NextNode1Edit->setCurrentIndex(0);
	NextNode2Edit->setCurrentIndex(0);
	NextNode3Edit->setCurrentIndex(0);
	TextEditReceive->clear();
    LineEditPacketsize2->clear();
	LineEditPacketnumber2->clear();
	thruputE->clear();
    gaptimeE->clear();
    LineEditHostIP->clear();
	receive_outputEdit->clear();	
    //*****************************************************************
    //��ʼ������	
    //*****************************************************************	
	Num_rece_packets=1;//ͳ�ƽ������ݰ���Ŀ
	timetofirst=0;//��ʱͳ����
	throughtput=0;//ͳ��������
    send_pcket_1=0;//�����հ���b�ַ�ʱ��һֱ����������send_pcket_1=1ʱ������ʱ��
	Length=0;//���յ������ݰ��ĳ�ʼ����
	Index=1;
    num=1;
	isStarted1=0;
	receivephotonum=0;
	fristbuf=NULL;//�洢����ͼƬ�Ľṹ��
}

void AeroNodeGeneral::Saverouteseting1()
{
    printf("enter saverouteseting1\n");
	QString desthop1=DestNode1Edit->currentText();
	QString nexthop1=NextNode1Edit->currentText();
	QString command1="rroute";
    routecmd1 = new QProcess;
    routecmd1->start(command1,QStringList()<<desthop1<<nexthop1); 
}

void AeroNodeGeneral::Saverouteseting2()
{
    printf("enter saverouteseting2\n");
	QString desthop2=DestNode2Edit->currentText();
	QString nexthop2=NextNode2Edit->currentText();
	QString command2="rroute";
    routecmd2 = new QProcess; 
    routecmd2->start(command2,QStringList()<<desthop2<<nexthop2); 
}

void AeroNodeGeneral::Saverouteseting3()
{
    printf("enter saverouteseting3\n");
	QString desthop3=DestNode3Edit->currentText();
	QString nexthop3=NextNode3Edit->currentText();
	QString command3="rroute";
    routecmd3 = new QProcess; 
    routecmd3->start(command3,QStringList()<<desthop3<<nexthop3); 
}

/*********************************************************************
// ��������PushButtonClose_clicked()
// ��  �ܣ��˳����棬�ر��׽��ֺ�ͼƬ���Զ����湦��
// ��  �أ���
*********************************************************************/                                                                              
void AeroNodeGeneral::PushButtonClose_clicked()                     
{          
	udpSocket->close();
	udpSocket_config->close();
	udpSocket_repicture->close();
	//routecmd1->close();
	//routecmd2->close();
	//routecmd3->close();
	pictureclear();
	on_btnHide_clicked();
	getpictures->exit();
	close();
}

/*********************************************************************
// ��������pictureclear()
// ��  �ܣ�ɾ��������Ƭ
// ��  �أ���
*********************************************************************/ 
void AeroNodeGeneral::pictureclear()
{
    QDir d("/home/photos");
	QFile file;
	foreach(QFileInfo mItem,d.entryInfoList())
	{
	    if(mItem.suffix()=="JPEG")
		    file.remove(mItem.filePath());
	}
} 
/*********************************************************************
// ��������moreButton_state
// ��  �ܣ��л����ڰ�ť��״̬����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::moreButton_state()                     
{   QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));	
	if(!isStarted)
	{  
	   moreButton->setText(tr("����"));
	   Title->setText(tr("                      ���ս���                            "));
	   extension_1->show();
	   extension->hide();
	   isStarted = true;
	}
	else
	{
	    moreButton->setText(tr("����"));
		Title->setText(tr("                    ���ͽ���                         "));
		isStarted = false;
		extension->show();
		extension_1->hide();
	}
}

/*********************************************************************
// ��������recv_slot()
// ��  �ܣ�ͳ�ƽ��յ����ݰ��������Ϣ
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::recv_slot()
{    
   	//��ַ���˿ڵȲ���ʵ����
    QByteArray datagram;
	QHostAddress addr;
    quint16 port_source;
	QString NS;	
	char recvBuf[600];	 
    while (udpSocket->hasPendingDatagrams())
    {
         memset(recvBuf,0,600);	
         udpSocket->readDatagram((char*)&recvBuf,sizeof(recvBuf),&addr, &port_source);
    
	     QByteArray byte;
         byte=QByteArray(recvBuf);
	     Length+=byte.size();
	     QString msgsize=QString::number(byte.size());
	     LineEditPacketsize2->setText(msgsize+"bytes" );//��ʾ�հ���С
         QString str;
	     str=addr.toString();
	     LineEditHostIP->setText(str);   //��ȡ�հ�Դ��ַ
	     QTime ReceTime=QTime::currentTime();	
	     //��¼��һ�ν��հ���ʱ��
	     if( Num_rece_packets==1)
	     {
	     	frTime=QTime::currentTime();
	     }	
	     if(Num_rece_packets>=2)
	     {
	       QString SPAN_Each=QString::number(last_ReceTime.msecsTo(ReceTime));//���뼶.msecsTo���ص�ǰʱ�����last_ReceTime��ReceTime��ָ����ʱ��ΪReceTime-last_ReceTime
	       gaptimeE->setText( SPAN_Each +"ms");
	     }
	     last_ReceTime=ReceTime;	
	     NS=QString::number(Num_rece_packets);	
	     TextEditReceive->append(NS+"    "+"\n"+recvBuf);	
	     LineEditPacketnumber2->setText(NS);		
	     if (Num_rece_packets>=2)//ͳ��������
         {
	     	 timetofirst=frTime.msecsTo(ReceTime); //���뼶��,���������ݰ�������ǰ���յ�����ʱ��
	     	 throughtput=(double)(Length*8000)/(double)timetofirst;
	     	 QString thput=QString::number( throughtput );
	     	 thruputE->setText(thput+"bps");
	     }	
         ++Num_rece_packets;//ͳ�ƽ������ݰ���
	}
}

/*********************************************************************
// ��������timeout
// ��  �ܣ���ʱ��ʱʱ����ʼ�������ݰ�
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::timeout() 
{  
  QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));  
    QString msg=guideinformationComboBox->currentText();;//���÷������ݰ�����(������Ϣ���ֶ�����)
	int L=0;//�������ݰ��Ĵ�С
	QByteArray msg_QByteArray = msg.toLatin1();    
    L=msg_QByteArray.size();
	char *msg_char;
	msg_char=msg_QByteArray.data();		
	QString Ipmsg = IPComboBox->currentText();//����Ŀ���ַ
	hostaddr1 = new QHostAddress(Ipmsg);	
    QTime sendtime=QTime::currentTime();//���÷���ʱ��
	QString sendtime_string=sendtime.toString();
	char *sendtime_char;
	QByteArray sendtime_QByteArray=sendtime_string.toLatin1();
	sendtime_char=sendtime_QByteArray.data();
    QString LS=QString::number(L);		//��ʾ���ݴ�С
	LineEditPacketsize->setText( LS+"  bytes" );		
	udpSocket->writeDatagram(msg_char,L,*hostaddr1, port_send);	
	QString NS=QString::number(num);//��ʾ�������ݰ�����Ŀ
	LineEditPacketnumber->setText(NS);	
	++num; 	 
}

/*********************************************************************
// ��������SLOT_Deal_guideinfo
// ��  �ܣ�����ײ㷢��Ҫ������������Ϣ
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::Deal_guideinfo()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
	  specified_timer_span = timer_spanComboBox->currentText();//�ж��Ƿ�ָ���˷�����������������
	      if(specified_timer_span == "")
	      {
		    QMessageBox::information(this,tr("����"),tr("����뷢�������"));
		    return;	
	      }	  
	      //���÷�������
          QString msg=guideinformationComboBox->currentText();;//���÷������ݰ�����(������Ϣ���ֶ�����)
	      int L=0;//�������ݰ��Ĵ�С	  
	      if(msg == "")//�����������Ϊ�գ���ӽ����ֶ����뷢����Ϣ�����򷵻�
	      {
	        QMessageBox::information(this,tr("����"),tr("����뷢����Ϣ��"));
		    return;	
	      }
		  	 if(formatComboBox->currentText() == "")
	      {
	       QMessageBox::information(this,tr("����"),tr("��������ʱ����"));
	         return;	
	       }
	   
        if(IPComboBox->currentText()!="")
        {
          QByteArray msg_QByteArray = msg.toLatin1();    
          L=msg_QByteArray.size();
	      char *msg_char;
	      msg_char=msg_QByteArray.data();		
	      QString Ipmsg = IPComboBox->currentText();//����Ŀ���ַ
	    
	      hostaddr1 = new QHostAddress(Ipmsg);	
          QTime sendtime=QTime::currentTime();//���÷���ʱ��
	      QString sendtime_string=sendtime.toString();
	      char *sendtime_char;
	      QByteArray sendtime_QByteArray=sendtime_string.toLatin1();
	      sendtime_char=sendtime_QByteArray.data();
	   
	      QString LS=QString::number(L);		//��ʾ���ݴ�С
	      LineEditPacketsize->setText( LS+"  bytes" );	
	      udpSocket->writeDatagram(msg_char,L,*hostaddr1, port_send);	
	      QString NS=QString::number(num);
		  LineEditPacketnumber->setText(NS);
	      ++num; 	 
	      mythread->start();
	    }
		else mythread->start();
}

/*********************************************************************
// ��������config
// ��  �ܣ�ͳһ���ò���
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::config()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));	
	//���ٻ���������ݰ�
    char recvBuf[1000];
    char *k;
    k=recvBuf;
	 while (udpSocket_config->hasPendingDatagrams())
        { 
		 memset(recvBuf,0,1000);
         udpSocket_config->readDatagram((char*)&recvBuf,sizeof(recvBuf));
        }
    char *addressIp=strtok(recvBuf," ");
	char *str_char;
	QByteArray str_QByteArray=str_IP.toLatin1();
	str_char=str_QByteArray.data();
    if(strcmp(addressIp,str_char)==0)
    {
    char *format=strtok(NULL," ");
	formatComboBox->setEditText(format);//����ʱ��
	char *IP_destination=strtok(NULL," ");
	IPComboBox->setEditText(IP_destination);//Ŀ���ַ
	char *timer_span=strtok(NULL," ");
	timer_spanComboBox->setEditText(timer_span);//�������
	char *LineEdit=strtok(NULL,"\n");
	guideinformationComboBox->setEditText(LineEdit);   
    }   
	else if(strcmp(addressIp,str_char)!=0)
	{
	   Reseting();
	}
}

/*********************************************************************
// ��������on_btnShow_clicked()
// ��  �ܣ���ʾ�����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::on_btnShow_clicked()
{
	virtualKeyBoard->show();
}
/*********************************************************************
// ��������on_btnHide_clicked()
// ��  �ܣ��ر������
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::on_btnHide_clicked()
{    
	virtualKeyBoard->hide();
}

/*********************************************************************
// ��������readOutput
// ��  �ܣ���ʾ���մ�ӡ��Ϣ
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::readOutput(QString output)   
{   
    outputEdit->setText(output);
	getpictures->exit();
	isStarted1=0;
}


/*********************************************************************
// ��������timerStrDisplay
// ��  �ܣ����߳���ʾʱ��
// ��  �أ���
*********************************************************************/ 
void AeroNodeGeneral::timerStrDisplay(QString str)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    QString  m_hour_str=str.mid(0,2);
    QString  m_minute_str=str.mid(2,2);
    QString  m_second_str=str.mid(4,2);
    QString  m_msecond_str=str.mid(6,1);
    m_hour=m_hour_str.toInt();
    m_minute=m_minute_str.toInt();
    m_second=m_second_str.toInt();
    m_msecond=m_msecond_str.toInt();
	QString  all_msecond_str=str.mid(7,5);
    QString  all_second_str=str.mid(12,4);
    all_msecond=all_msecond_str.toInt();
    all_second=all_second_str.toInt();
	specified_timer_span = timer_spanComboBox->currentText();
    time_span=specified_timer_span.toInt();
    simulationtime=(formatComboBox->currentText()).toInt();
    
   //���������ļ���������������ʱ�䣬ȷ����ز���
   if(all_second>=simulationtime)
   {
      mythread->exit(); 
      m_msecond=0;  
      IPComboBox->setCurrentIndex(0);;
   }
   else if(all_second<simulationtime&&IPComboBox->currentText()!="")
   {
        if((all_msecond*100)%time_span==0&&isStarted1==0)
        {
	      timeout();		
		}
   }
   QString strLCD;
   strLCD.sprintf("%.2d:%.2d:%.2d",m_hour,m_minute,m_second);
   m_timerLCD->display(strLCD);    
}

/*********************************************************************
// ��������run
// ��  �ܣ�������ʱ���߳�
// ��  �أ���
*********************************************************************/
void MyThread_General::run()
{
	m_second=0;
    m_minute=0;
    m_hour=0;
	m_msecond=0;
	all_msecond=0;
	all_second=0;	
	m_timer = new QTimer;  
    m_timer->setInterval(100); 
	connect(m_timer,SIGNAL(timeout()),this,SLOT(timerStrDisplay()));	
    QTimer::singleShot(0, m_timer,SLOT(start()));
    exec(); 
}

/*********************************************************************
// ��������timerStrDisplay
// ��  �ܣ���ʱ����¼ʱ�亯��
// ��  �أ���
*********************************************************************/
void MyThread_General::timerStrDisplay()
{   
  ++all_msecond;  
  if(++m_msecond==10)
 {
   m_msecond=0;
   ++all_second;
   if(++m_second==60)
   {        
        m_second=0;		
        if(++m_minute==60)
        {
            m_minute=0;
            ++m_hour;
        }
   }
  }
   QString str;
   str.sprintf("%.2d%.2d%.2d%.1d%.5d%.4d",m_hour,m_minute,m_second,m_msecond,all_msecond,all_second);
   emit changeText(str);
}
/*********************************************************************
// ��������getpictures_Preventjitter
// ��  �ܣ��̣߳�����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::getpictures_Preventjitter()   
{  
    if(isStarted1=0)
	{
    isStarted1=1;
	}
	runClicked();
}
/*********************************************************************
// ��������runClicked
// ��  �ܣ��̣߳��̴߳���ͼƬ����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::runClicked()   
{  
 	getpictures->exit();
	//paramter=IPComboBox->currentText();
	getpictures->start();	 
}
 
/*********************************************************************
// ��������run
// ��  �ܣ����������̺߳���
// ��  �أ���
*********************************************************************/
void GetPictures_General::run()
{      
    int i;
	//i=v4l2_camera();
	if(i==1)
	{
        //��⵱ǰ��Ƭ���
	    long i=1;
	    QString i_string=QString::number(i,10);
	    QString path;
	    QString path1="/home/photos/pic";
	    QString path2="from"+str_IP+".JPEG"; 
	    path=path1+i_string+path2;
	    QFile file;
	    bool yn;
	    yn=file.exists(path);
        while(yn)
        {  
	        i++;
	    	i_string=QString::number(i,10);
	     	path=path1+i_string+path2;
	        yn=file.exists(path);
	    }
	    //�ƶ�����Ƭ
	    QString source="/home/temp/p2.JPEG";
    	yn=QFile::copy(source,path);
    	//������Ƭ
    	//send(path);	
    	//ɾ����Ƭ
    	file.remove("/home/temp/p0.JPEG");
    	file.remove("/home/temp/p1.JPEG");
	    file.remove("/home/temp/p2.JPEG");
	    file.remove("/home/temp/p3.JPEG");
	    if(yn)
	    {
	        QString output;
	        QString output1;
	        QString output2;
	        output1="Photo ";
	        output2=" is successfully got.";
	        output=output1+i_string+output2;
	        emit Textchange(output);
	    }
	}
	else
	{
	    QString output; 
		output="No video device.\n";
		emit Textchange(output);
	}
	exec();  
}
/*********************************************************************
// ��������read_frame
// ��  �ܣ���ȡ����ͼ��
// ��  �أ��ɹ�����1
*********************************************************************/
//int GetPictures_General::read_frame (FILE *k)
//{
//    struct v4l2_buffer buf;                        //����ʢ��һ֡���ݣ�v4l2_buffer��v4l2���еĽṹ��
//   memset (&(buf), 0, sizeof (buf));              //���buf����ṹ�������
//    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;        //�̶�
//    buf.memory = V4L2_MEMORY_MMAP;                 //�ļ����䷽ʽ��read��mmap��userptr����
//    int ff = ioctl (fd, VIDIOC_DQBUF, &buf);   
//    //����Ƶ�����������������ȡ��һ���Ѿ�������һ֡��Ƶ���ݵ���Ƶ������,����������˻������ı��index����������Ϣ�������buffers�ṹ�������buf.index�������˻������������ַ���׵�ַ��
//    if(ff<0)
//    {
//        printf("failture\n");                          //���вɼ���֡���� 
//        return 0;
//    }		
//    assert (buf.index < n_buffers);                //�ж�buf.index�Ƿ�С��n_buffers
//    printf ("buf.index dq is %d\n",buf.index);

//    fwrite(buffers[buf.index].start, buffers[buf.index].length,1,k); 
    //����д���ļ��У���buffers[buf.index].start��ʼ��buffers[buf.index].length��������д��file_fd��

//    ff=ioctl (fd, VIDIOC_QBUF, &buf); //�ٽ������У�Ͷ��һ���յ���Ƶ����������Ƶ��������������У��Ա���һ�εĲɼ�����
//    if(ff<0)
//    {
//	    printf("failture VIDIOC_QBUF\n");
//		return 0;
//	}
//    printf ("buffers[%d].start is %ld\n",buf.index,buffers[buf.index].start);
//    return 1;

//}

/*********************************************************************
// �������� v4l2_camera
// ��  �ܣ�����
// ��  �أ���
*********************************************************************/
//int GetPictures_General::v4l2_camera()
//{

 //   struct v4l2_capability cap; //Ӳ������ͷ�������ܹ��ṩ�Ĺ��ܶ�����������ṹ������
 //   struct v4l2_format fmt;   //���������ͼ�����Ϣ��ÿ֡ͼ������أ��ֱ��ʵ���Ϣ
 //   unsigned int i;
 //   int j,m;

 //   FILE *file_fd[4]={NULL,NULL,NULL,NULL};   //ͼ��Ҫ�����浽��Ŀ���ļ������������ļ�����FILE
//	char *picture[4]={"/home/temp/p0.JPEG","/home/temp/p1.JPEG","/home/temp/p2.JPEG","/home/temp/p3.JPEG"};//zyj
	
 //   enum v4l2_buf_type type;   //����������V4L2_BUF_TYPE_VIDEO_CAPTURE,V4L2_BUF_TYPE_VIDEO_OUTPUT,V4L2_BUF_TYPE_VIDEO_OVERLAY

//    fd = open(dev_name, O_RDWR  | O_NONBLOCK, 0);//������ͷ�豸����������ʽ��
//
//    int ff=ioctl (fd, VIDIOC_QUERYCAP, &cap);//��ȡ����ͷ���������ұ�����cap��
 //   if(ff<0)
//	{    
//	    printf("failture VIDIOC_QUERYCAP\n");
//		return 0;
//	}

    //�������ͼ����Ϣ	
//    memset (&(fmt), 0, sizeof (fmt));  
//    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    fmt.fmt.pix.width       = 640;
//    fmt.fmt.pix.height      = 480;
//    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;//V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV;
//    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
//   ff = ioctl (fd, VIDIOC_S_FMT, &fmt); //����ͼ���ʽ
//    if(ff<0)
//    {
//        printf("failture VIDIOC_S_FMT\n");
//		return 0;
//	}
		
    //���뻺�棬count������Ļ��������
//    struct v4l2_requestbuffers req;
//    memset (&(req), 0, sizeof (req));  
//    req.count               = 4;
//    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
///    req.memory              = V4L2_MEMORY_MMAP;
//    ff=ioctl (fd, VIDIOC_REQBUFS, &req); 
//    if(ff<0)
 //   {
 ///       printf("failture VIDIOC_REQBUFS\n");
//		return 0;
//	}
//    if (req.count < 2)//���������������2������ӡ������������
//        printf("Insufficient buffer memory\n");
//    buffers = (buffer*)calloc (req.count, sizeof (*buffers));//�ڴ��н�����Ӧ�ռ䣬���ڴ��з���req.count����СΪsizeof(*buffers)�ĸ��˿ռ�
//    printf("the number of memory is %d\n",req.count);//��ӡ��������
//
 //   for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
 //   {
//        //�����е�һ֡�����һ֡���ݣ�ÿ�������һ֡
///        struct v4l2_buffer buf;   
//        memset (&(buf), 0, sizeof (buf));   
//        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//        buf.memory      = V4L2_MEMORY_MMAP;
//        buf.index       = n_buffers;
//        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))//ӳ���û��ռ䣬��REQBUFS����Ļ���ӳ��Ϊ�����ַ�����Բ�����
//        {
 //   		printf ("VIDIOC_QUERYBUF error\n");
//			return 0;
//	    }
//        buffers[n_buffers].length = buf.length;
//        buffers[n_buffers].start = (long unsigned int*)mmap (NULL,buf.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd, buf.m.offset);//�����ַӳ��Ϊ�����ַ
//        //ͨ��mmap����ӳ���ϵ��NULL�� /* start anywhere */��PROT_READ | PROT_WRITE ��/* required */��
//        if (MAP_FAILED == buffers[n_buffers].start)
///        {
//    		printf ("mmap failed\n");
//			return 0;
//		}
//    }
///
//    for (i = 0; i < n_buffers; ++i)
//    {
//
//        struct v4l2_buffer buf;
//        memset (&(buf), 0, sizeof (buf));   
//        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//        buf.memory      = V4L2_MEMORY_MMAP;
//        buf.index       = i;
//        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))  //���뵽�Ļ�������ж�
//        {
//    		printf ("VIDIOC_QBUF failed\n");
//			return 0;
//		}
//        else
//		    printf("VIDIOC_QBUF %d success.\n",i);
//   }       
//    
//    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //��ʼ��׽ͼ������
//   {
//        printf ("VIDIOC_STREAMON failed\n");
//		return 0;
//	}
//    for (j=0;j<req.count;++j)      //��һ���漰���첽IO
//    {
//        fd_set fds;
//        struct timeval tv;
//        int r;
//        FD_ZERO (&fds);//��ָ�����ļ������������
//        FD_SET (fd, &fds);//���ļ�����������������һ���µ��ļ�������
//    /* Timeout. */
//        tv.tv_sec = 1;
//       tv.tv_usec = 0;
//        r = select (fd + 1, &fds, NULL, NULL, NULL);//�ж��Ƿ�ɶ���������ͷ�Ƿ�׼���ã���tv�Ƕ�ʱ
//
//        if (-1 == r) 
//		{
//            if (EINTR == errno)
//                continue;
//                   printf ("select err\n");
//        }
//        if (0 == r)
//		{
//            fprintf (stderr, "select timeout\n");
//            exit (EXIT_FAILURE);
//        }
//        file_fd[j] = fopen(picture[j], "w");//��ֻд��ʽ���ļ�
//        printf("n_buffers=%d\n",n_buffers);  
//        if (read_frame (file_fd[j])==1)
//            printf("picture(%d)...\n",j);   //����ɶ���ִ��read_frame ()������������ѭ��
        //   printf("n_buffers=%d\n",n_buffers);
        //   break;
//    }
//    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
//    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))  
//   {
//        printf("VIDIOC_STREAMOFF");
//		return 0;
//	}
//
//    for(m=0;m<n_buffers;m++)
//	{
//        fclose(file_fd[m]);
//        printf("fclose file_fd[%d]...\n",m);
//    } 
//    for (i = 0; i <n_buffers; ++i)
//	{
//        if (-1 == munmap (buffers[i].start, buffers[i].length))
//            printf ("munmap error...\n");
//        printf("�ͷŻ�����%d����...\n",i);
//    }
//
//    close (fd);
//    return 1;
//}
/*********************************************************************
// �������� send
// ��  �ܣ�����ͼƬ
// ��  �أ���
*********************************************************************/
//void GetPictures_General::send(QString path)
//{
//   	port_sendpicture=6800;
//	udpSocket_sendpicture = new QUdpSocket(this); 
//	udpSocket_sendpicture->bind(port_sendpicture);
//	hostaddr_picture = new QHostAddress(paramter);//����Ŀ��IP��
//	QByteArray ba = path.toLatin1();
//    char *pa = ba.data();
//	char a[100];
//    strcpy(a,pa);
//    char *T,*N;
//    char buf1[10240]={0};
//    char buf2[50]={0};
//    struct stat buf;
//    int err = stat(pa,&buf);
//   if(err == -1)
//	{
//        int errsv = errno;
//        printf("errsv %d\n", errsv);
//    }
//    printf("buf.st_size %d\n",buf.st_size);

//    FILE *picfd=fopen(pa,"rb");
//    T = strtok(a,"/");
//    while(T != NULL)
//	{
//        N = T;
//        T = strtok(NULL,"/");
//    }
   
//    buf2[0] = 'Z';
//    int i;
//    for(i = 1; (i<48) && (*(N+i-1) != '\0'); i++)
//        buf2[i] = *(N+i-1);
//    buf2[i] = '\0';
    //sprintf(&buf2[262],"%d",buf.st_size);
    //buf2[271]= '\0';
//    udpSocket_sendpicture->writeDatagram(buf2,50,*hostaddr_picture,6800);//�����ļ���
//	sleep(1);
//    int n = buf.st_size/10235;
//    int m = buf.st_size%10235;
//	for(i = 0; i<n; i++)
//    {
//        buf1[0]='M';
//       int rread = fread(&buf1[5],1,10235,picfd);
//        printf("rread %d\n",rread);
//        udpSocket_sendpicture->writeDatagram(buf1,10240,*hostaddr_picture,6800);      
//    }	
//	sleep(2);
//   	buf1[0] ='N';
//  	sprintf(&buf1[1],"%d",m);
//   	int rread = fread(&buf1[5],1,m,picfd);//m=0�����Σ�
//	udpSocket_sendpicture->writeDatagram(buf1,10240,*hostaddr_picture,6800);
//    printf("the last buf send\n");	
//    fclose(picfd); 
//    udpSocket_sendpicture->close();
//}
/*********************************************************************
// �������� picturereceive
// ��  �ܣ� ����ͼƬ
// ��  �أ� ��
*********************************************************************/
void AeroNodeGeneral::picturereceive()
{
	printf("enter picture receive\n");
	QHostAddress addr;
    quint16 port;
	char recvBuf[10240]={0};
	char buf2[50]={'/','h','o','m','e','/','p','h','o','t','o','s','/'};
	int m;
	while (udpSocket_repicture->hasPendingDatagrams())//�ж��Ƿ������ݰ�����
    { 
		memset(recvBuf,0,10240);
        udpSocket_repicture->readDatagram((char*)&recvBuf ,sizeof(recvBuf),&addr, &port);
		struct receivebuf *T=fristbuf;
		struct receivebuf *S=NULL;		
		if(recvBuf[0]=='Z')
		{
		    printf("the frist buf\n");
		    sprintf(&buf2[13],"%s",&recvBuf[1]);
		    if(!fristbuf)
		    {
		        fristbuf=new struct receivebuf; 
			    fristbuf->source_addr=addr;
			    fristbuf->source_port=port;
			    fristbuf->next=NULL;
			    T=fristbuf;
		    }
		    else
		    {
		        while(T!=NULL)
			    {
			        if(T->source_addr==addr)
		        		break;
			        else
			    	{
     		    		S=T;
			    	    T=T->next;
			    	}
			    }
			    if(T==NULL)
			    {
			    	T=new struct receivebuf; 
			    	T->source_addr=addr;
			        T->source_port=port;
			        T->next=NULL;
			    	S->next=T;
			    }
		    }
            T->picfd=fopen(buf2,"wb");	
		}
		else if(recvBuf[0]=='M')
		{
			printf("the middle buf\n");
			
			if(!fristbuf)
		    {
		        printf("first buf=NULL\n");
				fristbuf=new struct receivebuf; 
			    fristbuf->source_addr=addr;
			    fristbuf->source_port=port;
			    fristbuf->next=NULL;
				fristbuf->picfd=NULL;
			    T=fristbuf;
		    }
		    else
		    {
		        printf("first buf!=NULL\n");
				while(T!=NULL)
			    {
			        printf("T!=NULL\n");
					if(T->source_addr==addr)
		        		break;
			        else
			    	{
     		    		printf("CONTINUE FIND T\n");
						S=T;
			    	    T=T->next;
			    	}
			    }
			    if(T==NULL)
			    {
			    	printf("T==NULL\n");
					T=new struct receivebuf; 
			    	T->source_addr=addr;
			        T->source_port=port;
			        T->next=NULL;
					T->picfd=NULL;
			    	S->next=T;
			    }
		    }
			if(T->picfd!=NULL)
            {
     			printf("T->picfd!=NULL,FWRITE\n");
				fwrite(&recvBuf[5],1,10235,T->picfd);
				
			}
		}
		else if(recvBuf[0]=='N')
		{
		    printf("the last buf\n");
			while(T!=NULL)
			{
			    if(T->source_addr==addr)
				    break;
				else 
				    T=T->next;
			}
			if(T!=NULL)
			{
			    if(T->picfd!=NULL)
				{
			        sscanf(&recvBuf[1],"%d",&m);
        	        fwrite(&recvBuf[5],1,m,T->picfd);
        	        fclose(T->picfd);
			        //�жϽ��յ�ͼƬ��
			        receivephotonum++;
			        receive_output="Photo "+QString::number(receivephotonum,10)+" is successfully received.";
			        receive_outputEdit->append(receive_output); 
				}
				else
				    printf("not receive first buf.\n");
			}
			else
			    printf("first and middle buf not received.\n");
		}
    }
} 

/*********************************************************************
// ��������receiveVideos()
// ��  �ܣ�����Ƶ�����
// ��  �أ���
*********************************************************************/
void AeroNodeGeneral::receiveVideos()  
{  
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    VideoDisplay *videodisp = new VideoDisplay();
    videodisp->show();
}