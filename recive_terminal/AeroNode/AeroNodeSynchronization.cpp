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
**文件名：AeroNodeSynchronization.cpp
**功能：AeroNodeSynchronization类定义了同步界面。 
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QTranslator>
#include <QtGui>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDesktopWidget>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeSynchronization.h"
/*********************************************************************
// 函数名：AeroNodeSynchronization
// 功  能：同步界面构造函数
// 返  回：无
*********************************************************************/
AeroNodeSynchronization::AeroNodeSynchronization(QWidget *parent)
    : QWidget(parent)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8")); 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));        //支持Tr中文 
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8")); //支持中文文件名显示 

	QVBoxLayout *VVV3=new QVBoxLayout;//垂直布局
    //*****************************************************************	
    //创建时间显示组
    //*****************************************************************
	QGroupBox *llcd= new QGroupBox(tr("系统时间"),this);
    QVBoxLayout *main=new QVBoxLayout;//垂直布局	
	timer=new QTimer;//时钟对象实体化	
	D = new QLabel(this);//设置年月日标签
	D->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter); 
    //*****************************************************************	
    //设置时分秒
    //*****************************************************************	
	lcdNumber=new QLCDNumber;//显示当前时间
	lcdNumber->setFrameStyle ( QLCDNumber::NoFrame);
	lcdNumber->setFixedSize(QSize(240,50));	//数字时钟的大小
	lcdNumber->setNumDigits(8);//显示八个数字，默认是显示5个数字
	lcdNumber->setSegmentStyle(QLCDNumber::Flat);//填充颜色，默认是白色的
	lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));//设置格式，还有其他形式的
    //*****************************************************************	
    //设置毫秒级的表示
    //*****************************************************************	
	/*lcdNumber_msec=new QLCDNumber;	//显示当前毫秒级时间
	lcdNumber_msec->setFrameStyle ( QLCDNumber::NoFrame);
	lcdNumber_msec->setFixedSize(QSize(240,50));//设置数字时钟的大小	
	lcdNumber_msec->setNumDigits(3);//设置毫秒级的数字位数：显示三个数字
	lcdNumber_msec->setSegmentStyle(QLCDNumber::Flat);	//填充颜色，默认是白色的	
	lcdNumber_msec->display( QTime::currentTime().msec());//设置格式，还有其他形式的 
	*/
	connect(timer,SIGNAL(timeout()),this,SLOT(clockChangeall()));//定时显示时间
	timer->start(100);//时间更新间隔
	
    QHBoxLayout *datelayout=new QHBoxLayout;//水平布局
	datelayout->addWidget(D);  	
	QHBoxLayout * timelayout=new QHBoxLayout;//水平布局
	timelayout->addWidget(lcdNumber);//时分秒
	//timelayout->addWidget(lcdNumber_msec);//毫秒	
	main->addLayout(datelayout);
	main->addLayout(timelayout);
	main->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	llcd->setLayout(main);	
    VVV3->addWidget(llcd);
    setLayout(VVV3); 		
	setFont(QFont("wenquanyi"));//字体设置
	setWindowFlags(Qt::FramelessWindowHint);
}

/*********************************************************************
// 函数名：clockChange
// 功  能：更新时间
// 返  回：无
*********************************************************************/
void AeroNodeSynchronization::clockChangeall()
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8")); 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));        //支持Tr中文 
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8")); //支持中文文件名显示	
	lcdNumber->display( QTime::currentTime().toString("hh:mm:ss") );
	//lcdNumber_msec->display( QTime::currentTime().msec() );
	QFont font("",12);
    D->setFont(font); 
	D->setText( QObject::tr("日期:")+QDateTime::currentDateTime().date().toString()+"\n" );
	D->setFont(QFont("wenquanyi"));
}