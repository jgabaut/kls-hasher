AC_INIT([kls_hasher], [0.0.1], [jgabaut@github.com])
AM_INIT_AUTOMAKE([foreign -Wall])
AC_CANONICAL_HOST
echo "Host os:  $host_os"
AM_CONDITIONAL([OS_DARWIN], [test "$host_os" = "darwin"])
AM_CONDITIONAL([MINGW32_BUILD], [test "$host_os" = "mingw32"])
AC_ARG_ENABLE([debug],  [AS_HELP_STRING([--enable-debug], [Enable debug build])],  [enable_debug=$enableval],  [enable_debug=no])
AM_CONDITIONAL([DEBUG_BUILD], [test "$enable_debug" = "yes"])
if test "$host_os" = "mingw32"; then
  echo "Building for mingw32: [$host_cpu-$host_vendor-$host_os]"
  AC_SUBST([KLS_HASHER_CFLAGS], ["-I/usr/x86_64-w64-mingw32/include -static -fstack-protector -DMINGW32_BUILD"])
  AC_SUBST([KLS_HASHER_LDFLAGS], ["-L/usr/x86_64-w64-mingw32/lib"])
  AC_SUBST([CCOMP], ["/usr/bin/x86_64-w64-mingw32-gcc"])
  AC_SUBST([OS], ["w64-mingw32"])
  AC_SUBST([TARGET], ["kls_hasher.exe"])
fi
if test "$host_os" = "darwin"; then
  echo "Building for macos: [$host_cpu-$host_vendor-$host_os]"
  AC_SUBST([KLS_HASHER_CFLAGS], [""])
  AC_SUBST([KLS_HASHER_LDFLAGS], [""])
  AC_SUBST([OS], ["darwin"])
  AC_SUBST([TARGET], ["kls_hasher"])
fi
if test "$host_os" = "linux-gnu"; then
  echo "Building for Linux: [$host_cpu-$host_vendor-$host_os]"
  AC_SUBST([HW_CFLAGS], [""])
  AC_SUBST([HW_LDFLAGS], [""])
  AC_SUBST([OS], ["Linux"])
  AC_SUBST([TARGET], ["kls_hasher"])
fi
AC_ARG_VAR([VERSION], [Version number])
if test -z "$VERSION"; then
  VERSION="0.0.1"
fi
AC_DEFINE_UNQUOTED([VERSION], ["$VERSION"], [Version number])
AC_CHECK_PROGS([CCOMP], [gcc clang])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_FUNCS([malloc calloc])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
