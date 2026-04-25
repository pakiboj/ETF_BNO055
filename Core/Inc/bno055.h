#ifndef BNO055_H
#define BNO055_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* PAGE 0 REGISTER ADRESSES*/
#define BNO055_REG_CHIP_ID          0x00
#define BNO055_REG_ACC_ID           0x01
#define BNO055_MAG_ID           0x02    // value: 0x32
#define BNO055_GYRO_ID          0x03    // value: 0x0F
#define BNO055_SW_REV_ID_LSB    0x04    // value: 0x08
#define BNO055_SW_REV_ID_MSB    0x05		// value: 0x03
#define BNO055_BL_REV_ID        0x06		// N/A
#define BNO055_REG_PAGE_ID 			0x07

#define BNO055_REG_ACC_DATA_X_LSB   0x08
#define BNO055_REG_ACC_DATA_X_MSB   0x09
#define BNO055_REG_ACC_DATA_Y_LSB   0x0A
#define BNO055_REG_ACC_DATA_Y_MSB   0x0B
#define BNO055_REG_ACC_DATA_Z_LSB   0x0C
#define BNO055_REG_ACC_DATA_Z_MSB   0x0D
#define BNO055_MAG_DATA_X_LSB   0x0E
#define BNO055_MAG_DATA_X_MSB   0x0F
#define BNO055_MAG_DATA_Y_LSB   0x10
#define BNO055_MAG_DATA_Y_MSB   0x11
#define BNO055_MAG_DATA_Z_LSB   0x12
#define BNO055_MAG_DATA_Z_MSB   0x13
#define BNO055_GYR_DATA_X_LSB   0x14
#define BNO055_GYR_DATA_X_MSB   0x15
#define BNO055_GYR_DATA_Y_LSB   0x16
#define BNO055_GYR_DATA_Y_MSB   0x17
#define BNO055_GYR_DATA_Z_LSB   0x18
#define BNO055_GYR_DATA_Z_MSB   0x19
#define BNO055_EUL_HEADING_LSB  0x1A
#define BNO055_EUL_HEADING_MSB  0x1B
#define BNO055_EUL_ROLL_LSB     0x1C
#define BNO055_EUL_ROLL_MSB     0x1D
#define BNO055_EUL_PITCH_LSB    0x1E
#define BNO055_EUL_PITCH_MSB    0x1F
#define BNO055_QUA_DATA_W_LSB   0x20
#define BNO055_QUA_DATA_W_MSB   0x21
#define BNO055_QUA_DATA_X_LSB   0x22
#define BNO055_QUA_DATA_X_MSB   0x23
#define BNO055_QUA_DATA_Y_LSB   0x24
#define BNO055_QUA_DATA_Y_MSB   0x25
#define BNO055_QUA_DATA_Z_LSB   0x26
#define BNO055_QUA_DATA_Z_MSB   0x27
#define BNO055_LIA_DATA_X_LSB   0x28
#define BNO055_LIA_DATA_X_MSB   0x29
#define BNO055_LIA_DATA_Y_LSB   0x2A
#define BNO055_LIA_DATA_Y_MSB   0x2B
#define BNO055_LIA_DATA_Z_LSB   0x2C
#define BNO055_LIA_DATA_Z_MSB   0x2D
#define BNO055_GRV_DATA_X_LSB   0x2E
#define BNO055_GRV_DATA_X_MSB   0x2F
#define BNO055_GRV_DATA_Y_LSB   0x30
#define BNO055_GRV_DATA_Y_MSB   0x31
#define BNO055_GRV_DATA_Z_LSB   0x32
#define BNO055_GRV_DATA_Z_MSB   0x33
#define BNO055_TEMP             0x34
#define BNO055_REG_CALIB_STAT       0x35
#define BNO055_REG_ST_RESULT        0x36
#define BNO055_REG_INT_STA       	0x37
#define BNO055_REG_SUS_CLK_STATUS   0x38
#define BNO055_SYS_STATUS       0x39
#define BNO055_REG_SYS_ERR         	0x3A
#define BNO055_REG_UNIT_SEL       	0x3B

#define BNO055_REG_OPR_MODE         0x3D
#define BNO055_REG_PWR_MODE         0x3E
#define BNO055_REG_SYS_TRIGGER      0x3F
#define BNO055_TEMP_SOURCE      0x40
#define BNO055_REG_MAP_CONFIG       0x41
#define BNO055_REG_MAP_SIGN        	0x42

#define BNO055_REG_SIC_MATRIX_LSB0  0x43
#define BNO055_REG_SIC_MATRIX_MSB0  0x44
#define BNO055_REG_SIC_MATRIX_LSB1  0x45
#define BNO055_REG_SIC_MATRIX_MSB1  0x46
#define BNO055_REG_SIC_MATRIX_LSB2  0x47
#define BNO055_REG_SIC_MATRIX_MSB2  0x48
#define BNO055_REG_SIC_MATRIX_LSB3  0x49
#define BNO055_REG_SIC_MATRIX_MSB3  0x4A
#define BNO055_REG_SIC_MATRIX_LSB4  0x4B
#define BNO055_REG_SIC_MATRIX_MSB4  0x4C
#define BNO055_REG_SIC_MATRIX_LSB5  0x4D
#define BNO055_REG_SIC_MATRIX_MSB5  0x4E
#define BNO055_REG_SIC_MATRIX_LSB6  0x4F
#define BNO055_REG_SIC_MATRIX_MSB6  0x50
#define BNO055_REG_SIC_MATRIX_LSB7  0x51
#define BNO055_REG_SIC_MATRIX_MSB7  0x52
#define BNO055_REG_SIC_MATRIX_LSB8  0x53
#define BNO055_REG_SIC_MATRIX_MSB8  0x54

#define BNO055_REG_ACC_OFFSET_X_LSB  0x55
#define BNO055_REG_ACC_OFFSET_X_MSB  0x56
#define BNO055_REG_ACC_OFFSET_Y_LSB  0x57
#define BNO055_REG_ACC_OFFSET_Y_MSB  0x58
#define BNO055_REG_ACC_OFFSET_Z_LSB  0x59
#define BNO055_REG_ACC_OFFSET_Z_MSB  0x5A
#define BNO055_MAG_OFFSET_X_LSB 0x5B
#define BNO055_MAG_OFFSET_X_MSB 0x5C
#define BNO055_MAG_OFFSET_Y_LSB 0x5D
#define BNO055_MAG_OFFSET_Y_MSB 0x5E
#define BNO055_MAG_OFFSET_Z_LSB 0x5F
#define BNO055_MAG_OFFSET_Z_MSB 0x60
#define BNO055_GYR_OFFSET_X_LSB 0x61
#define BNO055_GYR_OFFSET_X_MSB 0x62
#define BNO055_GYR_OFFSET_Y_LSB 0x63
#define BNO055_GYR_OFFSET_Y_MSB 0x64
#define BNO055_GYR_OFFSET_Z_LSB 0x65
#define BNO055_GYR_OFFSET_Z_MSB 0x66
#define BNO055_REG_ACC_RADIUS_LSB  0x67
#define BNO055_REG_ACC_RADIUS_MSB  0x68
#define BNO055_MAG_RADIUS_LSB   0x69
#define BNO055_MAG_RADIUS_MSB   0x6A
/* PAGE 1 REGISTER ADRESSES*/
#define BNO055_REG_ACC_CONFIG            0x08
#define BNO055_MAG_CONFIG       0x09
#define BNO055_GYRO_CONFIG_0    0x0A
#define BNO055_GYRO_CONFIG_1    0x0B
#define BNO055_REG_ACC_SLEEP_CONFIG  	 0x0C
#define BNO055_GYR_SLEEP_CONFIG 0x0D
#define BNO055_REG_INT_MSK				 0x0F
#define BNO055_REG_INT_EN				 0x10
#define BNO055_REG_ACC_AM_THRES		     0x11
#define BNO055_REG_ACC_INT_SETTINGS		 0x12
#define BNO055_REG_ACC_HG_DURATION		 0x13
#define BNO055_REG_ACC_HG_THRES		     0x14
#define BNO055_REG_ACC_NM_THRES		     0x15
#define BNO055_REG_ACC_NM_SET		     0x16
#define BNO055_GYR_INT_SETTINGS 0x17
#define BNO055_GYR_HR_X_SET     0x18
#define BNO055_GYR_DUR_X        0x19
#define BNO055_GYR_HR_Y_SET     0x1A
#define BNO055_GYR_DUR_Y        0x1B
#define BNO055_GYR_HR_Z_SET     0x1C
#define BNO055_GYR_DUR_Z        0x1D
#define BNO055_GYR_AM_THRESH    0x1E
#define BNO055_GYR_AM_SET       0x1F

/* =========================================
 * I2C config
 * ========================================= */

#define START_BYTE							0xAA
#define RESPONSE_BYTE						0xBB
#define ERROR_BYTE		  				0xEE

#define BNO055_I2C_ADDR_HI			0x29
#define BNO055_I2C_ADDR_LO			0x28

#define ERROR_WRITE_SUCCESS			0x01		// Everything worker as expected
#define ERROR_WRITE_FAIL				0x03		// Check connection, protocol settings and operation more of BNO055
#define ERROR_REGMAP_INV_ADDR		0x04		// Invalid register address
#define ERROR_REGMAP_WRITE_DIS	0x05		// Register is read-only
#define ERROR_WRONG_START_BYTE	0x06		// Check if the first byte
#define ERROR_BUS_OVERRUN_ERR		0x07		// Resend the command, BNO055 was not able to clear the receive buffer
#define ERROR_MAX_LEN_ERR				0x08		// Split the command, max fire size can be up to 128 bytes
#define ERROR_MIN_LEN_ERR				0x09		// Min length of data is less than 1
#define ERROR_RECV_CHAR_TIMEOUT	0x0A		// Decrease the waiting time between sending of two bytes of one frame

#define REG_WRITE								0x00
#define REG_READ								0x01


/* =========================================
 * POWER MODES
 * ========================================= */
typedef enum {
    BNO055_POWER_NORMAL   = 0x00,
    BNO055_POWER_LOWPOWER = 0x01,
    BNO055_POWER_SUSPEND  = 0x02,
	BNO055_POWER_INVALID = 0x03
} BNO055_PowerMode_t;

/* =========================================
 * OPERATION MODES
 * ========================================= */
typedef enum {
    BNO055_MODE_CONFIG       = 0x00,
    BNO055_MODE_ACCONLY      = 0x01,
    BNO055_MODE_MAGONLY      = 0x02,
    BNO055_MODE_GYROONLY     = 0x03,
    BNO055_MODE_ACCMAG       = 0x04,
    BNO055_MODE_ACCGYRO      = 0x05,
    BNO055_MODE_MAGGYRO      = 0x06,
    BNO055_MODE_AMG          = 0x07,
    BNO055_MODE_IMUPLUS      = 0x08,
    BNO055_MODE_COMPASS      = 0x09,
    BNO055_MODE_M4G          = 0x0A,
    BNO055_MODE_NDOF_FMC_OFF = 0x0B,
    BNO055_MODE_NDOF         = 0x0C
} BNO055_OperationMode_t;



/* =========================================
 * SENSOR CONFIGURATION (PAGE 1)
 * ========================================= */


// Set initial input parameters
typedef enum  {  // ACC Full Scale
  AFS_2G = 0x00,
  AFS_4G = 0x01,
  AFS_8G = 0x02,
  AFS_16G = 0x03
}BNO055_Acc_scale;

typedef enum  { // ACC Bandwidth
  ABW_7_81Hz = 0x00,
  ABW_15_63Hz = 0x01,
  ABW_31_25Hz = 0x02,
  ABW_62_5Hz = 0x03,
  ABW_125Hz = 0x04,
  ABW_250Hz = 0x05,
  ABW_500Hz = 0x06,
  ABW_1000Hz = 0x07    //0x07
} BNO055_Acc_BW;

typedef enum  { // ACC Pwr Mode
  NormalA = 0x00,
  SuspendA = 0x01,
  LowPower1A = 0x02,
  StandbyA = 0x03,
  LowPower2A = 0x04,
  DeepSuspendA = 0x05
} BNO055_Acc_PwrMode;

typedef enum  {  // gyro full scale
  GFS_2000DPS = 0x00,
  GFS_1000DPS = 0x01,
  GFS_500DPS = 0x02,
  GFS_250DPS = 0x03,
  GFS_125DPS = 0x04   // 0x04
} BNO055_Gyro_scale;

typedef enum  { // GYR Pwr Mode
  NormalG = 0x00,
  FastPowerUpG = 0x01,
  DeepSuspendedG = 0x02,
  SuspendG = 0x03,
  AdvancedPowerSaveG = 0x04
} BNO055_Gyro_PwrMode;

typedef enum  { // gyro bandwidth
  GBW_523Hz = 0x00,
  GBW_230Hz = 0x01,
  GBW_116Hz = 0x02,
  GBW_47Hz = 0x03,
  GBW_23Hz = 0x04,
  GBW_12Hz = 0x05,
  GBW_64Hz = 0x06,
  GBW_32Hz = 0x07
} BNO055_Gyro_BW;

typedef enum  {         // magnetometer output data rate
  MODR_2Hz = 0x00,
  MODR_6Hz = 0x01,
  MODR_8Hz = 0x02,
  MODR_10Hz = 0x03,
  MODR_15Hz = 0x04,
  MODR_20Hz = 0x05,
  MODR_25Hz = 0x06,
  MODR_30Hz = 0x07
} BNO055_Mag_odr;

typedef enum  { // MAG Op Mode
  LowPowerM = 0x00,
  RegularM = 0x01,
  EnhancedRegularM = 0x02,
  HighAccuracyM = 0x04
} BNO055_Mag_OpMode;

typedef enum  { // MAG power mode
  NormalM = 0x00,
  SleepM = 0x01,
  SuspendM = 0x02,
  ForceModeM = 0x03
} BNO055_Mag_PwrMode;

/* =========================================
 * OUTPUT DATA
 * ========================================= */

typedef enum {
    ms2 = 0x00,
    mg = 0x01
} BNO055_UNIT_acceleration;

/* Quaternion */
typedef enum {
	DPS = 0x00,
	RPS = 0x01
} BNO055_UNIT_angular;

/* Euler angles */
typedef enum {
	degrees = 0x00,
	radians = 0x01
} BNO055_UNIT_Euler;

typedef enum {
	C = 0x00,
	F = 0x01,
} BNO055_UNIT_temperature;

/* =========================================
 * SYSTEM STATUS
 * ========================================= */

typedef enum {
    BNO055_OK    = 0,
    BNO055_ERROR = 1
} BNO055_Status_t;

#endif
