#include "StdAfx.h"
#include "CoreFeature.h"


CCoreFeature::CCoreFeature(void)
{
}


CCoreFeature::~CCoreFeature(void)
{
}

HRESULT CCoreFeature::Download( 
    /* [in] */ IMoniker *pmk,
    /* [in] */ IBindCtx *pbc,
    /* [in] */ DWORD dwBindVerb,
    /* [in] */ LONG grfBINDF,
    /* [in] */ BINDINFO *pBindInfo,
    /* [in] */ LPCOLESTR pszHeaders,
    /* [in] */ LPCOLESTR pszRedir,
    /* [in] */ UINT uiCP){
        return E_NOTIMPL;
}

