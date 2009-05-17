/*
* sync operate function
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
#include "OSAtomic.h"

/***********************************************************************
*		InterlockedCompareExchange (KERNEL32.@)
*
* Atomically swap one value with another.
*
* PARAMS
*  dest    [I/O] The value to replace
*  xchq    [I]   The value to be swapped
*  compare [I]   The value to compare to dest
*
* RETURNS
*  The resulting value of dest.
*
* NOTES
*  dest is updated only if it is equal to compare, otherwise no swap is done.
*/
LONG WINE_WINAPI InterlockedCompareExchange( LONG volatile *dest, LONG xchg, LONG compare )
{
	 os_compare_and_store( compare, xchg, dest );
	 return *dest;
}

/***********************************************************************
*		InterlockedExchange (KERNEL32.@)
*
* Atomically swap one value with another.
*
* PARAMS
*  dest [I/O] The value to replace
*  val  [I]   The value to be swapped
*
* RETURNS
*  The resulting value of dest.
* The function returns the initial value pointed to by Target.
*/
LONG WINE_WINAPI InterlockedExchange( LONG volatile *dest, LONG val )//
{
	int ov = *dest;
	os_compare_and_store(*dest,val,dest);
	return ov;
}

/***********************************************************************
*		InterlockedExchangeAdd (KERNEL32.@)
*
* Atomically add one value to another.
*
* PARAMS
*  dest [I/O] The value to add to
*  incr [I]   The value to be added
*
* RETURNS
*  The resulting value of dest.
*/
LONG WINE_WINAPI InterlockedExchangeAdd( LONG volatile *dest, LONG incr )
{
#if 0
	LONG old = *dest;
	LONG ret =  os_atomic_add(dest,incr) - incr;
	printf("InterlockedExchangeAdd dest:%p,old:%d,new:%d,incr:%d,ret:%d\r\n",dest,old,*dest,incr,ret);
	return ret;
#else
	return os_atomic_add(dest,incr) - incr;
#endif
}

/***********************************************************************
*		InterlockedIncrement (KERNEL32.@)
*
* Atomically increment a value.
*
* PARAMS
*  dest [I/O] The value to increment
*
* RETURNS
*  The resulting value of dest.
*/
LONG WINE_WINAPI InterlockedIncrement( LONG volatile *dest )
{
	return os_atomic_add(dest,1);
}

/***********************************************************************
*		InterlockedDecrement (KERNEL32.@)
*
* Atomically decrement a value.
*
* PARAMS
*  dest [I/O] The value to decrement
*
* RETURNS
*  The resulting value of dest.
*/
LONG WINE_WINAPI InterlockedDecrement( LONG volatile *dest )
{
	return os_atomic_sub(dest,1);
}


#endif
