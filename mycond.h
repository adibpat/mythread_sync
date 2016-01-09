/* Single Author Info:
 *  savidhal Shivaji Vidhale
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */

 #include "mymutex.h"

typedef struct mythread_cond {
	int wait_count;
	int alive;
	mythread_queue_t q_head;
} mythread_cond_t;

typedef struct mythread_condattr_t_ mythread_condattr_t;

int mythread_cond_init(mythread_cond_t *cond, const mythread_condattr_t * attr);

int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t * mutex);

int mythread_cond_signal(mythread_cond_t *cond);

int mythread_cond_broadcast(mythread_cond_t *cond);

int mythread_cond_destroy(mythread_cond_t *cond);