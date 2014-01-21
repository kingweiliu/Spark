#include "StdAfx.h"
#include "AyncPlugginableProtocol.h"
#include "../third_party/passthruapp/ProtocolCF.h"

CAyncPlugginableProtocol::CAyncPlugginableProtocol(void)
{
}


CAyncPlugginableProtocol::~CAyncPlugginableProtocol(void)
{
}

//The URL moniker calls this method before it sends an HTTP request. It notifies the client of the URL that is being bound to at the beginning 
//of the HTTP transaction. It also enables the client to add additional headers, such as Accept-Language, to the request.
//每一个网络请求都经过这个函数, 可以通过这个过滤请求。 
// IHttpNegotiate
//
STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::BeginningTransaction(
	/* [in] */ LPCWSTR szURL,
	/* [in] */ LPCWSTR szHeaders,
	/* [in] */ DWORD dwReserved,
	/* [out] */ LPWSTR *pszAdditionalHeaders){
    
    //获取默认的实现， 回调默认实现
    CComPtr<IHttpNegotiate> spHttpNegotiate;
    QueryServiceFromClient(&spHttpNegotiate);
    if (spHttpNegotiate)
    {
        return spHttpNegotiate->BeginningTransaction(szURL, szHeaders, dwReserved, pszAdditionalHeaders);
    }
    

    m_strUrl = szURL;
    OutputDebugString(CString(szURL) + "\n");
    /*CString strUrl = szURL;
    if (-1 != strUrl.Find(L".jpg") || -1 != strUrl.Find(L".gif") || -1!=strUrl.Find(L".png"))
    {
        return E_ABORT;
    }
*/
    //增加header; 
    WCHAR* szNewHeader = (LPWSTR)CoTaskMemAlloc(1024 * sizeof(WCHAR));
    memset(szNewHeader, 0, sizeof(WCHAR)*1024);
    *pszAdditionalHeaders = szNewHeader;
	return S_OK;
}

STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::OnResponse(
	/* [in] */ DWORD dwResponseCode,
	/* [in] */ LPCWSTR szResponseHeaders,
	/* [in] */ LPCWSTR szRequestHeaders,
	/* [out] */ LPWSTR *pszAdditionalRequestHeaders){
    CComPtr<IHttpNegotiate> spHttpNegotiate;
    QueryServiceFromClient(&spHttpNegotiate);
    if (spHttpNegotiate)
    {
        return spHttpNegotiate->OnResponse(dwResponseCode, szResponseHeaders, szRequestHeaders, pszAdditionalRequestHeaders);
    }
}

STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::ReportProgress(
	/* [in] */ ULONG ulStatusCode,
	/* [in] */ LPCWSTR szStatusText){
    return BaseClass::ReportProgress(ulStatusCode, szStatusText);
}


void CAyncPlugginableProtocol::RegisterPlugginProtocol(){
	typedef PassthroughAPP::CMetaFactory<PassthroughAPP::CComClassFactoryProtocol,
		CMyAPP> MetaFactory;
	
	CComPtr<IClassFactory> m_CFHTTP;
	CComPtr<IClassFactory> m_CFHTTPS;

	CComPtr<IInternetSession> pInternetSession;
	CoInternetGetSession(0, &pInternetSession, 0);

	MetaFactory::CreateInstance(CLSID_HttpProtocol, &m_CFHTTP);
	pInternetSession->RegisterNameSpace(m_CFHTTP, CLSID_NULL, L"http", 0, 0, 0);

	MetaFactory::CreateInstance(CLSID_HttpSProtocol, &m_CFHTTPS);
	pInternetSession->RegisterNameSpace(m_CFHTTPS, CLSID_NULL, L"https", 0, 0, 0);
}


// IInternetProtocol
STDMETHODIMP CAyncPlugginableProtocol::CMyAPP::Read(
    /* [in, out] */ void *pv,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbRead)
{
    return BaseClass::Read(pv, cb, pcbRead);
    //we can save
    HRESULT hr = BaseClass::Read(pv, cb, pcbRead);
    CMyProtocolSink* pSink = BaseClass::GetSink();
   // OutputDebugString(pSink->m_strUrl+"\n");
    ATLASSERT(TRUE);
    return hr;
}