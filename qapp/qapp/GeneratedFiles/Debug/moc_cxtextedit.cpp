/****************************************************************************
** Meta object code from reading C++ file 'cxtextedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cxtextedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cxtextedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CxTextEdit_t {
    QByteArrayData data[10];
    char stringdata[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CxTextEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CxTextEdit_t qt_meta_stringdata_CxTextEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CxTextEdit"
QT_MOC_LITERAL(1, 11, 7), // "runFile"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "std::string"
QT_MOC_LITERAL(4, 32, 8), // "fileName"
QT_MOC_LITERAL(5, 41, 11), // "selectedPos"
QT_MOC_LITERAL(6, 53, 16), // "insertCompletion"
QT_MOC_LITERAL(7, 70, 10), // "completion"
QT_MOC_LITERAL(8, 81, 15), // "runSimpleString"
QT_MOC_LITERAL(9, 97, 8) // "openFile"

    },
    "CxTextEdit\0runFile\0\0std::string\0"
    "fileName\0selectedPos\0insertCompletion\0"
    "completion\0runSimpleString\0openFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CxTextEdit[] = {

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
       5,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   45,    2, 0x08 /* Private */,
       8,    0,   48,    2, 0x08 /* Private */,
       9,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CxTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CxTextEdit *_t = static_cast<CxTextEdit *>(_o);
        switch (_id) {
        case 0: _t->runFile((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 1: _t->selectedPos((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->insertCompletion((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->runSimpleString(); break;
        case 4: _t->openFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CxTextEdit::*_t)(const std::string & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CxTextEdit::runFile)) {
                *result = 0;
            }
        }
        {
            typedef void (CxTextEdit::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CxTextEdit::selectedPos)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CxTextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_CxTextEdit.data,
      qt_meta_data_CxTextEdit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CxTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CxTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CxTextEdit.stringdata))
        return static_cast<void*>(const_cast< CxTextEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int CxTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
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
void CxTextEdit::runFile(const std::string & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CxTextEdit::selectedPos(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
