/* 
 * File:   PosixMutex.cpp
 * Author: alexey
 * 
 * Created on May 28, 2014, 2:19 PM
 */

#include "Mutex.h"


namespace Posix
{
    Mutex::Mutex()
    {
        pthread_mutex_init(&m_Mutex, NULL);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(&m_Mutex);
    }

    void Mutex::lock() const
    {
        pthread_mutex_lock(&m_Mutex);
    }

    bool Mutex::tryLock() const
    {
        return pthread_mutex_trylock(&m_Mutex) == 0;
    }

    void Mutex::unlock() const
    {
        pthread_mutex_unlock(&m_Mutex);
    }

    Mutex::ScopedLock::ScopedLock(const Mutex * mutex): m_Mutex(mutex)
    {
        m_Mutex->lock();
    }

    Mutex::ScopedLock::~ScopedLock()
    {
        m_Mutex->unlock();
    }
}
