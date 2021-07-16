#pragma once
#if defined(__clang__)
 #define _CUSTOMUTILS_comp "clang"
#elif defined(__GNUC__)
 #define _CUSTOMUTILS_comp "GNU"
#elif defined(_MSC_VER)
 #define _CUSTOMUTILS_comp "MS"
#else
 #error "unrecognized compiler: not 'clang', a GNU compiler, or Windows compiler"
#endif
#ifndef RECAST
 #define RECAST(type, exp) reinterpret_cast<type>(exp)
#endif
#ifndef DEPRECATE
 #if defined (__GNUC__)
  #define DEPRECATE(fun, msg) fun __attribute__((deprecated(msg)))
 #elif define (_MSC_VER)
  #define DEPRECATE(fun, msg) __declspec(deprecated(msg)) fun
 #endif
#endif
#ifndef Utils_offset
 #define Utils_offset(type, field) ((decltype(sizeof 0))(&((type*)0)->field))
#endif
