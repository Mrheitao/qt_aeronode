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
**�ļ�����AeroNodeImageWidget.cpp
**���ܣ�AeroNodeImageWidget�ļ�������AeroNodeImageWidget�࣬��ͼ�����
        ����һ��������
*********************************************************************/


#include <QtCore>
#include <QtDebug>
#include "AeroNodeImageWidget.h"
/*********************************************************************
// ��������ImageWidget
// ��  �ܣ�ͼ�����������
// ��  �أ���
*********************************************************************/
ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent)
{
	QDesktopWidget desktop;//��ȡ��������صĸ�����Ϣ
	pixmap = QPixmap(desktop.width(), desktop.height());//��ȡdesktop�Ŀ�͸ߣ�����������ͼ�����pixmap
    backColor=qRgb(128,128,128);//���û�������
    pixmap.fill(backColor);//��䱳��ɫ
	scale = 1;
	angle = 0;
	bFit = true;
}

/*********************************************************************
// ��������paintEvent
// ��  �ܣ����»����¼�
// ��  �أ���
*********************************************************************/
void ImageWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QDesktopWidget desktop;
	if(angle)
	{
	    QPointF center(width()/2,height()/2);
		painter.translate(center);
		painter.rotate(angle);
		painter.translate(-center);
	}
	if(bFit)
	{
		QPixmap fitPixmap = pixmap.scaled(width(),height(), Qt::KeepAspectRatio);
		painter.translate(110,5);			
		painter.drawPixmap(0,0,fitPixmap);
	}
	else
		painter.drawPixmap(0, 0, pixmap);
}

/*********************************************************************
// ��������setPixmap
// ��  �ܣ�����Ŀ��
// ��  �أ���
*********************************************************************/
void ImageWidget::setPixmap(QString fileName)
{
	pixmap.load(fileName);//Ŀ������ļ�
	update();//����
}

/*********************************************************************
// ��������getPixmap
// ��  �ܣ���ȡͼ��
// ��  �أ�pixmap�����ļ�
*********************************************************************/
QPixmap ImageWidget::getPixmap()
{
	return pixmap;
}

/*********************************************************************
// ��������setAngle
// ��  �ܣ����ò���
// ��  �أ���
*********************************************************************/
void ImageWidget::setAngle(qreal rotateAngle)
{
	angle += rotateAngle;
	update();
}

/*********************************************************************
// ��������dragEnterEvent
// ��  �ܣ�����ק����������ק�Ŀؼ�ʱ����
// ��  �أ���
*********************************************************************/
void ImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) 
	{
		QString localFile;
		QRegExp rx("\\.(jpg|bmp|jpeg|png|xpm)$", Qt::CaseInsensitive);
		foreach(QUrl url , event->mimeData()->urls()) 
		{
			localFile = url.toLocalFile();
			if(rx.indexIn(localFile) >= 0) 
			{
				event->accept();
				return;
			}
			else 
			{
			     event->ignore();
			}
		}
    } else 
	{
        event->ignore();
    }
}

/*********************************************************************
// ��������dropEvent
// ��  �ܣ���ק״̬���ͷ����ʱ����
// ��  �أ���
*********************************************************************/
void ImageWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) 
	{
		QString localFile;
		QRegExp rx("\\.(jpg|bmp|jpeg|png|xpm)$", Qt::CaseInsensitive);
		foreach(QUrl url , event->mimeData()->urls())
		{
			localFile = url.toLocalFile();
			if(rx.indexIn(localFile) >= 0) 
			{
				event->accept();
				setPixmap(localFile);
				return;
			}
			else 
			{
			    event->ignore();
			}
		}
    } 
	else 
	{
        event->ignore();
    }
}

/*********************************************************************
// ��������dragLeaveEvent
// ��  �ܣ�����ק(û���ͷ����ʱ)�뿪������ק�����ʱ����
// ��  �أ���
*********************************************************************/
void ImageWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
 	event->accept();
}