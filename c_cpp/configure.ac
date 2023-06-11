#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [1], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([include/keyboard.h])
AC_CONFIG_HEADERS([include/config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
LT_INIT

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([ncurses], [init_pairs])
AC_CHECK_LIB([other_tools], [sl_tools_getsign])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h unistd.h])
AC_SUBST([OTHER_TOOLS_DEPS], [-lz])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strspn])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 lib/nbt/Makefile
                 lib/other_tools/Makefile
                 src/Makefile])
AC_OUTPUT
