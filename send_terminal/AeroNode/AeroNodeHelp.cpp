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
**文件名：AeroNodeHelp.cpp
**功能：AeroNodeHelp类定义了帮助界面。    
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QTranslator>
#include <QtGui>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDesktopWidget>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeHelp.h"
/*********************************************************************
// 函数名：AeroNodeHelp
// 功  能：帮助界面的设置
// 返  回：无
*********************************************************************/
AeroNodeHelp::AeroNodeHelp(QWidget *parent)
    : QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312")); //支持中文文件名显示 
	
    desktopGeometry = QApplication::desktop()->availableGeometry();// 获取可用桌面大小
    QScrollArea *scrollArea = new QScrollArea(this);//设置滚动区域  
	QWidget  *W= new QWidget(scrollArea);	
	W->setMinimumSize (600,480);	
	QLabel* TITLE = new QLabel(W);  
	TITLE->setText(tr("                                   功 能 简 介"));
	
    QLabel* label = new QLabel(W);
    label->setText(tr("\t本实物节点界面分为四个模块：系统模块、功能模块、\n同步模块和帮助模块。下面对前三个模块的功能做简要的说\n明。\n\t系统模块：该模块介绍了测试板卡的基本信息，包括硬\n件信息和软件信息。\n\t功能模块：该模块提供了界面收发数据包通用模式。通\n用模式可提供基本数据收发、图片拍摄、图片的发送和接收\n以及静态路由的设置功能。可设置发送数据包的间隔、内容\n及目标地址等信息。\n\t同步模块：该模块提供了系统时间，方便测试板卡间的\n时间同步。\n\t研发单位：南京航空航天大学航空通信网络研究室\n       烧写时间：2014年9月10日"));
	QVBoxLayout *VV1 = new QVBoxLayout();//垂直重列
	VV1->addWidget(TITLE);
	VV1->addWidget(label);
	W->setLayout(VV1);
	scrollArea->setWidget(W);	
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(scrollArea);
    setLayout(layout);	
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(QApplication::desktop(), SIGNAL(workAreaResized(int)),this, SLOT(desktopResizedall(int)));  

	setFont(QFont("wenquanyi"));//设置字体类型
}

/*********************************************************************
// 函数名：desktopResized
// 功  能：调整界面大小
// 返  回：无
*********************************************************************/
void AeroNodeHelp::desktopResizedall(int screen)
{
    if (screen != 0)
        return;
    reactToSIPall();
}

/*********************************************************************
// 函数名：reactToSIP
// 功  能：合理布局界面
// 返  回：无
*********************************************************************/
void AeroNodeHelp::reactToSIPall()
{
    QRect availableGeometry = QApplication::desktop()->availableGeometry();
    if (desktopGeometry != availableGeometry) {
        if (windowState() | Qt::WindowMaximized)
            setWindowState(windowState() & ~Qt::WindowMaximized);
        setGeometry(availableGeometry);
    }
   desktopGeometry = availableGeometry;
}