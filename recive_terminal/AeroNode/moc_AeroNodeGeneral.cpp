/****************************************************************************
** Meta object code from reading C++ file 'AeroNodeGeneral.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AeroNodeGeneral.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AeroNodeGeneral.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyThread_General[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyThread_General[] = {
    "MyThread_General\0\0str\0changeText(QString)\0"
    "timerStrDisplay()\0"
};

void MyThread_General::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyThread_General *_t = static_cast<MyThread_General *>(_o);
        switch (_id) {
        case 0: _t->changeText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->timerStrDisplay(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyThread_General::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyThread_General::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MyThread_General,
      qt_meta_data_MyThread_General, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyThread_General::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyThread_General::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyThread_General::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyThread_General))
        return static_cast<void*>(const_cast< MyThread_General*>(this));
    return QThread::qt_metacast(_clname);
}

int MyThread_General::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MyThread_General::changeText(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_GetPictures_General[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   21,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_GetPictures_General[] = {
    "GetPictures_General\0\0output\0"
    "Textchange(QString)\0"
};

void GetPictures_General::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GetPictures_General *_t = static_cast<GetPictures_General *>(_o);
        switch (_id) {
        case 0: _t->Textchange((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GetPictures_General::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GetPictures_General::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_GetPictures_General,
      qt_meta_data_GetPictures_General, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GetPictures_General::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GetPictures_General::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GetPictures_General::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GetPictures_General))
        return static_cast<void*>(const_cast< GetPictures_General*>(this));
    return QThread::qt_metacast(_clname);
}

int GetPictures_General::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void GetPictures_General::Textchange(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_AeroNodeGeneral[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   17,   16,   16, 0x08,
      46,   16,   16,   16, 0x08,
      63,   16,   16,   16, 0x08,
      73,   16,   16,   16, 0x08,
      99,   16,   16,   16, 0x08,
     111,   16,   16,   16, 0x08,
     124,   16,   16,   16, 0x08,
     143,   16,   16,   16, 0x08,
     154,   16,   16,   16, 0x08,
     163,   16,   16,   16, 0x08,
     179,   16,   16,   16, 0x08,
     200,   16,   16,   16, 0x08,
     228,  221,   16,   16, 0x08,
     248,   16,   16,   16, 0x08,
     259,   16,   16,   16, 0x08,
     283,   16,   16,   16, 0x08,
     311,   16,   16,   16, 0x08,
     328,   16,   16,   16, 0x08,
     347,   16,   16,   16, 0x08,
     366,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AeroNodeGeneral[] = {
    "AeroNodeGeneral\0\0str\0timerStrDisplay(QString)\0"
    "Deal_guideinfo()\0timeout()\0"
    "PushButtonClose_clicked()\0recv_slot()\0"
    "runClicked()\0moreButton_state()\0"
    "Reseting()\0config()\0receiveVideos()\0"
    "on_btnShow_clicked()\0on_btnHide_clicked()\0"
    "output\0readOutput(QString)\0pictures()\0"
    "picture_Preventjitter()\0"
    "getpictures_Preventjitter()\0"
    "picturereceive()\0Saverouteseting1()\0"
    "Saverouteseting2()\0Saverouteseting3()\0"
};

void AeroNodeGeneral::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AeroNodeGeneral *_t = static_cast<AeroNodeGeneral *>(_o);
        switch (_id) {
        case 0: _t->timerStrDisplay((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Deal_guideinfo(); break;
        case 2: _t->timeout(); break;
        case 3: _t->PushButtonClose_clicked(); break;
        case 4: _t->recv_slot(); break;
        case 5: _t->runClicked(); break;
        case 6: _t->moreButton_state(); break;
        case 7: _t->Reseting(); break;
        case 8: _t->config(); break;
        case 9: _t->receiveVideos(); break;
        case 10: _t->on_btnShow_clicked(); break;
        case 11: _t->on_btnHide_clicked(); break;
        case 12: _t->readOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->pictures(); break;
        case 14: _t->picture_Preventjitter(); break;
        case 15: _t->getpictures_Preventjitter(); break;
        case 16: _t->picturereceive(); break;
        case 17: _t->Saverouteseting1(); break;
        case 18: _t->Saverouteseting2(); break;
        case 19: _t->Saverouteseting3(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AeroNodeGeneral::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AeroNodeGeneral::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AeroNodeGeneral,
      qt_meta_data_AeroNodeGeneral, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AeroNodeGeneral::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AeroNodeGeneral::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AeroNodeGeneral::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AeroNodeGeneral))
        return static_cast<void*>(const_cast< AeroNodeGeneral*>(this));
    return QDialog::qt_metacast(_clname);
}

int AeroNodeGeneral::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
