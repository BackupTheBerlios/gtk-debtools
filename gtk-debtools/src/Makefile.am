## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES = \
	$(GNOME_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

AM_CXXFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gtk-debtools

gtk_debtools_SOURCES = \
	main.cc\
	debpackagecontents.cc\
	debpackagecontents.h\
	debpackageshow.cc\
	debpackageshow.h\
	debtoolsexception.cc\
	debtoolsexception.h\
	debtoolsutil.cc\
	debtoolsutil.h

gtk_debtools_LDFLAGS = 

gtk_debtools_LDADD =  \
	$(GNOME_LIBS)\
	 -lapt-pkg -lapt-inst -lglade -lgtk

