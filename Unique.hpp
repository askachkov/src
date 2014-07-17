#ifndef UNIQUE_HPP
#define UNIQUE_HPP

#include <queue>
#include <stack>
#include <set>
#include <Algorithm.hpp>

namespace ex
{
    using namespace std;
    
    template <class Type, class Container = deque<Type> >
    class UniqueFIFO
    {
        public:
            Type pop()
            {
                Type result = m_Container.front();
                m_Container.pop();
                m_UniqueChecking.erase(result);
                return result;
            }
            bool push(const Type & value)
            {
                if ( !contains(m_UniqueChecking, value) ){
                    m_Container.push(value);
                    m_UniqueChecking.insert(value);
                    return true;
                }
                return false;
            }
            bool isEmpty()const
            {
                return m_Container.empty();
            }
        
        private:
            queue<Type, Container>  m_Container;
            set<Type>               m_UniqueChecking;
    };
    
    template <class Type, class Container = deque<Type> >
    class UniqueLIFO
    {
        public:
            Type pop()
            {
                Type result = m_Container.top();
                m_Container.pop();
                m_UniqueChecking.erase(result);
                return result;
            }
            bool push(const Type & value)
            {
                if ( !contains(m_UniqueChecking, value) ){
                    m_Container.push(value);
                    m_UniqueChecking.insert(value);
                    return true;
                }
                return false;
            }
            bool isEmpty()const
            {
                return m_Container.empty();
            }
        
        private:
            stack<Type, Container>  m_Container;
            set<Type>               m_UniqueChecking;
    };
}

#endif//UNIQUE_HPP