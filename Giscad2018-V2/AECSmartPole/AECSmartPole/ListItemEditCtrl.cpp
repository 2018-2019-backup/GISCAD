// ListItemEditCtrl.cpp : implementation file
//

#include "stdafx.h"

#include "ListItemEditCtrl.h"
//#include "CostEntryDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListItemEditCtrl

CListItemEditCtrl::CListItemEditCtrl()
{
  m_iCurCol = -1;
  m_iCurRow = -1;
  m_bTextEdit = TRUE;
}

CListItemEditCtrl::~CListItemEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CListItemEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CListItemEditCtrl)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListItemEditCtrl message handlers

void CListItemEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if (m_bTextEdit == TRUE)
  {
    CEdit::OnChar(nChar, nRepCnt, nFlags); 
    GetWindowText(csTemp); 
    UpdateParentListCtrlItem();
    return;
  }
  
  if ((( nChar > 47 ) && ( nChar < 58 ) ) || (nChar == 8) )
  {
    CEdit::OnChar(nChar, nRepCnt, nFlags); 
    GetWindowText (csTemp);
    UpdateParentListCtrlItem();  
  }
	else if (nChar == 46 ) 
	{
		GetWindowText(csTemp); 
		if (csTemp.Find('.') < 0)
    {
      CEdit::OnChar(nChar, nRepCnt, nFlags); 
      UpdateParentListCtrlItem();
    }
  }
}

void CListItemEditCtrl::OnKillfocus() 
{ 
  m_pbEditControlVisible = FALSE;
  UpdateParentListCtrlItem();
  
  //((CCostEntryDlg* )GetParent ()->GetParent ())->UpdateMfgAndDesignCost ();
	//DestroyWindow();	
  ShowWindow(SW_HIDE);
}

void CListItemEditCtrl::UpdateParentListCtrlItem()
{
    // Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iCurRow  ;
	dispinfo.item.iSubItem = m_iCurCol; 
	dispinfo.item.pszText = LPTSTR((LPCTSTR)csTemp);
	dispinfo.item.cchTextMax = csTemp.GetLength();

	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), 
					(LPARAM)&dispinfo );

}

void CListItemEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);	  
  if (nChar == 46) UpdateParentListCtrlItem();
}
