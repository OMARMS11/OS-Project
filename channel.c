/*
 * channel.c
 *
 *  Created on: Sep 22, 2024
 *      Author: HP
 */
#include "channel.h"
#include <kern/proc/user_environment.h>
#include <kern/cpu/sched.h>
#include <inc/string.h>
#include <inc/disk.h>

//===============================
// 1) INITIALIZE THE CHANNEL:
//===============================
// initialize its lock & queue
void init_channel(struct Channel *chan, char *name)
{
	strcpy(chan->name, name);
	init_queue(&(chan->queue));
}

//===============================
// 2) SLEEP ON A GIVEN CHANNEL:
//===============================
// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
// Ref: xv6-x86 OS code
void sleep(struct Channel *chan, struct spinlock  *lk)
{
	//TODO: [PROJECT'24.MS1 - #10] [4] LOCKS - sleep
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("sleep is not implemented yet");
	//Your Code is Here...
	// Acquire the process queue lock for protection
	acquire_spinlock(&ProcessQueues.qlock);
	// Get the current process
	struct Env *p = get_cpu_proc();
    p->channel = chan;              // Assign the channel to the process
    p->env_status = ENV_BLOCKED;    // Set the process status to blocked
    // Release the provided lock before sleeping
    release_spinlock(lk);
    // Enqueue the current process into the channel's process queue
	enqueue(&chan->queue, p);
	// Yield the CPU by calling the scheduler
	sched();
	// After being woken up, the process continues here
	acquire_spinlock(lk);  // Reacquire the lock
	// Clear the channel and mark the process as ready
	p->channel = 0;
	// Release the process queue lock after queue manipulation
	release_spinlock(&ProcessQueues.qlock);
}

//==================================================
// 3) WAKEUP ONE BLOCKED PROCESS ON A GIVEN CHANNEL:
//==================================================
// Wake up ONE process sleeping on chan.
// The qlock must be held.
// Ref: xv6-x86 OS code
// chan MUST be of type "struct Env_Queue" to hold the blocked processes
void wakeup_one(struct Channel *chan)
{
	//TODO: [PROJECT'24.MS1 - #11] [4] LOCKS - wakeup_one
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("wakeup_one is not implemented yet");
	//Your Code is Here...
	// Dequeue one process from the channel's queue
	struct Env *p = dequeue(&chan->queue);
    // If a process was dequeued (i.e., there was a process in the queue)
	if (p != NULL) {
	// Set process state to runnable (ready to be scheduled)
	p->env_status = ENV_READY;
	// Insert the process into the scheduler's ready queue
	acquire_spinlock(&ProcessQueues.qlock);
	sched_insert_ready0(p);
	release_spinlock(&ProcessQueues.qlock);
}
}
//====================================================
// 4) WAKEUP ALL BLOCKED PROCESSES ON A GIVEN CHANNEL:
//====================================================
// Wake up all processes sleeping on chan.
// The queues lock must be held.
// Ref: xv6-x86 OS code
// chan MUST be of type "struct Env_Queue" to hold the blocked processes

void wakeup_all(struct Channel *chan)
{
	//TODO: [PROJECT'24.MS1 - #12] [4] LOCKS - wakeup_all
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("wakeup_all is not implemented yet");
	//Your Code is Here...

	// Iterate over all processes in the channel's process queue
	struct Env *p;
	while ((p = dequeue(&chan->queue)) != NULL) {
	// Set process state to READY (ready to be scheduled)
	p->env_status = ENV_READY;
	// Insert the process into the scheduler's READY queue
	acquire_spinlock(&ProcessQueues.qlock);
	sched_insert_ready0(p);
	release_spinlock(&ProcessQueues.qlock);
}

}
