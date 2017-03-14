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
**文件名：AeroNodeGeneral.cpp
**功能：AeroNodeGeneral类定义了高效并发节点的发送与接收界面，同时还定义
        了与之配套的定时器线程与牌照线程。 
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
//static char *dev_name = "/dev/video15";//摄像头设备名
//static int   fd = -1;   //文件描述符fd
//static struct buffer *buffers = NULL;
//static unsigned int  n_buffers = 0;
/*********************************************************************
// 函数名：AeroNodeGeneral
// 功  能：构造函数
// 返  回：无
*********************************************************************/
AeroNodeGeneral::AeroNodeGeneral( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//设置字体编码
    //*****************************************************************
    //设置计时器线程与拍照线程	
    //*****************************************************************
	mythread = new MyThread_General;
	getpictures=new GetPictures_General;
	connect(getpictures,SIGNAL(Textchange(QString)),this,SLOT(readOutput(QString)),Qt::QueuedConnection);
	connect(mythread,SIGNAL(changeText(QString)),this,SLOT(timerStrDisplay(QString)),Qt::QueuedConnection);
    //*****************************************************************
    //设置整个收发界面底层	
    //*****************************************************************
	QVBoxLayout *VSRbMain = new QVBoxLayout( this );	
	extension = new QWidget;
	extension_1 = new QWidget;
    //*****************************************************************
    //初始化参数	
    //*****************************************************************
	Num_rece_packets=1;//统计接收数据包数目
	timetofirst=0;//延时统计量
	throughtput=0;//统计吞吐量
    send_pcket_1=0;//当接收包非b字符时就一直发包，即当send_pcket_1=1时开启定时器
	Length=0;//接收到的数据包的初始长度
	Index=1;
    num=1;
	isStarted=false;
	isStarted1=0;
	receivephotonum=0;
	fristbuf=NULL;//存储接收图片的结构体
    //*****************************************************************
    //设置仿真时间
    //*****************************************************************
	QLabel *LCD=new QLabel(this);
	LCD->setText(tr("运行时间:"));
	m_timerLCD=new QLCDNumber;
  	m_timerLCD->setFrameStyle ( QLCDNumber::NoFrame);
	m_timerLCD->setFixedSize(280,60);
    m_timerLCD->setNumDigits(8);
    m_timerLCD->display("00:00:00");
	m_timerLCD->setSegmentStyle(QLCDNumber::Flat);
    //*****************************************************************
    //设置重置、接收\发送、关闭按钮	
    //*****************************************************************	
	StartButton = new QPushButton( this);
    StartButton->setText( tr( "开始" ) ); 
	StartButton ->setFixedSize(120,60);
	connect(StartButton,SIGNAL(clicked()),this,SLOT(Deal_guideinfo()));
	ResetButton = new QPushButton( this);
    ResetButton->setText( tr( "重置" ) ); 
	ResetButton ->setFixedSize(120,60);
	connect(ResetButton,SIGNAL(clicked()),this,SLOT(Reseting()));
	moreButton = new QPushButton( QIcon(":/images/interface.png"),tr("接收"),this);
	moreButton->setFixedSize( 160,60);
    PushButtonClose_R = new QPushButton( this);
    PushButtonClose_R->setText( tr( "关闭" ) ); 
	PushButtonClose_R ->setFixedSize(120,60);
	connect(PushButtonClose_R,SIGNAL(clicked()),this,SLOT(PushButtonClose_clicked()));
    //*****************************************************************
    //设置抬头	
    //*****************************************************************	
	Title=new QLabel(this);
	Title->setText(tr("                    发送界面                         "));
	Title->setAlignment(Qt::AlignHCenter);
    //*****************************************************************
    //布局第一行	
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
    //设置发送界面	
    //*****************************************************************
	QVBoxLayout *vbMain = new QVBoxLayout;
	extension->setLayout(vbMain);
	 //*****************************************************************
    //设置页面布局业务参数部分
    //***************************************************************** 	
	QGroupBox *count_send1= new QGroupBox(tr("业务参数"));
    //*****************************************************************
    //设置仿真时长输入框
    //***************************************************************** 	
    QLabel *simulation=new QLabel(this);
	simulation->setText(tr("运行时长:"));
    QLineEdit *lineEdit1 = new QLineEdit;//居中
    lineEdit1->setAlignment(Qt::AlignCenter);	
    formatComboBox = new QComboBox;
	formatComboBox->addItem(tr("80"));
    formatComboBox->addItem(tr("120"));
	formatComboBox->setEditable(true);//可编辑
	formatComboBox->setMaxCount(2);//最多两个选项
	formatComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	formatComboBox->setLineEdit(lineEdit1); 
	simulation->setBuddy(formatComboBox);
	//*****************************************************************
    //设置发包内容输入框
    //***************************************************************** 
	QLabel *guideinformation=new QLabel(this);	
	guideinformation->setText(tr("发包内容:"));	
    QLineEdit *lineEdit2 = new QLineEdit;//居中
    lineEdit2->setAlignment(Qt::AlignCenter);
	guideinformationComboBox = new QComboBox;
    guideinformationComboBox->addItem(tr("01010"));
	guideinformationComboBox->setEditable(true);
	guideinformationComboBox->setMaxCount(1);
	guideinformationComboBox->setInsertPolicy(QComboBox::InsertAfterCurrent);
	guideinformationComboBox->setLineEdit(lineEdit2);
	guideinformation->setBuddy(guideinformationComboBox);
	//*****************************************************************
    //设置软键盘
    //***************************************************************** 
	virtualKeyBoard = new WidgetKeyboard(this);//创建软键盘
	OPenkeyboard = new QPushButton( QIcon(":/images/keyboard.png"),tr(""),this);
	OPenkeyboard->setFixedSize(200,120);	
	OPenkeyboard->setStyleSheet("QPushButton {border-style:outset;border-color: beige;font: bold 12px;min-width: 2.6em;padding: 6px;}");
    connect(OPenkeyboard, SIGNAL(clicked()), this, SLOT(on_btnShow_clicked()));	//链接
	//*****************************************************************
    //设置发包间隔输入框
    //*****************************************************************
	QLabel *Timegap=new QLabel(this);
	Timegap->setText(tr("发包间隔:"));
    QLineEdit *lineEdit3 = new QLineEdit;//居中
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
    //设置目标地址
    //***************************************************************** 	
	QLabel *LabelPeerIP=new QLabel(this);
	LabelPeerIP->setText(tr("目标地址:"));
    QLineEdit *lineEdit4 = new QLineEdit;//居中
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
    //设置发包数目
    //***************************************************************** 	
    QLabel *Packetnumber= new QLabel(this);
	Packetnumber->setText(tr("发包数目:"));
	LineEditPacketnumber= new QLineEdit(this);
	LineEditPacketnumber->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //设置发包大小
    //*****************************************************************
	QLabel *Packetsize=new QLabel(this);
	Packetsize->setText(tr("发包大小:"));
    LineEditPacketsize= new QLineEdit(this);
	LineEditPacketsize->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //设置图像获取信息	
    //*****************************************************************	  	
    QLabel *status= new QLabel(this);
	status->setText(tr("发送信息:"));
	outputEdit= new QLineEdit(this);
	outputEdit->setAlignment(Qt::AlignCenter);
	//*****************************************************************
    //设置查看图片按钮	
    //*****************************************************************	
	PictureButton = new QPushButton( this);//查看图像和拍摄图像按钮
    PictureButton->setText( tr( "查看图像" ) );
    PictureButton ->setFixedSize(160,60);
	connect(PictureButton, SIGNAL(clicked()), this, SLOT(pictures()));	//设置触发提取图片
	//*****************************************************************
    //设置拍摄图片按钮	
    //*****************************************************************	
	HandPictureButton = new QPushButton( this);
    HandPictureButton->setText( tr( "拍摄图片" ) );
    HandPictureButton->setFixedSize(160,60);
	connect(HandPictureButton, SIGNAL(clicked()), this, SLOT(getpictures_Preventjitter()));	
	//*****************************************************************
    //设置接收视频按钮
    //*****************************************************************	
	VideoSendButton = new QPushButton( this);//接收视频按钮
    VideoSendButton->setText( tr( "接收视频" ) );
    VideoSendButton ->setFixedSize(160,60);
	connect(VideoSendButton, SIGNAL(clicked()), this, SLOT(receiveVideos()));	//设置触发开始摄像
	//*****************************************************************
    //布局基本发送参数框
    //***************************************************************** 
	QGridLayout *mainLayout1 = new QGridLayout;//布局
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
    //设置页面布局路由参数部分
    //***************************************************************** 	
	QGroupBox *count_send2= new QGroupBox(tr("路由设置"));

	//QLabel *OrangenNode1= new QLabel(this);
	//OrangenNode1->setText(tr("源节点:"));
    //QLineEdit *lineEdit5 = new QLineEdit;//居中
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
	DestNode1->setText(tr("目的节点:"));
    QLineEdit *lineEdit6 = new QLineEdit;//居中
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
	NextNode1->setText(tr("下一跳节点:"));
    QLineEdit *lineEdit7 = new QLineEdit;//居中
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
	
	Saveseting1 = new QPushButton( this);//保存路由设置按钮
    Saveseting1->setText( tr( "保存" ) );
    Saveseting1 ->setFixedSize(120,60);
	connect(Saveseting1, SIGNAL(clicked()), this, SLOT(Saverouteseting1()));	//保存路由设置
	
	//QLabel *OrangenNode2= new QLabel(this);
	//OrangenNode2->setText(tr("源节点:"));
    //QLineEdit *lineEdit8 = new QLineEdit;//居中
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
	DestNode2->setText(tr("目的节点:"));
    QLineEdit *lineEdit9 = new QLineEdit;//居中
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
	NextNode2->setText(tr("下一跳节点:"));
    QLineEdit *lineEdit10 = new QLineEdit;//居中
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
	
	Saveseting2 = new QPushButton( this);//保存路由设置按钮
    Saveseting2->setText( tr( "保存" ) );
    Saveseting2 ->setFixedSize(120,60);
	connect(Saveseting2, SIGNAL(clicked()), this, SLOT(Saverouteseting2()));	//保存路由设置
	
	//QLabel *OrangenNode3= new QLabel(this);
	//OrangenNode3->setText(tr("源节点:"));
    //QLineEdit *lineEdit11 = new QLineEdit;//居中
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
	DestNode3->setText(tr("目的节点:"));
    QLineEdit *lineEdit12 = new QLineEdit;//居中
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
	NextNode3->setText(tr("下一跳节点:"));
    QLineEdit *lineEdit13 = new QLineEdit;//居中
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
	
	Saveseting3 = new QPushButton(this);//保存路由设置按钮
    Saveseting3->setText( tr( "保存" ) );
    Saveseting3 ->setFixedSize(120,60);
	connect(Saveseting3, SIGNAL(clicked()), this, SLOT(Saverouteseting3()));	//保存路由设置
	//*****************************************************************
    //布局路由设置框
    //***************************************************************** 	
    QGridLayout *mainLayout2 = new QGridLayout;//布局
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
    //向主窗口加入子窗口	
	VSRbMain->addWidget(extension);
	//extension->setLayout(vbMain);
    //*****************************************************************
    //设置接收界面
    //*****************************************************************
	//界面布局
	QVBoxLayout *vbrMain = new QVBoxLayout;
	extension_1->setLayout(vbrMain);
	VSRbMain->addWidget(extension_1);//向窗口加入子窗口extension_1
    //*****************************************************************
    //实现窗口切换功能
    //*****************************************************************		
	extension_1->hide();
	connect(moreButton, SIGNAL(clicked()), this,SLOT(moreButton_state()));
    //*****************************************************************
    //设置收包显示
    //*****************************************************************
	QLabel *receivepacket= new QLabel(this);
	receivepacket->setText(tr("收包显示:"));
	TextEditReceive = new QTextEdit( this);
	QVBoxLayout *receivemessage= new QVBoxLayout;
	receivemessage->addWidget(receivepacket );
	receivemessage->addWidget(TextEditReceive ); 
	vbrMain->addLayout(receivemessage);
    //*****************************************************************
    //设置收包大小
    //*****************************************************************
	QLabel *Packetsize2=new QLabel(this);
    Packetsize2->setText(tr("收包大小:"));
    LineEditPacketsize2= new QLineEdit(this);
	LineEditPacketsize2->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //设置收包数目
    //*****************************************************************	
    QLabel *Packetnumber2= new QLabel(this);
    Packetnumber2->setText(tr("收包数目:"));
	LineEditPacketnumber2= new QLineEdit(this);
	LineEditPacketnumber2->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //设置吞吐量
    //*****************************************************************	
	QLabel *thruput=new QLabel(this);
    thruput->setText(tr("吞  吐  量:"));
	thruputE=new QLineEdit(this);
	thruputE->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //设置刷新周期
    //*****************************************************************	
	QLabel *time=new QLabel(this);
	time->setText(tr("刷新周期:"));	
	gaptimeE=new QLineEdit(this);
    gaptimeE->setAlignment(Qt::AlignCenter);
    //*****************************************************************
    //提取显示源地址
    //*****************************************************************  
    QLabel *LabelHostIP=new QLabel(this);
	LabelHostIP->setText(tr("源  地  址:"));
    LineEditHostIP= new QLineEdit(this);
	LineEditHostIP->setAlignment(Qt::AlignHCenter);
    //*****************************************************************
    //网格布局
    //*****************************************************************		
	QGridLayout *mainLayouter = new QGridLayout;//布局
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
    //设置侦查图像接收信息
    //*****************************************************************	
	QLabel *receive_status= new QLabel(this);
	receive_status->setText(tr("图像接收信息:"));
    receive_outputEdit = new QTextEdit;   
    //*****************************************************************
    //设置查看图像按钮
    //*****************************************************************
	receive_PictureButton = new QPushButton( this);
    receive_PictureButton->setText( tr( "查看图像" ) );  
	receive_PictureButton ->setFixedSize(160,60);
	connect(receive_PictureButton, SIGNAL(clicked()), this, SLOT(pictures()));	//设置触发提取图片
	 //*****************************************************************
    //设置接收视频按钮
    //*****************************************************************
	receive_VideoButton = new QPushButton( this);
    receive_VideoButton->setText( tr( "接收视频" ) );  
	receive_VideoButton ->setFixedSize(160,60);
	connect(receive_VideoButton, SIGNAL(clicked()), this, SLOT(receiveVideos()));	//设置触发接收视频
    //*****************************************************************
    //设置一行
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
    //设置布局窗口底层
    //*****************************************************************   
	setLayout(VSRbMain);
    //*****************************************************************
    //设置发送和监听端口	
    //*****************************************************************		 
    port_send=5555;
	port_rece=5555;
	port_config=4000;
	port_serve=6800;
    //*****************************************************************
    //创建套接字	
    //*****************************************************************		
	udpSocket = new QUdpSocket(this); 
	udpSocket_config=new QUdpSocket(this);
	udpSocket_repicture=new QUdpSocket(this);
    //*****************************************************************
    //绑定端口
    //*****************************************************************		
	udpSocket->bind(port_rece);
	udpSocket_config->bind(port_config);
	udpSocket_repicture->bind(port_serve);
    //*****************************************************************
    //实现各个socket功能槽	
    //*****************************************************************		
	connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recv_slot()));
	connect(udpSocket_config,SIGNAL(readyRead()),this,SLOT(config()));
	connect(udpSocket_repicture,SIGNAL(readyRead()),this,SLOT(picturereceive()));
	pictureclear(); //进入界面删除已有图片
    //*****************************************************************
    //提取本机IP	
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
// 函数名：picture_Preventjitter()
// 功  能：配合函数pictures()完成界面查看按钮防抖
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::picture_Preventjitter()
{
	 Index=1;
}

/*********************************************************************
// 函数名：pictures()
// 功  能：打开图像浏览器
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::pictures()
{  
   if(Index==1)
   {
    Index=0;
   	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    MainWindow *udpclient2 = new MainWindow();	
    udpclient2->show();
	QTimer::singleShot(2*1000, this, SLOT(picture_Preventjitter()));//界面防抖
	}
}

/*********************************************************************
// 函数名：~AeroNodeGeneral()
// 功  能：取消套接字
// 返  回：无
*********************************************************************/
AeroNodeGeneral::~AeroNodeGeneral()
{
  delete udpSocket;
  delete udpSocket_config;
  delete udpSocket_repicture;
} 

/*********************************************************************
// 函数名：Reseting()
// 功  能：重置
// 返  回：无
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
    //清空相关空间
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
    //初始化参数	
    //*****************************************************************	
	Num_rece_packets=1;//统计接收数据包数目
	timetofirst=0;//延时统计量
	throughtput=0;//统计吞吐量
    send_pcket_1=0;//当接收包非b字符时就一直发包，即当send_pcket_1=1时开启定时器
	Length=0;//接收到的数据包的初始长度
	Index=1;
    num=1;
	isStarted1=0;
	receivephotonum=0;
	fristbuf=NULL;//存储接收图片的结构体
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
// 函数名：PushButtonClose_clicked()
// 功  能：退出界面，关闭套接字和图片的自动保存功能
// 返  回：无
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
// 函数名：pictureclear()
// 功  能：删除已有照片
// 返  回：无
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
// 函数名：moreButton_state
// 功  能：切换窗口按钮的状态设置
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::moreButton_state()                     
{   QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));	
	if(!isStarted)
	{  
	   moreButton->setText(tr("发送"));
	   Title->setText(tr("                      接收界面                            "));
	   extension_1->show();
	   extension->hide();
	   isStarted = true;
	}
	else
	{
	    moreButton->setText(tr("接收"));
		Title->setText(tr("                    发送界面                         "));
		isStarted = false;
		extension->show();
		extension_1->hide();
	}
}

/*********************************************************************
// 函数名：recv_slot()
// 功  能：统计接收到数据包的相关信息
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::recv_slot()
{    
   	//地址、端口等参数实例化
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
	     LineEditPacketsize2->setText(msgsize+"bytes" );//显示收包大小
         QString str;
	     str=addr.toString();
	     LineEditHostIP->setText(str);   //提取收包源地址
	     QTime ReceTime=QTime::currentTime();	
	     //记录第一次接收包的时间
	     if( Num_rece_packets==1)
	     {
	     	frTime=QTime::currentTime();
	     }	
	     if(Num_rece_packets>=2)
	     {
	       QString SPAN_Each=QString::number(last_ReceTime.msecsTo(ReceTime));//毫秒级.msecsTo返回当前时间对象last_ReceTime到ReceTime所指定的时间差。为ReceTime-last_ReceTime
	       gaptimeE->setText( SPAN_Each +"ms");
	     }
	     last_ReceTime=ReceTime;	
	     NS=QString::number(Num_rece_packets);	
	     TextEditReceive->append(NS+"    "+"\n"+recvBuf);	
	     LineEditPacketnumber2->setText(NS);		
	     if (Num_rece_packets>=2)//统计吞吐量
         {
	     	 timetofirst=frTime.msecsTo(ReceTime); //毫秒级别,返回了数据包截至当前接收到的总时间
	     	 throughtput=(double)(Length*8000)/(double)timetofirst;
	     	 QString thput=QString::number( throughtput );
	     	 thruputE->setText(thput+"bps");
	     }	
         ++Num_rece_packets;//统计接收数据包数
	}
}

/*********************************************************************
// 函数名：timeout
// 功  能：定时超时时，开始发送数据包
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::timeout() 
{  
  QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));  
    QString msg=guideinformationComboBox->currentText();;//设置发送数据包内容(发送信息，手动输入)
	int L=0;//发送数据包的大小
	QByteArray msg_QByteArray = msg.toLatin1();    
    L=msg_QByteArray.size();
	char *msg_char;
	msg_char=msg_QByteArray.data();		
	QString Ipmsg = IPComboBox->currentText();//设置目标地址
	hostaddr1 = new QHostAddress(Ipmsg);	
    QTime sendtime=QTime::currentTime();//设置发包时间
	QString sendtime_string=sendtime.toString();
	char *sendtime_char;
	QByteArray sendtime_QByteArray=sendtime_string.toLatin1();
	sendtime_char=sendtime_QByteArray.data();
    QString LS=QString::number(L);		//显示数据大小
	LineEditPacketsize->setText( LS+"  bytes" );		
	udpSocket->writeDatagram(msg_char,L,*hostaddr1, port_send);	
	QString NS=QString::number(num);//显示发送数据包的数目
	LineEditPacketnumber->setText(NS);	
	++num; 	 
}

/*********************************************************************
// 函数名：SLOT_Deal_guideinfo
// 功  能：处理底层发来要发包的引导信息
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::Deal_guideinfo()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
	  specified_timer_span = timer_spanComboBox->currentText();//判断是否指定了发包间隔（发包间隔）
	      if(specified_timer_span == "")
	      {
		    QMessageBox::information(this,tr("警告"),tr("请键入发包间隔！"));
		    return;	
	      }	  
	      //设置发包内容
          QString msg=guideinformationComboBox->currentText();;//设置发送数据包内容(发送信息，手动输入)
	      int L=0;//发送数据包的大小	  
	      if(msg == "")//如果发送内容为空（需从界面手动输入发送信息），则返回
	      {
	        QMessageBox::information(this,tr("警告"),tr("请键入发送信息！"));
		    return;	
	      }
		  	 if(formatComboBox->currentText() == "")
	      {
	       QMessageBox::information(this,tr("警告"),tr("请键入仿真时长！"));
	         return;	
	       }
	   
        if(IPComboBox->currentText()!="")
        {
          QByteArray msg_QByteArray = msg.toLatin1();    
          L=msg_QByteArray.size();
	      char *msg_char;
	      msg_char=msg_QByteArray.data();		
	      QString Ipmsg = IPComboBox->currentText();//设置目标地址
	    
	      hostaddr1 = new QHostAddress(Ipmsg);	
          QTime sendtime=QTime::currentTime();//设置发包时间
	      QString sendtime_string=sendtime.toString();
	      char *sendtime_char;
	      QByteArray sendtime_QByteArray=sendtime_string.toLatin1();
	      sendtime_char=sendtime_QByteArray.data();
	   
	      QString LS=QString::number(L);		//显示数据大小
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
// 函数名：config
// 功  能：统一配置参数
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::config()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));	
	//开辟缓存接收数据包
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
	formatComboBox->setEditText(format);//仿真时长
	char *IP_destination=strtok(NULL," ");
	IPComboBox->setEditText(IP_destination);//目标地址
	char *timer_span=strtok(NULL," ");
	timer_spanComboBox->setEditText(timer_span);//发包间隔
	char *LineEdit=strtok(NULL,"\n");
	guideinformationComboBox->setEditText(LineEdit);   
    }   
	else if(strcmp(addressIp,str_char)!=0)
	{
	   Reseting();
	}
}

/*********************************************************************
// 函数名：on_btnShow_clicked()
// 功  能：显示软键盘
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::on_btnShow_clicked()
{
	virtualKeyBoard->show();
}
/*********************************************************************
// 函数名：on_btnHide_clicked()
// 功  能：关闭软键盘
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::on_btnHide_clicked()
{    
	virtualKeyBoard->hide();
}

/*********************************************************************
// 函数名：readOutput
// 功  能：显示拍照打印信息
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::readOutput(QString output)   
{   
    outputEdit->setText(output);
	getpictures->exit();
	isStarted1=0;
}


/*********************************************************************
// 函数名：timerStrDisplay
// 功  能：主线程显示时间
// 返  回：无
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
    
   //根据配置文件读入入网和退网时间，确定相关操作
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
// 函数名：run
// 功  能：开启定时器线程
// 返  回：无
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
// 函数名：timerStrDisplay
// 功  能：定时器记录时间函数
// 返  回：无
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
// 函数名：getpictures_Preventjitter
// 功  能：线程：防抖
// 返  回：无
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
// 函数名：runClicked
// 功  能：线程：线程触发图片传输
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::runClicked()   
{  
 	getpictures->exit();
	//paramter=IPComboBox->currentText();
	getpictures->start();	 
}
 
/*********************************************************************
// 函数名：run
// 功  能：开启拍照线程函数
// 返  回：无
*********************************************************************/
void GetPictures_General::run()
{      
    int i;
	//i=v4l2_camera();
	if(i==1)
	{
        //检测当前照片序号
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
	    //移动新照片
	    QString source="/home/temp/p2.JPEG";
    	yn=QFile::copy(source,path);
    	//发送照片
    	//send(path);	
    	//删除照片
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
// 函数名：read_frame
// 功  能：读取拍照图像
// 返  回：成功返回1
*********************************************************************/
//int GetPictures_General::read_frame (FILE *k)
//{
//    struct v4l2_buffer buf;                        //用于盛放一帧数据，v4l2_buffer是v4l2现有的结构体
//   memset (&(buf), 0, sizeof (buf));              //清除buf这个结构体的内容
//    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;        //固定
//    buf.memory = V4L2_MEMORY_MMAP;                 //文件传输方式，read，mmap，userptr三种
//    int ff = ioctl (fd, VIDIOC_DQBUF, &buf);   
//    //从视频缓冲区的输出队列中取得一个已经保存有一帧视频数据的视频缓冲区,这里面包含此缓冲区的编号index，等其他信息，下面的buffers结构体就利用buf.index来索引此缓冲区的物理地址（首地址）
//    if(ff<0)
//    {
//        printf("failture\n");                          //出列采集的帧缓冲 
//        return 0;
//    }		
//    assert (buf.index < n_buffers);                //判断buf.index是否小于n_buffers
//    printf ("buf.index dq is %d\n",buf.index);

//    fwrite(buffers[buf.index].start, buffers[buf.index].length,1,k); 
    //将其写入文件中，以buffers[buf.index].start开始的buffers[buf.index].length长的数据写入file_fd中

//    ff=ioctl (fd, VIDIOC_QBUF, &buf); //再将其入列，投放一个空的视频缓冲区到视频缓冲区输入队列中，以便下一次的采集数据
//    if(ff<0)
//    {
//	    printf("failture VIDIOC_QBUF\n");
//		return 0;
//	}
//    printf ("buffers[%d].start is %ld\n",buf.index,buffers[buf.index].start);
//    return 1;

//}

/*********************************************************************
// 函数名： v4l2_camera
// 功  能：拍照
// 返  回：无
*********************************************************************/
//int GetPictures_General::v4l2_camera()
//{

 //   struct v4l2_capability cap; //硬件摄像头的驱动能够提供的功能都包含在这个结构体里面
 //   struct v4l2_format fmt;   //包含输出的图像的信息，每帧图像的像素，分辨率等信息
 //   unsigned int i;
 //   int j,m;

 //   FILE *file_fd[4]={NULL,NULL,NULL,NULL};   //图像将要被保存到的目标文件名，类型是文件类型FILE
//	char *picture[4]={"/home/temp/p0.JPEG","/home/temp/p1.JPEG","/home/temp/p2.JPEG","/home/temp/p3.JPEG"};//zyj
	
 //   enum v4l2_buf_type type;   //数据流类型V4L2_BUF_TYPE_VIDEO_CAPTURE,V4L2_BUF_TYPE_VIDEO_OUTPUT,V4L2_BUF_TYPE_VIDEO_OVERLAY

//    fd = open(dev_name, O_RDWR  | O_NONBLOCK, 0);//打开摄像头设备，无阻塞方式打开
//
//    int ff=ioctl (fd, VIDIOC_QUERYCAP, &cap);//获取摄像头参数，并且保存在cap中
 //   if(ff<0)
//	{    
//	    printf("failture VIDIOC_QUERYCAP\n");
//		return 0;
//	}

    //设置输出图像信息	
//    memset (&(fmt), 0, sizeof (fmt));  
//    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    fmt.fmt.pix.width       = 640;
//    fmt.fmt.pix.height      = 480;
//    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;//V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV;
//    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
//   ff = ioctl (fd, VIDIOC_S_FMT, &fmt); //设置图像格式
//    if(ff<0)
//    {
//        printf("failture VIDIOC_S_FMT\n");
//		return 0;
//	}
		
    //申请缓存，count是申请的缓存的数量
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
//    if (req.count < 2)//如果缓存数量少于2个，打印缓存数量不足
//        printf("Insufficient buffer memory\n");
//    buffers = (buffer*)calloc (req.count, sizeof (*buffers));//内存中建立对应空间，在内存中分配req.count个大小为sizeof(*buffers)的个人空间
//    printf("the number of memory is %d\n",req.count);//打印缓存数量
//
 //   for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
 //   {
//        //驱动中的一帧，存放一帧数据，每个缓存放一帧
///        struct v4l2_buffer buf;   
//        memset (&(buf), 0, sizeof (buf));   
//        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//        buf.memory      = V4L2_MEMORY_MMAP;
//        buf.index       = n_buffers;
//        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))//映射用户空间，将REQBUFS申请的缓存映射为物理地址，可以不连续
//        {
 //   		printf ("VIDIOC_QUERYBUF error\n");
//			return 0;
//	    }
//        buffers[n_buffers].length = buf.length;
//        buffers[n_buffers].start = (long unsigned int*)mmap (NULL,buf.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd, buf.m.offset);//物理地址映射为虚拟地址
//        //通过mmap建立映射关系。NULL： /* start anywhere */。PROT_READ | PROT_WRITE ：/* required */。
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
//        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))  //申请到的缓冲进入列队
//        {
//    		printf ("VIDIOC_QBUF failed\n");
//			return 0;
//		}
//        else
//		    printf("VIDIOC_QBUF %d success.\n",i);
//   }       
//    
//    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
//   {
//        printf ("VIDIOC_STREAMON failed\n");
//		return 0;
//	}
//    for (j=0;j<req.count;++j)      //这一段涉及到异步IO
//    {
//        fd_set fds;
//        struct timeval tv;
//        int r;
//        FD_ZERO (&fds);//将指定的文件描述符集清空
//        FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符
//    /* Timeout. */
//        tv.tv_sec = 1;
//       tv.tv_usec = 0;
//        r = select (fd + 1, &fds, NULL, NULL, NULL);//判断是否可读（即摄像头是否准备好），tv是定时
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
//        file_fd[j] = fopen(picture[j], "w");//以只写方式打开文件
//        printf("n_buffers=%d\n",n_buffers);  
//        if (read_frame (file_fd[j])==1)
//            printf("picture(%d)...\n",j);   //如果可读，执行read_frame ()函数，并跳出循环
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
//        printf("释放缓冲区%d结束...\n",i);
//    }
//
//    close (fd);
//    return 1;
//}
/*********************************************************************
// 函数名： send
// 功  能：发送图片
// 返  回：无
*********************************************************************/
//void GetPictures_General::send(QString path)
//{
//   	port_sendpicture=6800;
//	udpSocket_sendpicture = new QUdpSocket(this); 
//	udpSocket_sendpicture->bind(port_sendpicture);
//	hostaddr_picture = new QHostAddress(paramter);//设置目标IP，
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
//    udpSocket_sendpicture->writeDatagram(buf2,50,*hostaddr_picture,6800);//发送文件名
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
//   	int rread = fread(&buf1[5],1,m,picfd);//m=0情况如何？
//	udpSocket_sendpicture->writeDatagram(buf1,10240,*hostaddr_picture,6800);
//    printf("the last buf send\n");	
//    fclose(picfd); 
//    udpSocket_sendpicture->close();
//}
/*********************************************************************
// 函数名： picturereceive
// 功  能： 接收图片
// 返  回： 无
*********************************************************************/
void AeroNodeGeneral::picturereceive()
{
	printf("enter picture receive\n");
	QHostAddress addr;
    quint16 port;
	char recvBuf[10240]={0};
	char buf2[50]={'/','h','o','m','e','/','p','h','o','t','o','s','/'};
	int m;
	while (udpSocket_repicture->hasPendingDatagrams())//判断是否有数据包到来
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
			        //判断接收的图片数
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
// 函数名：receiveVideos()
// 功  能：打开视频浏览器
// 返  回：无
*********************************************************************/
void AeroNodeGeneral::receiveVideos()  
{  
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    VideoDisplay *videodisp = new VideoDisplay();
    videodisp->show();
}