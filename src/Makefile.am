AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-svg
moba_svg_SOURCES = \
    main.cpp \
    layoutparser.cpp \
    svgdocument.cpp


moba_svg_LDADD = -lMoBaCommon $(GTKMM_LIBS) $(srcdir)/../modules/lib-tracklayout/src/moba/libtracklayout.a
LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = $(GTKMM_CFLAGS) -I$(srcdir)/../modules/lib-tracklayout/src -g3 -ggdb -O3 -Wall

CLEANFILES = *~
