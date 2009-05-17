
/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2005 Tranzda Technologies Co.,Ltd.                       */
/*                    深圳市全智达科技有限公司  版权所有 2005             */
/*                                                                        */
/*  PROPRIETARY RIGHTS of Tranzda Technologies Co.,Ltd. are involved in   */
/*  the subject matter of this material. All manufacturing, reproduction, */
/*  use, and sales rights pertaining to this subject matter are governed  */
/*  by the license agreement.  The recipient of this software implicitly  */ 
/*  accepts the terms of the license.                                     */
/*  本软件文档资料是深圳市全智达科技有限公司的合法资产，任何人士阅读和使  */
/*  用本资料必须获得相应的书面授权,承担保密责任和接受相应的法律约束。     */
/*                                                                        */
/**************************************************************************/

#ifndef __TG3_REGKEY_H__
#define __TG3_REGKEY_H__

#include "ssGlobal.h"

#ifdef __cplusplus
extern "C" {
#endif

    //最大的名字（SubKey的名字和Value的名字）的长度
#define REGHKEY_MAX_NAME_LENGTH             128

    //定义注册表Key句柄类型
    typedef void * HREGKEY;

    //注册表预定义Key，可以当作已经打开的Key使用，不过使用起来有些限制。
#define REGHKEY_SYSKEY_CLASSES_ROOT         ((HREGKEY)0x00000001)
#define REGHKEY_SYSKEY_CURRENT_USER         ((HREGKEY)0x00000002)
#define REGHKEY_SYSKEY_LOCAL_MACHINE        ((HREGKEY)0x00000003)
#define REGHKEY_SYSKEY_USERS                ((HREGKEY)0x00000004)
#define REGHKEY_SYSKEY_TEMPORARY            ((HREGKEY)0x00000005)	//注意：此项建立在内存中，访问速度快，但关机不会保存
#define REGHKEY_SYSKEY_MAX_VAULE            ((HREGKEY)0x00000020)

    //注册表打开状态描述
#define REGHKEY_STATUS_CREATED_NEW_KEY      (0x00000001L)   // 指定的Key不存在，系统新建立的Key
#define REGHKEY_STATUS_OPENED_EXISTING_KEY  (0x00000002L)   // 指定的Key已存在，仅仅进行了打开操作

    //注册表ITEM类型
#define REGHKEY_ATTR_SYSKEY                 1   //注册表预定义Key
#define REGHKEY_ATTR_NORMALKEY              2   //是一个SubKey
#define REGHKEY_ATTR_VALUE                  4   //是一个Value项

    //注册表Value项数据类型
#define REGHKEY_VALUE_TYPE_NONE             0   // 没有指定类型
#define REGHKEY_VALUE_TYPE_SZ               1   // TUChar类型的字符串
#define REGHKEY_VALUE_TYPE_BINARY           3   // 任意类型
#define REGHKEY_VALUE_TYPE_DWORD            4   // 32Bit无符号整数
#define REGHKEY_VALUE_TYPE_MULTI_SZ         7   // 多个TUChar类型的字符串
#define REGHKEY_VALUE_TYPE_QWORD            11  // 64Bit无符号整数

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

    //在打开的 Key 下建立指定的 SubKey
    //如果需要建立的 SubKey 已经存在，系统会简单打开并返回，情况在 puStatus 参数中指明，具体参考 REGHKEY_STATUS_系列宏
    //初始 hKey 值需要采用 REGHKEY_SYSKEY_系列宏，以后可以采用自己打开的 Key
    //打开的 Key 句柄在 phkResult 中返回
	//如果 hKey 为系统预定义REGHKEY_SYSKEY_系列入口，pSubKeyName 为 NULL，则返回一个打开的系统预定义的 HREGKEY，
	//目的是为优化打开速度使用，否则 pSubKeyName 为 NULL 返回失败
    SS_EXPORT Int32 SS_RegKey_CreateKeyEx(IN HREGKEY hKey, IN const TUChar * pSubKeyName, OUT HREGKEY * phkResult,
        OUT UInt32 * puStatus);

    //删除指定的 Key。注意：系统 Key 不可删除
    SS_EXPORT Int32 SS_RegKey_DeleteKey(IN HREGKEY hKey, IN const TUChar * pSubKeyName);

    //在给出的 Key 下打开指定的 SubKey
	//如果 hKey 为系统预定义REGHKEY_SYSKEY_系列入口，pSubKeyName 为 NULL，则返回一个打开的系统预定义的 HREGKEY，
	//目的是为优化打开速度使用，否则 pSubKeyName 为 NULL 返回失败
    SS_EXPORT Int32 SS_RegKey_OpenKeyEx(IN HREGKEY hKey, IN const TUChar * pSubKeyName, OUT HREGKEY * phkResult);

    //关闭一个Key。注意：系统 Key （REGHKEY_SYSKEY_系列宏）不可关闭
    SS_EXPORT Int32 SS_RegKey_CloseKey(IN HREGKEY hKey);

    //在当前 Key 下设置指定的 ValueName 的 Value 和 Type
    //uValueType 参考 REGHKEY_VALUE_TYPE_系列宏
    //pValueData 指向具体数据的指针
    //nValueDataLength 数据内容长度，仅对不可计算长度的ValueType有效。对于可计算长度的ValueType，系统自动计算长度。
    SS_EXPORT Int32 SS_RegKey_SetValueEx(IN HREGKEY hKey, IN const TUChar * pValueName, IN UInt32 uValueType,
        IN const void * pValueData, IN Int32 nValueDataLength);

    //在当前 Key 下读取指定的 ValueName 的 Value 值
    //puValueType 参考 REGHKEY_VALUE_TYPE_系列宏，此域可以为 NULL。
    //pValueData 存放输出数据，此域可以为 NULL。
    //pnValueDataLength 输入：存放 pValueData 的可以使用长度；输出：Value 值的实际长度（Byte单位）
    //                  （对于字符串，长度包含0结束符），此域可以为 NULL。
    SS_EXPORT Int32 SS_RegKey_QueryValueEx(IN HREGKEY hKey, IN const TUChar * pValueName, OUT UInt32 * puValueType,
        OUT void * pValueData, IN OUT Int32 * pnValueDataLength);

    //在当前 Key 下删除指定的 ValueName 的 Value。
    SS_EXPORT Int32 SS_RegKey_DeleteValue(IN HREGKEY hKey, IN const TUChar * pValueName);

    //查询当前 Key 下的 SubKey 和 Value 等信息
    //pnSubKeysCount SubKey 的数量。此域可以为 NULL。
    //pnMaxSubKeyNameLen SubKey 的名字的最大长度，TUChar单位，结果包含结尾0结束符的长度。此域可以为 NULL。
    //pnValuesCount Values的数量。此域可以为 NULL。
    //pnMaxValueNameLen ValueName的最大长度，TUChar单位，结果包含结尾0结束符的长度。此域可以为 NULL。
    //pnMaxValueDataLen Value值的最大长度，Byte为单位。此域可以为 NULL。
    SS_EXPORT Int32 SS_RegKey_QueryInfoKey(IN HREGKEY hKey, OUT Int32 * pnSubKeysCount, OUT Int32 * pnMaxSubKeyNameLen,
        OUT Int32 * pnValuesCount, OUT Int32 * pnMaxValueNameLen,
        OUT Int32 * pnMaxValueDataLen);

    //获取当前 Key 下的指定 Index 的 Value 的所有信息
    //函数返回值：<0：错误；=0：不存在指定的 Index 的 Value 信息；>0：成功
    //nIndex Value 的 Index，从 0 开始，可以用 SS_RegKey_QueryInfoKey 得到总数量
    //pValueName Value 的名字，包含0结束符。此域可以为 NULL。
    //pnValueNameLength 输入：存放 pValueName 的可以使用长度，TUChar单位；输出：Value名字的实际长度，
    //                  TUChar单位（长度包含0结束符），此域可以为 NULL。
    //puValueType 参考 REGHKEY_VALUE_TYPE_系列宏，此域可以为 NULL。
    //pValueData 存放数据的Buffer，此域可以为 NULL。
    //pnValueDataLength 输入：存放 pValueData 的可以使用长度；输出：ValueData的实际长度（Byte单位）。
    //                  此域可以为 NULL。
    SS_EXPORT Int32 SS_RegKey_EnumValue(IN HREGKEY hKey, IN Int32 nIndex, OUT TUChar * pValueName,
        IN OUT Int32 * pnValueNameLength, OUT UInt32 * puValueType,
        OUT void * pValueData, IN OUT Int32 * pnValueDataLength);

    //获取当前 Key 下的指定 Index 的 SubKey 的名字
    //函数返回值：<0：错误；=0：不存在指定的 Index 的 SubKey；>0：成功
    //nIndex SubKey 的 Index，从 0 开始，可以用 SS_RegKey_QueryInfoKey 得到总数量
    //pSubKeyName SubKey 的名字，包含0结束符。此域可以为 NULL。
    //pnSubKeyNameLength 输入：存放 pSubKeyName 的可以使用长度，TUChar单位；
    //                   输出：SubKey名字的实际长度（长度包含0结束符），此域可以为 NULL。
    SS_EXPORT Int32 SS_RegKey_EnumKeyEx(IN HREGKEY hKey, IN Int32 nIndex, OUT TUChar * pSubKeyName,
        IN OUT Int32 * pnSubKeyNameLength);

	//启用事务进行加快速度
	//注意：只有在特殊情况下才能使用，否则会造成不可估计的后果，仅限于系统内部使用，应用禁止使用。
	//开始事务
	SS_EXPORT Int32 SS_RegKey_BeginBatch(IN HREGKEY hKey);

	//启用事务进行加快速度
	//注意：只有在特殊情况下才能使用，否则会造成不可估计的后果，仅限于系统内部使用，应用禁止使用。
	//提交事务
	SS_EXPORT Int32 SS_RegKey_CommitBatch(IN HREGKEY hKey);

	//启用事务进行加快速度
	//注意：只有在特殊情况下才能使用，否则会造成不可估计的后果，仅限于系统内部使用，应用禁止使用。
	//回滚事务
	SS_EXPORT Int32 SS_RegKey_RollBackBatch(IN HREGKEY hKey);


#ifdef __cplusplus
}
#endif

#endif  //__TG3_REGKEY_H__
