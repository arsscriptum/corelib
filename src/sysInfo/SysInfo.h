/*************************************************
Author:

Date:2021-07-15

Description:��ȡϵͳ����

**************************************************/

#pragma once
#include <atlstr.h>

/*************************************************

Function:                   GetMFDiskSpaceInfo

Description:              ��ȡ���ʣ������������Ϣ

Input:					   diskName //���ʣ����������(��λG)

                           freeSpace    // ʣ������(��λG)
Return:                    

*************************************************/ 

void GetMFDiskSpaceInfo(CString &diskName,CString &freeSpace);

/*************************************************

Function:                   GetCPUType

Description:              ��ȡCPU�ͺ���Ϣ

Input:					   mCPUType //CPU�ͺ�

Return:                    

*************************************************/ 

void GetCPUType(CString &mCPUType);

/*************************************************

Function:                   GetMemoryInfo

Description:              ��ȡϵͳ�����ڴ���Ϣ

Input:					   dwTotalPhys //�����ڴ�����(��λG)

Return:                    

*************************************************/ 

void  GetMemoryInfo(CString &dwTotalPhys);
/*************************************************

Function:                   GetOSVersion

Description:              ��ȡ����ϵͳ�汾��Ϣ

Input:					   strOSVersion //����ϵͳ�汾

Return:                    

*************************************************/ 
void GetOSVersion(CString &strOSVersion);

int round(double number);
