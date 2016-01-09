/* Single Author Info:
 *  abpatwar Aditya B Patwardhan
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */

#include <errno.h>
#include "mymutex.h"

//#define MYTHREAD_MUTEX_INITIALIZER {0}

 
int mythread_mutex_init ( mythread_mutex_t *mutex, void * attr ){

  if(mutex == NULL){      /* Check if the address of the mutex is NULL */
    mythread_enter_kernel();
    mutex = (mythread_mutex_t*)malloc(sizeof(mythread_mutex_t));
    mythread_leave_kernel();
    
    if(mutex==NULL){
      return -ENOMEM;     /* Return Not Enough memory error if not allocated memory */
    }
  }
  
  mutex->lock = 0;        /* Set the lock variable to initial value of 0; So now lock can be acquired */
  mutex->q_head = NULL;
  
  return 0;

}

int mythread_mutex_lock ( mythread_mutex_t * mutex ){
  
  if(mutex==NULL){    /* Mutex lock called without initialization */
    return -EINVAL;
  }

  int count = 0; /* Counts the number of spins on the lock */
  
  while(1){
    
    while(mutex->lock != 0){    /* Spin 50 times => Late back-off */
      count++;
      if(count==50){
	     mythread_enter_kernel();
	     mythread_block(&(mutex->q_head),1); // Block thread since it reaached the threshold of 50 spins.
	     count = 0; //Initialize count for subsequent attempts to acquire the lock
      }
    }
    
    
    if(!compare_and_swap(&(mutex->lock),1,0)){   /* Lock the mutex here */
      return 0;
    }
    
    if(mutex->q_head != NULL){       /* Early back-off */
      mythread_enter_kernel();
      mythread_block(&(mutex->q_head),1);
    }
  }
}

int mythread_mutex_unlock (mythread_mutex_t *mutex){
  
  if(mutex==NULL){
    return -EINVAL;
  }
  
  mythread_enter_kernel();
  mutex->lock = 0;
  mythread_leave_kernel();
  mythread_enter_kernel(); 
  mythread_unblock(&(mutex->q_head),1); //Unblock the next thread who needs the lock

  return 0;
}

int mythread_mutex_destroy ( mythread_mutex_t * mutex ){
  
  if(mutex == NULL){
    return -EINVAL;
  }

  mutex = NULL;
  return 0;
}


