//gcc -Wall -o redis redis.c -lhiredis -L/usr/local/lib -I/usr/local/include/hiredis

#include <stdio.h>
#include <stdlib.h>
#include <hiredis.h>

int main(void){
  redisContext *conn = NULL;
  redisReply *resp   = NULL;
  int loop           = 0;

  // 接続
  conn = redisConnect( "10.103.96.99" ,  // 接続先redisサーバ
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

  // Valueの取得
  for( loop=1 ; loop < 4 ; loop++ ){
    resp = (redisReply *) redisCommand( conn ,          // コネクション
                                        "GET %d" , loop // コマンド
                                      );
    if( NULL == resp ){
      // error
      redisFree( conn );
      exit(-1);
    }
    if( REDIS_REPLY_ERROR == resp->type ){
      // error
      redisFree( conn );
      freeReplyObject( resp );
      exit(-1);
    }
    printf( "%d : %s\n" , loop , resp->str );
    freeReplyObject( resp );
  }

  // 後片づけ
  redisFree( conn );
  return 0;
}
