#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//A Struct to store the Peterson Algorithm variables
typedef struct
{
  int flag[2];
  int turn;
}PetersonLock;

// Member variables.
int time_parent;
int time_child;
int time_parent_non_cs;
int time_child_non_cs;
int shared_memory_id;
PetersonLock* lock = NULL;

/**
* This function will allocate the peterson data in shared memory
*/
void allocate_shared_memory()
{
  shared_memory_id = shmget(IPC_PRIVATE, sizeof(PetersonLock), IPC_CREAT | 0666);
  if(shared_memory_id < 0)
  {
    fprintf(stderr, "shmget: failed in an unexpected way\n");
    exit(1);
  }

  lock = (PetersonLock*) shmat(shared_memory_id, NULL, 0);
  lock->flag[0] = 0;
  lock->flag[1] = 0;
  lock->turn = -1;
}

/**
Deallocates shared memory.
*/
void deallocate_shared_memory()
{
  int return_value = shmdt(lock);
  if(return_value < 0)
  {
    fprintf(stderr, "shmdt failed with error code: %d\n", return_value);
    exit(1);
  }
}

/**
Parses command line arguments and stores values.
Exits if invalid number of args.
*/
void parse_args(int argc, char** argv)
{
  // if argcount == 5, parse values.
  if (argc == 5)
  {
    time_parent = atoi(argv[1]);
    time_child = atoi(argv[2]);
    time_parent_non_cs = atoi(argv[3]);
    time_child_non_cs = atoi(argv[4]);
  }
  // default values if argc =1
  else if (argc == 1)
  {
     time_parent = 2;
     time_child = 2;
     time_parent_non_cs = 1;
     time_child_non_cs = 1;
  }
  // display error message otherwise
  else
  {
    printf("Invalid number of args!\n");
    exit(1);
  }

}

/**
Executes the CS for either parent or child.
*/
void cs(char process, int time_crit_sect)
{
 if (process == 'p')
  {
   printf("parent in critical sction\n");
   sleep(time_crit_sect);
   printf("parent leaving critical section\n");
  }
 else
  {
   printf("child in critical sction\n");
   sleep(time_crit_sect);
   printf("child leaving critical section\n");
  }
}

/**
Executes the non CS for both.
*/
void non_cs(int time_non_crit_sect)
{
 sleep(time_non_crit_sect);
}

/**
Loops the parent through the critical and non critical sections.
*/
void parent()
{
 for (int i = 0; i < 10; i++)
  {
   //protect this
   lock->flag[0] = 1;
   lock->turn = 1;
   while(lock->flag[1] && lock->turn == 1);
   cs('p', time_parent);
   lock->flag[0] = 0;

   non_cs(time_parent_non_cs);
  }
}

/**
Loops the child through the critical and non critical sections.
*/
void child()
{
 for (int i = 0; i < 10; i++)
  {
   //protect this
   lock->flag[1] = 1;
   lock->turn = 0;
   while (lock->flag[0] && lock->turn == 0);
   cs('c', time_child);	// critical section
   lock->flag[1] = 0;

   non_cs(time_child_non_cs);
  }

  // exit child.
  exit(0);
}

int main(int argc, char** argv){

  // Parse arguments.
  parse_args(argc, argv);

  // Allocate shared memories.
  allocate_shared_memory();

  // Fork child and process.
  int value;
  if ((value = fork()) < 0)
    printf("Child could not be created\n");
  else
    if (value == 0) // child
      child();
    else // parent
      parent();

  // Cleanup.
  deallocate_shared_memory();
}
