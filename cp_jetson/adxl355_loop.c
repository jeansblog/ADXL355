#include "adxl355.h"
#include <unistd.h>
#include <time.h>
#include <math.h>

int main(int argc, char const *argv[]) {
  /* code */
  char *i2cFileName = "/dev/i2c-1";
  //int driverAddress = 0x53;
  int driverAddress = 0x1D;
  int axisX;
  int axisY;
  int axisZ;
  int m_nI2c;
  clock_t time;
  FILE *fp;

  if ((fp = fopen("data.txt", "w")) == NULL) {
      return -1;
  }

  if((m_nI2c = open(i2cFileName, O_RDWR)) < 0){
      printf("m_nI2c open err\n");
      return -1;
  }

  if(ioctl(m_nI2c, I2C_SLAVE, driverAddress) < 0){
      printf("ioctl err\n");
      return -1;
  }

//  if( AXDXL355_isRunning()){
//      AXDXL355_end();
//  }
  AXDXL355_begin(m_nI2c);
  //AXDXL355_setRange(RANGE_2G);



//  printf("Current temperature is %fC",AXDXL355_getTemperature());
//  printf("");
//  printf("Press Ctrl-C to exit");

  while (1) {
    while(1){
        int t = (clock() / 100) % 10;
        if((t == 0) || (t == 2) || (t == 4) || (t == 6) || (t == 8)) {
          break;
        }
        usleep(60);
    }
    time = clock();
    AXDXL355_getAxes(&axisX,&axisY,&axisZ);
    printf("%ld,%d,%d,%d\n",time,axisX,axisY,axisZ);
    fprintf(fp,"%ld,%d,%d,%d\n",time,axisX,axisY,axisZ);
  }

  AXDXL355_end();
  close(m_nI2c);
  fclose(fp);
  return 0;
}
