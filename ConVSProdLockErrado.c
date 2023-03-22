#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//acontece starvation as vezes
#define MAX_THREAD 4

typedef struct pkg_thread {
	int id;
	int count;
	int* shr_res;
	pthread_mutex_t* shr_mtx;
	pthread_mutex_t* sgn_term;
}pkg_thread;

//codigo de cada thread
void* producer(void* arg) {
	pkg_thread* pkg = (pkg_thread*)arg;
	pkg->count = 0;
	while (1) {
		pthread_mutex_lock(pkg->shr_mtx);
		pkg->shr_res[0]++;
		pkg->count++;
		//printf("produtor produziu\n");
		pthread_mutex_unlock(pkg->shr_mtx);
		if (pkg->count > 5000) {
			printf("indo liberar a principal\n");
			pthread_mutex_unlock(pkg->sgn_term);
			//mutex só pode ser liberado por quem lockou, furo grave aqui
			printf("liberando a principal\n");
			sleep(2);
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
			//printf("consumidor %d consumiu\n", pkg->id);
		}
		else {
			//printf("consumidor %d passou fome\n", pkg->id);
		}
		pthread_mutex_unlock(pkg->shr_mtx);
	}
	return NULL;
}

int main() {
	pthread_t threads[MAX_THREAD + 1];
	pkg_thread pkg[MAX_THREAD + 1];
	int i, resource = 0;
	pthread_mutex_t mtx;
	pthread_mutex_t end;

	pthread_mutex_init(&mtx, NULL);
	pthread_mutex_init(&end, NULL);
	
	//criando MAX-THREAD THREADS
	for (i = 1; i < MAX_THREAD + 1; i++) {
		pkg[i].id = i;
		pkg[i].shr_res = &resource;
		pkg[i].shr_mtx = &mtx;
		pthread_create(&threads[i], NULL, consumer, (void*)(pkg + i));
	}

	pkg[0].id = 0;
	pkg[0].shr_res = &resource;
	pkg[0].shr_mtx = &mtx;
	pkg[0].sgn_term = &end;
	pthread_create(&threads[0], NULL, producer, (void*)pkg);

	pthread_mutex_lock(&end);
	printf("lockei\n");
	printf("alguem vai liberar o novo lock \n");
	pthread_mutex_lock(&end);
	printf("alguem liberou o lock\n");
	//mutex só pode ser liberado por quem lockou, furo grave aqui
	sleep(5);

	for (int i = 1; i < MAX_THREAD + 1; i++) {
		printf("consumidor %d consumiu %d recursos\n", pkg[i].id, pkg[i].count);
	}
	return 0;
}
