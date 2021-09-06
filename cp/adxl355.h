/* ADXL355 include file
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#define POWER_CTL           0x2D

#define POWER_CTL_OFF       0x01
#define POWER_CTL_ON        ~POWER_CTL_OFF
#define POWER_CTL_DRDY_OFF  0x04
#define POWER_CTL_DRDY_ON   ~POWER_CTL_DRDY_OFF
#define POWER_CTL_TEMP_OFF  0x02
#define POWER_CTL_TEMP_ON   ~POWER_CTL_TEMP_OFF

#define RANGE               0x2C
#define RANGE_MASK          0x03
#define RANGE_2G            0b01
#define RANGE_4G            0b10
#define RANGE_8G            0b11

#define LOWPASS_FILTER        0x28
#define LOWPASS_FILTER_MASK   0b00001111
#define LOWPASS_FILTER_4000   0b00000000
#define LOWPASS_FILTER_2000   0b00000001
#define LOWPASS_FILTER_1000   0b00000010
#define LOWPASS_FILTER_500    0b00000011
#define LOWPASS_FILTER_250    0b00000100
#define LOWPASS_FILTER_125    0b00000101
#define LOWPASS_FILTER_62_5   0b00000110
#define LOWPASS_FILTER_31_25  0b00000111
#define LOWPASS_FILTER_15_625 0b00001000
#define LOWPASS_FILTER_7_813  0b00001001
#define LOWPASS_FILTER_3_906  0b00001010

#define HIGHPASS_FILTER        0x28
#define HIGHPASS_FILTER_MASK   0b01110000
#define HIGHPASS_FILTER_247    0b00010000
#define HIGHPASS_FILTER_62_084 0b00100000
#define HIGHPASS_FILTER_15_545 0b00110000
#define HIGHPASS_FILTER_3_862  0b01000000
#define HIGHPASS_FILTER_0_954  0b01010000
#define HIGHPASS_FILTER_0_238  0b01100000

#define XDATA3                0x08
#define XDATA2                0x09
#define XDATA1                0x0A
#define YDATA3                0x0B
#define YDATA2                0x0C
#define YDATA1                0x0D
#define ZDATA3                0x0E
#define ZDATA2                0x0F
#define ZDATA1                0x10

#define TEMP2                 0x06
#define TEMP1                 0x07

#define TEMP_START            TEMP2
#define TEMP_LENGTH           2

#define AXIS_START            XDATA3
#define AXIS_LENGTH           9

#define STATUS                0x04
#define STATUS_MASK_DATARDY   0x01
#define STATUS_MASK_NVMBUSY   0x10

uint8_t i2c_read(uint8_t adr);
void i2c_block_read(uint8_t adr, int len, uint8_t *dat);
void i2c_write(uint8_t adr, uint8_t dat);
uint8_t AXDXL355_isRunning();
uint8_t AXDXL355_getStatus();
uint8_t AXDXL355_getRange();
void AXDXL355_setRange(uint8_t newRange);
uint8_t AXDXL355_getLowpassFilter();
void AXDXL355_setLowpassFilter(uint8_t newLowpassFilter);
uint8_t AXDXL355_getHighpassFilter();
void AXDXL355_setHighpassFilter(uint8_t newHighpassFilter);
float AXDXL355_getTemperature();
void AXDXL355_getAxes(int *axisX,int *axisY,int *axisZ);
void AXDXL355_begin();
void AXDXL355_end();
