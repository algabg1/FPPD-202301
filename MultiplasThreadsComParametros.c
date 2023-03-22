#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

/*Exemplo 3: Multiplas threads com parametros*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
	int tid;
	tid = (int)threadid; //a thread vai receber o mesmo id de quando foi lida na main
	printf("Hello world! its me, thread #%d!\n", tid);
	pthread_exit(NULL); //da exit na thread
}

int main(int argc, char *argv[]){
	pthread_t threads[NUM_THREADS]; //cria um thread[vetor] = varias threads
	int rc, t;

	for (t = 0; t < NUM_THREADS; t++) { //vai passar pelas 5 threads
		printf("in main: creating thread %d\n", t);
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
		//vai fazer o printhello com t como parametro no local de memória da thread lida
		if (rc) {
			printf("ERROR code is %d\n", rc);
			exit(-1);
		}
	}

	system("pause");

	pthread_exit(NULL); //da exit na thread
}