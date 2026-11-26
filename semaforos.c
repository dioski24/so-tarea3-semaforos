/*
 * Tarea 3 - Semaforos
 * Dos hilos imprimen cada uno una cuenta del 1 al 10000.
 * Del 4000 al 6000 (seccion critica) un hilo no puede ser
 * interrumpido por el otro: nadie mas imprime en ese tramo.
 */
#include <stdio.h>       /* printf, fprintf */
#include <stdlib.h>      /* exit, EXIT_FAILURE */
#include <pthread.h>     /* pthread_* */
#include <semaphore.h>   /* sem_t, sem_init, sem_wait, sem_post */

#define SHARED   0       /* 0 = semaforo compartido solo entre hilos */
#define LIMITE   10000
#define INICIO_SC 4000   /* inicio de la seccion critica */
#define FIN_SC    6000   /* fin de la seccion critica    */

sem_t mutex;             /* semaforo global, visible para ambos hilos */

void *contar(void *arg)
{
    int id = *(int *)arg;   /* numero de hilo: 1 o 2 */
    int i;

    /* Parte 1: del 1 al 3999. Cada numero se imprime con P y V. */
    for (i = 1; i < INICIO_SC; i++) {
        sem_wait(&mutex);                 /* P(mutex) */
        printf("Hilo %d: %d\n", id, i);
        sem_post(&mutex);                 /* V(mutex) */
    }

    /* Parte 2: seccion critica, del 4000 al 6000.
       Se toma el semaforo UNA vez y no se suelta hasta el 6000. */
    sem_wait(&mutex);                     /* P(mutex) - protocolo de entrada */
    printf("----- Hilo %d ENTRA a la seccion critica -----\n", id);
    for (i = INICIO_SC; i <= FIN_SC; i++) {
        printf("Hilo %d: %d\n", id, i);
    }
    printf("----- Hilo %d SALE de la seccion critica -----\n", id);
    sem_post(&mutex);                     /* V(mutex) - protocolo de salida */

    /* Parte 3: del 6001 al 10000, igual que la parte 1 */
    for (i = FIN_SC + 1; i <= LIMITE; i++) {
        sem_wait(&mutex);
        printf("Hilo %d: %d\n", id, i);
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}

int main(void)
{
    pthread_attr_t tattr;
    pthread_t tid[2];
    int ids[2] = {1, 2};
    int i;

    /* Semaforo binario con valor inicial 1 (libre) */
    if (sem_init(&mutex, SHARED, 1) != 0) {
        perror("Error en sem_init");
        exit(EXIT_FAILURE);
    }

    pthread_attr_init(&tattr);
    pthread_attr_setscope(&tattr, PTHREAD_SCOPE_SYSTEM);

    for (i = 0; i < 2; i++) {
        if (pthread_create(&tid[i], &tattr, contar, &ids[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo %d\n", ids[i]);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < 2; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&mutex);
    pthread_attr_destroy(&tattr);
    printf("Ambos hilos terminaron.\n");
    return 0;
}
