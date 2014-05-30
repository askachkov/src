/* 
 * File:   PosixMutex.h
 * Author: alexey
 *
 * Created on May 28, 2014, 2:19 PM
 */

#ifndef POSIXMUTEX_H
#define	POSIXMUTEX_H

#include <pthread.h>

#ifndef POSIX_SYNCHRONIZE
#define POSIX_SYNCHRONIZE(MUTEX) Posix::Mutex::ScopedLock loker(MUTEX);
#endif

namespace Posix
{
    class Mutex
    {
        public:
            Mutex();
            virtual ~Mutex();
            void lock();
            bool tryLock();
            void unlock();    

        private:
            pthread_mutex_t m_Mutex;
            
        public:
            class ScopedLock
            {
                public:
                    ScopedLock(Mutex * mutex);
                    ~ScopedLock();

                private:        
                    ScopedLock(const ScopedLock &);
                    void operator=(const ScopedLock&);
                    Mutex* m_Mutex;
            };
    };
}

#endif	/* POSIXMUTEX_H */

