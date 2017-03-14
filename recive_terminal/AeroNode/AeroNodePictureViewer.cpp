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
**�ļ�����AeroNodePictureViewer.cpp
**���ܣ�AeroNodePictureViewer�ļ�������AeroNodePictureViewerͼ�����
        ���ࡣ
*********************************************************************/


#include "AeroNodePictureViewer.h"
#include <QTranslator>
#include <QDebug>
#include <QTextCodec>
extern QTranslator *tor;
extern MainWindow *w;
/*********************************************************************
// ��������MainWindow
// ��  �ܣ�ͼ�������
// ��  �أ���
*********************************************************************/
MainWindow::MainWindow()
{
	imageWidget = new ImageWidget;
	
	this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	this->setWindowIcon(QIcon(" "));//ȥ��������ͼ��
	this->setFixedSize(900,600);//�̶���С
	
    scrollArea = new QScrollArea;
	scrollArea->setPalette(QPalette( QColor(128,128,128) ) );
    scrollArea->setWidget(imageWidget);
	scrollArea->widget()->setMaximumSize(1280,720);	
	setCentralWidget(scrollArea);
    createActions();
    createToolBars();
    createStatusBar();
    //���ô��ڱ���
    setWindowTitle(tr("ͼ�������"));
	setFocusPolicy(Qt::StrongFocus);
	index = 0;
    //imageDir.setPath(QDir::currentPath());//��ȡ��ǰ�ļ���·��
    //imageDir.setPath("G:/100CASIO");
	//QStringList filter;
    //set file filter
	//filter << "*.jpg" << "*.bmp" << "*.jpeg" << "*.png" << "*.xpm";
	//imageList = imageDir.entryList ( filter, QDir::Files );//���ļ��е��б����imagelist����
	//imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
	//next();
	//currentFile = imageList.constBegin();
}
/*********************************************************************
// ��������resizeEvent
// ��  �ܣ����ο�
// ��  �أ���
*********************************************************************/
void MainWindow::resizeEvent(QResizeEvent * event)
{
	QRect childRect = scrollArea->childrenRect();
	imageWidget->resize(childRect.size());
}
/*********************************************************************
// ��������createActions
// ��  �ܣ������˵�
// ��  �أ���
*********************************************************************/
void MainWindow::createActions()
{
    //*****************************************************************
    //��
    //*****************************************************************
    dirAct = new QAction(QIcon(":/images/open.png"), tr("Open"), this);
    dirAct->setShortcut(QKeySequence::Open);
    connect(dirAct, SIGNAL(triggered()), this, SLOT(selectDir()));
    //*****************************************************************
    //ǰһ��
    //*****************************************************************
    prevAct = new QAction(QIcon(":/images/previous.png"), tr("Previous"), this);
    prevAct->setShortcut(QKeySequence::Back);
	prevAct->setEnabled(false);
    connect(prevAct, SIGNAL(triggered()), this, SLOT(prev()));
    //*****************************************************************
    //��һ��
    //*****************************************************************
    nextAct = new QAction(QIcon(":/images/next.png"), tr("Next"), this);
    nextAct->setShortcut(QKeySequence::Forward);
	nextAct->setEnabled(false);
    connect(nextAct, SIGNAL(triggered()), this, SLOT(next()));
    //*****************************************************************
    //�Ŵ�
    //***************************************************************** 
    zoomInAct = new QAction(QIcon(":/images/zoomin.png"), tr("ZoomIn"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
	zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    //*****************************************************************
    //��С
    //*****************************************************************
    zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), tr("ZoomOut"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
}
/*********************************************************************
// ��������createToolBars
// ��  �ܣ����Ӳ˵�������
// ��  �أ���
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
// ��������createStatusBar
// ��  �ܣ�����״̬��
// ��  �أ���
*********************************************************************/
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
/*********************************************************************
// ��������selectDir
// ��  �ܣ�ѡ�񴰿�
// ��  �أ���
*********************************************************************/
void MainWindow::selectDir()
{
    prevAct->setEnabled(true);
	nextAct->setEnabled(true);
	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	pFileDialog = new QFileDialog(this, tr("Open File"),"/mnt/hgft/",NULL);//����һ�����ڣ����ڱ���Ϊopen file
    pFileDialog->setGeometry(0,0,1280,800); //���öԻ����СΪ���������Ļ��С
    pFileDialog->setOption(QFileDialog::ShowDirsOnly);//����QFileDialog��OptionΪShowDirsOnly
	pFileDialog->setFileMode(QFileDialog::ExistingFiles);//����QFileDialog��ModeΪDirectoryExistingFiles
	pFileDialog->setFilter( "Images (*.png *.xpm *.jpg *.jpeg *.bmp)" ); //���ù��������������͵��ļ����Ա���ʾ
	if ( pFileDialog->exec() == QDialog::Accepted )
    {
        imageDir= pFileDialog->directory();//��ȡ�ļ���
    }
	imageList = imageDir.entryList ( QDir::Files,QDir::Unsorted );//�г����˺�������ļ�������
	next();	
}
/*********************************************************************
// ��������next
// ��  �ܣ���һ��
// ��  �أ���
*********************************************************************/
void MainWindow::next()
{
	if(index < imageList.size())
	{
		imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
		statusBar()->showMessage(imageList.at(index));//��ʾ�ļ�����
        index++;
	}
    else
    {
        index = 0;
        statusBar()->showMessage("start again!!!");
    }
}
/*********************************************************************
// ��������prev
// ��  �ܣ�ǰһ��
// ��  �أ���
*********************************************************************/
void MainWindow::prev()
{
	if(index>0&&index<imageList.size())
	{
		imageWidget->setPixmap(imageDir.absolutePath() + QDir::separator() + imageList.at(index));
		statusBar()->showMessage(imageList.at(index));//��ʾ�ļ�����
        index--;
    }
    else
    {
        index = imageList.size()-1;
        statusBar()->showMessage("start again!!!");
    }
}
/*********************************************************************
// ��������zoomIn
// ��  �ܣ��Ŵ�
// ��  �أ���
*********************************************************************/
void MainWindow::zoomIn()
{
	imageWidget->scale *= 1.25;
	zoomInAct->setEnabled(imageWidget->scale < 3);
	zoomOutAct->setEnabled(imageWidget->scale > 0.333);
	imageWidget->resize(imageWidget->scale * scrollArea->size());
}
/*********************************************************************
// ��������zoomOut
// ��  �ܣ���СͼƬ
// ��  �أ���
*********************************************************************/
void MainWindow::zoomOut()
{
	imageWidget->scale *= 0.8;
	zoomInAct->setEnabled(imageWidget->scale < 3);
	imageWidget->resize(imageWidget->scale * scrollArea->size());
}
/*********************************************************************
// ��������retranslateUi
// ��  �ܣ�����
// ��  �أ���
*********************************************************************/
void MainWindow::retranslateUi(QWidget *LangForm)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    LangForm->setWindowTitle(QApplication::translate("LangForm","Micro2440ͼƬ�����","0"));
    nextAct->setText(QApplication::translate("LangForm", "��һҳ", "0"));
    dirAct->setText(QApplication::translate("LangForm", "��", "0"));
    prevAct->setText(QApplication::translate("LangForm", "ǰһҳ", "0"));
    zoomInAct->setText(QApplication::translate("LangForm", "�Ŵ�", "0"));
    zoomOutAct->setText(QApplication::translate("LangForm", "��С", "0"));
    statusBar()->showMessage(tr("׼��"));//��ʾ�ļ�����
}
