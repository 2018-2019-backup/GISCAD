// SortListCtrl.cpp : implementation file
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
	m_nSortedCol = -1;
	m_bSortAscending = TRUE;
	m_bSaveColumnState = FALSE;
	m_strSection.Empty();
	m_strControl.Empty();

  //.. By default donot allow sorting of columns
  m_bAllowSort = FALSE;
}

CSortListCtrl::~CSortListCtrl()
{
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnItemclick) 
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnItemclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

void CSortListCtrl::AllowSort(BOOL bAllow)
{
  m_bAllowSort = bAllow;
}

void CSortListCtrl::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

	if ( (m_bAllowSort) && (phdn->iButton == 0) )
	{
		// User clicked on header using left mouse button
		if( phdn->iItem == m_nSortedCol )
			m_bSortAscending = !m_bSortAscending;
		else
			m_bSortAscending = TRUE;

		m_nSortedCol = phdn->iItem;
    UINT nSortCol = static_cast<UINT>(m_nSortedCol); 
    // If the column found in the numbers column list then use seperate function
    for (int iC = 0; iC < m_uiaNumberSortColumns.GetSize(); iC++)
    {
      if (nSortCol == m_uiaNumberSortColumns.GetAt(iC))
      {
        BeginWaitCursor();
        SortTextItemsOnNumber( m_nSortedCol, m_bSortAscending );
        m_headerCtrl.SetSortImage(m_nSortedCol, m_bSortAscending);
        EndWaitCursor();
        *pResult = 0;
        return;
      }
    }
    // Check if the column is in special sort type
    for ( int  iC = 0; iC < m_uiaSpecialSortColumns.GetSize(); iC++)
    {
      if (nSortCol == m_uiaSpecialSortColumns.GetAt(iC))
      {
        BeginWaitCursor();
        // First sort on the length
        SortTextItemsOnLength( m_nSortedCol, m_bSortAscending );
        int iLength = GetItemText(0, m_nSortedCol).GetLength();
        int iStart = 0;
        for (int iCount = 1; iCount < GetItemCount();iCount++)
        {
          // Sort on the item text if their legths are different
          if (GetItemText(iCount, m_nSortedCol).GetLength() != iLength)
          {          
            SortSpecialTextItems(m_nSortedCol, m_bSortAscending, iStart, iCount - 1);
            iStart = iCount;
            iLength = GetItemText(iCount, m_nSortedCol).GetLength();          
          }
        }
      
        if (GetItemText(GetItemCount() - 1, m_nSortedCol).GetLength() == iLength)
        {
          SortSpecialTextItems(m_nSortedCol, m_bSortAscending, iStart);
        }
        EndWaitCursor();
        m_headerCtrl.SetSortImage(m_nSortedCol, m_bSortAscending);
        *pResult = 0;
        return;
      }
    }
    BeginWaitCursor();    
		SortTextItems( m_nSortedCol, m_bSortAscending );
    EndWaitCursor();
		m_headerCtrl.SetSortImage(m_nSortedCol, m_bSortAscending);
	}

	*pResult = 0;
}

BOOL CSortListCtrl::SortTextItemsOnNumber( int nCol, BOOL bAscending, int low /*= 0*/, int high /*= -1*/ )
{
	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;
  

	if( high == -1 ) high = GetItemCount() - 1;

	int lo = low;
	int hi = high;
	
	if( hi <= lo ) return FALSE;
  
	double dMidItem = _tstof(GetItemText( (lo+hi)/2, nCol ));

	// loop through the list until indices cross
	while( lo <= hi )
	{		
		// find the first element that is greater than or equal to
		// the partition element starting from the left Index.
		if( bAscending ) {
			while( ( lo < high ) && ( _tstof(GetItemText(lo, nCol)) < dMidItem ) )
				++lo;
		}
		else {
			while( ( lo < high ) && ( _tstof(GetItemText(lo, nCol)) > dMidItem ) )
				++lo;
		}

		// find an element that is smaller than or equal to
		// the partition element starting from the right Index.
		if( bAscending ) {
			while( ( hi > low ) && ( _tstof(GetItemText(hi, nCol)) > dMidItem ) )
				--hi;
		}

		else {
			while( ( hi > low ) && ( _tstof(GetItemText(hi, nCol)) < dMidItem ) )
				--hi;
		}

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( GetItemText(lo, nCol) != GetItemText(hi, nCol))
			{
        // rowText will hold all column text for one row
    		CStringArray rowText;

				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount =
					((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
					LVIS_FOCUSED | LVIS_SELECTED |
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				
				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;
				
				GetItem( &lvitemlo );
				GetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(lo, i, GetItemText(hi, i));
				
				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(hi, i, rowText[i]);
				
				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}
			
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItemsOnNumber( nCol, bAscending , low, hi);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItemsOnNumber( nCol, bAscending , lo, high );
	
	return TRUE;
}


// SortTextItems	- Sort the list based on column text
// Returns		- Returns true for success
// nCol			- column that contains the text to be sorted
// bAscending		- indicate sort order
// low			- row to start scanning from - default row is 0
// high			- row to end scan. -1 indicates last row

BOOL CSortListCtrl::SortTextItems( int nCol, BOOL bAscending, int low /*= 0*/, int high /*= -1*/ )
{
	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;
  

	if( high == -1 ) high = GetItemCount() - 1;

	int lo = low;
	int hi = high;
	CString midItem;

	if( hi <= lo ) return FALSE;
  
	midItem = GetItemText( (lo+hi)/2, nCol );

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;

		// find the first element that is greater than or equal to
		// the partition element starting from the left Index.
		if( bAscending ) {
			while( ( lo < high ) && ( GetItemText(lo, nCol) < midItem ) )
				++lo;
		}
		else {
			while( ( lo < high ) && ( GetItemText(lo, nCol) > midItem ) )
				++lo;
		}

		// find an element that is smaller than or equal to
		// the partition element starting from the right Index.
		if( bAscending ) {
			while( ( hi > low ) && ( GetItemText(hi, nCol) > midItem ) )
				--hi;
		}

		else {
			while( ( hi > low ) && ( GetItemText(hi, nCol) < midItem ) )
				--hi;
		}

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( GetItemText(lo, nCol) != GetItemText(hi, nCol))
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount =
					((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
					LVIS_FOCUSED | LVIS_SELECTED |
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				
				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;
				
				GetItem( &lvitemlo );
				GetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(lo, i, GetItemText(hi, i));
				
				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(hi, i, rowText[i]);
				
				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}
			
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItems( nCol, bAscending , low, hi);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItems( nCol, bAscending , lo, high );
	
	return TRUE;
}

// SortTextItems	- Sort the list based on column text
// Returns		- Returns true for success
// nCol			- column that contains the text to be sorted
// bAscending		- indicate sort order
// low			- row to start scanning from - default row is 0
// high			- row to end scan. -1 indicates last row
BOOL CSortListCtrl::SortTextItemsOnLength( int nCol, BOOL bAscending, int low /*= 0*/, int high /*= -1*/ )
{
	if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;  

	if( high == -1 ) high = GetItemCount() - 1;

	int lo = low;
	int hi = high;
	
	if( hi <= lo ) return FALSE;
  
	int nMidItemLength = GetItemText( (lo+hi)/2, nCol ).GetLength();

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;

		// find the first element that is greater than or equal to
		// the partition element starting from the left Index.
		if( bAscending ) {
			while( ( lo < high ) && ( GetItemText(lo, nCol).GetLength() < nMidItemLength ) )
				++lo;
		}
		else {
			while( ( lo < high ) && ( GetItemText(lo, nCol).GetLength() > nMidItemLength ) )
				++lo;
		}

		// find an element that is smaller than or equal to
		// the partition element starting from the right Index.
		if( bAscending ) {
			while( ( hi > low ) && ( GetItemText(hi, nCol).GetLength() > nMidItemLength) )
				--hi;
		}

		else {
			while( ( hi > low ) && ( GetItemText(hi, nCol).GetLength() < nMidItemLength ) )
				--hi;
		}

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( GetItemText(lo, nCol).GetLength() != GetItemText(hi, nCol).GetLength())
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount =
					((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
					LVIS_FOCUSED | LVIS_SELECTED |
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				
				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;
				
				GetItem( &lvitemlo );
				GetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(lo, i, GetItemText(hi, i));
				
				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(hi, i, rowText[i]);
				
				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}
			
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItemsOnLength( nCol, bAscending , low, hi);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItemsOnLength( nCol, bAscending , lo, high );
	
	return TRUE;
}

BOOL CSortListCtrl::SortSpecialTextItems(int nCol, BOOL bAscending, int low /*= 0*/, int high /*= -1*/ )
{
  if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
		return FALSE;
  
	if( high == -1 ) high = GetItemCount() - 1;

	int lo = low;
	int hi = high;
	
	if( hi <= lo ) return FALSE;
  
  CString csMidText  = GetItemText( (lo+hi)/2, nCol );
	int nMidItemLength = csMidText.GetLength();
  
	// loop through the list until indices cross
	while( lo <= hi )
	{
		// find the first element that is greater than or equal to
		// the partition element starting from the left Index.
		if( bAscending ) {
			while( ( lo < high) && ( GetItemText(lo, nCol).GetLength() == nMidItemLength) && (GetItemText(lo, nCol) < csMidText))
				++lo;
		}
		else {
			while( ( lo < high) && ( GetItemText(lo, nCol).GetLength() == nMidItemLength) && (GetItemText(lo, nCol) > csMidText))
				++lo;
		}

		// find an element that is smaller than or equal to
		// the partition element starting from the right Index.
		if( bAscending ) {
			while( ( hi > low ) && ( GetItemText(hi, nCol).GetLength() == nMidItemLength) && (GetItemText(hi, nCol) > csMidText))
				--hi;
		}

		else {
			while( ( hi > low ) && ( GetItemText(hi, nCol).GetLength() == nMidItemLength) && (GetItemText(hi, nCol) < csMidText))
				--hi;
		}

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if (( GetItemText(lo, nCol).GetLength() == GetItemText(hi, nCol).GetLength()) && (GetItemText(lo, nCol) != GetItemText(hi, nCol)))
			{
        // rowText will hold all column text for one row
		    CStringArray rowText;
        
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount =
					((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
					LVIS_FOCUSED | LVIS_SELECTED |
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;
				
				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;
				
				GetItem( &lvitemlo );
				GetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(lo, i, GetItemText(hi, i));
				
				lvitemhi.iItem = lo;
				SetItem( &lvitemhi );
				
				for( i=0; i<nColCount; i++)
					SetItemText(hi, i, rowText[i]);
				
				lvitemlo.iItem = hi;
				SetItem( &lvitemlo );
			}
			
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortSpecialTextItems( nCol, bAscending , low, hi);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortSpecialTextItems( nCol, bAscending , lo, high );
	
	return TRUE;
}

void CSortListCtrl::SaveColumnWidths()
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	// You must set a unique name for every listctrl
	ASSERT( m_strSection.GetLength() );
	ASSERT( m_strControl.GetLength() );
	CString strValue;

	for( int nItem = 0; nItem < pHeader->GetItemCount(); nItem++ )
	{
		CString strTemp;
		strTemp.Format(_T("%d,"), GetColumnWidth(nItem) );
		strValue += strTemp;
	}

	AfxGetApp()->WriteProfileString( m_strSection, m_strControl, strValue );
}

void CSortListCtrl::LoadColumnWidths()
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	for( int nItem = 0; nItem < pHeader->GetItemCount(); nItem++ ) {
		SetColumnWidth(nItem);
	}
}

void CSortListCtrl::SetColumnWidth(int nCol)
{
	int iWidth = GetRegColumnWidth( nCol );
	if( iWidth < MINCOLWIDTH )
		AutoSizeColumn( nCol );
	else
		CListCtrl::SetColumnWidth( nCol, iWidth );
}

void CSortListCtrl::AutoSizeColumn(int iColumn)
{
	SetRedraw(TRUE);
	
	CListCtrl::SetColumnWidth(iColumn,LVSCW_AUTOSIZE);
	int wc1 = GetColumnWidth( iColumn );
	CListCtrl::SetColumnWidth(iColumn,LVSCW_AUTOSIZE_USEHEADER);
	int wc2 = GetColumnWidth( iColumn );
	int wc = max(MINCOLWIDTH,max( wc1,wc2 ));
	
	if( wc > MAXCOLWIDTH )
		wc = MAXCOLWIDTH;
	
	CListCtrl::SetColumnWidth( iColumn,wc );  
	SetRedraw(FALSE);
}

int CSortListCtrl::GetRegColumnWidth(int iColumn)
{
	ASSERT( m_strSection.GetLength() );
	ASSERT( m_strControl.GetLength() );
	
	CString strEntry( m_strControl );
	CString strValue, strSubString;

	strValue = AfxGetApp()->GetProfileString( m_strSection, strEntry, _T("") );
	AfxExtractSubString(strSubString, strValue, iColumn, _T(','));
	return _ttoi((TCHAR*)(LPCTSTR)strSubString);
}

void CSortListCtrl::OnDestroy() 
{
	if (m_bSaveColumnState)
		SaveColumnWidths();
	CListCtrl::OnDestroy();
}

BOOL CSortListCtrl::BuildColumns(int nCols, int * nWidth, int * nColString)
{
	//insert columns
	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp;
	for(i = 0; i < nCols; i++)
	{
		lvc.iSubItem = i;
		strTemp.LoadString(nColString[i]);
		lvc.pszText = strTemp.GetBuffer(0);
		strTemp.ReleaseBuffer(-1);
		lvc.cx = nWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		InsertColumn(i,&lvc);
	}

	if (m_bSaveColumnState)
		LoadColumnWidths();
	return TRUE;
}

void CSortListCtrl::SaveColumnState(CString strSection, CString strControl)
{
	m_strSection = strSection;
	m_strControl = strControl;
	m_bSaveColumnState = TRUE;
}

// ie: LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP
void CSortListCtrl::SetExtendedStyle(DWORD dwNewStyle)
{
	// Returns the current extended style ( a DWORD ).
	DWORD dwStyle = ::SendMessage (m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);

	// Add the full row select and grid line style to the existing extended styles.
	dwStyle |= dwNewStyle;

	// Sets the current extended style ( a DWORD ).
	::SendMessage (m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

int CSortListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
  
	return InitializeFlatHeader();
}

void CSortListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();
  ModifyStyle(NULL, LVS_SINGLESEL|LVS_REPORT, 0);
  SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	InitializeFlatHeader();
}

int CSortListCtrl::InitializeFlatHeader()
{
	if (!m_headerCtrl.SubclassWindow(GetDlgItem(0)->GetSafeHwnd()))
		return -1;

	m_headerCtrl.FlatHeader();
	SaveColumnState(_T("Settings"), _T("Control"));
	return 0;
}

BOOL CSortListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
  //ModifyStyleEx(NULL, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT, 0);
	return CListCtrl::PreCreateWindow(cs);

}

