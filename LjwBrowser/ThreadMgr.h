#pragma once
#include <map>

class CThread;
class ITask;

class CThreadMgr
{
public:
    CThreadMgr(void);
    ~CThreadMgr(void);

    DWORD CreateThread();
    void RunTask(UINT nThreadID, ITask* pTask);
    
private:
    std::map<UINT, CThread*> m_mapThreads;

};

