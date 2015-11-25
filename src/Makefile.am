# Copyright (c) 2013-2015 Mellanox Technologies, Inc.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

lib_LTLIBRARIES = libibprof.la

AM_CPPFLAGS := \
	-I. \
	-I$(top_srcdir)/src/api \
	-I$(top_srcdir)/src/cmn \
	-I$(top_srcdir)/src/core \
	-I$(top_srcdir)/src/core/io

noinst_HEADERS = \
	api/ibprof_api.h \
	cmn/ibprof_cmn.h \
	core/ibprof_types.h \
	core/ibprof_task.h \
	core/ibprof_hash.h \
	core/ibprof_conf.h \
	core/io/ibprof_io.h \
	core/ibv/ibprof_ibv.h \
	core/mxm/ibprof_mxm.h \
	core/hcol/ibprof_hcol.h \
	core/pmix/ibprof_pmix.h

dist_libibprof_la_HEADERS = \
	api/ibprof_api.h

libibprof_la_SOURCES = \
	./api/ibprof_api.c \
	./cmn/ibprof_cmn.c \
	./core/ibprof_task.c \
	./core/ibprof_hash.c \
	./core/ibprof_conf.c \
	./core/io/ibprof_plain.c \
	./core/io/ibprof_xml.c \
	./core/ibv/ibprof_ibv.c \
	./core/mxm/ibprof_mxm.c \
	./core/hcol/ibprof_hcol.c \
	./core/pmix/ibprof_pmix.c

libibprof_ladir = $(includedir)

