# Copyright (c) 2013-2015 Mellanox Technologies, Inc.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

EXTRA_DIST = \
	README \
	LICENSE \
	NEWS \
	debian \
	$(PACKAGE).pc.in
	

SUBDIRS = src
docdir=$(prefix)
dist_doc_DATA = README

ACLOCAL_FLAGS = -I config/m4

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=$(PACKAGE).pc

rpmspec = $(PACKAGE).spec
rpmmacros =\
           --define='_rpmdir $(PWD)/rpm-dist'\
           --define='_srcrpmdir $(PWD)/rpm-dist'\
           --define='_sourcedir $(PWD)'\
           --define='_specdir $(PWD)'\
           --define='_builddir $(PWD)'

rpmopts = --nodeps --buildroot='$(PWD)/_rpm'

rpmcheck:
	@which rpmbuild &> /dev/null; \
		if [ $$? -ne 0 ]; then \
		echo "*** This make target requires an rpm-based linux distribution."; \
		(exit 1); exit 1; \
		fi
	-mkdir -p rpm-dist

srcrpm: distcheck rpmcheck $(rpmspec)
	rpmbuild -bs $(rpmmacros) $(rpmopts) $(rpmspec); \
		if [ $$? -ne 0 ]; then \
		(exit 1); exit 1; \
		fi

binrpm: distcheck rpmcheck $(rpmspec)
	rpmbuild -bb $(rpmmacros) $(rpmopts) $(rpmspec); \
		if [ $$? -ne 0 ]; then \
		(exit 1); exit 1; \
		fi

rpms: binrpm srcrpm

pack:
	./autogen.sh && ./configure && make dist
