/****************************************************************************
** Meta object code from reading C++ file 'WifiManager.h'
**
** Created: Mon 27. Feb 15:53:59 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CaptainAdhoc/WifiManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WifiManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WifiManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,   12,   12,   12, 0x0a,
      70,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WifiManager[] = {
    "WifiManager\0\0ConnectionStatusChanged(CONNECTION_STATUS)\0"
    "ConnectWifi()\0DisconnectWifi()\0"
};

void WifiManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WifiManager *_t = static_cast<WifiManager *>(_o);
        switch (_id) {
        case 0: _t->ConnectionStatusChanged((*reinterpret_cast< CONNECTION_STATUS(*)>(_a[1]))); break;
        case 1: _t->ConnectWifi(); break;
        case 2: _t->DisconnectWifi(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WifiManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WifiManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WifiManager,
      qt_meta_data_WifiManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WifiManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WifiManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WifiManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WifiManager))
        return static_cast<void*>(const_cast< WifiManager*>(this));
    return QObject::qt_metacast(_clname);
}

int WifiManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WifiManager::ConnectionStatusChanged(CONNECTION_STATUS _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE