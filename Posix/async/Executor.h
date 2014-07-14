/* 
 * File:   FixedExecutor.h
 * Author: alexey
 *
 * Created on May 28, 2014, 3:43 PM
 */

#ifndef POSIXEXECUTOR_H
#define	POSIXEXECUTOR_H

#include "Mutex.h"
#include "Runnable.h"
#include <map>
#include <list>
#include <vector>

namespace Posix
{
    class ExecutorThread;

    class Executor
    {
        public:
            Executor( int size );
            ~Executor();
            void execute(Runnable & runnable);
            int poolSize()const;

        protected:
            friend class ExecutorThread;//Used to hide function "Runnable *next(int)";
            Runnable* next(int id);

        private:
            Executor(const Executor& orig);
            void operator=(const Executor& orig);

        private:
            int                     m_Size;
            ExecutorThread*         m_Threads;
            Mutex                   m_Mutex;
            std::list<int>          m_FreeThreads;
            std::list<Runnable*>    m_Tasks;
    };
}

#endif	/* POSIXEXECUTOR_H */

