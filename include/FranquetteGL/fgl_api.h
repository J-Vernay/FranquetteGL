/// @file fgl_api.h
/// @author Julien Vernay, 2022
/// @copyright Under GNU General Public License v3 or later.
/// @brief Common API definitions required for everything else.

#ifndef FRANQUETTEGL_API_H
#define FRANQUETTEGL_API_H

#if !(__STDC_VERSION__ >= 201112L || __cplusplus >= 2011032L)
#error "FranquetteGL expects at least a C11 or C++11 compiler."
#endif

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <KHR/khrplatform.h>

/// @name C/C++ compatibility
/// @{

#ifdef __cplusplus
#define fgl_STATIC_ASSERT static_assert
#define fgl_ENUM_TYPE(T) : T
#define fgl_THREAD_LOCAL thread_local
#else
#define fgl_STATIC_ASSERT _Static_assert
#define fgl_ENUM_TYPE(T)
#define fgl_THREAD_LOCAL _Thread_local
#endif

/// @def fgl_STATIC_ASSERT
/// C/C++ compatibility for compile-time error reporting.

/// @def fgl_ENUM_TYPE
/// C/C++ compatibility to specify the underlying type of an enumeration.

/// @def fgl_THREAD_LOCAL
/// C/C++ compatibility to specify a static thread_local variable.

/// @}

#endif
