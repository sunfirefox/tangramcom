
/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2005 Tranzda Technologies Co.,Ltd.                       */
/*                    ������ȫ�Ǵ�Ƽ����޹�˾  ��Ȩ���� 2005             */
/*                                                                        */
/*  PROPRIETARY RIGHTS of Tranzda Technologies Co.,Ltd. are involved in   */
/*  the subject matter of this material. All manufacturing, reproduction, */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  ������ĵ�������������ȫ�Ǵ�Ƽ����޹�˾�ĺϷ��ʲ����κ���ʿ�Ķ���ʹ  */
/*  �ñ����ϱ�������Ӧ��������Ȩ,�е��������κͽ�����Ӧ�ķ���Լ����     */
/*                                                                        */
/**************************************************************************/

#ifndef __SS_TYPES_H__
#define __SS_TYPES_H__

#define MAX_PATH        260

#ifndef NULL
    #ifdef __cplusplus
        #define NULL            0
    #else
        #define NULL            ((void *)0)
    #endif
#endif

#ifndef FALSE
    #define FALSE           0
#endif

#ifndef TRUE
    #define TRUE            1
#endif

#if !(defined(INT8_DEFINE) || defined(INT8))
#define INT8_DEFINE
    typedef signed char         INT8;
    typedef unsigned char       UINT8;
    typedef signed short        INT16;
    typedef unsigned short      UINT16;
    typedef signed int          INT32;
    typedef unsigned int        UINT32;
    typedef long long           INT64;
    typedef unsigned long long  UINT64;
#endif

#if !(defined(DWORD_DEFINE) || defined(DWORD))
#define DWORD_DEFINE
	typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef float               FLOAT;
	typedef signed int          INT;
	typedef unsigned int        UINT;
	typedef unsigned int        *PUINT;
#endif

#ifndef MAX
    #define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
    #define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS
    #define ABS(X) (((X) >= 0) ? (X) : -(X))
#endif

#ifndef OffsetOf
    #define OffsetOf(type, member)	((Int32) &((((type *) 0)->member)))
#endif

    //��ȡ�ṹ��Ա�ڽṹ�ڵ�ƫ��
#ifndef GET_MEMBER_OFFSET
    #define GET_MEMBER_OFFSET(Type, Member) ((Int32)(&((Type *)0)->Member))
#endif

	//�ж���������ռ�ÿռ��Ƿ�ȳ������������������ʹ�
	//���ռ�ÿռ�Ҫ��ָ���Ĵ󣬻���ֱ�����󣬴��������ǣ����±�
	//�÷����Ƚ�����ռ�ÿռ��Ƿ�ȳ�����CHECK_TYPE_SIZE_TOO_BIG(Your_type, 100);
	//      �Ƚ�����ռ�ÿռ��Ƿ�������������ʹ�CHECK_TYPE_SIZE_TOO_BIG(Your_type, sizeof(Other_type));
#define CHECK_TYPE_SIZE_TOO_BIG(_typename, size)	\
	struct _BUG_T_too_big_##_typename  {	\
		char BUG_T_too_big[sizeof(_typename) <= size ? 1 : -1];	\
	};

	//�ж���������ռ�ÿռ��Ƿ�ͳ�������������������һ����
	//���ռ�ÿռ�Ҫ��ָ���Ĳ�һ��������ֱ�����󣬴��������ǣ����±�
	//�÷����Ƚ�����ռ�ÿռ��Ƿ�ͳ���һ����CHECK_TYPE_SIZE_NOT_EQUAL(Your_type, 100);
	//      �Ƚ�����ռ�ÿռ��Ƿ��������������һ����CHECK_TYPE_SIZE_NOT_EQUAL(Your_type, sizeof(Other_type));
#define CHECK_TYPE_SIZE_NOT_EQUAL(_typename, size)	\
	struct _BUG_T_not_equal_##_typename  {	\
		char BUG_T_too_big[sizeof(_typename) <= size ? 1 : -1];	\
		char BUG_T_too_small[sizeof(_typename) >= size ? 1 : -1];	\
	};

#if !(defined(Int8_DEFINE) || defined(Int8))
#define Int8_DEFINE
    typedef signed char         Int8;
    typedef unsigned char       UInt8;
    typedef signed short        Int16;
    typedef unsigned short      UInt16;
    typedef signed int          Int32;
    typedef unsigned int        UInt32;
    typedef long long           Int64;
    typedef unsigned long long  UInt64;
	typedef Int32               Boolean;
#endif

    typedef Int32           Handle; //�����
    typedef UInt8           Char;
    typedef UInt16			WChar;
    typedef UInt16          TUChar;

#ifdef __cplusplus
    extern "C" {
#endif

    typedef struct _SYSTIMESTAMP
    {
        WORD wYear;
        BYTE bMonth;
        BYTE bDay;
        BYTE bHour;
        BYTE bMinute;
        BYTE bSecond;
    }TIMESTAMP;

#ifdef __cplusplus
    }
#endif

#if defined(_WIN32)
    #if defined(SS_MAKEDLL)
        #define SS_EXPORT  __declspec(dllexport)
    #elif defined(SS_IGNORE_EXPORT)
        #define SS_EXPORT
    #else 		/* use a DLL library */
        #define SS_EXPORT  __declspec(dllimport)
    #endif
#else
    #if defined(SS_SHARED)
        #define SS_EXPORT   __attribute__((visibility("default")))
    #elif defined(SS_IGNORE_EXPORT)
        #define SS_EXPORT
    #else
        #define SS_EXPORT
    #endif
#endif

#if defined(_WIN32)
	#define SS_IMPORT  __declspec(dllimport)
#else
	#define SS_IMPORT
#endif

#ifdef _WIN32
    #define _TZD_ALIGNED_4_     __declspec(align(4))
#else 
    #define _TZD_ALIGNED_4_     __attribute__((aligned(4)))
#endif

#define SS_UNUSED(x) (void)x

    //ǿ���ڷ�������һ��Ҫ���� _TRANZDA_VM_ ��
#ifdef _WIN32
#ifndef _TRANZDA_VM_
    #error Please define _TRANZDA_VM_ macro In TG3 Emluator!!!
#endif
#endif

#endif  //__SS_TYPES_H__


