#ifndef	__PMDLL_H
#define	__PMDLL_H

#ifdef	PMDLL_EXPORTS
#define	DLL_DECLARE __declspec(dllexport)
#else
#define	DLL_DECLARE __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//driver initialize
DLL_DECLARE BOOL LoadPhyMemDriver();
DLL_DECLARE BYTE LoadPhyMemDriverMY();
DLL_DECLARE VOID UnloadPhyMemDriver();

//access PCI bus
DLL_DECLARE BOOL ReadPCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, PVOID pValue);
DLL_DECLARE BOOL WritePCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, DWORD Value);

#ifdef __cplusplus
}
#endif

#endif	//__PMDLL_H