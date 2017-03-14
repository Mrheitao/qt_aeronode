/*********************************************************************
**文件夹名：AeroNode
**Copyright (c) 2016-2017 南京航空航天 航空通信网络研究室
**创建人：张云开
**日期：2016.12.
**描述：
**版本：AN-1型AeroNode半实物测试节点界面
**文件名：main.cpp
**功能：main函数是整个QT工程的入口函数，在该函数中主要调用了系统主界面
        设置的AeroNodeConfigDialogall类。
*********************************************************************/


#include <QtCore/QTextCodec>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "AeroNodeConfigDialogall.h"
/*********************************************************************
// 函数名：main
// 功  能：初始化界面
// 返  回：Qt接收和处理用户及系统事件，并把这些事件传递给相应的窗口
*********************************************************************/
int main(int argc, char *argv[])
{
    //*****************************************************************
    //初始化界面
	//*****************************************************************
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));//设置可显示中文
    QApplication app(argc, argv);
    AeroNodeConfigDialogall dialog;
    QFont font; 
    font.setPointSize(12); 
    font.setFamily(("wenquanyi")); 
    font.setBold(false); 
    app.setFont(font);
	//*****************************************************************
    //背景设置
	//*****************************************************************
    QPalette p;
    p.setBrush(dialog.backgroundRole(),QBrush(QColor("#C1D2F0")));
    dialog.setPalette(p);
    dialog.setAutoFillBackground(true);	
    dialog.show();//显示主界面
    return dialog.exec();//事件传递给相应的出口
}