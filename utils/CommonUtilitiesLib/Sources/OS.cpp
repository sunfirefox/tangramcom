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
    File:       OS.cpp

    Contains:   OS utility functions


        
    
*/

#include <stdlib.h>
#include "SafeStdLib.h"
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifndef __Win32__
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

#ifdef __sgi__ 
#include <unistd.h>
#endif

#include "OSHeaders.h"
#include "OS.h"
#include "OSThread.h"
#include "MyAssert.h"
#include "OSFileSource.h"

#if __MacOSX__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
//extern "C" { void Microseconds (UnsignedWide *microTickCount); }
#endif

#endif


#if (__FreeBSD__ ||  __MacOSX__)
    #include <sys/sysctl.h>
#endif

#if (__solaris__ || __linux__ || __linuxppc__)
    #include "StringParser.h"
#endif

#if __sgi__
	#include <sys/systeminfo.h>
#endif


double  OS::sDivisor = 0;
double  OS::sMicroDivisor = 0;
SInt64  OS::sMsecSince1970 = 0;
SInt64  OS::sMsecSince1900 = 0;
SInt64  OS::sInitialMsec = 0;
SInt64  OS::sWrapTime = 0;
SInt64  OS::sCompareWrap = 0;
SInt64  OS::sLastTimeMilli = 0;
OSMutex OS::sStdLibOSMutex;


/*!   
 *  ���ζ�DEBUG�Ĵ�����txf 20070118��ӣ�
 *  ��Ϊ,ԭ���Ĵ���Initialize�е�����NEW��������Զû��delete
 */
#if 0

	#if DEBUG || __Win32__
	#include "OSMutex.h"
	#include "OSMemory.h"
	//static OSMutex* sLastMillisMutex = NULL;
	#endif

#else

	#if DEBUG || __Win32__
	#include "OSMutex.h"
	#include "OSMemory.h"
	//static OSMutex* get_sLastMillisMutex()
	//{
	//	static OSMutex _smutex;
	//	return &_smutex;
	//}
	//#define sLastMillisMutex get_sLastMillisMutex()
	#endif

#endif

void OS::Initialize()
{	
	if (sInitialMsec != 0) return;
	OSGlobalCriticalSection _ogcs;
    
	//Assert (sInitialMsec == 0);  // do only once
    if (sInitialMsec != 0) return;

    //setup t0 value for msec since 1900

    //t.tv_sec is number of seconds since Jan 1, 1970. Convert to seconds since 1900    
    SInt64 the1900Sec = (SInt64) (24 * 60 * 60) * (SInt64) ((70 * 365) + 17) ;
    sMsecSince1900 = the1900Sec * 1000;
    
    sWrapTime = (SInt64) 0x00000001 << 32;
    sCompareWrap = (SInt64) 0xffffffff << 32;
    sLastTimeMilli = 0;
    
    sInitialMsec = OS::Milliseconds(); //Milliseconds uses sInitialMsec so this assignment is valid only once.

    sMsecSince1970 = ::time(NULL);  // POSIX time always returns seconds since 1970
    sMsecSince1970 *= 1000;         // Convert to msec
    

//!����ע�ʹ�����txf 20070118���
//#if DEBUG || __Win32__ 
//    sLastMillisMutex = NEW OSMutex();
//#endif
}

ULONG	OS::GetTickCount(void)
{
#if __Win32__
	return ::GetTickCount();
#else
	//��׼��ticks/��
	struct tms buffer;
	static ULONG	clkPerSec   = (ULONG)::sysconf(_SC_CLK_TCK);
	static UInt64	clk_init    = (UInt64)(ULONG)::times(&buffer);
	//if(clkPerSec == 0 && clk_init == 0)
	//{
	//	clkPerSec = (ULONG)::sysconf(_SC_CLK_TCK);
	//	//p rintf("clkPerSec:%lu\r\n",clkPerSec);
	//	clk_init	= (UInt64)(ULONG)::times(&buffer);
	//}
	//clock_t clk = ::times(&buffer);
    Assert(clkPerSec >0);
	UInt64 clk_curr = (UInt64)(ULONG)::times(&buffer);
	UInt64 clk_diff = (UInt64)((clk_curr + 0XFFFFFFFFUL - clk_init)%0XFFFFFFFFUL);
	return (ULONG)(clk_diff * 1000 / clkPerSec);   //1ms��λ  //clock_t��ARM����long����
#endif
}

SInt64 OS::Milliseconds()
{
/*
#if __MacOSX__

#if DEBUG
    OSMutexLocker locker(sLastMillisMutex);
#endif

   UnsignedWide theMicros;
    ::Microseconds(&theMicros);
    SInt64 scalarMicros = theMicros.hi;
    scalarMicros <<= 32;
    scalarMicros += theMicros.lo;
    scalarMicros = ((scalarMicros / 1000) - sInitialMsec) + sMsecSince1970; // convert to msec

#if DEBUG
    static SInt64 sLastMillis = 0;
    //Assert(scalarMicros >= sLastMillis); // currently this fails on dual processor machines
    sLastMillis = scalarMicros;
#endif
    return scalarMicros;
*/
#if __Win32__
#if DEBUG
   // OSMutexLocker locker(sLastMillisMutex);
	OSGlobalCriticalSection _ogcs;
#endif
    // curTimeMilli = timeGetTime() + ((sLastTimeMilli/ 2^32) * 2^32)
    // using binary & to reduce it to one operation from two
    // sCompareWrap and sWrapTime are constants that are never changed
    // sLastTimeMilli is updated with the curTimeMilli after each call to this function
    SInt64 curTimeMilli = (UInt32) ::timeGetTime() + (sLastTimeMilli & sCompareWrap);
    if((curTimeMilli - sLastTimeMilli) < 0)
    {
        curTimeMilli += sWrapTime;
    }
    sLastTimeMilli = curTimeMilli;
    
    // For debugging purposes
    //SInt64 tempCurMsec = (curTimeMilli - sInitialMsec) + sMsecSince1970;
    //SInt32 tempCurSec = tempCurMsec / 1000;
    //char buffer[kTimeStrSize];
    //qtss_printf("OS::MilliSeconds current time = %s\n", qtss_ctime(&tempCurSec, buffer, sizeof(buffer)));

    return (curTimeMilli - sInitialMsec) + sMsecSince1970; // convert to application time
#else
    struct timeval t;
    struct timezone tz;
    int theErr = ::gettimeofday(&t, &tz);
    Assert(theErr == 0);

    SInt64 curTime;
    curTime = t.tv_sec;
    curTime *= 1000;                // sec -> msec
    curTime += t.tv_usec / 1000;    // usec -> msec

    return (curTime - sInitialMsec) + sMsecSince1970;
#endif

}

SInt64 OS::Microseconds()
{
/*
#if __MacOSX__
    UnsignedWide theMicros;
    ::Microseconds(&theMicros);
    SInt64 theMillis = theMicros.hi;
    theMillis <<= 32;
    theMillis += theMicros.lo;
    return theMillis;
*/
#if __Win32__
    SInt64 curTime = (SInt64) ::timeGetTime(); // unsigned long system time in milliseconds
    curTime -= sInitialMsec; // convert to application time
    curTime *= 1000; // convert to microseconds                   
    return curTime;
#else
    struct timeval t;
    struct timezone tz;
    int theErr = ::gettimeofday(&t, &tz);
    Assert(theErr == 0);

    SInt64 curTime;
    curTime = t.tv_sec;
    curTime *= 1000000;     // sec -> usec
    curTime += t.tv_usec;

    return curTime - (sInitialMsec * 1000);
#endif
}

SInt32 OS::GetGMTOffset()
{
#ifdef __Win32__
    TIME_ZONE_INFORMATION tzInfo;
    DWORD theErr = ::GetTimeZoneInformation(&tzInfo);
    if (theErr == TIME_ZONE_ID_INVALID)
        return 0;
    
    return ((tzInfo.Bias / 60) * -1);
#else
    struct timeval  tv;
    struct timezone tz;

    int err = ::gettimeofday(&tv, &tz);
    if (err != 0)
        return 0;
        
    return ((tz.tz_minuteswest / 60) * -1);//return hours before or after GMT
#endif
}


SInt64  OS::HostToNetworkSInt64(SInt64 hostOrdered)
{
#if BIGENDIAN
    return hostOrdered;
#else
    return (SInt64) (  (UInt64)  (hostOrdered << 56) | (UInt64)  (((UInt64) 0x00ff0000 << 32) & (hostOrdered << 40))
        | (UInt64)  ( ((UInt64)  0x0000ff00 << 32) & (hostOrdered << 24)) | (UInt64)  (((UInt64)  0x000000ff << 32) & (hostOrdered << 8))
        | (UInt64)  ( ((UInt64)  0x00ff0000 << 8) & (hostOrdered >> 8)) | (UInt64)     ((UInt64)  0x00ff0000 & (hostOrdered >> 24))
        | (UInt64)  (  (UInt64)  0x0000ff00 & (hostOrdered >> 40)) | (UInt64)  ((UInt64)  0x00ff & (hostOrdered >> 56)) );
#endif
}

SInt64  OS::NetworkToHostSInt64(SInt64 networkOrdered)
{
#if BIGENDIAN
    return networkOrdered;
#else
    return (SInt64) (  (UInt64)  (networkOrdered << 56) | (UInt64)  (((UInt64) 0x00ff0000 << 32) & (networkOrdered << 40))
        | (UInt64)  ( ((UInt64)  0x0000ff00 << 32) & (networkOrdered << 24)) | (UInt64)  (((UInt64)  0x000000ff << 32) & (networkOrdered << 8))
        | (UInt64)  ( ((UInt64)  0x00ff0000 << 8) & (networkOrdered >> 8)) | (UInt64)     ((UInt64)  0x00ff0000 & (networkOrdered >> 24))
        | (UInt64)  (  (UInt64)  0x0000ff00 & (networkOrdered >> 40)) | (UInt64)  ((UInt64)  0x00ff & (networkOrdered >> 56)) );
#endif
}


OS_Error OS::MakeDir(char *inPath)
{
    struct stat theStatBuffer;
    if (::stat(inPath, &theStatBuffer) == -1)
    {
        //this directory doesn't exist, so let's try to create it
#ifdef __Win32__
        if (::mkdir(inPath) == -1)
#else
        if (::mkdir(inPath, S_IRWXU) == -1)
#endif
            return (OS_Error)OSThread::GetErrno();
    }
#ifdef __Win32__
    else if (!(theStatBuffer.st_mode & _S_IFDIR)) // MSVC++ doesn't define the S_ISDIR macro
        return EEXIST; // there is a file at this point in the path!
#else
    else if (!S_ISDIR(theStatBuffer.st_mode))
        return EEXIST;//there is a file at this point in the path!
#endif

    //directory exists
    return OS_NoErr;
}

OS_Error OS::RecursiveMakeDir(char *inPath)
{
    Assert(inPath != NULL);
    
    //iterate through the path, replacing '/' with '\0' as we go
    char *thePathTraverser = inPath;
    
    //skip over the first / in the path.
    if (*thePathTraverser == kPathDelimiterChar)
        thePathTraverser++;
        
    while (*thePathTraverser != '\0')
    {
        if (*thePathTraverser == kPathDelimiterChar)
        {
            //we've found a filename divider. Now that we have a complete
            //filename, see if this partial path exists.
            
            //make the partial path into a C string
            *thePathTraverser = '\0';
            OS_Error theErr = MakeDir(inPath);
            //there is a directory here. Just continue in our traversal
            *thePathTraverser = kPathDelimiterChar;

            if (theErr != OS_NoErr)
                return theErr;
        }
        thePathTraverser++;
    }
    
    //need to create the last directory in the path
    return MakeDir(inPath);
}


UInt32  OS::GetNumProcessors()
{
#if (__Win32__)
    SYSTEM_INFO theSystemInfo;
    ::GetSystemInfo(&theSystemInfo);
    
    return (UInt32)theSystemInfo.dwNumberOfProcessors;
#endif

#if (__MacOSX__ || __FreeBSD__)
    int numCPUs = 1;
    size_t len = sizeof(numCPUs);
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    (void) ::sysctl(mib,2,&numCPUs,&len,NULL,0);
    if (numCPUs < 1) 
        numCPUs = 1;
    return (UInt32) numCPUs;
#endif

#if(__linux__ || __linuxppc__)
    
    char cpuBuffer[8192] = "";
    StrPtrLen cpuInfoBuf(cpuBuffer, sizeof(cpuBuffer));
    FILE    *cpuFile = ::fopen( "/proc/cpuinfo", "r" );
    if (cpuFile)
    {   cpuInfoBuf.Len = ::fread(cpuInfoBuf.Ptr, sizeof(char),  cpuInfoBuf.Len, cpuFile);
        ::fclose(cpuFile);
    }
    
    StringParser cpuInfoFileParser(&cpuInfoBuf);
    StrPtrLen line;
    StrPtrLen word;
    UInt32 numCPUs = 0;
    
    while( cpuInfoFileParser.GetDataRemaining() != 0 ) 
    {
        cpuInfoFileParser.GetThruEOL(&line);    // Read each line   
        StringParser lineParser(&line);
        lineParser.ConsumeWhitespace();         //skip over leading whitespace

        if (lineParser.GetDataRemaining() == 0) // must be an empty line
            continue;

        lineParser.ConsumeUntilWhitespace(&word);
               
        if ( word.Equal("processor") ) // found a processor as first word in line
        {   numCPUs ++; 
        }
    }
    
    if (numCPUs == 0)
        numCPUs = 1;
        
    return numCPUs;
#endif

#if(__solaris__)
{
    UInt32 numCPUs = 0;
    char linebuff[512] = "";
    StrPtrLen line(linebuff, sizeof(linebuff));
    StrPtrLen word;

    FILE *p = ::popen("uname -X","r");
    while((::fgets(linebuff, sizeof(linebuff -1), p)) > 0)
    {
        StringParser lineParser(&line);
        lineParser.ConsumeWhitespace(); //skip over leading whitespace

        if (lineParser.GetDataRemaining() == 0) // must be an empty line
            continue;

        lineParser.ConsumeUntilWhitespace(&word);

        if ( word.Equal("NumCPU")) // found a tag as first word in line
        {
            lineParser.GetThru(NULL,'=');
            lineParser.ConsumeWhitespace();  //skip over leading whitespace
            lineParser.ConsumeUntilWhitespace(&word); //read the number of cpus
            if (word.Len > 0)
                ::sscanf(word.Ptr, "%lu", &numCPUs);

            break;
        }
    }
    if (numCPUs == 0)
        numCPUs = 1;
        
    ::pclose(p);
    
	return numCPUs;
}
#endif

#if(__sgi__) 
    UInt32 numCPUs = 0;

    numCPUs = sysconf(_SC_NPROC_ONLN);
	
	return numCPUs;
#endif		


    return 1;
}

SInt64	OS::TimeMilli_To_1900Fixed64Secs(SInt64 inMilliseconds)
{ return TimeMilli_To_Fixed64Secs(sMsecSince1900) + TimeMilli_To_Fixed64Secs(inMilliseconds); }

time_t   OS::Time1900Fixed64Secs_To_UnixTimeSecs(SInt64 in1900Fixed64Secs)
{ return (time_t)( (SInt64)  ((SInt64)  ( in1900Fixed64Secs - TimeMilli_To_Fixed64Secs(sMsecSince1900) ) /  ((SInt64) 1 << 32)  ) ); }


SInt64   OS::Time1900Fixed64Secs_To_TimeMilli(SInt64 in1900Fixed64Secs)
{ return   ( (SInt64) ( (Float64) ((SInt64) in1900Fixed64Secs - (SInt64) TimeMilli_To_Fixed64Secs(sMsecSince1900) ) / (Float64)  ((SInt64) 1 << 32) ) * 1000) ; }

SInt64   OS::InitialMSec()       { return sInitialMsec; }

OSMutex* OS::GetStdLibMutex()  { return &sStdLibOSMutex; }
