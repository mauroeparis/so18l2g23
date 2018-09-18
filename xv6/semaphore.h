#define SEM_CREATE ASDASF

int sem_open(int semaphore, int flags, int value);
int sem_close(int semaphore);
int sem_up(int semaphore);
int sem_down(unsigned int semaphore);
