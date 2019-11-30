/*++
RwDrv Driver
Ver: 1.0
Date: 2019/11/30
--*/

#include <windows.h>
#include <winioctl.h>
#include "pmdll.h"
#include "phymem.h"

HANDLE hDriver=INVALID_HANDLE_VALUE;

BOOL InstallDriver(PCSTR pszDriverPath, PCSTR pszDriverName);
BOOL RemoveDriver(PCSTR pszDriverName);
BOOL StartDriver(PCSTR pszDriverName);
BOOL StopDriver(PCSTR pszDriverName);

//get driver(chipsec_hlpr.sys) full path
static BOOL GetDriverPath(PSTR szDriverPath)
{
	PSTR pszSlash;

	if (!GetModuleFileName(GetModuleHandle(NULL), szDriverPath, MAX_PATH))
		return FALSE;

	pszSlash=strrchr(szDriverPath, '\\');

	if (pszSlash)
		pszSlash[1]='\0';
	else
		return FALSE;

	return TRUE;
}

BYTE LoadPhyMemDriverMY()
{
	return 77;
}
//install and start driver

BOOL LoadPhyMemDriver()
{
	BOOL bResult;
	CHAR szDriverPath[MAX_PATH];

	hDriver=CreateFile( "\\\\.\\rwdrv",
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	//If the driver is not running, install it
	if (hDriver==INVALID_HANDLE_VALUE)
	{
		GetDriverPath(szDriverPath);
		strcat(szDriverPath, "rwdrv.sys");

		bResult=InstallDriver(szDriverPath, "PHYMEM");

		if (!bResult)
			return FALSE;

		bResult=StartDriver("PHYMEM");

		if (!bResult)
			return FALSE;

		hDriver=CreateFile( "\\\\.\\rwdrv",
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

		if (hDriver==INVALID_HANDLE_VALUE)
			return FALSE;
	}

	return TRUE;
}

//stop and remove driver
VOID UnloadPhyMemDriver()
{
	if (hDriver!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hDriver);
		hDriver=INVALID_HANDLE_VALUE;
	}

	RemoveDriver("rwdrv");
}

//read pci configuration
BOOL ReadPCIDW(WORD busNum, WORD devNum, WORD funcNum,	 WORD regOff, PVOID pValue)
{
	BOOL bRet=FALSE;
	DWORD dwBytes;
	PHYMEM_IOPR pp;//8 bytes
	DWORD io_port;


	pp.io_port = 0xCF8;
	pp.value = (busNum << 16) | (devNum << 11) | (funcNum << 8) | (regOff & 0xFC) | 0x80000000;
	if (hDriver!=INVALID_HANDLE_VALUE)
	{
	  /*  HANDLE       hDevice,
		  DWORD        dwIoControlCode,
		  LPVOID       lpInBuffer,
		  DWORD        nInBufferSize,
		  LPVOID       lpOutBuffer,
		  DWORD        nOutBufferSize,
		  LPDWORD      lpBytesReturned,
		  LPOVERLAPPED lpOverlapped*/
		bRet=DeviceIoControl(hDriver, 0x222824, &pp,sizeof(PHYMEM_IOPR), pValue, 8, &dwBytes, NULL);//write dw
		pp.io_port = 0xCFC +  (regOff & 0x03);
		pp.value = 0;
		if (bRet)
		bRet=DeviceIoControl(hDriver, 0x222820, &pp,sizeof(PHYMEM_IOPR), pValue, 8, &dwBytes, NULL);//read dw
	}

	if (bRet)
		return TRUE;
	else
		return FALSE;
}

//write pci configuration
BOOL WritePCIDW(WORD busNum, WORD devNum, WORD funcNum,  WORD regOff, DWORD Value)
{
	BOOL bRet=FALSE;
	DWORD dwBytes;
	PHYMEM_IOPR pp;//8 bytes

	pp.io_port = 0xCF8;
	pp.value = (busNum << 16) | (devNum << 11) | (funcNum << 8) | (regOff & 0xFC) | 0x80000000;

	if (hDriver!=INVALID_HANDLE_VALUE)
	{
		bRet=DeviceIoControl(hDriver, 0x222824, &pp,sizeof(PHYMEM_IOPR), &pp, sizeof(PHYMEM_IOPR), &dwBytes, NULL);//write dw
		pp.io_port = 0xCFC +  (regOff & 0x03);
		pp.value = Value;
		if (bRet)
			bRet=DeviceIoControl(hDriver, 0x222824, &pp,sizeof(PHYMEM_IOPR), &pp, sizeof(PHYMEM_IOPR), &dwBytes, NULL);//write dw
	}

	if (bRet)
		return TRUE;
	else
		return FALSE;
}

