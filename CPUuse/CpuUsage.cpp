/////////////////////////////////////////////////////////////////////////////
// 类名：CCpuUsage
// 功能：CPU利用率接口声明部分(服务器端)
// 说明：提供三个接口涵数：
//       IsShellSystemType()用来检测系统版本;
//		 CpuUsage2000()来完成NT/2000下CPU利用率;
//		 CpuUsage9x()来完成9x下CPU利用率;
// 作者：徐景周(johnny Xu, xujingzhou2016@gmail.com)
// 组织：未来工作室(Future Studio)
// 日期：2003.1.1
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CPUuse.h"
#include "CpuUsage.h"
/////////////////////////////////////////////////////////////////////////////
// CCpuUsage

STDMETHODIMP CCpuUsage::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ICpuUsage
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// ------------------------------------------------------------------------------------------------
// 判断操作系统涵数及变量，jingzhou xu
// Operating system Version number(https://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx)
// Windows 8 6.2
// Windows 7 6.1
// Windows Server 2008 R2 6.1
// Windows Server 2008 6.0
// Windows Vista 6.0
// Windows Server 2003 R2 5.2
// Windows Server 2003 5.2
// Windows XP 5.1
// Windows 2000 5.0

typedef enum tagWin32SysType
{
	Windows32s,
	WindowsNT3,
	Windows95,
	Windows98,
	WindowsME,
	WindowsNT4,
	Windows2000,
	WindowsXP,
	WindowsVista,
	Windows7,
	Windows8,
	Windows81,
	Windows10
} Win32SysType;

//BOOL IsWinVerEqualTo(DWORD dwMajorVersion, DWORD dwMinorVersion)
//{
//    OSVERSIONINFOEXW osvi = {0};
//    DWORDLONG dwlConditionMask = 0;
// 
//    ZeroMemory(&osvi, sizeof(osvi));
//    osvi.dwOSVersionInfoSize = sizeof(osvi);
//    osvi.dwMajorVersion = dwMajorVersion;
//    osvi.dwMinorVersion = dwMinorVersion;
// 
//    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
//    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
// 
//    return ::VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
//}

STDMETHODIMP CCpuUsage::GetOSVersion(int *plMajor, int *plMinor)
{
	HMODULE hKernel32 = GetModuleHandleA("KERNEL32.DLL");
	char kernel32Path[256] = {0};
	GetModuleFileNameA(hKernel32, kernel32Path, 256);

	DWORD               dwSize              = 0;
	BYTE                *pVersionInfo       = NULL;
	VS_FIXEDFILEINFO    *pFileInfo          = NULL;
	UINT                pLenFileInfo        = 0;

	/*getting the file version info size */
	dwSize = GetFileVersionInfoSizeA(kernel32Path, NULL);
	if ( dwSize == 0 ) 
	{
		return S_FALSE;
	}

	pVersionInfo = new BYTE[dwSize]; /*allocation of space for the verison size */

	if (!GetFileVersionInfoA( kernel32Path, 0, dwSize, pVersionInfo )) 
	{ 
		/*entering all info data to pbVersionInfo*/
		delete[] pVersionInfo;
		return S_FALSE;
	}

	if (!VerQueryValueA(pVersionInfo, "\\", (LPVOID*) &pFileInfo, &pLenFileInfo)) 
	{
		delete[] pVersionInfo;
		return S_FALSE;
	}

	int major  = (pFileInfo->dwFileVersionMS >> 16) & 0xffff ;
	int minor  = (pFileInfo->dwFileVersionMS)       & 0xffff;
	int hotfix = (pFileInfo->dwFileVersionLS >> 16) & 0xffff;
	int other  = (pFileInfo->dwFileVersionLS)       & 0xffff; 

	*plMajor = major;
	*plMinor = minor;

	delete[] pVersionInfo;

	return S_OK;
}

STDMETHODIMP CCpuUsage::IsShellSystemType(BSTR *pbstrSysType)
{
	Win32SysType  ShellType;
	DWORD winVer;
	OSVERSIONINFO *osvi;
	
	winVer = GetVersion();
	if(winVer < 0x80000000)
	{	/*NT */
		ShellType = WindowsNT3;
		osvi = (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi != NULL)
		{
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);

			if(osvi->dwMajorVersion == 4L)
				ShellType = WindowsNT4;
			else if(osvi->dwMajorVersion == 5L && osvi->dwMinorVersion == 0L)
				ShellType=Windows2000;
			else if(osvi->dwMajorVersion == 5L && osvi->dwMinorVersion == 1L)
				ShellType = WindowsXP;
			else if(osvi->dwMajorVersion == 6L && osvi->dwMinorVersion == 0L)
				ShellType = WindowsVista;
			else if(osvi->dwMajorVersion == 6L && osvi->dwMinorVersion == 1L)
				ShellType = Windows7;
			else if(osvi->dwMajorVersion == 6L && osvi->dwMinorVersion == 2L)
			{
				ShellType = Windows8;
				
				// Secondary Detection 
				if(osvi->dwBuildNumber == 9200L)
				{
					ShellType = Windows10;
				}
			}
			else if(osvi->dwMajorVersion == 6L && osvi->dwMinorVersion == 3L)
				ShellType = Windows81;
			else if(osvi->dwMajorVersion == 10L && osvi->dwMinorVersion == 0L)
				ShellType = Windows10;

			free(osvi);
		}
	}
	else if  (LOBYTE(LOWORD(winVer))< 4)
		ShellType = Windows32s;
	else
	{
		ShellType = Windows95;
		osvi = (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi != NULL)
		{
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);

			if(osvi->dwMajorVersion == 4L && osvi->dwMinorVersion == 10L)
				ShellType = Windows98;
			else 
				if(osvi->dwMajorVersion == 4L && osvi->dwMinorVersion == 90L)
					ShellType = WindowsME;

			free(osvi);
		}
	}

	if(pbstrSysType)
	{
		USES_CONVERSION;
		switch(ShellType)
		{
			case Windows32s:
				*pbstrSysType = SysAllocString(A2W("Windows32s"));
				break;
			case WindowsNT3:
				*pbstrSysType = SysAllocString(A2W("WindowsNT3"));
				break;
			case Windows95:
				*pbstrSysType = SysAllocString(A2W("Windows95"));
				break;
			case Windows98:
				*pbstrSysType = SysAllocString(A2W("Windows98"));
				break;
			case WindowsME:
				*pbstrSysType = SysAllocString(A2W("WindowsME"));
				break;
			case WindowsNT4:
				*pbstrSysType = SysAllocString(A2W("WindowsNT4"));
				break;
			case Windows2000:
				*pbstrSysType = SysAllocString(A2W("Windows2000"));
				break;
			case WindowsXP:
				*pbstrSysType = SysAllocString(A2W("WindowsXP"));
				break;
			case WindowsVista:
				*pbstrSysType = SysAllocString(A2W("WindowsVista"));
				break;
			case Windows7:
				*pbstrSysType = SysAllocString(A2W("Windows7"));
				break;
			case Windows8:
				*pbstrSysType = SysAllocString(A2W("Windows8"));
				break;
			case Windows81:
				*pbstrSysType = SysAllocString(A2W("Windows8.1"));
				break;
			case Windows10:
				*pbstrSysType = SysAllocString(A2W("Windows10"));
				break;
			default:
				*pbstrSysType = SysAllocString(A2W("Unknown"));
		}
	}

	return S_OK;
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Windows9x下CPU使用率
STDMETHODIMP CCpuUsage::CpuUsage9x(long *plResult)
{
	HKEY hkey;
    DWORD dwDataSize;
    DWORD dwType;
    DWORD dwCpuUsage;

    // 启动计数
    if ( RegOpenKeyEx( HKEY_DYN_DATA,
                       "PerfStats\\StartStat",
                       0,KEY_ALL_ACCESS,
                       &hkey ) != ERROR_SUCCESS)
       return S_FALSE;

    dwDataSize=sizeof(DWORD);
    RegQueryValueEx( hkey,
                     "KERNEL\\CPUUsage",
                     NULL,&dwType,
                     (LPBYTE)&dwCpuUsage,
                     &dwDataSize );

    RegCloseKey(hkey);
    
    // 得到当前计数值
    if ( RegOpenKeyEx( HKEY_DYN_DATA,
                       "PerfStats\\StatData",
                       0,KEY_READ,
                       &hkey ) != ERROR_SUCCESS)
       return S_FALSE;

    dwDataSize=sizeof(DWORD);
    RegQueryValueEx( hkey,
                         "KERNEL\\CPUUsage",
                         NULL,&dwType,
                         (LPBYTE)&dwCpuUsage,
                         &dwDataSize );
    *plResult = dwCpuUsage;
    
    RegCloseKey(hkey);
/*    
    // 停止计数
    if ( RegOpenKeyEx( HKEY_DYN_DATA,
                       "PerfStats\\StopStat",
                       0,KEY_ALL_ACCESS,
                       &hkey ) != ERROR_SUCCESS)
       return S_FALSE;

    dwDataSize=sizeof(DWORD);
    RegQueryValueEx( hkey,
                     "KERNEL\\CPUUsage",
                     NULL,&dwType,
                     (LPBYTE)&dwCpuUsage,
                     &dwDataSize );

    RegCloseKey(hkey);
*/
	return S_OK;
}
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// WindowsNT/2000下CPU使用率
#define SystemBasicInformation       0
#define SystemPerformanceInformation 2
#define SystemTimeInformation        3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

typedef struct
{
    DWORD   dwUnknown1;
    ULONG   uKeMaximumIncrement;
    ULONG   uPageSize;
    ULONG   uMmNumberOfPhysicalPages;
    ULONG   uMmLowestPhysicalPage;
    ULONG   uMmHighestPhysicalPage;
    ULONG   uAllocationGranularity;
    PVOID   pLowestUserAddress;
    PVOID   pMmHighestUserAddress;
    ULONG   uKeActiveProcessors;
    BYTE    bKeNumberProcessors;
    BYTE    bUnknown2;
    WORD    wUnknown3;
} SYSTEM_BASIC_INFORMATION;

typedef struct
{
    LARGE_INTEGER   liIdleTime;
    DWORD           dwSpare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG         uCurrentTimeZoneId;
    DWORD         dwReserved;
} SYSTEM_TIME_INFORMATION;


// ntdll!NtQuerySystemInformation (NT specific!)
//
// The function copies the system information of the
// specified type into a buffer
//
// NTSYSAPI
// NTSTATUS
// NTAPI
// NtQuerySystemInformation(
//    IN UINT SystemInformationClass,    // information type
//    OUT PVOID SystemInformation,       // pointer to buffer
//    IN ULONG SystemInformationLength,  // buffer size in bytes
//    OUT PULONG ReturnLength OPTIONAL   // pointer to a 32-bit
//                                       // variable that receives
//                                       // the number of bytes
//                                       // written to the buffer 
// );
typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);

PROCNTQSI NtQuerySystemInformation;

STDMETHODIMP CCpuUsage::CpuUsage2000(long *plResult)
{
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
    SYSTEM_TIME_INFORMATION        SysTimeInfo;
    SYSTEM_BASIC_INFORMATION       SysBaseInfo;
    double                         dbIdleTime;
    double                         dbSystemTime;
    LONG                           status;
	static LARGE_INTEGER           liOldIdleTime = {0,0};
    static LARGE_INTEGER           liOldSystemTime = {0,0};

    NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(
                                          GetModuleHandle("ntdll"),
                                         "NtQuerySystemInformation"
                                         );

    if (!NtQuerySystemInformation)
        return S_FALSE;

    // 得到系统中处理器数
    status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL);
    if (status != NO_ERROR)
        return S_FALSE;
    
    // 得到新的系统时间
    status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0);
    if (status!=NO_ERROR)
        return S_FALSE;

    // 得到新的CPU空闲时间
    status = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL);
    if (status != NO_ERROR)
        return S_FALSE;

    // 如果第一次调用跳过
    if (liOldIdleTime.QuadPart != 0)
    {
        // 当前值 = 最新值 - 旧值
        dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime);
        dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);

        // 当前CPU空闲时间 = 空闲时间 / 系统时间
        dbIdleTime = dbIdleTime / dbSystemTime;

        // 当前CPU利用率% = 100 - (当前CPU空闲时间 * 100) / 处理器数
        dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;

        *plResult = (UINT)dbIdleTime;
    }

    // 存储新的CPU空闲及系统时间
    liOldIdleTime = SysPerfInfo.liIdleTime;
    liOldSystemTime = SysTimeInfo.liKeSystemTime;

	return S_OK;
}
// ------------------------------------------------------------------------------------------------