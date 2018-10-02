#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"

# define SEM_PING 0
# define SEM_PONG 1

int
main(int argc, char *argv[])
{
  if (argc > 2)
    printf(1, "This function takes an integer as an argument.\n");
  else {
    int pingpong_count = atoi(argv[1]);

    // create ping semaphore -> resorces = 1
    if(sem_open(SEM_PING, SEM_CREAT, 1) == -1)
      printf(1, "La barrera 0 ya fue inicializada\n");

    // create pong semaphore -> resorces = 0
    if(sem_open(SEM_PONG, SEM_CREAT, 0) == -1)
      printf(1, "La barrera 1 ya fue inicializada\n");

    // open pingsem
    sem_open(SEM_PING, SEM_OPEN, 1);
    // open pongsem
    sem_open(SEM_PONG, SEM_OPEN, 0);

    if (fork() > 0) { // ping fork
      int i;
      for(i = 0; i < pingpong_count; i++) {
        // semping down()
        sem_down(SEM_PING);
        // print ("PING")
        printf(1, "PING\n");
        // sempong up()
        sem_up(SEM_PONG);
      }
      // close pongsem
      sem_close(SEM_PING);
    } else { // pong fork
      int j;
      for(j = 0; j < pingpong_count; j++) {
        // sempong down()
        sem_down(SEM_PONG);
        // print ("PONG")
        printf(1, "PONG\n");
        // semping up()
        sem_up(SEM_PING);
      }
      // close pingsem
      sem_close(SEM_PONG);
    }
  }
  wait();
  exit();
}
