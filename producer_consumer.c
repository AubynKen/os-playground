#include <stdio.h>
#include <pthread.h>

const int loops = 50;

pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t filled = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int buffer; // queue with capacity=1
int count = 0;

/**
 * put puts an element into the queue
 * @param element: the element to put into the queue
 */
void put(int element) {
    buffer = element;
    count += 1;
}

/**
 * get retrieves an element from the queue
 * @return the retrieved element
 */
int get() {
    count -= 1;
    return buffer;
}

/**
 * producer thread for producing elements
 * @return NULL
 */
void *producer(void *_) {
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&m);
        while (count > 0) {
            pthread_cond_wait(&empty, &m);
        }
        put(i);
        pthread_cond_signal(&filled);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

/**
 * consumer thread function for consuming elements
 * @return NULL
 */
void *consumer(void *_) {
    for (int i = 0; i < loops; ++i) {
        pthread_mutex_lock(&m);
        while (count == 0) {
            pthread_cond_wait(&filled, &m);
        }
        printf("got element: %d\n", get());
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

/**
 * rename to main before running
 */
int main() {
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}