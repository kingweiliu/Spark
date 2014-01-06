// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DocUIHandler.h"
#include "aboutdlg.h"
#include "LjwBrowserView.h"
#include "MainFrm.h"
#include <wininet.h>
#include "Setting.h"
#include "../third_party/Detours3/include/detours.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	m_edtAddress.Create(this, MAINFRAME_ADDRESS_MSGID, m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	AddSimpleReBarBand(m_edtAddress);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	CMenuHandle menuMain = m_CmdBar.GetMenu();
	m_view.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CLjwBrowserView* pView = new CLjwBrowserView;
	//TODO: Replace with a URL of your choice
	pView->Create(m_view, rcDefault, _T("about:blank"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, 0);

	m_view.AddPage(pView->m_hWnd, _T("Document"), -1, pView);

	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void proxy()
{
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[5];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());
	if(Option[0].Value.pszValue != NULL)
		printf("%s\n", Option[0].Value.pszValue);

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
		printf("PROXY_TYPE_AUTO_PROXY_URL\n");

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
		printf("PROXY_TYPE_AUTO_DETECT\n");

	INTERNET_VERSION_INFO      Version;
	nSize = sizeof(INTERNET_VERSION_INFO);

	InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);

	if(Option[0].Value.pszValue != NULL)
		GlobalFree(Option[0].Value.pszValue);

	if(Option[3].Value.pszValue != NULL)
		GlobalFree(Option[3].Value.pszValue);

	if(Option[4].Value.pszValue != NULL)
		GlobalFree(Option[4].Value.pszValue);
}

int setProxy() 
{ 
	//wchar_t buff[256] = L"http=172.19.1.2:9217"; 
	//// To include server for FTP, HTTPS, and so on, use the string
	//// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
	//INTERNET_PER_CONN_OPTION_LIST    List; 
	//INTERNET_PER_CONN_OPTION         Option[3]; 
	//unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 

	//Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER; 
	//Option[0].Value.pszValue = buff; 

	//Option[1].dwOption = INTERNET_PER_CONN_FLAGS; 
	//Option[1].Value.dwValue = PROXY_TYPE_PROXY; 
	//Option[1].Value.dwValue |= PROXY_TYPE_DIRECT; 
	//// This option sets all the possible connection types for the client. 
	//// This case specifies that the proxy can be used or direct connection is possible.

	//Option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS; 
	//Option[2].Value.pszValue = L"<local>"; 

	//List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 
	//List.pszConnection = NULL; 
	//List.dwOptionCount = 3; 
	//List.dwOptionError = 0; 
	//List.pOptions = Option; 
;
	//if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize)) 
	//	printf("InternetSetOption failed! (%d)\n", GetLastError()); 

	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
	proxyInfo.lpszProxy = (LPCTSTR)"http://172.19.1.2:9217"; 
	proxyInfo.lpszProxyBypass=NULL;

	//session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyInfo,sizeof(INTERNET_OPTION_PROXY),0);
	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY, (LPVOID)&proxyInfo,sizeof(proxyInfo),0); 


	//InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL,NULL); 
	//The connection settings for other instances of Internet Explorer. 

	return 0; 

}


int setNoProxy() 
{ 
	// To include server for FTP, HTTPS, and so on, use the string
	// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
	//INTERNET_PER_CONN_OPTION_LIST    List; 
	//INTERNET_PER_CONN_OPTION         Option[1]; 
	//unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 

	//Option[0].dwOption = INTERNET_PER_CONN_FLAGS; 
	//Option[0].Value.dwValue = PROXY_TYPE_DIRECT; 
	//// This option sets all the possible connection types for the client. 
	//// This case specifies that the proxy can be used or direct connection is possible.



	//List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 
	//List.pszConnection = NULL; 
	//List.dwOptionCount = 1; 
	//List.dwOptionError = 0; 
	//List.pOptions = Option; 

	//if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize)) 
	//	printf("InternetSetOption failed! (%d)\n", GetLastError()); 



	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
	proxyInfo.lpszProxy = NULL;
	proxyInfo.lpszProxyBypass = NULL;

	//session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyInfo,sizeof(INTERNET_OPTION_PROXY),0);
	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY,&proxyInfo,sizeof(proxyInfo),0); 
	//InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL,NULL); 

	return 0; 

}



void readProxy(){

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[5];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());
	if(Option[0].Value.pszValue != NULL)
		AtlTrace("%s\n", Option[0].Value.pszValue);


	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
		AtlTrace("PROXY_TYPE_AUTO_PROXY_URL\n");

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
		AtlTrace("PROXY_TYPE_AUTO_DETECT\n");

	INTERNET_VERSION_INFO      Version;
	nSize = sizeof(INTERNET_VERSION_INFO);

	InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);

	if(Option[0].Value.pszValue != NULL)
		GlobalFree(Option[0].Value.pszValue);

	if(Option[3].Value.pszValue != NULL)
		GlobalFree(Option[3].Value.pszValue);

	if(Option[4].Value.pszValue != NULL)
		GlobalFree(Option[4].Value.pszValue);

	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
	proxyInfo.lpszProxy = NULL;
	proxyInfo.lpszProxyBypass = NULL;

	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY,&proxyInfo,sizeof(proxyInfo),0); 


}	

LRESULT CMainFrame::OnProxyNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	setNoProxy();

	return 0;
}


LRESULT CMainFrame::OnProxyNo2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Setting::m_bProxy = false;

	return 0;
}

LRESULT CMainFrame::OnProxyIE(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	readProxy();
	return 0;
}
LRESULT CMainFrame::OnProxyMySetting(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	setProxy();
	return 0;
}


LRESULT CMainFrame::OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();
	if(nActivePage != -1){
		CLjwBrowserView* pView  = (CLjwBrowserView* ) m_view.GetPageData(nActivePage);
		m_view.RemovePage(nActivePage);
		delete pView;
	}
	else
		::MessageBeep((UINT)-1);

	return 0;
}

LRESULT CMainFrame::OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{


	m_view.RemoveAllPages();

	return 0;
}

LRESULT CMainFrame::OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_view.SetActivePage(nPage);

	return 0;
}

HRESULT ( STDMETHODCALLTYPE my_blur )(IHTMLWindow2 * This)
{
 return S_OK;
}

typedef HRESULT ( STDMETHODCALLTYPE *pblur )(IHTMLWindow2 * This);
pblur g_blur = NULL;

LRESULT CMainFrame::OnAddressKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	IUnknown * pUnk = 0;
	CoCreateInstance(CLSID_HTMLWindow2 , NULL, CLSCTX_INPROC, IID_IHTMLWindow2, (void**)&pUnk );
	if (wParam == VK_RETURN)
	{
		CString strUrl;
		m_edtAddress.GetWindowText(strUrl);
		int nPageIndex = m_view.GetActivePage();
		CLjwBrowserView * pView = NULL;

		if (nPageIndex == -1)
		{
			CLjwBrowserView* pView = new CLjwBrowserView;
			//TODO: Replace with a URL of your choice
			pView->Create(m_view, rcDefault, _T("http://www.microsoft.com"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, 0);
			m_view.AddPage(pView->m_hWnd, _T("Document"), -1, pView);
		}		
		else
		{
			pView = (CLjwBrowserView*) m_view.GetPageData(nPageIndex);
		}
		if (pView)
		{
			CComPtr<IWebBrowser2> spWeb;
			pView->QueryControl(IID_IWebBrowser2, (void**)&spWeb);
			if (spWeb)
			{
				if (!g_blur)
				{
					CComPtr<IDispatch> spDispDoc;
					spWeb->get_Document(&spDispDoc);
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


				
				spWeb->Navigate(CComBSTR(strUrl), NULL, NULL, NULL, NULL);
			}


			CComObject<CDocUIHandler>* comObjUiHandler;
			CComObject<CDocUIHandler>::CreateInstance(&comObjUiHandler);
			comObjUiHandler->AddRef();
			CComPtr<IDispatch> spDis;
			spWeb->get_Document(&spDis);
			CComQIPtr<ICustomDoc> spDoc = spDis;
			//CComQIPtr<IDocHostUIHandlerDispatch> spHandler = comObjUiHandler;
			//HRESULT hret = pView->SetExternalUIHandler(spHandler);
			//CComQIPtr<IDocHostUIHandler> spHandler = comObjUiHandler;
			//spDoc->SetUIHandler(spHandler);



		}

	}
	return ::DefWindowProc(m_edtAddress, uMsg, wParam, lParam);

}

LRESULT CMainFrame::OnAddressShow(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(SW_PARENTCLOSING == lParam)
	{
		while(true);
	}

	return 0;

}
