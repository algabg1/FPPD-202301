#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

/*Exemplo 4: Usando Mutex*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 1000
#define NTHREADS 10

int a[TAM];
int global_index = 0;
int sum = 0;
pthread_mutex_t mutex1; //declaração do mutex | mutex1 é o objeto

void* slave(void* nenhum) {
	int local_index, partial_sum = 0;
	do {
		pthread_mutex_lock(&mutex1);
		//deu lock no objeto mutex1 | o local de memória do mutex1 não ta disponível
		local_index = global_index;
		global_index++;
		pthread_mutex_unlock(&mutex1);
		//unlock no mutex1, agora o local de memória ta disponível

		if (local_index < TAM) partial_sum += a[local_index];
	} while (local_index < TAM); //vai fazer isso aqui 1000 x

	pthread_mutex_lock(&mutex1); //lock no local de memória do mutex1
	sum += partial_sum;
	pthread_mutex_unlock(&mutex1); //unlock no local de memória do mutex1

	return(NULL);
}

main() {
	int i;
	pthread_t thread[NTHREADS]; //cria 10 threads

	pthread_mutex_init(&mutex1, NULL);
	//inicializa a referencia pro local de memoria do mutex1

	//colocando valores no a[] de 1 a 1000
	for (i = 0; i < TAM; i++) {
		a[i] = i + 1;
	}

	for (i = 0; i < NTHREADS; i++) {
		//cria o slave no local de memoria da thread[i] e ve se retorna 0
		if (pthread_create(&thread[i], NULL, slave, NULL) != 0){
			perror("pthread_create falhou!");
			exit(-1);
		}
	}

	for (i = 0; i < NTHREADS; i++) {
		if (pthread_join(thread[i], NULL) != 0) {
			perror("pthread_join falhou!");
			exit(-2);
		}
	}

	printf("a soma e %d\n", sum);
	system("pause");
}
