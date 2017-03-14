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
**�ļ�����AeroNodeFunction.cpp
**���ܣ�AeroNodeFunction�ඨ���˹��ܽ��档�ڹ��ܽ����е�����AeroNodeCCSMA
        ��Ч�����Ľڵ��ࡢAeroNodeESTDMAZZ_Backbone����͸���ĹǸɽڵ��ࡢ
        AeroNodeESTDMAZZ_SubnetGatway����͸�������������ࡢ
		AeroNodeDTDMA_Aircraft����Эͬ���ػ��ࡢAeroNodeDTDMA_Missile
		����Эͬ�ĵ����ࡣ
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
// ��������PlanePageall
// ��  �ܣ����캯��
// ��  �أ���
*********************************************************************/
AeroNodefunction::AeroNodefunction(QWidget *parent)
    : QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312")); //֧��������ʾ
    //*****************************************************************
    //���ù��ܼ��
    //*****************************************************************
    QGroupBox *pp= new QGroupBox(tr("���ܼ��"));
	QLabel *functionE=new QLabel(this);
    functionE->setText(tr("       ��ģ���ṩ�˽����շ����ݰ�ͨ��ģʽ��ͨ��ģʽ����\n�����������շ���ͼƬ���㡢ͼƬ�ķ��ͺͽ����Լ���̬·\n�ɵ����ù��ܡ������÷������ݰ��ļ�������ݼ�Ŀ���ַ\n����Ϣ��"));
	QHBoxLayout *pplayout = new QHBoxLayout;
    pplayout->addWidget(functionE);
    pp->setLayout(pplayout);
    //*****************************************************************	
    //��������ģʽ��ť�շ��Ż�
    //*****************************************************************
	receiver1 = new QPushButton( QIcon(""),tr("�ڵ�"),this);
	receiver1->setFixedSize(240,60);	
    //*****************************************************************	
    //�����źźͲ�
    //��һ�ε����ť�����һ��ʱ��ڶ��ε��������Ч
    //*****************************************************************
	connect(receiver1,SIGNAL(clicked()),this,SLOT(clientDialog_Preventjitter_General()));
    //*****************************************************************
    //��������ģʽ
    //*****************************************************************	
	QHBoxLayout *receiver1layout = new QHBoxLayout;
	receiver1layout->addWidget(receiver1);
	QHBoxLayout *receiver3Hlayout = new QHBoxLayout;

	QHBoxLayout *pp2layout = new QHBoxLayout;
	
    QGroupBox *module1 = new QGroupBox(tr("ͨ��ģʽ"));
	//module1->setAlignment (Qt::AlignHCenter);
	module1->setLayout(receiver1layout);
	pp2layout->addWidget(module1);

    //*****************************************************************	
    //���ֽ����ָ����������
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
// ��������clientDialog_DJXT_plane
// ��  �ܣ�ͨ��ģʽ�ڵ㷢�ͽ��汳��
// ��  �أ���
*********************************************************************/
void AeroNodefunction::clientDialog_General()
{
	//���ñ��뷽ʽ
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    AeroNodeGeneral *udpclient_General = new AeroNodeGeneral();//����������
	udpclient_General->setMaximumSize(1280,800);//���ô�С
	QPalette p;//����
    p.setBrush(udpclient_General->backgroundRole(),QBrush(QColor("#C1D2F0")));
    udpclient_General->setPalette(p);
    udpclient_General->setAutoFillBackground(true);
    udpclient_General->showFullScreen();
	udpclient_General->setFont(QFont("wenquanyi"));
}





/*********************************************************************
// ��������clientDialog_Preventjitter_General
// ��  �ܣ�����ͨ��ģʽ��������ɰ�ť����
// ��  �أ���
*********************************************************************/
void AeroNodefunction::clientDialog_Preventjitter_General()                   
{   
    if(!isStarted_module)
	{  
	   m_timer_module->stop();//m_timer_module=new QTimer(this)
	   clientDialog_General();//��ͨ�ý���
	   isStarted_module = true;
	   m_second_module=0; 
	   m_msecond_module=0;
	   m_timer_module->start(100);
	}
}

/*********************************************************************
// ��������timerStrDisplay_module
// ��  �ܣ���ʱ������
// ��  �أ���
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