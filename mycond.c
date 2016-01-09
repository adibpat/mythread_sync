/* Single Author Info:
 *  savidhal Shivaji Vidhale
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */


#include "mycond.h"

int mythread_cond_init( mythread_cond_t *cond, const mythread_condattr_t * attr){

	if(cond == NULL){      /* Allocate memory if pointer is NULL */
    	cond = (mythread_cond_t*)malloc(sizeof(mythread_cond_t));
  	}

  	if(cond == NULL){
  		printf("Not Enough memory\n");
  		return -ENOMEM;  /* Not enough memory to assign pointer */
  	}
  	
  	cond->wait_count = 0;
  	//cond->q_head=NULL;

	return 0; /* On successfull init */
}


int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t * mutex){ 
	
	mythread_enter_kernel();
	mythread_block_phase1(&(cond->q_head), 1);
	mythread_mutex_unlock (mutex);
	
	/* Man pthread_cond_signal says:
	It may be impossible for an implementation of pthread_cond_signal() to avoid the unblocking of more than one thread blocked on a condition variable.
	Hence we don't try to provide any mechanism as such to provide atomicity to the situation where mutex is released and the thread is blocked 
	*/

	if((cond->q_head) != NULL){ 
	/* This condition checks if the signal was made to the thread that just got into block_phase1 , in which case 
	   it would have been unblocked and the queue wwould be empty. 
	 */
		cond->wait_count++;
		mythread_block_phase2();
	}
	
	mythread_mutex_lock(mutex);

	return 0;
}

int mythread_cond_signal(mythread_cond_t *cond){
	if(cond->q_head != NULL){
		mythread_enter_kernel();
		cond->wait_count--;
		mythread_unblock(&(cond->q_head),1); //unblock the thread waiting on condition cond
	}
	return 0;
}

int mythread_cond_broadcast(mythread_cond_t *cond){
/*	
	while(cond->wait_count != 0){
		mythread_enter_kernel();
		cond->wait_count--;
		mythread_unblock(&(cond->q_head),1);	
	}
*/ 
	while(cond->q_head != NULL){
		mythread_enter_kernel();
		mythread_unblock(&(cond->q_head),1);	/* Unblock all threads waiting on condition cond */
	}

	return 0;
}

int mythread_cond_destroy(mythread_cond_t *cond){
	
	if(cond == NULL){
    return -EINVAL;
  }
  cond = NULL;
  return 0;
}

