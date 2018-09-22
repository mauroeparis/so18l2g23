#include "types.h"
#include "spinlock.h"

#define SEM_CREAT 0
#define SEM_OPEN 1
#define SEM_OPEN_OR_CREAT 2

struct _sem_t{
  uint resorces;
  uint is_open;
  uint proc_count;
  struct spinlock slock;
};

typedef struct _sem_t sem_t;

sem_t sem_list[5];

int init_sems(void);
