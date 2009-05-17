/*
* GetTickCount
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
#if __linux__
#include "config.h"
#include "wine/port.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>

#include "windef.h"
#include "winbase.h"
#include "tgmlog.h"

TGMLOG_DEFAULT_DEBUG_CHANNEL(process);

#include <unistd.h>
#include <sys/times.h>

DWORD WINE_WINAPI GetTickCount(void)
{
	//基准：ticks/秒
	struct tms buffer;
	static unsigned long	clkPerSec = 0;
	static unsigned long	clk_init = 0;
	if(clkPerSec == 0 && clk_init == 0)
	{
		clkPerSec = (ULONG)sysconf(_SC_CLK_TCK);
		//p rintf("clkPerSec:%lu\r\n",clkPerSec);
		clk_init	= (unsigned long)(ULONG)times(&buffer);
	}
	//clock_t clk = ::times(&buffer);
	unsigned long clk_curr = (unsigned long)(ULONG)times(&buffer);
	unsigned long clk_diff = (unsigned long)((clk_curr + 0XFFFFFFFFUL - clk_init)%0XFFFFFFFFUL);
	return (ULONG)(clk_diff * 1000 / clkPerSec);   //1ms单位  //clock_t在ARM上是long类型
}

#endif //__linux__

