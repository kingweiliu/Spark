#pragma once
class CProxySetting
{
public:
	CProxySetting(void);
	~CProxySetting(void);

	static void readProxy();
	static int setNoProxy();
	static void proxy();
	static int setProxy() ;


};

