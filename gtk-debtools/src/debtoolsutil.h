/***************************************************************************
 *            debtoolsutil.h
 *
 *  Sun Jun 19 13:58:52 2005
 *  Copyright  2005  User
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _DEBTOOLSUTIL_H
#define _DEBTOOLSUTIL_H

#include <apt-pkg/debfile.h>
#include <apt-pkg/error.h>
#include <apt-pkg/extracttar.h>


#include <vector>
#include <string>

using namespace std;

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

class ListDirStream:public pkgDirStream {
    vector < string > content;
  public:
    virtual bool DoItem(Item & item, int &itfd);
     vector < string > getContent(void);
};



#endif				/* _DEBTOOLSUTIL_H */
