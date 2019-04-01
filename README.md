# ADXL 345 for NRF52

This is the library for Accelerometer ADXL345 using NRF52. 
This library uses the I2C bus.



  
![alt text](https://cdn-shop.adafruit.com/970x728/1231-00.jpg)



### How to use

The easiet way to use this library is to open the nRF5 SDK 15 example using I2C (TWI) and add the header file and source code to your project Depends on your PCA.

### Example project 

You can test this library using Segger Example. Download the example project and open the Solution using segger with path /examples/peripheral/twi_sensor/your_PCA/blank/ses


|Function                          |Description               |
|-------------------------------|-----------------------------|
|`ADXL345_I2C_Normal_Mode`            |Initial function use it first           |
|`ADXL345_I2C_Init_Measure_Mode`            |Set measure mode           |
|`ADXL345_I2C_Sleep_Mode`|Enable sleep mode to save power|
|`ADXL345_I2C_Set_Format`            |Set formating (todo: add function parameters)           |
|`ADXL345_I2C_Inactivity_Threshold(val)`            |Set set inactivity mode value 0-255          |
|`ADXL345_I2C_Activity_Threshold(val)`|Set set inactivity mode value 0-255|
|`ADXL345_I2C_Set_DATAX0`            |Send address to register for enable X axis data (read only)          |
|`ADXL345_Read_Sensor_Data`            |Read sensor data from axis          |
|`ADXL345_Read_Acceleration`|Compute and read acceleration data from each axis|




### Todos

 - Write more Tests
 - Add more functionallity 
 - Clean up the code 



