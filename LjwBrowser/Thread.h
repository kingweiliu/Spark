#pragma once

class CThread
{
public:
	CThread(void);
	~CThread(void);

	DWORD Start();
	void Stop();

	UINT GetThreadID();
    HWND GetMessageHwnd();

    class CMsgHander : public CMessageFilter
    {
        BOOL PreTranslateMessage(MSG* pMsg);
    };

    HANDLE GetEventHandle(){
        return m_pEvent->m_h;
    }
private:
	void RunThread();
	static unsigned __stdcall ThreadFunc( void* pArguments );
    CEvent* m_pEvent;

    CMsgHander m_msgHandler;
    UINT m_threadID;
    HWND m_hWndMsg;
};

