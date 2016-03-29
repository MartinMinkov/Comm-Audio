/****************************************************************************
** Meta object code from reading C++ file 'clienthandlerthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Server/clienthandlerthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clienthandlerthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ClientHandlerThread_t {
    QByteArrayData data[6];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientHandlerThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientHandlerThread_t qt_meta_stringdata_ClientHandlerThread = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ClientHandlerThread"
QT_MOC_LITERAL(1, 20, 16), // "signalDisconnect"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 8), // "finished"
QT_MOC_LITERAL(4, 47, 15), // "receiveRequests"
QT_MOC_LITERAL(5, 63, 10) // "disconnect"

    },
    "ClientHandlerThread\0signalDisconnect\0"
    "\0finished\0receiveRequests\0disconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientHandlerThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ClientHandlerThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientHandlerThread *_t = static_cast<ClientHandlerThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalDisconnect(); break;
        case 1: _t->finished(); break;
        case 2: _t->receiveRequests(); break;
        case 3: _t->disconnect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ClientHandlerThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientHandlerThread::signalDisconnect)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ClientHandlerThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientHandlerThread::finished)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ClientHandlerThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ClientHandlerThread.data,
      qt_meta_data_ClientHandlerThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ClientHandlerThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientHandlerThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ClientHandlerThread.stringdata0))
        return static_cast<void*>(const_cast< ClientHandlerThread*>(this));
    return QObject::qt_metacast(_clname);
}

int ClientHandlerThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ClientHandlerThread::signalDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ClientHandlerThread::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
