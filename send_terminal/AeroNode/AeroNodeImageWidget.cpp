/*********************************************************************
**文件夹名：AeroNode
**Copyright (c) 2012-2013 南京航空航天 航空通信网络研究室
**创建人：李科艳、张雅静
**日期：2013.10
**描述：AN-1型半实物测试节点界面AeroNode是基于QT编程与Linux系统的界面，
        该界面简洁、清晰，代码结构清晰明了，具有很强的可读性与可修改性。
		该界面提供了四个模块，分别为：
		1、系统（AeroNodeSystem）模块
		2、功能（AeroNodeFunction）模块，功能模块还包含高效并发、拓扑
		   透明和弹机协同三个子模块。
		   高效并发界面在AeroNodeCCSMA文件中定义；
		   拓扑透明的骨干节点在AeroNodeESTDMA_Backbone文件中定义；
		   拓扑透明的子网网关节点在AeroNodeESTDMAZZ_SubnetGatway文件中定义；
		   弹机协同的载机节点在AeroNodeDTDMA_Aircraft文件中定义；
		   弹机协同的导弹节点在AeroNodeDTDMA_Missile文件中定义。
		3、同步（AeroNodeSynchronization）模块
		4、帮助（AeroNodeHelp）模块
		AeroNodeConfigDialogall文件设置了整体界面的布局。
		AeroNodePictureViewer文件中设置了图片浏览器的界面。
		AeroNodeVirtualKeyboard文件中设置了虚拟键盘。
**版本：AN-1型AeroNode半实物测试节点界面
**文件名：AeroNodeImageWidget.cpp
**功能：AeroNodeImageWidget文件定义了AeroNodeImageWidget类，是图像浏览
        器的一个部件。
*********************************************************************/


#include <QtCore>
#include <QtDebug>
#include "AeroNodeImageWidget.h"
/*********************************************************************
// 函数名：ImageWidget
// 功  能：图像浏览器部件
// 返  回：无
*********************************************************************/
ImageWidget::ImageWidget(QWidget *parent)
	: QWidget(parent)
{
	QDesktopWidget desktop;//获取与桌面相关的各种信息
	pixmap = QPixmap(desktop.width(), desktop.height());//获取desktop的宽和高，并创建加载图像对象pixmap
    backColor=qRgb(128,128,128);//设置画布背景
    pixmap.fill(backColor);//填充背景色
	scale = 1;
	angle = 0;
	bFit = true;
}

/*********************************************************************
// 函数名：paintEvent
// 功  能：重新绘制事件
// 返  回：无
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
// 函数名：setPixmap
// 功  能：设置目标
// 返  回：无
*********************************************************************/
void ImageWidget::setPixmap(QString fileName)
{
	pixmap.load(fileName);//目标加载文件
	update();//更新
}

/*********************************************************************
// 函数名：getPixmap
// 功  能：获取图像
// 返  回：pixmap加载文件
*********************************************************************/
QPixmap ImageWidget::getPixmap()
{
	return pixmap;
}

/*********************************************************************
// 函数名：setAngle
// 功  能：设置参数
// 返  回：无
*********************************************************************/
void ImageWidget::setAngle(qreal rotateAngle)
{
	angle += rotateAngle;
	update();
}

/*********************************************************************
// 函数名：dragEnterEvent
// 功  能：当拖拽后进入接收拖拽的控件时触发
// 返  回：无
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
// 函数名：dropEvent
// 功  能：拖拽状态下释放鼠标时触发
// 返  回：无
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
// 函数名：dragLeaveEvent
// 功  能：当拖拽(没有释放鼠标时)离开接收拖拽浏览器时触发
// 返  回：无
*********************************************************************/
void ImageWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
 	event->accept();
}