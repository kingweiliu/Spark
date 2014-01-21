#include "StdAfx.h"
#include "HookCenter.h"
#include "../third_party/Detours3/src/detours.h"
#include <WinInet.h>
#include "Setting.h"

CHookCenter::CHookCenter(void)
{
}


CHookCenter::~CHookCenter(void)
{
}

//*****************************************
//以下代码用来测试自定义proxy

typedef  HINTERNET (__stdcall *pInternetConnect)(
	HINTERNET hInternet,
	LPCTSTR lpszServerName,
	INTERNET_PORT nServerPort,
	LPCTSTR lpszUsername,
	LPCTSTR lpszPassword,
	DWORD dwService,
	DWORD dwFlags,
	DWORD_PTR dwContext
	);

pInternetConnect g_originConnect = InternetConnectW;



HINTERNET hook_InternetConnect(
	HINTERNET hInternet,
	LPCTSTR lpszServerName,
	INTERNET_PORT nServerPort,
	LPCTSTR lpszUsername,
	LPCTSTR lpszPassword,
	DWORD dwService,
	DWORD dwFlags,
	DWORD_PTR dwContext
	)
{
	if (Setting::m_bProxy)
	{
		INTERNET_PER_CONN_OPTION_LIST    List;
		INTERNET_PER_CONN_OPTION         Option[2];
		unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);


		Option[0].dwOption = INTERNET_PER_CONN_FLAGS;

		Option[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

		List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
		List.pszConnection = NULL;
		List.dwOptionCount = 2;
		List.dwOptionError = 0;
		List.pOptions = Option;

		Option[1].Value.pszValue = L"172.19.1.2:9217";
		Option[0].Value.dwValue = PROXY_TYPE_PROXY ;

		BOOL bRet = InternetSetOption(hInternet, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize);
	}
	return g_originConnect(hInternet, lpszServerName, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

void CHookCenter::StartHook(){	
	LoadLibrary(L"Wininet.dll");
	g_originConnect = (pInternetConnect)DetourFindFunction("Wininet.dll", "InternetConnectW");
	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	DetourAttach((PVOID*)&g_originConnect, hook_InternetConnect);
	DetourTransactionCommit();	

	HookCookieDir();
}

//*****************************************

// 以下代码演示hook一个com接口
typedef HRESULT ( STDMETHODCALLTYPE *pblur )(IHTMLWindow2 * This);
pblur g_blur = NULL;

HRESULT ( STDMETHODCALLTYPE my_blur )(IHTMLWindow2 * pThis)
{
	if (g_blur)
	{
		return g_blur(pThis);
	}
	return S_OK;
}

bool CHookCenter::m_bHookBlur = false;
void CHookCenter::HookBlur(IWebBrowser2* pWeb){
	if (!g_blur)
	{
		CComPtr<IDispatch> spDispDoc;
		pWeb->get_Document(&spDispDoc);
		CComQIPtr<IHTMLDocument2> spHTMLDoc = spDispDoc;
		CComPtr<IHTMLWindow2> spWnd;
		spHTMLDoc->get_parentWindow(&spWnd);
		//			spWnd->blur();

		DWORD * pVptr =(DWORD*) *(DWORD*)spWnd.p;
		g_blur = (pblur)(*(pVptr +61));

		::DetourTransactionBegin();
		::DetourUpdateThread(::GetCurrentThread());
		::DetourAttach((PVOID*)&g_blur, my_blur);
		::DetourTransactionCommit();

	}
}

//**************** 测试hook cookie 重定向 ***************************

typedef HRESULT (__stdcall *pFunc_SHGetFolderPath)(
	_In_   HWND hwndOwner,
	_In_   int nFolder,
	_In_   HANDLE hToken,
	_In_   DWORD dwFlags,
	_Out_  LPTSTR pszPath
	);

pFunc_SHGetFolderPath g_SHGetFolderPath = NULL;

HRESULT __stdcall Hook_SHGetFolderPathW(
	   HWND hwndOwner,
	   int nFolder,
	   HANDLE hToken,
	   DWORD dwFlags,
	  LPTSTR pszPath){
	HRESULT hr = g_SHGetFolderPath(hwndOwner, nFolder, hToken, dwFlags, pszPath);

		if ((nFolder&0x00FF) == CSIDL_COOKIES)
		{
			wcscpy(pszPath, L"d:\\cookies\\");
			//ATLASSERT(FALSE);
		}
	return hr;
}

void CHookCenter::HookCookieDir(){
	LoadLibrary(L"shell32.dll");

	g_SHGetFolderPath = (pFunc_SHGetFolderPath)DetourFindFunction("shell32.dll", "SHGetFolderPathW");

	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	DetourAttach((PVOID*)&g_SHGetFolderPath, Hook_SHGetFolderPathW);
	DetourTransactionCommit();	
	
}