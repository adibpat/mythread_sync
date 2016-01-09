/* Single Author Info:
 *  smnaik Sanskruti Naik
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */

#include <errno.h>
#include "mybarrier.h"

#define MYTHREAD_BARRIER_SERIAL_THREAD 3451

int mythread_barrier_init (mythread_barrier_t *barrier, const mythread_barrierattr_t *attr, unsigned count){

 	if(barrier == NULL){      /* Check if the address of the barrier is NULL */
    
    mythread_enter_kernel();
    barrier = (mythread_barrier_t*)malloc(sizeof(mythread_barrier_t));
    mythread_leave_kernel();
  }
    
  if(barrier == NULL){
    printf("Not Enough memory\n");
    return -ENOMEM;     /* Return Not Enough memory error if not allocated memory */
  }
  

  if (count == 0)       /* Return Invalid if the number of threads to wait for is 0 */
  {
  	return -EINVAL;			
  }

  /* Reference: man pthread_barrier_init() */
  if (barrier->alive == 1)
  {
  	return - EBUSY;			/* Return Busy if there is an attempt to re-initialize a barrier when it is already in use */
  }

  
  /* Initializing mutex */  
  if(mythread_mutex_init(&(barrier->barrier_mutex), NULL)!=0){
    printf("Mutex init failed\n");
    return 1;
  }	

  /* Initializing condition variable */
  if(mythread_cond_init(&(barrier->barrier_cond), NULL)!=0){
    printf("Cond init failed\n");
    return 1;			
  }

  barrier->count_pending = count;  /* This value will be decremented following every barrier_wait call*/
  barrier->count_pending_reset = count; /* Reset the count_pending to this value after every broadcast */
  barrier->destroyable = 1; /* If 1, safe to destroy the barrier, else somebody is waiting on it so not safe to destroy */
  barrier->alive = 1;

  return 0;
}


int mythread_barrier_wait (mythread_barrier_t *barrier){

  if(barrier == NULL){
    return -EINVAL;
  }
	
  mythread_mutex_lock(&(barrier->barrier_mutex));
  barrier->count_pending--; //This should happen atomically. 

	if(barrier->count_pending == 0){	
    /*This is the last thread calling the respective barrier_wait call */
    barrier->count_pending = barrier->count_pending_reset; //Reset the count for subsequent barrier_wait calls. 
    mythread_cond_broadcast(&(barrier->barrier_cond));  //Broadcast to all the waiting threads that all the threads are at par
    barrier->destroyable = 1;  // Now it is safe to destroy this barrier until barrier_wait is not used anymore
    mythread_mutex_unlock(&(barrier->barrier_mutex));
  return MYTHREAD_BARRIER_SERIAL_THREAD ;
	}
  else{
    barrier->destroyable = 0;    
    mythread_cond_wait(&(barrier->barrier_cond), &(barrier->barrier_mutex)); //Waiting for other threads, not safe to destroy
	}

  mythread_mutex_unlock(&(barrier->barrier_mutex));
  return 0;

}



int mythread_barrier_destroy (mythread_barrier_t *barrier){

	if(barrier == NULL){		
    return -EINVAL;											/* Return Invalid if no memory was allocated for the barrier */
  }

  if (barrier->destroyable == 0){								/* Return Busy if threads are waiting on this barrier */
  	return -EBUSY;
  }

  if (barrier->alive == 0){
    return -EINVAL; 										/* Return Invalid if trying to destroy an already destroyed barrier */
  }

  mythread_mutex_destroy(&(barrier->barrier_mutex));				/* Destroy the mutex */
  
  mythread_cond_destroy(&(barrier->barrier_cond));				/* Destroy the condition variable */

  barrier->alive = 0;											/* Destroy the barrier */

  barrier = NULL;
  
  return 0;
  
}