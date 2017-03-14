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
**文件名：AeroNodePictureViewer.cpp
**功能：AeroNodePictureViewer文件定义了AeroNodePictureViewer图像浏览
        器类。
*********************************************************************/


#include "AeroNodePictureViewer.h"
#include <QTranslator>
#include <QDebug>
#include <QTextCodec>
extern QTranslator *tor;
extern MainWindow *w;
/*********************************************************************
// 函数名：MainWindow
// 功  能：图像浏览器
// 返  回：无
*********************************************************************/
MainWindow::MainWindow()
{
	imageWidget = new ImageWidget;
	
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	this->setWindowIcon(QIcon(" "));//去掉标题栏图标
	this->setFixedSize(900,600);//固定大小
	
    scrollArea = new QScrollArea;
	scrollArea->setPalette(QPalette( QColor(128,128,128) ) );
    scrollArea->setWidget(imageWidget);
	scrollArea->widget()->setMaximumSize(1280,720);	
	setCentralWidget(scrollArea);
    createActions();
    createToolBars();
    createStatusBar();
    //设置窗口标题
    setWindowTitle(tr("图像浏览器"));
	setFocusPolicy(Qt::StrongFocus);
	index = 0;
    //imageDir.setPath(QDir::currentPath());//获取当前文件的路径
    //imageDir.setPath("G:/100CASIO");
	//QStringList filter;
    //set file filter
	//filter << "*.jpg" << "*.bmp" << "*.jpeg" << "*.png" << "*.xpm";
	//imageList = imageDir.entryList ( filter, QDir::Files );//将文件夹的列表放入imagelist当中
	//imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
	//next();
	//currentFile = imageList.constBegin();
}
/*********************************************************************
// 函数名：resizeEvent
// 功  能：矩形框
// 返  回：无
*********************************************************************/
void MainWindow::resizeEvent(QResizeEvent * event)
{
	QRect childRect = scrollArea->childrenRect();
	imageWidget->resize(childRect.size());
}
/*********************************************************************
// 函数名：createActions
// 功  能：创建菜单
// 返  回：无
*********************************************************************/
void MainWindow::createActions()
{
    //*****************************************************************
    //打开
    //*****************************************************************
    dirAct = new QAction(QIcon(":/images/open.png"), tr("Open"), this);
    dirAct->setShortcut(QKeySequence::Open);
    connect(dirAct, SIGNAL(triggered()), this, SLOT(selectDir()));
    //*****************************************************************
    //前一个
    //*****************************************************************
    prevAct = new QAction(QIcon(":/images/previous.png"), tr("Previous"), this);
    prevAct->setShortcut(QKeySequence::Back);
	prevAct->setEnabled(false);
    connect(prevAct, SIGNAL(triggered()), this, SLOT(prev()));
    //*****************************************************************
    //下一个
    //*****************************************************************
    nextAct = new QAction(QIcon(":/images/next.png"), tr("Next"), this);
    nextAct->setShortcut(QKeySequence::Forward);
	nextAct->setEnabled(false);
    connect(nextAct, SIGNAL(triggered()), this, SLOT(next()));
    //*****************************************************************
    //放大
    //***************************************************************** 
    zoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("ZoomIn"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
	zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    //*****************************************************************
    //缩小
    //*****************************************************************
    zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("ZoomOut"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
}
/*********************************************************************
// 函数名：createToolBars
// 功  能：增加菜单工具栏
// 返  回：无
*********************************************************************/
void MainWindow::createToolBars()
{
    naviToolBar = addToolBar(tr("Navigation"));
    naviToolBar->addAction(dirAct);
    naviToolBar->addSeparator();
    naviToolBar->addAction(prevAct);
    naviToolBar->addAction(nextAct);
    naviToolBar->addAction(zoomInAct);
    naviToolBar->addAction(zoomOutAct);
}
/*********************************************************************
// 函数名：createStatusBar
// 功  能：创建状态栏
// 返  回：无
*********************************************************************/
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
/*********************************************************************
// 函数名：selectDir
// 功  能：选择窗口
// 返  回：无
*********************************************************************/
void MainWindow::selectDir()
{
    prevAct->setEnabled(true);
	nextAct->setEnabled(true);
	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	pFileDialog = new QFileDialog(this, tr("Open File"),"/mnt/hgft/",NULL);//设置一个窗口，窗口标题为open file
    pFileDialog->setGeometry(0,0,1280,800); //设置对话框大小为开发板的屏幕大小
    pFileDialog->setOption(QFileDialog::ShowDirsOnly);//设置QFileDialog中Option为ShowDirsOnly
	pFileDialog->setFileMode(QFileDialog::ExistingFiles);//设置QFileDialog中Mode为DirectoryExistingFiles
	pFileDialog->setFilter( "Images (*.png *.xpm *.jpg *.jpeg *.bmp)" ); //设置过滤器，这种类型的文件可以被显示
	if ( pFileDialog->exec() == QDialog::Accepted )
    {
        imageDir= pFileDialog->directory();//获取文件夹
    }
	imageList = imageDir.entryList ( QDir::Files,QDir::Unsorted );//列出过滤后的所有文件不排序
	next();	
}
/*********************************************************************
// 函数名：next
// 功  能：下一个
// 返  回：无
*********************************************************************/
void MainWindow::next()
{
	if(index < imageList.size())
	{
		imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
		statusBar()->showMessage(imageList.at(index));//显示文件名称
        index++;
	}
    else
    {
        index = 0;
        statusBar()->showMessage("start again!!!");
    }
}
/*********************************************************************
// 函数名：prev
// 功  能：前一个
// 返  回：无
*********************************************************************/
void MainWindow::prev()
{
	if(index>0&&index<imageList.size())
	{
		imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
		statusBar()->showMessage(imageList.at(index));//显示文件名称
        index--;
    }
    else
    {
        index = imageList.size()-1;
        statusBar()->showMessage("start again!!!");
    }
}
/*********************************************************************
// 函数名：zoomIn
// 功  能：放大
// 返  回：无
*********************************************************************/
void MainWindow::zoomIn()
{
	imageWidget->scale *= 1.25;
	zoomInAct->setEnabled(imageWidget->scale < 3);
	zoomOutAct->setEnabled(imageWidget->scale > 0.333);
	imageWidget->resize(imageWidget->scale * scrollArea->size());
}
/*********************************************************************
// 函数名：zoomOut
// 功  能：缩小图片
// 返  回：无
*********************************************************************/
void MainWindow::zoomOut()
{
	imageWidget->scale *= 0.8;
	zoomInAct->setEnabled(imageWidget->scale < 3);
	imageWidget->resize(imageWidget->scale * scrollArea->size());
}
/*********************************************************************
// 函数名：retranslateUi
// 功  能：中文
// 返  回：无
*********************************************************************/
void MainWindow::retranslateUi(QWidget *LangForm)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    LangForm->setWindowTitle(QApplication::translate("LangForm","Micro2440图片浏览器","0"));
    nextAct->setText(QApplication::translate("LangForm", "下一页", "0"));
    dirAct->setText(QApplication::translate("LangForm", "打开", "0"));
    prevAct->setText(QApplication::translate("LangForm", "前一页", "0"));
    zoomInAct->setText(QApplication::translate("LangForm", "放大", "0"));
    zoomOutAct->setText(QApplication::translate("LangForm", "缩小", "0"));
    statusBar()->showMessage(tr("准备"));//显示文件名称
}
