#pragma once

#include <UrlMon.h>
#include "../third_party/passthruapp/ProtocolImpl.h"

class CAyncPlugginableProtocol
{
public:
	CAyncPlugginableProtocol(void);
	~CAyncPlugginableProtocol(void);

	//注册异步插件协议
	static void RegisterPlugginProtocol();


	class CMyProtocolSink:
		public PassthroughAPP::CInternetProtocolSinkWithSP<CMyProtocolSink>,
		public IHttpNegotiate
	{
	public:
		typedef public PassthroughAPP::CInternetProtocolSinkWithSP<CMyProtocolSink> BaseClass;

		BEGIN_COM_MAP(CMyProtocolSink)
			COM_INTERFACE_ENTRY(IHttpNegotiate)
			COM_INTERFACE_ENTRY_CHAIN(BaseClass)
		END_COM_MAP()

		BEGIN_SERVICE_MAP(CMyProtocolSink)
			SERVICE_ENTRY(IID_IHttpNegotiate)
		END_SERVICE_MAP()

		// IHttpNegotiate
		STDMETHODIMP BeginningTransaction(
			/* [in] */ LPCWSTR szURL,
			/* [in] */ LPCWSTR szHeaders,
			/* [in] */ DWORD dwReserved,
			/* [out] */ LPWSTR *pszAdditionalHeaders);

		STDMETHODIMP OnResponse(
			/* [in] */ DWORD dwResponseCode,
			/* [in] */ LPCWSTR szResponseHeaders,
			/* [in] */ LPCWSTR szRequestHeaders,
			/* [out] */ LPWSTR *pszAdditionalRequestHeaders);

		STDMETHODIMP ReportProgress(
			/* [in] */ ULONG ulStatusCode,
			/* [in] */ LPCWSTR szStatusText);

	};


	class CMyAPP;
	typedef PassthroughAPP::CustomSinkStartPolicy<CMyAPP, CMyProtocolSink> MyStartPolicy;

	class CMyAPP :
		public PassthroughAPP::CInternetProtocol<MyStartPolicy>
	{
	};



};

