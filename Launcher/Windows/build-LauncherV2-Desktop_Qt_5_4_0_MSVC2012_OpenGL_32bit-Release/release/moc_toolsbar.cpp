/****************************************************************************
** Meta object code from reading C++ file 'toolsbar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LauncherV2/toolsbar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolsbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolsBar_t {
    QByteArrayData data[3];
    char stringdata[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolsBar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolsBar_t qt_meta_stringdata_ToolsBar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ToolsBar"
QT_MOC_LITERAL(1, 9, 17), // "__launchMapEditor"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "ToolsBar\0__launchMapEditor\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolsBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ToolsBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolsBar *_t = static_cast<ToolsBar *>(_o);
        switch (_id) {
        case 0: _t->__launchMapEditor(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ToolsBar::staticMetaObject = {
    { &Field::staticMetaObject, qt_meta_stringdata_ToolsBar.data,
      qt_meta_data_ToolsBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ToolsBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolsBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ToolsBar.stringdata))
        return static_cast<void*>(const_cast< ToolsBar*>(this));
    return Field::qt_metacast(_clname);
}

int ToolsBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Field::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
