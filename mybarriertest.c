#include <stdio.h>
#include <stdlib.h>
#include "mybarrier.h"
#include <unistd.h>

mythread_t threadp;
mythread_t threadc[10];
int count=0;

mythread_barrier_t mybarrier;

void *barrierTest(void* arg){
  mythread_enter_kernel();
  printf("Going to sleep...%ld\n",mythread_self());
  mythread_leave_kernel();
  //sleep(5);
  mythread_barrier_wait(&mybarrier);
  mythread_enter_kernel();
  printf("Moving towards new barrier... %ld\n",mythread_self());
  mythread_leave_kernel();

  mythread_barrier_wait(&mybarrier);
  mythread_enter_kernel();
  printf("%ld exiting... %ld\n",mythread_self());
  mythread_leave_kernel();
  mythread_exit(NULL);
}

int main(void)
{
  
  int err1, err2;
  int i;
    
  if (mythread_barrier_init(&mybarrier, NULL, 10) != 0){
    printf("Barrier Test Fail\n");
    return 1;
  }



  for(i=0;i<10;i++){
    err1 = mythread_create(&threadc[i],NULL,barrierTest,NULL);
    if (err1 != 0) printf("Barrier Test Fail\n");
  }
    

  for(i=0;i<10;i++){
    err1 = mythread_join(threadc[i],NULL);
    if (err1 != 0) printf("Barrier Test Fail\n");
  }
  
  if (mythread_barrier_destroy(&mybarrier) != 0){
        printf("Barrier Test Fail\n");
        return 1;
    }

  printf("Barrier Test PASS\n");

  return 0;

}
