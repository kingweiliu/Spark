#pragma once
class CHookCenter
{
public:
	CHookCenter(void);
	~CHookCenter(void);

	static void StartHook();

	//使用hook blur 事件 , 测试对com接口的hook
	static bool m_bHookBlur;
	static void HookBlur(IWebBrowser2* pWeb);

	//cookie 通过 重定向隔离 , IE8 生效
	static void HookCookieDir();


};

