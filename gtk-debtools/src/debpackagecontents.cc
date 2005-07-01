#include "debpackagecontents.h"
#include "debtoolsutil.h"

#include <iostream>

#include <apt-pkg/debfile.h>
#include <apt-pkg/error.h>
#include <apt-pkg/extracttar.h>

#include <sys/stat.h>


/*
 * This is the constructor, which do all the job of 
 * getting the properties of the deb file. 
 */
DebPackageContents::
DebPackageContents (string filename) throw (DebViewerException)
{
	FileFd *fd;
	debDebFile *df;
	debDebFile::MemControlExtract ctrl;
	fd = new FileFd (filename, FileFd::ReadOnly);
	if (_error->PendingError ())
	{
		_error->DumpErrors ();
		_error->Error ("Failure opening package package for read.");
		throw DebViewerException (201,
					  "Failure opening package for read");

	}
	df = new debDebFile (*fd);
	if (!ctrl.Read (*df))
	{
		_error->DumpErrors ();
		_error->Error ("Failure reading the package's contents.");
		throw DebViewerException (201,
					  "Failure reading package's contents");
	}
	if (!ctrl.Control)
	{
		_error->DumpErrors ();
		_error->Error
			("Could not extract the package's control record.");
		throw DebViewerException (202,
					  "Could not extract the package's control record");
	}

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
	stringtok (auxdepends, ctrl.Section.FindS ("Depends"), ", ");
	this->setDepends (auxdepends);
	/*
	 * Get the Predepends 
	 */
	vector < string > auxpredepends;
	stringtok (auxpredepends, ctrl.Section.FindS ("Predepends"), " ");
	this->setPreDepends (auxpredepends);
	/*
	 * Get the Recommends 
	 */
	vector < string > auxrecommends;
	stringtok (auxrecommends, ctrl.Section.FindS ("Recommends"), " ");
	this->setRecommends (auxrecommends);
	/*
	 * Get the Suggests 
	 */
	vector < string > auxsuggests;
	stringtok (auxsuggests, ctrl.Section.FindS ("Suggests"), " ");
	this->setSuggests (auxsuggests);
	/*
	 * Get the Conflicts 
	 */
	vector < string > auxconflicts;
	stringtok (auxconflicts, ctrl.Section.FindS ("Conflicts"), " ");
	this->setConflicts (auxconflicts);
	/*
	 * Get the Provides 
	 */
	vector < string > auxprovides;
	stringtok (auxprovides, ctrl.Section.FindS ("Conflicts"), " ");
	this->setProvides (auxprovides);
	/*
	 * Get the Replaces 
	 */
	vector < string > auxreplaces;
	stringtok (auxreplaces, ctrl.Section.FindS ("Replaces"), " ");
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
		this->setSize ("Unknown");
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
			stringtok (aux,"Unknown");
			this->setFileList (aux);
		}
		extractProg = "bzip2";
	}
	ExtractTar tpart (df->GetFile (), data->Size, extractProg);
	ListDirStream list;
	if (!tpart.Go (list))
	{
		vector < string > aux;
		stringtok (aux,"Unknown");
		this->setFileList (aux);
	}
	this->setFileList (list.getContent ());

}

/*
 * This is a default Constructor which will construct a fake DebPackageContents
 */
DebPackageContents::DebPackageContents ()
{
	this->setFile("No File Selected");
	this->setName ("No Package Selected");
	this->setVersion ("");
	this->setShortDescription ("");
	this->setLongDescription ("");
	this->setSection ("");
	this->setPriority ("");
	this->setInstalledSize ("");
	this->setMaintainer ("");
	vector < string > aux;
	stringtok (aux,"");
	this->setDepends (aux);
	this->setPreDepends (aux);
	this->setRecommends (aux);
	this->setSuggests (aux);
	this->setConflicts (aux);
	this->setProvides (aux);	
	this->setReplaces (aux);
}

/* 
 * This is the destructor
 */
DebPackageContents::~DebPackageContents ()
{

}

string
DebPackageContents::getName ()
{
	return this->name;
}

string
DebPackageContents::getShortDescription ()
{
	return this->shortdescription;
}

string
DebPackageContents::getMaintainer ()
{
	return this->maintainer;
}

string
DebPackageContents::getSection ()
{
	return this->section;
}

string
DebPackageContents::getPriority ()
{
	return this->priority;
}

string
DebPackageContents::getFile ()
{
	return this->file;
}

string
DebPackageContents::getVersion ()
{
	return this->version;
}

string
DebPackageContents::getSize ()
{
	return this->size;
}

string
DebPackageContents::getInstalledSize ()
{
	return this->installedsize;
}

string
DebPackageContents::getLongDescription ()
{
	return this->longdescription;
}

vector < string > DebPackageContents::getPreDepends ()
{
	return this->predepends;
}

vector < string > DebPackageContents::getDepends ()
{
	return this->depends;
}

vector < string > DebPackageContents::getRecommends ()
{
	return this->recommends;
}

vector < string > DebPackageContents::getSuggests ()
{
	return this->suggests;
}

vector < string > DebPackageContents::getConflicts ()
{
	return this->conflicts;
}

vector < string > DebPackageContents::getReplaces ()
{
	return this->replaces;
}

vector < string > DebPackageContents::getProvides ()
{
	return this->provides;
}

vector < string > DebPackageContents::getFileList ()
{
	return this->filelist;
}

void
DebPackageContents::setName (string name)
{
	this->name = name;
}

void
DebPackageContents::setShortDescription (string shortdescription)
{
	this->shortdescription = shortdescription;
}

void
DebPackageContents::setMaintainer (string maintainer)
{
	this->maintainer = maintainer;
}

void
DebPackageContents::setSection (string section)
{
	this->section = section;
}

void
DebPackageContents::setPriority (string priority)
{
	this->priority = priority;
}

void
DebPackageContents::setFile (string file)
{
	this->file = file;
}

void
DebPackageContents::setVersion (string version)
{
	this->version = version;
}

void
DebPackageContents::setSize (string size)
{
	this->size = size;
}

void
DebPackageContents::setInstalledSize (string installedsize)
{
	this->installedsize = installedsize;
}

void
DebPackageContents::setLongDescription (string longdescription)
{
	this->longdescription = longdescription;
}

void
DebPackageContents::setPreDepends (vector < string > predepends)
{
	this->predepends = predepends;
}

void
DebPackageContents::setDepends (vector < string > depends)
{
	this->depends = depends;
}

void
DebPackageContents::setRecommends (vector < string > recommends)
{
	this->recommends = recommends;
}

void
DebPackageContents::setSuggests (vector < string > suggests)
{
	this->suggests = suggests;
}

void
DebPackageContents::setConflicts (vector < string > conflicts)
{
	this->conflicts = conflicts;
}

void
DebPackageContents::setReplaces (vector < string > replaces)
{
	this->replaces = replaces;
}

void
DebPackageContents::setProvides (vector < string > provides)
{
	this->provides = provides;
}

void
DebPackageContents::setFileList (vector < string > filelist)
{
	this->filelist = filelist;
}
