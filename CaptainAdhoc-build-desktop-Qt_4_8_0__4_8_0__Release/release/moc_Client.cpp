/****************************************************************************
** Meta object code from reading C++ file 'Client.h'
**
** Created: Tue 28. Feb 09:56:23 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CaptainAdhoc/Client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Client[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      21,    7,    7,    7, 0x05,
      45,    7,    7,    7, 0x05,
      64,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,    7,    7,    7, 0x08,
      95,    7,    7,    7, 0x08,
     118,    7,    7,    7, 0x08,
     138,    7,    7,    7, 0x08,
     150,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Client[] = {
    "Client\0\0NewData(int)\0NetworkSpeedUpdate(int)\0"
    "connected(Client*)\0disconnected()\0"
    "donneesRecues()\0donneesEcrites(qint64)\0"
    "deconnexionSocket()\0connexion()\0"
    "dlSpeedMeasure()\0"
};

void Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Client *_t = static_cast<Client *>(_o);
        switch (_id) {
        case 0: _t->NewData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->NetworkSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->connected((*reinterpret_cast< Client*(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->donneesRecues(); break;
        case 5: _t->donneesEcrites((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: _t->deconnexionSocket(); break;
        case 7: _t->connexion(); break;
        case 8: _t->dlSpeedMeasure(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Client::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Client::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Client,
      qt_meta_data_Client, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Client))
        return static_cast<void*>(const_cast< Client*>(this));
    return QObject::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Client::NewData(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Client::NetworkSpeedUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Client::connected(Client * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Client::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
