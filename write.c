#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY 24602
#define KEY2 24603

int main(){
  int semaphore;
  int prev_line_size;
  int* previousLength;
  char input[1000000];

  semaphore = semget(KEY, 0, 0);
  prev_line_size = shmget(KEY2, 0, 0);

  struct sembuf sb;
  sb.sem_op = -1; // Down
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  semop(prev_line_size, &sb, 1);

  previousLength = shmat(prev_line_size, 0, 0);
  char line[1000000];

  if (*previousLength != 0){
    int fd = open("telephone.txt", O_RDONLY);
    lseek(fd, -(*previousLength), SEEK_END);
    int test = read(fd, line, *previousLength);
    printf("The previous line was: %s\n", line);
  }

  printf("Please enter the new line: \n");
  fgets(line, 1000000, stdin);
  *previousLength = strlen(line);

  test = open("telephone.txt", O_WRONLY | O_APPEND);
  int err = write(test, line, strlen(line));


  shmdt(previousLength);

  sb.sem_op = 1; // Up
  semop(prev_line_size, &sb, 1);
  return 0;
}
