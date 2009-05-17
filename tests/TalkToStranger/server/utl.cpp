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
#include "utl.h"

static inline WCHAR utolower(WCHAR c)
{
	if('A' <= c && c <= 'Z')
		c |= 0x20;
	return c;
}

static inline char utolower(char c)
{
	if('A' <= c && c <= 'Z')
		c |= 0x20;
	return c;
}


int	u_strcasecmp(const char *s1, const char *s2)
{
	const char *us1 = (const char *)s1, *us2 = (const char *)s2;

	while (utolower(*us1) == utolower(*us2)) {
		if (*us1++ == '\0')
			return (0);
		us2++;
	}
	return (utolower(*us1) - utolower(*us2));
}

int	u_strcasecmp(const char *s1, const WCHAR *s2)
{
	const char *us1 = (const char *)s1;
	const WCHAR *us2 = (const WCHAR *)s2;

	while (utolower(*us1) == utolower(*us2)) {
		if (*us1++ == '\0')
			return (0);
		us2++;
	}
	return (utolower(*us1) - utolower(*us2));
}

char *q_ustrcpy(char *pszTarget, const WCHAR *pszSrc)
{
	char *t = pszTarget;
	while((*t++ = ((*pszSrc++)&0xFF)) != 0)
	{
	}
	return pszTarget;
}	

WCHAR *q_ustrcpy(WCHAR *pszTarget, const char *pszSrc)
{
	WCHAR *t = pszTarget;
	while((*t++ = *pszSrc++) != 0)
	{
	}
	return pszTarget;
}

WCHAR *q_ustrcpy(WCHAR *pszTarget, const WCHAR *pszSrc)
{
	WCHAR *t = pszTarget;
	while((*t++ = *pszSrc++) != 0)
	{
	}
	return pszTarget;
}

char* q_ustrcpy(char *pszTarget, const char *pszSrc)
{
	return ::strcpy(pszTarget, pszSrc), pszTarget;
}

