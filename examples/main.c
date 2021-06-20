#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <signal.h>     //signal()
#include <string.h>

#include "DEV_Config.h"
#include <time.h>
#include "Waveshare_EMC2301.h"

void  EMC2301_Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_ModuleExit();

    exit(0);
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("use 'rpm' to get fan speed\n");
		printf("use 'set' to set fan speed (in steps 0-255)\n");
		exit(0);
	}

    // Exception handling:ctrl + c
    signal(SIGINT, EMC2301_Handler);
    DEV_ModuleInit();
    DEV_I2C_Init(EMC2301_ADDRESS);
	int product_ID = I2C_Read_Byte(PRODUCT_ID);
	int Manufacturer_ID = I2C_Read_Byte(MANUFACTURER_ID);
	if(product_ID == 0x37 && Manufacturer_ID == 0x5D)
	{
		printf("I2C ok !\r\n");
		EMC2301_start();

		if(0 == strcmp(argv[1], "rpm"))
		{
			EMC2301_fetchFanSpeed();
			exit(0);
		}

		char *a = argv[2];
		int num = atoi(a);

		if(0 == strcmp(argv[1], "set"))
		{
			printf("Setting fan to: %s\n", a);
			EMC2301_Directspeedcontrol(num);//0~255
		}

		 {
			 EMC2301_fetchFanSpeed();
			 DEV_Delay_ms(500);
		 }
	
	}
	else
	{
		printf("I2C Error !");
	}
	

	//System Exit
	DEV_ModuleExit();
	return 0;
	
}

