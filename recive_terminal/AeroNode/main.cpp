/*********************************************************************
**�ļ�������AeroNode
**Copyright (c) 2016-2017 �Ͼ����պ��� ����ͨ�������о���
**�����ˣ����ƿ�
**���ڣ�2016.12.
**������
**�汾��AN-1��AeroNode��ʵ����Խڵ����
**�ļ�����main.cpp
**���ܣ�main����������QT���̵���ں������ڸú�������Ҫ������ϵͳ������
        ���õ�AeroNodeConfigDialogall�ࡣ
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeConfigDialogall.h"
/*********************************************************************
// ��������main
// ��  �ܣ���ʼ������
// ��  �أ�Qt���պʹ����û���ϵͳ�¼���������Щ�¼����ݸ���Ӧ�Ĵ���
*********************************************************************/
int main(int argc, char *argv[])
{
    //*****************************************************************
    //��ʼ������
	//*****************************************************************
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//���ÿ���ʾ����
    QApplication app(argc, argv);
    AeroNodeConfigDialogall dialog;
    QFont font; 
    font.setPointSize(12); 
    font.setFamily(("wenquanyi")); 
    font.setBold(false); 
    app.setFont(font);
	//*****************************************************************
    //��������
	//*****************************************************************
    QPalette p;
    p.setBrush(dialog.backgroundRole(),QBrush(QColor("#C1D2F0")));
    dialog.setPalette(p);
    dialog.setAutoFillBackground(true);	
    dialog.show();//��ʾ������
    return dialog.exec();//�¼����ݸ���Ӧ�ĳ���
}