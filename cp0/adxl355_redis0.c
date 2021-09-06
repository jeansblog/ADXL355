#include "adxl355.h"
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <hiredis.h> //Redis対応
#include <pthread.h> //並列処理

#define QUEUE_MAX 1024
typedef struct {
  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
} stack_t;
stack_t stack[QUEUE_MAX]; //スタック用の構造体配列
//int spt = -1;             //スタックポインタ（空のときは-1）
int queue_head=0, queue_tail =0;

int enqueue( int axisX, int axisY, int axisZ, clock_t time ) {
    if( (queue_tail+1)%(QUEUE_MAX+1) == queue_head ) return -1;

    stack[queue_tail].axisX = axisX;
    stack[queue_tail].axisY = axisY;
    stack[queue_tail].axisZ = axisZ;
    stack[queue_tail].time = time;
    queue_tail = (queue_tail+1)%(QUEUE_MAX +1);
    printf("in:%d\n",queue_tail);
    return 0;
}

int dequeue( int *axisX, int *axisY, int *axisZ, clock_t *time) {
    if( queue_head == queue_tail ) return -1;

    *axisX = stack[queue_head].axisX;
    *axisY = stack[queue_head].axisY;
    *axisZ = stack[queue_head].axisZ;
    *time = stack[queue_head].time;
    queue_head = (queue_head+1)%(QUEUE_MAX +1);
    printf("out:%d\n",queue_head);
    return 0;
}

void *i2c_to_stack(void *dmy){
  /* code */
  uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148  ;
  uint8_t slave_address = 0x53;

  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
  FILE *fp;

  if ((fp = fopen("data.txt", "w")) == NULL) {
      return NULL;
  }

  if (!bcm2835_init())
  {
    printf("bcm2835_init failed.\n");
    return NULL;
  }

  if (!bcm2835_i2c_begin())
  {
    printf("bcm2835_i2c_begin failed.\n");
    return NULL;
  }
  bcm2835_i2c_setSlaveAddress(slave_address);
  bcm2835_i2c_setClockDivider(clk_div);
  //bcm2835_i2c_set_baudrate(BAUDRATE);
//  if( AXDXL355_isRunning()){
//      AXDXL355_end();
//  }
  ADXL355_begin();
  ADXL355_setRange(RANGE_4G);
  ADXL355_setLowpassFilter(LOWPASS_FILTER_4000);

//  printf("Current temperature is %fC",AXDXL355_getTemperature());
//  printf("");
//  printf("Press Ctrl-C to exit");
  int t_old = 0;
  while (1) {
    while(1){
            int t = (clock() / 100) % 10;
            if(((t == 0) || (t == 2) || (t == 4) || (t == 6) || (t == 8)) && t != t_old) {
            //if(((t == 0) || (t == 5)) && t != t_old) {
                t_old = t;
                break;
            }
            usleep(60);
        }
    time = clock();
    ADXL355_getAxes(&axisX,&axisY,&axisZ);
    //printf("%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
    //fprintf(fp,"%d,%d,%d,%d\n",time,axisX,axisY,axisZ);
    //printf("ee\n");
    if(enqueue( axisX, axisY, axisZ, time ) == -1){
      //printf("e");
      break;
    }
  }
  ADXL355_end();
  bcm2835_i2c_end();
  bcm2835_close();
  return 0;
}

void  *stack_to_redis(void *dmy){
  int axisX;
  int axisY;
  int axisZ;
  clock_t time;
  redisContext *conn = NULL;
  redisReply *reply   = NULL;
  char    str[100];

  //printf("s");

  // 接続
  conn = redisConnect( "localhost" ,  // 接続先redisサーバ
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

  //printf("c\n");

  while (1) {
    reply  = (redisReply *)redisCommand( conn , "LLEN fft");
    if( NULL == reply ){
    // error
      redisFree( conn );
      exit(-1);
    }
    if(reply->integer == 0){
      //printf( " %d\n" , reply->integer );
      freeReplyObject(reply);
      break;
    }
    freeReplyObject(reply);
    usleep(1000);
  }

  while (1) {
    if(dequeue( &axisX, &axisY, &axisZ, &time)==-1){
      //printf("s\n");
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


int main(int argc, char const *argv[]) {
  //pthread_t th;

  //pthread_create(&th, NULL, &stack_to_redis, NULL);

  i2c_to_stack(NULL);
  stack_to_redis(NULL);

  //pthread_join(th, NULL);

  return 0;
  /* code */
}
