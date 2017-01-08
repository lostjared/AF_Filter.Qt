/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "main_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FilterControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   26,   26,   26, 0x0a,
      27,   44,   26,   26, 0x0a,
      48,   44,   26,   26, 0x0a,
      60,   44,   26,   26, 0x0a,
      79,   97,   26,   26, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilterControl[] = {
    "FilterControl\0setSource()\0\0updateIndex(int)\0"
    "pos\0setPos(int)\0setSliderText(int)\0"
    "checkChanged(int)\0chk\0"
};

void FilterControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FilterControl *_t = static_cast<FilterControl *>(_o);
        switch (_id) {
        case 0: _t->setSource(); break;
        case 1: _t->updateIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setSliderText((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->checkChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FilterControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FilterControl::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FilterControl,
      qt_meta_data_FilterControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FilterControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FilterControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FilterControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilterControl))
        return static_cast<void*>(const_cast< FilterControl*>(this));
    return QDialog::qt_metacast(_clname);
}

int FilterControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_FilterWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   24,   24,   24, 0x0a,
      25,   24,   24,   24, 0x0a,
      36,   24,   24,   24, 0x0a,
      46,   24,   24,   24, 0x0a,
      57,   24,   24,   24, 0x0a,
      68,   24,   24,   24, 0x0a,
      79,   24,   24,   24, 0x0a,
      90,   24,   24,   24, 0x0a,
     102,   24,   24,   24, 0x0a,
     114,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilterWindow[] = {
    "FilterWindow\0fileLoad()\0\0fileSave()\0"
    "fileSet()\0fileExit()\0editUndo()\0"
    "editRedo()\0editCopy()\0editPaste()\0"
    "helpHowTo()\0helpAbout()\0"
};

void FilterWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FilterWindow *_t = static_cast<FilterWindow *>(_o);
        switch (_id) {
        case 0: _t->fileLoad(); break;
        case 1: _t->fileSave(); break;
        case 2: _t->fileSet(); break;
        case 3: _t->fileExit(); break;
        case 4: _t->editUndo(); break;
        case 5: _t->editRedo(); break;
        case 6: _t->editCopy(); break;
        case 7: _t->editPaste(); break;
        case 8: _t->helpHowTo(); break;
        case 9: _t->helpAbout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FilterWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FilterWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FilterWindow,
      qt_meta_data_FilterWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FilterWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FilterWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FilterWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilterWindow))
        return static_cast<void*>(const_cast< FilterWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FilterWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
