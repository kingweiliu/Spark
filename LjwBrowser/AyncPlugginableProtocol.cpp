#include "StdAfx.h"
#include "AyncPlugginableProtocol.h"
#include "../third_party/passthruapp/ProtocolCF.h"

CAyncPlugginableProtocol::CAyncPlugginableProtocol(void)
{
}


CAyncPlugginableProtocol::~CAyncPlugginableProtocol(void)
{
}

// IHttpNegotiate
STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::BeginningTransaction(
	/* [in] */ LPCWSTR szURL,
	/* [in] */ LPCWSTR szHeaders,
	/* [in] */ DWORD dwReserved,
	/* [out] */ LPWSTR *pszAdditionalHeaders){
		return S_OK;
}

STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::OnResponse(
	/* [in] */ DWORD dwResponseCode,
	/* [in] */ LPCWSTR szResponseHeaders,
	/* [in] */ LPCWSTR szRequestHeaders,
	/* [out] */ LPWSTR *pszAdditionalRequestHeaders){
		return S_OK;
}

STDMETHODIMP CAyncPlugginableProtocol::CMyProtocolSink::ReportProgress(
	/* [in] */ ULONG ulStatusCode,
	/* [in] */ LPCWSTR szStatusText){
		return S_OK;
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
