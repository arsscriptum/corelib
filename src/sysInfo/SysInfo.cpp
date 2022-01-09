#include "StdAfx.h"
#include "SysInfo.h"
#include <windows.h>

int round(double number)
{
	return (number>0.0)?(number+0.5):(number-0.5);
}

void GetMFDiskSpaceInfo(CString &diskName,CString &freeSpace)
{

	ULARGE_INTEGER nFreeBytesAvailable;
	ULARGE_INTEGER nTotalNumberOfBytes;
	ULARGE_INTEGER nTotalNumberOfFreeBytes;

	WCHAR Drive[MAX_PATH] = { 0 };//��ʼ���������Դ洢�̷���Ϣ
	GetLogicalDriveStrings(MAX_PATH, Drive);//��ñ��������̷�����Drive������

	CString myDrive;
	double mostFreeSpace = 0;
	int i = 0;
	while (Drive[i - 1] != '\0' || Drive[i] != '\0')//�������鳢�Եõ��̷���Ϣ��������߽Է���������������ͷ
	{
		myDrive = L"";
		myDrive = Drive[i];
		myDrive = myDrive + L":";//���̷���Ϣ�ĸ�ʽ����Ϊ��C:�����ָ�ʽ������ΪGetDiskFreeSpaceEx�����ĵ�һ������
		i += 4;
		
		double mywholespace,myfreespace;
		if (GetDiskFreeSpaceEx(myDrive,
			&nFreeBytesAvailable,
			&nTotalNumberOfBytes,
			&nTotalNumberOfFreeBytes))//����GetDiskFreeSpaceEx�������ָ��������Ϣ
		{
			mywholespace = nTotalNumberOfBytes.QuadPart;
			mywholespace = mywholespace/1024/1024/1024;//�õ������ܿռ䣬��λΪG
			myfreespace = nTotalNumberOfFreeBytes.QuadPart;
			myfreespace = myfreespace/1024/1024/1024;//�õ�����ʣ��ռ䣬��λΪG

			if (mostFreeSpace<myfreespace)
			{
				mostFreeSpace = myfreespace;
				freeSpace.Format(_T("%.2lf"),mostFreeSpace);
				//freeSpace = myfreespace;
				diskName = myDrive;
			}
			
		}
	}
}
DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;
void ExeCPUID(DWORD veax)//��ʼ��CPU
{
	__asm
	{
		mov eax,veax
			cpuid
			mov deax,eax
			mov debx,ebx
			mov decx,ecx
			mov dedx,edx
	}
}
void GetCPUType(CString &mCPUType)
{

	const DWORD id = 0x80000002; //��0x80000002��ʼ,��0x80000004����
	CHAR CPUType[49];//�����洢CPU�ͺ���Ϣ
	memset(CPUType,0,sizeof(CPUType));//��ʼ������

	for(DWORD t = 0 ; t < 3 ; t++ )
	{
		ExeCPUID(id+t);
		//ÿ��ѭ������,������Ϣ������
		memcpy(CPUType+16*t+ 0,&deax,4);
		memcpy(CPUType+16*t+ 4,&debx,4);
		memcpy(CPUType+16*t+ 8,&decx,4);
		memcpy(CPUType+16*t+12,&dedx,4);
	}
	mCPUType = CPUType;
}
void  GetMemoryInfo(CString &dwTotalPhys) 
{ 
	double dMaxMemory,dFreeMemory;
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	DWORD nMemoryLoad = statex.dwMemoryLoad;
	DWORD nMaxMemory = statex.ullTotalPhys/1024/1024;
	DWORD nFreeMemory = statex.ullAvailPhys/1024/1024;
	dMaxMemory = nMaxMemory;
	dMaxMemory = dMaxMemory/1024;
	dFreeMemory = nFreeMemory;
	dFreeMemory = dFreeMemory/1024;

	dwTotalPhys.Format(_T("%.2lf"),dMaxMemory);
}
void GetOSVersion(CString &strOSVersion)
{
	//setlocale(LC_ALL, "Chinese-simplified");


	OSVERSIONINFOEX OsVersionInfortion;
	OsVersionInfortion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&OsVersionInfortion);


	CString OSystemVersinName(_T("δ֪�汾"));

	switch (OsVersionInfortion.dwPlatformId)
	{

	case VER_PLATFORM_WIN32_NT:
		if (OsVersionInfortion.dwMajorVersion <= 4)
			OSystemVersinName = _T("WindowsNT");
		if (OsVersionInfortion.dwMajorVersion == 5 && OsVersionInfortion.dwMinorVersion == 0)
			OSystemVersinName = _T("Windows2000");
		if (OsVersionInfortion.dwMajorVersion == 5 && OsVersionInfortion.dwMinorVersion == 1)
			OSystemVersinName = _T("WindowsXP");
		if (OsVersionInfortion.dwMajorVersion == 5 && OsVersionInfortion.dwMinorVersion == 2)
			OSystemVersinName = _T("Windows2003");
		if (OsVersionInfortion.dwMajorVersion == 6 && OsVersionInfortion.dwMinorVersion == 0)
			OSystemVersinName = _T("WindowsVista");
		if (OsVersionInfortion.dwMajorVersion == 6 && OsVersionInfortion.dwMinorVersion == 1)
			OSystemVersinName = _T("Windows7");
		if (OsVersionInfortion.dwMajorVersion == 6 &&OsVersionInfortion.dwMinorVersion == 2)
			OSystemVersinName = _T("Windows10");
		break;
	}
	strOSVersion = OSystemVersinName;
}