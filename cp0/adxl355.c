//gcc adxl355.c -o adxl355
#include "adxl355.h"

uint8_t i2c_read(uint8_t adr){
    uint8_t sendData;
    uint8_t readData;

    sendData = adr;
    if(bcm2835_i2c_write_read_rs(&sendData, 1,&readData, 1)!= BCM2835_I2C_REASON_OK ){
        printf("AXDXL355_read() err1\n");
    }
    return readData;
}

void i2c_block_read(uint8_t adr, int len, uint8_t *dat){
    uint8_t sendData;

    sendData = adr;
    if(bcm2835_i2c_write_read_rs(&sendData, 1,dat, len)!= BCM2835_I2C_REASON_OK ){
        printf("AXDXL355_read() err1\n");
    }
}

void i2c_write(uint8_t adr, uint8_t dat){
    uint8_t buf[2];

    buf[0] = adr;
    buf[1] = dat;
    if(bcm2835_i2c_write(buf, 2) != BCM2835_I2C_REASON_OK){
        printf("AXDXL355_write() err\n");
    }
}

uint8_t ADXL355_isRunning(){
  uint8_t powerCtl;
  powerCtl = i2c_read(POWER_CTL);
  return (powerCtl & POWER_CTL_OFF) != POWER_CTL_OFF;
}

uint8_t ADXL355_getStatus(){
  uint8_t status;
  status = i2c_read(STATUS);
  return status & STATUS_MASK_DATARDY;
}

uint8_t ADXL355_getRange(){
  uint8_t range;
  range = i2c_read(RANGE);
  return range & RANGE_MASK;
}

void ADXL355_setRange(uint8_t newRange){
  uint8_t range;
  range = i2c_read(RANGE);
  range = (range & ~RANGE_MASK) | newRange;
  i2c_write(RANGE,range);
}

uint8_t ADXL355_getLowpassFilter(){
  uint8_t lowpassFilter;
  lowpassFilter = i2c_read(LOWPASS_FILTER);
  return lowpassFilter & LOWPASS_FILTER_MASK;
}

void ADXL355_setLowpassFilter(uint8_t newLowpassFilter){
  uint8_t lowpassFilter;
  lowpassFilter = i2c_read(LOWPASS_FILTER);
  lowpassFilter = (lowpassFilter & HIGHPASS_FILTER_MASK) | newLowpassFilter;
  i2c_write(LOWPASS_FILTER,lowpassFilter);
}

uint8_t ADXL355_getHighpassFilter(){
  uint8_t highpassFilter;
  highpassFilter = i2c_read(HIGHPASS_FILTER);
  return highpassFilter & HIGHPASS_FILTER_MASK;
}

void ADXL355_setHighpassFilter(uint8_t newHighpassFilter){
  uint8_t highpassFilter;
  highpassFilter = i2c_read(HIGHPASS_FILTER);
  highpassFilter = (highpassFilter & LOWPASS_FILTER_MASK) | newHighpassFilter;
  i2c_write(HIGHPASS_FILTER,highpassFilter);
}

float ADXL355_getTemperature(){
  uint8_t tempBytes[2];
  float temp;

  i2c_block_read(TEMP_START,TEMP_LENGTH,tempBytes);
  temp = tempBytes[0] << 8 | tempBytes[1];
  temp = (1852 - temp) / 9.05 + 19.21;
  return temp;
}

void ADXL355_getAxes(int *axisX,int *axisY,int *axisZ){
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

void ADXL355_begin(){
    uint8_t powerCtl;

    powerCtl = i2c_read(POWER_CTL);
    if ((powerCtl & POWER_CTL_OFF)==POWER_CTL_OFF) {
      i2c_write(POWER_CTL,(powerCtl & POWER_CTL_ON));
    }
}

void ADXL355_end(){
    uint8_t powerCtl;

    powerCtl = i2c_read(POWER_CTL);
    if ((powerCtl & POWER_CTL_OFF)!=POWER_CTL_OFF) {
      i2c_write(POWER_CTL,(powerCtl | POWER_CTL_ON));
    }
}
