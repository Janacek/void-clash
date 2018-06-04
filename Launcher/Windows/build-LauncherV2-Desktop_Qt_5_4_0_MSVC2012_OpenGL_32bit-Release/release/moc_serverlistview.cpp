/****************************************************************************
** Meta object code from reading C++ file 'serverlistview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LauncherV2/serverlistview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverlistview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ServerListView_t {
    QByteArrayData data[10];
    char stringdata[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServerListView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServerListView_t qt_meta_stringdata_ServerListView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ServerListView"
QT_MOC_LITERAL(1, 15, 19), // "__refreshServerList"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "__displayIpInput"
QT_MOC_LITERAL(4, 53, 15), // "__removeIpInput"
QT_MOC_LITERAL(5, 69, 19), // "__sortServersByName"
QT_MOC_LITERAL(6, 89, 19), // "__sortServersByMode"
QT_MOC_LITERAL(7, 109, 18), // "__sortServersByMap"
QT_MOC_LITERAL(8, 128, 23), // "__sortServersByPlayerNb"
QT_MOC_LITERAL(9, 152, 17) // "__connectToServer"

    },
    "ServerListView\0__refreshServerList\0\0"
    "__displayIpInput\0__removeIpInput\0"
    "__sortServersByName\0__sortServersByMode\0"
    "__sortServersByMap\0__sortServersByPlayerNb\0"
    "__connectToServer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServerListView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ServerListView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ServerListView *_t = static_cast<ServerListView *>(_o);
        switch (_id) {
        case 0: _t->__refreshServerList(); break;
        case 1: _t->__displayIpInput(); break;
        case 2: _t->__removeIpInput(); break;
        case 3: _t->__sortServersByName(); break;
        case 4: _t->__sortServersByMode(); break;
        case 5: _t->__sortServersByMap(); break;
        case 6: _t->__sortServersByPlayerNb(); break;
        case 7: _t->__connectToServer(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ServerListView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ServerListView.data,
      qt_meta_data_ServerListView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ServerListView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerListView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ServerListView.stringdata))
        return static_cast<void*>(const_cast< ServerListView*>(this));
    return QWidget::qt_metacast(_clname);
}

int ServerListView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
