// LjwBrowserView.h : interface of the CLjwBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IEEventHandler.h"
#include "ExternalDisp.h"
#include "DocUIHandler.h"
#include "DownloadMgr.h"
#include "CoreFeature.h"

class CLjwBrowserView : 
    public CWindowImpl<CLjwBrowserView, CAxWindow>
{
public:

	CLjwBrowserView();

	~CLjwBrowserView()
	{
		int i=0;
		i+=1;
		return;
	}
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())


	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CLjwBrowserView)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
	END_MSG_MAP()

   /* BEGIN_COM_MAP(CLjwBrowserView)
        COM_INTERFACE_ENTRY(IServiceProvider)
        COM_INTERFACE_ENTRY(IDownloadManager)
    END_COM_MAP()

    BEGIN_SERVICE_MAP(CLjwBrowserView)
        SERVICE_ENTRY(IDownloadManager)
    END_SERVICE_MAP()*/

	HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		CWindowImpl<CLjwBrowserView, CAxWindow>::Create(hWndParent, rect, szWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam);
		if (m_hWnd)
		{
            this->QueryControl(IID_IWebBrowser2, (void**)&m_spWebBrowser);
			if (m_spWebBrowser)
			{
                CComObject<CIEEventHandler>* objEventHandler;
                CComObject<CIEEventHandler>::CreateInstance(&objEventHandler);                                
                objEventHandler->setWebbrowser(m_spWebBrowser);

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
		return m_hWnd;
	}

	LRESULT onCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)


private:
	CComPtr<IWebBrowser2> m_spWebBrowser;
    CComPtr<IDownloadManager> m_spDownloadMgr;

};
