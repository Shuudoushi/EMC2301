#ifndef Waveshare_EMC2301_h
#define Waveshare_EMC2301_h

#include "DEV_Config.h"

#define EMC2301_ADDRESS        0x2F
#define TACHO_FREQUENCY      32768
#define EDGES               5 //(typically 5 for a 2 pole fan)

#define true  1

  //REGISTER LIST
  #define EMC2301_PWMBASEFREQ           0x2D
  #define EMC2301_FANSETTING            0x30
  #define EMC2301_PWMDIVIDE              0x31
  #define EMC2301_FANCONFIG1             0x32
  #define EMC2301_FANCONFIG2             0x33
  #define EMC2301_FANSPINUP              0x36
  #define EMC2301_FANMAXSTEP             0x37
  #define EMC2301_FANMINDRIVE            0x38
  #define EMC2301_FANVALTACHCOUNT        0x39
  #define EMC2301_TACHTARGETLSB          0x3C
  #define EMC2301_TACHTARGETMSB          0x3D
  #define EMC2301_TACHREADMSB            0x3E
  #define EMC2301_TACHREADLSB            0x3F
  #define PRODUCT_ID                     0xFD
  #define MANUFACTURER_ID                0xFE
  //PWM base freq
  #define EMC2301_PWMBASEFREQ_26KHZ    0x00
  #define EMC2301_PWMBASEFREQ_19KHZ    0x01
  #define EMC2301_PWMBASEFREQ_4KHZ     0x02
  #define EMC2301_PWMBASEFREQ_2KHZ     0x03

  // EMC2301_FANCONFIG1
  #define EMC2301_FANCONFIG1_RPMCONTROL      0x80
  #define EMC2301_FANCONFIG1_MINRPM_CLEAR      (~0x60)
  #define EMC2301_FANCONFIG1_MINRPM_500      0x00
  #define EMC2301_FANCONFIG1_MINRPM_1000     0x20
  #define EMC2301_FANCONFIG1_MINRPM_2000     0x40
  #define EMC2301_FANCONFIG1_MINRPM_4000     0x60
  #define EMC2301_FANCONFIG1_FANPOLES_CLEAR    (~0x18)
  #define EMC2301_FANCONFIG1_FANPOLES_1      0x00
  #define EMC2301_FANCONFIG1_FANPOLES_2      0x08
  #define EMC2301_FANCONFIG1_FANPOLES_3      0x10
  #define EMC2301_FANCONFIG1_FANPOLES_4      0x18
  #define EMC2301_FANCONFIG1_UPDATE_CLEAR      (~0x07)
  #define EMC2301_FANCONFIG1_UPDATE_100      0x00
  #define EMC2301_FANCONFIG1_UPDATE_200      0x01
  #define EMC2301_FANCONFIG1_UPDATE_300      0x02
  #define EMC2301_FANCONFIG1_UPDATE_400      0x03
  #define EMC2301_FANCONFIG1_UPDATE_500      0x04
  #define EMC2301_FANCONFIG1_UPDATE_800      0x05
  #define EMC2301_FANCONFIG1_UPDATE_1200     0x06
  #define EMC2301_FANCONFIG1_UPDATE_1600     0x07

  // EMC2301_FANCONFIG2
  #define EMC2301_FANCONFIG2_RAMPCONTROL     0x40
  #define EMC2301_FANCONFIG2_GLITCHFILTER    0x20
  #define EMC2301_FANCONFIG2_DEROPT_CLEAR      (~0x18)
  #define EMC2301_FANCONFIG2_DEROPT_NONE     0x00
  #define EMC2301_FANCONFIG2_DEROPT_BASIC    0x08
  #define EMC2301_FANCONFIG2_DEROPT_STEP     0x10
  #define EMC2301_FANCONFIG2_DEROPT_BOTH     0x18
  #define EMC2301_FANCONFIG2_ERRRANGE_CLEAR    (~0x06)
  #define EMC2301_FANCONFIG2_ERRRANGE_0      0x00
  #define EMC2301_FANCONFIG2_ERRRANGE_50     0x02
  #define EMC2301_FANCONFIG2_ERRRANGE_100    0x04
  #define EMC2301_FANCONFIG2_ERRRANGE_200    0x06

  // EMC2301_FANSPINUP
  #define EMC2301_FANSPINUP_NOKICK             0x20
  #define EMC2301_FANSPINUP_SPINLVL_CLEAR        (~0x1C)
  #define EMC2301_FANSPINUP_SPINLVL_30         0x00
  #define EMC2301_FANSPINUP_SPINLVL_35         0x04
  #define EMC2301_FANSPINUP_SPINLVL_40         0x08
  #define EMC2301_FANSPINUP_SPINLVL_45         0x0C
  #define EMC2301_FANSPINUP_SPINLVL_50         0x10
  #define EMC2301_FANSPINUP_SPINLVL_55         0x14
  #define EMC2301_FANSPINUP_SPINLVL_60         0x18
  #define EMC2301_FANSPINUP_SPINLVL_65         0x1C
  #define EMC2301_FANSPINUP_SPINUPTIME_CLEAR     (~0x03)
  #define EMC2301_FANSPINUP_SPINUPTIME_250     0x00
  #define EMC2301_FANSPINUP_SPINUPTIME_500     0x01
  #define EMC2301_FANSPINUP_SPINUPTIME_1000    0x02
  #define EMC2301_FANSPINUP_SPINUPTIME_2000    0x03
  
  // EMC2301_REG_FANMAXSTEP
  #define EMC2301_FANMAXSTEP_MAX              0b00111111 

  uint8_t MIN_RPM_MULTIPLIER;
  uint8_t MULTIPLIER;
  uint8_t POLES;
  uint16_t TARGET_TACHCOUNT;
  int FAN_SPEED;
  
/***************************************************************************/
void  EMC2301_setPWMFrequencyBase(double frequencyKHz);
void  EMC2301_setPWMFrequencyDivider(uint8_t divisor);
void  EMC2301_toggleControlAlgorithm(uint8_t enable);
void  EMC2301_setTachMinRPM(uint16_t minRPM);
uint8_t  EMC2301_setFanPoles(uint8_t poleCount);
void  EMC2301_setDriveUpdatePeriod(uint16_t periodMs);
void  EMC2301_toggleRampControl(int enable);
void  EMC2301_toggleGlitchFilter(int enable);
uint8_t  EMC2301_setDerivativeMode(uint8_t modeType);
void  EMC2301_setControlErrRange(uint8_t errorRangeRPM);
void  EMC2301_toggleSpinUpMax(int enable);
void  EMC2301_setSpinUpDrive(uint8_t drivePercent);
void  EMC2301_setSpinUpTime(uint16_t timeMs);
void  EMC2301_setControlMaxStep(uint8_t stepSize);
void  EMC2301_setFanMinDrive(double minDrivePercent);
void  EMC2301_setMinValidRPM(uint16_t minRPM);
void  EMC2301_setRPMTarget(uint16_t targetRPM);//
void  EMC2301_toggleFan(int enable);
uint16_t  EMC2301_fetchFanSpeed();

void EMC2301_writeTachoTarget(uint16_t tachoTarget);
void EMC2301_start();
void EMC2301_RPMEnable();
void EMC2301_Directspeedcontrol(UBYTE value);
/***************************************************************************/

  
  #endif
