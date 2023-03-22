#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS_
#endif 

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

//acontece starvation as vezes

#define MAX_THREAD 4

//pacote de thread
typedef struct pkg_thread{
	int id;
	int count; //o contador de recursos
	int* shr_res;
	pthread_mutex_t* shr_mtx;
	sem_t* sgn_term;
} pkg_thread;

//codigo de cada thread
void* producer(void* arg) {
	pkg_thread* pkg = (pkg_thread*)arg;
	pkg->count = 0;
	while (1) {
		pthread_mutex_lock(pkg->shr_mtx);
		pkg->shr_res[0]++;
		pkg->count++;
		printf("produtor produziu\n");
		pthread_mutex_unlock(pkg->shr_mtx);
		if (pkg->count > 5000) {
			printf("indo liberar a principal\n");
			sem_post(pkg->sgn_term); //avisando a main pra terminar
			printf("liberando a principal\n");
			return NULL;
		}
	}
	return NULL;
}

void* consumer(void* arg) {
	pkg_thread* pkg = (pkg_thread*)arg;
	pkg->count = 0;
	while (1) {
		pthread_mutex_lock(pkg->shr_mtx);
		if (pkg->shr_res[0] > 0) {
			pkg->shr_res[0]--;
			pkg->count++;
			printf("consumidor %d consumiu\n", pkg->id);
		}
		else {
			printf("consumidor %d passou fome\n", pkg->id);
		}
		pthread_mutex_unlock(pkg->shr_mtx);
	}
	return NULL;
}

int main() {
	pthread_t threads[MAX_THREAD + 1]; //um vetor de 5
	pkg_thread pkg[MAX_THREAD + 1]; //um vetor de 5 pkgs | 1 producer e 4 consumers
	int i, resource = 0;
	pthread_mutex_t mtx; //mutex
	sem_t end; //semaforo

	pthread_mutex_init(&mtx, NULL); //inicializa o mutex naquele local de memória
	sem_init(&end, 0, 0); //inicializa o semaforo | 'binario' para finalizar a main

	//criando MAX_THREAD threads
	for (i = 1; i < MAX_THREAD + 1; i++) { //1 2 3 4 vão ser os consumers
		pkg[i].id = i;
		pkg[i].shr_res = &resource; //compartilham recursos
		pkg[i].shr_mtx = &mtx; //compartilham o mutex
		pthread_create(&threads[i], NULL, consumer, (void*)(pkg + i));
		//cria a thread do tipo consumer (&local de memória, NULL, rotina, argumento da rotina)
	}

	//assim estão tentando consumir antes mesmo de ter um produtor

	//declara o produce | posição 0 no vetor
	pkg[0].id = 0;
	pkg[0].shr_res = &resource;
	pkg[0].shr_mtx = &mtx;
	pkg[0].sgn_term = &end;
	pthread_create(&threads[0], NULL, producer, (void*)pkg);
	//cria a thread do producer
	sem_wait(&end); //esperando sinal do producer pra morrer

	for (int i = 0; i < MAX_THREAD + 1; i++) {
		printf("consumidor %d consumiu %d recursos\n", pkg[i].id, pkg[i].count);
	}

	//obviamente o código ainda da problema porque as threads estão
	//rodando tentando acessar variáveis que serao destruidas
	//assim que o main acabar, logo, tentaram acessar NULL e
	//com isso da errado.

	return 0;
}