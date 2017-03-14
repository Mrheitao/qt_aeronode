/********************************************************************************
** Form generated from reading UI file 'videodisplay.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEODISPLAY_H
#define UI_VIDEODISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_VideoDisplayClass
{
public:
    QPushButton *receiveButton;
    QPushButton *exitButton;
    QLabel *displayLabel;

    void setupUi(QDialog *VideoDisplayClass)
    {
        if (VideoDisplayClass->objectName().isEmpty())
            VideoDisplayClass->setObjectName(QString::fromUtf8("VideoDisplayClass"));
        VideoDisplayClass->resize(660, 580);
        receiveButton = new QPushButton(VideoDisplayClass);
        receiveButton->setObjectName(QString::fromUtf8("receiveButton"));
        receiveButton->setGeometry(QRect(40, 520, 160, 40));
        exitButton = new QPushButton(VideoDisplayClass);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(460, 520, 160, 40));
        displayLabel = new QLabel(VideoDisplayClass);
        displayLabel->setObjectName(QString::fromUtf8("displayLabel"));
        displayLabel->setGeometry(QRect(10, 20, 640, 480));
        displayLabel->raise();
        receiveButton->raise();
        exitButton->raise();

        retranslateUi(VideoDisplayClass);

        QMetaObject::connectSlotsByName(VideoDisplayClass);
    } // setupUi

    void retranslateUi(QDialog *VideoDisplayClass)
    {
        VideoDisplayClass->setWindowTitle(QApplication::translate("VideoDisplayClass", "\350\247\206\351\242\221\346\265\217\350\247\210\345\231\250", 0, QApplication::UnicodeUTF8));
        receiveButton->setText(QApplication::translate("VideoDisplayClass", "\346\216\245\346\224\266\350\247\206\351\242\221", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("VideoDisplayClass", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        displayLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VideoDisplayClass: public Ui_VideoDisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEODISPLAY_H
