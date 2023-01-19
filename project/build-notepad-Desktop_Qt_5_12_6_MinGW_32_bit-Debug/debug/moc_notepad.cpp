/****************************************************************************
** Meta object code from reading C++ file 'notepad.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../binary_tools/notepad.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notepad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NotePad_t {
    QByteArrayData data[15];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NotePad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NotePad_t qt_meta_stringdata_NotePad = {
    {
QT_MOC_LITERAL(0, 0, 7), // "NotePad"
QT_MOC_LITERAL(1, 8, 6), // "search"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 7), // "replace"
QT_MOC_LITERAL(4, 24, 10), // "replaceAll"
QT_MOC_LITERAL(5, 35, 8), // "gotoLine"
QT_MOC_LITERAL(6, 44, 10), // "lineNumber"
QT_MOC_LITERAL(7, 55, 14), // "deleteSelected"
QT_MOC_LITERAL(8, 70, 11), // "toUpperCase"
QT_MOC_LITERAL(9, 82, 11), // "toLowerCase"
QT_MOC_LITERAL(10, 94, 17), // "blockCountChanged"
QT_MOC_LITERAL(11, 112, 5), // "count"
QT_MOC_LITERAL(12, 118, 14), // "contentsChange"
QT_MOC_LITERAL(13, 133, 19), // "ensureCursorVisible"
QT_MOC_LITERAL(14, 153, 8) // "reconfig"

    },
    "NotePad\0search\0\0replace\0replaceAll\0"
    "gotoLine\0lineNumber\0deleteSelected\0"
    "toUpperCase\0toLowerCase\0blockCountChanged\0"
    "count\0contentsChange\0ensureCursorVisible\0"
    "reconfig"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NotePad[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   74,    2, 0x0a /* Public */,
       3,    5,   83,    2, 0x0a /* Public */,
       4,    4,   94,    2, 0x0a /* Public */,
       5,    1,  103,    2, 0x0a /* Public */,
       7,    0,  106,    2, 0x08 /* Private */,
       8,    0,  107,    2, 0x08 /* Private */,
       9,    0,  108,    2, 0x08 /* Private */,
      10,    1,  109,    2, 0x08 /* Private */,
      12,    3,  112,    2, 0x08 /* Private */,
      13,    0,  119,    2, 0x08 /* Private */,
      14,    1,  120,    2, 0x08 /* Private */,
      14,    0,  123,    2, 0x28 /* Private | MethodCloned */,

 // slots: parameters
    QMetaType::Int, QMetaType::QString, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void NotePad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NotePad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->search((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->replace((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 2: _t->replaceAll((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 3: _t->gotoLine((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->deleteSelected(); break;
        case 5: _t->toUpperCase(); break;
        case 6: _t->toLowerCase(); break;
        case 7: _t->blockCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->contentsChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->ensureCursorVisible(); break;
        case 10: _t->reconfig((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->reconfig(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NotePad::staticMetaObject = { {
    &QPlainTextEdit::staticMetaObject,
    qt_meta_stringdata_NotePad.data,
    qt_meta_data_NotePad,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NotePad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NotePad::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NotePad.stringdata0))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int NotePad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
