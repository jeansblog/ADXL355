#include "adxl355.h"

int main(int argc, char const *argv[]) {
  /* code */
  char *i2cFileName = "/dev/i2c-1";
  int driverAddress = 0x53;
  int i;
  double dfVal;
  int axisX;
  int axisY;
  int axisZ;
  int m_nI2c;

  if((m_nI2c = open(i2cFileName, O_RDWR)) < 0){
      printf("m_nI2c open err\n");
      return -1;
  }

  if(ioctl(m_nI2c, I2C_SLAVE, driverAddress) < 0){
      printf("ioctl err\n");
      return -1;
  }

  AXDXL355_begin(m_nI2c);

  AXDXL355_setLowpassFilter(LOWPASS_FILTER_4000);
  uint8_t filter = AXDXL355_getLowpassFilter();
  printf("filter = %d\n",filter);

  //AXDXL355_setRange(RANGE_4G);
  uint8_t range = AXDXL355_getRange();
  printf("range = %d\n",range);

  //float temperature = AXDXL355_getTemperature();
  //printf("%f\n",temperature);

  //AXDXL355_getAxes(&axisX,&axisY,&axisZ);
  //printf("%d,%d,%d\n",axisX,axisY,axisZ);

  uint8_t running = AXDXL355_isRunning();
  uint8_t status = AXDXL355_getStatus();
  printf("running = %d,status = %d\n",running,status);

  AXDXL355_end();
  close(m_nI2c);

  return 0;
}
