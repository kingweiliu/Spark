#pragma once
class CHookCenter
{
public:
	CHookCenter(void);
	~CHookCenter(void);

	static void StartHook();

	//ʹ��hook blur �¼� , ���Զ�com�ӿڵ�hook
	static bool m_bHookBlur;
	static void HookBlur(IWebBrowser2* pWeb);

	//cookie ͨ�� �ض������ , IE8 ��Ч
	static void HookCookieDir();


};

