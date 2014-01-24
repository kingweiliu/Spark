// LjwBrowserView.cpp : implementation of the CLjwBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "LjwBrowserView.h"
#include "MainFrm.h"
#include "ThreadMgr.h"
#include "Singleton.h"

CCore* CCore::CreateBrowserView(HWND hWndParent, CMainFrame* pFrame)
{


    CCore* pView = new CCore;
    pView->m_dwThreadID = 0;

//    pView->Create(hWndParent, rcDefault, L"about:blank", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, 0);
    if(hWndParent)
        pView->Create(hWndParent, rcDefault,L"{8856F961-340A-11D0-A96B-00C04FD705A2}", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
    else
        pView->Create(hWndParent, rcDefault,L"{8856F961-340A-11D0-A96B-00C04FD705A2}", WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
    //pView->Create(hWndParent, rcDefault,NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, 0);
    //pView->CreateControl(L"{8856F961-340A-11D0-A96B-00C04FD705A2}", NULL, NULL);

    pView->Init(pFrame);
    
    return pView;
}

CCore::CCore()
{    
}

BOOL CCore::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;

	// give HTML page a chance to translate this message
	return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

LRESULT CCore::onCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



void CCore::Init(CMainFrame* pFrame){
    m_pMainFrame = pFrame;
    if (m_hWnd)
    {
        this->QueryControl(IID_IWebBrowser2, (void**)&m_spWebBrowser);
        if (m_spWebBrowser)
        {
            CComObject<CIEEventHandler>* objEventHandler;
            CComObject<CIEEventHandler>::CreateInstance(&objEventHandler);                                
            objEventHandler->setWebbrowser(m_spWebBrowser);
            objEventHandler->setUiDelegate(pFrame);

            CComObject<CExternalDisp> * objExtDisp;
            CComObject<CExternalDisp>::CreateInstance(&objExtDisp);      
            CComQIPtr<IDispatch> spDis = objExtDisp;
            HRESULT hr  = this->SetExternalDispatch(spDis);

            // TODO: these codes below will cause crash in case of navigating to sina.com
            CComObject<CDocUIHandler> * objUIHandler;
            CComObject<CDocUIHandler>::CreateInstance(&objUIHandler);
            objUIHandler->SetExternalDisp(spDis);
            CComQIPtr<IDocHostUIHandlerDispatch> spUiDisp = objUIHandler;                
            this->SetExternalUIHandler(spUiDisp);


            CComPtr<IAxWinHostWindow> spAxHostWnd;
            this->QueryHost(IID_IAxWinHostWindow, (void**)&spAxHostWnd);
            CComQIPtr<IObjectWithSite> spObjWithSite = spAxHostWnd;
            if(spObjWithSite){
                CComObject<CCoreFeature>* spCoreFeature ;
                CComObject<CCoreFeature>::CreateInstance(&spCoreFeature);
                spCoreFeature->QueryInterface(&m_spDownloadMgr);
                CComPtr<IUnknown> spUnk = m_spDownloadMgr;
                spObjWithSite->SetSite(spUnk);

                CComPtr<IDownloadManager> spDownload;
                CComQIPtr<IServiceProvider> spService = spAxHostWnd;
                spService->QueryService(SID_SDownloadManager, IID_IDownloadManager, 
                    (void**)&spDownload);
            }



        }
    }
}