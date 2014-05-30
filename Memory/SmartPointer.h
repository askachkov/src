/* 
 * File:   SmartPointer.h
 * Author: alexey
 *
 * Created on May 28, 2014, 6:06 PM
 */

#ifndef SMARTPOINTER_H
#define	SMARTPOINTER_H

template <typename T>
struct DefaultDeleter
{
    static void free(T * p)
    {
        delete p;
    }
};

template <typename T>
struct DefaultDeleter<T[]>
{
    static void free(T * p)
    {
        delete[] p;
    }
};

template<typename T, typename Deleter = DefaultDeleter<T> >
class ScopePointer
{
    public:
        ScopePointer(T * p): m_Pointer(p)
        {
        }
        ~ScopePointer()
        {
            Deleter::free(m_Pointer);
        }
        operator T*()
        {
            return m_Pointer;
        }
        operator const T*()const
        {
            return m_Pointer;
        }
        T * operator->()
        {
            return m_Pointer;
        }
        const T * operator->()const
        {
            return m_Pointer;
        }
        T& operator*()
        {
            return m_Pointer;
        }
        const T& operator*()const
        {
            return m_Pointer;
        }
        T& operator[](int id)
        {
            return m_Pointer[id];
        }
        const T& operator[](int id)const
        {
            return m_Pointer[id];
        }
        
    private:
        ScopePointer(const ScopePointer&);
        void operator=(const ScopePointer&);
        
    private:
        T * m_Pointer;
};

template<typename T, typename Deleter = DefaultDeleter<T> >
class SharedPointer
{
    public:
        SharedPointer():
            m_pData(0),
            m_ppCount(new int *(0))
        {
        }
        SharedPointer(T * pointer): 
            m_pData(pointer),
            m_ppCount(new int *(new int(1)))
        {
        }
        SharedPointer(const SharedPointer & other):
            m_pData(0),
            m_ppCount(new int *(0))
        {
            copy(other);
            inc();
        }
        ~SharedPointer()
        {
            release();
            delete m_ppCount;
        }
        SharedPointer& operator=(const SharedPointer & other)
        {
            release();
            copy(other);
            inc();
            return *this;
        }
        bool isNull()const
        {
            return m_pData == 0;
        }
        operator T*()
        {
            return m_pData;
        }
        operator const T*()const
        {
            return m_pData;
        }
        T* operator->()
        {
            return m_pData;
        }
        const T* operator->()const
        {
            return m_pData;
        }
        T& operator*()
        {
            return m_pData;
        }
        const T& operator*()const
        {
            return m_pData;
        }
    
    private:
        int inc()
        {
            return ++(*(*m_ppCount));
        }
        int dec()
        {
            return --(*(*m_ppCount));
        }
        void release()
        {
            if (*m_ppCount != 0 && dec() == 0 ){
                delete *m_ppCount;
                Deleter::free(m_pData);
            }
        }
        void copy(const SharedPointer & other)
        {
            *m_ppCount = *other.m_ppCount;
            m_pData = other.m_pData;
        }
        
    private:
        T *         m_pData;
        int**       m_ppCount;
};

#endif	/* SMARTPOINTER_H */

