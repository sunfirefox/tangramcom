/*
* talk server
*
* Copyright 2009  Jokul for Tranzda
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef UTLI_H
#define UTLI_H
#include "OSBaseTypes.h"
int	u_strcasecmp(const char *s1, const WCHAR *s2);
int	u_strcasecmp(const char *s1, const char *s2);

char *		q_ustrcpy(char *pszTarget, const WCHAR *pszSrc);
WCHAR *		q_ustrcpy(WCHAR *pszTarget, const WCHAR *pszSrc);
WCHAR *		q_ustrcpy(WCHAR *pszTarget, const char *pszSrc);
char*		q_ustrcpy(char *pszTarget, const char *pszSrc);



#define wstr_from_astr(str) ((const WCHAR*)(CSTR_A2W(str)))
class CSTR_A2W{
public:
	CSTR_A2W(const char* inStr){
		strW = new WCHAR[strlen(inStr)+1];
		q_ustrcpy(strW,inStr);
	}
	~CSTR_A2W(){delete [] strW;}

	operator const WCHAR*(){return strW;};
private:
	WCHAR*	strW;
};

#endif
