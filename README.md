# Tarea 3 - Semaforos

Dos hilos POSIX imprimen cada uno del 1 al 10,000.
Del 4,000 al 6,000 cada hilo esta en su seccion critica, protegida con un semaforo
(`sem_wait` / `sem_post`), por lo que el otro hilo no puede intercalar su salida.

- `semaforos.c`: codigo fuente
- `salida.txt`: salida de la ejecucion en Debian 13 (Trixie)

Compilar: `gcc -Wall -Wextra -pthread -o semaforos semaforos.c`

Ejecutar: `./semaforos > salida.txt`
