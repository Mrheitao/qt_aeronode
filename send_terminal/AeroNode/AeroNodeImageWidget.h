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
**�ļ�����AeroNodeImageWidget.h
**���ܣ�AeroNodeImageWidget.h������AeroNodeImageWidget�࣬��ͼ�����
        ����һ��������
*********************************************************************/


#ifndef IMAGEWIDGET_H_
#define IMAGEWIDGET_H_

#include <QtGui>
/*********************************************************************
// ��  ����ImageWidget
// ��  �ܣ�ͼƬ�����������
*********************************************************************/
class ImageWidget : public QWidget
{
	Q_OBJECT	
public:
	bool bFit;
	qreal scale;
	QPixmap pixmap; 
public:
	ImageWidget(QWidget *parent = 0);
	void setPixmap(QString fileName);
	QPixmap getPixmap();
	void setAngle(qreal rotateAngle);   
protected:
	void paintEvent(QPaintEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
private:
    QRgb backColor;
	qreal angle;
};
#endif 
