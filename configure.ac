AC_PREREQ([2.69])
AC_INIT([moba-svg], [0.1.0-0000], [pappi-@gmx.de])

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SUBDIRS([modules/lib-msghandling])
AC_CONFIG_SUBDIRS([modules/lib-tracklayout])
AC_CONFIG_AUX_DIR([.])

AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])

AC_CHECK_LIB([mobacommon], [main])

AX_CXX_COMPILE_STDCXX_17()

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
