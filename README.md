# Lab2 Documentation

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
