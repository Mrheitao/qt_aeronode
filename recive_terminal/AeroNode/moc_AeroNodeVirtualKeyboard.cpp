/****************************************************************************
** Meta object code from reading C++ file 'AeroNodeVirtualKeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AeroNodeVirtualKeyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AeroNodeVirtualKeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WidgetKeyboard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x08,
      48,   40,   15,   15, 0x08,
      73,   40,   15,   15, 0x08,
     103,   40,   15,   15, 0x08,
     134,   40,   15,   15, 0x08,
     163,   40,   15,   15, 0x08,
     191,   15,   15,   15, 0x08,
     234,  226,   15,   15, 0x08,
     263,  256,   15,   15, 0x08,
     295,  289,  284,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WidgetKeyboard[] = {
    "WidgetKeyboard\0\0btn\0on_btn_clicked(int)\0"
    "checked\0on_btnCaps_toggled(bool)\0"
    "on_btnShiftLeft_toggled(bool)\0"
    "on_btnShiftRight_toggled(bool)\0"
    "on_btnCtrlLeft_toggled(bool)\0"
    "on_btnAltLeft_toggled(bool)\0"
    "on_sliderOpacity_valueChanged(int)\0"
    "isShift\0changeTextShift(bool)\0isCaps\0"
    "changeTextCaps(bool)\0bool\0keyId\0"
    "checkNotTextKey(int)\0"
};

void WidgetKeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WidgetKeyboard *_t = static_cast<WidgetKeyboard *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_btnCaps_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_btnShiftLeft_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_btnShiftRight_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_btnCtrlLeft_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_btnAltLeft_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_sliderOpacity_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->changeTextShift((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->changeTextCaps((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: { bool _r = _t->checkNotTextKey((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WidgetKeyboard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WidgetKeyboard::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WidgetKeyboard,
      qt_meta_data_WidgetKeyboard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WidgetKeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WidgetKeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WidgetKeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WidgetKeyboard))
        return static_cast<void*>(const_cast< WidgetKeyboard*>(this));
    if (!strcmp(_clname, "Ui::WidgetKeyboard"))
        return static_cast< Ui::WidgetKeyboard*>(const_cast< WidgetKeyboard*>(this));
    return QWidget::qt_metacast(_clname);
}

int WidgetKeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
