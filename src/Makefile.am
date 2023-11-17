ACLOCAL_AMFLAGS = -I m4

includedir = $(prefix)/include/libesc

lib_LTLIBRARIES = libesc.la

include_HEADERS = $(top_srcdir)/include/libecat-subdev/coe.h

libesc_la_SOURCES	= coe.c mailbox.c hw.c object.c ecsd.c

libesc_la_CFLAGS	= -std=gnu99 -Wall -Werror -Wno-unused-const-variable -I$(top_srcdir)/include
libesc_la_LDFLAGS	= -Wall -Werror -no-undefined
libesc_la_LIBS      = 

install-exec-hook: 
	find $(DESTDIR)$(libdir) -type f -name \*.la -delete


