// SortListCtrl.h : header file
/////////////////////////////////////////////////////////////////////////////

#ifndef __CSORTLISTCTRL_H__
#define __CSORTLISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortHeaderCtrl.h"

#define MINCOLWIDTH	       70
#define MAXCOLWIDTH	       70

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

// Attributes
public:
  CUIntArray  m_uiaNumberSortColumns;
  CUIntArray  m_uiaSpecialSortColumns;
protected:
	int					m_nSortedCol; 
	BOOL				m_bSortAscending; 
	BOOL				m_bSaveColumnState;
	CString				m_strSection;
	CString				m_strControl;
	CSortHeaderCtrl	m_headerCtrl;
  
  BOOL        m_bAllowSort;
// Operations
public:
  void AllowSort(BOOL bAllow);
	int InitializeFlatHeader();
	BOOL SortTextItems        ( int nCol, BOOL bAscending, int low = 0, int high = -1 );
  BOOL SortTextItemsOnNumber( int nCol, BOOL bAscending, int low = 0, int high = -1 );
  BOOL SortTextItemsOnLength( int nCol, BOOL bAscending, int low = 0, int high = -1 );
  BOOL SortSpecialTextItems ( int nCol, BOOL bAscending, int low = 0, int high = -1 );
	void SetExtendedStyle(DWORD dwNewStyle);
	void SaveColumnState(CString strSection, CString strControl);
	BOOL BuildColumns(int nCols, int* nWidth, int* nColString);
	int GetRegColumnWidth( int iColumn );
	void AutoSizeColumn( int iColumn );
	void SetColumnWidth( int nCol );
	void LoadColumnWidths();
	void SaveColumnWidths();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CSORTLISTCTRL_H__
