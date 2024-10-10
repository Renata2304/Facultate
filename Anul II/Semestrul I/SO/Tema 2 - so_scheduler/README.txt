Vaideanu Renata - Georgia 322 CD
(I wrote many more comments throughout the code)

- typedef struct Thread -> is used for implementing the threads of the scheduler.
- typedef struct Scheduler -> is used for implementing the scheduler.
   the thread list contains all the threads added so far in the scheduler, without
them being in a specific order.
   the queue is the priority queue and will be used to sort the threads, comparing their
priority.
- so_init -> implements and allocs the arraays of the new scheduler. In the begginging
it tests to see if the scheduler was already implemented and if not, it will alloc memory
and return 0 if the action was finished succesfully. If the parameters were inncorect, the 
fuction will return -1.
- so_fork -> used to add a new thread to the scheduler. In the begginging, the program will
thest the parameters and return an invalid tid if the parameters were inncorect. Otherwise, 
the function will alloc memory to the new thread and will be added to the scheduler and if 
it is the case will be turned into the current thread. 
- so_wait -> The function will test the paramaters and if they are correct, it will block 
the current thread until io is signaled. In the end so_exec will be called.
- so_signal -> The function will test the paramaters and if they are correct, it will go 
through all the threads and will "wake up" (change status from WAITING to READY) all the 
threads that are waiting for a specific io.
- so_exec -> The function will test the paramaters and if they are correct, it will consume
time from the current thread and will update the priority queue, using verif();
- so_end -> The function will test the paramaters and if they are correct, it will wait for 
scheduler to be done and afterwards will dealloc all the memory.


Functions added by me:
- func_thread -> used to start a routine for the new thread that is going to be forked, changing 
its handler and setting its status to TERMINATED.
- verif -> updates the priority queue and treats the round robin cases.
- add_thread -> adds a new thread to the priority queue.