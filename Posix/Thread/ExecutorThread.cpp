/* 
 * File:   ExecutorThread.cpp
 * Author: alexey
 * 
 * Created on May 28, 2014, 2:12 PM
 */

#include "ExecutorThread.h"

namespace Posix
{
    ExecutorThread::ExecutorThread():
        m_Id(0), m_TaskProvider(0), m_isEnd(false)
    {
        TaskProvider::Task t = {0, false};
        m_Task = t;
        pthread_mutex_init(&m_Mutex, NULL);
    }

    void ExecutorThread::init(int id, TaskProvider* provider)
    {
        m_Id = id;
        m_TaskProvider = provider;
        pthread_create(&m_Thread, NULL, &ExecutorThread::threadFunc, reinterpret_cast<void*>(this));
        pthread_mutex_lock(&m_Mutex);
    }

    void * ExecutorThread::threadFunc(void *d)
    {
        ExecutorThread * pExecutorThread = static_cast<ExecutorThread*>(d);
        do {
            if ( pExecutorThread->hasTask() ){
                pExecutorThread->start();
                if ( !pExecutorThread->updateRunnable() ){
                    pExecutorThread->threadWaitEvent();
                }
            }
        } while ( !pExecutorThread->isEnd() );
    }

    bool ExecutorThread::updateRunnable()
    {
        m_Task = m_TaskProvider->next(m_Id);
        return m_Task.runnable != 0;
    }

    ExecutorThread::~ExecutorThread() 
    {
    }

    bool ExecutorThread::hasTask() const
    {
        return m_Task.runnable != 0;
    }

    void ExecutorThread::start()
    {
        m_Task.runnable->run();
        if ( m_Task.autoReleas ){
            delete m_Task.runnable;
        }
    }

    void ExecutorThread::setTask(const TaskProvider::Task & task)
    {
        m_Task = task;
        emitEventToThread();
    }

    void ExecutorThread::emitEventToThread()
    {
        pthread_mutex_unlock(&m_Mutex);
        pthread_mutex_lock(&m_Mutex);    
    }

    void ExecutorThread::threadWaitEvent()
    {
        pthread_mutex_lock(&m_Mutex);
        pthread_mutex_unlock(&m_Mutex);
    }

    void ExecutorThread::wait()
    {
         pthread_join(m_Thread, NULL); 
    }

    void ExecutorThread::setEnd(bool val)
    {
        pthread_mutex_unlock(&m_Mutex);
        m_isEnd = val;
    }

    bool ExecutorThread::isEnd() const
    {
        return m_isEnd && (m_Task.runnable == 0);
    }
}
