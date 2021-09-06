#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print_hello(void *dmy)
{
        int i;

        for(i = 0; i < 20; i++) {
                printf("Hello\n");
                usleep(10);
        }
        return NULL;
}

void *print_world(void *dmy)
{
        int i;

        for(i = 0; i < 20; i++) {
                printf("World\n");
                usleep(10);
        }
        return NULL;
}

int main()
{
        pthread_t th;

        pthread_create(&th, NULL, &print_hello, NULL);
        print_world(NULL);
        pthread_join(th, NULL);
        return 0;
}
