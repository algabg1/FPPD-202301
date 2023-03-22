#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
/*Exemplo 2: Escopo de variáveis*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global; //variavel global
void* thr_func(void* arg); //define a função

int main(void) {
	pthread_t tid; //cria a thread principal
	global = 20;
	printf("thread princial: %d\n", global);
	pthread_create(&tid, NULL, thr_func, NULL); //função que cria um thread, chamado thr_func que define os parametros da thread
	pthread_join(tid, NULL); //pede pra thread tid terminar
	printf("thread principal: %d\n", global);
	system("pause");
	return 0;
}

void* thr_func(void* arg) {
	global = 40;
	printf("novo thread: %d\n", global);
	return NULL;
}