#include "stdafx.h"

#include <TlHelp32.h>
#include <strsafe.h>
#include "Helper.h"



static BOOLEAN EnableDebugPrivilege()
{
     HANDLE hToken = NULL;
     TOKEN_PRIVILEGES tokenPriv;
     LUID luidDebug;
     if ( OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken ) != FALSE )
     {
          if ( LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luidDebug ))
          {
               tokenPriv.PrivilegeCount = 1;
               tokenPriv.Privileges[0].Luid = luidDebug;
               tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
               AdjustTokenPrivileges( hToken, FALSE, &tokenPriv, 0, NULL, NULL );
          }
     }
     CloseHandle( hToken );
     return TRUE;
}


BOOLEAN Helper::IsProcessWow64( DWORD Pid , PBOOL IsWow64)
{

     EnableDebugPrivilege();
     HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, Pid );
     if ( hProcess &&
          IsWow64Process( hProcess, IsWow64 ) )
     {
          return TRUE;
     }

     return FALSE;
}

BOOLEAN Helper::GetProcessFullpath(DWORD Pid, CString & ImagePath)
{
	TCHAR tmpPath[MAX_PATH] = { 0 };
	
	if (GetProcessFullpath(Pid, tmpPath))
	{
		ImagePath = tmpPath;
		return TRUE;
	}

	ImagePath = _T("Unknown path");
	return FALSE;
}

BOOLEAN Helper::GetProcessFullpath( DWORD Pid ,LPTSTR ImagePath)
{
     if ( Pid == 0 )	return FALSE;	// Skip Idle process

     HANDLE hModuleSnap;
     MODULEENTRY32  me32 = { 0 };
     me32.dwSize = sizeof( me32 );

     EnableDebugPrivilege();
     hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, Pid );
     if ( hModuleSnap != INVALID_HANDLE_VALUE )
     {
          if ( Module32First( hModuleSnap, &me32 ) )
          {
               _tcscpy_s( ImagePath, MAX_PATH, me32.szExePath );
               CloseHandle( hModuleSnap );
               return TRUE;
               
          }
          CloseHandle( hModuleSnap );
     }
     return FALSE;
}

BOOLEAN Helper::GetProcessFilename(DWORD Pid, LPTSTR Filename)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return FALSE;
	
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return FALSE;
	}

	do
	{
		if (Pid == pe32.th32ProcessID)
		{
			_tcscpy_s(Filename, MAX_PATH, pe32.szExeFile);
			return TRUE;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return FALSE;
}

BOOLEAN Helper::GetProcessFilename(DWORD Pid, CString& Filename)
{
	TCHAR tmpName[MAX_PATH] = { 0 };
	if (GetProcessFilename(Pid, tmpName))
	{
		Filename = tmpName;
		return TRUE;
	}

	Filename = _T("Unknown name");
	return FALSE;
}

#ifndef _AMD64_
BOOLEAN Helper::IsWow64Emulator()
{
     // IsWow64Process()
     // For 32 bit process in x86 : return false
     // For 32 bit process in x64 : return true
     BOOL isWow64;
     if ( IsProcessWow64( GetCurrentProcessId(), &isWow64 ) )
          return isWow64;
     return FALSE;
}
#endif