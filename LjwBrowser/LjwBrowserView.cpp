// LjwBrowserView.cpp : implementation of the CLjwBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "LjwBrowserView.h"
#include "MainFrm.h"

CLjwBrowserView::CLjwBrowserView()
{

}

BOOL CLjwBrowserView::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;

	// give HTML page a chance to translate this message
	return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

LRESULT CLjwBrowserView::onCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



void CLjwBrowserView::Init(CMainFrame* pFrame){
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