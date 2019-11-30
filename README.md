# RwDrv-helper-dll
Helper library for RwDrv.sys to access PCI bus registers

it exports:
DLL_DECLARE BOOL LoadPhyMemDriver();
DLL_DECLARE VOID UnloadPhyMemDriver();
//access PCI bus
DLL_DECLARE BOOL ReadPCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, PVOID pValue); //dword values r/w
DLL_DECLARE BOOL WritePCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, DWORD Value);
