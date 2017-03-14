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
**�ļ�����AeroNodeConfigDialogall
**���ܣ�AeroNodeConfigDialogall���Ƕ���������������˲������ã�����
        ������AeroNodeSystem��ϵͳ���ࡢAeroNodeFunction�����ܣ��ࡢ
		AeroNodeSynchronization��ͬ�������AeroNodeHelp���������ࡣ    
*********************************************************************/
#include <QtGui>
#include "AeroNodeConfigDialogall.h"
#include "AeroNodeSystem.h"
#include "AeroNodeFunction.h"
#include "AeroNodeSynchronization.h"
#include "AeroNodeHelp.h"
/*********************************************************************
// ��������AeroNodeConfigDialogall
// ��  �ܣ������������
// ��  �أ���
*********************************************************************/
AeroNodeConfigDialogall::AeroNodeConfigDialogall()
{
    //*****************************************************************  
    //�����б����
    //*****************************************************************
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);//ͼ����ʾ
    contentsWidget->setIconSize(QSize(240,104));
	contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(180);
    contentsWidget->setSpacing(15);//���15
    contentsWidget->setStyleSheet("background-color:rgb(193,210,240)");
	contentsWidget->setCurrentRow(0);  //Ĭ����ʾϵͳ��
    //*****************************************************************	
    //��QStackedWidget����������
    //*****************************************************************
    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(new AeroNodeSystem);//ϵͳ
    pagesWidget->addWidget(new AeroNodefunction);//����
    pagesWidget->addWidget(new AeroNodeSynchronization);//ͬ��
    pagesWidget->addWidget(new AeroNodeHelp);//����
    
    createIconsall();//����ͼ��	
    //*****************************************************************
    //���ñ�ǩ
    //*****************************************************************
    QLabel *nuaa=new QLabel( tr("�Ͼ����պ����ѧ����ͨ�������о���"),this);
    QFont font("wenquanyi",12);
    nuaa->setFont(font);  
    nuaa->setAlignment(Qt::AlignHCenter);
    //*****************************************************************	
    //���񲼾�ҳ��
    //*****************************************************************
	QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(contentsWidget, 0, 0,1,1);
	mainLayout->addWidget(pagesWidget, 0, 1,1,1);
    mainLayout->addWidget(nuaa,1,0,1,2);
	mainLayout->setContentsMargins ( 35, 25,20, 10);//setContentsMargins ( int left, int top, int right, int bottom )
	
    setLayout(mainLayout);
    setGeometry(0,0,1280,800);    
    setFont(QFont("wenquanyi"));//���ô�С����������
    setWindowFlags(Qt::FramelessWindowHint);//ȥ���߿�
}

/*********************************************************************
// ��������createIcons
// ��  �ܣ��� contensWidget �д���ͼ��
// ��  �أ���
*********************************************************************/
void AeroNodeConfigDialogall::createIconsall()
{ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//���������������
    //*****************************************************************
    //ϵͳ��ť
    //*****************************************************************
    QListWidgetItem *configButton_ALL = new QListWidgetItem(contentsWidget);
    configButton_ALL->setIcon(QIcon(":/images/home3.png"));
	configButton_ALL->setText(tr("ϵͳ"));
    configButton_ALL->setTextAlignment(Qt::AlignHCenter);
    configButton_ALL->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //*****************************************************************
    //ģ�鰴ť
    //*****************************************************************
    QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
    updateButton->setIcon(QIcon(":/images/module.png"));
	updateButton->setText(tr("����"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //*****************************************************************
    //ʱ�䰴ť
    //*****************************************************************
    QListWidgetItem *synchronizationButton = new QListWidgetItem(contentsWidget);
    synchronizationButton->setIcon(QIcon(":/images/synchronization.png"));
	synchronizationButton->setText(tr("ͬ��"));
    synchronizationButton->setTextAlignment(Qt::AlignHCenter);
    synchronizationButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
   //*****************************************************************
   //������ť
   //*****************************************************************
	QListWidgetItem *showtime = new QListWidgetItem(contentsWidget);
    showtime ->setIcon(QIcon(":/images/help3.png"));
	showtime ->setText(tr("����"));
    showtime ->setTextAlignment(Qt::AlignHCenter);
    showtime ->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
   //*****************************************************************
   //������Ӧͼ�갴ť���䴥���Ĳ�
   //*****************************************************************
    connect(contentsWidget,SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),this, SLOT(changePageall(QListWidgetItem *, QListWidgetItem*)));
}

/*********************************************************************
// ��������changePage
// ��  �ܣ��ı���ʾҳ
// ��  �أ���
*********************************************************************/
void AeroNodeConfigDialogall::changePageall(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}






