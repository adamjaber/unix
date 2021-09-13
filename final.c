#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct food
{
    bool left;
    bool right;
    bool *stick;
    int size;
    int thread_num;
    int prev;
    int next;

} food;

void *foo(void *f)
{
    int i, eatTime=0, ThinkTime=0;

    struct food *x = (struct food *)f;

   // printf("phil num %d, %d\n", x->thread_num, x->thread_num);

    while (1)
    {

        pthread_mutex_lock(&mutex);
        if ((x->stick[x->thread_num] == 0) && (x->stick[x->prev]==0) )
        {
            x->stick[x->thread_num] = 1;
            x->left = 1;
            printf("Tread number %d has a stick on left hand number %d\n", x->thread_num, x->thread_num);
            pthread_mutex_unlock(&mutex);
        }
        else if(x->stick[x->thread_num+1]==0 && x->left == 1 && x->right==0)
        {
            x->right=1;
            x->stick[x->thread_num+1]==1;
            printf("Tread number %d has a stick on right hand number %d\n", x->thread_num, x->thread_num+1);
            pthread_mutex_unlock(&mutex);
            if ((x->left == 1) && (x->right == 1))
            {
                printf("Thread number %d is eating with sticks numbwer %d and %d for %d seconds \n", x->thread_num, x->thread_num,x->thread_num+1, eatTime);
                eatTime = rand() % 10 + 1;
                sleep(eatTime);
                printf("Thread number %d has funished eating with sticks number %d and %d \n", x->thread_num, x->thread_num, x->thread_num+1);
                pthread_mutex_lock(&mutex);
                x->stick[x->thread_num] = 0;
                x->stick[x->thread_num+1] = 0;
                x->right=0;
                x->left=0;
                pthread_mutex_unlock(&mutex);
                ThinkTime = rand() % 10 + 11;
                printf("Thread number %d is Thinking after eating for %d seconds\n",x->thread_num ,ThinkTime);
                sleep(ThinkTime);
            }

        }
        else
        {
            pthread_mutex_unlock(&mutex);
        }
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    int num = atoi(argv[1]);
    printf("%d",num);

    int count = 0;
    bool *stick;
    pthread_t *tid = (pthread_t *)malloc(num * sizeof(pthread_t));
    struct food *phils = (struct food *)calloc(num, sizeof(struct food));
    stick = (bool *)calloc(num, sizeof(bool));

    for (int i = 0; i < num; i++)
    {
        phils[i].size = num;
        phils[i].thread_num = i;
        phils[i].stick = stick;

        if(i==0)
            phils[i].prev=num-1;
        else
            phils[i].prev=i-1;
        if(i==num-1)
            phils[i].next=0;
        else
            phils[i].next=i+1;

        if (pthread_create(&tid[i], NULL, foo, &phils[i]))
            return 1;
    }
    for (int i = 0; i <num; i++)
    {
        pthread_join(tid[i], NULL);
    }


    return 0;
}