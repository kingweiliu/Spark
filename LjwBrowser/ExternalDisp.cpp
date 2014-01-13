#include "StdAfx.h"
#include "ExternalDisp.h"


CExternalDisp::CExternalDisp(void)
{
}


CExternalDisp::~CExternalDisp(void)
{
}


 HRESULT CExternalDisp:: GetTypeInfoCount( 
    /* [out] */ __RPC__out UINT *pctinfo) {
        return E_NOTIMPL;
 }

 HRESULT CExternalDisp:: GetTypeInfo( 
    /* [in] */ UINT iTInfo,
    /* [in] */ LCID lcid,
    /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo) {
        return E_NOTIMPL;
 }

 HRESULT CExternalDisp:: GetIDsOfNames( 
    /* [in] */ __RPC__in REFIID riid,
    /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
    /* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
    /* [in] */ LCID lcid,
    /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId){
        if(!::wcscmp(*rgszNames, L"showname")){
            *rgDispId = 260;
               return S_OK;
        }
         
        return E_NOTIMPL;
 }

 /* [local] */ HRESULT CExternalDisp:: Invoke( 
    /* [in] */ DISPID dispIdMember,
    /* [in] */ REFIID riid,
    /* [in] */ LCID lcid,
    /* [in] */ WORD wFlags,
    /* [out][in] */ DISPPARAMS *pDispParams,
    /* [out] */ VARIANT *pVarResult,
    /* [out] */ EXCEPINFO *pExcepInfo,
    /* [out] */ UINT *puArgErr) {

        if(dispIdMember == 260){
            
            CComVariant var;
            var.bstrVal = ::SysAllocString(L"the one");
            var.vt = VT_BSTR;
            var.Detach(pVarResult);
            return S_OK;
        }

        return E_NOTIMPL;
 }

