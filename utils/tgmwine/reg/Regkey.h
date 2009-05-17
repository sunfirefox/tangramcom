
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

#ifndef __TG3_REGKEY_H__
#define __TG3_REGKEY_H__

#include "ssGlobal.h"

#ifdef __cplusplus
extern "C" {
#endif

    //�������֣�SubKey�����ֺ�Value�����֣��ĳ���
#define REGHKEY_MAX_NAME_LENGTH             128

    //����ע���Key�������
    typedef void * HREGKEY;

    //ע���Ԥ����Key�����Ե����Ѿ��򿪵�Keyʹ�ã�����ʹ��������Щ���ơ�
#define REGHKEY_SYSKEY_CLASSES_ROOT         ((HREGKEY)0x00000001)
#define REGHKEY_SYSKEY_CURRENT_USER         ((HREGKEY)0x00000002)
#define REGHKEY_SYSKEY_LOCAL_MACHINE        ((HREGKEY)0x00000003)
#define REGHKEY_SYSKEY_USERS                ((HREGKEY)0x00000004)
#define REGHKEY_SYSKEY_TEMPORARY            ((HREGKEY)0x00000005)	//ע�⣺��������ڴ��У������ٶȿ죬���ػ����ᱣ��
#define REGHKEY_SYSKEY_MAX_VAULE            ((HREGKEY)0x00000020)

    //ע����״̬����
#define REGHKEY_STATUS_CREATED_NEW_KEY      (0x00000001L)   // ָ����Key�����ڣ�ϵͳ�½�����Key
#define REGHKEY_STATUS_OPENED_EXISTING_KEY  (0x00000002L)   // ָ����Key�Ѵ��ڣ����������˴򿪲���

    //ע���ITEM����
#define REGHKEY_ATTR_SYSKEY                 1   //ע���Ԥ����Key
#define REGHKEY_ATTR_NORMALKEY              2   //��һ��SubKey
#define REGHKEY_ATTR_VALUE                  4   //��һ��Value��

    //ע���Value����������
#define REGHKEY_VALUE_TYPE_NONE             0   // û��ָ������
#define REGHKEY_VALUE_TYPE_SZ               1   // TUChar���͵��ַ���
#define REGHKEY_VALUE_TYPE_BINARY           3   // ��������
#define REGHKEY_VALUE_TYPE_DWORD            4   // 32Bit�޷�������
#define REGHKEY_VALUE_TYPE_MULTI_SZ         7   // ���TUChar���͵��ַ���
#define REGHKEY_VALUE_TYPE_QWORD            11  // 64Bit�޷�������

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

    //�ڴ򿪵� Key �½���ָ���� SubKey
    //�����Ҫ������ SubKey �Ѿ����ڣ�ϵͳ��򵥴򿪲����أ������ puStatus ������ָ��������ο� REGHKEY_STATUS_ϵ�к�
    //��ʼ hKey ֵ��Ҫ���� REGHKEY_SYSKEY_ϵ�к꣬�Ժ���Բ����Լ��򿪵� Key
    //�򿪵� Key ����� phkResult �з���
	//��� hKey ΪϵͳԤ����REGHKEY_SYSKEY_ϵ����ڣ�pSubKeyName Ϊ NULL���򷵻�һ���򿪵�ϵͳԤ����� HREGKEY��
	//Ŀ����Ϊ�Ż����ٶ�ʹ�ã����� pSubKeyName Ϊ NULL ����ʧ��
    SS_EXPORT Int32 SS_RegKey_CreateKeyEx(IN HREGKEY hKey, IN const TUChar * pSubKeyName, OUT HREGKEY * phkResult,
        OUT UInt32 * puStatus);

    //ɾ��ָ���� Key��ע�⣺ϵͳ Key ����ɾ��
    SS_EXPORT Int32 SS_RegKey_DeleteKey(IN HREGKEY hKey, IN const TUChar * pSubKeyName);

    //�ڸ����� Key �´�ָ���� SubKey
	//��� hKey ΪϵͳԤ����REGHKEY_SYSKEY_ϵ����ڣ�pSubKeyName Ϊ NULL���򷵻�һ���򿪵�ϵͳԤ����� HREGKEY��
	//Ŀ����Ϊ�Ż����ٶ�ʹ�ã����� pSubKeyName Ϊ NULL ����ʧ��
    SS_EXPORT Int32 SS_RegKey_OpenKeyEx(IN HREGKEY hKey, IN const TUChar * pSubKeyName, OUT HREGKEY * phkResult);

    //�ر�һ��Key��ע�⣺ϵͳ Key ��REGHKEY_SYSKEY_ϵ�к꣩���ɹر�
    SS_EXPORT Int32 SS_RegKey_CloseKey(IN HREGKEY hKey);

    //�ڵ�ǰ Key ������ָ���� ValueName �� Value �� Type
    //uValueType �ο� REGHKEY_VALUE_TYPE_ϵ�к�
    //pValueData ָ��������ݵ�ָ��
    //nValueDataLength �������ݳ��ȣ����Բ��ɼ��㳤�ȵ�ValueType��Ч�����ڿɼ��㳤�ȵ�ValueType��ϵͳ�Զ����㳤�ȡ�
    SS_EXPORT Int32 SS_RegKey_SetValueEx(IN HREGKEY hKey, IN const TUChar * pValueName, IN UInt32 uValueType,
        IN const void * pValueData, IN Int32 nValueDataLength);

    //�ڵ�ǰ Key �¶�ȡָ���� ValueName �� Value ֵ
    //puValueType �ο� REGHKEY_VALUE_TYPE_ϵ�к꣬�������Ϊ NULL��
    //pValueData ���������ݣ��������Ϊ NULL��
    //pnValueDataLength ���룺��� pValueData �Ŀ���ʹ�ó��ȣ������Value ֵ��ʵ�ʳ��ȣ�Byte��λ��
    //                  �������ַ��������Ȱ���0�����������������Ϊ NULL��
    SS_EXPORT Int32 SS_RegKey_QueryValueEx(IN HREGKEY hKey, IN const TUChar * pValueName, OUT UInt32 * puValueType,
        OUT void * pValueData, IN OUT Int32 * pnValueDataLength);

    //�ڵ�ǰ Key ��ɾ��ָ���� ValueName �� Value��
    SS_EXPORT Int32 SS_RegKey_DeleteValue(IN HREGKEY hKey, IN const TUChar * pValueName);

    //��ѯ��ǰ Key �µ� SubKey �� Value ����Ϣ
    //pnSubKeysCount SubKey ���������������Ϊ NULL��
    //pnMaxSubKeyNameLen SubKey �����ֵ���󳤶ȣ�TUChar��λ�����������β0�������ĳ��ȡ��������Ϊ NULL��
    //pnValuesCount Values���������������Ϊ NULL��
    //pnMaxValueNameLen ValueName����󳤶ȣ�TUChar��λ�����������β0�������ĳ��ȡ��������Ϊ NULL��
    //pnMaxValueDataLen Valueֵ����󳤶ȣ�ByteΪ��λ���������Ϊ NULL��
    SS_EXPORT Int32 SS_RegKey_QueryInfoKey(IN HREGKEY hKey, OUT Int32 * pnSubKeysCount, OUT Int32 * pnMaxSubKeyNameLen,
        OUT Int32 * pnValuesCount, OUT Int32 * pnMaxValueNameLen,
        OUT Int32 * pnMaxValueDataLen);

    //��ȡ��ǰ Key �µ�ָ�� Index �� Value ��������Ϣ
    //��������ֵ��<0������=0��������ָ���� Index �� Value ��Ϣ��>0���ɹ�
    //nIndex Value �� Index���� 0 ��ʼ�������� SS_RegKey_QueryInfoKey �õ�������
    //pValueName Value �����֣�����0���������������Ϊ NULL��
    //pnValueNameLength ���룺��� pValueName �Ŀ���ʹ�ó��ȣ�TUChar��λ�������Value���ֵ�ʵ�ʳ��ȣ�
    //                  TUChar��λ�����Ȱ���0�����������������Ϊ NULL��
    //puValueType �ο� REGHKEY_VALUE_TYPE_ϵ�к꣬�������Ϊ NULL��
    //pValueData ������ݵ�Buffer���������Ϊ NULL��
    //pnValueDataLength ���룺��� pValueData �Ŀ���ʹ�ó��ȣ������ValueData��ʵ�ʳ��ȣ�Byte��λ����
    //                  �������Ϊ NULL��
    SS_EXPORT Int32 SS_RegKey_EnumValue(IN HREGKEY hKey, IN Int32 nIndex, OUT TUChar * pValueName,
        IN OUT Int32 * pnValueNameLength, OUT UInt32 * puValueType,
        OUT void * pValueData, IN OUT Int32 * pnValueDataLength);

    //��ȡ��ǰ Key �µ�ָ�� Index �� SubKey ������
    //��������ֵ��<0������=0��������ָ���� Index �� SubKey��>0���ɹ�
    //nIndex SubKey �� Index���� 0 ��ʼ�������� SS_RegKey_QueryInfoKey �õ�������
    //pSubKeyName SubKey �����֣�����0���������������Ϊ NULL��
    //pnSubKeyNameLength ���룺��� pSubKeyName �Ŀ���ʹ�ó��ȣ�TUChar��λ��
    //                   �����SubKey���ֵ�ʵ�ʳ��ȣ����Ȱ���0�����������������Ϊ NULL��
    SS_EXPORT Int32 SS_RegKey_EnumKeyEx(IN HREGKEY hKey, IN Int32 nIndex, OUT TUChar * pSubKeyName,
        IN OUT Int32 * pnSubKeyNameLength);

	//����������мӿ��ٶ�
	//ע�⣺ֻ������������²���ʹ�ã��������ɲ��ɹ��Ƶĺ����������ϵͳ�ڲ�ʹ�ã�Ӧ�ý�ֹʹ�á�
	//��ʼ����
	SS_EXPORT Int32 SS_RegKey_BeginBatch(IN HREGKEY hKey);

	//����������мӿ��ٶ�
	//ע�⣺ֻ������������²���ʹ�ã��������ɲ��ɹ��Ƶĺ����������ϵͳ�ڲ�ʹ�ã�Ӧ�ý�ֹʹ�á�
	//�ύ����
	SS_EXPORT Int32 SS_RegKey_CommitBatch(IN HREGKEY hKey);

	//����������мӿ��ٶ�
	//ע�⣺ֻ������������²���ʹ�ã��������ɲ��ɹ��Ƶĺ����������ϵͳ�ڲ�ʹ�ã�Ӧ�ý�ֹʹ�á�
	//�ع�����
	SS_EXPORT Int32 SS_RegKey_RollBackBatch(IN HREGKEY hKey);


#ifdef __cplusplus
}
#endif

#endif  //__TG3_REGKEY_H__
