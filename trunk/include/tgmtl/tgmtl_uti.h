/*
* tangram template library
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

#ifndef tgmtl_uti_h_345
#define tgmtl_uti_h_345

#ifndef TANGRAM_TEMPLATE_COMMON_HEADER
#error this file must include in tgmtl.h
#endif

#include "OSBaseTypes.h"
#include "OSErrors.h"

#if defined(__MSC_VER) || defined(WIN32)
# define TGMTL_DECLSPEC_HIDDEN
# define TGMTL_DLLENTRY_API STDAPI
#elif defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
# define TGMTL_DECLSPEC_HIDDEN __attribute__((visibility ("hidden")))
# define TGMTL_DLLENTRY_API EXTERN_C __attribute__((visibility("default"))) HRESULT STDAPICALLTYPE
#else
# define TGMTL_DECLSPEC_HIDDEN
# define TGMTL_DLLENTRY_API
#endif

#define YACL_MAX_PATH				260

#ifdef __cplusplus
extern "C" {
#endif

void ModuleAddRef(void)		TGMTL_DECLSPEC_HIDDEN;
void ModuleRelease(void)	TGMTL_DECLSPEC_HIDDEN;
BOOL ModuleIsStopping(void) TGMTL_DECLSPEC_HIDDEN;
BOOL ModuleIsIdle(void)		TGMTL_DECLSPEC_HIDDEN;
void ModuleServerListen(void) TGMTL_DECLSPEC_HIDDEN;

#ifdef __cplusplus
};
#endif

#endif//#ifndef tgmtl_uti_h_345
