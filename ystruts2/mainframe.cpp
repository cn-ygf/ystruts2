#include "stdafx.h"
#include "mainframe.h"
#include "scan.h"
#include "unit.h"


CMainFrame *m_pThis = NULL;

CMainFrame::CMainFrame()
{
	m_pThis = this;
}


CMainFrame::~CMainFrame()
{
}


DUI_BEGIN_MESSAGE_MAP(CMainFrame, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()


CDuiString CMainFrame::GetSkinFolder()
{
	return _T("");
}

CDuiString CMainFrame::GetSkinFile()
{
	return _T("main_frame.xml");
}

LPCTSTR	CMainFrame::GetWindowClassName() const
{
	return _T("DUIMainFrame");
}

UINT CMainFrame::GetClassStyle() const
{
	return CS_DBLCLKS; 
}

UILIB_RESOURCETYPE CMainFrame::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIP;
#endif
}

CDuiString CMainFrame::GetZIPFileName() const
{
	return _T("ystruts2.res");
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_USER + 1001)
	{
		CRichEditUI *pShowInfo = (CRichEditUI *)m_PaintManager.FindControl(_T("showinforichedit"));
		CButtonUI *pExecButton = (CButtonUI *)m_PaintManager.FindControl(_T("commandbtn"));
		int ret = wParam;
		if (ret < 0)
		{
			pShowInfo->SetText(_T("连接服务器失败..."));
		}
		else
		{
			wchar_t *lpszResult = (wchar_t *)lParam;
			if (lpszResult)
			{
				pShowInfo->SetText(lpszResult);
				delete[]lpszResult;
			}
		}
		pExecButton->SetEnabled(true);
		return FALSE;
	}
	else if(uMsg == WM_USER + 1002)
	{
		CRichEditUI *pShowInfo = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadshowinfo"));
		CButtonUI *pUploadButton = (CButtonUI *)m_PaintManager.FindControl(_T("uploadbtn"));
		int ret = wParam;
		if (ret < 0)
		{
			pShowInfo->SetText(_T("连接服务器失败..."));
		}
		else
		{
			wchar_t *lpszResult = (wchar_t *)lParam;
			if (lpszResult)
			{
				pShowInfo->SetText(lpszResult);
				delete[]lpszResult;
			}
		}
		pUploadButton->SetEnabled(true);
		return FALSE;
	}
	//else if (uMsg == WM_KEYDOWN && wParam == VK_RETURN)
	//{
	//	int a = lParam;
	//	OnExecButtonClick();
	//	return FALSE;
	//}
	return WindowImplBase::MessageHandler(uMsg, wParam, lParam, bHandled);
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	//if (_tcsicmp(pstrClass, _T("HostList")) == 0)
	//{
	//	return new CHostListUI(m_PaintManager);
	//}
	return NULL;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	CDuiString SenderName = msg.pSender->GetName();
	if (msg.sType == _T("windowinit"))
	{
		CRichEditUI *pCommand = (CRichEditUI *)m_PaintManager.FindControl(_T("commandedit"));
		pCommand->SetFocus();
	}
	else if (msg.sType == _T("itemclick"))
	{
		/*CHostListUI* pHostList = (CHostListUI *)(m_PaintManager.FindControl(_T("hosts")));
		int nSel = pHostList->GetCurSel();
		if (nSel > -1)
		{
			CDuiString name;
			name.Format(_T("option_layout_%d"),nSel);
			CVerticalLayoutUI *pOption = (CVerticalLayoutUI *)pHostList->paint_manager_.FindControl(name);
			pOption->SetVisible(true);
		}*/
	}
	__super::Notify(msg);
}


void CMainFrame::OnClick(TNotifyUI& msg)
{
	CDuiString pSenderName = msg.pSender->GetName();
	if (pSenderName == _T("closebtn"))
	{
		if (MessageBox(GetHWND(),TEXT("是否真的退出程序？"), TEXT("提示"), MB_YESNO | MB_ICONINFORMATION) == IDNO)
		{
			return;
		}
		
		PostQuitMessage(0);
	}
	else if (pSenderName == _T("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	}
	else if (pSenderName == _T("maxbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
	}
	else if (pSenderName == _T("restorebtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, NULL);
	}
	else if (pSenderName == _T("hostbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(0);
	}
	else if (pSenderName == _T("setbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("tab_bar"));
		pTabLayout->SelectItem(2);
	}
	else if (pSenderName == _T("netsetbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("set_tab"));
		pTabLayout->SelectItem(0);
		CButtonUI *pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("netsetbtn"));
		pBtn->SetTextColor(0xff1790ed);
		pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("aboutbtn"));
		pBtn->SetTextColor(0xff000000);

		CVerticalLayoutUI *pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("net_layout"));
		pLayout->SetVisible(true);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("about_layout"));
		pLayout->SetVisible(false);
	}
	else if (pSenderName == _T("aboutbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("set_tab"));
		pTabLayout->SelectItem(1);
		CButtonUI *pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("netsetbtn"));
		pBtn->SetTextColor(0xff000000);
		pBtn = (CButtonUI *)m_PaintManager.FindControl(_T("aboutbtn"));
		pBtn->SetTextColor(0xff1790ed);
		CVerticalLayoutUI *pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("net_layout"));
		pLayout->SetVisible(false);
		pLayout = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("about_layout"));
		pLayout->SetVisible(true);
	}
	else if (pSenderName == _T("commandbtn"))
	{
		OnExecButtonClick();
	}
	else if (pSenderName == _T("uploadbtn"))
	{
		OnUploadButtonClick();
	}
	else if (pSenderName == _T("commandtabbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("main_tab"));
		pTabLayout->SelectItem(0);
	}
	else if (pSenderName == _T("uploadtabbtn"))
	{
		CTabLayoutUI *pTabLayout = (CTabLayoutUI *)m_PaintManager.FindControl(_T("main_tab"));
		pTabLayout->SelectItem(1);
	}
}

void CMainFrame::OnUploadButtonClick()
{
	CButtonUI *pUploadButton = (CButtonUI *)m_PaintManager.FindControl(_T("uploadbtn"));
	CEditUI *pTarget = (CEditUI *)m_PaintManager.FindControl(_T("targetedit"));
	CDuiString target_string = pTarget->GetText();
	if (target_string.GetLength() < 1)
	{
		return;
	}
	CRichEditUI *pUploadCode = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadcode"));
	CDuiString uploadcode_string = pUploadCode->GetText();
	if (uploadcode_string.GetLength() < 1)
	{
		return;
	}
	CRichEditUI *pUploadpath = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadpathrichedit"));
	CDuiString uploadpath_string = pUploadpath->GetText();
	if (uploadpath_string.GetLength() < 1)
	{
		return;
	}
	pUploadButton->SetEnabled(false);
	std::thread th1(&CMainFrame::UploadThread, this);
	th1.detach();
}

void CMainFrame::UploadThread()
{
	CButtonUI *pUploadButton = (CButtonUI *)m_PaintManager.FindControl(_T("uploadbtn"));
	CEditUI *pTarget = (CEditUI *)m_PaintManager.FindControl(_T("targetedit"));
	CDuiString target_string = pTarget->GetText();
	CComboUI *pEncodeCombo = (CComboUI *)m_PaintManager.FindControl(_T("encodecombo"));
	CComboUI *pExpCombo = (CComboUI *)m_PaintManager.FindControl(_T("expcombo"));
	int encode = 0;
	int exp = 0;
	encode = pEncodeCombo->GetCurSel();
	exp = pExpCombo->GetCurSel();
	CRichEditUI *pShowInfo = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadshowinfo"));
	CRichEditUI *pUploadpath = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadpathrichedit"));
	CDuiString upload_string = pUploadpath->GetText();
	CRichEditUI *pUploadCode = (CRichEditUI *)m_PaintManager.FindControl(_T("uploadcode"));
	CDuiString uploadcode_string = pUploadCode->GetText();
	std::string result;
	std::wstring url;
	std::wstring filename;
	std::wstring content;
	
	url.append(target_string.GetData());
	filename.append(upload_string.GetData());
	content.append(uploadcode_string.GetData());

	char *c_content = UnicodeToLocal(CP_ACP, content.c_str());
	int newlen = 0;
	char *dest_content = php_url_encode(c_content, lstrlenA(c_content), &newlen);
	delete[]c_content;
	wchar_t *w_content = LocalToUnicode(CP_ACP, dest_content);
	free(dest_content);
	wstring new_content;
	new_content.append(w_content);
	delete[]w_content;
	int ret = s2045_upload(url, filename, new_content, result);

	if (ret < 0)
	{
		this->PostMessage(WM_USER + 1002, ret, 0);
		return;
	}
	pUploadButton->SetEnabled(true);
	DWORD dwWriteByte = 0;
	wchar_t *lpszResult = NULL;
	if (encode == 0)
	{
		lpszResult = Utf8ToUnicode(result.c_str(), &dwWriteByte);
	}
	else if (encode == 1)
	{
		lpszResult = LocalToUnicode(936, result.c_str());
	}
	else if (encode == 2)
	{
		lpszResult = LocalToUnicode(950, result.c_str());
	}
	else
	{
		lpszResult = Utf8ToUnicode(result.c_str(), &dwWriteByte);
	}
	this->PostMessage(WM_USER + 1002, ret, (LPARAM)lpszResult);
}

void CMainFrame::OnExecButtonClick()
{
	CButtonUI *pExecButton = (CButtonUI *)m_PaintManager.FindControl(_T("commandbtn"));
	CEditUI *pTarget = (CEditUI *)m_PaintManager.FindControl(_T("targetedit"));
	CDuiString target_string = pTarget->GetText();
	if (target_string.GetLength() < 1)
	{
		return;
	}
	CRichEditUI *pCommand = (CRichEditUI *)m_PaintManager.FindControl(_T("commandedit"));
	CDuiString command_string = pCommand->GetText();
	if (command_string.GetLength() < 1)
	{
		return;
	}
	CRichEditUI *pShowInfo = (CRichEditUI *)m_PaintManager.FindControl(_T("showinforichedit"));
	pShowInfo->SetText(_T(""));
	pExecButton->SetEnabled(false);
	pShowInfo->SetText(_T("测试中..."));
	std::thread th1(&CMainFrame::TestThread, this);
	th1.detach();
}



void CMainFrame::TestThread()
{
	CButtonUI *pExecButton = (CButtonUI *)m_PaintManager.FindControl(_T("commandbtn"));
	CEditUI *pTarget = (CEditUI *)m_PaintManager.FindControl(_T("targetedit"));
	CDuiString target_string = pTarget->GetText();
	CComboUI *pEncodeCombo = (CComboUI *)m_PaintManager.FindControl(_T("encodecombo"));
	CComboUI *pExpCombo = (CComboUI *)m_PaintManager.FindControl(_T("expcombo"));
	int encode = 0;
	int exp = 0;
	encode = pEncodeCombo->GetCurSel();
	exp = pExpCombo->GetCurSel();
	CRichEditUI *pCommand = (CRichEditUI *)m_PaintManager.FindControl(_T("commandedit"));
	CDuiString command_string = pCommand->GetText();
	CRichEditUI *pShowInfo = (CRichEditUI *)m_PaintManager.FindControl(_T("showinforichedit"));
	std::string result;
	std::wstring url;
	std::wstring cmd;
	url.append(target_string.GetData());
	cmd.append(command_string.GetData());

	int ret = s2045_exec(url, cmd, result);

	if (ret < 0)
	{
		this->PostMessage(WM_USER + 1001, ret, 0);
		return;
	}
	pExecButton->SetEnabled(true);
	DWORD dwWriteByte = 0;
	wchar_t *lpszResult = NULL;
	if (encode == 0)
	{
		lpszResult = Utf8ToUnicode(result.c_str(), &dwWriteByte);
	}
	else if (encode == 1)
	{
		lpszResult = LocalToUnicode(936, result.c_str());
	}
	else if (encode == 2)
	{
		lpszResult = LocalToUnicode(950, result.c_str());
	}
	else
	{
		lpszResult = Utf8ToUnicode(result.c_str(), &dwWriteByte);
	}
	this->PostMessage(WM_USER + 1001, ret, (LPARAM)lpszResult);
}