#include <stdio.h>
#include <stdlib.h>
#include "mybarrier.h"
#include <unistd.h>

mythread_t threadp;
mythread_t threadc;
mythread_t barthreads[10];
int count=0;

mythread_mutex_t locks;
mythread_cond_t condc, condp;
mythread_barrier_t mybarrier;

int buffer = 0;

void* producer(void *ptr) {
  int i;

  for (i = 1; i <= 10; i++) {
    
    mythread_mutex_lock(&locks); /* protect buffer */
    buffer = i;
    count++;
    mythread_enter_kernel();
    printf("Thread with ID %ld Produced %d\n",(long)mythread_self()->tid,buffer);
    printf("Signal: Producer -> Consumer\n");
    mythread_leave_kernel();
    mythread_cond_signal(&condc);    /* wake up consumer */
     
    while (buffer != 0){
        mythread_cond_wait(&condp, &locks);     
    }            
    /* If there is something 
    in the buffer then wait */
    mythread_cond_broadcast(&condc); 
    mythread_mutex_unlock(&locks);   /* release the buffer */
    
  }
  mythread_exit(0);
}



void* consumer(void *ptr) {
  while(count !=10){
    
    mythread_mutex_lock(&locks); /* protect buffer */
     
    while(buffer == 0 && count != 10){
        mythread_cond_wait(&condc,&locks);
      }  
      while(buffer != 0){
      mythread_enter_kernel();
      printf("Consumer with ID %ld consumed %d\n",(long)mythread_self()->tid,buffer);
      printf("Signal: Consumer -> Producer\n");
      mythread_leave_kernel();
      buffer = 0;
      mythread_cond_signal(&condp);
      }
      mythread_mutex_unlock(&locks);   
  }
      
  mythread_exit(0);
}

void *barrierTest(void*arg){
  mythread_enter_kernel();
  printf("Thread with ID %ld moving towards first barrier\n",mythread_self());
  mythread_leave_kernel();
  //sleep(5);
  mythread_barrier_wait(&mybarrier);
  mythread_enter_kernel();
  printf("Thread with ID %ld moving towards second barrier\n",mythread_self());
  mythread_leave_kernel();

  mythread_barrier_wait(&mybarrier);
  mythread_enter_kernel();
  printf("Thread with ID %ld exiting... %ld\n",mythread_self());
  mythread_leave_kernel();
  mythread_exit(NULL);
}

int main(void)
{
    int err1, err2;
    int i;
    
    if (mythread_mutex_init(&locks, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }

    if (mythread_barrier_init(&mybarrier, NULL, 10) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }



    if(mythread_cond_init(&condp,NULL)!=0) return 1;
    if(mythread_cond_init(&condc,NULL)!=0) return 1;
    
    
    err1 = mythread_create(&threadc, NULL, consumer, NULL);
    if (err1 != 0) printf("%d",err1);
    
    err2 = mythread_create(&threadp, NULL, producer, NULL);
    if (err2 != 0) printf("%d",err2);

    mythread_join(threadp,NULL); 
    mythread_join(threadc,NULL);
  

    printf("Conditional Variable and Mutex Test PASS\n\n\n");

    for(i=0;i<10;i++){
    err1 = mythread_create(&barthreads[i],NULL,barrierTest,NULL);
    if (err1 != 0) printf("Barrier Test Fail\n");
    }

    for(i=0;i<10;i++){
    err1 = mythread_join(barthreads[i],NULL);
    if (err1 != 0) printf("Barrier Test Fail\n");
    }

    if (mythread_barrier_destroy(&mybarrier) != 0){
        printf("Barrier Test Fail\n");
        return 1;
    }

    if(mythread_cond_destroy(&condp)!=0) return 1;
    if(mythread_cond_destroy(&condc)!=0) return 1;
    if (mythread_mutex_destroy(&locks) != 0){
        printf("\n mutex destroy failed\n");
        return 1;
    }

    printf("Barrier Test PASS\n");
            
    return 0;
}
