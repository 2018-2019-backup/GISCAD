// EditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EditListCtrl.h"
#include "PoleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl

CEditListCtrl::CEditListCtrl()
{
  m_iCurCol = -1;
  m_iCurRow = -1;
  m_ciaEditColumns.RemoveAll ();    
  m_bEditControlVisible = FALSE;
  m_bIsHV = FALSE;
}

CEditListCtrl::~CEditListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CEditListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
  ON_EN_CHANGE(IDC_FLOATEDIT, OnFloatEditChange)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditListCtrl::SetFirstColumnEditable(BOOL bEdit)
{
  m_bFirstColumnEdit = bEdit;
}

void CEditListCtrl::SetIsHV(BOOL bIsHV)
{
  m_bIsHV = bIsHV;
}

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl message handlers

int CEditListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	
	if( col ) *col = 0;

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();
	
	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left 
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

// The returned pointer should not be saved
void CEditListCtrl::EditSubLabel(int nItem, int nCol)
{
  // If this column number is not editable, return
  BOOL bEditable = FALSE;
  for (int iCtr = 0; iCtr < m_ciaEditColumns.GetSize(); iCtr++) if (nCol == m_ciaEditColumns.GetAt(iCtr)) bEditable = TRUE;
  if ((m_ciaEditColumns.GetSize() > 0) && (bEditable == FALSE)) return;

	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return ;

  // If the list item is not checked, do not allow editing
  if ( !GetCheck(nItem) ) return ;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return ;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	if( rect.right > rcClient.right) rect.right = rcClient.right;
  rect.top -= 1;
  rect.bottom -= 1;
  rect.right -=1;
  if (m_edFloatEdit.m_hWnd == NULL)
  {
    m_edFloatEdit.m_pbEditControlVisible = &m_bEditControlVisible;
    dwStyle |= WS_CHILD | WS_VISIBLE|WS_BORDER;
    m_edFloatEdit.CreateEx(WS_EX_CONTROLPARENT, _T("Edit") , _T(""), dwStyle, rect, this, IDC_FLOATEDIT);
    m_edFloatEdit.SetFont(GetParent ()->GetFont ());
  }
  m_edFloatEdit.MoveWindow (rect );
  m_edFloatEdit.SetWindowText (GetItemText( nItem, nCol ));
  m_edFloatEdit.ShowWindow (SW_SHOW);
  m_edFloatEdit.SetSel(0, 100);
  m_edFloatEdit.SetFocus();
  m_bEditControlVisible = TRUE;
  
  m_edFloatEdit.m_iCurRow = m_iCurRow = nItem;
  m_edFloatEdit.m_iCurCol = m_iCurCol = nCol;
  return ;
}

void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CSortListCtrl::OnLButtonDown(nFlags, point);
  if (m_bEditControlVisible)
  {
    m_bEditControlVisible = FALSE;
    CString csTemp;
    m_edFloatEdit.GetWindowText (csTemp);
    m_edFloatEdit.ShowWindow (SW_HIDE);
    SetItemText (m_iCurRow, m_iCurCol, csTemp);
    //m_edFloatEdit.SetWindowText (_T(""));
  }
}

void CEditListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
  if (m_bEditControlVisible)
  {
    m_bEditControlVisible = FALSE;
    CString csTemp;
    m_edFloatEdit.GetWindowText (csTemp);
    m_edFloatEdit.ShowWindow (SW_HIDE);
    SetItemText (m_iCurRow, m_iCurCol, csTemp);
    //m_edFloatEdit.SetWindowText (_T(""));
  }	
	
	*pResult = 0;
}


void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  if (m_bEditControlVisible)
  {
    m_bEditControlVisible = FALSE;
    CString csTemp;
    m_edFloatEdit.GetWindowText (csTemp);
    SetItemText (m_iCurRow, m_iCurCol, csTemp);
    //m_edFloatEdit.ShowWindow (SW_HIDE);
    //m_edFloatEdit.SetWindowText (_T(""));
  }

	int iIndex, iColNum;
	if ((iIndex = HitTestEx(point, &iColNum)) != -1)
	{
		UINT flag = LVIS_FOCUSED;
		if( (GetItemState(iIndex, flag) & flag) == flag && iColNum > 0)
		{
			// Add check for LVS_EDITLABELS
			//if (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS) EditSubLabel(iIndex, iColNum);

      // Removed check for LVS_EDITLABELS (SJ, 23/05/2000)
      //.. Call this function only if it is editable. (GS on 27/05/02)
      //for (int iCounter = 0; iCounter < m_ciaEditColumns.GetSize (); iCounter++)
      {
        //if (int(m_ciaEditColumns.GetAt (iCounter)) == iColNum)
        {
          EditSubLabel(iIndex, iColNum);
        }        
      }
      
		}
		else SetItemState(iIndex, LVIS_SELECTED | LVIS_FOCUSED,	LVIS_SELECTED | LVIS_FOCUSED); 
	}
	
	CSortListCtrl::OnLButtonDblClk(nFlags, point);
}


void CEditListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
  LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &plvDispInfo->item;

  // Do not allow editing of first column, if the flag is set
  if ((m_bFirstColumnEdit == FALSE) && (plvDispInfo->item.iSubItem == 0)) return;

	if (plvItem->pszText != NULL) { SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText); }
	if (m_bIsPoleDlg)
	{
		if (m_bIsHV)
			((CPoleDlg *) GetParent())->OnLvnEndlabeleditPoleHvList(pNMHDR,pResult);
		else
			((CPoleDlg *) GetParent())->OnLvnEndlabeleditPoleLvList(pNMHDR,pResult);
	}
			
  *pResult = FALSE;
}

void CEditListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CEditListCtrl::OnFloatEditChange()
{
  if (m_bEditControlVisible = FALSE)
  {
    CString csTemp;
    m_edFloatEdit.GetWindowText (csTemp);
    SetItemText (m_iCurRow, m_iCurCol, csTemp);
  }
}
