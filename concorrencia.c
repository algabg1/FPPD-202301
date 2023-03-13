/*faca um codigo simples no VS que crie 10 threads e um vetor de
CEM MIL posicoes, popule o mesmo vetor na thread principal, crie
cada uma das 10 threads fazendo com que as mesmas concorram entre
si[OU SEJA, VC NAO PODE DIVIDIR O VETOR DE FORMA IGUALITARIA ENTRE
AS THREADS]  para somar as posicoes do vetor sem repeticoes das
posicoes do mesmo, cada thread soh pode somar uma posicao por vez,
USE MUTEX. [No video da aula tem praticamente isso feito]

Seu codigo deve, ALEM de expor o resultado final correto, imprimir
quantas posicoes de soma cada thread foi responsavel [e eh aqui o
problema real :D ]*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define num_threads 10
#define num_posicoes 100000
pthread_mutex_t mut;

void* sum_array(void* arg){
	//esse codigo separa a mesma quantidade pra cada thread
	int thread_part = part++;
	int *a_vector = (int*)arg;
	
	for(int i=thread_part * (num_posicoes/num_threads); i<(thread_part+1) * (num_posicoes/num_threads); i++){
		pthread_mutex_lock(&mut);
		sum += a_vector[i];
		pthread_mutex_unlock(&mut);
	}
	return NULL;
}

int main( int argc, char* argv[]){
	pthread_t threads[num_threads];
	int a[num_posicoes];
	
	
	pthread_mutex_init(&mut, NULL);
	//criando as threads
	for(int=0; i<num_threads; i++){
		pthread_create(&threads[i], NULL, sum_array, (void*)a;
	}
	
	//espera
	for(int i=0; i<threads; i++){
		pthread_join(threads[i], NULL);
	}
	
	//junta
	printf("resultado eh: %d\n", sum);
	
	return 0;
}