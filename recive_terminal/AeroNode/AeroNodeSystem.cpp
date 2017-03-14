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
**�ļ�����AeroNodeSystem.cpp
**���ܣ�AeroNodeSystem���ϵͳ�������Ӳ����Ϣ�����˼򵥽��ܡ�    
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
// ��������AeroNodeSystem
// ��  �ܣ���ʾ������Ϣ 
// ��  �أ���
*********************************************************************/
AeroNodeSystem::AeroNodeSystem(QWidget *parent)
    : QWidget(parent)
{ 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//�趨�������,֧����ʾ����
	
    QVBoxLayout *Vlayout = new QVBoxLayout;//��ֱ����
	Vlayout->setSpacing(20);//���20
    //*****************************************************************	
    //��ʾӲ����Ϣ
    //*****************************************************************
	QGroupBox *Hostgroup= new QGroupBox(tr("Ӳ����Ϣ"),this);
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
    //��������
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
    //��ʾϵͳ��Ϣ
    //*****************************************************************
	QGroupBox *Hostsystem= new QGroupBox(tr("�����Ϣ"),this);
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
    //��ʾ����IP
    //*****************************************************************
    QHBoxLayout *IPlayout = new QHBoxLayout; 
    QLabel *LabelHostIP=new QLabel(this);	
	QLabel *Interface=new QLabel(this);
    //*****************************************************************
    //��ʾIP��ַ
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
    //��ֱ����
    //*****************************************************************			
    QVBoxLayout *mainlayoutall = new QVBoxLayout;
	mainlayoutall->addLayout(Vlayout);
    //mainlayoutall->addStretch(1);
    setLayout(mainlayoutall);    
	setFont(QFont("wenquanyi"));
}

