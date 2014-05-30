/* 
 * File:   PosixThread.cpp
 * Author: alexey
 * 
 * Created on May 28, 2014, 12:47 PM
 */

#include "Thread.h"

namespace Posix
{
    Thread::Thread():
        m_Thread(0)
    {
    }

    Thread::~Thread()
    {
    }

    void * Thread::threadFunc(void *d)
    { 
        Runnable * pRannable = static_cast<Runnable*>(d);
        pRannable->run();
        return 0;
    }

    int Thread::start()          
    { 
        return start(this);
    }

    int Thread::start(Runnable * runnable)
    { 
        return pthread_create(&m_Thread, NULL, &Thread::threadFunc, reinterpret_cast<void*>(runnable)); 
    }

    int Thread::wait()
    {
        return pthread_join(m_Thread, NULL); 
    }

    void Thread::run()
    {
    }

}
