ACLOCAL_AMFLAGS = -I m4

includedir = $(prefix)/include/libesc

lib_LTLIBRARIES = libesc.la

include_HEADERS = $(top_srcdir)/include/libecat-subdev/coe.h

libesc_la_SOURCES	= coe.c mailbox.c hw.c object.c ecsd.c $(top_srcdir)/libecat-base/src/common.c

libesc_la_CFLAGS	= -std=gnu99 -Wall -Wno-unused-const-variable -I$(top_srcdir)/include -I$(top_srcdir)/libecat-base/include
libesc_la_LDFLAGS	= -Wall -no-undefined
libesc_la_LIBS      = 

LDADD = 
LIBS  = 

bin_PROGRAMS = subdevtest
subdevtest_SOURCES = main.c coe.c mailbox.c hw.c object.c ecsd.c $(top_srcdir)/libecat-base/src/common.c
subdevtest_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/libecat-base/include

install-exec-hook: 
	find $(DESTDIR)$(libdir) -type f -name \*.la -delete


