/* Single Author Info:
 *   abpatwar Aditya B Patwardhan
 * Group Info:
 *   abpatwar Aditya B Patwardhan
 *   savidhal Shivaji A Vidhale
 *	 smnaik Sanskruti M Naik
 */

Introduction:
-------------

This project is an implementation of synchronization primitives for a non-preemptive user-level threading library -> mythread.a
We have provided the following methods for synchronization primitives:

mythread_mutex_init()
mythread_mutex_lock()
mythread_mutex_unlock()
mythread_mutex_destroy()

mythread_cond_init()
mythread_cond_wait()
mythread_cond_signal()
mythread_cond_broadcast()
mythread_cond_destroy()

mythread_barrier_inti()
mythread_barrier_wait()
mythread_barrier_destroy()

Mutex:
------

Mutex lock provides mutal exclusion to a critical section.
We have implemented early and late back-off strategy for locking.

Condition Variable:
-------------------

The condition variable makes the threads wait for a particular condition to happen.
Threads wait till the condition does not take place.
When the condition is met:
Signal : Wake up one thread waiting on the condition
Broadcast : Wake up all threads waiting on the condition

Barrier:
--------

The barrier provides mechanism to block all threads till all threads reach the barrier.
When all threads reach the barrier, the threads are unblocked.

How to Build:
------------
Our Makefile provides a bunch of targets; each one builds a particular part 
of the project.
Refer to the following commands for building ->

Compiling:
     $ make
# This is equivalent to "make all" and "make a5" which can also be alternatively used.
# It compiles all the code files, creates the corresponding object files and builds 
# the library file "mythreadlib.a”. It also builds the mythread_test file - a sample
# test program provided - which tests out all the mythread_ functions.
		     
Library Compilation:
     $ make lib

Cleaning:
     $ make clean

Test file:
     $ make test
