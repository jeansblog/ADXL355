#include "adxl355.h"
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <hiredis.h> //Redis対応

#define STACK_MAX 1000
typedef struct {
  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
} stack_t;
stack_t stack[STACK_MAX]; //スタック用の構造体配列
int spt = -1;             //スタックポインタ（空のときは-1）

int push( int axisX, int axisY, int axisZ, clock_t time ) {
    if( spt >= STACK_MAX-1 ) return -1;

    spt++;
    stack[spt].axisX = axisX;
    stack[spt].axisY = axisY;
    stack[spt].axisZ = axisZ;
    stack[spt].time = time;
    return 0;
}

int pop( int *axisX, int *axisY, int *axisZ, clock_t *time) {
    if( spt < 0 ) return -1;

    *axisX = stack[spt].axisX;
    *axisY = stack[spt].axisY;
    *axisZ = stack[spt].axisZ;
    *time = stack[spt].time;
    spt--;
    return 0;
}

int main(int argc, char const *argv[]) {
  /* code */
  uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148  ;
  uint8_t slave_address = 0x53;

  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
  FILE *fp;
  redisContext *conn = NULL;
  redisReply *reply   = NULL;
  char    str[100];

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
    ADXL355_getAxes(&axisX,&axisY,&axisZ);
    printf("%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
    fprintf(fp,"%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
    if(push( axisX, axisY, axisZ, time )==-1){
      break;
    }
  }
  ADXL355_end();
  bcm2835_i2c_end();
  bcm2835_close();

  // 接続
  conn = redisConnect( "10.103.96.20" ,  // 接続先redisサーバ
                       6379           // ポート番号
         );
  if( ( NULL != conn ) && conn->err ){
    // error
    printf("error : %s\n" , conn->errstr );
    redisFree( conn );
    exit(-1);
  }else if( NULL == conn ){
    exit(-1);
  }

  while (1) {
    if(pop( &axisX, &axisY, &axisZ, &time)==-1){
      break;
    }
    sprintf(str,"%d,%d,%d,%d",time,axisX,axisY,axisZ);
    reply  = (redisReply *)redisCommand( conn , "RPUSH fft %s" , str);
    if( NULL == reply ){
      // error
      redisFree( conn );
      exit(-1);
    }
    freeReplyObject(reply);
  }
  // 後片づけ
  redisFree( conn );

  return 0;
}
