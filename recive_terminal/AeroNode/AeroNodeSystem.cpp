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
**文件名：AeroNodeSystem.cpp
**功能：AeroNodeSystem类对系统的软件及硬件信息进行了简单介绍。    
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QTranslator>
#include <QtGui>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDesktopWidget>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeSystem.h"

/*********************************************************************
// 函数名：AeroNodeSystem
// 功  能：显示本地信息 
// 返  回：无
*********************************************************************/
AeroNodeSystem::AeroNodeSystem(QWidget *parent)
    : QWidget(parent)
{ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//设定字体编码,支持显示中文
	
    QVBoxLayout *Vlayout = new QVBoxLayout;//垂直布局
	Vlayout->setSpacing(20);//间隔20
    //*****************************************************************	
    //显示硬件信息
    //*****************************************************************
	QGroupBox *Hostgroup= new QGroupBox(tr("硬件信息"),this);
	QFont font1("",12);	
    QLabel *typelabel = new QLabel(tr("CPU:"));
	QLabel *typelabelEdit = new QLabel(tr("ARM Cortex-A9"));
    typelabel->setFont(font1);
	typelabelEdit->setFont(font1);
	QLabel *NandF = new QLabel(tr("NAND FLASH:"));
	QLabel *NandFEdit = new QLabel(tr("4G"));
	NandF->setFont(font1);
	NandFEdit->setFont(font1);

	QLabel *SDRAM = new QLabel(tr("DDR3 SDRAM:"));
	QLabel *SDRAMEdit = new QLabel(tr("1G"));
	SDRAM->setFont(font1);
	SDRAMEdit->setFont(font1);
    //*****************************************************************
    //布局设置
    //*****************************************************************	
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setSpacing(20);
    mainLayout->addWidget(typelabel, 0, 0);
    mainLayout->addWidget(typelabelEdit, 0, 1);
    mainLayout->addWidget(NandF, 1,0);
    mainLayout->addWidget(NandFEdit, 1, 1);
    mainLayout->addWidget(SDRAM, 2, 0);
    mainLayout->addWidget(SDRAMEdit, 2, 1);
	Hostgroup->setLayout(mainLayout);
	Vlayout->addWidget(Hostgroup);
    //*****************************************************************
    //显示系统信息
    //*****************************************************************
	QGroupBox *Hostsystem= new QGroupBox(tr("软件信息"),this);
	QVBoxLayout *V2= new QVBoxLayout;
	V2->setSpacing(20);
	QHBoxLayout *systemlayout= new QHBoxLayout;
   	QLabel *L= new QLabel(tr("Embedded-Linux-3.5"));
    QLabel *bootload = new QLabel(tr("U-boot-Tiny4412"));
	L->setFont(font1);
	bootload->setFont(font1);
    systemlayout->addWidget(L);
    systemlayout->addWidget(bootload);
	V2->addLayout(systemlayout);
    //*****************************************************************   
    //显示本机IP
    //*****************************************************************
    QHBoxLayout *IPlayout = new QHBoxLayout; 
    QLabel *LabelHostIP=new QLabel(this);	
	QLabel *Interface=new QLabel(this);
    //*****************************************************************
    //显示IP地址
    //*****************************************************************
    QString str;
    QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach(QHostAddress address, list)
    {
     if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
          if(address.toString().contains("127.0."))
           {	 
		 continue;
           }
       	  str=address.toString();
	    }
    }
    if (str=="127.0.0.1")
    {
         LabelHostIP->setText("IP:  get local ip fail");
		 return; 
    }
    else
    {
         LabelHostIP->setText("IP:  "+str);
    }
	Interface->setText("Graphical Interface:  Qt/E");
	LabelHostIP->setFont(font1);	
	
	IPlayout->addWidget(Interface);
	IPlayout->addWidget(LabelHostIP);
	V2->addLayout(IPlayout);
	Hostsystem->setLayout(V2);
	Vlayout->addWidget(Hostsystem);
    //*****************************************************************
    //垂直布局
    //*****************************************************************			
    QVBoxLayout *mainlayoutall = new QVBoxLayout;
	mainlayoutall->addLayout(Vlayout);
    //mainlayoutall->addStretch(1);
    setLayout(mainlayoutall);    
	setFont(QFont("wenquanyi"));
}

