#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_i2c.h"
#include "bno055.h"
#include "accel.h"
/*
//uint8_t Mscale 	= MFS_4Gauss;		// Select magnetometer full-scale resolution
uint8_t MOpMode 	= EnhancedRegular;    	// Select magnetometer perfomance mode
uint8_t MPwrMode 	= Normal;    		// Select magnetometer power mode
uint8_t Modr 			= MODR_30Hz;    // Select magnetometer ODR when in BNO055 bypass mode
//
uint8_t GPwrMode 	= NormalG;   		// Gyro power mode
uint8_t Gscale 		= GFS_2000DPS; 	// Gyro full scale
//uint8_t Godr	 	= GODR_250Hz;  	// Gyro sample rate
uint8_t Gbw 			= GBW_230Hz;    // Gyro bandwidth
*/


// Choosing working mode for accelerometer
uint8_t Ascale 		= AFS_4G;      // Accel full scale
uint8_t APwrMode 	= NormalA;   		// Accel power mode
uint8_t Abw 		= ABW_62_5Hz;    // Accel bandwidth, accel sample rate divided by ABW_divx
//
uint8_t PWRMode 	= BNO055_POWER_NORMAL;  	// Select BNO055 power mode
uint8_t OPRMode 	= BNO055_MODE_ACCONLY;    	// specify operation mode for sensors [ACCONLY|MAGONLY|GYROONLY|ACCMAG|ACCGYRO|MAGGYRO|AMG|NDOF|NDOF_FMC_OFF]


uint8_t status;               // BNO055 data status register
float aRes, gRes, mRes; 			// scale resolutions per LSB for the sensors

// IMU calibration variables
uint8_t cal_sys 	= 0;
uint8_t cal_gyro 	= 0;
uint8_t cal_acc 	= 0;
uint8_t cal_mag 	= 0;
uint8_t cal_imu 	= 0;

const uint8_t num_of_bytes_read = 18;		// Read number of bytes from IMU (24 for ACCGYRO; 38 for NDOF)

const char read_devid[] 	= {START_BYTE, REG_READ, BNO055_REG_CHIP_ID, 0x01};
//const char read_acc[] 		= {REG_READ, BNO055_ACC_DATA_X_LSB, num_of_bytes_read};
const char read_calib[2] 	= {REG_READ, BNO055_REG_CALIB_STAT};
const char reset_sensor[3]	= {REG_WRITE, BNO055_REG_SYS_TRIGGER, 0x01 << 5};
uint8_t get_readings[1] 	= {BNO055_REG_ACC_DATA_X_LSB};


// Configure BNO sensor
void BNO055_Init_I2C(I2C_HandleTypeDef* hi2c_device) {
	// Select BNO055 config mode
	uint8_t opr_config_mode[2] = {BNO055_REG_OPR_MODE, BNO055_MODE_CONFIG};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, opr_config_mode, sizeof(opr_config_mode), 10);
	HAL_Delay(10);

	// Select page 1 to configure sensors
	uint8_t conf_page1[2] = {BNO055_REG_PAGE_ID, 0x01};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_page1, sizeof(conf_page1), 10);
	HAL_Delay(10);

	// Configure ACC (Page 1; 0x08)
	uint8_t conf_acc[2] = {BNO055_REG_ACC_CONFIG, APwrMode << 5 | Abw << 2 | Ascale};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_acc, sizeof(conf_acc), 10);
	HAL_Delay(10);
	/*
	// Configure GYR
	uint8_t conf_gyro[2] = {BNO055_GYRO_CONFIG_0, Gbw << 3 | Gscale};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_gyro, sizeof(conf_gyro), 10);
	HAL_Delay(10);

	uint8_t conf_gyro_pwr[2] = {BNO055_GYRO_CONFIG_1, GPwrMode};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_gyro_pwr, sizeof(conf_gyro_pwr), 10);
	HAL_Delay(10);

	// Configure MAG
	uint8_t conf_mag_pwr[4] = {REG_WRITE, BNO055_MAG_CONFIG, 0x01, MPwrMode << 5 | MOpMode << 3 | Modr};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_mag_pwr, sizeof(conf_mag_pwr), 10);
	HAL_Delay(10);
	*/
	// Select BNO055 gyro temperature source 
	//PutHexString(START_BYTE, BNO055_TEMP_SOURCE, 0x01 );
	
	// Select page 0
	uint8_t conf_page0[2] = {BNO055_REG_PAGE_ID, 0x00};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, conf_page0, sizeof(conf_page0), 10);
	HAL_Delay(10);

	// Select BNO055 sensor units (Page 0; 0x3B, default value 0x80)
	/*- ORIENTATION_MODE		 - Android					(default)
		- VECTOR_ACCELEROMETER - m/s^2  					(default)
		- VECTOR_MAGNETOMETER  - uT								(default)
		- VECTOR_GYROSCOPE     - rad/s        v		(must be configured)
		- VECTOR_EULER         - degrees					(default)
		- VECTOR_LINEARACCEL   - m/s^2        v		(default)
		- VECTOR_GRAVITY       - m/s^2						(default)
	*/
	//const char conf_units[4] = {REG_WRITE, BNO055_UNIT_SEL, 0x01, 0x82};
	//SendAccelData(USART1, (uint8_t*)conf_units);
	//HAL_Delay(50);

	// Select BNO055 system power mode (Page 0; 0x3E)
	uint8_t pwr_pwrmode[2] = {BNO055_REG_PWR_MODE, PWRMode};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, pwr_pwrmode, sizeof(pwr_pwrmode), 10);
	HAL_Delay(10);

	// Select BNO055 system operation mode (Page 0; 0x3D)
	uint8_t opr_oprmode[2] = {BNO055_REG_OPR_MODE, OPRMode};
	HAL_I2C_Master_Transmit(hi2c_device, BNO055_I2C_ADDR_HI<<1, opr_oprmode, sizeof(opr_oprmode), 10);
	HAL_Delay(50);
}

