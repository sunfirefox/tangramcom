/*
* Copyright (C) the TangramCOM project
*
* This file is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This file is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
*/

#ifndef OS_DATATYPES_HHH_123
#define OS_DATATYPES_HHH_123

#ifdef		WIN32
#undef		__Win32__
#define		__Win32__ 1
#endif

#include <limits.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



/* Platform-specific components */
#if __linux__ 

/* Includes */
#include <sys/types.h>
#include <memory.h>

/* Typedefs */
typedef unsigned char       UInt8;
typedef signed char         SInt8;
typedef unsigned short      UInt16;
typedef signed short        SInt16;
typedef unsigned int		UInt32;
typedef signed int			SInt32;
typedef signed long long    SInt64;
typedef unsigned long long  UInt64;

typedef UInt16              Bool16;
typedef UInt8               Bool8;

typedef unsigned short		WCHAR;
typedef int					BOOL;
typedef int					LONG;
typedef unsigned int		ULONG;
typedef	UInt8				BYTE;
typedef unsigned short		WORD;
typedef unsigned int		DWORD;

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef int					HRESULT;
#endif // !_HRESULT_DEFINED

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#elif __Win32__ || defined WIN32

#include "wtypes.h"

/* Typedefs */
typedef unsigned char       UInt8;
typedef signed char         SInt8;
typedef unsigned short      UInt16;
typedef signed short        SInt16;
typedef unsigned int		UInt32;
typedef signed int			SInt32;
typedef __int64				SInt64;
typedef unsigned __int64    UInt64;
typedef float               Float32;
typedef double              Float64;

typedef UInt16              Bool16;
typedef UInt8               Bool8;



#else

#error you must define os enviroment:WIN32 __Win32__=1 or __linux__=1

#endif


/////////////////////////////////////////////////////////////////

#ifndef OS_DIM //get dimension of array

#ifdef __cplusplus

template <int N>
struct array_size_struct
{char c[N];};

template <class T,int N> 
array_size_struct<N> static_array_size_fn(T (&)[N]);

#define OS_DIM(x)  sizeof(static_array_size_fn(x).c)

#else

#define OS_DIM(array_) (sizeof(array_)/sizeof(*(array_)))

#endif

#endif //OS_DIM

//////////////////////////////////////////////////////////////////
#ifndef UNUSED
#define UNUSED(x) ( void )(x)
#endif

#include "OSErrors.h"


#endif


