/****************************************************************************
** Meta object code from reading C++ file 'cxcentral.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cxcentral.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cxcentral.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CxCentral_t {
    QByteArrayData data[18];
    char stringdata[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CxCentral_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CxCentral_t qt_meta_stringdata_CxCentral = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CxCentral"
QT_MOC_LITERAL(1, 10, 11), // "openXmlFile"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "std::string"
QT_MOC_LITERAL(4, 35, 4), // "file"
QT_MOC_LITERAL(5, 40, 17), // "resultWidgetAdded"
QT_MOC_LITERAL(6, 58, 12), // "QTreeWidget*"
QT_MOC_LITERAL(7, 71, 9), // "newWidget"
QT_MOC_LITERAL(8, 81, 14), // "readScriptFile"
QT_MOC_LITERAL(9, 96, 14), // "cueTextChanged"
QT_MOC_LITERAL(10, 111, 17), // "paramNameModified"
QT_MOC_LITERAL(11, 129, 24), // "std::vector<std::string>"
QT_MOC_LITERAL(12, 154, 16), // "listParamClicked"
QT_MOC_LITERAL(13, 171, 9), // "paramName"
QT_MOC_LITERAL(14, 181, 16), // "listParamEntered"
QT_MOC_LITERAL(15, 198, 5), // "clear"
QT_MOC_LITERAL(16, 204, 18), // "removeResultWidget"
QT_MOC_LITERAL(17, 223, 8) // "testCall"

    },
    "CxCentral\0openXmlFile\0\0std::string\0"
    "file\0resultWidgetAdded\0QTreeWidget*\0"
    "newWidget\0readScriptFile\0cueTextChanged\0"
    "paramNameModified\0std::vector<std::string>\0"
    "listParamClicked\0paramName\0listParamEntered\0"
    "clear\0removeResultWidget\0testCall"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CxCentral[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       8,    1,   70,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x0a /* Public */,
      12,    1,   77,    2, 0x0a /* Public */,
      14,    1,   80,    2, 0x0a /* Public */,
      15,    0,   83,    2, 0x0a /* Public */,
      16,    0,   84,    2, 0x0a /* Public */,
      17,    0,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CxCentral::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CxCentral *_t = static_cast<CxCentral *>(_o);
        switch (_id) {
        case 0: _t->openXmlFile((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 1: _t->resultWidgetAdded((*reinterpret_cast< QTreeWidget*(*)>(_a[1]))); break;
        case 2: _t->readScriptFile((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 3: _t->cueTextChanged(); break;
        case 4: _t->paramNameModified((*reinterpret_cast< const std::vector<std::string>(*)>(_a[1]))); break;
        case 5: _t->listParamClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->listParamEntered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->clear(); break;
        case 8: _t->removeResultWidget(); break;
        case 9: _t->testCall(); break;
        default: ;
        }
    }
}

const QMetaObject CxCentral::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CxCentral.data,
      qt_meta_data_CxCentral,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CxCentral::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CxCentral::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CxCentral.stringdata))
        return static_cast<void*>(const_cast< CxCentral*>(this));
    return QWidget::qt_metacast(_clname);
}

int CxCentral::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
