# Copyright (c) 2013-2015 Mellanox Technologies, Inc.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

define([proj_ver_major], 1)
define([proj_ver_minor], 1)
define([proj_ver_micro], esyscmd([sh -c "git rev-list HEAD | wc -l | sed -e 's/ *//g' | xargs -n1 printf"])) dnl

AC_DEFINE(PROJ_MAJOR_VER, proj_ver_major, [Major version of the project])
AC_DEFINE(PROJ_MINOR_VER, prof_ver_minor, [Minor version of the project])
AC_DEFINE(PROJ_MICRO_VER, proj_ver_micro, [Micro version of the project])

AC_INIT([LIBIBPROF], [proj_ver_major.proj_ver_minor.proj_ver_micro], [support@mellanox.com])

define([libibprof_prefix], [/opt/mellanox/libibprof])
AC_PREFIX_DEFAULT([libibprof_prefix])


MAJOR_VERSION=proj_ver_major
MINOR_VERSION=proj_ver_minor
MICRO_VERSION=proj_ver_micro
VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION
AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)

AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([config/aux])
AM_INIT_AUTOMAKE([1.10 foreign tar-ustar silent-rules subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([config/m4])


LT_INIT
CFLAGS="$CFLAGS -Wall -Werror"
CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE -D__LINUX__ -D__MODULE_VERSION=$VERSION"

##########################
# Compile with OpenFabrics
#
AC_ARG_WITH([ofed],
            [AC_HELP_STRING([--with-ofed(=DIR)],
                            [IBPROF: build OpenFabrics support, adding DIR/include, DIR/lib, and DIR/lib64 to the search path for headers and libraries])],
            [],
            [with_ofed=/usr]
            )

if test -d "$with_ofed/include/infiniband"; then
    CFLAGS="$CFLAGS -I$with_ofed/include"
    CPPFLAGS="$CPPFLAGS -I$with_ofed/include"
    LDFLAGS="$LDFLAGS -L$with_ofed/lib64 -L$with_ofed/lib"
    AC_MSG_RESULT($with_ofed)
else
    AC_MSG_ERROR([libibverbs not found at $with_ofed])
fi


##########################
# Enable debug build
#
AC_ARG_ENABLE(
    [debug],
    AC_HELP_STRING([--enable-debug],
                   [IBPROF: turn on debugging (default=no)]))
if test x$enable_debug = xyes; then
    CFLAGS="$CFLAGS -g -D_DEBUG"
else
    CFLAGS="$CFLAGS -O2"
fi


##########################
# Enable internal check
#
AC_ARG_ENABLE(
    [check],
    [AC_HELP_STRING([--enable-check],
                   [IBPROF: turn on internal checking (default=yes)])],
    [enable_check=$enableval],
    [enable_check=yes])
if test x$enable_check = xyes; then
    CFLAGS="$CFLAGS -DHAVE_CHECK"
fi


#####################################
# Checks for header files.
#
AC_HEADER_STDC
AC_CHECK_HEADERS(
    [stdint.h stdlib.h \
    string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])


#####################################
# Checks for libraries.
#
AC_CHECK_LIB([dl], [dlopen],
    [LIBS="$LIBS -ldl"],
    [AC_MSG_ERROR([libdl not found])])

AC_CHECK_LIB([pthread], [pthread_create], 
    [LIBS="$LIBS -lpthread"],
    [AC_MSG_ERROR([pthread not found])])

AC_HEADER_STDC

dnl Check VERBS API
prj_cv_verbs=0
AC_CHECK_HEADER([infiniband/verbs.h],
    [],
    [AC_MSG_ERROR([ibverbs header files not found])])
AC_CHECK_LIB([ibverbs], [ibv_get_device_list],
    [AC_CHECK_MEMBERS([struct ibv_context_ops._compat_query_device],
        [AC_MSG_WARN([Installed libibverbs version is not supported])],[CFLAGS="$CFLAGS -DUSE_IBV=1" prj_cv_verbs=1],[[#include <infiniband/verbs.h>]])],
    [AC_MSG_ERROR([libibverbs not found])])

if test "$prj_cv_verbs" -ne 0; then

MOFED_VERSION=-1

AC_COMPILE_CHECK([VERBS API libibverbs 1.1.2 or earlier],
    [#include <infiniband/verbs.h>],
    [struct ibv_mr *ibv_reg_mr(struct ibv_pd *pd, void *addr,size_t length, enum ibv_access_flags access);],
    ac_cv_libibverbs_legacy=yes,
    ac_cv_libibverbs_legacy=no)
AC_MSG_RESULT($ac_cv_libibverbs_legacy)
if test $ac_cv_libibverbs_legacy = yes ; then
      CFLAGS="$CFLAGS -DIBV_API_LEGACY=1"
fi

dnl Check VERBS API Extension support
AC_CHECK_LIB([ibverbs], [verbs_register_driver],
    [use_ibv_ext=1],
    [use_ibv_ext=0])
AC_MSG_CHECKING([for VERBS API Extension support in libibverbs])
if test $use_ibv_ext -eq 1; then
    MOFED_VERSION=1
    AC_MSG_RESULT(yes)

	AC_CHECK_MEMBERS([struct verbs_context.open_qp],[CFLAGS="$CFLAGS -DHAVE_IBV_OPEN_QP"],[],[[#include <infiniband/verbs.h>]])
	AC_CHECK_MEMBERS([struct verbs_context.create_qp_ex],[CFLAGS="$CFLAGS -DHAVE_IBV_CREATE_QP_EX"],[],[[#include <infiniband/verbs.h>]])
	AC_CHECK_MEMBERS([struct verbs_context.open_xrcd],[CFLAGS="$CFLAGS -DHAVE_IBV_OPEN_XRCD"],[],[[#include <infiniband/verbs.h>]])
	AC_CHECK_MEMBERS([struct verbs_context.close_xrcd],[CFLAGS="$CFLAGS -DHAVE_IBV_CLOSE_XRCD"],[],[[#include <infiniband/verbs.h>]])
else
    AC_MSG_RESULT(no)
fi

dnl Check VERBS API Extension support
AC_CHECK_HEADER([infiniband/verbs_exp.h],
    [use_ibv_exp=1],
    [use_ibv_exp=0])
AC_MSG_CHECKING([for VERBS API Experimental support in libibverbs])


#
# Check for new experimental api in ofed 2.2+ in which all flags passed to
# ibv_exp_..() calls begin with IBV_EXP_...
#
AC_CHECK_DECLS([IBV_EXP_ACCESS_LOCAL_WRITE, IBV_EXP_SEND_INLINE, IBV_EXP_QP_STATE],
                               [], [], [[#include <infiniband/verbs.h>]])
AS_IF([test "$ac_cv_have_decl_IBV_EXP_ACCESS_LOCAL_WRITE" == "yes" &&
       test "$ac_cv_have_decl_IBV_EXP_SEND_INLINE" == "yes" &&
       test "$ac_cv_have_decl_IBV_EXP_QP_STATE" == "yes"],
       [use_ibv_exp=1],[use_ibv_exp=0])

if test $use_ibv_exp -eq 1; then
	MOFED_VERSION=2
	AC_MSG_RESULT(yes)
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_query_device],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_QUERY_DEVICE"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_modify_cq],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_MODIFY_CQ"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_ibv_create_flow],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_CREATE_FLOW"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_ibv_destroy_flow],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_DESTROY_FLOW"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_post_task],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_POST_TASK"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.drv_exp_ibv_poll_cq],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_POLL_CQ"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_create_qp],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_CREATE_QP"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_modify_qp],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_MODIFY_QP"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.drv_exp_post_send],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_POST_SEND"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_query_port],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_QUERY_PORT"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_bind_mw],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_BIND_MW"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_reg_mr],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_REG_MR"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_create_mr],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_CREATE_MR"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.exp_create_cq],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_CREATE_CQ"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.create_dct],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_CREATE_DCT"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.query_dct],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_QUERY_DCT"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.destroy_dct],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_DESTROY_DCT"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.drv_exp_query_values],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_QUERY_VALUES"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_arm_dct],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_ARM_DCT"],[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_query_mkey],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_QUERY_MKEY"],[],[[#include <infiniband/verbs_exp.h>]])

	# MOFED 2.3
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_alloc_mkey_list_memory],
		AC_CHECK_TYPES([struct non_inline_data],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY=0"],[],[[#include <infiniband/verbs_exp.h>]]),
		[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_dealloc_mkey_list_memory],
		AC_CHECK_TYPES([struct non_inline_data],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY=0"],[],[[#include <infiniband/verbs_exp.h>]]),
		[],[[#include <infiniband/verbs_exp.h>]])

	# MOFED 2.4+
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_alloc_mkey_list_memory],
		AC_CHECK_TYPES([struct ibv_exp_mkey_list_container],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_ALLOC_MKEY_LIST_MEMORY=1"],[],[[#include <infiniband/verbs_exp.h>]]),
		[],[[#include <infiniband/verbs_exp.h>]])
	AC_CHECK_MEMBERS([struct verbs_context_exp.lib_exp_dealloc_mkey_list_memory],
		AC_CHECK_TYPES([struct ibv_exp_mkey_list_container],[CFLAGS="$CFLAGS -DHAVE_IBV_EXP_DEALLOC_MKEY_LIST_MEMORY=1"],[],[[#include <infiniband/verbs_exp.h>]]),
		[],[[#include <infiniband/verbs_exp.h>]])
else
    AC_MSG_RESULT(no)
fi

if test $MOFED_VERSION -gt 0; then
	CFLAGS="$CFLAGS -DIBV_API_EXT=$MOFED_VERSION"
	AC_MSG_NOTICE([Current VERBS API version : $MOFED_VERSION])
else
	AC_MSG_NOTICE([Current VERBS API version : legacy])
fi

fi

# Check if the compiler has support for visibility, like some
# versions of gcc, icc Sun Studio cc.
AC_ARG_ENABLE(visibility, 
    AC_HELP_STRING([--enable-visibility],
        [IBPROF: enable visibility feature of certain compilers/linkers (default: enabled)]))

ac_visibility_define=0
ac_msg="whether to enable symbol visibility"

if test "$ac_enable_visibility" = "no"; then
    AC_MSG_CHECKING([$ac_msg])
    AC_MSG_RESULT([no (disabled)]) 
else
    CFLAGS_orig=$CFLAGS

    # Check using -fvisibility=hidden
    ac_add=-fvisibility=hidden
    CFLAGS="$ac_add -Werror"

    AC_MSG_CHECKING([if $CC supports $ac_add])
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[
        #include <stdio.h>
        __attribute__((visibility("default"))) int foo;
        ]],[[fprintf(stderr, "Hello, world\n");]])],
        [AS_IF([test -s conftest.err],
               [$GREP -iq visibility conftest.err
                # If we find "visibility" in the stderr, then
                # assume it doesn't work
                AS_IF([test "$?" = "0"], [ac_add=])])
        ], [ac_add=])
    AS_IF([test "$ac_add" = ""],
          [AC_MSG_RESULT([no])],
          [AC_MSG_RESULT([yes])])

    CFLAGS=$CFLAGS_orig

    if test "$ac_add" != "" ; then
        AC_MSG_CHECKING([$ac_msg])
        AC_MSG_RESULT([yes (via $ac_add)])
        CFLAGS="$CFLAGS -fvisibility=hidden -DHAVE_VISIBILITY"
    elif test "$enable_visibility" = "yes"; then
        AC_MSG_ERROR([Symbol visibility support requested but compiler does not seem to support it.  Aborting])
    else
        AC_MSG_CHECKING([$ac_msg])
        AC_MSG_RESULT([no (unsupported)])
    fi
    unset ac_add
fi

RPM_RELEASE=1
AC_SUBST(RPM_RELEASE)
AC_SUBST(RPM_DIR)
RPM_TARBALL=${PACKAGE}-${VERSION}.tar.gz
AC_SUBST(RPM_TARBALL)

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    ${PACKAGE}.spec
    ${PACKAGE}.pc
    debian/rules
    debian/control
    debian/changelog
    debian/${PACKAGE}.postinst
    debian/${PACKAGE}.prerm
    ])

AC_OUTPUT

