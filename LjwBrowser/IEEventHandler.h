#pragma once
#include <ExDispid.h>
#include <ExDisp.h>

class CIEEventHandler:
	public CComObjectRoot,
	public IDispEventImpl<0 , CIEEventHandler, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
	CIEEventHandler();
	~CIEEventHandler(void);

	BEGIN_COM_MAP(CIEEventHandler)
	END_COM_MAP()

	BEGIN_SINK_MAP(CIEEventHandler)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, NavigateComplete2)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)

	END_SINK_MAP()

	STDMETHOD( NavigateComplete2)(IDispatch *pDisp,
	VARIANT *URL
	);

	STDMETHOD( OnBeforeNavigate2)( IDispatch *pDisp,
		VARIANT *&url,
		VARIANT *&Flags,
		VARIANT *&TargetFrameName,
		VARIANT *&PostData,
		VARIANT *&Headers,
		VARIANT_BOOL *&Cancel
		)
	{
		//::MessageBox(NULL, L"KO", L"KO", MB_OK);
		return S_OK;
	}

	void setWebbrowser(IWebBrowser2* pWeb);


};
