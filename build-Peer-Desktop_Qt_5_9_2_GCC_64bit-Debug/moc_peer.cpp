/****************************************************************************
** Meta object code from reading C++ file 'peer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Peer/peer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'peer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Peer_t {
    QByteArrayData data[10];
    char stringdata0[261];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Peer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Peer_t qt_meta_stringdata_Peer = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Peer"
QT_MOC_LITERAL(1, 5, 30), // "on_sign_up_push_button_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 27), // "on_bind_push_button_clicked"
QT_MOC_LITERAL(4, 65, 30), // "on_sign_in_push_button_clicked"
QT_MOC_LITERAL(5, 96, 31), // "on_sign_out_push_button_clicked"
QT_MOC_LITERAL(6, 128, 27), // "on_send_push_button_clicked"
QT_MOC_LITERAL(7, 156, 33), // "on_show_image_push_button_cli..."
QT_MOC_LITERAL(8, 190, 33), // "on_get_online_push_button_cli..."
QT_MOC_LITERAL(9, 224, 36) // "on_request_quota_push_button_..."

    },
    "Peer\0on_sign_up_push_button_clicked\0"
    "\0on_bind_push_button_clicked\0"
    "on_sign_in_push_button_clicked\0"
    "on_sign_out_push_button_clicked\0"
    "on_send_push_button_clicked\0"
    "on_show_image_push_button_clicked\0"
    "on_get_online_push_button_clicked\0"
    "on_request_quota_push_button_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Peer[] = {

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
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

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

void Peer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Peer *_t = static_cast<Peer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_sign_up_push_button_clicked(); break;
        case 1: _t->on_bind_push_button_clicked(); break;
        case 2: _t->on_sign_in_push_button_clicked(); break;
        case 3: _t->on_sign_out_push_button_clicked(); break;
        case 4: _t->on_send_push_button_clicked(); break;
        case 5: _t->on_show_image_push_button_clicked(); break;
        case 6: _t->on_get_online_push_button_clicked(); break;
        case 7: _t->on_request_quota_push_button_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Peer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Peer.data,
      qt_meta_data_Peer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Peer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Peer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Peer.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Peer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
