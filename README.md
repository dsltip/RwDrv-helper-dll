# RwDrv-helper-dll
Helper library for RwDrv.sys to access PCI bus registers

it exports:
DLL_DECLARE BOOL LoadPhyMemDriver();
DLL_DECLARE VOID UnloadPhyMemDriver();
//access PCI bus
DLL_DECLARE BOOL ReadPCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, PVOID pValue); //dword values r/w
DLL_DECLARE BOOL WritePCIDW(WORD busNum, WORD devNum, WORD funcNum, WORD regOff, DWORD Value);

Example:
BOOL b=LoadPhyMemDriver();
	if (b==FALSE)
	{
		printf("load RwDrv.sys failed\n");
		exit(-1);
	}
	int bus=0;
	int dev=0;
	int func=0;
	DWORD dev_ven[2];
	ReadPCIDW(bus, dev, func, 0, &dev_ven);
	printf(" Vendor ID 0x%x, Device ID 0x%x\n", LOWORD(dev_ven[1]), HIWORD(dev_ven[1]));
	//WritePCIDW(bus, dev, func, 0, 0x12345678);
	UnloadPhyMemDriver();
