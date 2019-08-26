// SortHeaderCtrl.cpp : implementation file
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortHeaderCtrl.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int GetBitmapWidth (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(LPTSTR)&bm); return bm.bmWidth;}
/////////////////////////////////////////////////////////////////////////////
int GetBitmapHeight (HBITMAP hBitmap)
{ BITMAP bm; GetObject(hBitmap,sizeof(BITMAP),(LPTSTR)&bm); return bm.bmHeight;}

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl

CSortHeaderCtrl::CSortHeaderCtrl()
{
	m_nSortCol	  = -1;
	m_bFlatHeader = FALSE;
	m_bBoldFont	  = FALSE;
	m_nOffset	    = 6;
	m_bLBtnDown	  = FALSE;
  m_pbmpHeader  = NULL;
  m_bDrawn = FALSE;
  m_iSpacing    = 6;
  m_bDoubleBuffer = TRUE;

  //CAcModuleResourceOverride usethis;
  //m_pbmpHeader  = new CBitmap;
  //m_pbmpHeader->LoadBitmap(IDB_HEADER_BMP);
}

CSortHeaderCtrl::~CSortHeaderCtrl()
{
  if (m_pbmpHeader)
  {
    m_pbmpHeader->DeleteObject();
    delete m_pbmpHeader;
  }
}

BEGIN_MESSAGE_MAP(CSortHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSortHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl message handlers

void CSortHeaderCtrl::FlatHeader(BOOL bBoldFont/*=TRUE*/)
{
	// Get the log font.
	NONCLIENTMETRICS ncm;
	memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);

	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));
	//ncm.lfMessageFont.lfWeight = bBoldFont?700:400;
	ncm.lfMessageFont.lfWeight = 400;

	m_HeaderFont.CreateFontIndirect(&ncm.lfMessageFont);
	SetFont(&m_HeaderFont);

	m_bBoldFont   = bBoldFont;
	m_bFlatHeader = TRUE;
}

int CSortHeaderCtrl::SetSortImage(int nCol, BOOL bAsc)
{
	int nPrevCol = m_nSortCol;
	
	m_nSortCol = nCol;
	m_bSortAsc = bAsc;
	
	// Change the item to owner drawn
	HD_ITEM hditem;
	
	hditem.mask = HDI_BITMAP | HDI_FORMAT;
	GetItem( nCol, &hditem );
	if (hditem.hbm == NULL)
	{
		hditem.fmt |= HDF_OWNERDRAW;
		SetItem( nCol, &hditem );

		// Invalidate header control so that it gets redrawn
		Invalidate();
	}
	
	return nPrevCol;
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void CSortHeaderCtrl::DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r)
{
  int DrawMode = 1;
  //	DrawMode: 0=Normal; 1=stretch; 2=tiled fill
	
	if(!hbmp) return;	//safe check

	int cx=r.right  - r.left;
	int cy=r.bottom - r.top;
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(dc);
	dcBmp.SelectObject(hbmp);
	
	int bx=GetBitmapWidth(hbmp);
	int by=GetBitmapHeight(hbmp);
	dc->StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCCOPY);

	DeleteDC(dcBmp);
}


void CSortHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  CDC dc;

	dc.Attach( lpDrawItemStruct->hDC );
	
	// Get the column rect
	CRect rcLabel( lpDrawItemStruct->rcItem );
	
	// Save DC
	int nSavedDC = dc.SaveDC();
	
	// Set clipping region to limit drawing within column
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcLabel );
	dc.SelectObject( &rgn );
	rgn.DeleteObject();
	
  CFont* pFont = dc.SelectObject(GetFont());

	// Draw the background
  if (m_pbmpHeader)
  {
    //DrawBitmap(pDC, *m_pbmpHeader, rectClient);
    //DrawBitmap(pDC, *m_pbmpHeader, )
  }
	else dc.FillRect(rcLabel, &CBrush(::GetSysColor(COLOR_3DFACE)));
	dc.SetBkMode(TRANSPARENT);

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;
	
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;
	
	GetItem( lpDrawItemStruct->itemID, &hditem );
	
	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
		| DT_VCENTER | DT_END_ELLIPSIS | DT_CENTER;
	
	
	// Adjust the rect if the mouse button is pressed on it
	if( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		rcLabel.left++;
		rcLabel.top += 2;
		rcLabel.right++;
	}
	
	
	rcLabel.left += m_nOffset;
	rcLabel.right -= m_nOffset;

	// Draw column label
	if( rcLabel.left < rcLabel.right )
	{
		dc.DrawText(buf,-1,rcLabel, uFormat);
	}
  
  // Adjust the rect further if Sort arrow is to be displayed
	if( lpDrawItemStruct->itemID == (UINT)m_nSortCol )
	{
		rcLabel.right -= 3 * m_nOffset;
	}
	
  
	// Draw the Sort arrow
	if( lpDrawItemStruct->itemID == (UINT)m_nSortCol )
	{
		CRect rcIcon( lpDrawItemStruct->rcItem );
		
		// Set up pens to use for drawing the triangle
		CPen penLite(PS_SOLID, 1, ::GetSysColor(COLOR_3DHILIGHT));
		CPen penShad(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen = dc.SelectObject( &penLite );
		
		if( m_bSortAsc )
		{
			// Draw triangle pointing upwards
			dc.MoveTo( rcIcon.right - 2*m_nOffset, m_nOffset-1);
			dc.LineTo( rcIcon.right - 3*m_nOffset/2, rcIcon.bottom - m_nOffset );
			dc.LineTo( rcIcon.right - 5*m_nOffset/2-2, rcIcon.bottom - m_nOffset );
			dc.MoveTo( rcIcon.right - 5*m_nOffset/2-1, rcIcon.bottom - m_nOffset-1 );
			
			dc.SelectObject( &penShad );
			dc.LineTo( rcIcon.right - 2*m_nOffset, m_nOffset-2);
		}
		else
		{
			// Draw triangle pointing downwards
			dc.MoveTo( rcIcon.right - 3*m_nOffset/2, m_nOffset-1);
			dc.LineTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset + 1 );
			dc.MoveTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset );
			
			dc.SelectObject( &penShad );
			dc.LineTo( rcIcon.right - 5*m_nOffset/2-1, m_nOffset -1 );
			dc.LineTo( rcIcon.right - 3*m_nOffset/2, m_nOffset -1);
		}
		
		// Restore the pen
		dc.SelectObject( pOldPen );
	}
  
  dc.SelectObject(pFont);
	// Restore dc
	dc.RestoreDC( nSavedDC );
	
	// Detach the dc before returning
	dc.Detach();
}


///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CSortHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	
	//rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
	//rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
	//	iWidth + m_iSpacing : 0;
	DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
void CSortHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	//pDC->SetTextColor(RGB(0,0,255));

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
		| DT_VCENTER | DT_END_ELLIPSIS | DT_CENTER);
    //AfxMessageBox(lphdi->pszText);
	}
}

void CSortHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

    if (m_bDoubleBuffer)
    {
        CMemDC MemDC(&dc);
        DrawCtrl(&MemDC);
    }
    else
        DrawCtrl(&dc);
}

void CSortHeaderCtrl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CHeaderCtrl::OnWindowPosChanging(lpwndpos);
	Invalidate();
}

void CSortHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);
  
  pDC->SetBkMode(TRANSPARENT);
  if (m_pbmpHeader)
  {
    DrawBitmap(pDC, *m_pbmpHeader, rectClient);
  }
  else pDC->FillSolidRect(rectClip, ::GetSysColor(COLOR_3DFACE));

	int iItems = GetItemCount();
	if (iItems <= 0) return; 
  CPen penShadow(PS_SOLID, 1, RGB(0, 0 ,0) );
	/*
  CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	*/
	CPen* pPen = pDC->GetCurrentPen();
  
	CFont* pFont = pDC->SelectObject(GetFont());

  /*
	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);
  */
	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[255];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;
        
				DrawItem(&disItem);
			}
			else
			{
        
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_iSpacing, 0);

				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
			}

			if (i < iItems-1)
			{
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rectItem.right-1, rectItem.top+2);
				pDC->LineTo(rectItem.right-1, rectItem.bottom-2);

				/*
        pDC->SelectObject(&penHighLight);
				pDC->MoveTo(rectItem.right, rectItem.top+2);
				pDC->LineTo(rectItem.right, rectItem.bottom-2);
        */
			}
		}

		//iWidth += hditem.cxy;
	}
  pDC->SelectObject(&penShadow);
  pDC->MoveTo(rectItem.right-1, rectItem.top+2);
  pDC->LineTo(rectItem.right-1, rectItem.bottom-2);
  
  /*
	if (iWidth > 0)
	{
		rectClient.right = rectClient.left + iWidth;
		pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
	}
  */
	pDC->SelectObject(pFont);
	//pDC->SelectObject(pPen);

	//penHighLight.DeleteObject();
	//penShadow.DeleteObject();
}
