#pragma once
#include "DownloadMgr.h"


//improve the power of core
class CCoreFeature
    : public CComObjectRoot
    , public IServiceProviderImpl<CCoreFeature>
    , public IDownloadManager
    , public INewWindowManager 
{
public:
    CCoreFeature(void);
    ~CCoreFeature(void);

    BEGIN_COM_MAP(CCoreFeature)
        COM_INTERFACE_ENTRY(IServiceProvider)
        COM_INTERFACE_ENTRY(IDownloadManager)
    END_COM_MAP()

    BEGIN_SERVICE_MAP(CCoreFeature)
        SERVICE_ENTRY(IID_IDownloadManager)
        SERVICE_ENTRY(IID_INewWindowManager)
    END_SERVICE_MAP()


    virtual HRESULT STDMETHODCALLTYPE Download( 
        /* [in] */ IMoniker *pmk,
        /* [in] */ IBindCtx *pbc,
        /* [in] */ DWORD dwBindVerb,
        /* [in] */ LONG grfBINDF,
        /* [in] */ BINDINFO *pBindInfo,
        /* [in] */ LPCOLESTR pszHeaders,
        /* [in] */ LPCOLESTR pszRedir,
        /* [in] */ UINT uiCP);

    virtual HRESULT STDMETHODCALLTYPE EvaluateNewWindow( 
        /* [string][in] */ __RPC__in_string LPCWSTR pszUrl,
        /* [string][in] */ __RPC__in_string LPCWSTR pszName,
        /* [string][in] */ __RPC__in_string LPCWSTR pszUrlContext,
        /* [string][in] */ __RPC__in_string LPCWSTR pszFeatures,
        /* [in] */ BOOL fReplace,
        /* [in] */ DWORD dwFlags,
        /* [in] */ DWORD dwUserActionTime) {
            return E_NOTIMPL;
    }

};

