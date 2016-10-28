#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Member variables.
int time_parent;
int time_child;
int time_parent_non_cs;
int time_child_non_cs;

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
     time_parent = 3;
     time_child = 3;
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

int main(int argc, char** argv){

  parse_args(argc, argv);
}


