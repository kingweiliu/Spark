#pragma once
class CThread
{
public:
	CThread(void);
	~CThread(void);

	void Start();
	void Stop();

	UINT m_threadID;

private:
	void RunThread();
	static unsigned __stdcall ThreadFunc( void* pArguments );
};

