/* debviewer.cc
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

#include "config.h"

#include <cassert>
#include "rgwindow.h"
#include "rgdebviewer.h"
#include "rggladewindow.h"
#include "debpackage.h"
#include "sections_trans.h"

/*
 * Constructor and Destructor
 */

DebPackage* RGDebViewer::_package = 0;

RGDebViewer::RGDebViewer() : RGGladeWindow(NULL, "debviewer")
{
	glade_xml_signal_connect_data (_gladeXML, "on_quit_activate",
				  G_CALLBACK (on_quit_activate), this);
	glade_xml_signal_connect (_gladeXML, "on_window_debviewer_destroy",
				  G_CALLBACK (on_window_debviewer_destroy)); 
	glade_xml_signal_connect_data (_gladeXML, "on_install_activate",
				  G_CALLBACK (on_install_activate), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_open_activate",
				  G_CALLBACK (on_open_activate), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_compress_activate",
				  G_CALLBACK (on_compress_activate), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_about_activate",
				  G_CALLBACK (on_about_activate), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_button_open_clicked",
				  G_CALLBACK (on_button_open_clicked), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_button_install_clicked",
				  G_CALLBACK (on_button_install_clicked), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_button_compress_clicked",
				  G_CALLBACK (on_button_compress_clicked), this); 
	glade_xml_signal_connect_data (_gladeXML, "on_optionmenu_depends_changed",
				  G_CALLBACK (on_optionmenu_depends_changed), this);
	this->show();
}

RGDebViewer::~RGDebViewer ()
{
	
}

/*
 * Close the window
 */ 
bool RGDebViewer::close()
{
	hide();
	cmd_exit();
	return true;
}

/*
 * Fill the values in the window
 */
void RGDebViewer::fillInValues(RGGladeWindow *me, string *strpkg, bool setTitle)
{
	DebPackage *pkg;
	if (strpkg == NULL){
		pkg = new DebPackage();
	}
	else{
		try{
			pkg = new DebPackage(*strpkg);
		}
		catch(DebPackageException ex){
			GtkWidget *statusbox = gtk_message_dialog_new(GTK_WINDOW(me->window()),
                                GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                ex.what()); 
			gtk_dialog_run(GTK_DIALOG(statusbox));
			gtk_widget_destroy(statusbox);
			pkg = new DebPackage();
		}		
	}
	fillInValues(me,pkg,setTitle);
}

void RGDebViewer::fillInValues(RGGladeWindow *me, DebPackage *pkg, bool setTitle)
{
	
	assert(me!=NULL);
	if (pkg == NULL){
		_package = new DebPackage();
	}
	else{
		_package = pkg;
	}
	if(setTitle) {
		me->setTitle("Debian Package Viewer");
		gchar *str = g_strdup_printf(_("%s - %s"),
				gtk_window_get_title (GTK_WINDOW(me->window())),
				_package->getName().c_str());
		me->setTitle(str);
		g_free(str);
	}
	char *pkg_summary = g_strdup_printf("%s\n%s",
					_package->getName().c_str(), 
					_package->getShortDescription().c_str());
	me->setTextView("text_pkgcommon", pkg_summary, true);
	me->setTextView("text_pkgcommon2", pkg_summary, true);
	g_free(pkg_summary);
	me->setLabel("label_maintainer", _package->getMaintainer().c_str());
	me->setLabel("label_file", _package->getFile().c_str());
	me->setLabel("label_priority", _package->getPriority().c_str());
	me->setLabel("label_section", trans_section(_package->getSection()).c_str());
	me->setLabel("label_size", _package->getSize().c_str());
	me->setLabel("label_installedsize", _package->getInstalledSize().c_str());
	me->setLabel("label_version", _package->getVersion().c_str());
	
	gchar *pkg_description = g_strdup_printf("%s\n%s", 
					_package->getShortDescription().c_str(),
					_package->getLongDescription().c_str());
	me->setTextView("text_descr", pkg_description, true);
	g_free(pkg_description);
    
	gchar *pkg_filelist = g_strdup("");
	if (_package->getFileList().size() >1){
	  for (unsigned int i = 1; i < _package->getFileList().size(); i ++){
		pkg_filelist = g_strconcat(pkg_filelist,"/", 
				_package->getFileList().at(i).c_str(), "\n");
	  }
    }
	me->setTextView("text_files", pkg_filelist, false);
	g_free(pkg_filelist);
	
	vector<string> deplistStrings;
	vector<string> providesStrings;
	
	for (unsigned int i = 0; i < _package->getDepends().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Depends")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getDepends().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getPreDepends().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("PreDepends")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getPreDepends().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getRecommends().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Recommends")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getRecommends().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getSuggests().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Suggests")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getSuggests().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getConflicts().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Conflicts")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getConflicts().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getReplaces().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Replaces")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getReplaces().at(i).c_str(),-1));
		deplistStrings.push_back(depStr);
	}
	for (unsigned int i = 0; i < _package->getProvides().size(); i++)
	{
	    string depStr = "";
		depStr += string("<b>") + string(_("Provides")) + string(":</b>");
 		depStr += string(g_markup_escape_text(_package->getProvides().at(i).c_str(),-1));
		providesStrings.push_back(depStr);
	}
	
	me->setTreeList("treeview_deplist", deplistStrings, true);
	me->setTreeList("treeview_provides", providesStrings, true);

}


/*
 * Callbacks
 */

void
RGDebViewer::on_quit_activate (GtkMenuItem * item, gpointer user_data)
{
	cmd_exit();
}

void
RGDebViewer::on_install_activate (GtkMenuItem * item, gpointer user_data)
{
	cmd_install();
}

void
RGDebViewer::on_open_activate (GtkMenuItem * item, gpointer user_data)
{
	cmd_open((RGDebViewer*)user_data);
}

void
RGDebViewer::on_compress_activate (GtkMenuItem * item, gpointer user_data)
{
	cmd_compress();
}

void
RGDebViewer::on_about_activate (GtkMenuItem * item, gpointer user_data)
{
	cmd_about((RGDebViewer*)user_data);
}

void
RGDebViewer::on_button_open_clicked (GtkWidget * self, void *data)
{
	cmd_open((RGDebViewer*)data);
}

void
RGDebViewer::on_button_install_clicked (GtkWidget * self, void *data)
{
	cmd_install();
}

void
RGDebViewer::on_button_compress_clicked (GtkWidget * self, void *data)
{
	cmd_compress();
}

void
RGDebViewer::on_window_debviewer_destroy (GtkWidget * self, void *data)
{
	cmd_exit();
}

void
RGDebViewer::on_optionmenu_depends_changed (GtkWidget * self, void *data)
{
   /* Code from Synaptic */
   RGDebViewer *me = (RGDebViewer*)data;
   int nr =  gtk_option_menu_get_history(GTK_OPTION_MENU(self));
   GtkWidget *notebook = glade_xml_get_widget(me->_gladeXML, "notebook_dep_tab");
   assert(notebook);
   gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), nr);
}


/*
 * Methods to do the job
 */

void
RGDebViewer::cmd_about (RGDebViewer *parent)
{
	RGGladeWindow *window_about = new RGGladeWindow((RGWindow*)parent, "about");
	gtk_window_set_icon(GTK_WINDOW(window_about->window()), 
			gtk_window_get_icon(GTK_WINDOW(parent->window())));
	window_about->show();
}

void
RGDebViewer::cmd_exit ()
{
	gtk_main_quit ();
}

void
RGDebViewer::cmd_install ()
{
	// TODO
}

void
RGDebViewer::cmd_compress ()
{
	// TODO	
}



void
RGDebViewer::cmd_open (RGDebViewer *parent)
{
	string* filepath = new string("");
	GtkWidget *fileselection;
	GtkFileFilter *filterdeb, *filterall;
	gint dialogval;
	GError *dialogerror = NULL;
	
	fileselection = gtk_file_chooser_dialog_new(_("Open Debian Package File"),
			GTK_WINDOW(parent->window()), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, 
			GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (fileselection), GTK_RESPONSE_ACCEPT);
	filterdeb = gtk_file_filter_new ();
	filterall = gtk_file_filter_new ();
	gtk_file_filter_set_name (filterdeb, _("Deb Files (*.deb)"));
    gtk_file_filter_add_pattern(filterdeb, "*.deb");
	gtk_file_filter_set_name (filterall, _("All Files (*.*)"));
    gtk_file_filter_add_pattern(filterall, "*.*");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (fileselection), filterdeb);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (fileselection), filterall);
  	dialogval = gtk_dialog_run(GTK_DIALOG(fileselection));
  	gtk_widget_hide (fileselection);

	if (dialogval == GTK_RESPONSE_ACCEPT) {
		gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (fileselection));
		filepath = new string(filename);
		g_free(filename);

    	if (dialogerror) {
			GtkWidget *statusbox = gtk_message_dialog_new(GTK_WINDOW(parent->window()),
                                GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                dialogerror->message); 
			gtk_dialog_run(GTK_DIALOG(statusbox));

			gtk_widget_destroy(statusbox);	
			g_error_free(dialogerror);

		}
		fillInValues((RGGladeWindow*)parent,filepath,true);
	}
	gtk_widget_destroy(fileselection);
}
