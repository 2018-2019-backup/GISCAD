#if !defined(AFX_EDITLISTCTRL_H__21C7CB86_2B10_11D4_B01B_000001187638__INCLUDED_)
#define AFX_EDITLISTCTRL_H__21C7CB86_2B10_11D4_B01B_000001187638__INCLUDED_

#include "ListItemEditCtrl.h"
#include "SortListCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl window

class CEditListCtrl : public CSortListCtrl
{
// Construction
public:
	CEditListCtrl();
// Attributes
public:
  CUIntArray m_ciaEditColumns;
  int m_iCurCol, m_iCurRow;
  BOOL m_bEditControlVisible;
  BOOL m_bIsHV;
	BOOL m_bIsPoleDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListCtrl)
  protected: 

	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFirstColumnEditable(BOOL);
  void SetIsHV(BOOL bIsHV);
	virtual ~CEditListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnFloatEditChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bFirstColumnEdit;
	void EditSubLabel(int, int);
  CListItemEditCtrl m_edFloatEdit;
	int HitTestEx(CPoint &, int *) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLISTCTRL_H__21C7CB86_2B10_11D4_B01B_000001187638__INCLUDED_)
