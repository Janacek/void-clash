/****************************************************************************
** Meta object code from reading C++ file 'gameconfigview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LauncherV2/gameconfigview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gameconfigview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GameConfigView_t {
    QByteArrayData data[10];
    char stringdata[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GameConfigView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GameConfigView_t qt_meta_stringdata_GameConfigView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "GameConfigView"
QT_MOC_LITERAL(1, 15, 15), // "__normalMatchCB"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 15), // "__rankedMatchCB"
QT_MOC_LITERAL(4, 48, 6), // "__dmCB"
QT_MOC_LITERAL(5, 55, 7), // "__ffaCB"
QT_MOC_LITERAL(6, 63, 7), // "__ctfCB"
QT_MOC_LITERAL(7, 71, 8), // "__tsvrCB"
QT_MOC_LITERAL(8, 80, 7), // "__horCB"
QT_MOC_LITERAL(9, 88, 8) // "__kothCB"

    },
    "GameConfigView\0__normalMatchCB\0\0"
    "__rankedMatchCB\0__dmCB\0__ffaCB\0__ctfCB\0"
    "__tsvrCB\0__horCB\0__kothCB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameConfigView[] = {

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

void GameConfigView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GameConfigView *_t = static_cast<GameConfigView *>(_o);
        switch (_id) {
        case 0: _t->__normalMatchCB(); break;
        case 1: _t->__rankedMatchCB(); break;
        case 2: _t->__dmCB(); break;
        case 3: _t->__ffaCB(); break;
        case 4: _t->__ctfCB(); break;
        case 5: _t->__tsvrCB(); break;
        case 6: _t->__horCB(); break;
        case 7: _t->__kothCB(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GameConfigView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GameConfigView.data,
      qt_meta_data_GameConfigView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GameConfigView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameConfigView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GameConfigView.stringdata))
        return static_cast<void*>(const_cast< GameConfigView*>(this));
    return QWidget::qt_metacast(_clname);
}

int GameConfigView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
