/*****************************************************************************
* | File      	:   Waveshare_EMC2301.c
* | Author      :   Waveshare team
* | Function    :   EMC2301 driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-02-02
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"
#include "Waveshare_EMC2301.h"


int TachoRead_Hb[] = {32,64,128,256,512,1024,2048,4096};
int TachoRead_Lb[] =  {0,0,0,1,2,4,8,16};

int TachoTar_Hb[] = {4096,2048,1024,512,256,128,64,32};
int TachoTar_Lb[] = {16,8,4,2,1};
int TachoTar_Sum[] =  {128,64,32,16,8,4,2,1};

/******************************************************************************
function:	Read one byte of data to EMC2301 via I2C
parameter:  
            Addr: Register address
Info:
******************************************************************************/
static UBYTE EMC2301_Read_Byte(UBYTE Addr)
{
    return I2C_Read_Byte(Addr);
}

/******************************************************************************
function:	Send one byte of data to EMC2301 via I2C
parameter:
            Addr: Register address
           Value: Write to the value of the register
Info:
******************************************************************************/
static void EMC2301_Write_Byte(UBYTE Addr, UBYTE Value)
{
    I2C_Write_Byte(Addr, Value);
}

/******************************************************************************
function:	The data to be written is processed before written
parameter:
            Addr: Register address
            Configvalue: Clear the corresponding value after the corresponding 
		                bit of the register
			value：Write to the value of the register		
Info:
******************************************************************************/
static void EMC2301_WriteRegconfig_Byte(UBYTE Addr, uint16_t Configvalue,UBYTE Value)
{
	UBYTE data = EMC2301_Read_Byte(Addr);
	data &= Configvalue;
	data |= Value;
	EMC2301_Write_Byte(Addr,data);
}

void EMC2301_start()
{
  MIN_RPM_MULTIPLIER   = 1;
  MULTIPLIER           = 1;
  POLES           = 2;
}

void EMC2301_RPMEnable()
{
  EMC2301_Write_Byte(EMC2301_FANCONFIG2, 0x40);//Enable Ramp Rate Control	
  EMC2301_Write_Byte(EMC2301_FANCONFIG1, 0x80);//Enables Fan Speed Control Algorithm
}


void EMC2301_setPWMFrequencyBase(double frequencyKHz)// Set the base frequency of the PWM output
{
  uint8_t writeByte;
  if (frequencyKHz < 4.882)
  {
    writeByte = EMC2301_PWMBASEFREQ_2KHZ;
  }
  else if (frequencyKHz < 19.531)
  {
    writeByte = EMC2301_PWMBASEFREQ_4KHZ;
  }
  else if (frequencyKHz < 26)
  {
    writeByte = EMC2301_PWMBASEFREQ_19KHZ;
  }
  else
  {
    writeByte = EMC2301_PWMBASEFREQ_26KHZ;
  }
    EMC2301_Write_Byte(EMC2301_PWMBASEFREQ,writeByte);
}

void EMC2301_setPWMFrequencyDivider(uint8_t divisor)// Set the PWM frequency divisor factor
{
	EMC2301_Write_Byte(EMC2301_PWMDIVIDE,divisor);
}


void EMC2301_toggleControlAlgorithm(uint8_t enable)// Switch control algorithm
{
  if (enable == true)
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG1, ~EMC2301_FANCONFIG1_RPMCONTROL, EMC2301_FANCONFIG1_RPMCONTROL);
  }
  else
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG1, ~EMC2301_FANCONFIG1_RPMCONTROL, 0);
  }
}

void EMC2301_setTachMinRPM(uint16_t minRPM)// Set the minimum RPM
{
  uint8_t writeByte;
  if (minRPM < 1000)
  {
    writeByte = EMC2301_FANCONFIG1_MINRPM_500;
    MIN_RPM_MULTIPLIER = 1;
  }
  else if (minRPM < 2000)
  {
    writeByte = EMC2301_FANCONFIG1_MINRPM_1000;
    MIN_RPM_MULTIPLIER = 2;
  }
  else if (minRPM < 4000)
  {
    writeByte = EMC2301_FANCONFIG1_MINRPM_2000;
    MIN_RPM_MULTIPLIER = 4;
  }
  else
  {
    writeByte = EMC2301_FANCONFIG1_MINRPM_4000;
    MIN_RPM_MULTIPLIER = 8;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG1, EMC2301_FANCONFIG1_MINRPM_CLEAR, writeByte);
}

uint8_t EMC2301_setFanPoles(uint8_t poleCount)// Set the number of fan poles
{
  uint8_t writeByte;
  switch (poleCount)
  {
  case 1:
    writeByte = EMC2301_FANCONFIG1_FANPOLES_1;
    MULTIPLIER = 1;
    break;
  case 2:
    writeByte = EMC2301_FANCONFIG1_FANPOLES_2;
    MULTIPLIER = 2;
    break;
  case 3:
    writeByte = EMC2301_FANCONFIG1_FANPOLES_3;
    MULTIPLIER = 3;
    break;
  case 4:
    writeByte = EMC2301_FANCONFIG1_FANPOLES_4;
    MULTIPLIER = 4;
  default:
    return 0;
    break;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG1, EMC2301_FANCONFIG1_FANPOLES_CLEAR, writeByte);
}

void EMC2301_setDriveUpdatePeriod(uint16_t periodMs)// Set the driver update cycle
{
  uint8_t writeByte;
  if (periodMs < 200)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_100;
  }
  else if (periodMs < 300)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_200;
  }
  else if (periodMs < 400)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_300;
  }
  else if (periodMs < 500)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_400;
  }
  else if (periodMs < 800)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_500;
  }
  else if (periodMs < 1200)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_800;
  }
  else if (periodMs < 1600)
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_1200;
  }
  else
  {
    writeByte = EMC2301_FANCONFIG1_UPDATE_1600;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG1, EMC2301_FANCONFIG1_UPDATE_CLEAR, writeByte);
}


void EMC2301_toggleRampControl(int enable)// Switch ramp control to direct control
{
  if (enable == true)
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, ~EMC2301_FANCONFIG2_RAMPCONTROL, EMC2301_FANCONFIG2_RAMPCONTROL);
  }
  else
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, ~EMC2301_FANCONFIG2_RAMPCONTROL, 0);
  }
}

void EMC2301_toggleGlitchFilter(int enable)// When you need to eliminate the high frequency noise on the Tach pin, switch to the fault filter
{
  if (enable == true)
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, ~EMC2301_FANCONFIG2_GLITCHFILTER, EMC2301_FANCONFIG2_GLITCHFILTER);
  }
  else
  {
    EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, ~EMC2301_FANCONFIG2_GLITCHFILTER, 0);
  }
}

uint8_t EMC2301_setDerivativeMode(uint8_t modeType)// Change the derivative type used in the RPM based speed control PID algorithm
{
  uint8_t writeByte;

  switch (modeType)
  {
  case 0:
    writeByte = EMC2301_FANCONFIG2_DEROPT_NONE;
    break;
  case 1:
    writeByte = EMC2301_FANCONFIG2_DEROPT_BASIC;
    break;
  case 2:
    writeByte = EMC2301_FANCONFIG2_DEROPT_STEP;
    break;
  case 3:
    writeByte = EMC2301_FANCONFIG2_DEROPT_BOTH;
  default:
    return 0;
    break;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, EMC2301_FANCONFIG2_DEROPT_CLEAR, writeByte);
}


void EMC2301_setControlErrRange(uint8_t errorRangeRPM)// Set the stall tolerance
{
  uint8_t writeByte;
  if (errorRangeRPM < 0.01) // Account for doubles sometimes not being exactly 0
  {
    writeByte = EMC2301_FANCONFIG2_ERRRANGE_0;
  }
  else if (errorRangeRPM <= 50)
  {
    writeByte = EMC2301_FANCONFIG2_ERRRANGE_50;
  }
  else if (errorRangeRPM <= 100)
  {
    writeByte = EMC2301_FANCONFIG2_ERRRANGE_100;
  }
  else
  {
    writeByte = EMC2301_FANCONFIG2_ERRRANGE_200;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANCONFIG2, EMC2301_FANCONFIG2_ERRRANGE_CLEAR, writeByte);
}

void EMC2301_toggleSpinUpMax(int enable)// Turn on maximum rotation acceleration and have the fan set to 100% duty cycle 1/4 of the time the program starts
{
  if (enable == true)
  {
     EMC2301_WriteRegconfig_Byte(EMC2301_FANSPINUP, ~EMC2301_FANSPINUP_NOKICK, EMC2301_FANSPINUP_NOKICK);
  }
  else
  {
     EMC2301_WriteRegconfig_Byte(EMC2301_FANSPINUP, ~EMC2301_FANSPINUP_NOKICK, 0);
  }
}

void EMC2301_setSpinUpDrive(uint8_t drivePercent)// Sets the level of drive that should be used during startup
{
  uint8_t writeByte;
  if (drivePercent < 35)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_30;
  }
  else if (drivePercent < 40)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_35;
  }
  else if (drivePercent < 45)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_40;
  }
  else if (drivePercent < 50)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_45;
  }
  else if (drivePercent < 55)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_50;
  }
  else if (drivePercent < 60)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_55;
  }
  else if (drivePercent < 65)
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_60;
  }
  else
  {
    writeByte = EMC2301_FANSPINUP_SPINLVL_65;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANSPINUP, EMC2301_FANSPINUP_SPINLVL_CLEAR, writeByte);
}

void EMC2301_setSpinUpTime(uint16_t timeMs)// Set the duration of the spinner
{
  uint8_t writeByte;
  if (timeMs < 500)
  {
    writeByte = EMC2301_FANSPINUP_SPINUPTIME_250;
  }
  else if (timeMs < 1000)
  {
    writeByte = EMC2301_FANSPINUP_SPINUPTIME_500;
  }
  else if (timeMs < 2000)
  {
    writeByte = EMC2301_FANSPINUP_SPINUPTIME_1000;
  }
  else
  {
    writeByte = EMC2301_FANSPINUP_SPINUPTIME_2000;
  }

   EMC2301_WriteRegconfig_Byte(EMC2301_FANSPINUP, EMC2301_FANSPINUP_SPINUPTIME_CLEAR, writeByte);
}

void EMC2301_setControlMaxStep(uint8_t stepSize)// Sets the maximum fan drive change that can be performed in a single update cycle
{
  if (stepSize > EMC2301_FANMAXSTEP_MAX)
  {
    stepSize = EMC2301_FANMAXSTEP_MAX;
	EMC2301_Write_Byte(EMC2301_FANMAXSTEP, stepSize);
  }
  
}

void EMC2301_setFanMinDrive(double minDrivePercent)// Set the minimum drive allowed by the RPM-based fan speed control algorithm
{
  uint8_t writeByte = (uint8_t)( 50/ 100 * 255);
  EMC2301_Write_Byte(EMC2301_FANMINDRIVE, writeByte);
}

void EMC2301_setMinValidRPM(uint16_t minRPM)// Set the minimum RPM
{
  uint16_t tachMinRPM;
  switch (MIN_RPM_MULTIPLIER)
  {
  case 1:
    tachMinRPM = 500;
    break;
  case 2:
    tachMinRPM = 1000;
    break;
  case 3:
    tachMinRPM = 2000;
    break;
  default:
  case 4:
    tachMinRPM = 4000;
    break;
  }

  if (minRPM < tachMinRPM)
  {
    minRPM = tachMinRPM;
  }
  
  uint8_t maxTachCount_ = 60 * MIN_RPM_MULTIPLIER * TACHO_FREQUENCY * (MULTIPLIER - 2) / 2 / POLES / minRPM;
  EMC2301_Write_Byte(EMC2301_FANVALTACHCOUNT, maxTachCount_);
  
}



uint16_t EMC2301_fetchFanSpeed()// Read tachometer reading
{
	unsigned char tachoCount_MSB = EMC2301_Read_Byte(EMC2301_TACHREADMSB);
	//printf("MSB addr: %x\r\n",tachoCount_MSB);
	int Res_value = 0;
	unsigned char tachread_flag = 0x01;
	for(int i=0;i<8;i++)
	{
		if(tachoCount_MSB&tachread_flag )
		{
		Res_value = Res_value+TachoRead_Hb[i];
		}
		tachread_flag = tachread_flag<<1;		
	}
	//printf("MSB Res_value: %d\r\n",Res_value);//------------------	
	int tachoCount_LSB = EMC2301_Read_Byte(EMC2301_TACHREADLSB);	
	//printf("LSB addr: %x\r\n",tachoCount_LSB);	
	tachread_flag = 0x01;
	for(int i=0;i<8;i++)
	{
		if(tachoCount_LSB&tachread_flag)
		{
			Res_value = Res_value+TachoRead_Lb[i];
		}
		tachread_flag = tachread_flag<<1;
	}
	printf("End Res_value: %d\r\n",Res_value);
	FAN_SPEED = ((EDGES-1)*MULTIPLIER*TACHO_FREQUENCY*60)/(POLES*Res_value);
	printf("FAN_SPEED: %d\r\n",FAN_SPEED);
	printf("---------------------\r\n");	
}



void EMC2301_writeTachoTarget(uint16_t tachoTarget)// Write target speed
{	
  printf("write tachoTarget: %d ------------>>>\r\n",tachoTarget);
  uint8_t tachCountLSB = 0;
  uint8_t tachCountMSB = 0;

	if(tachoTarget<=31)
	{
		
		int sum_xx = 0;
		int temp_xx = tachoTarget;
		tachCountLSB = 0;
		for(int i=0;i<5;i++)
		{
			if(temp_xx >= TachoTar_Lb[i])
			{
			sum_xx = sum_xx + TachoTar_Lb[i];
			tachCountLSB = tachCountLSB + TachoTar_Sum[i];
			temp_xx = tachoTarget - sum_xx;
			}
		}
		printf("tachCountLSB: 0x%x\r\n",tachCountLSB);
		EMC2301_Write_Byte(EMC2301_TACHTARGETLSB, tachCountLSB);
	}
	else
	{
		int sum = 0;
		int temp = tachoTarget;
		tachCountMSB = 0;
		for(int i=0;i<8;i++)
		{
			if(temp >= TachoTar_Hb[i])
			{
				sum = sum+TachoTar_Hb[i];
				tachCountMSB = tachCountMSB + TachoTar_Sum[i];
				temp = tachoTarget - sum;
			}
		}
		
		int sum_xx = 0;
		int temp_xx = tachoTarget;
		tachCountLSB = 0;
		for(int i=0;i<5;i++)
		{
			if(temp_xx >= TachoTar_Lb[i])
			{
			sum_xx = sum_xx + TachoTar_Lb[i];
			tachCountLSB = tachCountLSB + TachoTar_Sum[i];
			temp_xx = tachoTarget - sum_xx;
			}
		}
		tachCountLSB = tachCountLSB;
	}
	//printf("tachCountLSB: 0x%x\r\n",tachCountLSB);
	//printf("tachCountMSB: 0x%x\r\n",tachCountMSB);
	EMC2301_Write_Byte(EMC2301_TACHTARGETLSB, tachCountLSB);
    EMC2301_Write_Byte(EMC2301_TACHTARGETMSB, tachCountMSB);   
}

//Direct control of fan speed
//value:0~255
void EMC2301_Directspeedcontrol(UBYTE value)
{
  EMC2301_Write_Byte(EMC2301_FANCONFIG2, 0x40);
  EMC2301_Write_Byte(EMC2301_FANCONFIG1, 0x00); 
  EMC2301_Write_Byte(EMC2301_FANSETTING,value);
  DEV_Delay_ms(2000); 
}