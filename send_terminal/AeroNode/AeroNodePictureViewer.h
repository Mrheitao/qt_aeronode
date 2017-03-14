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
**�ļ�����AeroNodePictureViewer.h
**���ܣ�AeroNodePictureViewer.h������AeroNodePictureViewerͼ�����
        ���ࡣ
*********************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QDir>
#include <QClipboard>
#include <QMessageBox>
#include <QDirModel>
#include <QTreeWidgetItem>
#include <QLabel>
#include "AeroNodeImageWidget.h"

/*********************************************************************
// ��  ����MainWindow
// ��  �ܣ�ͼƬ�������
*********************************************************************/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void selectDir();
    void next();
    void prev();
    void zoomIn();
    void zoomOut();
    void retranslateUi(QWidget *LangForm);
protected:
	void resizeEvent(QResizeEvent * event);
private:
    void createActions();
    void createToolBars();
    void createStatusBar();
	QScrollArea *scrollArea;
    ImageWidget *imageWidget;

    QToolBar *naviToolBar;
    QAction *dirAct;
    QAction *nextAct;
    QAction *prevAct;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QFileDialog *pFileDialog;
	QStringList imageList;
	int index;
	QDir imageDir;
};
#endif
