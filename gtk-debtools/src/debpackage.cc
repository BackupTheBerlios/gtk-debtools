/* debpackage.cc 
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
 
#include "debpackage.h"
#include "rgmisc.h"
#include "i18n.h"

#include <sys/stat.h>

#include <apt-pkg/debfile.h>
#include <apt-pkg/error.h>
#include <apt-pkg/extracttar.h>

DebPackage::DebPackage(string filepath) throw (DebPackageException)
{
	FileFd *fd;
	debDebFile *df;
	debDebFile::MemControlExtract ctrl;
	fd = new FileFd (filepath, FileFd::ReadOnly);
	if (_error->PendingError ())
	{
		_error->DumpErrors ();
		_error->Error (_("Failure opening package package for read."));
		_error->Discard();
		throw DebPackageException (201, _("Failure opening package for read"));

	}
	df = new debDebFile (*fd);
	if (!ctrl.Read (*df))
	{
		_error->DumpErrors ();
		_error->Error (_("Failure reading the package's contents."));
		_error->Discard();
		throw DebPackageException (201, _("Failure reading package's contents"));
	}
	if (!ctrl.Control)
	{
		_error->DumpErrors ();
		_error->Error(_("Could not extract the package's control record."));
		throw DebPackageException (202, _("Could not extract the package's control record"));
	}
	
	/*
	 * Get the file 
	 */
	this->setFile (filepath);
	/*
	 * Get the name 
	 */
	this->setName (ctrl.Section.FindS ("Package"));
	/*
	 * Get the version 
	 */
	this->setVersion (ctrl.Section.FindS ("Version"));
	/*
	 * Get the ShortDescription and LongDescription 
	 */
	/*
	 * The LongDescription doesn't include ShortDescription 
	 */
	string desc = ctrl.Section.FindS ("Description");
	string::size_type pos = desc.find ('\n');
	if (pos == string::npos)
	{
		this->setShortDescription (desc);
		this->setLongDescription ("");
	}
	else
	{
		this->setShortDescription (string (desc, 0, pos));
		this->setLongDescription (string
					  (desc, pos + 1,
					   desc.length () - pos));
	}
	/*
	 * Get the Section 
	 */
	this->setSection (ctrl.Section.FindS ("Section"));
	/*
	 * Get the Priority 
	 */
	this->setPriority (ctrl.Section.FindS ("Priority"));
	/*
	 * Get the Installed Size 
	 */
	this->setInstalledSize (ctrl.Section.FindS ("Installed-Size") + "kB");
	/*
	 * Get the Maintainer 
	 */
	this->setMaintainer (ctrl.Section.FindS ("Maintainer"));
	/*
	 * Get the Depends 
	 */
	vector < string > auxdepends;
	stringtok (auxdepends, ctrl.Section.FindS ("Depends"), ",");
	if (auxdepends.size() > 0) auxdepends[0] = " " + auxdepends[0];
	this->setDepends (auxdepends);
	/*
	 * Get the Predepends 
	 */
	vector < string > auxpredepends;
	stringtok (auxpredepends, ctrl.Section.FindS ("Predepends"), ",");
	if (auxpredepends.size() > 0) auxpredepends[0] = " " + auxpredepends[0];
	this->setPreDepends (auxpredepends);
	/*
	 * Get the Recommends 
	 */
	vector < string > auxrecommends;
	stringtok (auxrecommends, ctrl.Section.FindS ("Recommends"), ",");
	if (auxrecommends.size() > 0) auxrecommends[0] = " " + auxrecommends[0];
	this->setRecommends (auxrecommends);
	/*
	 * Get the Suggests 
	 */
	vector < string > auxsuggests;
	stringtok (auxsuggests, ctrl.Section.FindS ("Suggests"), ",");
	if (auxsuggests.size() > 0) auxsuggests[0] = " " + auxsuggests[0];
	this->setSuggests (auxsuggests);
	/*
	 * Get the Conflicts 
	 */
	vector < string > auxconflicts;
	stringtok (auxconflicts, ctrl.Section.FindS ("Conflicts"), ",");
	if (auxconflicts.size() > 0) auxconflicts[0] = " " + auxconflicts[0];
	this->setConflicts (auxconflicts);
	/*
	 * Get the Provides 
	 */
	vector < string > auxprovides;
	stringtok (auxprovides, ctrl.Section.FindS ("Provides"), ",");
	if (auxprovides.size() > 0) auxprovides[0] = " " + auxprovides[0];
	this->setProvides (auxprovides);
	/*
	 * Get the Replaces 
	 */
	vector < string > auxreplaces;
	stringtok (auxreplaces, ctrl.Section.FindS ("Replaces"), ",");
	if (auxreplaces.size() > 0) auxreplaces[0] = " " + auxreplaces[0];
	this->setReplaces (auxreplaces);

	/*
	 * Get the package size 
	 */
	struct stat st;
	if (fstat (fd->Fd (), &st) == 0)
	{
		char size[65], suffix = 0;
		int fsize = st.st_size;
		if (fsize > 4096)
		{
			fsize /= 1024;
			suffix = 'k';
		}
		if (fsize > 8096)
		{
			fsize /= 1024;
			suffix = 'M';
		}
		snprintf (size, 63, "%d%cB", fsize, suffix);
		size[63] = 0;
		this->setSize (size);
	}
	else
	{
		this->setSize (_("Unknown"));
	}
	/*
	 * Get the file list 
	 */
	const char *extractProg = "gzip";
	const ARArchive::Member * data = df->GotoMember ("data.tar.gz");
	if (!data)
	{
		data = df->GotoMember ("data.tar.bz2");
		if (!data)
		{
			vector < string > aux;
			stringtok (aux,_("Unknown"));
			this->setFileList (aux);
		}
		extractProg = "bzip2";
	}
	ExtractTar tpart (df->GetFile (), data->Size, extractProg);
	ListDirStream list;
	if (!tpart.Go (list))
	{
		vector < string > aux;
		stringtok (aux,_("Unknown"));
		this->setFileList (aux);
	}
	this->setFileList (list.getContent ());
	
}


DebPackage::DebPackage()
{
	this->setFile("");
	this->setName (_("No Package"));
	this->setMaintainer ("");
	this->setSection ("");
	this->setPriority ("");
	this->setVersion ("");
	this->setShortDescription (_("No Package has been selected"));
	this->setLongDescription (_(" Select \"Open\" to view a package"));
	this->setInstalledSize ("");
	this->setSize("");
	this->setDepends (vector<string>(0));
	this->setPreDepends (vector<string>(0));
	this->setRecommends (vector<string>(0));
	this->setSuggests (vector<string>(0));
	this->setConflicts (vector<string>(0));
	this->setProvides (vector<string>(0));	
	this->setReplaces (vector<string>(0));	
	vector<string> auxvector;
	auxvector.push_back("");
	this->setFileList (auxvector);
}
