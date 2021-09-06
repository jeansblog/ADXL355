#include "adxl355.h"
#include <unistd.h>
#include <time.h>
#include <math.h>

#define BAUDRATE        3400000

int main(int argc, char const *argv[]) {
  /* code */
  uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148  ;
  uint8_t slave_address = 0x53;

  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
  FILE *fp;

  if ((fp = fopen("data.txt", "w")) == NULL) {
      return -1;
  }

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
  //bcm2835_i2c_set_baudrate(BAUDRATE);
//  if( AXDXL355_isRunning()){
//      AXDXL355_end();
//  }
  ADXL355_begin();
  ADXL355_setRange(RANGE_2G);
  ADXL355_setLowpassFilter(LOWPASS_FILTER_4000);
//  ADXL355_setRange(RANGE_2G);

//  printf("Current temperature is %fC",AXDXL355_getTemperature());
//  printf("");
//  printf("Press Ctrl-C to exit");
  int old_t=0;
  while (1) {
    /*
    while(1){
       int t = (clock() / 100) % 10;
       if(t != old_t)  {
          old_t = t;
          break;
        }
        usleep(1);
        */
        while(1){
            int t = (clock() / 100) % 10;
            if((t == 0) || (t == 2) || (t == 4) || (t == 6) || (t == 8)) {
              break;
            }
            usleep(60);
        }
    time = clock();
    ADXL355_getAxes(&axisX,&axisY,&axisZ);
    printf("%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
    fprintf(fp,"%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
  }

  ADXL355_end();
  bcm2835_i2c_end();
  bcm2835_close();
  return 0;
}
