#include "types.h"
#include "stat.h"
#include "user.h"
#include "semaphore.h"
#define SEM_BARRIER 0

int
main(int argc, char *argv[])
{
  if (sem_open(SEM_BARRIER, SEM_CREAT, 0)==-1){
  	printf(1, "La barrera ya fue inicializada\n");
  }
  exit();
}
