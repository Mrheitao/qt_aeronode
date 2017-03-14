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
**文件名：AeroNodeFunction.cpp
**功能：AeroNodeFunction类定义了功能界面。在功能界面中调用了AeroNodeCCSMA
        高效并发的节点类、AeroNodeESTDMAZZ_Backbone拓扑透明的骨干节点类、
        AeroNodeESTDMAZZ_SubnetGatway拓扑透明的子网网关类、
		AeroNodeDTDMA_Aircraft弹机协同的载机类、AeroNodeDTDMA_Missile
		弹机协同的导弹类。
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QTranslator>
#include <QtGui>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDesktopWidget>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeFunction.h"
#include "AeroNodeGeneral.h"
/*********************************************************************
// 函数名：PlanePageall
// 功  能：构造函数
// 返  回：无
*********************************************************************/
AeroNodefunction::AeroNodefunction(QWidget *parent)
    : QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312")); //支持中文显示
    //*****************************************************************
    //设置功能简介
    //*****************************************************************
    QGroupBox *pp= new QGroupBox(tr("功能简介"));
	QLabel *functionE=new QLabel(this);
    functionE->setText(tr("       该模块提供了界面收发数据包通用模式。通用模式可提\n供基本数据收发、图片拍摄、图片的发送和接收以及静态路\n由的设置功能。可设置发送数据包的间隔、内容及目标地址\n等信息。"));
	QHBoxLayout *pplayout = new QHBoxLayout;
    pplayout->addWidget(functionE);
    pp->setLayout(pplayout);
    //*****************************************************************	
    //设置三个模式按钮收发信机
    //*****************************************************************
	receiver1 = new QPushButton( QIcon(""),tr("节点"),this);
	receiver1->setFixedSize(240,60);	
    //*****************************************************************	
    //连接信号和槽
    //第一次点击按钮后相隔一定时间第二次点击才能生效
    //*****************************************************************
	connect(receiver1,SIGNAL(clicked()),this,SLOT(clientDialog_Preventjitter_General()));
    //*****************************************************************
    //布局三个模式
    //*****************************************************************	
	QHBoxLayout *receiver1layout = new QHBoxLayout;
	receiver1layout->addWidget(receiver1);
	QHBoxLayout *receiver3Hlayout = new QHBoxLayout;

	QHBoxLayout *pp2layout = new QHBoxLayout;
	
    QGroupBox *module1 = new QGroupBox(tr("通用模式"));
	//module1->setAlignment (Qt::AlignHCenter);
	module1->setLayout(receiver1layout);
	pp2layout->addWidget(module1);

    //*****************************************************************	
    //布局界面和指定字体类型
    //*****************************************************************
	QVBoxLayout *Vlayout = new QVBoxLayout;
    Vlayout->addWidget(pp);
    Vlayout->addLayout(pp2layout);
    setLayout(Vlayout);    
    setFont(QFont("wenquanyi"));	

	isStarted_module=false;
	m_second_module=0; 
	m_msecond_module=0;	 
	
	m_timer_module=new QTimer(this);
	connect(m_timer_module,SIGNAL(timeout()),this,SLOT(timerStrDisplay_module()));
}

/*********************************************************************
// 函数名：clientDialog_DJXT_plane
// 功  能：通用模式节点发送界面背景
// 返  回：无
*********************************************************************/
void AeroNodefunction::clientDialog_General()
{
	//设置编码方式
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    AeroNodeGeneral *udpclient_General = new AeroNodeGeneral();//导弹界面类
	udpclient_General->setMaximumSize(1280,800);//设置大小
	QPalette p;//背景
    p.setBrush(udpclient_General->backgroundRole(),QBrush(QColor("#C1D2F0")));
    udpclient_General->setPalette(p);
    udpclient_General->setAutoFillBackground(true);
    udpclient_General->showFullScreen();
	udpclient_General->setFont(QFont("wenquanyi"));
}





/*********************************************************************
// 函数名：clientDialog_Preventjitter_General
// 功  能：链接通用模式函数，完成按钮防抖
// 返  回：无
*********************************************************************/
void AeroNodefunction::clientDialog_Preventjitter_General()                   
{   
    if(!isStarted_module)
	{  
	   m_timer_module->stop();//m_timer_module=new QTimer(this)
	   clientDialog_General();//打开通用界面
	   isStarted_module = true;
	   m_second_module=0; 
	   m_msecond_module=0;
	   m_timer_module->start(100);
	}
}

/*********************************************************************
// 函数名：timerStrDisplay_module
// 功  能：定时器设置
// 返  回：无
*********************************************************************/
void AeroNodefunction::timerStrDisplay_module()
{
  
  if(++m_msecond_module>10)
   {
        m_msecond_module=0;
		if(++m_second_module=2)
		{
		  isStarted_module= false;      	   
		}
   }

   QString str,tmp;
   tmp.setNum(m_second_module);
   if(m_second_module<10){str+='0';}
   str+=tmp;
   str+=':';

   tmp.setNum(m_msecond_module);
   if(m_msecond_module<10){ str+='0';}
   str+=tmp;
}