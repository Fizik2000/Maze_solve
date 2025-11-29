/****************************************************************************
** Meta object code from reading C++ file 'cavewidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../view/cavewidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cavewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_s21__CaveWidget_t {
    uint offsetsAndSizes[14];
    char stringdata0[16];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[18];
    char stringdata5[25];
    char stringdata6[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_s21__CaveWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_s21__CaveWidget_t qt_meta_stringdata_s21__CaveWidget = {
    {
        QT_MOC_LITERAL(0, 15),  // "s21::CaveWidget"
        QT_MOC_LITERAL(16, 17),  // "onOpenFileClicked"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 17),  // "onSaveFileClicked"
        QT_MOC_LITERAL(53, 17),  // "onGenerateClicked"
        QT_MOC_LITERAL(71, 24),  // "onCellularAvtomatClicked"
        QT_MOC_LITERAL(96, 14)   // "onTimerTimeout"
    },
    "s21::CaveWidget",
    "onOpenFileClicked",
    "",
    "onSaveFileClicked",
    "onGenerateClicked",
    "onCellularAvtomatClicked",
    "onTimerTimeout"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_s21__CaveWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x08,    1 /* Private */,
       3,    0,   45,    2, 0x08,    2 /* Private */,
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    0,   48,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject s21::CaveWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_s21__CaveWidget.offsetsAndSizes,
    qt_meta_data_s21__CaveWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_s21__CaveWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CaveWidget, std::true_type>,
        // method 'onOpenFileClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSaveFileClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onGenerateClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCellularAvtomatClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTimerTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void s21::CaveWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CaveWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onOpenFileClicked(); break;
        case 1: _t->onSaveFileClicked(); break;
        case 2: _t->onGenerateClicked(); break;
        case 3: _t->onCellularAvtomatClicked(); break;
        case 4: _t->onTimerTimeout(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *s21::CaveWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *s21::CaveWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_s21__CaveWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int s21::CaveWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
