#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUMTHREADS 6
#define MAXARRAYSIZE 10

int numbers[MAXARRAYSIZE] = {1,2,3,4,5,6,7,8,9,10};
int sum[NUMTHREADS] ;

pthread_mutex_t m1;

// Define a structure to hold the arguments
typedef struct {
    int index;
    int chunkSize;
    int threadNo;
} thread_args_t;

void* print_number(void* argsp)
{
  thread_args_t* args = (thread_args_t*) argsp;

  for(int i = 0; i < args->chunkSize; i++)
  {
    sum[args->threadNo] += numbers[args->index + i];
  }

  //printf("Thread %ld have number %d\n", pthread_self(), *value);
}

int main()
{
  pthread_t t[NUMTHREADS];
  thread_args_t args[NUMTHREADS];
  int totalSum = 0;
  
  pthread_mutex_init(&m1, NULL);
  
  for(int i = 0; i < NUMTHREADS; i++)
  {
    args[i].chunkSize = MAXARRAYSIZE / (NUMTHREADS-1);
    args[i].index = i * args[i].chunkSize;
    args[i].threadNo = i;
    if(i == NUMTHREADS -1)
      args[i].chunkSize = MAXARRAYSIZE % (NUMTHREADS-1);
    if(pthread_create(t+i, NULL, print_number, &args[i]) != 0)
    {
      return 1;
    }
  }
  
  for(int i = 0; i < NUMTHREADS; i++)
  {
    if(pthread_join(t[i], NULL) != 0)
    {
      return 2;
    }
    printf("sum = %d\n", sum[i]);
    totalSum += sum[i];
  }
  
  
  printf("Total sum = %d\n",totalSum);
  return 0;
}
