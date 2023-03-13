#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*faca um codigo simples no VS que crie 5 threads
e execute todas elas imprimindo seu proprio ID de
forma a visualizar a desordem das impressoes fazendo
com q o codigo principal espere ate que todas as 5
threads terminem.

ps: somente cria UMA funcao thread, nao vale se usar
5 funcoes de thread.*/

#define num_threads 5
pthread_mutex_t mut;

void* printThread(void* thread_id) {
	int id = (int)thread_id;
	pthread_mutex_lock(&mut);
	printf("thread id: %d\n", id);
	pthread_mutex_unlock(&mut);
}

int main(int argc, char* argv[]) {
	pthread_t threads[num_threads];
	int i;
	pthread_mutex_init(&mut, NULL);

	printf("COMECA PRINCIPAL\n");
	for (i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, printThread, (void*)i);
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("TERMINA PRINCIPAL\n");

	system("pause");
	pthread_exit(NULL);
}
