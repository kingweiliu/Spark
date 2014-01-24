#pragma once


class ITask{
public:
    virtual void Run() = 0;
};

template<class T>
class CTask0 : public ITask{
public:
    typedef void (T::*FuncRun)();

    void Run(){
        (m_pObj->*m_pFunc)();
    }

    CTask0(T* pObj, FuncRun pFunc):m_pObj(pObj), m_pFunc(pFunc)
    {
    }

private:
    T* m_pObj;
    FuncRun m_pFunc;
};

template<class T, class P0>
class CTask1 : public ITask{
public:
    typedef void (T::*FuncRun1)(P0 p);

    void Run(){
        m_pObj->m_pFunc(m_p0);
    }


private:
    P0 m_p0;
    T* m_pObj;
    FuncRun1 m_pFunc;
};

template<class T, class P0, class P1>
class CTask2 : public ITask{
public:
    typedef void (T::*FuncRun2)(P0 p0, P1 p1);

    void Run(){
        m_pObj->m_pFunc(m_p0, m_p1);
    }

    CTask2(T* pObj, FuncRun2 pFunc, P0 p , P1 p1)
        :m_pObj(pObj), m_pFunc(pFunc) m_p0(p), m_p1(p1)
    {

    }


private:
    P1 m_p1;
    P0 m_p0;
    T* m_pObj;
    FuncRun2 m_pFunc;
};