AC_INIT([πFS], [1.0], [philipl@overt.org], [πfs])
AC_CONFIG_AUX_DIR(build)
AC_CONFIG_SRCDIR(src/πfs.c)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

# Checks for programs
AC_PROG_CC_C99

# Set language to C
AC_LANG_C

# Checks for header files
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_MEMBERS([struct dirent.d_off], [], [],
                 [[#include <dirent.h>]])

PKG_PROG_PKG_CONFIG
macfuse_root="/Library/Filesystems/macfuse.fs/Contents/Resources"
AS_IF([test -d "$macfuse_root/lib/pkgconfig"],
      [PKG_CONFIG_PATH="$macfuse_root/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
       export PKG_CONFIG_PATH])

PKG_CHECK_MODULES([FUSE], [fuse >= 2.8], [have_fuse=yes], [have_fuse=no])
AS_IF([test "x$have_fuse" != "xyes"],
      [AS_IF([test -r "$macfuse_root/include/fuse.h" ||
              test -r "$macfuse_root/include/fuse/fuse.h"],
             [FUSE_CFLAGS="-I$macfuse_root/include"
              FUSE_LIBS="-L$macfuse_root/lib -lfuse"],
             [AC_MSG_ERROR([FUSE 2 development files are required. On macOS, install macFUSE.])])])

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $FUSE_CFLAGS"
AC_CHECK_HEADERS([fuse/fuse.h fuse.h], [break], [],
                 [[#define FUSE_USE_VERSION 26]])
CPPFLAGS="$save_CPPFLAGS"

AS_IF([test "x$ac_cv_header_fuse_fuse_h" != "xyes" &&
       test "x$ac_cv_header_fuse_h" != "xyes"],
      [AC_MSG_ERROR([Could not find a FUSE header.])])

AC_CHECK_FUNCS([fdatasync utimensat])

AC_SUBST([EXTRA_CFLAGS], ["-Wall -Werror -Wextra -Wno-unused-parameter"])

AC_CONFIG_FILES([Makefile
 src/Makefile
 ])
AC_OUTPUT
