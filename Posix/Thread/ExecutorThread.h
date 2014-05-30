/* 
 * File:   ExecutorThread.h
 * Author: alexey
 *
 * Created on May 28, 2014, 2:12 PM
 */

#ifndef POSIXEXECUTORTHREAD_H
#define	POSIXEXECUTORTHREAD_H

#include "Runnable.h"
#include <pthread.h>

namespace Posix
{
    class ExecutorThread
    {
        public:
            struct TaskProvider
            {
                struct Task {Runnable * runnable; bool autoReleas;};
                virtual ~TaskProvider(){}
                virtual Task next(int threadId) = 0;
            };

        public:
            ExecutorThread();
            ~ExecutorThread();
            void init(int id, TaskProvider * provider);
            void setTask(const TaskProvider::Task & task);
            bool hasTask()const;
            void start();
            bool updateRunnable();
            void wait();

            bool isEnd()const;
            void setEnd(bool val);

            /**
             * ExecutorThread::emitEventToThread()
             * Resume thread via mutex
             */
            void emitEventToThread();

            /**
             * ExecutorThread::threadWaitEvent()
             * Suspend thread via mutex.
             * Must be called inside thread.
             */
            void threadWaitEvent();

        private:
            ExecutorThread(const ExecutorThread& orig); //disable copy
            void operator = (const ExecutorThread& orig); //disable copy
            static void * threadFunc(void *d);

        private:        
            pthread_t           m_Thread;
            pthread_mutex_t     m_Mutex;
            int                 m_Id;
            TaskProvider::Task  m_Task;
            TaskProvider *      m_TaskProvider;
            bool                m_isEnd;
    };
}

#endif	/* POSIXEXECUTORTHREAD_H */

