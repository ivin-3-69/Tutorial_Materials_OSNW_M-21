// source: GFG
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t cnt_lock;
int people_fucked = 0;
struct arg_struct
{
    int arg1;
    int arg2;
    int arg3;
    int arg4;
};
int machinenum = 2;
int total_time_wasted = 0;
struct arg_struct arg_struct_array[100];

sem_t curr_sem;
void *thread(void *arguments)
{
    struct arg_struct *args = arguments;
    int count = 0;
    int arrive_time = args->arg1;
    int wash_time = args->arg2;
    int patience = args->arg3;
    int p = patience;
    int indexs = args->arg4;
    printf("%d : Student %d arrives\n", arrive_time, indexs);
    while (patience >= 0)
    {
        if (machinenum > 0)
        {
            sem_wait(&curr_sem);
            pthread_mutex_lock(&cnt_lock);
            machinenum--;
            pthread_mutex_unlock(&cnt_lock);
            printf("%d : Student %d starts washing\n", arrive_time + count, indexs);
            sleep(wash_time);
            printf("%d : Student %d leaves after washing\n", arrive_time + count + wash_time, indexs);
            machinenum++;
            sem_post(&curr_sem);
            return NULL;
        }
        sleep(1);
        count++;
        patience--;
        total_time_wasted++;
    }
    printf("%d : Student %d leaves without getting his clothes washed\n", arrive_time + p, indexs);
    people_fucked++;
    return NULL;
}

int main()
{
    sem_init(&curr_sem, 0, 2);
    pthread_t t1, t2, t3, t4, t5;
    pthread_mutex_init(&cnt_lock, NULL);

    sleep(2);
    arg_struct_array[0].arg1 = 2;
    arg_struct_array[0].arg2 = 9;
    arg_struct_array[0].arg3 = 6;
    arg_struct_array[0].arg4 = 4;
    pthread_create(&t1, NULL, thread, (void *)&arg_struct_array[0]);

    sleep(1);
    arg_struct_array[1].arg1 = 3;
    arg_struct_array[1].arg2 = 4;
    arg_struct_array[1].arg3 = 3;
    arg_struct_array[1].arg4 = 2;
    pthread_create(&t2, NULL, thread, (void *)&arg_struct_array[1]);

    sleep(3);
    arg_struct_array[2].arg1 = 6;
    arg_struct_array[2].arg2 = 3;
    arg_struct_array[2].arg3 = 5;
    arg_struct_array[2].arg4 = 1;
    pthread_create(&t3, NULL, thread, (void *)&arg_struct_array[2]);

    arg_struct_array[3].arg1 = 6;
    arg_struct_array[3].arg2 = 5;
    arg_struct_array[3].arg3 = 2;
    arg_struct_array[3].arg4 = 3;
    pthread_create(&t4, NULL, thread, (void *)&arg_struct_array[3]);

    sleep(2);
    arg_struct_array[4].arg1 = 8;
    arg_struct_array[4].arg2 = 5;
    arg_struct_array[4].arg3 = 2;
    arg_struct_array[4].arg4 = 5;
    pthread_create(&t5, NULL, thread, (void *)&arg_struct_array[4]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    sem_destroy(&curr_sem);
    pthread_mutex_destroy(&cnt_lock);
    printf("%d\n", people_fucked);
    printf("%d\n", total_time_wasted - people_fucked);
    return 0;
}
