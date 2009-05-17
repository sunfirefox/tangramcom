
#include <limits.h>
#include <stdio.h>


#include "wine/unicode.h"

 int wine_is_dbcs_leadbyte( const union cptable *table, unsigned char ch )
{
	return (table->info.char_size == 2) && (table->dbcs.cp2uni_leadbytes[ch]);
}
#if 1
 WCHAR tolowerW( WCHAR ch )
{
	extern /*WINE_UNICODE_API*/ const WCHAR wine_casemap_lower[];
	return ch + wine_casemap_lower[wine_casemap_lower[ch >> 8] + (ch & 0xff)];
}


 WCHAR toupperW( WCHAR ch )
{
	extern /*WINE_UNICODE_API*/ const WCHAR wine_casemap_upper[];
	return ch + wine_casemap_upper[wine_casemap_upper[ch >> 8] + (ch & 0xff)];
}

/* the character type contains the C1_* flags in the low 12 bits */
/* and the C2_* type in the high 4 bits */
 unsigned short get_char_typeW( WCHAR ch )
{
	extern /*WINE_UNICODE_API*/ const unsigned short wine_wctype_table[];
	return wine_wctype_table[wine_wctype_table[ch >> 8] + (ch & 0xff)];
}

 int iscntrlW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_CNTRL;
}

 int ispunctW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_PUNCT;
}


 int isspaceW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_SPACE;
}


 int isdigitW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_DIGIT;
}


 int isxdigitW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_XDIGIT;
}


 int islowerW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_LOWER;
}


 int isupperW( WCHAR wc )
{
	return get_char_typeW(wc) & C1_UPPER;
}


 int isalnumW( WCHAR wc )
{
	return get_char_typeW(wc) & (C1_ALPHA|C1_DIGIT|C1_LOWER|C1_UPPER);
}


 int isalphaW( WCHAR wc )
{
	return get_char_typeW(wc) & (C1_ALPHA|C1_LOWER|C1_UPPER);
}


 int isgraphW( WCHAR wc )
{
	return get_char_typeW(wc) & (C1_ALPHA|C1_PUNCT|C1_DIGIT|C1_LOWER|C1_UPPER);
}


 int isprintW( WCHAR wc )
{
	return get_char_typeW(wc) & (C1_ALPHA|C1_BLANK|C1_PUNCT|C1_DIGIT|C1_LOWER|C1_UPPER);
}

/* some useful string manipulation routines */


 unsigned int strlenW( const WCHAR *str )
{
	const WCHAR *s = str;
	while (*s) s++;
	return s - str;
}


 WCHAR *strcpyW( WCHAR *dst, const WCHAR *src )
{
	WCHAR *p = dst;
	while ((*p++ = *src++));
	return dst;
}



 int strcmpW( const WCHAR *str1, const WCHAR *str2 )
{
	while (*str1 && (*str1 == *str2)) { str1++; str2++; }
	return *str1 - *str2;
}


 int strncmpW( const WCHAR *str1, const WCHAR *str2, int n )
{
	if (n <= 0) return 0;
	while ((--n > 0) && *str1 && (*str1 == *str2)) { str1++; str2++; }
	return *str1 - *str2;
}


 WCHAR *strcatW( WCHAR *dst, const WCHAR *src )
{
	strcpyW( dst + strlenW(dst), src );
	return dst;
}


 WCHAR *strchrW( const WCHAR *str, WCHAR ch )
{
	do { if (*str == ch) return (WCHAR *)(ULONG_PTR)str; } while (*str++);
	return NULL;
}


 WCHAR *strrchrW( const WCHAR *str, WCHAR ch )
{
	WCHAR *ret = NULL;
	do { if (*str == ch) ret = (WCHAR *)(ULONG_PTR)str; } while (*str++);
	return ret;
}


 WCHAR *strpbrkW( const WCHAR *str, const WCHAR *accept )
{
	for ( ; *str; str++) if (strchrW( accept, *str )) return (WCHAR *)(ULONG_PTR)str;
	return NULL;
}


 size_t strspnW( const WCHAR *str, const WCHAR *accept )
{
	const WCHAR *ptr;
	for (ptr = str; *ptr; ptr++) if (!strchrW( accept, *ptr )) break;
	return ptr - str;
}


 size_t strcspnW( const WCHAR *str, const WCHAR *reject )
{
	const WCHAR *ptr;
	for (ptr = str; *ptr; ptr++) if (strchrW( reject, *ptr )) break;
	return ptr - str;
}


 WCHAR *strlwrW( WCHAR *str )
{
	WCHAR *ret = str;
	while ((*str = tolowerW(*str))) str++;
	return ret;
}


 WCHAR *struprW( WCHAR *str )
{
	WCHAR *ret = str;
	while ((*str = toupperW(*str))) str++;
	return ret;
}


 WCHAR *memchrW( const WCHAR *ptr, WCHAR ch, size_t n )
{
	const WCHAR *end;
	for (end = ptr + n; ptr < end; ptr++) if (*ptr == ch) return (WCHAR *)(ULONG_PTR)ptr;
	return NULL;
}


 WCHAR *memrchrW( const WCHAR *ptr, WCHAR ch, size_t n )
{
	const WCHAR *end;
	WCHAR *ret = NULL;
	for (end = ptr + n; ptr < end; ptr++) if (*ptr == ch) ret = (WCHAR *)(ULONG_PTR)ptr;
	return ret;
}

 long int atolW( const WCHAR *str )
{
	return strtolW( str, (WCHAR **)0, 10 );
}


 int atoiW( const WCHAR *str )
{
	return (int)atolW( str );
}

#endif
