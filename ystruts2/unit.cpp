#include "stdafx.h"
#include "unit.h"

char* UnicodeToUtf8(const wchar_t* szText, LPDWORD lpdwSize)
{
	if (!szText)
		return NULL;
	int len = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL);
	char *utf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, szText, -1, utf8, len, NULL, NULL);
	*lpdwSize = len;
	return utf8;
}

wchar_t* Utf8ToUnicode(const char *szUtf8, LPDWORD lpdwSize)
{
	if (!szUtf8)
	{
		return NULL;
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	wchar_t *unicode = new wchar_t[dwUnicodeLen + 2];
	memset(unicode, 0, dwUnicodeLen*sizeof(wchar_t)+2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, unicode, dwUnicodeLen);
	*lpdwSize = dwUnicodeLen;
	return unicode;
}


wchar_t* LocalToUnicode(UINT nCodePage, const char *szText)
{
	int nLength = MultiByteToWideChar(nCodePage, 0, szText, -1, NULL, 0);
	wchar_t *lpBuff = new wchar_t[nLength + 1];
	MultiByteToWideChar(nCodePage, 0, szText, -1, lpBuff, nLength);
	lpBuff[nLength] = 0;
	return lpBuff;
}

char* UnicodeToLocal(UINT nCodePage, const wchar_t* szText)
{
	int nLength = WideCharToMultiByte(nCodePage, 0, szText, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLength + 1];
	WideCharToMultiByte(nCodePage, 0, szText, -1, pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	return pBuffer;
}

CDuiString ToNumberChar(CDuiString lpszText)
{
	CDuiString result;
	for (int i = 0; i < lpszText.GetLength();i++)
	{
		TCHAR a = lpszText.GetAt(i);
		TCHAR b[2] = { a,0x00 };
		if (a >= 0x30 && a <= 0x39)
		{
			result.Append(b);
		}
	}
	return result;
}


static unsigned char char_to_hex(unsigned char x)
{
	return (unsigned char)(x > 9 ? x + 55 : x + 48);
}

static int is_alpha_number_char(unsigned char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return 1;
	return 0;
}

//url编码实现   

void urlencode(unsigned char * src, int  src_len, unsigned char * dest, int  dest_len)
{
	unsigned char ch;
	int  len = 0;

	while (len < (dest_len - 4) && *src)
	{
		ch = (unsigned char)*src;
		if (*src == ' ')
		{
			*dest++ = '+';
		}
		else if (is_alpha_number_char(ch) || strchr("=!~*'()", ch))
		{
			*dest++ = *src;
		}
		else
		{
			*dest++ = '%';
			*dest++ = char_to_hex((unsigned char)(ch >> 4));
			*dest++ = char_to_hex((unsigned char)(ch % 16));
		}
		++src;
		++len;
	}
	*dest = 0;
	return;
}



//解url编码实现   

unsigned char* urldecode(unsigned char* encd, unsigned char* decd)
{
	int j, i;
	char *cd = (char*)encd;
	char p[2];
	unsigned int num;
	j = 0;

	for (i = 0; i < strlen(cd); i++)
	{
		memset(p, '/0', 2);
		if (cd[i] != '%')
		{
			decd[j++] = cd[i];
			continue;
		}

		p[0] = cd[++i];
		p[1] = cd[++i];

		p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);
		p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);
		decd[j++] = (unsigned char)(p[0] * 16 + p[1]);

	}
	decd[j] = '/0';

	return decd;
}



static unsigned char hexchars[] = "0123456789ABCDEF";

static int php_htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}


char *php_url_encode(char const *s, int len, int *new_length)
{
	register unsigned char c;
	unsigned char *to, *start;
	unsigned char const *from, *end;

	from = (unsigned char *)s;
	end = (unsigned char *)s + len;
	start = to = (unsigned char *)calloc(1, 3 * len + 1);

	while (from < end)
	{
		c = *from++;

		if (c == ' ')
		{
			*to++ = '+';
		}
		else if ((c < '0' && c != '-' && c != '.') ||
			(c < 'A' && c > '9') ||
			(c > 'Z' && c < 'a' && c != '_') ||
			(c > 'z'))
		{
			to[0] = '%';
			to[1] = hexchars[c >> 4];
			to[2] = hexchars[c & 15];
			to += 3;
		}
		else
		{
			*to++ = c;
		}
	}
	*to = 0;
	if (new_length)
	{
		*new_length = to - start;
	}
	return (char *)start;
}


int php_url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--)
	{
		if (*data == '+')
		{
			*dest = ' ';
		}
		else if (*data == '%' && len >= 2 && isxdigit((int)*(data + 1)) && isxdigit((int)*(data + 2)))
		{
			*dest = (char)php_htoi(data + 1);
			data += 2;
			len -= 2;
		}
		else
		{
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '/0';
	return dest - str;
}