#include "StdAfx.h"
#include "IEEventHandler.h"
#include "DocUIHandler.h"

CIEEventHandler::CIEEventHandler()
{
}

CIEEventHandler::~CIEEventHandler(void)
{
}

void CIEEventHandler::setWebbrowser(IWebBrowser2* pWeb)
{
	DispEventAdvise(pWeb, &DIID_DWebBrowserEvents2);
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
		if (spHtmlDoc)
		{
			CComPtr<IHTMLWindow2> spHtmlWnd;
			spHtmlDoc->get_parentWindow(&spHtmlWnd);
			spHtmlWnd->blur();
		}
		
		CComQIPtr<ICustomDoc> spDoc = spDisp;
		if (spDoc)
		{
			CComObject<CDocUIHandler>* objUIHandler;
			CComObject<CDocUIHandler>::CreateInstance(&objUIHandler);
			objUIHandler->AddRef();
			CComQIPtr<IDocHostUIHandler> spHandler = objUIHandler;

			HRESULT hr = spDoc->SetUIHandler(spHandler);
			return hr;
		}
	}

	return S_OK;
}