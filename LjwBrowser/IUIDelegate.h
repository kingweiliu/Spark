#pragma once

class IUiDelegate
{
public:
    void virtual NewWindow(IDispatch **pDisp,
        VARIANT_BOOL *Cancel,
        DWORD dwFlags,
        BSTR bstrUrlContext,
        BSTR bstrUrl) = 0;

    void virtual TitleChange(BSTR Text) = 0;
};

