/* rgdebviewer.h
 *
 * Copyright (c) 2005 Josué Alcalde González
 *
 * Author: Josué Alcalde González <josuealcalde@gmail.com>
 *
 * Created on: Wed Jul 13 19:44:43 2005
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef _RGDEBVIEWER_H_
#define _RGDEBVIEWER_H_


#include <gtk/gtk.h>
#include <glade/glade.h>

#include "debpackage.h"
#include "rggladewindow.h"


class RGDebViewer : public RGGladeWindow {
	protected:
		/* The package which is open */
		static DebPackage *_package;
		/* Callbacks */
		static void on_quit_activate (GtkMenuItem *item, gpointer user_data);
		static void on_about_activate (GtkMenuItem *item, gpointer user_data);
		static void on_install_activate (GtkMenuItem *item, gpointer user_data);
		static void on_open_activate (GtkMenuItem *item, gpointer user_data);
		static void on_compress_activate (GtkMenuItem *item, gpointer user_data);
		static void on_button_open_clicked (GtkWidget *self, void *data);
		static void on_button_install_clicked (GtkWidget *self, void *data);
		static void on_button_compress_clicked (GtkWidget *self, void *data);
		static void on_window_debviewer_destroy (GtkWidget *self, void *data);
		static void on_optionmenu_depends_changed (GtkWidget *self, void *data);	
		/* Some methods which make the job */
  		static void cmd_exit ();
  		static void cmd_about (RGDebViewer *parent);
  		static void cmd_install ();
  		static void cmd_compress ();
  		static void cmd_open (RGDebViewer *parent);
		
	public:
		RGDebViewer();
		~RGDebViewer();
		static void fillInValues(RGGladeWindow *me, DebPackage *pkg, bool setTitle=false);
		static void fillInValues(RGGladeWindow *me, string *strpkg, bool setTitle=false);
		virtual bool close();
};

#endif	//_RGDEBVIEWER_H_

/*
class RGDebViewer
{
	private:
		RGDebViewer ();
    	static DebPackage *_package;
		static RGDebViewer *instance;
		static GladeXML *_gladeXML;
	public:
		~RGDebViewer ();
		static RGDebViewer *getInstance ();
		static void showPackage (string* debPackage);
		static DebPackage * getPackage ();
    	static GladeXML* getGladeXML();
  protected:
		static bool setLabel (const char *name, const char *value);
		static bool setTextView (const char *widget_name, const char *value, bool useHeadline = false);
		static bool setTreeList (const char *widget_name,  vector < string > values, bool useMarkup = false);		
		static void on_quit_activate(GtkMenuItem *item, gpointer user_data);
		static void on_about_activate(GtkMenuItem *item, gpointer user_data);
		static void on_install_activate(GtkMenuItem *item, gpointer user_data);
		static void on_open_activate(GtkMenuItem *item, gpointer user_data);
		static void on_compress_activate(GtkMenuItem *item, gpointer user_data);
		static void on_button_open_clicked(GtkWidget *self, void *data);
		static void on_button_install_clicked(GtkWidget *self, void *data);
		static void on_button_compress_clicked(GtkWidget *self, void *data);
		static void on_window_main_destroy(GtkWidget *self, void *data);
		static void on_optionmenu_depends_changed(GtkWidget *self, void *data);	
  		static void fcompress();
  		static void fexit();
  		static void fabout();
  		static void finstall();
  		static string* fopen();
};

*/
