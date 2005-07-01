/***************************************************************************
 *            debtoolsexception.h
 *
 *  Tue Jun 21 20:17:53 2005
 *  Copyright  2005  Josué Alcalde González
 *  Email josuealcalde@gmail.com
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

#ifndef _DEBVIEWEREXCEPTION_H
#define _DEBVIEWEREXCEPTION_H

#include <string>
#include <map>

using namespace std;

/* 
 * DebToolsException extends exception
 */
class DebToolsException: public exception {
  public:
    DebToolsException (int exNumber = 100, char * exDesc = NULL); 
  	const char* what() const throw(); 
  protected:
    int exNumber;
  	char* exDesc;
};

class DebViewerException: public DebToolsException {
  public:
    DebViewerException(int exNumber = 200, char * exDesc = NULL) : 
  			DebToolsException (exNumber, exDesc){};
};


#endif				

/* _DEBVIEWEREXCEPTION_H */
