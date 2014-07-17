#ifndef DELEGATE_H
#define	DELEGATE_H

template <class T, class Func>
class BaseDelegate
{
    public:
        BaseDelegate(): m_Object(0), m_Func(0)
        {
        }
        BaseDelegate(T * object, Func func): m_Object(object), m_Func(func)
        {
        }
        void setObject(T * object)
        {
            m_Object = object;
        }
        T * object()const
        {
            return m_Object;
        }
        void setFunc(Func func)
        {
            m_Func = func;
        }
        Func func()const
        {
            return m_Func;
        }
        bool isValid()const
        {
            return m_Object && m_Func;
        }
        
    private:
        T *     m_Object;
        Func    m_Func;
};

template <class T, class RetV>
class Delegate: public BaseDelegate<T, RetV (T::*)()>
{
    public:
        typedef RetV (T::*Func)();
        typedef BaseDelegate<T, Func> Base;
        Delegate()
        {
        }
        Delegate(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()()
        {
            return (Base::object()->*Base::func())();
        }
};

template <class T, class RetV>
class ConstDelegate: public BaseDelegate<T, RetV (T::*)()const>
{
    public:
        typedef RetV (T::*Func)()const;
        typedef BaseDelegate<T, Func> Base;
        ConstDelegate()
        {
        }
        ConstDelegate(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()()const
        {
            return (Base::object()->*Base::func())();
        }
};

template <class T, class RetV, class Arg>
class Delegate1: public BaseDelegate<T, RetV (T::*)(Arg)>
{
    public:
        typedef RetV (T::*Func)(Arg);
        typedef BaseDelegate<T, Func> Base;
        Delegate1()
        {
        }
        Delegate1(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg arg)
        {
            return (Base::object()->*Base::func())(arg);
        }
};

template <class T, class RetV, class Arg>
class ConstDelegate1: public BaseDelegate<T, RetV (T::*)(Arg)const>
{
    public:
        typedef RetV (T::*Func)(Arg)const;
        typedef BaseDelegate<T, Func> Base;
        ConstDelegate1()
        {
        }
        ConstDelegate1(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg arg)const
        {
            return (Base::object()->*Base::func())(arg);
        }
};

template <class T, class RetV, class Arg1, class Arg2>
class Delegate2: public BaseDelegate<T, RetV (T::*)(Arg1, Arg2)>
{
    public:
        typedef RetV (T::*Func)(Arg1, Arg2);
        typedef BaseDelegate<T, Func> Base;
        Delegate2()
        {
        }
        Delegate2(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg1 arg1, Arg2 arg2)
        {
            return (Base::object()->*Base::func())(arg1, arg2);
        }
};

template <class T, class RetV, class Arg1, class Arg2>
class ConstDelegate2: public BaseDelegate<T, RetV (T::*)(Arg1, Arg2)const>
{
    public:
        typedef RetV (T::*Func)(Arg1, Arg2)const;
        typedef BaseDelegate<T, Func> Base;
        ConstDelegate2()
        {
        }
        ConstDelegate2(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg1 arg1, Arg2 arg2)const
        {
            return (Base::object()->*Base::func())(arg1, arg2);
        }
};

template <class T, class RetV, class Arg1, class Arg2, class Arg3>
class Delegate3: public BaseDelegate<T, RetV (T::*)(Arg1, Arg2, Arg3)>
{
    public:
        typedef RetV (T::*Func)(Arg1, Arg2, Arg3);
        typedef BaseDelegate<T, Func> Base;
        Delegate3()
        {
        }
        Delegate3(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3)
        {
            return (Base::object()->*Base::func())(arg1, arg2, arg3);
        }
};

template <class T, class RetV, class Arg1, class Arg2, class Arg3>
class ConstDelegate3: public BaseDelegate<T, RetV (T::*)(Arg1, Arg2, Arg3)const>
{
    public:
        typedef RetV (T::*Func)(Arg1, Arg2, Arg3)const;
        typedef BaseDelegate<T, Func> Base;
        ConstDelegate3()
        {
        }
        ConstDelegate3(T * object, Func func): Base(object, func)
        {
        }
        RetV operator()(Arg1 arg1, Arg2 arg2, Arg3 arg3)const
        {
            return (Base::object()->*Base::func())(arg1, arg2, arg3);
        }
};

template <class T, class RetV>
Delegate<T, RetV> makeDelegate(T * object, RetV (T::*func)())
{
    return Delegate<T, RetV>(object, func);
}

template <class T, class RetV>
ConstDelegate<T, RetV> makeDelegate(T * object, RetV (T::*func)()const)
{
    return ConstDelegate<T, RetV>(object, func);
}

template <class T, class RetV, class Arg>
Delegate1<T, RetV, Arg> makeDelegate(T * object, RetV (T::*func)(Arg))
{
    return Delegate1<T, RetV, Arg>(object, func);
}

template <class T, class RetV, class Arg>
ConstDelegate1<T, RetV, Arg> makeDelegate(T * object, RetV (T::*func)(Arg)const)
{
    return ConstDelegate1<T, RetV, Arg>(object, func);
}

template <class T, class RetV, class Arg1, class Arg2>
Delegate2<T, RetV, Arg1, Arg2> makeDelegate(T * object, RetV (T::*func)(Arg1, Arg2))
{
    return Delegate2<T, RetV, Arg1, Arg2>(object, func);
}

template <class T, class RetV, class Arg1, class Arg2>
ConstDelegate2<T, RetV, Arg1, Arg2> makeDelegate(T * object, RetV (T::*func)(Arg1, Arg2)const)
{
    return ConstDelegate2<T, RetV, Arg1, Arg2>(object, func);
}

template <class T, class RetV, class Arg1, class Arg2, class Arg3>
Delegate3<T, RetV, Arg1, Arg2, Arg3> makeDelegate(T * object, RetV (T::*func)(Arg1, Arg2, Arg3))
{
    return Delegate3<T, RetV, Arg1, Arg2, Arg3>(object, func);
}

template <class T, class RetV, class Arg1, class Arg2, class Arg3>
ConstDelegate3<T, RetV, Arg1, Arg2, Arg3> makeDelegate(T * object, RetV (T::*func)(Arg1, Arg2, Arg3)const)
{
    return ConstDelegate3<T, RetV, Arg1, Arg2, Arg3>(object, func);
}

#endif	/* DELEGATE_H */

