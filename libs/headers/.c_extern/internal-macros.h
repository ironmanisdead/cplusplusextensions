#undef DLL_PUBLIC
#undef DLL_LOCAL
#undef DLL_HIDE
#undef DLL_RESTORE
#undef DLL_OS_unix
#undef DLL_OS_windows
#undef DLL_CC_clang
#undef DLL_CC_gcc
#if defined(__unix__)
 #ifndef DLL_OS_unix
  #define DLL_OS_unix 1
 #endif
#elif defined(_MSC_VER)
 #ifndef DLL_OS_windows
  #define DLL_OS_windows 1
 #endif
 #if (DLL_EXPORTED > 0)
  #define DLL_PUBLIC __declspec(dllexport)
 #else
  #define DLL_PUBLIC __declspec(dllimport)
 #endif
 #define DLL_LOCAL
#else
 #error "cannot detect operating system used for compilation, only supports Windows and **nix"
#endif
#if defined(__clang__)
 #define DLL_HIDE
 #define DLL_RESTORE
 #ifndef DLL_CC_clang
  #define DLL_CC_clang 1
 #endif
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#elif defined(__GNUC__)
 #define DLL_HIDE _Pragma("GCC visibility push (internal)")
 #define DLL_RESTORE _Pragma("GCC visibility pop")
 #define DLL_CC_gcc 1
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#else
 #define DLL_HIDE
 #define DLL_RESTORE
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC
  #define DLL_LOCAL
 #endif
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
