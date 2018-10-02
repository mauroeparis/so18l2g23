#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"

# define PING 0
# define PONG 1

int
main(int argc, char *argv[])
{
  if (argc > 2)
    printf(1, "This function takes an integer as an argument.\n");
  else {
    int pingpong_count = atoi(argv[1]);
    // create ping semaphore -> resorces = 1
    sem_open(PING, SEM_CREAT, 1);
    // create pong semaphore -> resorces = 0
    sem_open(PONG, SEM_CREAT, 0);
    // open pingsem
    sem_open(PING, SEM_OPEN, 1);
    // open pongsem
    sem_open(PONG, SEM_OPEN, 0);
    if (fork() > 0) { // ping fork
      int i;
      for(i=0; i < pingpong_count; i++) {
        // semping down()
        sem_down(PING);
        // print ("PING")
        printf(1, "PING\n");
        // sempong up()
        sem_up(PONG);
      }
      // close pongsem
      sem_close(PING);
    } else { // pong fork
      int j;
      for(j=0; j < pingpong_count; j++) {
        // sempong down()
        sem_down(PONG);
        // print ("PONG")
        printf(1, "PONG\n");
        // semping up()
        sem_up(PING);
      }
      // close pingsem
      sem_close(PONG);
    }
  }
  exit();
}
