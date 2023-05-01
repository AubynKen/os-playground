#define BUF_SIZE 20
#define LOOPS 100

#include <stdio.h>
#include <semaphore.h>

sem_t empty;
sem_t full;
int buffer[BUF_SIZE];
int enqueue_ptr = 0;
int dequeue_ptr = 0;

int get() {
    int value = buffer[dequeue_ptr];
    dequeue_ptr = dequeue_ptr % BUF_SIZE;
    return value;
}

void put(int val) {
    buffer[enqueue_ptr] = val;
    enqueue_ptr++;
}

void *producer(void *arg) {
    for (int i = 0; i < LOOPS; ++i) {
        // we need an empty stop to produce data without overflow
        sem_wait(&empty);
        put(i);
        printf("producer just produced data\n");
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < LOOPS; ++i) {
        sem_wait(&full);
        int value = get();
        sem_post(&empty);
        printf("consumer received value: %d\n", value);
    }
    return NULL;
}