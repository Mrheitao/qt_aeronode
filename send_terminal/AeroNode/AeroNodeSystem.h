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
**�ļ�����AeroNodeSystem.h
**���ܣ�AeroNodeSystem.h�ļ���Ҫ������AeroNodeSystemϵͳ�����ࡣ   
*********************************************************************/


#define PAGES_H
#include<QDialog>
#include<QSystemTrayIcon>
#include <QtCore/QTextCodec>
#include <QtGui>
#include <QWidget>
#include <QDate>
#include <QMainWindow>
#include <QtNetwork>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

class QTimer;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QLabel;
class QTimeEdit;
class QLCDNumber;
class QVariant;
class QTime;
class QAction;
class QTextBrowser;
class QCheckBox;
/*********************************************************************
// ��  ����AeroNodeSystem
// ��  �ܣ�ϵͳ���������
*********************************************************************/
class AeroNodeSystem : public QWidget
{
public:
    AeroNodeSystem(QWidget *parent = 0);
};