#if !defined(AFX_ListItemEditCtrl_H__6923D770_6649_11D5_907A_000021227B4F__INCLUDED_)
#define AFX_ListItemEditCtrl_H__6923D770_6649_11D5_907A_000021227B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListItemEditCtrl.h : header file
//
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CListItemEditCtrl window

class CListItemEditCtrl : public CEdit
{
// Construction
public:
	CListItemEditCtrl();
  int m_iCurCol, m_iCurRow;
  BOOL *m_pbEditControlVisible;
  void UpdateParentListCtrlItem();
  BOOL m_bTextEdit;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListItemEditCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CString csTemp;
	virtual ~CListItemEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListItemEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ListItemEditCtrl_H__6923D770_6649_11D5_907A_000021227B4F__INCLUDED_)
