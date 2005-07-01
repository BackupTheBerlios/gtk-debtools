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

#ifndef _DEBPACKAGECONTENTS_H_
#define _DEBPACKAGECONTENTS_H_

#include <vector>
#include <string>
#include "debtoolsutil.h"
#include "debtoolsexception.h"

using namespace std;

class DebPackageContents
{
      private:
	string name;
	string shortdescription;
	string maintainer;
	string section;
	string priority;
	string file;
	string version;
	string size;
	string installedsize;
	string longdescription;
	vector < string > predepends;
	vector < string > depends;
	vector < string > recommends;
	vector < string > suggests;
	vector < string > conflicts;
	vector < string > replaces;
	vector < string > provides;
    vector < string > filelist;
      public:
	/* Constructor and Destructor */
	  DebPackageContents (); /* Constructor for a fake DebPackageContents */
	  DebPackageContents (string filepath) throw (DebViewerException);
	  virtual ~ DebPackageContents ();
	/* Get Methods */
	string getName ();
	string getShortDescription ();
	string getMaintainer ();
	string getSection ();
	string getPriority ();
	string getFile ();
	string getVersion ();
	string getSize ();
	string getInstalledSize ();
	string getLongDescription ();
	  vector < string > getPreDepends ();
	  vector < string > getDepends ();
	  vector < string > getRecommends ();
	  vector < string > getSuggests ();
	  vector < string > getConflicts ();
	  vector < string > getReplaces ();
	  vector < string > getProvides ();
	  vector < string > getFileList ();

      protected:
	/* Set Methods */
	void setName (string name);
	void setShortDescription (string shortdescription);
	void setMaintainer (string maintainer);
	void setSection (string section);
	void setPriority (string priority);
	void setFile (string file);
	void setVersion (string version);
	void setSize (string size);
	void setInstalledSize (string installedsize);
	void setLongDescription (string longdescription);
	void setPreDepends (vector < string > predepends);
	void setDepends (vector < string > depends);
	void setRecommends (vector < string > recommends);
	void setSuggests (vector < string > suggests);
	void setConflicts (vector < string > conflicts);
	void setReplaces (vector < string > replaces);
	void setProvides (vector < string > provides);
	void setFileList (vector < string > filelist);
};

#endif /*_DEBPACKAGECONTENTS_H_*/
