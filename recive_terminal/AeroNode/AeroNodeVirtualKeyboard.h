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
**�ļ�����AeroNodeVirtualKeyboard.h
**���ܣ�AeroNodeVirtualKeyboard������AeroNodeVirtualKeyboard��������ࡣ
*********************************************************************/


#ifndef __WIDGETKEYBOARD_H_
#define __WIDGETKEYBOARD_H_

#include <QWidget>
#include "ui_AeroNodeVirtualKeyboard.h"
class QSignalMapper;

/*********************************************************************
// ��  ����WidgetKeyboard
// ��  �ܣ����������
*********************************************************************/
class WidgetKeyboard : public QWidget, public Ui::WidgetKeyboard
{
    Q_OBJECT
    
    private:
		QWidget *m_pParent;
		qreal opacity;
		bool isCaps;
		bool isShift;
		bool isCtrl;
		bool isAlt;
		bool isIns;
		QSignalMapper *signalMapper;
		QList<QToolButton *> allButtons;        
    private slots:
		void on_btn_clicked(int btn);
		void on_btnCaps_toggled(bool checked);
		void on_btnShiftLeft_toggled(bool checked);
		void on_btnShiftRight_toggled(bool checked);
		void on_btnCtrlLeft_toggled(bool checked);
		void on_btnAltLeft_toggled(bool checked);
		void on_sliderOpacity_valueChanged(int);//͸���ȵ���
		void changeTextShift(bool isShift);
		void changeTextCaps(bool isCaps);
		bool checkNotTextKey(int keyId);		
    public:
		WidgetKeyboard(QWidget *parent = NULL);
		~WidgetKeyboard();
		void setOpacity(int);
		int getOpacity();
};
#endif 

