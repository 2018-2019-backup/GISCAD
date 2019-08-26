// IEComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "IEComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIEComboBox::CIEComboBox()
{

}

CIEComboBox::~CIEComboBox()
{
	m_nDropHeight = 0; 
	m_nMaxChars = 0;
}

void CIEComboBox::SwitchToSimple()
{
	// if ((GetStyle() & CBS_SIMPLE) == CBS_SIMPLE) return;
	
	CRect rc;
	GetDroppedControlRect(&rc);
	m_nDropHeight = rc.Height();

	ModifyStyle(CBS_DROPDOWNLIST, CBS_SIMPLE);
	RecreateComboBox();

	SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );

	COMBOBOXINFO info;
	ZeroMemory(&info, sizeof(COMBOBOXINFO)); 
	info.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&info);
	if (m_nMaxChars > 0)	::SendMessage(info.hwndItem, EM_LIMITTEXT, m_nMaxChars, 0);
}

void CIEComboBox::SwitchToDropDownList()
{
	if ((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST) return;

	ModifyStyle(CBS_SIMPLE, CBS_DROPDOWNLIST | WS_VSCROLL);
	RecreateComboBox();

	SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

  COMBOBOXINFO info;
	ZeroMemory(&info, sizeof(COMBOBOXINFO));
	info.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&info);
	
	CRect rectEdit, rectCombo;

	GetWindowRect(&rectCombo);
  rectEdit = rectCombo;
	::GetWindowRect(info.hwndItem, &rectEdit);

	int nNonDropHeight = rectEdit.Height() + (::GetSystemMetrics(SM_CYEDGE) * 2);

	rectCombo.bottom = rectCombo.top + nNonDropHeight + m_nDropHeight;

	if (::GetParent(GetSafeHwnd()))
	{
		::ScreenToClient(::GetParent(GetSafeHwnd()), &rectCombo.TopLeft());
		::ScreenToClient(::GetParent(GetSafeHwnd()), &rectCombo.BottomRight());
	}

	::MoveWindow(GetSafeHwnd(), rectCombo.left, rectCombo.top, rectCombo.Width(), rectCombo.Height(), TRUE);

	if (m_nMaxChars > 0)::SendMessage(info.hwndItem, EM_LIMITTEXT, m_nMaxChars, 0);
}

BOOL CIEComboBox::RecreateComboBox(LPVOID lpParam)
{
	if (this->GetSafeHwnd() == NULL)
		return FALSE;

	CWnd* pParent = GetParent();

	if (pParent == NULL)
		return FALSE;

	// get current attributes
	DWORD dwStyle = GetStyle();
	DWORD dwStyleEx = GetExStyle();
	CRect rc;

	// GetDroppedControlRect(&rc);

	// DP: Altered as per suggestion by CheGueVerra
	GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);    // map to client co-ords
	UINT nID = GetDlgCtrlID();
	CFont* pFont = GetFont();
	CWnd* pWndAfter = GetNextWindow(GW_HWNDPREV);

	// get the currently selected text (and whether it is a valid list selection)
	CString sCurText;
	int nCurSel = GetCurSel();
	BOOL bItemSelValid = nCurSel != -1;
	if (bItemSelValid)
		GetLBText(nCurSel, sCurText);
	else
		GetWindowText(sCurText);

	// copy the combo box items into a temp combobox (not sorted)
	// along with each item's data
	CComboBox comboNew;
	if (! comboNew.CreateEx(dwStyleEx, _T("COMBOBOX"), _T(""), dwStyle, rc, pParent, nID, lpParam))
		return FALSE;
	comboNew.SetFont(pFont);
	int nNumItems = GetCount();
	for (int n = 0; n < nNumItems; n++)
	{
		CString sText;
		GetLBText(n, sText);
		int nNewIndex = comboNew.AddString(sText);
		comboNew.SetItemData(nNewIndex, GetItemData(n));
	}
	// re-set selected text
	if (bItemSelValid)
		comboNew.SetCurSel(comboNew.FindStringExact(-1, sCurText));
	else
		comboNew.SetWindowText(sCurText);

	// destroy the existing window, then attach the new one
	DestroyWindow();
	HWND hwnd = comboNew.Detach();
	Attach(hwnd);

	// position correctly in z-order
	SetWindowPos(pWndAfter == NULL ? &CWnd::wndBottom : pWndAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

// CIEComboBox message handlers

