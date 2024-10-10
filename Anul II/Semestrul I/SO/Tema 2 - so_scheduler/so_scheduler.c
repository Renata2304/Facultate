#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#include "so_scheduler.h"
#include <errno.h>

/* defining thread statuss */
#define NEW 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define TERMINATED 4

/* maximum number of threads */
#define MAX_NO_THREADS 10000

// Thread struct, that is going to be used to describe every thread
typedef struct Thread {
	tid_t id; // the id of the thread
	unsigned int status; // the status of the thread
	unsigned int prio; // the priority of the thread
	unsigned int io;
	so_handler *handler; // the handler of the thread
	sem_t in_exec; // the semaphore of the thread
	int remaining_time; // the remaining time of the thread
}thread;

// Scheduler structure that is going to be used as the main thread scheduler
typedef struct Scheduler{
	int is_init; // checking if the scheduler is initialized
	unsigned int time_quantum; // the time quantum of the scheduler
	unsigned int no_events; // the number of events 
	unsigned int no_threads; // the number of threads
	unsigned int q_size; // the size of the priority queue
	sem_t done; // the semaphore of the scheduler
	thread *curr_thread; // the current thread of the scheduler
	thread *threads[MAX_NO_THREADS]; // the list of threads (not in order)
	thread *q[MAX_NO_THREADS]; // the list of threads (in order) - priority queue
}scheduler;

scheduler* sched = NULL;

// function that initialize and allocs memory for every 
int so_init(unsigned int time_quantum, unsigned int io) {
	// testing the parameters
	if ((sched && sched->is_init == 1) || time_quantum <= 0 
		|| io > SO_MAX_NUM_EVENTS)
		return -1;

	// allocating memory for scheduler itself
	sched = (scheduler*) malloc(sizeof(scheduler));
	if (!sched) {
		return -1;
	}

	// used to test if a scheduler is already initialized
	// used for the test with multiple initializations
	sched->is_init = 1; 
	
	// implementing the internal data of the scheduler
	sched->time_quantum = time_quantum;
	sched->no_events = io;
	sched->is_init = 1;
	sched->curr_thread = NULL;
	sched->q_size = 0;
	sched->no_threads = 0;
	sem_init(&sched->done, 0, 1);

	return 0; // if everything was implemented successfully
}

// function created to start a routine when we want to initialize 
// a new thread's handler
void *func_thread(void *args) {
	// creating a new thread
	thread* thr = (thread *) args;
	// waiting for the thread to finish
	sem_wait(&thr->in_exec);
	// changing the handler
	thr->handler(thr->prio);
	thr->status = TERMINATED;
	// verifying if the priority queue is still correct
	verif();
}

tid_t so_fork(so_handler *func, unsigned int priority) {
	// testing the paramethers
	if (SO_MAX_PRIO < priority || func == NULL) {
		return (INVALID_TID);
	}
	if (sched->no_threads == 0) {
		sem_wait(&sched->done);
	}

	// allocs memory for the new added thread
	thread* new_thread = (thread*) malloc(sizeof(thread));
	// if the allocation failed
	if (new_thread == NULL) {
		return (INVALID_TID);
	}
	
	// increasing the number of threads in the scheduler
	sched->no_threads++;
	// implements all the atributes 
	new_thread->id = INVALID_TID;
	new_thread->prio = priority;
	new_thread->handler = func;
	new_thread->remaining_time = sched->time_quantum;
	new_thread->status = NEW;
	new_thread->io = SO_MAX_NUM_EVENTS;

	sem_init(&new_thread->in_exec, 0, 0);

	// implements the handler of the new thread
	pthread_create(&new_thread->id, NULL, &func_thread, (void *) new_thread);

	/* adding new thread to the scheduler */
	sched->threads[sched->no_threads - 1] = new_thread;
	
	add_thread(new_thread);

	if (sched->curr_thread != NULL)
		so_exec();
	else
		verif();

	return new_thread->id;
}

int so_wait(unsigned int io) {
	if (io < 0 || io >= sched->no_events)
		return -1;

	// we are blocking the current thread's actions
	sched->curr_thread->status = WAITING;
	sched->curr_thread->io = io;
	
	so_exec();
	return 0;
}

int so_signal(unsigned int io) {
	if (io < 0 || io >= sched->no_events)
		return -1;

	int nr = 0;

	// chnaging the threads with a specific io from waiting state to ready state
	for (int i = 0; i < sched->no_threads; ++i) {
		thread *thr = sched->threads[i];

		if (thr->io == io && thr->status == WAITING) {
			nr++;
			thr->status = READY;
			thr->io = SO_MAX_NUM_EVENTS;
			add_thread(thr);
		}
	}

	so_exec();

	return nr;
}

void so_exec(void) {
	thread *t = sched->curr_thread;

	// decrementing the time of the scheduler
	t->remaining_time--;

	// reuploading the thread
	verif();

	sem_wait(&t->in_exec);
}

void so_end(void) {
	// testing the parameters
	if (!sched)
		return;
	if (sched && !sched->is_init) {
		free(sched);
		sched = NULL;
	}

	// waiting for the scheduler to be done
	sem_wait(&sched->done);

	// waiting for all the threads, changing their handlers
	for (int i = 0; i < sched->no_threads; i++) {
		pthread_join(sched->threads[i]->id, NULL);
	}
	// freeing all the threads
	for (int i = 0; i < sched->no_threads; i++) {
		sem_destroy(&sched->threads[i]->in_exec);
		free(sched->threads[i]);
	}

	sched->is_init = 0;
	sem_destroy(&sched->done);

	free(sched);
	sched = NULL;
}

// function used for updating the priority queue
void verif(void)
{
	thread *curr_t = sched->curr_thread, *next = NULL;

	// testing to see if the queue is empty
	if (sched->q_size == 0) {
		// checking to see if we can stop the current thread
		if (curr_t->status == TERMINATED) {
			sem_post(&sched->done);
		}
		sem_post(&curr_t->in_exec);
		return;
	}

	next = sched->q[sched->q_size - 1];

	/* if current thread is blocked/done or not yet initalised */
	if (curr_t == NULL || curr_t->status == WAITING || curr_t->status == TERMINATED) {
		sched->curr_thread = next;
		
		// running thread next
		next->status = RUNNING;
		next->remaining_time = sched->time_quantum;
		// decrementing queue size
		sched->q_size--;
		sched->q[sched->q_size] = NULL;

		sem_post(&next->in_exec);

		return;
	}

	/* if 1st thread in q has a bigger prio */
	if (next->prio > curr_t->prio ) {
		add_thread(curr_t);
		sched->curr_thread = next;
		
		// running thread next
		next->status = RUNNING;
		next->remaining_time = sched->time_quantum;
		// decrementing queue size
		sched->q_size--;
		sched->q[sched->q_size] = NULL;

		sem_post(&next->in_exec);
		return;
	}

	/* if current thread's quantum expired */
	if (curr_t->remaining_time <= 0) {
		/* round robin */
		if (curr_t->prio == next->prio) {
			add_thread(curr_t);
			sched->curr_thread = next;

			// running thread next
			next->status = RUNNING;
			next->remaining_time = sched->time_quantum;
			// decrementing queue size
			sched->q_size--;
			sched->q[sched->q_size] = NULL;

			sem_post(&next->in_exec);
			return;
		}

		curr_t->remaining_time = sched->time_quantum;
	}

	sem_post(&curr_t->in_exec);
}

// adds a new thread to the priority queue, based on its priority, making sure to keep
// the ascending rule 
void add_thread(thread *t)	{
	int i = 0;
	// finding the right place to add the new thread
	while (i < sched->q_size) {
		if (sched->q[i]->prio >= t->prio) {
			break;
		}
		i++;
	}
	// moving all the elements one position to the right
	int j = sched->q_size;
	while (j > i) {
		sched->q[j] = sched->q[j - 1];
		j--;
	}
	// adding the new thread
	sched->q[i] = t;
	sched->q[i]->status = READY;
	sched->q_size++; // incrementing the size of the queue
}