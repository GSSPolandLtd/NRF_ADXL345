#include "ADXL345.h"
#include "app_error.h"
#include "boards.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include <stdio.h>

static const nrf_drv_twi_t *p_twi_master;

/*
  ADXL345
  WRITE TO REGISTER FUNCTION

*/
ret_code_t ADXL345_I2C_register_write(uint8_t slave_addr, uint8_t *pdata, uint32_t bytes) {
  ret_code_t ret_code;

  ret_code = nrf_drv_twi_tx(p_twi_master, slave_addr, pdata, bytes, false);

  return ret_code;
}

/*
   ADXL345 
   Enable NORMAL mode function
*/

ret_code_t ADXL345_I2C_Normal_Mode(const nrf_drv_twi_t *p_twi_instance) {
  ret_code_t err_code;
  p_twi_master = p_twi_instance;

  /* Writing to ADXL345_POWER_CTL "0" set temperature sensor in NORMAL mode. */
  uint8_t reg[2] = {ADXL345_POWER_CTL, 0};
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  ADXL345_Check_Status(err_code, rx_data, reg[1], "Normal");
  return err_code;
}

/*
  ADXL345
  Enable Sleep mode
*/
void ADXL345_I2C_Sleep_Mode(void) {
  ret_code_t err_code;

  uint8_t reg[2];

  /* Enable Sleep mode */
  reg[0] = ADXL345_POWER_CTL;
  reg[1] = 16;
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);

  ADXL345_Check_Status(err_code, rx_data, reg[1], "Sleep");
}

/*
   ADXL345 
   Enagle Measure mode 
*/

void ADXL345_I2C_Init_Measure_Mode(void) {

  ret_code_t err_code;

  uint8_t reg[2];

  //    /* Enable Measure Mode*/
  reg[0] = ADXL345_POWER_CTL;
  reg[1] = 8;
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);

  ADXL345_Check_Status(err_code, rx_data, reg[1], "Measure");
}

/*
  ADXL345 
  Enable formating mode 
*/

void ADXL345_I2C_Set_Format(void) {
  ret_code_t err_code;

   /* Set Format */
  uint8_t reg[2];
  reg[0] = ADXL345_DATA_FORMAT;
  reg[1] = 0x0B;
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);
  ADXL345_Check_Status(err_code, rx_data, reg[1], "Formating");
}

/*
  ADXL345 
  Enable Inactivity Threshold  
*/

void ADXL345_I2C_Inactivity_Threshold(int val) {
  ret_code_t err_code;
  uint8_t reg[2];
  int intActivityThreshold = constrain(val, 0, 255);
  int _b = (intActivityThreshold);
  reg[0] = ADXL345_THRESH_INACT;
  reg[1] = _b;
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);
  ADXL345_Check_Status(err_code, rx_data, reg[1], "Inactivity");
}

/*
  ADXL345 
  Enable Activity Threshold 
*/

void ADXL345_I2C_Activity_Threshold(int val) {
  ret_code_t err_code;
  uint8_t reg[2];
  int ActivityThreshold = constrain(val, 0, 255);
  int _b = (ActivityThreshold);
  reg[0] = ADXL345_THRESH_ACT;
  reg[1] = _b;
  err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);
  ADXL345_Check_Status(err_code, rx_data, reg[1], "Activity");
}

/*
  ADXL345
  Enable Data X reading 
 
*/

void ADXL345_I2C_Set_DATAX0(void) {

  uint8_t reg[1] = {ADXL345_DATAX0};

  ret_code_t err_code = ADXL345_I2C_register_write(ADXL345_ADDRESS, reg, sizeof(reg));
  APP_ERROR_CHECK(err_code);
}

/*
  ADXL345
  Reading data from DataX register
  With I2C protocol, it is possible to request multiple bytes in one reading session.
  To do this request for 6 bytes when providing DATAX0 address
 
*/

void ADXL345_Read_Sensor_Data(void) {

  ADXL345_I2C_Set_DATAX0();

  /* Read 6 bytes from the specified address - m_sample is 6 byte size */
  ret_code_t err_code = nrf_drv_twi_rx(p_twi_master, ADXL345_ADDRESS, m_sample, sizeof(m_sample));

  APP_ERROR_CHECK(err_code);
}


/*
  ADXL345
  Receiving data from DATAX0 comes in 2 byte format 
  the format is LSB is first then followed by MSB
  XYZ first two bytes are X, then Y, then Z
 
*/


void ADXL345_read_XYZ(int *x, int *y, int *z) {
  ADXL345_Read_Sensor_Data();
  *x = (short)((((unsigned short)m_sample[1]) << 8) | m_sample[0]);
  *y = (short)((((unsigned short)m_sample[3]) << 8) | m_sample[2]);
  *z = (short)((((unsigned short)m_sample[5]) << 8) | m_sample[4]);
}





void ADXL345_Read_Acceleration(int *xyz) {
  ADXL345_read_XYZ(xyz, xyz + 1, xyz + 2); // This is just an array 
}

void ADXL345_Get_Acceleration(double *xyz) {
  int i;
  double gain[3];
  gain[0] = 0.00376390;
  gain[1] = 0.00376009;
  gain[2] = 0.00349265;
  int xyz_int[3];
  ADXL345_Read_Acceleration(xyz_int);
  for (i = 0; i < 3; i++) {
    xyz[i] = xyz_int[i] * gain[i];
  }
}

/*
  ADXL345
  Function just checks if everything is OK
  If so than print that information to the user 
*/

void ADXL345_Check_Status(ret_code_t err_code, uint8_t rx_data, uint8_t reg, char *description) {
  err_code = nrf_drv_twi_rx(p_twi_master, ADXL345_ADDRESS, &rx_data, 1);
  APP_ERROR_CHECK(err_code);
  printf("---------------------------------\n");
  if (err_code == NRF_SUCCESS && rx_data == reg) {
    printf("%s Mode has been set\n", description);
  } else {
    printf("Something goes wrong while setting the %s mode\n", description);
  }
}