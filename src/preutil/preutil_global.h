#ifndef PREUTIL_GLOBAL_H
#define PREUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PREUTIL_LIBRARY)
#  define PREUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PREUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PREUTIL_GLOBAL_H

