#include "StdAfx.h"
#include "Thread.h"


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

void CThread::Start(){	
	_beginthreadex(NULL, NULL, &CThread::ThreadFunc, this, 0, &m_threadID);
}

//typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

LRESULT __stdcall WndProc(HWND hwnd, UINT msgID, WPARAM wParam, LPARAM lParam){
	return ::DefWindowProc(hwnd, msgID, wParam, lParam);
}

void CThread::RunThread(){
	WNDCLASSEX wndCls = {0};
	wndCls.lpfnWndProc = &WndProc;
	wndCls.cbSize = sizeof(wndCls);
	TCHAR szClsName[] = L"LjwThread";
	wndCls.lpszClassName = szClsName;
	::RegisterClassEx(&wndCls);

	HWND hwnd = ::CreateWindow(szClsName, NULL, NULL, 0,0,0,0, HWND_MESSAGE, 0, 0, 0);
	int i = 0;
	i+=1;

	CMessageLoop msgLoop;
	msgLoop.Run();

}

void CThread::Stop(){
	_endthreadex( 0 );
}

