#include "adxl355.h"

int main(int argc, char const *argv[]) {
  /* code */
  uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148 ;
  uint8_t slave_address = 0x53;
  int i;
  double dfVal;
  int axisX;
  int axisY;
  int axisZ;

  if (!bcm2835_init())
  {
    printf("bcm2835_init failed.\n");
    return 1;
  }

  if (!bcm2835_i2c_begin())
  {
    printf("bcm2835_i2c_begin failed.\n");
    return 1;
  }
  bcm2835_i2c_setSlaveAddress(slave_address);
  bcm2835_i2c_setClockDivider(clk_div);

  ADXL355_begin();

  ADXL355_setLowpassFilter(LOWPASS_FILTER_4000);
  uint8_t filter = ADXL355_getLowpassFilter();
  printf("filter = %d\n",filter);

  //AXDXL355_setRange(RANGE_4G);
  uint8_t range = ADXL355_getRange();
  printf("range = %d\n",range);

  //float temperature = AXDXL355_getTemperature();
  //printf("%f\n",temperature);

  //AXDXL355_getAxes(&axisX,&axisY,&axisZ);
  //printf("%d,%d,%d\n",axisX,axisY,axisZ);

  uint8_t running = ADXL355_isRunning();
  uint8_t status = ADXL355_getStatus();
  printf("running = %d,status = %d\n",running,status);
  ADXL355_end();

  bcm2835_i2c_end();
  bcm2835_close();
  return 0;
}
