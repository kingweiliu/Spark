#include "StdAfx.h"
#include "Thread.h"
#include "Task.h"
#include "publicDef.h"

BOOL CThread::CMsgHander::PreTranslateMessage(MSG* pMsg){
    if (pMsg->message == MSG_WM_NEWTASK)
    {
        ITask * pTask = (ITask*)pMsg->wParam;
        pTask->Run();
        return TRUE;
    }
    return FALSE;
}

CThread::CThread(void)
{
}


CThread::~CThread(void)
{
}

unsigned CThread::ThreadFunc( void* pArguments )
{
	CThread* pThread = (CThread*)pArguments;
	pThread->RunThread();
	return 0;
} 

DWORD CThread::Start(){	
    m_pEvent = new CEvent(FALSE, FALSE);
	_beginthreadex(NULL, NULL, &CThread::ThreadFunc, this, 0, &m_threadID);
    return m_threadID;
}

UINT CThread::GetThreadID(){
    return m_threadID;
}
HWND CThread::GetMessageHwnd(){
    return m_hWndMsg;
}


//typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

LRESULT __stdcall WndProc(HWND hwnd, UINT msgID, WPARAM wParam, LPARAM lParam){
    if (msgID == MSG_WM_NEWTASK)
    {
        ITask * pTask = (ITask*)wParam;
        pTask->Run();
        return 1;
    }
	return ::DefWindowProc(hwnd, msgID, wParam, lParam);
}

void CThread::RunThread(){
	WNDCLASSEX wndCls = {0};
	wndCls.lpfnWndProc = &WndProc;
	wndCls.cbSize = sizeof(wndCls);
	TCHAR szClsName[] = L"LjwThread";
	wndCls.lpszClassName = szClsName;
	::RegisterClassEx(&wndCls);

	m_hWndMsg = ::CreateWindow(szClsName, NULL, NULL, 0,0,0,0, HWND_MESSAGE, 0, 0, 0);
	int i = 0;
	i+=1;

	CMessageLoop msgLoop;
    m_pEvent->Set();
    //msgLoop.AddMessageFilter(&m_msgHandler);
	msgLoop.Run();
    
    
}

void CThread::Stop(){
	_endthreadex( 0 );
}

