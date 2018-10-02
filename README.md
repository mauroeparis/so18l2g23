# ROGER ROGER - Lab2 Documentation

## Glosario de funciones

`acquire(struct spinlock*)`: Espera el uso del **lock** hasta que está disponible.

`release(struct spinlock*)`: Devuelve el uso del **lock** y se lo da a quien lo está esperando.

`sleep(void*, struct spinlock*)`: Cambia el estado de un **proceso** a *dormido* y libera el **lock** en el canal.

`wakeup(void *chan)`: Levanta todos los **procesos**.

`argint(int n, int *ip)`: Busca el enésimo integer de la **instrucción** y lo guarda en una *ip*.

## Procedimiento

### Día 1

Se leyó la implementación de `proc.c` para entender la `sleep()`, `awake()` y **Process Table**.

Entendios cómo funcionan los spinlocks, específicamente, analizamos el funcionamiento de `acquire()`, y cómo a través de `xchg(a,b)`, analizamos el estado del lock en cuestión.

Empezamos a crear semáforos. Creamos una estructura de un semáforo con un valor, un canal y un booleano para corroborar su apertura.

```
struct {
  unsigned int is_open;     // Apertura
  unsigned int resorces;    // Recursos en uso
  int proc_q[10];           // Call stack
} t_sem_;
```

Intentamos entender la diferencia entre Open y Create, después de debatir un poco, nos dimos cuenta que en este punto, el Open y el Close son simbólicos.

### Día 2

Implementamos `OPEN` y empezamos a implementar `CLOSE`. Nos cuesta entender la estructura de `sleep` y `awake`. No son lo suficientemente claras las declaraciones de las cosas que os piden, como la diferencias.

Debatimos mucho sobre exactamente cuáles eran las diferencias y los modos de implementar `up`, `down`.

Intentamos entender el orden de ejecución de las funciones. Decidimos intentar con la siguiente lógica:

```
create(sem, val) {
  if (proc_count != 0) {
    error;
  } else {
    sem.val = val
  }
}

open(sem) {
  acquire(sem);
  proc_count ++;
  release(sem);
}

close(sem) {
  acquire(sem);
  proc_count --;
  release(sem);
}
```

Probamos implementando una función que la llamamos `init_sems()`, para iniciar todos los semáforos.

### Día 3

El kernel compila bien, pero no anda el barrier. Hoy vamos a intentarsolucionar el problema.

**¿Por qúe da Zombies!?**  Esto es porque hace un fork.

Los barrier tiran un error. Pusimos a dormir `barrier_echo`, pero no estaba despertando. Con el comando `Ctrl+P` vemos el estado de los procesos.

El problema que teníamos era que en el `acquire` dentro del `while` que dormía el proceso restábamos el valor del semáforo. Pero debíamos hacerlo fuera del mismo.

So far andando:

```
$ barrier_init

$ barrier_echo hola mundo &

$ barrier_rise

hola mundo

$ barrier_release
$ barrier_init
```
**Error!**: La barrera ya fue inicializada. Eso quiere decir que `barrier_release` no lo hace bien.

No estábamos contemplando el hecho de que al llegar a 0 el proc_counter, se cerrara.

### Día 4

Hubo un problema para implementar dos semáforos andando al mismo tiempo. El problema se debía a que el `barrier_init` llamaba siempre al mismo.

Finalmente pudimos solucionar dicho problema.

### Día 5

Para hacer el programa **pingpong** primero escribimos un flujo del programa teniendo en cuenta cuando se tenia que habilitar tal o cual cosa para que no se repitieran **Pings** o **Pongs**.

Pasamos de planear utilizar todos los semáforos disponibles a solo 1, a 2. Nos quedamos con la ultima imaginándolo como una intersección de dos calles en donde cuando un semáforo se pone en rojo, el otro en verde habilitándose uno al otro.

También tuvimos que corregir una *syscall* porque estábamos tomando mal los argumentos con *argint*.

Creemos que funciona cómo funciona *zombies*. Cuando un **child** hace *exit* se queda "el zombie" esperando que el padre espere por él y pueda morir en paz.

Intentaremos implementar un `wait()` antes del *exit* de `pingpong.c` no tira "zombies!" al final!.

Esto no funcionó. Tira un error al final.

En el cambio que hicimos de `exists()` quedo mal implementado `ssem_open()`, pero estaba "andando" de casualidad porque nunca se ejecutaba el `sem_init()` (lo que inicia todos los semáforos en -1).

### Día 6

Al implementar el **Ping Pong** nos dimos cuenta que el `argint()` estaba mal implementado, entonces hacía cualquier cosa con los resorces que le pasábamos.

Además hacíamos el `semclose()` antes del `exit()` lo que hacía que al hacerse un `fork()` antes de cerrarse, se cerraban dos veces los semáforos.

Subimos al `wait()` al PING para que sólo el *padre* ejecute el ismo.
