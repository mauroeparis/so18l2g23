#include "types.h"
#include "defs.h"
#include "x86.h"
#include "semaphore.h"


int
init_sems(void)
{
  int i;
  for(i = 0; i < 5; i++){
    sem_t s;
    s.proc_count = -1;
    sem_list[i] = s;
  }

  return 0;
}

int exists(int semaphore);

int
ssem_open(int semaphore, int flags, int value)
{
  acquire(&sem_list[semaphore].slock);
  switch (flags) {
    case SEM_CREAT:
      if(!exists(semaphore)){
        sem_list[semaphore].proc_count = 0;
        release(&sem_list[semaphore].slock);
        return -1;
      }

      sem_list[semaphore].resorces = value;
      release(&sem_list[semaphore].slock);
      return 0;

    case SEM_OPEN:
      if(!exists(semaphore)){
        release(&sem_list[semaphore].slock);
        return -1;
      }

      sem_list[semaphore].proc_count++;
      release(&sem_list[semaphore].slock);
      return 0;

    case SEM_OPEN_OR_CREAT:
      if(ssem_open(semaphore, SEM_OPEN, value) == -1)
        ssem_open(semaphore, SEM_CREAT, value);

      release(&sem_list[semaphore].slock);
      return 0;
  }
  return -2;
}

int
ssem_close(int semaphore)
{

/*
  Cierra el uso del semáforo con nombre semaphore. Sí ningún otro proceso lo está usando,
  semaphore queda liberado para volver a ser usado. Si semaphore no existe esto será considerado un error.
  En caso de éxito devuelve ‘n’ con n la cantidad de procesos que están usando todavía el semáforo
  (notar que la función devuelva 0 implica que el semáforo está libre para volver a ser usado).
  En caso de error la función devuelve -1.
*/
  acquire(&sem_list[semaphore].slock);
  if(!exists(semaphore)) {
    release(&sem_list[semaphore].slock);
    return -1;
  }

  if(sem_list[semaphore].proc_count == 1) {
    sem_list[semaphore].proc_count = -1;

    release(&sem_list[semaphore].slock);
    return 0;
  }

  int count = --(sem_list[semaphore].proc_count);

  release(&sem_list[semaphore].slock);
  return count;
}

int
ssem_up(int semaphore)
{
  /*
  Incrementa (destraba) el semáforo con nombre semaphore.
  Si semaphore no existe esto será considerado un error. En caso de éxito devuelve 0 y en caso de error -1.
  */
  acquire(&sem_list[semaphore].slock);
  if(!exists(semaphore)){
    release(&sem_list[semaphore].slock);
    return -1;
  }
  sem_list[semaphore].resorces++;

  wakeup(&sem_list[semaphore]);
  release(&sem_list[semaphore].slock);

  return 0;
}


int
ssem_down(int semaphore)
{
  /*
  Decrementa (traba) el semáforo con nombre semaphore.
  Si semaphore no existe esto será considerado un error.
  En caso de éxito devuelve 0 y en caso de error -1.
  */
  acquire(&sem_list[semaphore].slock);
  if(!exists(semaphore)){
    release(&sem_list[semaphore].slock);
    return -1;
  }

  while(sem_list[semaphore].resorces == 0) {
    sleep(&sem_list[semaphore], &sem_list[semaphore].slock);
  }

  sem_list[semaphore].resorces--;

  release(&sem_list[semaphore].slock);
  return 0;
}




int
exists(int semaphore)
{
  return sem_list[semaphore].proc_count != -1;
}
