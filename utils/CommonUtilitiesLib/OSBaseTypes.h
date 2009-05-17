/*
*
* @APPLE_LICENSE_HEADER_START@
* 
* Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
* 
* This file contains Original Code and/or Modifications of Original Code
* as defined in and that are subject to the Apple Public Source License
* Version 2.0 (the 'License'). You may not use this file except in
* compliance with the License. Please obtain a copy of the License at
* http://www.opensource.apple.com/apsl/ and read it before using this
* file.
* 
* The Original Code and all software distributed under the License are
* distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
* EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
* Please see the License for the specific language governing rights and
* limitations under the License.
* 
* @APPLE_LICENSE_HEADER_END@
*
*/
/*
File:       OSBaseTypes.h

Contains:   common data type defines.


*/

#ifndef OS_BASE_TYPES_H
#define OS_BASE_TYPES_H
#include "OSConfigure.h"
#include <limits.h>

#define kSInt16_Max USHRT_MAX
#define kUInt16_Max USHRT_MAX

#define kSInt32_Max LONG_MAX
#define kUInt32_Max ULONG_MAX

#define kSInt64_Max LONG_LONG_MAX
#define kUInt64_Max ULONG_LONG_MAX


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



/* Platform-specific components */
#if __linux__ || __linuxppc__ || __FreeBSD__ || __MacOSX__
/* Defines */
#define _64BITARG_ "q"

/* paths */
#define kEOLString "\n"
#define kPathDelimiterString "/"
#define kPathDelimiterChar '/'
#define kPartialPathBeginsWithDelimiter 0

/* Includes */
#include <sys/types.h>
#include <memory.h>
#include "guiddef.h" //must include LinuxSupport Dir

/* Constants */
#define QT_TIME_TO_LOCAL_TIME   (-2082844800)
#define QT_PATH_SEPARATOR       '/'

/* Typedefs */
typedef unsigned int        PointerSizedInt;
typedef unsigned char       UInt8;
typedef signed char         SInt8;
typedef unsigned short      UInt16;
typedef signed short        SInt16;
typedef unsigned int		UInt32;
typedef signed int			SInt32;
typedef signed long long    SInt64;
typedef unsigned long long  UInt64;
typedef float               Float32;
typedef double              Float64;
typedef UInt16              Bool16;
typedef UInt8               Bool8;



typedef unsigned long       FourCharCode;
typedef FourCharCode        OSType;

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

#ifdef  FOUR_CHARS_TO_INT
#error Conflicting Macro "FOUR_CHARS_TO_INT"
#endif

#define FOUR_CHARS_TO_INT( c1, c2, c3, c4 )  ( c1 << 24 | c2 << 16 | c3 << 8 | c4 )

#ifdef  TW0_CHARS_TO_INT
#error Conflicting Macro "TW0_CHARS_TO_INT"
#endif

#define TW0_CHARS_TO_INT( c1, c2 )  ( c1 << 8 | c2 )

#ifndef interface
#define interface struct
#endif


#elif __Win32__ || defined WIN32

#include "wtypes.h"

/* Defines */
#define _64BITARG_ "I64"

/* paths */
#define kEOLString "\r\n"
#define kPathDelimiterString "\\"
#define kPathDelimiterChar '\\'
#define kPartialPathBeginsWithDelimiter 0

#define crypt(buf, salt) ((char*)buf)





#define R_OK 0
#define W_OK 1

// POSIX errorcodes
#define ENOTCONN 1002
#define EADDRINUSE 1004
#define EINPROGRESS 1007
#define ENOBUFS 1008
#define EADDRNOTAVAIL 1009


/* Constants */
#define QT_TIME_TO_LOCAL_TIME   (-2082844800)
#define QT_PATH_SEPARATOR       '/'

/* Typedefs */
typedef unsigned int        PointerSizedInt;
typedef unsigned char       UInt8;
typedef signed char         SInt8;
typedef unsigned short      UInt16;
typedef signed short        SInt16;
typedef unsigned int		UInt32;
typedef signed int			SInt32;
//typedef LONGLONG            SInt64;
//typedef ULONGLONG           UInt64;
typedef __int64				SInt64;
typedef unsigned __int64    UInt64;
typedef float               Float32;
typedef double              Float64;
typedef UInt16              Bool16;
typedef UInt8               Bool8;

typedef unsigned long       FourCharCode;
typedef FourCharCode        OSType;

#ifdef  FOUR_CHARS_TO_INT
#error Conflicting Macro "FOUR_CHARS_TO_INT"
#endif

#define FOUR_CHARS_TO_INT( c1, c2, c3, c4 )  ( c1 << 24 | c2 << 16 | c3 << 8 | c4 )

#ifdef  TW0_CHARS_TO_INT
#error Conflicting Macro "TW0_CHARS_TO_INT"
#endif

#define TW0_CHARS_TO_INT( c1, c2 )  ( c1 << 8 | c2 )

#define kSInt16_Max USHRT_MAX
#define kUInt16_Max USHRT_MAX

#define kSInt32_Max LONG_MAX
#define kUInt32_Max ULONG_MAX

#undef kSInt64_Max
#define kSInt64_Max  9223372036854775807i64

#undef kUInt64_Max
#define kUInt64_Max  (kSInt64_Max * 2ULL + 1)

#else

#error you must define os enviroment:WIN32 __Win32__=1 or __linux__=1

#endif

typedef SInt32 OS_Error;

enum
{
	OS_NoErr = (OS_Error) 0,
	OS_BadURLFormat = (OS_Error) -100,
	OS_NotEnoughSpace = (OS_Error) -101
};

/////////////////////////////////////////////////////////////////
//提供OS_DIM定义
#ifndef OS_DIM

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
