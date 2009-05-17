/*
 * Kernel32 undocumented and private functions definition
 *
 * Copyright 2003 Eric Pouech
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

#ifndef __WINE_KERNEL_PRIVATE_H
#define __WINE_KERNEL_PRIVATE_H

#include "wine/server.h"

struct tagSYSLEVEL;

struct kernel_thread_data
{
    UINT                code_page;      /* thread code page */
    WORD                stack_sel;      /* 16-bit stack selector */
    WORD                htask16;        /* Win16 task handle */
    DWORD               sys_count[4];   /* syslevel mutex entry counters */
    struct tagSYSLEVEL *sys_mutex[4];   /* syslevel mutex pointers */
    void               *pad[44];        /* change this if you add fields! */
};

static inline struct kernel_thread_data *kernel_get_thread_data(void)
{
    return (struct kernel_thread_data *)NtCurrentTeb()->SystemReserved1;
}

HANDLE  WINE_WINAPI OpenConsoleW(LPCWSTR, DWORD, BOOL, DWORD);
BOOL    WINE_WINAPI VerifyConsoleIoHandle(HANDLE);
HANDLE  WINE_WINAPI DuplicateConsoleHandle(HANDLE, DWORD, BOOL, DWORD);
BOOL    WINE_WINAPI CloseConsoleHandle(HANDLE handle);
HANDLE  WINE_WINAPI GetConsoleInputWaitHandle(void);

static inline BOOL is_console_handle(HANDLE h)
{
    return h != INVALID_HANDLE_VALUE && ((UINT_PTR)h & 3) == 3;
}

/* map a real wineserver handle onto a kernel32 console handle */
static inline HANDLE console_handle_map(HANDLE h)
{
    return h != INVALID_HANDLE_VALUE ? (HANDLE)((UINT_PTR)h ^ 3) : INVALID_HANDLE_VALUE;
}

///* map a kernel32 console handle onto a real wineserver handle */
//static inline obj_handle_t console_handle_unmap(HANDLE h)
//{
//    return wine_server_obj_handle( h != INVALID_HANDLE_VALUE ? (HANDLE)((UINT_PTR)h ^ 3) : INVALID_HANDLE_VALUE );
//}

extern HMODULE kernel32_handle;

/* Size of per-process table of DOS handles */
#define DOS_TABLE_SIZE 256
extern HANDLE dos_handles[DOS_TABLE_SIZE];

extern const WCHAR *DIR_Windows;
extern const WCHAR *DIR_System;

extern VOID SYSLEVEL_CheckNotLevel( INT level );

extern void FILE_SetDosError(void);
extern WCHAR *FILE_name_AtoW( LPCSTR name, BOOL alloc );
extern DWORD FILE_name_WtoA( LPCWSTR src, INT srclen, LPSTR dest, INT destlen );

extern DWORD __wine_emulate_instruction( EXCEPTION_RECORD *rec, CONTEXT86 *context );
extern LONG CALLBACK INSTR_vectored_handler( EXCEPTION_POINTERS *ptrs );

/* return values for MODULE_GetBinaryType */
enum binary_type
{
    BINARY_UNKNOWN,
    BINARY_PE_EXE,
    BINARY_PE_DLL,
    BINARY_WIN16,
    BINARY_OS216,
    BINARY_DOS,
    BINARY_UNIX_EXE,
    BINARY_UNIX_LIB
};

/* module.c */
extern WCHAR *MODULE_get_dll_load_path( LPCWSTR module );
extern enum binary_type MODULE_GetBinaryType( HANDLE hfile, void **res_start, void **res_end );

extern BOOL NLS_IsUnicodeOnlyLcid(LCID);

extern HANDLE VXD_Open( LPCWSTR filename, DWORD access, LPSECURITY_ATTRIBUTES sa );

extern WORD DOSMEM_0000H;
extern WORD DOSMEM_BiosDataSeg;
extern WORD DOSMEM_BiosSysSeg;

/* dosmem.c */
extern BOOL   DOSMEM_Init(void);
extern LPVOID DOSMEM_MapRealToLinear(DWORD); /* real-mode to linear */
extern LPVOID DOSMEM_MapDosToLinear(UINT);   /* linear DOS to Wine */
extern UINT   DOSMEM_MapLinearToDos(LPVOID); /* linear Wine to DOS */
extern BOOL   load_winedos(void);

/* environ.c */
extern void ENV_CopyStartupInformation(void);

/* computername.c */
extern void COMPUTERNAME_Init(void);

/* locale.c */
extern void LOCALE_Init(void);
extern void LOCALE_InitRegistry(void);

/* oldconfig.c */
extern void convert_old_config(void);

extern struct winedos_exports
{
    /* for global16.c */
    void*    (*AllocDosBlock)(UINT size, UINT16* pseg);
    BOOL     (*FreeDosBlock)(void* ptr);
    UINT     (*ResizeDosBlock)(void *ptr, UINT size, BOOL exact);
    /* for instr.c */
    BOOL (WINE_WINAPI *EmulateInterruptPM)( CONTEXT86 *context, BYTE intnum );
    void (WINE_WINAPI *CallBuiltinHandler)( CONTEXT86 *context, BYTE intnum );
    DWORD (WINE_WINAPI *inport)( int port, int size );
    void (WINE_WINAPI *outport)( int port, int size, DWORD val );
    void (* BiosTick)(WORD timer);
} winedos;

/* returns directory handle for named objects */
extern HANDLE get_BaseNamedObjects_handle(void);

/* Register functions */

#ifdef __i386__
#define DEFINE_REGS_ENTRYPOINT( name, args, pop_args ) \
    __ASM_GLOBAL_FUNC( name, \
                       "pushl %eax\n\t" \
                       "call " __ASM_NAME("__wine_call_from_32_regs") "\n\t" \
                       ".long " __ASM_NAME("__regs_") #name "-.\n\t" \
                       ".byte " #args "," #pop_args )
#endif

#endif
