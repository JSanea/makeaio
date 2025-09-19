/*
 * @file queue.h
 *
 *  Created on: Jul 3, 2025
 *      Author: Josu Alexandru
 * 
 * This file defines a simple queue data structure that can be used to store tasks in a thread pool.
 * It provides functions to initialize the queue, enqueue and dequeue elements.
 * 
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "typedefs.h"

typedef struct queue_node queue_node_t;

struct queue_node{
    void* data;
    queue_node_t* next;
    queue_node_t* prev;
};

typedef struct queue{
    queue_node_t* front;
    queue_node_t* rear;
    u32 count;
}queue_t;

static inline void queue_init(queue_t* q){
    q -> front = NULL;
    q -> rear = NULL;
    q -> count = 0;
}

static inline void enqueue(queue_t* q, void* data){
    if(q == NULL || data == NULL) return;

    queue_node_t* n = (queue_node_t*)malloc(sizeof(queue_node_t));

    if (n == NULL) return;

    n->data = data;
    n->next = NULL;
    n->prev = q->rear;

    if(q->count == 0){
        q->front = n;
        q->rear = n;
    }else{
        q->rear->next = n;
        q->rear = n;
    }

    q->count++;
}

static inline void* dequeue(queue_t* q){
    if(q == NULL || q->count == 0) return NULL;

    queue_node_t* front_node = q->front;
    void* data = front_node->data;

    q->front = front_node->next;
    if (q->front != NULL) {
        q->front->prev = NULL;
    } else {
        q->rear = NULL;
    }

    q->count--;

    free(front_node);

    return data;
}

static inline void queue_destroy(queue_t* q){
    if (q == NULL) return;

    queue_node_t* current = q->front;

    while (current != NULL) {
        queue_node_t* temp = current;
        current = current->next;
        free(temp);
        temp = NULL;
    }

    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

static inline bool is_empty(queue_t* q){
    if (q == NULL) return true;
    return q->count <= 0;
}

static inline int get_size(queue_t* q){
    return q->count;
}

#endif





























