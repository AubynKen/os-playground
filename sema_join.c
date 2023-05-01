#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t p;
sem_t s;

void *child(void *arg) {
    sleep(5);
    printf("child\n");
    sem_post(&s);
    return NULL;
}

int main() {
    printf("parent: begin\n");
    sem_init(&s,0, 0);
    pthread_create(&p, NULL, child, NULL);
    sem_wait(&s);
    printf("parent: end\n");
    return 0;
}
