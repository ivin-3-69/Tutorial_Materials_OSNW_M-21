// source: GFG

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct arg_struct
{
    int arg1;
    int arg2;
    int arg3;
};

struct arg_struct arg_struct_array[5];

sem_t curr_sem;
void *thread(void *arguments)
{
    struct arg_struct *args = arguments;
    sem_wait(&curr_sem);
    printf("Entering, the thread id = %d\n", args->arg3); // get current thread id

    printf("my washing time is %d\n", args->arg1);
    printf("my patience time is %d\n", args->arg2);
    sleep(args->arg1);

    // signal
    printf("\nExiting, the thread id = %d\n=====\n", args->arg3);
    sem_post(&curr_sem);
    return NULL;
}

int main()
{
    sem_init(&curr_sem, 0, 2);
    pthread_t t1, t2, t3, t4, t5;

    sleep(2);
    arg_struct_array[0].arg1 = 9;
    arg_struct_array[0].arg2 = 6;
    arg_struct_array[0].arg3 = 2;
    pthread_create(&t1, NULL, thread, (void *)&arg_struct_array[0]);

    sleep(1);
    arg_struct_array[1].arg1 = 4;
    arg_struct_array[1].arg2 = 3;
    arg_struct_array[1].arg3 = 3;
    pthread_create(&t2, NULL, thread, (void *)&arg_struct_array[1]);

    sleep(3);
    arg_struct_array[2].arg1 = 3;
    arg_struct_array[2].arg2 = 5;
    arg_struct_array[2].arg3 = 6;
    pthread_create(&t3, NULL, thread, (void *)&arg_struct_array[2]);

    arg_struct_array[3].arg1 = 5;
    arg_struct_array[3].arg2 = 2;
    arg_struct_array[3].arg3 = 6;
    pthread_create(&t4, NULL, thread, (void *)&arg_struct_array[3]);

    sleep(2);
    arg_struct_array[4].arg1 = 5;
    arg_struct_array[4].arg2 = 2;
    arg_struct_array[4].arg3 = 8;
    pthread_create(&t5, NULL, thread, (void *)&arg_struct_array[4]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    sem_destroy(&curr_sem);
    return 0;
}
