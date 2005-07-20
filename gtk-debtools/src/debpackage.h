/* debpackage.h
 *
 * Copyright (c) 2005 Josué Alcalde González
 *
 * Author: Josué Alcalde González <josuealcalde@gmail.com>
 *
 * Created on: Wed Jul 13 00:59:55 2005 
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
 
#ifndef _DEBPACKAGE_H_
#define _DEBPACKAGE_H_

#include <vector>
#include <string>

using namespace std;


//------------------------------------------------------------------------------
// debpackage Declaration
//------------------------------------------------------------------------------

class DebPackageException: public exception {
	public:
		DebPackageException (int exNumber = 0, char * exDesc = NULL): 
				_exNumber(exNumber), _exDesc(exDesc) { } ; 
		const char* what() const throw() { return _exDesc;}; 
	protected:
		int _exNumber;
		char* _exDesc;
};

class DebPackage
{
	public:
		DebPackage(string filepath) throw (DebPackageException);
		DebPackage(); /* Constructor for a fake package */
		string getName () { return name; };
		string getShortDescription () { return shortdescription; };
		string getMaintainer () { return maintainer; };
		string getSection () { return section; };
		string getPriority () { return priority; };
		string getFile () { return file; };
		string getVersion () { return version; };
		string getSize () { return size; };
		string getInstalledSize () { return installedsize; };
		string getLongDescription () { return longdescription; };
		vector <string> getPreDepends () { return predepends; };
		vector <string> getDepends () { return depends; };
		vector <string> getRecommends () { return recommends; };
		vector <string> getSuggests () { return suggests; };
		vector <string> getConflicts () { return conflicts; };
		vector <string> getReplaces () { return replaces; };
		vector <string> getProvides () { return provides; };
		vector <string> getFileList () { return filelist; };
	protected:
		void setName (string name) { this->name = name; };
		void setShortDescription (string shortdescription) { this->shortdescription = shortdescription; };
		void setMaintainer (string maintainer) { this->maintainer = maintainer; };
		void setSection (string section) { this->section = section; };
		void setPriority (string priority) { this->priority = priority; };
		void setFile (string file) { this->file = file; };
		void setVersion (string version) { this->version = version; };
		void setSize (string size) { this->size = size; };
		void setInstalledSize (string installedsize) { this->installedsize = installedsize; };
		void setLongDescription (string longdescription) { this->longdescription = longdescription; };
		void setPreDepends (vector < string > predepends) { this->predepends = predepends; };
		void setDepends (vector < string > depends) { this->depends = depends; };
		void setRecommends (vector < string > recommends) { this->recommends = recommends; };
		void setSuggests (vector < string > suggests) { this->suggests = suggests; };
		void setConflicts (vector < string > conflicts) { this->conflicts = conflicts; };
		void setReplaces (vector < string > replaces) { this->replaces = replaces; };
		void setProvides (vector < string > provides) { this->provides = provides; };
		void setFileList (vector < string > filelist) { this->filelist = filelist; };		
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
		vector <string> predepends;
		vector <string> depends;
		vector <string> recommends;
		vector <string> suggests;
		vector <string> conflicts;
		vector <string> replaces;
		vector <string> provides;
    	vector <string> filelist;
};

#endif /* _DEBPACKAGE_H_ */
