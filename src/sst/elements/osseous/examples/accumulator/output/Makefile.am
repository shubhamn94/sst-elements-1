# -*- Makefile -*-
#
#
INCLUDE = -I$(SST_WORKDIR)/src/sst/elements/osseous/eras_datatypes/ -I$(SST_WORKDIR)/src/sst/elements/osseous/examples/accumulator/tests/ -I$(SST_WORKDIR)/src/sst/elements/ariel/
FLAGS = -fpermissive -std=c++11

AM_CPPFLAGS = \
	$(MPI_CPPFLAGS) \
	$(INCLUDE) \
	$(FLAGS)

compdir = $(pkglibdir)
comp_LTLIBRARIES = libsstAccumulator.la 
libsstAccumulator_la_SOURCES = \
	sstAccumulator.h \
	sstAccumulator.cc \
	rtlevent.h \
	rtlevent.cc \
	rtlmemmgr.h \
	rtlmemmgr_cache.h \
	rtlmemmgr_simple.h \
	rtlmemmgr_simple.cc \
	rtlreadev.h \
	rtlwriteev.h 

libsstAccumulator_la_LDFLAGS = -module -avoid-version

install-exec-hook:
	$(SST_REGISTER_TOOL) SST_ELEMENT_SOURCE  sstAccumulator=$(abs_srcdir) 

