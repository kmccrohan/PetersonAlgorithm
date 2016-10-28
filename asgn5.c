#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//A Struct to store the Peterson Algorithm variables
typedef struct {
  int flag[2];
  int turn;
}PetersonLock;

// Member variables.
int time_parent;
int time_child;
int time_parent_non_cs;
int time_child_non_cs;

/**
* This function will allocate the peterson data in shared memory
*/
void allocate_shared_memory(){

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
   printf("chile leaving critical section\n");
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
   cs('p', time_parent);
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
   cs('c', time_child);
   non_cs(time_child_non_cs); 
  }
}

int main(int argc, char** argv){

  // Parse arguments.
  parse_args(argc, argv);

  // Allocate shared memories.

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
}
