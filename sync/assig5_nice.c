#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#define ARRSIZE(arr) (sizeof(arr) / sizeof(*(arr)))

pthread_mutex_t cnt_lock;
int people_fucked = 0;
struct arg_struct
{
    int arg1;
    int arg2;
    int arg3;
    int arg4;
};
int machinenum;
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

int compare(const void *a, const void *b)
{
    int x1 = *(const int *)a;
    int x2 = *(const int *)b;
    if (x1 > x2)
        return 1;
    if (x1 < x2)
        return -1;
    int y1 = *(((const int *)a) + 3);
    int y2 = *(((const int *)b) + 3);
    if (y1 > y2)
        return 1;
    if (y1 < y2)
        return -1;
    return 0;
}

int main()
{
    int n, m;
    scanf("%d", &n);
    scanf("%d", &m);
    machinenum = m;
    sem_init(&curr_sem, 0, m);
    pthread_t t[n];
    pthread_mutex_init(&cnt_lock, NULL);
    int inputmat[n][4];
    for (int k = 0; k < n; k++)
    {
        scanf("%d", &inputmat[k][0]);
        scanf("%d", &inputmat[k][1]);
        scanf("%d", &inputmat[k][2]);
        inputmat[k][3] = k + 1;
    }
    qsort(inputmat, ARRSIZE(inputmat), sizeof(*inputmat), compare);

    // printf("the input matrix is \n");
    // for (int k = 0; k < n; k++)
    // {
    //     printf("%d %d %d\n", inputmat[k][0], inputmat[k][1], inputmat[k][2]);
    //     // scanf("%d", &inputmat[k][0]);
    //     // scanf("%d", &inputmat[k][1]);
    //     // scanf("%d", &inputmat[k][2]);
    // }

    int sleep_time = 0;

    for (int k = 0; k < n; k++)
    {
        sleep(inputmat[k][0] - sleep_time);
        sleep_time = inputmat[k][0];
        arg_struct_array[k].arg1 = inputmat[k][0];
        arg_struct_array[k].arg2 = inputmat[k][1];
        arg_struct_array[k].arg3 = inputmat[k][2];
        arg_struct_array[k].arg4 = inputmat[k][3];
        pthread_create(&t[k], NULL, thread, (void *)&arg_struct_array[k]);
    }

    // sleep(inputmat[0][0] - sleep_time);
    // sleep_time = inputmat[0][0];
    // arg_struct_array[0].arg1 = inputmat[0][0];
    // arg_struct_array[0].arg2 = inputmat[0][1];
    // arg_struct_array[0].arg3 = inputmat[0][2];
    // arg_struct_array[0].arg4 = inputmat[0][3];
    // pthread_create(&t[0], NULL, thread, (void *)&arg_struct_array[0]);

    // sleep(inputmat[1][0] - sleep_time);
    // sleep_time = inputmat[1][0];
    // arg_struct_array[1].arg1 = inputmat[1][0];
    // arg_struct_array[1].arg2 = inputmat[1][1];
    // arg_struct_array[1].arg3 = inputmat[1][2];
    // arg_struct_array[1].arg4 = inputmat[1][3];
    // pthread_create(&t[1], NULL, thread, (void *)&arg_struct_array[1]);

    // sleep(inputmat[2][0] - sleep_time);
    // sleep_time = inputmat[2][0];
    // arg_struct_array[2].arg1 = inputmat[2][0];
    // arg_struct_array[2].arg2 = inputmat[2][1];
    // arg_struct_array[2].arg3 = inputmat[2][2];
    // arg_struct_array[2].arg4 = inputmat[2][3];
    // pthread_create(&t[2], NULL, thread, (void *)&arg_struct_array[2]);

    // sleep(inputmat[3][0] - sleep_time);
    // sleep_time = inputmat[3][0];
    // arg_struct_array[3].arg1 = inputmat[3][0];
    // arg_struct_array[3].arg2 = inputmat[3][1];
    // arg_struct_array[3].arg3 = inputmat[3][2];
    // arg_struct_array[3].arg4 = inputmat[3][3];
    // pthread_create(&t[3], NULL, thread, (void *)&arg_struct_array[3]);

    // sleep(inputmat[4][0] - sleep_time);
    // sleep_time = inputmat[4][0];
    // arg_struct_array[4].arg1 = inputmat[4][0];
    // arg_struct_array[4].arg2 = inputmat[4][1];
    // arg_struct_array[4].arg3 = inputmat[4][2];
    // arg_struct_array[4].arg4 = inputmat[4][3];
    // pthread_create(&t[4], NULL, thread, (void *)&arg_struct_array[4]);

    for (int k = 0; k < n; k++)
    {
        pthread_join(t[k], NULL);
    }
    // pthread_join(t[0], NULL);
    // pthread_join(t[1], NULL);
    // pthread_join(t[2], NULL);
    // pthread_join(t[3], NULL);
    // pthread_join(t[4], NULL);
    sem_destroy(&curr_sem);
    pthread_mutex_destroy(&cnt_lock);
    printf("%d\n", people_fucked);
    printf("%d\n", total_time_wasted - people_fucked);
    return 0;
}