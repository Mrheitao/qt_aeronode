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
**�ļ�����AeroNodeHelp.cpp
**���ܣ�AeroNodeHelp�ඨ���˰������档    
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
// ��������AeroNodeHelp
// ��  �ܣ��������������
// ��  �أ���
*********************************************************************/
AeroNodeHelp::AeroNodeHelp(QWidget *parent)
    : QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312")); //֧�������ļ�����ʾ 
	
    desktopGeometry = QApplication::desktop()->availableGeometry();// ��ȡ���������С
    QScrollArea *scrollArea = new QScrollArea(this);//���ù�������  
	QWidget  *W= new QWidget(scrollArea);	
	W->setMinimumSize (600,480);	
	QLabel* TITLE = new QLabel(W);  
	TITLE->setText(tr("                                   �� �� �� ��"));
	
    QLabel* label = new QLabel(W);
    label->setText(tr("\t��ʵ��ڵ�����Ϊ�ĸ�ģ�飺ϵͳģ�顢����ģ�顢\nͬ��ģ��Ͱ���ģ�顣�����ǰ����ģ��Ĺ�������Ҫ��˵\n����\n\tϵͳģ�飺��ģ������˲��԰忨�Ļ�����Ϣ������Ӳ\n����Ϣ�������Ϣ��\n\t����ģ�飺��ģ���ṩ�˽����շ����ݰ�ͨ��ģʽ��ͨ\n��ģʽ���ṩ���������շ���ͼƬ���㡢ͼƬ�ķ��ͺͽ���\n�Լ���̬·�ɵ����ù��ܡ������÷������ݰ��ļ��������\n��Ŀ���ַ����Ϣ��\n\tͬ��ģ�飺��ģ���ṩ��ϵͳʱ�䣬������԰忨���\nʱ��ͬ����\n\t�з���λ���Ͼ����պ����ѧ����ͨ�������о���\n       ��дʱ�䣺2014��9��10��"));
	QVBoxLayout *VV1 = new QVBoxLayout();//��ֱ����
	VV1->addWidget(TITLE);
	VV1->addWidget(label);
	W->setLayout(VV1);
	scrollArea->setWidget(W);	
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(scrollArea);
    setLayout(layout);	
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(QApplication::desktop(), SIGNAL(workAreaResized(int)),this, SLOT(desktopResizedall(int)));  

	setFont(QFont("wenquanyi"));//������������
}

/*********************************************************************
// ��������desktopResized
// ��  �ܣ����������С
// ��  �أ���
*********************************************************************/
void AeroNodeHelp::desktopResizedall(int screen)
{
    if (screen != 0)
        return;
    reactToSIPall();
}

/*********************************************************************
// ��������reactToSIP
// ��  �ܣ������ֽ���
// ��  �أ���
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