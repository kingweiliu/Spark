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
#include "HookCenter.h"
#include "ProxySetting.h"
#include "ThreadMgr.h"
#include "Singleton.h"
#include "Task.h"

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

    RECT rect = {0, 0, 30, 30};


	m_edtAddress.Create(this, MAINFRAME_ADDRESS_MSGID, m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	AddSimpleReBarBand(m_edtAddress, NULL, TRUE);

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
	//CLjwBrowserView* pView = new CLjwBrowserView;
	//
	//pView->Create(m_view, rcDefault, NULL /*_T("http://www.baidu.com")*/, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, 0);
 //   pView->Init(this);


    DWORD dwThreadID = CSingleton<CThreadMgr>::Instance()->CreateThread();
    //ITask * pTask = new CTask0(this, CMainFrame::FileNewInternal);
    FileNewInternal();
    //CSingleton<CThreadMgr>::Instance()->RunTask(dwThreadID, new CTask0<CMainFrame>(this, &CMainFrame::FileNewInternal));
    return 0;
}

void CMainFrame::FileNewInternal(){
    CCore* pView  = CCore::CreateBrowserView(NULL, this);    
    CSingleton<CThreadMgr>::Instance()->RunTask(0, new CTask0<CMainFrame>(this, &CMainFrame::FileNewInternal));
    DockCore(pView);
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


LRESULT CMainFrame::OnProxyNo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CProxySetting::setNoProxy();
	return 0;
}


LRESULT CMainFrame::OnProxyNo2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Setting::m_bProxy = false;
	return 0;
}

LRESULT CMainFrame::OnProxyIE(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CProxySetting::readProxy();
	return 0;
}
LRESULT CMainFrame::OnProxyMySetting(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CProxySetting::setProxy();
	return 0;
}

LRESULT CMainFrame::OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nActivePage = m_view.GetActivePage();
	if(nActivePage != -1){
		CCore* pView  = (CCore* ) m_view.GetPageData(nActivePage);
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

LRESULT CMainFrame::OnAddressKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	IUnknown * pUnk = 0;
	CoCreateInstance(CLSID_HTMLWindow2 , NULL, CLSCTX_INPROC, IID_IHTMLWindow2, (void**)&pUnk );
	if (wParam == VK_RETURN)
	{
		CString strUrl;
		m_edtAddress.GetWindowText(strUrl);
		int nPageIndex = m_view.GetActivePage();
		CCore * pView = NULL;

		if (nPageIndex == -1)
		{
            CCore* pView  = CCore::CreateBrowserView(m_view, this);
			m_view.AddPage(pView->m_hWnd, _T(""), -1, pView);
		}		
		else
		{
			pView = (CCore*) m_view.GetPageData(nPageIndex);
		}
		if (pView)
		{
			CComPtr<IWebBrowser2> spWeb;
			pView->QueryControl(IID_IWebBrowser2, (void**)&spWeb);
			if (spWeb)
			{
				if (CHookCenter::m_bHookBlur)
				{
					CHookCenter::HookBlur(spWeb);
				}
				
				spWeb->Navigate(CComBSTR(strUrl), NULL, NULL, NULL, NULL);
				//CComObject<CDocUIHandler>* comObjUiHandler;
				//CComObject<CDocUIHandler>::CreateInstance(&comObjUiHandler);
				//comObjUiHandler->AddRef();
				//CComPtr<IDispatch> spDis;
				//spWeb->get_Document(&spDis);
				//CComQIPtr<ICustomDoc> spDoc = spDis;
				//CComQIPtr<IDocHostUIHandlerDispatch> spHandler = comObjUiHandler;
				//HRESULT hret = pView->SetExternalUIHandler(spHandler);
				//CComQIPtr<IDocHostUIHandler> spHandler = comObjUiHandler;
				//spDoc->SetUIHandler(spHandler);
			}
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


void CMainFrame::NewWindow(IDispatch **pDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
{
    CCore* pView = CCore::CreateBrowserView(m_view, this);
    m_view.AddPage(pView->m_hWnd, _T("Document"), -1, pView);
    CComPtr<IWebBrowser2> spWeb;
    pView->QueryControl(&spWeb);
    spWeb->QueryInterface(pDisp);
    return;
}
void CMainFrame::TitleChange(BSTR Text)
{
    int idx = m_view.GetActivePage();
    m_view.SetPageTitle(idx, Text);
}

void CMainFrame::DockCore(CCore* pCore){
    DWORD dwStyle = ::GetWindowLong(pCore->m_hWnd, GWL_STYLE);
    dwStyle &= ~WS_POPUP;
    dwStyle |= WS_CHILD;
    pCore->SetParent(m_view);
    ::SetWindowLong(pCore->m_hWnd, GWL_STYLE, dwStyle);
    m_view.AddPage(pCore->m_hWnd, _T("Document"), -1, pCore);
}