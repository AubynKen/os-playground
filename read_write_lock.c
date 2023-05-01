#include <semaphore.h>
//#include <stdio.h>


typedef struct {
    int readers;
    sem_t *lock;
    sem_t *write;
} rwlock;

void rwlock_init(rwlock *rw) {
    sem_init(rw->lock, 0, 1);
    sem_init(rw->write, 0, 1);
}

// lock for write and read
void lock(rwlock *rw) {
    sem_wait(rw->write);
}

// unlock for write and read
void unlock(rwlock *rw) {
    sem_post(rw->write);
}

// read-lock, can be acquired by multiple threads
void rlock(rwlock *rw) {
    sem_wait(rw->lock);
    rw->readers++;
    if (rw->readers == 1) {
        sem_wait(rw->write);
    }
    sem_post(rw->lock);
}

// unlock read-lock
void runlock(rwlock *rw) {
    sem_wait(rw->lock);
    rw->readers--;
    if (rw->readers == 0) {
        sem_post(rw->write);
    }
    sem_post(rw->lock);
}