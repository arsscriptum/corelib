
//==============================================================================
//
//   win32.cpp
//
//   note those 2 are originally from https://github.com/hlldz/Phant0m: 
//     WinEvent::GetPIDFromSCManager
//     WinEvent::GetPIDFromWMI
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================


#include "stdafx.h"
#include <Windows.h>
#include <minwindef.h>
#include "svclog.h"
#include <stdio.h>
#include <tlhelp32.h>
#include "psapi.h"
#include <stdio.h>
#include <tchar.h>
#include <windows-api-ex.h>
#include <windows.h>
#include <Psapi.h>
#include <sddl.h>
#include <AclAPI.h>
#include <winternl.h>
// #define USE_VDMDBG // Uncomment this if you don´t wat to use vdmdbg at all
#include <string>
#include <fstream>

#include <comdef.h>
#include <Wbemidl.h>


#pragma comment(lib, "wbemuuid.lib")
using namespace std;

namespace C
{
	namespace Convert
	{



		int wstrlen(_TCHAR* wstr)
		{
			int l_idx = 0;
			while (((char*)wstr)[l_idx] != 0) l_idx += 2;
			return l_idx;
		}

		// Allocate char string and copy TCHAR->char->string 

		char* wstrdup(_TCHAR* wSrc)
		{
			int l_idx = 0;
			int l_len = wstrlen(wSrc);
			char* l_nstr = (char*)malloc(l_len);
			if (l_nstr)
			{
				do
				{
					l_nstr[l_idx] = (char)wSrc[l_idx];
					l_idx++;
				} while ((char)wSrc[l_idx] != 0);
			}
			l_nstr[l_idx] = 0;
			return l_nstr;
		}

		// allocate argn structure parallel to argv 
		// argn must be released 

		char** allocate_argn(int argc, _TCHAR* argv[])
		{
			char** l_argn = (char**)malloc(argc * sizeof(char*));
			for (int idx = 0; idx < argc; idx++)
			{
				l_argn[idx] = wstrdup(argv[idx]);
			}

			return l_argn;
		}

		// release argn and its content 

		void release_argn(int argc, char** nargv)
		{
			for (int idx = 0; idx < argc; idx++)
			{
				free(nargv[idx]);
			}
			free(nargv);
		}

		void __TrimDecimalString(PWCHAR str)
		{
			int length = lstrlenW(str);
			for (; str[length - 1] == L'0'; length--)
			{
				str[length - 1] = L'\0';
			}

			if (str[length - 1] == L'.') str[length - 1] = L'\0';
		}

		LPWSTR StringToString(LPCSTR str)
		{
			int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
			PWCHAR result = new WCHAR[size];
			MultiByteToWideChar(CP_UTF8, 0, str, -1, result, size);

			return result;
		}
		LPSTR StringToString(LPCWSTR str)
		{
			int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
			PCHAR result = new CHAR[size];
			WideCharToMultiByte(CP_UTF8, 0, str, -1, result, size, NULL, NULL);

			return result;
		}

		LPWSTR Int32ToString(__int32 value, int base)
		{
			WCHAR buffer[20];
			_ltow_s(value, buffer, base);

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR UInt32ToString(unsigned __int32 value, int base)
		{
			WCHAR buffer[20];
			_ultow_s(value, buffer, base);

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR Int64ToString(__int64 value, int base)
		{
			WCHAR buffer[30];
			_i64tow_s(value, buffer, sizeof(buffer) / sizeof(WCHAR), base);

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR UInt64ToString(unsigned __int64 value, int base)
		{
			WCHAR buffer[30];
			_ui64tow_s(value, buffer, sizeof(buffer) / sizeof(WCHAR), base);

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR FloatToString(float value)
		{
			WCHAR buffer[50];
			swprintf_s(buffer, L"%f", value);

			__TrimDecimalString(buffer);
			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR DoubleToString(double value)
		{
			WCHAR buffer[50];
			swprintf_s(buffer, L"%.20f", value);

			__TrimDecimalString(buffer);
			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}

		__int32 StringToInt32(LPCWSTR str, int base)
		{
			return wcstol(str, NULL, base);
		}
		unsigned __int32 StringToUInt32(LPCWSTR str, int base)
		{
			return wcstoul(str, NULL, base);
		}
		__int64 StringToInt64(LPCWSTR str, int base)
		{
			return wcstoll(str, NULL, base);
		}
		unsigned __int64 StringToUInt64(LPCWSTR str, int base)
		{
			return wcstoull(str, NULL, base);
		}
		float StringToFloat(LPCWSTR str)
		{
			return wcstof(str, NULL);
		}
		double StringToDouble(LPCWSTR str)
		{
			return wcstod(str, NULL);
		}

		LPWSTR UInt32ToHexString(unsigned __int32 value)
		{
			LPWSTR buffer = UInt32ToString(value, 16);
			PWCHAR result = new WCHAR[11];

			StrCpyW(result, L"0x00000000");
			StrCpyW(&result[10 - lstrlenW(buffer)], buffer);
			delete[] buffer;

			return result;
		}
		LPWSTR UInt64ToHexString(unsigned __int64 value)
		{
			LPWSTR buffer = UInt64ToString(value, 16);
			PWCHAR result = new WCHAR[19];

			StrCpyW(result, L"0x0000000000000000");
			StrCpyW(&result[18 - lstrlenW(buffer)], buffer);
			delete[] buffer;

			return result;
		}
		LPWSTR BytesToHexView(LPBYTE data, DWORD length)
		{
			PWCHAR result = new WCHAR[((length - 1) / 16 + 1) * 79 + 1];

			for (DWORD i = 0, offset = 0; i < length; i += 16)
			{
				LPWSTR line = UInt32ToString(i, 16);
				StrCpyW(&result[offset], L"00000000");
				StrCpyW(&result[offset + 8 - lstrlenW(line)], line);
				StrCpyW(&result[offset + 8], L"h: ");

				delete[] line;
				offset += 11;

				for (DWORD j = 0; j < 16; j++)
				{
					if (i + j < length)
					{
						LPWSTR number = UInt32ToString(data[i + j], 16);
						StrCpyW(&result[offset], L"00");
						StrCpyW(&result[offset + 2 - lstrlenW(number)], number);
						result[offset + 2] = L' ';

						delete[] number;
					}
					else
					{
						result[offset] = L' ';
						result[offset + 1] = L' ';
						result[offset + 2] = L' ';
					}

					offset += 3;
				}

				result[offset++] = L';';
				result[offset++] = L' ';

				for (DWORD j = 0; j < 16; j++)
				{
					if (i + j < length)
					{
						result[offset++] = data[i + j] >= 32 && data[i + j] <= 126 ? (WCHAR)data[i + j] : L'.';
					}
					else
					{
						result[offset++] = L' ';
					}
				}

				StrCpyW(&result[offset], L"\r\n");
				offset += 2;
			}

			return result;
		}
	}

	namespace Random
	{
		LPBYTE Bytes(DWORD length)
		{
			HCRYPTPROV cryptProvider;
			if (!CryptAcquireContextW(&cryptProvider, NULL, MS_DEF_PROV_W, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) return NULL;

			LPBYTE result = new BYTE[length];
			if (!CryptGenRandom(cryptProvider, length, result))
			{
				delete[] result;
				result = NULL;
			}

			CryptReleaseContext(cryptProvider, 0);
			return result;
		}
		LPWSTR HexadecimalString(DWORD length)
		{
			LPWSTR result = NULL;

			LPBYTE data = Bytes(length);
			if (data)
			{
				result = new WCHAR[length + 1];
				result[length] = L'\0';

				for (DWORD i = 0; i < length; i++) result[i] = L"0123456789abcdef"[data[i] & 15];
				delete[] data;
			}

			return result;
		}
	}

	namespace Environment
	{
		LPWSTR GetCurrentUser()
		{
			WCHAR buffer[UNLEN + 1];
			DWORD size = sizeof(buffer) / sizeof(WCHAR);
			if (!GetUserNameW(buffer, &size)) return NULL;

			PWCHAR result = new WCHAR[size];
			StrCpyW(result, buffer);

			return result;
		}
		BOOL GetWindowsVersion(LPDWORD major, LPDWORD minor)
		{
			LPBYTE buffer = NULL;
			if (NetWkstaGetInfo(NULL, 100, &buffer) != NERR_Success) return FALSE;

			WKSTA_INFO_100* workstationInfo = (LPWKSTA_INFO_100)buffer;
			*major = workstationInfo->wki100_ver_major;
			*minor = workstationInfo->wki100_ver_minor;
			NetApiBufferFree(buffer);

			return TRUE;
		}
		Array<LPWSTR>* GetCommandLineArgs()
		{
			int count;
			LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &count);

			Array<LPWSTR>* result = NULL;

			if (argv)
			{
				result = new Array<LPWSTR>(count);

				for (int i = 0; i < count; i++)
				{
					PWCHAR buffer = new WCHAR[lstrlenW(argv[i]) + 1];
					StrCpyW(buffer, argv[i]);
					result->Add(buffer);
				}

				LocalFree(argv);
			}

			return result;
		}
		LPWSTR GetTimeStamp(BOOL useFileFormat)
		{
			time_t currentTime = time(NULL);
			struct tm* timeInfo = localtime(&currentTime);

			WCHAR buffer[50];
			DWORD size = (DWORD)wcsftime(buffer, sizeof(buffer) / sizeof(WCHAR), useFileFormat ? L"%Y-%m-%d %H.%M.%S" : L"%Y-%m-%d %H:%M:%S", timeInfo);

			PWCHAR result = new WCHAR[size + 1];
			StrCpyW(result, buffer);

			return result;
		}
	}

	namespace Path
	{
		LPWSTR Combine(LPCWSTR path1, LPCWSTR path2)
		{
			WCHAR buffer[MAX_PATH + 1];
			if (!PathCombineW(buffer, path1, path2)) return FALSE;

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR GetDirectoryName(LPCWSTR path)
		{
			PWCHAR buffer = new WCHAR[lstrlenW(path) + 1];
			StrCpyW(buffer, path);

			LPWSTR result = NULL;

			if (PathRemoveFileSpecW(buffer))
			{
				result = new WCHAR[lstrlenW(buffer) + 1];
				StrCpyW(result, buffer);
			}
			delete[] buffer;

			return result;
		}
		LPWSTR GetFileName(LPCWSTR path)
		{
			LPWSTR buffer = PathFindFileNameW(path);

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}

		LPWSTR GetExecutablePath()
		{
			return GetModulePath(NULL);
		}
		LPWSTR GetStartupPath()
		{
			LPWSTR buffer = GetExecutablePath();
			if (!buffer) return NULL;

			wcsrchr(buffer, L'\\')[0] = L'\0';

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);
			delete[] buffer;

			return result;
		}
		LPWSTR GetModulePath(HMODULE module)
		{
			WCHAR buffer[MAX_PATH + 1];
			if (!SUCCEEDED(GetModuleFileNameW(module, buffer, MAX_PATH + 1))) return NULL;

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR GetTempFolderPath()
		{
			WCHAR buffer[MAX_PATH + 1];
			DWORD size = GetTempPathW(MAX_PATH + 1, buffer);

			if (buffer[size - 1] == L'\\') buffer[size - 1] = L'\0';

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
		LPWSTR GetFolderPath(SpecialFolder folder)
		{
			WCHAR buffer[MAX_PATH + 1];
			if (!SUCCEEDED(SHGetFolderPathW(NULL, (int)folder, NULL, SHGFP_TYPE::SHGFP_TYPE_CURRENT, buffer))) return NULL;

			PWCHAR result = new WCHAR[lstrlenW(buffer) + 1];
			StrCpyW(result, buffer);

			return result;
		}
	}

	namespace Directory
	{
		Array<LPWSTR>* __GetFilesOrDirectories(LPCWSTR path, BOOL getFiles)
		{
			int length = lstrlenW(path);
			PWCHAR search = new WCHAR[length + 3];
			StrCpyW(search, path);

			if (path[length - 1] != L'\\') StrCatW(search, L"\\");
			StrCatW(search, L"*");

			WIN32_FIND_DATAW data;
			HANDLE find = FindFirstFileW(search, &data);
			delete[] search;

			if (find != INVALID_HANDLE_VALUE)
			{
				Array<LPWSTR>* files = new Array<LPWSTR>();

				do
				{
					if (StrCmpW(data.cFileName, L".") && StrCmpW(data.cFileName, L".."))
					{
						if (((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) ^ getFiles)
						{
							PWCHAR name = new WCHAR[lstrlenW(data.cFileName) + 1];
							StrCpyW(name, data.cFileName);

							files->Add(name);
						}
					}
				} while (FindNextFileW(find, &data));
				FindClose(find);

				return files;
			}

			return NULL;
		}

		BOOL Exists(LPCWSTR path)
		{
			DWORD attributes = GetFileAttributesW(path);
			return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
		}
		Array<LPWSTR>* GetDirectories(LPCWSTR path)
		{
			return __GetFilesOrDirectories(path, FALSE);
		}
		Array<LPWSTR>* GetFiles(LPCWSTR path)
		{
			return __GetFilesOrDirectories(path, TRUE);
		}
	}

	namespace File
	{
		BOOL Exists(LPCWSTR path)
		{
			DWORD attributes = GetFileAttributesW(path);
			return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY;
		}
		LPBYTE Read(LPCWSTR path, LPDWORD bytesRead)
		{
			HANDLE file = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE) return NULL;

			LPBYTE result = NULL;

			DWORD size = GetFileSize(file, NULL);
			if (size != INVALID_FILE_SIZE)
			{
				result = new BYTE[size];

				if (ReadFile(file, result, size, NULL, NULL))
				{
					*bytesRead = size;
				}
				else
				{
					delete[] result;
					result = NULL;
				}
			}

			CloseHandle(file);
			return result;
		}
		BOOL Write(LPCWSTR path, LPBYTE data, DWORD length)
		{
			HANDLE file = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE) return NULL;

			BOOL result = WriteFile(file, data, length, NULL, NULL);
			CloseHandle(file);

			return result;
		}
	}

	namespace Registry
	{
		HKEY __OpenKey(HKEY hive, LPCWSTR subkey, REGSAM desiredAccess)
		{
			HKEY key;
			if (RegOpenKeyExW(hive, subkey, 0, desiredAccess, &key) != ERROR_SUCCESS) return NULL;

			return key;
		}
		BOOL __QueryKey(HKEY hive, LPDWORD subKeyCount, LPDWORD valueCount, LPDWORD maxKeyNameLength, LPDWORD maxValueNameLength)
		{
			WCHAR achClass[MAX_PATH] = L"";
			DWORD classNameSize = MAX_PATH;
			DWORD maxClassSize;
			DWORD maxValueLength;
			DWORD securityDescriptor;
			FILETIME lastWriteTime;

			return RegQueryInfoKeyW(hive, achClass, &classNameSize, NULL, subKeyCount, maxKeyNameLength, &maxClassSize, valueCount, maxValueNameLength, &maxValueLength, &securityDescriptor, &lastWriteTime) == ERROR_SUCCESS;
		}

		LPWSTR GetValueString(HKEY hive, LPCWSTR subkey, LPCWSTR name)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_QUERY_VALUE);
			if (!key) return NULL;

			LPWSTR result = NULL;

			WCHAR buffer[1024];
			DWORD size = sizeof(buffer);
			LSTATUS status = RegQueryValueExW(key, name, 0, NULL, (LPBYTE)buffer, &size);

			if (status == ERROR_SUCCESS)
			{
				result = new WCHAR[(size - 1) / 2 + 1];
				StrCpyW(result, buffer);
			}

			RegCloseKey(key);
			return result;
		}
		BOOL GetValueDword(HKEY hive, LPCWSTR subkey, LPCWSTR name, PDWORD value)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_QUERY_VALUE);
			if (!key) return FALSE;

			DWORD size = sizeof(DWORD);
			BOOL result = RegQueryValueExW(key, name, 0, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS;

			RegCloseKey(key);
			return result;
		}
		BOOL SetValueString(HKEY hive, LPCWSTR subkey, LPCWSTR name, LPCWSTR value, BOOL isExpandedString)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_ALL_ACCESS);
			if (!key) return FALSE;

			BOOL result = RegSetValueExW(key, name, 0, isExpandedString ? REG_EXPAND_SZ : REG_SZ, (LPBYTE)value, (lstrlenW(value) + 1) * sizeof(WCHAR)) == ERROR_SUCCESS;

			RegCloseKey(key);
			return result;
		}
		BOOL SetValueDword(HKEY hive, LPCWSTR subkey, LPCWSTR name, DWORD value)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_ALL_ACCESS);
			if (!key) return FALSE;

			BOOL result = RegSetValueExW(key, name, 0, REG_DWORD, (LPBYTE)&value, sizeof(value)) == ERROR_SUCCESS;

			RegCloseKey(key);
			return result;
		}
		BOOL DeleteValue(HKEY hive, LPCWSTR subkey, LPCWSTR name)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_ALL_ACCESS);
			if (!key) return FALSE;

			BOOL result = RegDeleteValueW(key, name) == ERROR_SUCCESS;

			RegCloseKey(key);
			return result;
		}

		BOOL CreateKey(HKEY hive, LPCWSTR subkey, LPCWSTR name)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_ALL_ACCESS);
			if (!key) return FALSE;

			HKEY createdKey;
			RegCreateKeyW(key, name, &createdKey);
			BOOL result = createdKey != NULL;

			RegCloseKey(key);
			RegCloseKey(createdKey);
			return result;
		}
		BOOL DeleteKey(HKEY hive, LPCWSTR subkey, LPCWSTR name)
		{
			HKEY key = __OpenKey(hive, subkey, KEY_ALL_ACCESS);
			if (!key) return FALSE;

			BOOL result = RegDeleteKeyW(key, name) == ERROR_SUCCESS;

			RegCloseKey(key);
			return result;
		}
		Array<LPWSTR>* GetKeyNames(HKEY hive, LPCWSTR subkey)
		{
			Array<LPWSTR>* result = NULL;

			if (hive && subkey)
			{
				HKEY key = __OpenKey(hive, subkey, KEY_READ);
				if (key)
				{
					DWORD subKeyCount;
					DWORD valueCount;
					DWORD maxKeyNameLength;
					DWORD maxValueNameLength;

					if (__QueryKey(key, &subKeyCount, &valueCount, &maxKeyNameLength, &maxValueNameLength))
					{
						PWCHAR name = new WCHAR[maxKeyNameLength + 1];
						result = new Array<LPWSTR>(subKeyCount);
						int resultIndex = 0;

						for (DWORD i = 0; i < subKeyCount; i++)
						{
							DWORD nameSize = maxKeyNameLength + 1;
							name[0] = L'\0';

							if (RegEnumKeyExW(key, i, name, &nameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) continue;

							PWCHAR buffer = new WCHAR[nameSize + 1];
							StrCpyW(buffer, name);
							result->Add(buffer);
						}

						delete[] name;
					}

					RegCloseKey(key);
				}
			}

			return result;
		}
		Array<LPWSTR>* GetValueNames(HKEY hive, LPCWSTR subkey)
		{
			Array<LPWSTR>* result = NULL;

			if (hive && subkey)
			{
				HKEY key = __OpenKey(hive, subkey, KEY_READ);
				if (key)
				{
					DWORD subKeyCount;
					DWORD valueCount;
					DWORD maxKeyNameLength;
					DWORD maxValueNameLength;

					if (__QueryKey(key, &subKeyCount, &valueCount, &maxKeyNameLength, &maxValueNameLength))
					{
						PWCHAR name = new WCHAR[maxValueNameLength + 1];
						result = new Array<LPWSTR>(valueCount);
						int resultIndex = 0;

						for (DWORD i = 0; i < valueCount; i++)
						{
							DWORD nameSize = maxValueNameLength + 1;
							name[0] = L'\0';

							if (RegEnumValueW(key, i, name, &nameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) continue;
							if (RegQueryValueExW(key, name, 0, NULL, NULL, NULL) != ERROR_SUCCESS) continue;

							PWCHAR buffer = new WCHAR[nameSize + 1];
							StrCpyW(buffer, name);
							result->Add(buffer);
						}

						delete[] name;
					}

					RegCloseKey(key);
				}
			}

			return result;
		}
	}

	namespace WinTime
	{

		VOID TimeToTimeFields(PLARGE_INTEGER Time, PTIME_FIELDS TimeFields)
		{
			const UCHAR* Months;
			ULONG SecondsInDay, CurYear;
			ULONG LeapYear, CurMonth;
			ULONG Days;
			ULONGLONG IntTime = Time->QuadPart;

			/* Extract millisecond from time and convert time into seconds */
			TimeFields->Milliseconds = (CSHORT)((IntTime % TICKSPERSEC) / TICKSPERMSEC);
			IntTime = IntTime / TICKSPERSEC;

			/* Split the time into days and seconds within the day */
			Days = (ULONG)(IntTime / SECSPERDAY);
			SecondsInDay = IntTime % SECSPERDAY;

			/* Compute time of day */
			TimeFields->Hour = (CSHORT)(SecondsInDay / SECSPERHOUR);
			SecondsInDay = SecondsInDay % SECSPERHOUR;
			TimeFields->Minute = (CSHORT)(SecondsInDay / SECSPERMIN);
			TimeFields->Second = (CSHORT)(SecondsInDay % SECSPERMIN);

			/* Compute day of week */
			TimeFields->Weekday = (CSHORT)((EPOCHWEEKDAY + Days) % DAYSPERWEEK);

			/* Compute year */
			CurYear = EPOCHYEAR;
			CurYear += Days / DAYSPERLEAPYEAR;
			Days -= DaysSinceEpoch(CurYear);
			while (TRUE)
			{
				LeapYear = IsLeapYear(CurYear);
				if (Days < YearLengths[LeapYear])
				{
					break;
				}
				CurYear++;
				Days = Days - YearLengths[LeapYear];
			}
			TimeFields->Year = (CSHORT)CurYear;

			/* Compute month of year */
			LeapYear = IsLeapYear(CurYear);
			Months = MonthLengths[LeapYear];
			for (CurMonth = 0; Days >= Months[CurMonth]; CurMonth++)
			{
				Days = Days - Months[CurMonth];
			}
			TimeFields->Month = (CSHORT)(CurMonth + 1);
			TimeFields->Day = (CSHORT)(Days + 1);
		}

	}
	namespace WinEvent
	{

		DWORD GetPIDFromSCManager() {

			PRINT_OUT("[*] Attempting to detect PID from Service Manager...\n");

			SC_HANDLE schSCManager, schService;
			SERVICE_STATUS_PROCESS ssProcess = {};
			DWORD dwBytesNeeded = 0;

			schSCManager = OpenSCManagerA(NULL, NULL, SERVICE_QUERY_STATUS);

			if (NULL == schSCManager) {

				PRINT_OUT("[!] SCM: OpenSCManager failed (%d)\n", GetLastError());
				return 0;

			}

			schService = OpenServiceA(schSCManager, "EventLog", SERVICE_QUERY_STATUS);

			if (schService == NULL) {

				PRINT_OUT("[!] SCM: OpenService failed (%d)\n", GetLastError());
				CloseServiceHandle(schSCManager);
				return 0;

			}

			if (!QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssProcess), sizeof(ssProcess), &dwBytesNeeded)) {

				PRINT_OUT("[!] SCM: QueryServiceStatusEx failed (%d)\n", GetLastError());
				CloseServiceHandle(schService);
				CloseServiceHandle(schSCManager);
				return 0;

			}

			return ssProcess.dwProcessId;
		}


		DWORD GetPIDFromWMI() {

			PRINT_OUT("[*] Attempting to detect PID from WMI....\n");

			DWORD dwEventLogPID = 0;

			HRESULT hRes;

			hRes = CoInitializeEx(0, COINIT_MULTITHREADED);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Failed to initialize COM library.\n");
				return 0;

			}

			hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Failed to initialize security.\n");
				CoUninitialize();
				return 0;

			}

			IWbemLocator* pLoc = NULL;

			hRes = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Failed to create IWbemLocator object.\n");
				CoUninitialize();
				return 0;

			}

			IWbemServices* pSvc = NULL;

			hRes = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Could not connect.");
				pLoc->Release();
				CoUninitialize();
				return 0;

			}

			hRes = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Could not set proxy blanket.\n");
				pSvc->Release();
				pLoc->Release();
				CoUninitialize();
				return 0;

			}

			IEnumWbemClassObject* pEnumerator = NULL;

			hRes = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Service"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

			if (FAILED(hRes)) {

				PRINT_OUT("[!] WMI: Query failed.\n");
				pSvc->Release();
				pLoc->Release();
				CoUninitialize();
				return 0;

			}

			IWbemClassObject* pclsObj = NULL;
			ULONG uReturn = 0;

			while (pEnumerator) {

				HRESULT hR = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

				if (0 == uReturn) {

					break;

				}

				VARIANT vtProp;
				VariantInit(&vtProp);

				hR = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);

				if (_wcsicmp(vtProp.bstrVal, L"eventlog") == 0) {

					hR = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);
					dwEventLogPID = vtProp.intVal;
					break;

				}

				VariantClear(&vtProp);
				pclsObj->Release();

			}

			pSvc->Release();
			pLoc->Release();
			pEnumerator->Release();
			CoUninitialize();

			return dwEventLogPID;
		}
	}

	namespace Process
	{
		LPVOID addr;
		typedef NTSTATUS(NTAPI* __NtQueryInformationProcess)(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, DWORD ProcessInformationLength, PDWORD ReturnLength);


		BOOL IsRunAsAdministrator()
		{
			BOOL fIsRunAsAdmin = FALSE;
			DWORD dwError = ERROR_SUCCESS;
			PSID pAdministratorsGroup = NULL;

			// Allocate and initialize a SID of the administrators group.
			SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
			if (!AllocateAndInitializeSid(
				&NtAuthority,
				2,
				SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_ALIAS_RID_ADMINS,
				0, 0, 0, 0, 0, 0,
				&pAdministratorsGroup))
			{
				dwError = GetLastError();
				goto Cleanup;
			}

			// Determine whether the SID of administrators group is enabled in 
			// the primary access token of the process.
			if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
			{
				dwError = GetLastError();
				goto Cleanup;
			}

		Cleanup:
			// Centralized cleanup for all allocated resources.
			if (pAdministratorsGroup)
			{
				FreeSid(pAdministratorsGroup);
				pAdministratorsGroup = NULL;
			}

			// Throw the error if something failed in the function.
			if (ERROR_SUCCESS != dwError)
			{
				throw dwError;
			}

			return fIsRunAsAdmin;
		}



		BOOL CheckIntegrityLevel() {

			BOOL checkResult = FALSE;

			HANDLE hToken, hProcess;

			DWORD dwLengthNeeded, dwIntegrityLevel;
			DWORD dwError = ERROR_SUCCESS;

			PTOKEN_MANDATORY_LABEL pTIL = NULL;

			hProcess = GetCurrentProcess();
			if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken)) {

				if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwLengthNeeded)) {

					dwError = GetLastError();
					if (dwError == ERROR_INSUFFICIENT_BUFFER) {

						pTIL = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwLengthNeeded);
						if (pTIL != NULL) {

							if (GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwLengthNeeded, &dwLengthNeeded)) {

								dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid, (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));

								if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID) {
									checkResult = TRUE;
								}

							}

							LocalFree(pTIL);
						}

					}

				}

				CloseHandle(hToken);

			}

			return checkResult;

		}

		DWORD GetCurrentSessionId()
		{
			WTS_SESSION_INFO* pSessionInfo;
			DWORD n_sessions = 0;
			BOOL ok = WTSEnumerateSessions(WTS_CURRENT_SERVER, 0, 1, &pSessionInfo, &n_sessions);
			if (!ok)
				return 0;

			DWORD SessionId = 0;

			for (DWORD i = 0; i < n_sessions; ++i)
			{
				if (pSessionInfo[i].State == WTSActive)
				{
					SessionId = pSessionInfo[i].SessionId;
					break;
				}
			}

			WTSFreeMemory(pSessionInfo);
			return SessionId;
		}
		BOOL EnableDebugPrivilege() {

			HANDLE hToken;

			if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
				TOKEN_PRIVILEGES tp;
				LUID luid;

				if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {

					tp.PrivilegeCount = 1;
					tp.Privileges[0].Luid = luid;
					tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

					if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) {

						return TRUE;

					}
				}
			}

			return FALSE;
		}

		BOOL IsDebugPrivilegeEnabled() {

			BOOL privilgeStatus = FALSE;

			LUID luid;
			PRIVILEGE_SET privs;
			HANDLE hProcess;
			HANDLE hToken;
			hProcess = GetCurrentProcess();

			if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {

				if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {

					privs.PrivilegeCount = 1;
					privs.Control = PRIVILEGE_SET_ALL_NECESSARY;
					privs.Privilege[0].Luid = luid;
					privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;

					BOOL privCheckResult;
					PrivilegeCheck(hToken, &privs, &privCheckResult);

					if (privCheckResult == TRUE) {

						PRINT_OUT("[+] SeDebugPrivilege is enable, continuing...\n\n");

						privilgeStatus = TRUE;
					}
					else {

						PRINT_OUT("[!] SeDebugPrivilege is not enabled, trying to enable...\n");

						if (EnableDebugPrivilege() == TRUE) {

							PRINT_OUT("[+] SeDebugPrivilege is enabled, continuing...\n\n");

							privilgeStatus = TRUE;

						}
						else {

							privilgeStatus = FALSE;

						}

					}

				}

			}

			return privilgeStatus;

		}
		LPCWSTR GetIntegrityLevelName(DWORD integrityLevel)
		{
			if (integrityLevel >= SECURITY_MANDATORY_UNTRUSTED_RID && integrityLevel < SECURITY_MANDATORY_LOW_RID) return L"Untrusted";
			else if (integrityLevel >= SECURITY_MANDATORY_LOW_RID && integrityLevel < SECURITY_MANDATORY_MEDIUM_RID) return L"Low";
			else if (integrityLevel >= SECURITY_MANDATORY_MEDIUM_RID && integrityLevel < SECURITY_MANDATORY_HIGH_RID) return L"Medium";
			else if (integrityLevel >= SECURITY_MANDATORY_HIGH_RID && integrityLevel < SECURITY_MANDATORY_SYSTEM_RID) return L"High";
			else if (integrityLevel >= SECURITY_MANDATORY_SYSTEM_RID) return L"System";
			else return NULL;
		}




		BOOL SetPrivilege(
			HANDLE hToken,          // access token handle
			LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
			BOOL bEnablePrivilege   // to enable or disable privilege
		)
		{
			TOKEN_PRIVILEGES tp;
			LUID luid;

			if (!LookupPrivilegeValue(
				NULL,            // lookup privilege on local system
				lpszPrivilege,   // privilege to lookup 
				&luid))        // receives LUID of privilege
			{
				COUTR("LookupPrivilegeValue error: %u", GetLastError());
				return FALSE;
			}

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			if (bEnablePrivilege)
				tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			else
				tp.Privileges[0].Attributes = 0;

			// Enable the privilege or disable all privileges.

			if (!AdjustTokenPrivileges(
				hToken,
				FALSE,
				&tp,
				sizeof(TOKEN_PRIVILEGES),
				(PTOKEN_PRIVILEGES)NULL,
				(PDWORD)NULL))
			{
				PRINT_OUT("AdjustTokenPrivileges error: %u", GetLastError());
				return FALSE;
			}

			if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

			{
				
				return FALSE;
			}

			return TRUE;
		}

		BOOL EnableRequiredPrivileges()
		{
			DWORD dwError = ERROR_SUCCESS;

			TCHAR szUserProfileDir[MAX_PATH];
			DWORD cchUserProfileDir = ARRAYSIZE(szUserProfileDir);
			STARTUPINFO si = { sizeof(si) };
			PROCESS_INFORMATION pi = { 0 };

			DWORD dwSessionId = GetCurrentSessionId();
			if (dwSessionId == 0)    // no-one logged in
			{
				PRINT_OUT("GetCurrentSessionId failed (%d).", GetLastError());
				return false;
			}

			RevertToSelf();
			HANDLE hToken = NULL;
			BOOL goodToken = WTSQueryUserToken(dwSessionId, &hToken);
			if (!goodToken)
			{
				DWORD err = GetLastError();
				PRINT_OUT("WTSQueryUserToken failed (%d).", GetLastError());

				if (!OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, TRUE, &hToken))
				{
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
						return FALSE;
					}
				}
			}

			BOOL bSeDebugPrivilege = SetPrivilege(hToken, TEXT("SeDebugPrivilege"), TRUE);
			if (!bSeDebugPrivilege) {
				COUTY("[*] SetPrivilege SeDebugPrivilege failed. CODE: 0x%08X", GetLastError());
			}
			BOOL bSeAssignPrimaryTokenPrivilege = SetPrivilege(hToken, TEXT("SeAssignPrimaryTokenPrivilege"), TRUE);
			if (!bSeAssignPrimaryTokenPrivilege) {
				COUTY("[*] SetPrivilege SeAssignPrimaryTokenPrivilege failed. CODE: 0x%08X", GetLastError());
			}
			BOOL bSeImpersonatePrivilege = SetPrivilege(hToken, TEXT("SeImpersonatePrivilege"), TRUE);
			if (!bSeImpersonatePrivilege) {
				COUTY("[*] SetPrivilege SeImpersonatePrivilege failed. CODE: 0x%08X", GetLastError());
			}
			BOOL bSeCreateTokenPrivilege = SetPrivilege(hToken, TEXT("SeCreateTokenPrivilege"), TRUE);
			if (!bSeCreateTokenPrivilege) {
				COUTY("[*] SetPrivilege SeCreateTokenPrivilege failed. CODE: 0x%08X", GetLastError());
			}
			BOOL bSeTcbPrivilege = SetPrivilege(hToken, TEXT("SeTcbPrivilege"), TRUE);
			if (!bSeTcbPrivilege) {
				COUTY("[*] SetPrivilege SeTcbPrivilege failed. CODE: 0x%08X\n", GetLastError());
			}
			BOOL bSeIncreaseQuotaPrivilege = SetPrivilege(hToken, TEXT("SeIncreaseQuotaPrivilege"), TRUE);
			if (!bSeIncreaseQuotaPrivilege) {
				COUTY("[*] SetPrivilege SeIncreaseQuotaPrivilege failed. CODE: 0x%08X", GetLastError());
			}

			if (!bSeAssignPrimaryTokenPrivilege ||
				!bSeImpersonatePrivilege ||
				!bSeCreateTokenPrivilege ||
				!bSeTcbPrivilege ||
				!bSeIncreaseQuotaPrivilege)
			{
				COUTY("[*] SetPrivilege failed.\n");
				return false;
			}
			COUTY("[*] Privilege Enabled: SeAssignPrimaryTokenPrivilege\n");
			COUTY("[*] Privilege Enabled: SeImpersonatePrivilege\n");
			COUTY("[*] Privilege Enabled: SeCreateTokenPrivilege\n");
			COUTY("[*] Privilege Enabled: SeTcbPrivilege\n");
			COUTY("[*] Privilege Enabled: SeIncreaseQuotaPrivilege\n");

			return TRUE;
		}

		void ElevateNow(int argc, TCHAR argv[], TCHAR envp)
		{
			BOOL bAlreadyRunningAsAdministrator = FALSE;
			try
			{
				bAlreadyRunningAsAdministrator = IsRunAsAdministrator();
			}
			catch (...)
			{
				std::cout << "Failed to determine if application was running with admin rights" << std::endl;
				DWORD dwErrorCode = GetLastError();
				char szMessage[256];
				sprintf(szMessage, "Error code returned was 0x%08lx", dwErrorCode);
				std::cout << szMessage << std::endl;
			}
			if (!bAlreadyRunningAsAdministrator)
			{
				TCHAR szPath[MAX_PATH];
				if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
				{
					std::cout << "Running under ELEVATION: " << szPath << std::endl;
					char szParams[256]; 
					sprintf(szParams, "-u tet");
					std::cout << szParams << std::endl;

					// Launch itself as admin
					SHELLEXECUTEINFO sei = { sizeof(sei) };
					sei.lpParameters = C::Convert::StringToString(szParams);
					sei.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS;
					sei.lpVerb = TEXT("runas");
					sei.lpFile = szPath;
					sei.hwnd = NULL;
					sei.nShow = SW_NORMAL;

					if (!ShellExecuteEx(&sei))
					{
						DWORD dwError = GetLastError();
						if (dwError == ERROR_CANCELLED)
						{
							// The user refused to allow privileges elevation.
							std::cout << "End user did not allow elevation" << std::endl;
						}
					}
				}
			}
		}
		// Either returns true (for a retry) or false (success or failure)
		// Failure: pnbProcesses is 0 and there is no buffer to free
		// Success: pnbProcesses is greater than 0 and *pprocesses contains a pointer to be freed
		BOOL FillProcessesListWithAlloc(PDWORD* pprocesses, DWORD size, PDWORD pnbProcesses)
		{
			DWORD* processes, bytes = 0, result = 0;
			BOOL retry = FALSE, realloc = FALSE;

			// Attempt allocation or reallocation
			if (!(*pprocesses)) {
				processes = (PDWORD)HeapAlloc(GetProcessHeap(), 0, size);
			}
			else {
				processes = (PDWORD)HeapReAlloc(GetProcessHeap(), 0, *pprocesses, size);
				realloc = TRUE;
			}
			// If allocation for requested size succeeded
			if (processes) {
				if (EnumProcesses(processes, size, &bytes)) {
					// Success
					if (bytes < size) {
						result = bytes / sizeof(DWORD);
					}
					else {
						// Buffer too small to list all processIDs
						retry = TRUE;
					}
					// Writes the allocation pointer back in case of success or retry
					*pprocesses = processes;
				}
				else {
					HeapFree(GetProcessHeap(), 0, processes);
					PRINT_OUT("EnumProcesses() failure, error %#.8x", GetLastError());
				}
			} // if processes
			else {
				// Allocation failure handling
				PRINT_OUT("Allocation failure (requested %#.8x bytes), aborting", size);
				// If realloc failed, a free is necessary
				if (realloc) {
					HeapFree(GetProcessHeap(), 0, *pprocesses);
				}
			}
			// Write back nb of processe only if we are done
			if (!retry) {
				*pnbProcesses = result;
			}
			return retry;
		}

		// Attemps to fill the stack buffer if large enough, otherwise move on to allocations
		DWORD FillProcessesList(PDWORD* pprocesses, DWORD bufsize)
		{
			DWORD nb_processes = 0, bytes, size = bufsize;
			BOOL retry;

			// First attemps on stack buffer
			if (EnumProcesses(*pprocesses, size, &bytes)) {
				if (bytes >= size) {
					// Not large enough, allocating
					*pprocesses = NULL;
					do {
						size *= 2;    // doubling size of buffer for processIDs list
						retry = FillProcessesListWithAlloc(pprocesses, size, &nb_processes);
					} while (retry);
				}
				else {
					nb_processes = bytes / sizeof(DWORD);
				}
			} // if enumProcesses
			else {
				COUTY("EnumProcesses failed with error %#.8x", GetLastError());
			}
			return nb_processes;
		}
		BOOL ProcessIdToName(DWORD processId, TCHAR* processName, DWORD buffSize)
		{
			BOOL ret = FALSE;
			HANDLE handle = OpenProcess(
				PROCESS_QUERY_LIMITED_INFORMATION,
				FALSE,
				processId
			);
			if (handle) {
				DWORD copied = QueryFullProcessImageName(handle, 0, processName, &buffSize);
				if (copied > 0 && copied <= buffSize) {
					ret = TRUE;
				}
				else {
					PRINT_OUT("Error QueryFullProcessImageName : %lu", GetLastError());
				}
				CloseHandle(handle);
			}
			else {
				PRINT_OUT("Error OpenProcess : %lu", GetLastError());
			}
			return ret;
		}
		// Returns success boolean and outputs process handle with requested rights
		BOOL GetProcessbyNameOrId(LPTSTR searchstring, PHANDLE phProcess, DWORD rights)
		{
			BOOL found = FALSE;
			HMODULE hMod;
			DWORD* processes, lpProcesses[QUITE_LARGE_NB_PROCESSES], bytes, processId;
			SIZE_T nbProcesses, i;
			HANDLE hProcess;
			TCHAR processname[MAX_PATH + 1], * stop;

			processId = _tcstoul(searchstring, &stop, 0);
			if (processId && *stop == L'\0') {
				hProcess = OpenProcess(rights, FALSE, processId);
				if (hProcess) {
					*phProcess = hProcess;
					found = TRUE;
				}
			}
			else {
				processes = lpProcesses;
				nbProcesses = FillProcessesList(&processes, sizeof(lpProcesses));
				if (nbProcesses) {
					for (i = 0; i < nbProcesses && !found; i++) {
						hProcess = OpenProcess(IDENTIFICATION_RIGHTS, FALSE, processes[i]);
						if (hProcess) {
							if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &bytes)) {
								if (GetModuleBaseName(hProcess, hMod, processname, sizeof(processname) / sizeof(TCHAR))) {
									// Found the process by that name
									if (!_tcsicmp(searchstring, processname)) {
										// Close the handle and attempt reopenning with requested rights
										CloseHandle(hProcess);
										hProcess = OpenProcess(rights, FALSE, processes[i]);
										if (hProcess) {
											*phProcess = hProcess;
											found = TRUE;
										}
									} // if _tcsicmp
								} // if GetModuleBaseName
							} // if EnumProcessModules
							if (!found) {
								// Only close this process handle if it is not the one we are looking for
								CloseHandle(hProcess);
							}
						} // if hProcess
					} // for all processes
					if (processes != lpProcesses) {
						HeapFree(GetProcessHeap(), 0, processes);
					}
				} // if nbProcesses
			}
			return found;
		}


		BOOL ListProcessThreads(DWORD dwOwnerPID, TThreads& threads)
		{
			HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
			HANDLE hThread = NULL;
			THREADENTRY32 te32;

			// Take a snapshot of all running threads  
			hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			if (hThreadSnap == INVALID_HANDLE_VALUE)
				return(FALSE);

			// Fill in the size of the structure before using it. 
			te32.dwSize = sizeof(THREADENTRY32);

			// Retrieve information about the first thread,
			// and exit if unsuccessful
			if (!Thread32First(hThreadSnap, &te32))
			{
				printError(TEXT("Thread32First"));  // Show cause of failure
				CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
				return(FALSE);
			}

			// Now walk the thread list of the system,
			// and display information about each thread
			// associated with the specified process
			do
			{
				if (te32.th32OwnerProcessID == dwOwnerPID)
				{
					hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, te32.th32ThreadID);
					SThreadEntry thrd;
					thrd.threadId = te32.th32ThreadID;
					thrd.parentPid = dwOwnerPID;
					thrd.startAddress = GetThreadStartAddress(hThread);
					thrd.size = te32.dwSize;
					thrd.flags = te32.dwFlags;
					threads.push_back(thrd);
				}
			} while (Thread32Next(hThreadSnap, &te32));


			//  Don't forget to clean up the snapshot object.
			CloseHandle(hThreadSnap);
			return(TRUE);
		}

		DWORD GetThreadStartAddress(HANDLE hThread)
		{

			NTSTATUS ntStatus;
			DWORD dwStartAddress = 0;

			pNtQIT NtQueryInformationThread = (pNtQIT)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationThread");
			if (NtQueryInformationThread == NULL)
				return 0;

			ntStatus = NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &dwStartAddress, sizeof(DWORD), NULL);

			CloseHandle(hThread);


			if (ntStatus != STATUS_SUCCESS) return 0;

			return dwStartAddress;

		}
		void printError(TCHAR* msg)
		{
			DWORD eNum;
			TCHAR sysMsg[256];
			TCHAR* p;

			eNum = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, eNum,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				sysMsg, 256, NULL);

			// Trim the end of the line and terminate it with a null
			p = sysMsg;
			while ((*p > 31) || (*p == 9))
				++p;
			do { *p-- = 0; } while ((p >= sysMsg) &&
				((*p == '.') || (*p < 33)));

			// Display the message
			PRINT_OUT(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
		}

	}

	namespace Thread
	{
		typedef LONG    NTSTATUS;
		typedef NTSTATUS(WINAPI* pNtQIT)(HANDLE, LONG, PVOID, ULONG, PULONG);
#define STATUS_SUCCESS    ((NTSTATUS)0x00000000L)
#define ThreadQuerySetWin32StartAddress 9

		DWORD WINAPI GetThreadStartAddress(HANDLE hThread)
		{
			NTSTATUS ntStatus;
			HANDLE hDupHandle;
			DWORD dwStartAddress;

			pNtQIT NtQueryInformationThread = (pNtQIT)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationThread");

			if (NtQueryInformationThread == NULL) return 0;

			HANDLE hCurrentProcess = GetCurrentProcess();

			if (!DuplicateHandle(hCurrentProcess, hThread, hCurrentProcess, &hDupHandle, THREAD_QUERY_INFORMATION, FALSE, 0)) {
				SetLastError(ERROR_ACCESS_DENIED);
				return 0;
			}

			ntStatus = NtQueryInformationThread(hDupHandle, ThreadQuerySetWin32StartAddress, &dwStartAddress, sizeof(DWORD), NULL);

			CloseHandle(hDupHandle);

			if (ntStatus != STATUS_SUCCESS) return 0;
			return dwStartAddress;
		}
	}

	namespace Service
	{
		SC_HANDLE GetServiceByName(LPCWSTR name)
		{
			SC_HANDLE serviceManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (!serviceManager) return NULL;

			SC_HANDLE service = OpenServiceW(serviceManager, name, SC_MANAGER_ALL_ACCESS);

			CloseServiceHandle(serviceManager);
			return service;
		}
		DWORD GetServiceState(SC_HANDLE service)
		{
			SERVICE_STATUS_PROCESS status;
			DWORD bytesNeeded;
			if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) return 0;

			return status.dwCurrentState;
		}
		DWORD GetServiceProcessId(SC_HANDLE service)
		{
			SERVICE_STATUS_PROCESS status;
			DWORD bytesNeeded;
			if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) return 0;

			return status.dwProcessId;
		}
		BOOL StartServiceWait(SC_HANDLE service, DWORD expectedState, DWORD delayMilliseconds, DWORD timeoutMilliseconds)
		{
			BOOL result = FALSE;
			ULONGLONG startTime = GetTickCount64();

			while (!result && GetTickCount64() - startTime < timeoutMilliseconds)
			{
				StartServiceW(service, 0, NULL);
				Sleep(delayMilliseconds);

				result = GetServiceState(service) == expectedState;
			}

			return result;
		}
		BOOL ControlServiceWait(SC_HANDLE service, DWORD control, DWORD expectedState, DWORD delayMilliseconds, DWORD timeoutMilliseconds)
		{
			BOOL result = FALSE;
			ULONGLONG startTime = GetTickCount64();
			SERVICE_STATUS_PROCESS status;

			while (!result && GetTickCount64() - startTime < timeoutMilliseconds)
			{
				ControlService(service, control, (LPSERVICE_STATUS)&status);
				Sleep(delayMilliseconds);

				result = GetServiceState(service) == expectedState;
			}

			return result;
		}
	}

}