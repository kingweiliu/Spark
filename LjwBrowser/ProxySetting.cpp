#include "StdAfx.h"
#include "ProxySetting.h"
#include <wininet.h>

CProxySetting::CProxySetting(void)
{
}


CProxySetting::~CProxySetting(void)
{
}

void CProxySetting::proxy()
{
	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[5];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());
	if(Option[0].Value.pszValue != NULL)
		printf("%s\n", Option[0].Value.pszValue);

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
		printf("PROXY_TYPE_AUTO_PROXY_URL\n");

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
		printf("PROXY_TYPE_AUTO_DETECT\n");

	INTERNET_VERSION_INFO      Version;
	nSize = sizeof(INTERNET_VERSION_INFO);

	InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);

	if(Option[0].Value.pszValue != NULL)
		GlobalFree(Option[0].Value.pszValue);

	if(Option[3].Value.pszValue != NULL)
		GlobalFree(Option[3].Value.pszValue);

	if(Option[4].Value.pszValue != NULL)
		GlobalFree(Option[4].Value.pszValue);
}

int CProxySetting::setProxy() 
{ 
	//wchar_t buff[256] = L"http=172.19.1.2:9217"; 
	//// To include server for FTP, HTTPS, and so on, use the string
	//// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
	//INTERNET_PER_CONN_OPTION_LIST    List; 
	//INTERNET_PER_CONN_OPTION         Option[3]; 
	//unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 

	//Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER; 
	//Option[0].Value.pszValue = buff; 

	//Option[1].dwOption = INTERNET_PER_CONN_FLAGS; 
	//Option[1].Value.dwValue = PROXY_TYPE_PROXY; 
	//Option[1].Value.dwValue |= PROXY_TYPE_DIRECT; 
	//// This option sets all the possible connection types for the client. 
	//// This case specifies that the proxy can be used or direct connection is possible.

	//Option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS; 
	//Option[2].Value.pszValue = L"<local>"; 

	//List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 
	//List.pszConnection = NULL; 
	//List.dwOptionCount = 3; 
	//List.dwOptionError = 0; 
	//List.pOptions = Option; 
	;
	//if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize)) 
	//	printf("InternetSetOption failed! (%d)\n", GetLastError()); 

	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
	proxyInfo.lpszProxy = (LPCTSTR)"http://172.19.1.2:9217"; 
	proxyInfo.lpszProxyBypass=NULL;

	//session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyInfo,sizeof(INTERNET_OPTION_PROXY),0);
	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY, (LPVOID)&proxyInfo,sizeof(proxyInfo),0); 


	//InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL,NULL); 
	//The connection settings for other instances of Internet Explorer. 

	return 0; 

}

int CProxySetting::setNoProxy() 
{ 
	// To include server for FTP, HTTPS, and so on, use the string
	// (ftp=http://<ProxyServerName>:80; https=https://<ProxyServerName>:80) 
	//INTERNET_PER_CONN_OPTION_LIST    List; 
	//INTERNET_PER_CONN_OPTION         Option[1]; 
	//unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 

	//Option[0].dwOption = INTERNET_PER_CONN_FLAGS; 
	//Option[0].Value.dwValue = PROXY_TYPE_DIRECT; 
	//// This option sets all the possible connection types for the client. 
	//// This case specifies that the proxy can be used or direct connection is possible.



	//List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST); 
	//List.pszConnection = NULL; 
	//List.dwOptionCount = 1; 
	//List.dwOptionError = 0; 
	//List.pOptions = Option; 

	//if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize)) 
	//	printf("InternetSetOption failed! (%d)\n", GetLastError()); 



	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
	proxyInfo.lpszProxy = NULL;
	proxyInfo.lpszProxyBypass = NULL;

	//session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyInfo,sizeof(INTERNET_OPTION_PROXY),0);
	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY,&proxyInfo,sizeof(proxyInfo),0); 
	//InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL,NULL); 

	return 0; 

}



void CProxySetting::readProxy(){

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[5];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;
	Option[1].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
	Option[2].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[3].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	Option[4].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = NULL;
	List.dwOptionCount = 5;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
		printf("InternetQueryOption failed! (%d)\n", GetLastError());
	if(Option[0].Value.pszValue != NULL)
		AtlTrace("%s\n", Option[0].Value.pszValue);


	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL)
		AtlTrace("PROXY_TYPE_AUTO_PROXY_URL\n");

	if((Option[2].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT)
		AtlTrace("PROXY_TYPE_AUTO_DETECT\n");

	INTERNET_VERSION_INFO      Version;
	nSize = sizeof(INTERNET_VERSION_INFO);

	InternetQueryOption(NULL, INTERNET_OPTION_VERSION, &Version, &nSize);

	if(Option[0].Value.pszValue != NULL)
		GlobalFree(Option[0].Value.pszValue);

	if(Option[3].Value.pszValue != NULL)
		GlobalFree(Option[3].Value.pszValue);

	if(Option[4].Value.pszValue != NULL)
		GlobalFree(Option[4].Value.pszValue);

	INTERNET_PROXY_INFO proxyInfo;
	proxyInfo.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
	proxyInfo.lpszProxy = NULL;
	proxyInfo.lpszProxyBypass = NULL;

	HRESULT hr =UrlMkSetSessionOption(INTERNET_OPTION_PROXY,&proxyInfo,sizeof(proxyInfo),0); 


}	