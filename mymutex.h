/* Single Author Info:
 *  abpatwar Aditya B Patwardhan
 * Group Info:
 *  abpatwar Aditya B Patwardhan
 *  smnaik Sanskruti M Naik
 *  savidhal Shivaji A Vidhale 
 */
#include "mythread.h"
 
typedef struct mythread_mutex_t {
  int lock;
  mythread_queue_t q_head;
} mythread_mutex_t;

typedef struct mythread_mutex_attr_t {
  int lockValue;
} mythread_mutex_attr_t;



int mythread_mutex_init ( mythread_mutex_t *mutex, void * attr );

int mythread_mutex_lock ( mythread_mutex_t * mutex );

int mythread_mutex_unlock ( mythread_mutex_t *mutex);

int mythread_mutex_destroy ( mythread_mutex_t * mutex );