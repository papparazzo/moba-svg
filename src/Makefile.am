AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-svg

moba_svg_SOURCES = \
    main.cpp \
    layoutparser.cpp \
    svgdocument.cpp \
    msgloop.cpp


moba_svg_LDADD = \
    -lmobacommon \
    $(GTKMM_LIBS) \
    $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a \
    $(srcdir)/../modules/lib-tracklayout/src/moba/libtracklayout.a

LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = \
    $(GTKMM_CFLAGS) \
    -I$(srcdir)/../modules/lib-msghandling/src \
    -I$(srcdir)/../modules/lib-tracklayout/src \
    -g3 -g -ggdb -O3 -Wall

CLEANFILES = *~
