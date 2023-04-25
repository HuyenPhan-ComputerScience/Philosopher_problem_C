#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER 5
#define MAX_SLEEP_TIME 5
pthread_mutex_t lock;
pthread_cond_t condition[NUMBER];

enum
{
    THINKING,
    HUNGRY,
    EATING
} state[NUMBER];

void thinking(int number)
{
    sleep(rand() % MAX_SLEEP_TIME);
}

void eating(int id)
{
    printf("Philosopher %d is eating\n", id);
    sleep(rand() % MAX_SLEEP_TIME);
}

int left_neighbor(int number)
{
    if (number == 0)
    {
        return NUMBER - 1;
    }
    else
    {
        return number - 1;
    }
}

int right_neighbor(int number)
{
    if (number == NUMBER - 1)
    {
        return 0;
    }
    else
    {
        return number + 1;
    }
}

void test(int id)
{
    if (state[id] == HUNGRY && state[left_neighbor(id)] != EATING && state[right_neighbor(id)] != EATING)
    {
        state[id] = EATING;
        pthread_cond_signal(&condition[id]);
    }
}

void pickup_forks(int id)
{
    pthread_mutex_lock(&lock);
    state[id] = HUNGRY;
    test(id);
    while (state[id] != EATING)
    {
        pthread_cond_wait(&condition[id], &lock);
    }
    pthread_mutex_unlock(&lock);
}

void return_forks(int id)
{
    pthread_mutex_lock(&lock);
    printf("Philosopher %d is returning forks\n", id);
    state[id] = THINKING;
    test(left_neighbor(id));
    test(right_neighbor(id));
    pthread_mutex_unlock(&lock);
}

void *philosopher(void *param)
{
    int *lnumber = (int *)param;
    int number = *lnumber;
    int times_through_loop = 0;
    sleep(rand() % MAX_SLEEP_TIME);
    while (times_through_loop < 5)
    {
        printf("Philosopher %d is thinking\n", number);
        printf(" left is ");
        if (state[left_neighbor(number)] == THINKING)
        {
            printf("THINKING");
        }
        else if (state[left_neighbor(number)] == HUNGRY)
        {
            printf("HUNGRY");
        }
        else
        {
            printf("EATING");
        }

        printf(" right is ");
        if (state[right_neighbor(number)] == THINKING)
        {
            printf("THINKING");
        }
        else if (state[right_neighbor(number)] == HUNGRY)
        {
            printf("HUNGRY");
        }
        else
        {
            printf("EATING");
        }
        printf("\n");
        thinking(number);
        pickup_forks(number);
        eating(number);
        return_forks(number);
        times_through_loop++;
    }
}

int thread_id[NUMBER];
int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_t t_id[NUMBER];
    for (int i = 0; i < NUMBER; i++)
    {
        state[i] = THINKING;
        thread_id[i] = i;
        pthread_cond_init(&condition[i], NULL);
    }

    for (int i = 0; i < NUMBER; i++)
    {
        pthread_create(&t_id[i], NULL, philosopher, (void *)&thread_id[i]);
    }

    for (int i = 0; i < NUMBER; i++)
    {
        pthread_join(t_id[i], NULL);
    }
    return 0;
}