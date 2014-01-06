#include "StdAfx.h"
#include "DocUIHandler.h"
#include <strsafe.h>

CDocUIHandler::CDocUIHandler(void)
{
}

CDocUIHandler::~CDocUIHandler(void)
{
}

HRESULT CDocUIHandler::GetOptionKeyPath( 
	/* [out] */ BSTR *pchKey,
	/* [in] */ DWORD dw) 
{
	HRESULT hr;
	WCHAR* szKey = L"ljwnb";

	//  cbLength is the length of szKey in bytes.
	size_t cbLength;
	hr = StringCbLengthW(szKey, 1280, &cbLength);
	//  TODO: Add error handling code here.

	if (pchKey)
	{
		*pchKey = (LPOLESTR)CoTaskMemAlloc(cbLength + sizeof(WCHAR));
		if (*pchKey)
			hr = StringCbCopyW(*pchKey, cbLength + sizeof(WCHAR), szKey);
	}
	else
		hr = E_INVALIDARG;

	return hr;

}


HRESULT  CDocUIHandler::GetOverrideKeyPath( 
	/* [annotation][out] */ 
	__deref_out  LPOLESTR *pchKey,
	/* [in] */ DWORD dw) 
{
	return E_NOTIMPL;
}