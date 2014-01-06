#pragma once

#include <atliface.h>

class CDocUIHandler :
	public CComObjectRoot
	//public IDispatchImpl<IDocHostUIHandlerDispatch, &IID_IDocHostUIHandlerDispatch, &LIBID_ATLLib>
	, public IDispatchImpl<IDocHostUIHandler2>
{
public:
	CDocUIHandler(void);
	~CDocUIHandler(void);

	BEGIN_COM_MAP(CDocUIHandler)
		//COM_INTERFACE_ENTRY(IDocHostUIHandlerDispatch)
		COM_INTERFACE_ENTRY(IDocHostUIHandler2)
	END_COM_MAP()


//	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
//	/* [in] */ DWORD dwID,
//	/* [in] */ DWORD x,
//	/* [in] */ DWORD y,
//	/* [in] */ IUnknown *pcmdtReserved,
//	/* [in] */ IDispatch *pdispReserved,
//	/* [retval][out] */ HRESULT *dwRetVal){return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
//		/* [out][in] */ DWORD *pdwFlags,
//		/* [out][in] */ DWORD *pdwDoubleClick) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
//		/* [in] */ DWORD dwID,
//		/* [in] */ IUnknown *pActiveObject,
//		/* [in] */ IUnknown *pCommandTarget,
//		/* [in] */ IUnknown *pFrame,
//		/* [in] */ IUnknown *pDoc,
//		/* [retval][out] */ HRESULT *dwRetVal) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE HideUI( void) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
//		/* [in] */ VARIANT_BOOL fEnable) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
//		/* [in] */ VARIANT_BOOL fActivate) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
//		/* [in] */ VARIANT_BOOL fActivate) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
//		/* [in] */ long left,
//		/* [in] */ long top,
//		/* [in] */ long right,
//		/* [in] */ long bottom,
//		/* [in] */ IUnknown *pUIWindow,
//		/* [in] */ VARIANT_BOOL fFrameWindow) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
//		/* [in] */ DWORD_PTR hWnd,
//		/* [in] */ DWORD nMessage,
//		/* [in] */ DWORD_PTR wParam,
//		/* [in] */ DWORD_PTR lParam,
//		/* [in] */ BSTR bstrGuidCmdGroup,
//		/* [in] */ DWORD nCmdID,
//		/* [retval][out] */ HRESULT *dwRetVal) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
//		/* [out] */ BSTR *pbstrKey,
//		/* [in] */ DWORD dw) ;
//
//	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
//		/* [in] */ IUnknown *pDropTarget,
//		/* [out] */ IUnknown **ppDropTarget) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE GetExternal( 
//		/* [out] */ IDispatch **ppDispatch) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
//		/* [in] */ DWORD dwTranslate,
//		/* [in] */ BSTR bstrURLIn,
//		/* [out] */ BSTR *pbstrURLOut) {return E_NOTIMPL;}
//
//	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
//		/* [in] */ IUnknown *pDO,
//		/* [out] */ IUnknown **ppDORet) {return E_NOTIMPL;}


virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
	/* [in] */ DWORD dwID,
	/* [in] */ POINT *ppt,
	/* [in] */ IUnknown *pcmdtReserved,
	/* [in] */ IDispatch *pdispReserved) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
		/* [out][in] */ DOCHOSTUIINFO *pInfo) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [in] */ IOleCommandTarget *pCommandTarget,
		/* [in] */ IOleInPlaceFrame *pFrame,
		/* [in] */ IOleInPlaceUIWindow *pDoc) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE HideUI( void) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ BOOL fEnable) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ BOOL fActivate) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ BOOL fActivate) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow *pUIWindow,
		/* [in] */ BOOL fRameWindow) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
		/* [annotation][out] */ 
		__out  LPOLESTR *pchKey,
		/* [in] */ DWORD dw) ;

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
		/* [in] */ IDropTarget *pDropTarget,
		/* [out] */ IDropTarget **ppDropTarget) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetExternal( 
		/* [out] */ IDispatch **ppDispatch) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
		/* [in] */ DWORD dwTranslate,
		/* [annotation][in] */ 
		__in __nullterminated  OLECHAR *pchURLIn,
		/* [annotation][out] */ 
		__out  OLECHAR **ppchURLOut) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
		/* [in] */ IDataObject *pDO,
		/* [out] */ IDataObject **ppDORet) {return E_NOTIMPL;}

	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
		/* [annotation][out] */ 
		__deref_out  LPOLESTR *pchKey,
		/* [in] */ DWORD dw) ;


};
