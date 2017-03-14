######################################################################
# Automatically generated by qmake (2.01a) Mon Jan 16 03:26:27 2017
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network
INCLUDEPATH += -I /usr/local/include/jrtplib3/ -ljrtp
LIBS += -L /lib/ -ljrtp -ljthread
CONFIG+= qt warn_off
# Input
HEADERS += AeroNodeConfig.h \
           AeroNodeConfigDialogall.h \
           AeroNodeFunction.h \
           AeroNodeGeneral.h \
           AeroNodeHelp.h \
           AeroNodeImageWidget.h \
           AeroNodePictureViewer.h \
           AeroNodeSynchronization.h \
           AeroNodeSystem.h \
           AeroNodeVirtualKeyboard.h \
           CirculaerQueue.h \
           color_space_convertor.h \
           mfc_errno.h \
           mfc_interface.h \
           mfcenc.h \
           mylib.h \
           rtpsend.h \
           sec_format.h \
           sec_utils_v4l2.h \
           SsbSipMfcApi.h \
           ui_AeroNodeVirtualKeyboard.h \
           videodev2.h \
           videodev2_exynos_media.h \
           videodisplay.h
FORMS += videodisplay.ui
SOURCES += AeroNodeConfigDialogall.cpp \
           AeroNodeFunction.cpp \
           AeroNodeGeneral.cpp \
           AeroNodeHelp.cpp \
           AeroNodeImageWidget.cpp \
           AeroNodePictureViewer.cpp \
           AeroNodeSynchronization.cpp \
           AeroNodeSystem.cpp \
           AeroNodeVirtualKeyboard.cpp \
           color_space_convertor.c \
           main.cpp \
           mfcenc.c \
           rtpsendmylib.cpp \
           SsbSipMfcDecAPI.c \
           videodisplay.cpp
RESOURCES += configdialog.qrc videodisplay.qrc
