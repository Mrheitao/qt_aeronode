/*********************************************************************
**�ļ�������AeroNode
**Copyright (c) 2014 �Ͼ����պ��� ����ͨ�������о���
**�����ˣ����ž�
**���ڣ�2014.3
**������AN-1�Ͱ�ʵ����Խڵ����AeroNode�ǻ���QT�����Linuxϵͳ�Ľ��棬
        �ý����ࡢ����������ṹ�������ˣ����к�ǿ�Ŀɶ�������޸��ԡ�
		�ý����ṩ���ĸ�ģ�飬�ֱ�Ϊ��
		1��ϵͳ��AeroNodeSystem��ģ��
		2�����ܣ�AeroNodeFunction��ģ�飬����ģ�黹����ͨ��ģʽ�͵���Эͬ������ģ�顣
		   ͨ��ģʽ������AeroNodeGeneral�ļ��ж��壻
		   ����Эͬ���ػ��ڵ���AeroNodeDTDMA_Aircraft�ļ��ж��壻
		   ����Эͬ�ĵ����ڵ���AeroNodeDTDMA_Missile�ļ��ж��塣
		3��ͬ����AeroNodeSynchronization��ģ��
		4��������AeroNodeHelp��ģ��
		AeroNodeConfigDialogall�ļ��������������Ĳ��֡�
		AeroNodePictureViewer�ļ���������ͼƬ������Ľ��档
		AeroNodeVirtualKeyboard�ļ���������������̡�
**�汾��AN-1��AeroNode��ʵ����Խڵ����
**�ļ�����AeroNodeConfig.h
**���ܣ�AeroNodeConfig.h�ļ����������д洢ͼƬ�Ľṹ�塣 
*********************************************************************/

struct receivebuf//ʵ��ͬʱ���ܲ�ͬ�ڵ㷢�͵�ͼƬ
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

