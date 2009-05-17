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
*	History:
*		2009/04/01	Jokul for Tranzda
*			Add TGMCOMUTI_API declare,use tgmlog
*/


#ifndef _MYASSERT_H_
#define _MYASSERT_H_

#include <stdio.h>
#include "OSBaseTypes.h"
#include "SafeStdLib.h"


#include "tgmlog.h"



#ifdef __cplusplus
class TGMCOMUTI_API AssertLogger
{
    public:
        // An interface so the MyAssert function can write a message
        virtual void LogAssert(char* inMessage) = 0;
	private:
		virtual ~AssertLogger(){}//以免LINUX下编译时警告
};

// If a logger is provided, asserts will be logged. Otherwise, asserts will cause a bus error
TGMCOMUTI_API void SetAssertLogger(AssertLogger* theLogger);
#endif

#if ASSERT  
    TGMCOMUTI_API void  MyAssert(char *s);

    #define kAssertBuffSize 256
    
    #define Assert(condition)    {                              \
                                                                \
        if (!(condition))                                       \
        {                                                       \
            char s[kAssertBuffSize];                            \
            s[kAssertBuffSize -1] = 0;                          \
				qtss_snprintf (s,kAssertBuffSize -1, "_Assert: %s, %d,%s\r\n",__FILE__, __LINE__, #condition); \
            MyAssert(s);                                        \
        }   }


    #define AssertV(condition,errNo)    {                                   \
        if (!(condition))                                                   \
        {                                                                   \
            char s[kAssertBuffSize];                                        \
            s[kAssertBuffSize -1] = 0;                                      \
			qtss_snprintf( s,kAssertBuffSize -1, "_AssertV: %s, %d (%d),%s\r\n",__FILE__, __LINE__, errNo,#condition );    \
            MyAssert(s);                                                    \
        }   }
                                     
                                         
    #define Warn(condition) {                                       \
            if (!(condition))                                       \
			TRACE("_Warn: %s, %d, %s\n",__FILE__, __LINE__,#condition );     }                                                           
                                         
    #define WarnV(condition,msg)        {                               \
        if (!(condition))                                               \
            TRACE("_WarnV: %s, %d, %s (%s)\n",__FILE__, __LINE__, #condition, msg );  }                                                   
                                         
    #define WarnVE(condition,msg,err)  {                           		\
        if (!(condition))                                               \
        {   char buffer[kAssertBuffSize];								\
            buffer[kAssertBuffSize -1] = 0;                              \
            TRACE("_WarnV: %s, %d, %s (%s, %s [err=%d])\n",__FILE__, __LINE__,#condition, msg, qtss_strerror(err,buffer,sizeof(buffer) -1), err );  \
        }	}

#else
#error asdfas    
    #define Assert(condition) ((void) 0)
    #define AssertV(condition,errNo) ((void) 0)
    #define Warn(condition) ((void) 0)
    #define WarnV(condition,msg) ((void) 0)

#endif
#endif //_MY_ASSERT_H_
