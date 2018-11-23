AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-svg
moba_svg_SOURCES = \
    main.cpp \
    layoutparser.cpp \
    svgdocument.cpp 


moba_svg_LDADD = -lMoBaCommon $(GTKMM_LIBS)
LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = $(GTKMM_CFLAGS)

CLEANFILES = *~
