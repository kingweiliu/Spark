#pragma once

class CCore;

class CCoreMgr
{
public:
    CCoreMgr(void);
    ~CCoreMgr(void);

    CCore* CreateCore();
    void DestroyCore(CCore* pCore);
};

