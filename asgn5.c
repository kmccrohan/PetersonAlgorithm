#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int[2];
  int turn;
}PetersonLock;

int main(int argc, char** argv){

}
