/* 
 * File:   FixedExecutor.h
 * Author: alexey
 *
 * Created on May 28, 2014, 3:43 PM
 */

#ifndef POSIXFIXEDEXECUTOR_H
#define	POSIXFIXEDEXECUTOR_H

#include "ExecutorThread.h"
#include "Mutex.h"
#include <map>
#include <list>

namespace Posix
{
    class FixedExecutor: 
        public Executor,
        protected ExecutorThread::TaskProvider
    {
        public:
            FixedExecutor( int size );
            virtual ~FixedExecutor();
            virtual void execute(Runnable* runnable, bool autoRelease = true);
            int poolSize()const;

        protected:
            virtual Task next(int id);

        private:
            FixedExecutor(const FixedExecutor& orig);
            void operator=(const FixedExecutor& orig);
            static ExecutorThread * initThreads(int size, TaskProvider * provider);

        private:
            int              m_Size;
            ExecutorThread*  m_Threads;
            Mutex            m_Mutex;
            std::list<int>   m_FreeThreads;
            std::list<Task>  m_Tasks;
    };
}

#endif	/* POSIXFIXEDEXECUTOR_H */

