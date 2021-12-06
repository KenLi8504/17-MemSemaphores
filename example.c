#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 24602

int main() {

  union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};

  int semd;
  int v, r;
  char input[3];

  semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
    semd = semget(KEY, 1, 0);
    v = semctl(semd, 0, GETVAL, 0);
    printf("semctl returned: %d\n", v);
  }
  else {
    union semun us;
    us.val = 1;
    r = semctl(semd, 0, SETVAL, us);
    printf("semctl returned: %d\n", r);
  }

  printf("Would you like to create or remove the semaphore?(c/R)");
  fgets(input,3, stdin);

  if (input[0] == 'y') {
    semctl(semd, IPC_RMID, 0);
    int f = open ("telephone.txt", O_RDONLY, 0664);
    close(f);
    printf("segment deleted\n");
  }
  else if (input[0] == 'c') {
    semctl(semd, IPC_RMID, 0);
    printf("What would you like to write to the file?\n");
    fgets(input,10000, stdin);
    int f =  open("telephone.txt" ,O_CREAT | O_TRUNC, 0644);
    close(f);
    printf("segment deleted\n");
  }


  return 0;
}
