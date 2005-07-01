## Process this file with automake to produce Makefile.in
## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

SUBDIRS = intl po macros src pixmaps

gtk_debtoolsdocdir = ${prefix}/doc/gtk-debtools
gtk_debtoolsdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO\
	ABOUT-NLS

EXTRA_DIST = $(gtk_debtoolsdoc_DATA)

gnomemenudir = $(prefix)/@NO_PREFIX_PACKAGE_MENU_DIR@/Utilities
gnomemenu_DATA = gtk-debtools.desktop

gtk_debtools_glade_filedir = $(prefix)/@NO_PREFIX_PACKAGE_DATA_DIR@/@PACKAGE@/glade
gtk_debtools_glade_file_DATA = gtk-debtools.glade

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

