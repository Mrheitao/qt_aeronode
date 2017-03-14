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
static const uint qt_meta_data_SendVideosThread[] = {

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

static const char qt_meta_stringdata_SendVideosThread[] = {
    "SendVideosThread\0"
};

void SendVideosThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SendVideosThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SendVideosThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SendVideosThread,
      qt_meta_data_SendVideosThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SendVideosThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SendVideosThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SendVideosThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SendVideosThread))
        return static_cast<void*>(const_cast< SendVideosThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SendVideosThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
      26,   13,   13,   13, 0x08,
      39,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VideoDisplay[] = {
    "VideoDisplay\0\0flushBuff()\0sendVideos()\0"
    "closeWidget()\0"
};

void VideoDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoDisplay *_t = static_cast<VideoDisplay *>(_o);
        switch (_id) {
        case 0: _t->flushBuff(); break;
        case 1: _t->sendVideos(); break;
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
