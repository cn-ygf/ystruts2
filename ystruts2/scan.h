#pragma once
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")

int s2045_exec(const wstring url, const wstring cmd, string &result);
int s2045_upload(const wstring url, const wstring filename, const wstring filecontent, string &cresult);

string ws2s(const wstring& ws);
wstring s2ws(const string& s);