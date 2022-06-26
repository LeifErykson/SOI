#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define SIZE 10 //N

sem_t semEmpty1, semEmpty2, semEmpty3, semEmpty4;
sem_t semFull1, semFull2, semFull3, semFull4;

pthread_mutex_t mutexBuffer1, mutexBuffer2, mutexBuffer3, mutexBuffer4;

int buffer1[SIZE], buffer2[SIZE], buffer3[SIZE], buffer4[SIZE]; //ciasto, mieso, ser, kapusta
int	count1 = 0, count2 = 0, count3 = 0, count4 = 0;

void* producer1(void* args) {
	while (1) {
		int x = rand() % 100;
		sleep(1);
		sem_wait(&semEmpty1);
		pthread_mutex_lock(&mutexBuffer1);
		if (count1 < SIZE) {
			buffer1[count1] = x;
			count1++;
		}
		printf("Wyprodukowano ciasto %d\n", x);
		pthread_mutex_unlock(&mutexBuffer1);
		sem_post(&semFull1);
	}
}

void* producer2(void* args) {
	while (1) {
		int x = rand() % 100;
		sleep(1);
		sem_wait(&semEmpty2);
		pthread_mutex_lock(&mutexBuffer2);
		if (count2 < SIZE) {
			buffer2[count2] = x;
			count2++;
		}
		printf("Wyprodukowano mieso %d\n", x);
		pthread_mutex_unlock(&mutexBuffer2);
		sem_post(&semFull2);
	}
}

void* producer3(void* args) {
	while (1) {
		int x = rand() % 100;
		sleep(1);
		sem_wait(&semEmpty3);
		pthread_mutex_lock(&mutexBuffer3);
		if (count3 < SIZE) {
			buffer3[count3] = x;
			count3++;
		}
		printf("Wyprodukowano ser %d\n", x);
		pthread_mutex_unlock(&mutexBuffer3);
		sem_post(&semFull3);
	}
}

void* producer4(void* args) {
	while (1) {
		int x = rand() % 100;
		sleep(1);
		sem_wait(&semEmpty4);
		pthread_mutex_lock(&mutexBuffer4);
		if (count4 < SIZE) {
			buffer4[count4] = x;
			count4++;
		}
		printf("Wyprodukowano kapuste %d\n", x);
		pthread_mutex_unlock(&mutexBuffer4);
		sem_post(&semFull4);
	}
}


void* consumer1(void* args) {	//mieso
	while (1) {
		int y;
		sem_wait(&semFull2);
		pthread_mutex_lock(&mutexBuffer2);
		sem_wait(&semFull1);
		pthread_mutex_lock(&mutexBuffer1);
		y = buffer1[count1 - 1] + buffer2[count2 - 1];
		count1 --;
		count2 --;
		pthread_mutex_unlock(&mutexBuffer1);
		sem_post(&semEmpty1);
		pthread_mutex_unlock(&mutexBuffer2);
		sem_post(&semEmpty2);
		printf("Zrobiono pieroga z miesem %d\n", y);
		sleep(1);
	}
}

void* consumer2(void* args) {	//ser
	while (1) {
		int y;
		sem_wait(&semFull3);
		pthread_mutex_lock(&mutexBuffer3);
		sem_wait(&semFull1);
		pthread_mutex_lock(&mutexBuffer1);
		y = buffer1[count1 - 1] + buffer3[count3 - 1];
		count1 --;
		count3 --;
		pthread_mutex_unlock(&mutexBuffer1);
		sem_post(&semEmpty1);
		pthread_mutex_unlock(&mutexBuffer3);
		sem_post(&semEmpty3);
		printf("Zrobiono pieroga z serem %d\n", y);
		sleep(1);
	}
}

void* consumer3(void* args) {	//kapusta
	while (1) {
		int y;
		sem_wait(&semFull4);
		pthread_mutex_lock(&mutexBuffer4);
		sem_wait(&semFull1);
		pthread_mutex_lock(&mutexBuffer1);
		y = buffer1[count1 - 1] + buffer4[count4 - 1];
		count1 --;
		count4 --;
		pthread_mutex_unlock(&mutexBuffer1);
		sem_post(&semEmpty1);
		pthread_mutex_unlock(&mutexBuffer4);
		sem_post(&semEmpty4);
		printf("Zrobiono pieroga z kapusta %d\n", y);
		sleep(1);
	}
}

int main(int argc, char* argv[]) {
	int threads = 0;
	if (argc < 2){
		printf("Trzeba podać argument (liczbę wątków)\n");
		return 0;
	}
	else{
		threads = atoi(argv[1]);
	}
	#define THREAD_NUM (threads)
	srand(time(NULL));
	pthread_t th[THREAD_NUM];
	pthread_mutex_init(&mutexBuffer1, NULL);
	pthread_mutex_init(&mutexBuffer2, NULL);
	pthread_mutex_init(&mutexBuffer3, NULL);
	pthread_mutex_init(&mutexBuffer4, NULL);
	sem_init(&semEmpty1, 0, SIZE); // SIZE - number of slots at the beggining
	sem_init(&semEmpty2, 0, SIZE);
	sem_init(&semEmpty3, 0, SIZE);
	sem_init(&semEmpty4, 0, SIZE);
	sem_init(&semFull1, 0, 0);
	sem_init(&semFull2, 0, 0);
	sem_init(&semFull3, 0, 0);
	sem_init(&semFull4, 0, 0);
	int i;
	for (i=0; i < THREAD_NUM; i++) {
		int s;
		s = i % 7;
		switch (s)
		{
		case 0: if (pthread_create(&th[i], NULL, &producer1, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 1: if (pthread_create(&th[i], NULL, &producer2, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 2: if (pthread_create(&th[i], NULL, &producer3, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 3: if (pthread_create(&th[i], NULL, &producer4, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 4: if (pthread_create(&th[i], NULL, &consumer1, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 5: if (pthread_create(&th[i], NULL, &consumer2, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		case 6: if (pthread_create(&th[i], NULL, &consumer3, NULL) != 0) {
			perror("Failed to create thread");
			}; break;
		}
	}
	for (i = 0; i < THREAD_NUM; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join thread");
		}
	}
	sem_destroy(&semEmpty1);
	sem_destroy(&semFull1);
	sem_destroy(&semEmpty2);
	sem_destroy(&semFull2);
	sem_destroy(&semEmpty3);
	sem_destroy(&semFull3);
	sem_destroy(&semEmpty4);
	sem_destroy(&semFull4);
	pthread_mutex_destroy(&mutexBuffer1);
	pthread_mutex_destroy(&mutexBuffer2);
	pthread_mutex_destroy(&mutexBuffer3);
	pthread_mutex_destroy(&mutexBuffer4);
	return 0;
}
// gcc -pthread -o main3 main.c