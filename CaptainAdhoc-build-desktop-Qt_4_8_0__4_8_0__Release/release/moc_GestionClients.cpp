/****************************************************************************
** Meta object code from reading C++ file 'GestionClients.h'
**
** Created: Mon 27. Feb 11:55:29 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CaptainAdhoc/GestionClients.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GestionClients.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GestionClients[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      37,   15,   15,   15, 0x05,
      61,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   15,   15,   15, 0x08,
     106,   15,   15,   15, 0x08,
     141,   15,   15,   15, 0x08,
     172,   15,   15,   15, 0x08,
     191,   15,   15,   15, 0x08,
     216,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GestionClients[] = {
    "GestionClients\0\0TransfertUpdate(int)\0"
    "NetworkSpeedUpdate(int)\0"
    "ClientNumberChanged(int)\0clientReceived(int)\0"
    "newConnectionRequest(QHostAddress)\0"
    "newConnectionDone(QTcpSocket*)\0"
    "clientDisconnect()\0clientConnected(Client*)\0"
    "dlSpeedUpdate(int)\0"
};

void GestionClients::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GestionClients *_t = static_cast<GestionClients *>(_o);
        switch (_id) {
        case 0: _t->TransfertUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->NetworkSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ClientNumberChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->clientReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->newConnectionRequest((*reinterpret_cast< QHostAddress(*)>(_a[1]))); break;
        case 5: _t->newConnectionDone((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 6: _t->clientDisconnect(); break;
        case 7: _t->clientConnected((*reinterpret_cast< Client*(*)>(_a[1]))); break;
        case 8: _t->dlSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GestionClients::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GestionClients::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GestionClients,
      qt_meta_data_GestionClients, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GestionClients::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GestionClients::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GestionClients::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GestionClients))
        return static_cast<void*>(const_cast< GestionClients*>(this));
    return QObject::qt_metacast(_clname);
}

int GestionClients::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void GestionClients::TransfertUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GestionClients::NetworkSpeedUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GestionClients::ClientNumberChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
