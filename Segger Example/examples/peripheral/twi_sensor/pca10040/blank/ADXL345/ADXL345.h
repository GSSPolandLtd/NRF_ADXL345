#include <stdio.h>
#include "boards.h"
#include "nrf_drv_twi.h"
/* ------- Register names ------- */

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

#define ADXL345_ADDRESS 0x53
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

#define ADXL345_BW_1600 0xF // 1111
#define ADXL345_BW_800  0xE // 1110
#define ADXL345_BW_400  0xD // 1101  
#define ADXL345_BW_200  0xC // 1100
#define ADXL345_BW_100  0xB // 1011  
#define ADXL345_BW_50   0xA // 1010 
#define ADXL345_BW_25   0x9 // 1001 
#define ADXL345_BW_12   0x8 // 1000 
#define ADXL345_BW_6    0x7 // 0111
#define ADXL345_BW_3    0x6 // 0110

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

static volatile bool m_xfer_done = false;
static uint8_t m_sample[6];
static uint8_t rx_data;

/* ------- ADXL345 Methods ------- */


ret_code_t ADXL345_I2C_Normal_Mode(const nrf_drv_twi_t * p_twi_instance);
void ADXL345_I2C_Init_Measure_Mode(void);
void ADXL345_I2C_Sleep_Mode(void);
void ADXL345_I2C_Set_Format(void);
void ADXL345_I2C_Inactivity_Threshold(int val);
void ADXL345_I2C_Activity_Threshold(int val);
void ADXL345_I2C_Set_DATAX0(void);
void ADXL345_Read_Sensor_Data(void);
void ADXL345_read_XYZ(int *x, int *y, int *z);
void ADXL345_Read_Acceleration(int *xyz);
void ADXL345_Get_Acceleration(double *xyz);
void ADXL345_Check_Status(ret_code_t err_code, uint8_t rx_data, uint8_t reg, char *description);