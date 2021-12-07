#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>

#define KEY 24602
#define KEY2 24603

int main() {

  union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};

  int semd;
  int prev_line_size;
  int v, r;
  char input[3];

  printf("Would you like to create or remove the semaphore?(c/R)");
  fgets(input,3, stdin);

  if (input[0] == 'c') {
    printf("Creating the semaphore...\n");
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    prev_line_size = shmget(KEY2,sizeof(int), IPC_CREAT | 0644);
    union semun us;
    us.val = 1;
    int file = open("telephone.txt",O_CREAT | O_TRUNC, 0644);
  }

  else if (input[0] == 'R') {
    printf("Deleting the semaphore...\n");
    semd = semget(KEY, 1, 0); //get access
    semctl(semd, IPC_RMID, 0);
    int file = open("phone.txt",O_RDONLY);
    close(file);
  }

  else{
    printf("ERROR, please input a valid command\n.");
  }
  return 0;
}
