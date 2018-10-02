# ROGER ROGER - Lab2 Documentation

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
