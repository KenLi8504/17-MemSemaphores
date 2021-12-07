#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>

#define KEY 24602
#define KEY2 24603

int main() {

//   union semun {
//   int val;                  //used for SETVAL
//   struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
//   unsigned short  *array;   //used for SETALL
//   struct seminfo  *__buf;
// };

  int semaphore;
  int prev_line_size;
  int v, r;
  char input[3];

  printf("Would you like to create or remove the semaphore?(c/R)");
  fgets(input,3, stdin);

  if (input[0] == 'c') {
    prev_line_size = shmget(KEY2, sizeof(int), IPC_CREAT | 0640);
    semaphore = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    union semun us;
    us.val = 1;
    int r = semctl(semaphore, 0, SETVAL, us);

    int foo = open("telephone.txt", O_TRUNC | O_CREAT, 0644);

    int *first;
    first = shmat(semaphore, 0, 0);
    *first = 0;

    shmdt(first);
  }

  else if (input[0] == 'R') {
    printf("Deleting the semaphore...\n");
    semaphore = semget(KEY, 1, 0);
    prev_line_size = shmget(KEY2,sizeof(int), IPC_CREAT | 0644);
    semctl(semaphore, IPC_RMID, 0);
    shmctl(prev_line_size,IPC_RMID,0);
    int file = open("telephone.txt",O_RDONLY);
    struct stat st;
    stat("telephone.txt", &st);
    char *buff = malloc(st.st_size);
    int test = read(file, buff, st.st_size);
    printf("%s",buff);
    free(buff);
    close(file);
  }

  else{
    printf("ERROR, please input a valid command\n.");
  }
  return 0;
}
