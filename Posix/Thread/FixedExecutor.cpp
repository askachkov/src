/* 
 * File:   FixedExecutor.cpp
 * Author: alexey
 * 
 * Created on May 28, 2014, 3:43 PM
 */

#include "FixedExecutor.h"

namespace Posix
{
    ExecutorThread * FixedExecutor::initThreads(int size, TaskProvider * provider)
    {
        ExecutorThread * result = new ExecutorThread[size];
        for ( int i = 0; i < size; ++i ){
            result[i].init(i, provider);
        }
        return result;
    }

    FixedExecutor::FixedExecutor(int size):
        m_Size(size),
        m_Threads(initThreads(size, this))
    {
        POSIX_SYNCHRONIZE(&m_Mutex)
        for ( int i = 0; i < m_Size; ++i ){
            m_FreeThreads.push_back(i);
        }
    }

    FixedExecutor::~FixedExecutor()
    {
        for ( int i = 0; i < m_Size; ++i )
            m_Threads[i].setEnd(true);
        for ( int i = 0; i < m_Size; ++i )
            m_Threads[i].wait();
        delete[] m_Threads;
    }

    void FixedExecutor::execute(Runnable* runnable, bool autoRelease)
    {
        POSIX_SYNCHRONIZE(&m_Mutex)
        Task task = {runnable, autoRelease};
        if ( m_FreeThreads.empty() ){
            m_Tasks.push_back(task);
        } else {
            m_Threads[m_FreeThreads.front()].setTask(task);
            m_FreeThreads.pop_front();
        }
    }

    ExecutorThread::TaskProvider::Task FixedExecutor::next(int id)
    {
        POSIX_SYNCHRONIZE(&m_Mutex)
        Task result = {0, false};
        if ( !m_Tasks.empty() ){
            result = m_Tasks.front();
            m_Tasks.pop_front();
        } else {
            m_FreeThreads.push_back(id);
        }
        return result;
    }

    int FixedExecutor::poolSize() const
    {
        return m_Size;
    }
}
