#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "pmdll.h"


int main()
{
	BOOL b=LoadPhyMemDriver();
	//BYTE bb=LoadPhyMemDriverMY();printf("Num=  %d\n",bb);
	
	if (b==FALSE)
	{
		printf("load RwDrv.sys failed\n");
		exit(-1);
	}


	int bus=0;
	int dev=0;
	int func=0;
	DWORD dev_ven[2];
	//bus,dev,func,offset
	ReadPCIDW(bus, dev, func, 0, &dev_ven);
	//printf("Bus %d, Device %d, Function %d, Vendor ID 0x%x, Device ID 0x%x\n",	bus, dev, func, LOWORD(dev_ven[0]), HIWORD(dev_ven[0]));
	printf("Bus %d, Device %d, Function %d, Vendor ID 0x%x, Device ID 0x%x\n",	bus, dev, func, LOWORD(dev_ven[1]), HIWORD(dev_ven[1]));
	//WritePCIDW(bus, dev, func, 0, 0x12345678);

	//getchar();
	UnloadPhyMemDriver();
	return 0;
}
