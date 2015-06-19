/****************************************************************************
** Meta object code from reading C++ file 'cxknowledgemanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cxknowledgemanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cxknowledgemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CxKnowlegdeManager_t {
    QByteArrayData data[12];
    char stringdata[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CxKnowlegdeManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CxKnowlegdeManager_t qt_meta_stringdata_CxKnowlegdeManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "CxKnowlegdeManager"
QT_MOC_LITERAL(1, 19, 12), // "fileSelected"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "std::string"
QT_MOC_LITERAL(4, 45, 15), // "searchDirectory"
QT_MOC_LITERAL(5, 61, 8), // "openFile"
QT_MOC_LITERAL(6, 70, 4), // "save"
QT_MOC_LITERAL(7, 75, 18), // "descriptionChanged"
QT_MOC_LITERAL(8, 94, 4), // "desc"
QT_MOC_LITERAL(9, 99, 9), // "selectPix"
QT_MOC_LITERAL(10, 109, 11), // "generateUID"
QT_MOC_LITERAL(11, 121, 10) // "displayPix"

    },
    "CxKnowlegdeManager\0fileSelected\0\0"
    "std::string\0searchDirectory\0openFile\0"
    "save\0descriptionChanged\0desc\0selectPix\0"
    "generateUID\0displayPix"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CxKnowlegdeManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   57,    2, 0x08 /* Private */,
       5,    0,   58,    2, 0x08 /* Private */,
       6,    0,   59,    2, 0x08 /* Private */,
       7,    1,   60,    2, 0x08 /* Private */,
       9,    0,   63,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CxKnowlegdeManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CxKnowlegdeManager *_t = static_cast<CxKnowlegdeManager *>(_o);
        switch (_id) {
        case 0: _t->fileSelected((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 1: _t->searchDirectory(); break;
        case 2: _t->openFile(); break;
        case 3: _t->save(); break;
        case 4: _t->descriptionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->selectPix(); break;
        case 6: _t->generateUID(); break;
        case 7: _t->displayPix(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CxKnowlegdeManager::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CxKnowlegdeManager::fileSelected)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CxKnowlegdeManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CxKnowlegdeManager.data,
      qt_meta_data_CxKnowlegdeManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CxKnowlegdeManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CxKnowlegdeManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CxKnowlegdeManager.stringdata))
        return static_cast<void*>(const_cast< CxKnowlegdeManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int CxKnowlegdeManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CxKnowlegdeManager::fileSelected(const std::string & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
