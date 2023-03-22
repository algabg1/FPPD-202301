/*faca um codigo simples no VS que crie 10 threads e um vetor de
CEM MIL posicoes, popule o mesmo vetor na thread principal, crie
cada uma das 10 threads fazendo com que as mesmas concorram entre
si[OU SEJA, VC NAO PODE DIVIDIR O VETOR DE FORMA IGUALITARIA ENTRE
AS THREADS]  para somar as posicoes do vetor sem repeticoes das
posicoes do mesmo, cada thread soh pode somar uma posicao por vez,
USE MUTEX.
Seu codigo deve, ALEM de expor o resultado final correto, imprimir
quantas posicoes de soma cada thread foi responsavel [e eh aqui o
problema real :D ]*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lista2.h"

#define NUM_THREADS 10
#define ARRAY_SIZE 100000

struct thread_data {
    int thread_id;
    int* array;
    int* sum_per_thread;
    pthread_mutex_t* mutex;
};

void* thread_sum(void* arg) {
    struct thread_data* data = (struct thread_data*)arg;
    int thread_id = data->thread_id;
    int* array = data->array;
    int* sum_per_thread = data->sum_per_thread;
    pthread_mutex_t* mutex = data->mutex;
    int i, pos, sum = 0;

    for (i = thread_id; i < ARRAY_SIZE; i += NUM_THREADS) {
        pos = i;
        pthread_mutex_lock(mutex);
        if (array[pos] != 0) { // Verifica se a posição já foi somada por outra thread
            sum_per_thread[thread_id]--; // Se sim, desfaz a contagem dessa posição para essa thread
        }
        else {
            sum += pos;
            array[pos] = 1; // Marca a posição como já somada
        }
        pthread_mutex_unlock(mutex);
    }

    sum_per_thread[thread_id] += sum; // Adiciona a soma dessa thread ao vetor de somas

    pthread_exit(NULL);
}

int main() {
    int i;
    int array[ARRAY_SIZE];
    int sum_per_thread[NUM_THREADS] = { 0 };
    pthread_t threads[NUM_THREADS];
    struct thread_data data[NUM_THREADS];
    pthread_mutex_t mutex;

    // Preenche o vetor com números inteiros de 0 a ARRAY_SIZE-1
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    // Inicializa o mutex
    pthread_mutex_init(&mutex, NULL);

    // Cria as threads
    for (i = 0; i < NUM_THREADS; i++) {
        data[i].thread_id = i;
        data[i].array = array;
        data[i].sum_per_thread = sum_per_thread;
        data[i].mutex = &mutex;
        if (pthread_create(&threads[i], NULL, thread_sum, &data[i])) {
            fprintf(stderr, "Erro ao criar thread %d\n", i);
            exit(-1);
        }
    }

    // Espera as threads terminarem
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Erro ao esperar thread %d\n", i);
            exit(-1);
        }
    }

    // Imprime o resultado final e as posições de soma de cada thread
    int total_sum = 0;
    printf("Soma final: ");
    for (i = 0; i < NUM_THREADS; i++) {
        total_sum += sum_per_thread[i];
        printf("Thread %d: %d\t", i, sum_per_thread[i]);
    }
    printf("\nTotal: %d\n", total_sum);

    // Destroi o mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
