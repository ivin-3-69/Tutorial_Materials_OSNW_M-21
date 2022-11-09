#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
using namespace std;
#include <semaphore.h>
#define debug(x) cout << #x << " : " << x << endl

queue<int> q;
int CAPACITY;
int total_goods_produced_by_producer;

int use = 0;
int fill = 0;

sem_t empty1;
sem_t full;
sem_t mutex;
#define CMAX (10)
int consumers;

void do_fill(int value)
{
    q.push(value);
}

int do_get()
{
    int tmp = q.front();
    q.pop();
    return tmp;
}
#define CORRECT 1

#ifdef CORRECT

////////////////////  CORRECT SOLN ////////////////////////////
void *producer(void *arg)
{
    int i;
    for (i = 0; i < total_goods_produced_by_producer; i++)
    {
        sem_wait(&empty1);
        sem_wait(&mutex);
        do_fill(i);
        sem_post(&mutex);
        sem_post(&full);
    }

    // end case
    for (i = 0; i < consumers; i++)
    {
        sem_wait(&empty1);
        sem_wait(&mutex);
        do_fill(-1);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int tmp = 0;
    while (tmp != -1)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        tmp = do_get();
        // you can check that q.size() will never exceed CAPACITY
        //  printf("Queue size is %d\n", (int)q.size());
        sem_post(&mutex);
        sem_post(&empty1);
        if (tmp == -1)
        {
            break;
        }
        printf("Consumer %lld fetched item %d\n", (long long int)arg, tmp);
    }
    return NULL;
}
#endif

int main(int argc, char *argv[])
{

    int i;
    // capacity = max number of goods we allow to be present in the queue at any given time
    CAPACITY = 10;
    total_goods_produced_by_producer = 100;
    consumers = 3;
    assert(consumers <= CMAX);

    sem_init(&empty1, 0, CAPACITY); // CAPACITY are empty1
    sem_init(&full, 0, 0);          // 0 are full
    sem_init(&mutex, 0, 1);         // mutex

    pthread_t pid, cid[CMAX];

    pthread_create(&pid, NULL, producer, NULL);

    for (i = 0; i < consumers; i++)
    {
        pthread_create(&cid[i], NULL, consumer, (void *)(long long int)i);
    }

    pthread_join(pid, NULL);
    for (i = 0; i < consumers; i++)
    {
        pthread_join(cid[i], NULL);
    }
    return 0;
}