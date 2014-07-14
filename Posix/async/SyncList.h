/* 
 * File:   SyncList.h
 * Author: alexey
 *
 * Created on July 9, 2014, 6:42 PM
 */

#ifndef SYNCLIST_H
#define	SYNCLIST_H

#include <list>
#include "Mutex.h"

namespace Posix
{
    template <class T>
    class SyncList
    {
    public:
        bool isEmpty()const
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            return m_List.empty();
        }
        size_t size()const
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            return m_List.size();
        }
        void pushBack(const T & item)
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            m_List.push_back(item);
        }
        bool popBack(T * result)
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            if ( result && !m_List.empty() ){
                *result = m_List.back();
                m_List.pop_back();
                return true;
            }
            return false;
        }
        void pushFront(const T & item)
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            m_List.push_front(item);
        }
        bool popFront(T * result)
        {
            POSIX_SYNCHRONIZE(&m_Mutex);
            if ( result && !m_List.empty() ){
                *result = m_List.front();
                m_List.pop_front();
                return true;
            }
            return false;
        }
        
    private:
        Mutex        m_Mutex;
        std::list<T> m_List;
    };
}

#endif	/* SYNCLIST_H */

