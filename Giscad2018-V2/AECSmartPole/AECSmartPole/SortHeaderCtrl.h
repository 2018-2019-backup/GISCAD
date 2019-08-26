// SortHeaderCtrl.h : header file
//  
/////////////////////////////////////////////////////////////////////////////

#ifndef __CSORTHEADERCTRL_H__
#define __CSORTHEADERCTRL_H__

#include "MemDc.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl window

class CSortHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSortHeaderCtrl();
  void DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r);
  void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
  void DrawCtrl(CDC* pDC);
  void DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi);
  CBitmap *m_pbmpHeader;
  BOOL     m_bDrawn;
  BOOL     m_bDoubleBuffer;
  int      m_iSpacing;
  
// Attributes
public:
	BOOL  m_bFlatHeader;
	BOOL  m_bBoldFont;
	CFont m_HeaderFont;
	int	  m_nOffset;
	BOOL  m_bLBtnDown;

// Operations
public:
	void FlatHeader(BOOL bBoldFont = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortHeaderCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawFlatBorder();
	int SetSortImage( int nCol, BOOL bAsc );
	virtual ~CSortHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	int  m_nSortCol;
	BOOL m_bSortAsc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CSORTHEADERCTRL_H__
