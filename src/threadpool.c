/*
    * @file threadpool.c
    *
    *  Created on: 2025-08-28
    *      Author: Josu Alexandru
*/

#include "maio.h"
#include "threadpool.h"

static maio_threadpool_t pool;


static void* maio_worker(void* arg){
    (void)arg; 

    while(true){
        maio_mutex_lock(&pool->lock);

        bool empty = is_empty(&pool->task_queue);
        bool shutting_down = pool->shut_down;

        while(empty && !shutting_down){
            maio_cond_wait(&pool->notify, &pool->lock);
        }

        if(empty && shutting_down){
            maio_mutex_unlock(&pool->lock);
            break;
        }

        maio_task_t* task = (maio_task_t*)dequeue(&pool->task_queue);

        maio_mutex_unlock(&pool->lock);

        if(task){
            task->func(task->arg);
            free(task);
        }
    }

    return NULL;
}

u32 maio_threadpool_init(){
    pool.thread_count = MAIO_MAX_THREADS;
    pool.shut_down = false;

    pool.task_queue = (queue_t*)malloc(sizeof(queue_t));
    queue_init(pool.task_queue);

    if(maio_mutex_init(&pool.lock) != 0) exit(EXIT_FAILURE);

    if(maio_cond_init(&pool.notify) != 0) exit(EXIT_FAILURE);

    pool.threads = (maio_thread_t*)malloc(sizeof(maio_thread_t) * pool.thread_count);

    for(u32 i = 0; i < pool.thread_count; i++){
        if(maio_thread_create(&pool.threads[i], maio_worker, NULL) != 0){
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

u32 maio_threadpool_submit(void (*func)(void*), void* arg){
    if(func == NULL) return -1;

    maio_task_t* task = (maio_task_t*)malloc(sizeof(maio_task_t));
    if(task == NULL) return -1;

    task->func = func;
    task->arg = arg;

    maio_mutex_lock(&pool.lock);

    enqueue(pool.task_queue, task);

    maio_cond_signal(&pool.notify);
    maio_mutex_unlock(&pool.lock);

    return 0;
}

u32 maio_threadpool_shutdown(){
    maio_mutex_lock(&pool.lock);
    pool.shut_down = true;
    maio_cond_broadcast(&pool.notify);
    maio_mutex_unlock(&pool.lock);

    for(u32 i = 0; i < pool.thread_count; i++){
        maio_thread_join(pool.threads[i], NULL);
    }

    free(pool.threads);
    queue_destroy(pool.task_queue);
    free(pool.task_queue);

    maio_mutex_destroy(&pool.lock);
    maio_cond_destroy(&pool.notify);

    return 0;
}



