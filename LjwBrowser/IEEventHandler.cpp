#include "StdAfx.h"
#include "IEEventHandler.h"
#include "DocUIHandler.h"
#include "HookCenter.h"


CIEEventHandler::CIEEventHandler():m_uiDelegate(NULL)
{
}

CIEEventHandler::~CIEEventHandler(void)
{
}

void CIEEventHandler::setWebbrowser(IWebBrowser2* pWeb)
{
	DispEventAdvise(pWeb, &DIID_DWebBrowserEvents2);
}

void CIEEventHandler::setUiDelegate(IUiDelegate* uidelegate)
{
    m_uiDelegate = uidelegate;
}

HRESULT CIEEventHandler::NavigateComplete2(          IDispatch *pDisp,
					   VARIANT *URL
					   )
{
	CComQIPtr<IWebBrowser2> spWeb = pDisp;

	if(spWeb){


		CComPtr<IDispatch> spDisp;
		spWeb->get_Document(&spDisp);
		
		CComQIPtr<IHTMLDocument2> spHtmlDoc = spDisp;
		if (spHtmlDoc && CHookCenter::m_bHookBlur)
		{
			CComPtr<IHTMLWindow2> spHtmlWnd;
			spHtmlDoc->get_parentWindow(&spHtmlWnd);			
			//测试blur hook
			spHtmlWnd->blur();
		}
		
		CComQIPtr<ICustomDoc> spDoc = spDisp;
		if (spDoc)
		{

			CComObject<CDocUIHandler>* objUIHandler;
			CComObject<CDocUIHandler>::CreateInstance(&objUIHandler);
//			objUIHandler->AddRef(); 此处不需要加一 ?
			//CComQIPtr<IDocHostUIHandler> spHandler = objUIHandler;

			//return spDoc->SetUIHandler(spHandler);
		}
	}

	return S_OK;
}

void CIEEventHandler::OnNewWindow3(IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl){
    if (m_uiDelegate)
        m_uiDelegate->NewWindow(ppDisp, Cancel, dwFlags, bstrUrlContext, bstrUrl);
}

void CIEEventHandler::TitleChange(BSTR Text){
    if(m_uiDelegate)
        m_uiDelegate->TitleChange(Text);
}