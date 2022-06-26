
#include "monitor.h"
#include <stdio.h>


#define SIZE 10

class PCMonitor : Monitor
{
  private:
	int buffer[4][SIZE]; // ciasto, mieso, ser, kapusta
	int	count[4] = {0, 0, 0, 0};				// count - elements in buffer
	Condition Full[4], Empty[4];
  public: 
	PCMonitor();
	void put(int value, int which);
	char get(int which);
};

PCMonitor::PCMonitor()
{
}

void PCMonitor::put(int value, int which)
{
  enter();
  if(count[which]==SIZE)
	wait(Full[which]);
  buffer[which][count[which]]=value;
  count[which]++;
  switch (which)
  {
  case 0: 
  printf("Wyprodukowano ciasto %d\n", value);
  break;
  case 1:
  printf("Wyprodukowano mieso %d\n", value);
  break;
  case 2:
  printf("Wyprodukowano ser %d\n", value);
  break;
  case 3:
  printf("Wyprodukowano kapuste %d\n", value);
  break;
  }
  if(count[which] == 1)
  signal(Empty[which]);
  leave();
}

char PCMonitor::get(int which)
{
  enter();
  int value;
  if (count[which] == 0)
  wait(Empty[which]);
  value=buffer[which][count[which] - 1];
  count[which]--;
  if (count[which] == SIZE - 1)
  signal(Full[which]);
  leave();
  return value;
}

PCMonitor mon;

void* producer(void* args)
{
  int which_producer=*((int*)args);
  while(true)
  {
	int x = rand() % 100;
	mon.put(x, which_producer);
	sleep(1);
  }
}

void* consumer(void* args)
{
  int which_consumer=*((int*)args);
  while(true)
  {
  int y;
  y = mon.get(which_consumer + 1);
  y = y + mon.get(0); // ciasto obowiązkowe
  switch (which_consumer)
  {
  case 0: 
  printf("Wyprodukowano pieroga z miesem %d\n",  y);
  break;
  case 1:
  printf("Wyprodukowano pieroga z serem %d\n", y);
  break;
  case 2:
  printf("Wyprodukowano pieroga z kapusta %d\n", y);
  break;
  }
	sleep(1);
  }  
}

int main(int argc, char* argv[])
{
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
  //producer, consumer
  int a = 0; //ciasto, mieso
  int b = 1; //mieso, ser
  int c = 2; //ser, kapusta
  int d = 3; //kapusta
  int i;
  for (i=0; i < THREAD_NUM; i++) {
  int s;
  s = i % 7;
  switch (s)
  {
  case 0: if (pthread_create(&th[i], NULL, &producer, &a) != 0) {
    perror("Failed to create thread");
    }; break;
  case 1: if (pthread_create(&th[i], NULL, &producer, &b) != 0) {
    perror("Failed to create thread");
    }; break;
  case 2: if (pthread_create(&th[i], NULL, &producer, &c) != 0) {
    perror("Failed to create thread");
    }; break;
  case 3: if (pthread_create(&th[i], NULL, &producer, &d) != 0) {
    perror("Failed to create thread");
    }; break;
  case 4: if (pthread_create(&th[i], NULL, &consumer, &a) != 0) {
    perror("Failed to create thread");
    }; break;
  case 5: if (pthread_create(&th[i], NULL, &consumer, &b) != 0) {
    perror("Failed to create thread");
    }; break;
  case 6: if (pthread_create(&th[i], NULL, &consumer, &c) != 0) {
    perror("Failed to create thread");
    }; break;
  }
}
for (i = 0; i < THREAD_NUM; i++) {
  if (pthread_join(th[i], NULL) != 0) {
    perror("Failed to join thread");
  }
} 
  return 0;
}
// g++ -pthread -o main4 main.cpp