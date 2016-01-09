/* Single Author Info:
 *  smnaik Sanskruti Naik
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */
#include "mycond.h"

typedef struct mythread_barrier_t{
	mythread_mutex_t barrier_mutex; 	// Mutex for the barrier
	mythread_cond_t barrier_cond;		// Condition variable for the barrier 
	int count_pending;					// Count of threads left for the barrier to wake up
	int count_pending_reset;
	int destroyable; 
	int alive;	
	int res;						// To check if the barrier has been initialized
} mythread_barrier_t;



typedef struct mythread_barrierattr_t{
	int bar_attr;
} mythread_barrierattr_t;



int mythread_barrier_init (mythread_barrier_t*, const mythread_barrierattr_t*, unsigned);

int mythread_barrier_destroy (mythread_barrier_t*);

int mythread_barrier_wait (mythread_barrier_t*);