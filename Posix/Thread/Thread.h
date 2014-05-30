/* 
 * File:   PosixThread.h
 * Author: alexey
 *
 * Created on May 28, 2014, 12:47 PM
 */

#ifndef POSIXTHREAD_H
#define	POSIXTHREAD_H

#include "Runnable.h"
#include <pthread.h>

namespace Posix
{
    class Thread: public Runnable
    {
        public:
            Thread();
            virtual ~Thread();
            int start();
            int start(Runnable * runnable);
            int wait ();

        protected:
            void run();

        private:
            Thread(const Thread& orig); //disable copy
            void operator = (const Thread& orig); //disable copy
            static void * threadFunc(void *d);

        private:        
            pthread_t  m_Thread;
    };
}

#endif	/* POSIXTHREAD_H */

