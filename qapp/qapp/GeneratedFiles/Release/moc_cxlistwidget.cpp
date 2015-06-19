/****************************************************************************
** Meta object code from reading C++ file 'cxlistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cxlistwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cxlistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CxListWidget_t {
    QByteArrayData data[12];
    char stringdata[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CxListWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CxListWidget_t qt_meta_stringdata_CxListWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CxListWidget"
QT_MOC_LITERAL(1, 13, 12), // "paramClicked"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "paramName"
QT_MOC_LITERAL(4, 37, 12), // "paramEntered"
QT_MOC_LITERAL(5, 50, 16), // "paramNameChanged"
QT_MOC_LITERAL(6, 67, 8), // "previous"
QT_MOC_LITERAL(7, 76, 7), // "current"
QT_MOC_LITERAL(8, 84, 21), // "listItemDoubleClicked"
QT_MOC_LITERAL(9, 106, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(10, 123, 4), // "item"
QT_MOC_LITERAL(11, 128, 15) // "listItemEntered"

    },
    "CxListWidget\0paramClicked\0\0paramName\0"
    "paramEntered\0paramNameChanged\0previous\0"
    "current\0listItemDoubleClicked\0"
    "QListWidgetItem*\0item\0listItemEntered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CxListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   45,    2, 0x0a /* Public */,
       8,    1,   50,    2, 0x08 /* Private */,
      11,    1,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void CxListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CxListWidget *_t = static_cast<CxListWidget *>(_o);
        switch (_id) {
        case 0: _t->paramClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->paramEntered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->paramNameChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->listItemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->listItemEntered((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CxListWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CxListWidget::paramClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (CxListWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CxListWidget::paramEntered)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CxListWidget::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_CxListWidget.data,
      qt_meta_data_CxListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CxListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CxListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CxListWidget.stringdata))
        return static_cast<void*>(const_cast< CxListWidget*>(this));
    return QListWidget::qt_metacast(_clname);
}

int CxListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CxListWidget::paramClicked(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CxListWidget::paramEntered(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
