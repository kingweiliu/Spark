// LjwBrowserView.h : interface of the CLjwBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "IEEventHandler.h"

class CLjwBrowserView : public CWindowImpl<CLjwBrowserView, CAxWindow>
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
				objEventHandler->AddRef();

				objEventHandler->setWebbrowser(m_spWebBrowser);
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
	CIEEventHandler* m_eventHandler;
	CComPtr<IWebBrowser2> m_spWebBrowser;


};
