/* 
 * File:   Runnable.h
 * Author: alexey
 *
 * Created on May 28, 2014, 1:01 PM
 */

#ifndef POSIXRUNNABLE_H
#define	POSIXRUNNABLE_H

namespace Posix
{
    class Runnable
    {
        public:
            virtual ~Runnable(){};
            virtual void run() = 0;
    };

    class Executor
    {
        public:
            virtual ~Executor(){}
            virtual void execute(Runnable * runnable, bool autoRelease = true) = 0;
    };
}

#endif	/* RUNNABLE_H */

