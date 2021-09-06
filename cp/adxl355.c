//gcc adxl355.c -o adxl355
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include "adxl355.h"

int m_nI2c;

uint8_t i2c_read(uint8_t adr){
    uint8_t sendData;
    uint8_t readData;

    sendData = adr;
    if(write(m_nI2c, &sendData, 1) != 1){
        printf("AXDXL355_read() err1\n");
    }
    else{
        if(read(m_nI2c, &readData, 1) != 1){
            printf("AXDXL355_read() err2\n");
        }
     }

    return readData;
}

void i2c_block_read(uint8_t adr, int len, uint8_t *dat){
    uint8_t sendData;

    sendData = adr;
    if(write(m_nI2c, &sendData, 1) != 1){
        printf("AXDXL355_read() err1\n");
    }
    else{
        if(read(m_nI2c, dat, len) != len){
            printf("AXDXL355_read() err2\n");
        }
     }
}

void i2c_write(uint8_t adr, uint8_t dat){
    uint8_t buf[2];

    buf[0] = adr;
    buf[1] = dat;
    if(write(m_nI2c, buf, 2) != 2){
        printf("AXDXL355_write() err\n");
    }
}

uint8_t AXDXL355_isRunning(){
  uint8_t powerCtl;
  powerCtl = i2c_read(POWER_CTL);
  return (powerCtl & POWER_CTL_OFF) != POWER_CTL_OFF;
}

uint8_t AXDXL355_getStatus(){
  uint8_t status;
  status = i2c_read(STATUS);
  return status & STATUS_MASK_DATARDY;
}

uint8_t AXDXL355_getRange(){
  uint8_t range;
  range = i2c_read(RANGE);
  return range & RANGE_MASK;
}

void AXDXL355_setRange(uint8_t newRange){
  uint8_t range;
  range = i2c_read(RANGE);
  range = (range & ~RANGE_MASK) | newRange;
  i2c_write(RANGE,range);
}

uint8_t AXDXL355_getLowpassFilter(){
  uint8_t lowpassFilter;
  lowpassFilter = i2c_read(LOWPASS_FILTER);
  return lowpassFilter & LOWPASS_FILTER_MASK;
}

void AXDXL355_setLowpassFilter(uint8_t newLowpassFilter){
  uint8_t lowpassFilter;
  lowpassFilter = i2c_read(LOWPASS_FILTER);
  lowpassFilter = (lowpassFilter & HIGHPASS_FILTER_MASK) | newLowpassFilter;
  i2c_write(LOWPASS_FILTER,lowpassFilter);
}

uint8_t AXDXL355_getHighpassFilter(){
  uint8_t highpassFilter;
  highpassFilter = i2c_read(HIGHPASS_FILTER);
  return highpassFilter & HIGHPASS_FILTER_MASK;
}

void AXDXL355_setHighpassFilter(uint8_t newHighpassFilter){
  uint8_t highpassFilter;
  highpassFilter = i2c_read(HIGHPASS_FILTER);
  highpassFilter = (highpassFilter & LOWPASS_FILTER_MASK) | newHighpassFilter;
  i2c_write(HIGHPASS_FILTER,highpassFilter);
}

float AXDXL355_getTemperature(){
  uint8_t tempBytes[2];
  float temp;

  i2c_block_read(TEMP_START,TEMP_LENGTH,tempBytes);
  temp = tempBytes[0] << 8 | tempBytes[1];
  temp = (1852 - temp) / 9.05 + 19.21;
  return temp;
}

void AXDXL355_getAxes(int *axisX,int *axisY,int *axisZ){
  uint8_t axisBytes[9];

  i2c_block_read(AXIS_START,AXIS_LENGTH,axisBytes);
  axisX[0] = (axisBytes[0] << 16 | axisBytes[1] << 8 | axisBytes[2]) >> 4;
  axisY[0] = (axisBytes[3] << 16 | axisBytes[4] << 8 | axisBytes[5]) >> 4;
  axisZ[0] = (axisBytes[6] << 16 | axisBytes[7] << 8 | axisBytes[8]) >> 4;
  if(axisX[0] & (1 << 20 - 1)){
      axisX[0] = axisX[0] - (1 << 20);
    }
  if(axisY[0] & (1 << 20 - 1)){
      axisY[0] = axisY[0] - (1 << 20);
    }
  if(axisZ[0] & (1 << 20 - 1)){
      axisZ[0] = axisZ[0] - (1 << 20);
    }
}

void AXDXL355_begin(int g_nI2c){
    uint8_t powerCtl;

    m_nI2c = g_nI2c;
    powerCtl = i2c_read(POWER_CTL);
    if ((powerCtl & POWER_CTL_OFF)==POWER_CTL_OFF) {
      i2c_write(POWER_CTL,(powerCtl & POWER_CTL_ON));
    }
}

void AXDXL355_end(){
    uint8_t powerCtl;

    powerCtl = i2c_read(POWER_CTL);
    if ((powerCtl & POWER_CTL_OFF)!=POWER_CTL_OFF) {
      i2c_write(POWER_CTL,(powerCtl | POWER_CTL_ON));
    }
}
