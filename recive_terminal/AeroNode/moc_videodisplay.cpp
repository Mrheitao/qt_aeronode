/****************************************************************************
** Meta object code from reading C++ file 'videodisplay.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "videodisplay.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videodisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReceiveVideosThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_ReceiveVideosThread[] = {
    "ReceiveVideosThread\0"
};

void ReceiveVideosThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ReceiveVideosThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ReceiveVideosThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReceiveVideosThread,
      qt_meta_data_ReceiveVideosThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReceiveVideosThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReceiveVideosThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReceiveVideosThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiveVideosThread))
        return static_cast<void*>(const_cast< ReceiveVideosThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReceiveVideosThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_VideoDisplay[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      30,   13,   13,   13, 0x08,
      42,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VideoDisplay[] = {
    "VideoDisplay\0\0receiveVideos()\0flushBuff()\0"
    "closeWidget()\0"
};

void VideoDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoDisplay *_t = static_cast<VideoDisplay *>(_o);
        switch (_id) {
        case 0: _t->receiveVideos(); break;
        case 1: _t->flushBuff(); break;
        case 2: _t->closeWidget(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData VideoDisplay::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VideoDisplay::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_VideoDisplay,
      qt_meta_data_VideoDisplay, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoDisplay::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoDisplay))
        return static_cast<void*>(const_cast< VideoDisplay*>(this));
    return QDialog::qt_metacast(_clname);
}

int VideoDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
