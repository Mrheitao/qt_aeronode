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
**文件名：AeroNodeConfigDialogall
**功能：AeroNodeConfigDialogall类是对整个主界面进行了布局设置，其中
        调用了AeroNodeSystem（系统）类、AeroNodeFunction（功能）类、
		AeroNodeSynchronization（同步）类和AeroNodeHelp（帮助）类。    
*********************************************************************/
#include <QtGui>
#include "AeroNodeConfigDialogall.h"
#include "AeroNodeSystem.h"
#include "AeroNodeFunction.h"
#include "AeroNodeSynchronization.h"
#include "AeroNodeHelp.h"
/*********************************************************************
// 函数名：AeroNodeConfigDialogall
// 功  能：主界面的设置
// 返  回：无
*********************************************************************/
AeroNodeConfigDialogall::AeroNodeConfigDialogall()
{
    //*****************************************************************  
    //设置列表组件
    //*****************************************************************
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);//图标显示
    contentsWidget->setIconSize(QSize(240,104));
	contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(180);
    contentsWidget->setSpacing(15);//间隔15
    contentsWidget->setStyleSheet("background-color:rgb(193,210,240)");
	contentsWidget->setCurrentRow(0);  //默认显示系统行
    //*****************************************************************	
    //把QStackedWidget加入主窗口
    //*****************************************************************
    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(new AeroNodeSystem);//系统
    pagesWidget->addWidget(new AeroNodefunction);//功能
    pagesWidget->addWidget(new AeroNodeSynchronization);//同步
    pagesWidget->addWidget(new AeroNodeHelp);//帮助
    
    createIconsall();//增加图标	
    //*****************************************************************
    //设置标签
    //*****************************************************************
    QLabel *nuaa=new QLabel( tr("南京航空航天大学航空通信网络研究室"),this);
    QFont font("wenquanyi",12);
    nuaa->setFont(font);  
    nuaa->setAlignment(Qt::AlignHCenter);
    //*****************************************************************	
    //网格布局页面
    //*****************************************************************
	QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(contentsWidget, 0, 0,1,1);
	mainLayout->addWidget(pagesWidget, 0, 1,1,1);
    mainLayout->addWidget(nuaa,1,0,1,2);
	mainLayout->setContentsMargins ( 35, 25,20, 10);//setContentsMargins ( int left, int top, int right, int bottom )
	
    setLayout(mainLayout);
    setGeometry(0,0,1280,800);    
    setFont(QFont("wenquanyi"));//设置大小和字体类型
    setWindowFlags(Qt::FramelessWindowHint);//去掉边框
}

/*********************************************************************
// 函数名：createIcons
// 功  能：在 contensWidget 中创建图标
// 返  回：无
*********************************************************************/
void AeroNodeConfigDialogall::createIconsall()
{ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//字体编码类型设置
    //*****************************************************************
    //系统按钮
    //*****************************************************************
    QListWidgetItem *configButton_ALL = new QListWidgetItem(contentsWidget);
    configButton_ALL->setIcon(QIcon(":/images/home3.png"));
	configButton_ALL->setText(tr("系统"));
    configButton_ALL->setTextAlignment(Qt::AlignHCenter);
    configButton_ALL->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //*****************************************************************
    //模块按钮
    //*****************************************************************
    QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
    updateButton->setIcon(QIcon(":/images/module.png"));
	updateButton->setText(tr("功能"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //*****************************************************************
    //时间按钮
    //*****************************************************************
    QListWidgetItem *synchronizationButton = new QListWidgetItem(contentsWidget);
    synchronizationButton->setIcon(QIcon(":/images/synchronization.png"));
	synchronizationButton->setText(tr("同步"));
    synchronizationButton->setTextAlignment(Qt::AlignHCenter);
    synchronizationButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
   //*****************************************************************
   //帮助按钮
   //*****************************************************************
	QListWidgetItem *showtime = new QListWidgetItem(contentsWidget);
    showtime ->setIcon(QIcon(":/images/help3.png"));
	showtime ->setText(tr("帮助"));
    showtime ->setTextAlignment(Qt::AlignHCenter);
    showtime ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
   //*****************************************************************
   //连接相应图标按钮和其触发的槽
   //*****************************************************************
    connect(contentsWidget,SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),this, SLOT(changePageall(QListWidgetItem *, QListWidgetItem*)));
}

/*********************************************************************
// 函数名：changePage
// 功  能：改变显示页
// 返  回：无
*********************************************************************/
void AeroNodeConfigDialogall::changePageall(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}






