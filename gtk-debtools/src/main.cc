/* Created by Anjuta version 1.2.2 */
/*	This file will not be overwritten */


#include <apt-pkg/debfile.h>
#include <apt-pkg/error.h>
#include <apt-pkg/extracttar.h>
#include <iostream>
#include <config.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gnome.h>

#include "debpackagecontents.h"

void on_window_destroy(GtkWindow *window, gpointer user_data)
{
    gtk_main_quit();

}


int main(int argc, char ** argv)
{
	string anydeb ("/opt/debian/pool/bum_1.3.0-1_all.deb");
	
	
	DebPackageContents dpc(anydeb);
	cout << "Archivo: " << dpc.getFile() << endl;
	cout << "Nombre: " << dpc.getName() << endl;
	cout << "Descripcion:\n" << dpc.getShortDescription() << endl << "---" << endl;
	cout << dpc.getLongDescription() << endl;
	cout << "Maintainer: " << dpc.getMaintainer() << endl;
	cout << "Section: " << dpc.getSection() << endl;
	cout << "Priority: " << dpc.getPriority() << endl;
	cout << "Version: " << dpc.getVersion() << endl;
	cout << "Size: " << dpc.getSize() << endl;
	cout << "Installed Size: " << dpc.getInstalledSize() << endl;
	cout << "Version: " << dpc.getVersion() << endl;
	cout << "Size: " << dpc.getSize() << endl;
	cout << "Installed Size: " << dpc.getInstalledSize() << endl;
	for (unsigned int i = 0; i < dpc.getPreDepends().size(); i ++){
		cout << "PreDepends: " << dpc.getPreDepends().at(i) << endl;
	}
	for (unsigned int i = 0; i < dpc.getDepends().size(); i ++){
		cout << "Depends: " << dpc.getDepends().at(i) << endl;
	}
	for (unsigned int i = 0; i < dpc.getSuggests().size(); i ++){
		cout << "Suggests: " << dpc.getSuggests().at(i) << endl;
	}
	for (unsigned int i = 0; i < dpc.getConflicts().size(); i ++){
		cout << "Conflicts: " << dpc.getConflicts().at(i) << endl;
	}
	for (unsigned int i = 0; i < dpc.getProvides().size(); i ++){
		cout << "Provides: " << dpc.getProvides().at(i) << endl;
	}
	for (unsigned int i = 0; i < dpc.getReplaces().size(); i ++){
		cout << "Replaces: " << dpc.getReplaces().at(i) << endl;
	}	
	cout << "Filelist: " << endl;
	for (unsigned int i = 0; i < dpc.getFileList().size(); i ++){
		cout << "/" << dpc.getFileList().at(i) << endl;
	}
	
	
	GladeXML *xml;
	
    gtk_init(&argc, &argv);
    glade_init();

    xml = glade_xml_new("./gtk-debtools.glade", "window_main", NULL);

    if (!xml) {
        g_warning("problem while loading the .glade file");
        return 1;
    }

    glade_xml_signal_autoconnect(xml);
	
	GtkWidget * widget = glade_xml_get_widget(xml, "label_maintainer");
	if (widget == NULL) {
      cout << "widget == NULL with: " << "label_maintainer" << endl;
    }
	gtk_label_set_label(GTK_LABEL(widget), dpc.getMaintainer().c_str());
    gtk_main();
	
	return 0;
}
