/****************************************************************************
** Meta object code from reading C++ file 'BlurBehind.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../include/base/qt/ui/effects/BlurBehind.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlurBehind.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS = QtMocHelpers::stringData(
    "base::qt::ui::effects::BlurBehindEffect",
    "blurRadiusChanged",
    "",
    "radius",
    "blurOpacityChanged",
    "opacity",
    "sourceOpacityChanged",
    "downsampleFactorChanged",
    "factor",
    "backgroundBrushChanged",
    "brush",
    "repaintRequired",
    "BlurMethod",
    "BoxBlur",
    "StackBlur",
    "GLBlur"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   66, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    2 /* Public */,
       4,    1,   53,    2, 0x06,    4 /* Public */,
       6,    1,   56,    2, 0x06,    6 /* Public */,
       7,    1,   59,    2, 0x06,    8 /* Public */,
       9,    1,   62,    2, 0x06,   10 /* Public */,
      11,    0,   65,    2, 0x06,   12 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::QBrush,   10,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      12,   12, 0x2,    3,   71,

 // enum data: key, value
      13, uint(base::qt::ui::effects::BlurBehindEffect::BlurMethod::BoxBlur),
      14, uint(base::qt::ui::effects::BlurBehindEffect::BlurMethod::StackBlur),
      15, uint(base::qt::ui::effects::BlurBehindEffect::BlurMethod::GLBlur),

       0        // eod
};

Q_CONSTINIT const QMetaObject base::qt::ui::effects::BlurBehindEffect::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsEffect::staticMetaObject>(),
    qt_meta_stringdata_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS_t,
        // enum 'BlurMethod'
        QtPrivate::TypeAndForceComplete<BlurBehindEffect::BlurMethod, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BlurBehindEffect, std::true_type>,
        // method 'blurRadiusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'blurOpacityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'sourceOpacityChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'downsampleFactorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'backgroundBrushChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QBrush &, std::false_type>,
        // method 'repaintRequired'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void base::qt::ui::effects::BlurBehindEffect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BlurBehindEffect *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->blurRadiusChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->blurOpacityChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->sourceOpacityChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->downsampleFactorChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 4: _t->backgroundBrushChanged((*reinterpret_cast< std::add_pointer_t<QBrush>>(_a[1]))); break;
        case 5: _t->repaintRequired(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BlurBehindEffect::*)(int );
            if (_t _q_method = &BlurBehindEffect::blurRadiusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BlurBehindEffect::*)(double );
            if (_t _q_method = &BlurBehindEffect::blurOpacityChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BlurBehindEffect::*)(double );
            if (_t _q_method = &BlurBehindEffect::sourceOpacityChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BlurBehindEffect::*)(double );
            if (_t _q_method = &BlurBehindEffect::downsampleFactorChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BlurBehindEffect::*)(const QBrush & );
            if (_t _q_method = &BlurBehindEffect::backgroundBrushChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BlurBehindEffect::*)();
            if (_t _q_method = &BlurBehindEffect::repaintRequired; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *base::qt::ui::effects::BlurBehindEffect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *base::qt::ui::effects::BlurBehindEffect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSbaseSCOPEqtSCOPEuiSCOPEeffectsSCOPEBlurBehindEffectENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsEffect::qt_metacast(_clname);
}

int base::qt::ui::effects::BlurBehindEffect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsEffect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void base::qt::ui::effects::BlurBehindEffect::blurRadiusChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void base::qt::ui::effects::BlurBehindEffect::blurOpacityChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void base::qt::ui::effects::BlurBehindEffect::sourceOpacityChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void base::qt::ui::effects::BlurBehindEffect::downsampleFactorChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void base::qt::ui::effects::BlurBehindEffect::backgroundBrushChanged(const QBrush & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void base::qt::ui::effects::BlurBehindEffect::repaintRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
