// LjwBrowser.cpp : main source file for LjwBrowser.exe
//

#include "stdafx.h"

#include "resource.h"

#include "LjwBrowserView.h"
#include "aboutdlg.h"
#include "MainFrm.h"
#include "../third_party/Detours3/src/detours.h"
#include <WinInet.h>
#include "Setting.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int (*nAdd)(int a, int b);


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



void HookFunc()
{
	 DetourTransactionBegin();
	 DetourUpdateThread(::GetCurrentThread());
	 DetourAttach((PVOID*)&g_originConnect, hook_InternetConnect);
	 DetourTransactionCommit();	
}





int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);
	//HookFunc();

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
