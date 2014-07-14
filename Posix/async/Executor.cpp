/* 
 * File:   FixedExecutor.cpp
 * Author: alexey
 * 
 * Created on May 28, 2014, 3:43 PM
 */

#include "Executor.h"
#include "unistd.h"
#include "algorithmfwd.h"
#include <iostream>
#include <algorithm>

namespace Posix
{
    //------------ Declaration of the inner thread--------------
    class ExecutorThread
    {
        public:
            ExecutorThread();
            void init(int id, Executor * provider);
            void setTask(Runnable * runnable);
            bool hasTask()const;
            void start();
            bool updateRunnable();
            void wait();
            //void release();
            bool isEnd()const;
            void setEnd(bool val);
            
        private:
            ExecutorThread(const ExecutorThread& orig); //disable copy
            void operator = (const ExecutorThread& orig); //disable copy
            static void * threadFunc(void *d);

        private:        
            pthread_t           m_Thread;
            Mutex               m_Mutex;
            int                 m_Id;
            Runnable*           m_Task;
            Executor     *      m_TaskProvider;
            bool                m_isEnd;
    };
    
    //------------ Implementation of the inner thread--------------
    ExecutorThread::ExecutorThread():
        m_Id(0), m_TaskProvider(0), m_isEnd(false), m_Task(0)
    {
    }

    void ExecutorThread::init(int id, Executor* provider)
    {
        m_Id = id;
        m_TaskProvider = provider;
        pthread_create(&m_Thread, 0, &ExecutorThread::threadFunc, reinterpret_cast<void*>(this));
    }
    
    void * ExecutorThread::threadFunc(void *d)
    {
        ExecutorThread * pExecutorThread = static_cast<ExecutorThread*>(d);
        pthread_yield();
        do {
            pExecutorThread->start();
            pthread_yield();
        } while ( !pExecutorThread->isEnd() );
        pthread_yield();
        pthread_exit(0);
    }

    bool ExecutorThread::hasTask() const
    {
        POSIX_SYNCHRONIZE(&m_Mutex); 
        return m_Task != 0;
    }

    void ExecutorThread::start()
    {
        m_Mutex.lock();
        if ( m_Task != 0 ){
            m_Task->run();
            m_Task = 0;
        }
        m_Mutex.unlock();
        setTask(m_TaskProvider->next(m_Id));
    }

    void ExecutorThread::setTask(Runnable * runnable)
    {
        POSIX_SYNCHRONIZE(&m_Mutex);  
        m_Task = runnable;
    }
    void ExecutorThread::wait()
    {
        pthread_join(m_Thread, NULL);
    }

    void ExecutorThread::setEnd(bool val)
    {
        POSIX_SYNCHRONIZE(&m_Mutex); 
        m_isEnd = val;
    }

    bool ExecutorThread::isEnd() const
    {
        POSIX_SYNCHRONIZE(&m_Mutex);
        return m_isEnd && m_Task == 0;
    }

    //------------ Implementation of the Executor--------------    
    static ExecutorThread * initThreads(int size, Executor * provider)
    {
        ExecutorThread * result = new ExecutorThread[size];
        for ( int i = 0; i < size; ++i ){
            result[i].init(i, provider);
        }
        return result;
    }
    
    Executor::Executor(int size):
        m_Size(size),
        m_Threads(0)
    {
        POSIX_SYNCHRONIZE(&m_Mutex);
        m_Threads = initThreads(size, this);
        for ( int i = 0; i < m_Size; ++i ){
            m_FreeThreads.push_back(i);
        }
    }

    Executor::~Executor()
    {
        if( m_Threads ){
            for ( int i = 0; i < m_Size; ++i ){
                m_Mutex.lock();
                m_Threads[i].setEnd(true);
                m_Mutex.unlock();
                m_Threads[i].wait();
            }
            delete[] m_Threads;
            m_Threads = 0;
        }
    }

    void Executor::execute(Runnable & runnable)
    {
        POSIX_SYNCHRONIZE(&m_Mutex)
        if ( m_FreeThreads.empty() ){
            m_Tasks.push_back(&runnable);
        } else {
            m_Threads[m_FreeThreads.front()].setTask(&runnable);
            m_FreeThreads.pop_front();
        }
    }

    Runnable* Executor::next(int id)
    {
        POSIX_SYNCHRONIZE(&m_Mutex)
        Runnable * result = 0;
        if ( !m_Tasks.empty() ){
            result = m_Tasks.front();
            m_Tasks.pop_front();
        } else if ( std::find(m_FreeThreads.begin(), m_FreeThreads.end(), id) == m_FreeThreads.end() ) {
            m_FreeThreads.push_back(id);
        }
        return result;
    }

    int Executor::poolSize() const
    {
        return m_Size;
    }
}
