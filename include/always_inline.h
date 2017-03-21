#ifndef HDQRT_MACROS_H_
#define HDQRT_MACROS_H_


#if !(defined(ALWAYS_INLINE))

#  if defined(__clang__) || defined( __GNUC__ )
#     define ALWAYS_INLINE inline __attribute__((always_inline))
#  elif (COMPILER(MSVC) || COMPILER(RVCT))
#     define ALWAYS_INLINE __forceinline
#  else
#     define ALWAYS_INLINE inline
#  endif // compiler switch

#else
   // The include guard prevents this code from being seen if our definition
   // is active
#  ifdef _MSC_VER
#     pragma message( "warning: ALWAYS_INLINE defined by compiler or library and might not behave as expected." )
#  else
#     warning ( "ALWAYS_INLINE defined by compiler or library and might not behave as expected." )
#  endif

#endif // ALWAYS_INLINE


#endif // HDQRT_MACROS_H_
