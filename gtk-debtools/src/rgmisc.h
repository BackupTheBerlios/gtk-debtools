/* rgmisc.h
 * 
 * Copyright (c) 2003 Michael Vogt
 * 
 * Author: Michael Vogt <mvo@debian.org>
 * 
 * Modified by: Josué Alcalde González <josuealcalde@gmail.com>
 *
 * This source code has been taken from Synaptic
 * Has been modified to switch gtk-debtools requirements
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

#ifndef _RGMISC_H_
#define _RGMISC_H_

#include <apt-pkg/configuration.h>
#include <apt-pkg/pkgrecords.h>
#include <apt-pkg/debfile.h>
#include <apt-pkg/error.h>
#include <apt-pkg/extracttar.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include <vector>
#include <string>

using namespace std;

enum {
   PIXMAP_COLUMN,
   SUPPORTED_COLUMN,
   NAME_COLUMN,
   COMPONENT_COLUMN,
   SECTION_COLUMN,
   PKG_SIZE_COLUMN,
   PKG_DOWNLOAD_SIZE_COLUMN,
   INSTALLED_VERSION_COLUMN,
   AVAILABLE_VERSION_COLUMN,
   DESCR_COLUMN,
   COLOR_COLUMN,
   PKG_COLUMN,
   N_COLUMNS
};

void RGFlushInterface();

bool is_binary_in_path(const char *program);

char *gtk_get_string_from_color(GdkColor * colp);
void gtk_get_color_from_string(const char *cpp, GdkColor ** colp);

const char *utf8_to_locale(const char *str);
const char *utf8(const char *str);

GtkWidget *get_gtk_image(const char *name);

string SizeToStr(double Bytes);

/* 
 * Code from gcc.gnu.org
 * A string tokenizer 
 */
template < typename Container > void
stringtok(Container & container, std::string const &in,
	  const char *const delimiters = " \t\n")
{
    const string::size_type len = in.length();
    string::size_type i = 0;

    while (i < len) {
	// eat leading whitespace
	i = in.find_first_not_of(delimiters, i);
	if (i == string::npos)
	    return;		// nothing left but white space

	// find the end of the token
	string::size_type j = in.find_first_of(delimiters, i);

	// push token
	if (j == string::npos) {
	    container.push_back(in.substr(i));
	    return;
	} else
	    container.push_back(in.substr(i, j - i));
	 	// set up for next loop
	    i = j + 1;
    }
}

/**
 * Code adapted from gdeb Lalo Martins <lalo@debian.org>
 */
class ListDirStream:public pkgDirStream {
		vector < string > content;
	public:
		virtual bool DoItem(Item & item, int &itfd) { content.push_back(item.Name); return true; };
		vector < string > getContent(void) { return content; };
};

#endif
