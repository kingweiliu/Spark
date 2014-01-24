#include "StdAfx.h"
#include "ThreadMgr.h"
#include "Thread.h"
#include "publicDef.h"

CThreadMgr::CThreadMgr(void)
{
}


CThreadMgr::~CThreadMgr(void)
{
}

DWORD CThreadMgr::CreateThread(){
    CThread* pThread = new CThread;
    pThread->Start();
    m_mapThreads[pThread->GetThreadID()] = pThread;
    ::WaitForSingleObject(pThread->GetEventHandle(), INFINITE);
    return pThread->GetThreadID();
}

void CThreadMgr::RunTask(UINT nThreadID, ITask* pTask){
    CThread * pThread = m_mapThreads[nThreadID];
    PostMessage(pThread->GetMessageHwnd(), MSG_WM_NEWTASK, (WPARAM)pTask, NULL);
}

