/*********************************************************************
**文件夹名：AeroNode
**Copyright (c) 2014 南京航空航天 航空通信网络研究室
**创建人：张雅静
**日期：2014.3
**描述：AN-1型半实物测试节点界面AeroNode是基于QT编程与Linux系统的界面，
        该界面简洁、清晰，代码结构清晰明了，具有很强的可读性与可修改性。
		该界面提供了四个模块，分别为：
		1、系统（AeroNodeSystem）模块
		2、功能（AeroNodeFunction）模块，功能模块还包含通用模式和弹机协同两个子模块。
		   通用模式界面在AeroNodeGeneral文件中定义；
		   弹机协同的载机节点在AeroNodeDTDMA_Aircraft文件中定义；
		   弹机协同的导弹节点在AeroNodeDTDMA_Missile文件中定义。
		3、同步（AeroNodeSynchronization）模块
		4、帮助（AeroNodeHelp）模块
		AeroNodeConfigDialogall文件设置了整体界面的布局。
		AeroNodePictureViewer文件中设置了图片浏览器的界面。
		AeroNodeVirtualKeyboard文件中设置了虚拟键盘。
**版本：AN-1型AeroNode半实物测试节点界面
**文件名：AeroNodeConfig.h
**功能：AeroNodeConfig.h文件定义拍照中存储图片的结构体。 
*********************************************************************/

struct receivebuf//实现同时接受不同节点发送的图片
{
    QHostAddress source_addr;
    quint16 source_port;
	FILE *picfd;
	receivebuf *next;
};

//struct socketrevbuf
//{
//	QHostAddress source_addr;
//    quint16 source_port;
//	char recvBuf[10240];
//	bool havepacket;
//	socketrevbuf *next;
//};

