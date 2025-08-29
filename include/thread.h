/*
 * @file thread.h
 *
 *  Created on: Aug 27, 2025
 *      Author: Josu Alexandru
 *  This file provides a cross-platform abstraction for threading primitives such as mutexes,
 *  condition variables, and reader-writer locks. It supports both Windows and POSIX systems.
 *  The implementation uses Windows API on Windows and pthreads on POSIX systems.
 *  It includes functions to initialize, lock, unlock, and destroy these primitives.
 * 
*/

#ifndef THREAD_H
#define THREAD_H

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "typedefs.h"

// Thread structure
typedef struct {
    #if defined(_WIN32)
        HANDLE handle; // Windows thread handle
        DWORD thread_id; // Windows thread ID
    #else
        pthread_t thread; // POSIX thread
    #endif
}maio_thread_t;

// Mutex structure
typedef struct{
    #if defined(_WIN32)
        CRITICAL_SECTION cs; // Windows critical section
    #else
        pthread_mutex_t mutex; // POSIX mutex
    #endif
}maio_mutex_t;

// Condition variable structure
typedef struct{
    #if defined(_WIN32)
        CONDITION_VARIABLE cond; // Windows condition variable          
    #else
        pthread_cond_t cond;   // POSIX condition variable
    #endif
}maio_cond_t;

// Reader-writer lock structure
typedef struct {
    #if defined(_WIN32)
        SRWLOCK rw_lock; // Windows SRWLOCK
    #else
        pthread_rwlock_t rw_lock; // POSIX pthread_rwlock_t
    #endif
} maio_rwlock_t;

/*
* Thread functions
*/
extern u32 maio_thread_create(maio_thread_t* thread, void* (*start_routine)(void*), void* arg);
extern u32 maio_thread_join(maio_thread_t* thread);
extern u32 maio_thread_detach(maio_thread_t* thread);
extern void maio_thread_exit(void* retval);     

/*
* Mutex functions
*/
extern u32 maio_mutex_init(maio_mutex_t* mutex);
extern u32 maio_mutex_lock(maio_mutex_t* mutex);        
extern u32 maio_mutex_unlock(maio_mutex_t* mutex);
extern u32 maio_mutex_destroy(maio_mutex_t* mutex);

/*
* Condition variable functions
*/
extern u32 maio_cond_init(maio_cond_t* cond);
extern u32 maio_cond_wait(maio_cond_t* cond, maio_mutex_t* mutex);
extern u32 maio_cond_signal(maio_cond_t* cond);     
extern u32 maio_cond_broadcast(maio_cond_t* cond);
extern u32 maio_cond_destroy(maio_cond_t* cond);

/*
* Reader-writer lock functions
*/      
extern u32 maio_rwlock_init(maio_rwlock_t* lock);
extern u32 maio_rwlock_rdlock(maio_rwlock_t* lock);
extern u32 maio_rwlock_wrlock(maio_rwlock_t* lock);
extern u32 maio_rwlock_unlock(maio_rwlock_t* lock);
extern u32 maio_rwlock_destroy(maio_rwlock_t* lock);


#endif //THREAD_h
