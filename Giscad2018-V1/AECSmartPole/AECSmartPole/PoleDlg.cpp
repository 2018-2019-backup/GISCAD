////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : PoleDlg.cpp
// Created          : 20th April 2007
// Description      : Pole Information dialog implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "PoleDlg.h"
#include "TableColumnsDlg.h"

//////////////////////////////////////////////////////////////////////////
// Externally defined functions
//////////////////////////////////////////////////////////////////////////
void ModifyPoleTable(AcDbObjectId objPole, CString csExistingPoleNumber, CString csNewPoleNumber);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Pole Information dialog
IMPLEMENT_DYNAMIC(CPoleDlg, CDialog)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::CPoleDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPoleDlg::CPoleDlg(CWnd* pParent /*=NULL*/)	: CDialog(CPoleDlg::IDD, pParent)
{
  m_bFirstRun = TRUE;
  m_bEditing = FALSE;
  m_bStayReq = FALSE;
  m_bDisplayPoleNumber = TRUE;
  m_iActiveEditListCtrlID = 0;
	m_csExistingPoleSymbol = _T("");
	m_lcHV.m_bIsPoleDlg = true;
	m_lcLV.m_bIsPoleDlg = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::~CPoleDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPoleDlg::~CPoleDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  // Group boxes
  DDX_Control(pDX, IDC_STATIC_POLE_INFO,				m_stInfoBox);
  DDX_Control(pDX, IDC_STATIC_POLE_CONS,				m_stConsBox);
  DDX_Control(pDX, IDC_STATIC_POLE_HV,					m_stHVBox);
  DDX_Control(pDX, IDC_STATIC_POLE_LV,					m_stLVBox);
  DDX_Control(pDX, IDC_STATIC_POLE_LANTERNS,		m_stLanternBox);

  // Controls
  DDX_Control(pDX, IDC_POLE_STATUS,             m_cbPoleStatus);
  DDX_Control(pDX, IDC_POLE_STAY_TYPE,          m_cbPoleStay);
  DDX_Control(pDX, IDC_POLE_SIZE,               m_cbPoleSize);
  DDX_Control(pDX, IDC_POLE_FOOTING,            m_cbPoleFooting);
  DDX_Control(pDX, IDC_POLE_STAY_REQ,           m_btnStay);
  DDX_Control(pDX, IDC_POLE_NUMBER,             m_edPoleNumber);
  DDX_Control(pDX, IDC_POLE_FIELD_NUMBER_NEW,   m_edFieldNumberNew);
	DDX_Control(pDX, IDC_POLE_FIELD_NUMBER_EXIST, m_edFieldNumberExist);
  DDX_Control(pDX, IDC_POLE_DIA,								m_edPoleDia);
  DDX_Control(pDX, IDC_POLE_HOLE_DIA,						m_edHoleDia);
  DDX_Control(pDX, IDC_POLE_HOLE_DEPTH,					m_edHoleDepth);
  DDX_Control(pDX, IDC_POLE_DESCRIPTION,				m_edPoleDesc);
  DDX_Control(pDX, IDC_POLE_SPAN_LENGTH,				m_edSpanLength);
  DDX_Control(pDX, IDC_POLE_LINE_DEV_DEG,				m_edLineDevDeg);
  DDX_Control(pDX, IDC_POLE_HV_LIST,						m_lcHV);
  DDX_Control(pDX, IDC_POLE_LV_LIST,						m_lcLV);
  DDX_Control(pDX, IDC_POLE_LANTERN_BRACKET,		m_cbLanternBracket);
  DDX_Control(pDX, IDC_POLE_LANTERN_STATUS,			m_cbLanternStatus);
  DDX_Control(pDX, IDC_POLE_DISPLAY,						m_btnDisplay);
  DDX_Control(pDX, IDC_POLE_HV_ABC,							m_btnHVABC);
  DDX_Control(pDX, IDC_POLE_HV_BARE,						m_btnHVBare);
  DDX_Control(pDX, IDC_POLE_HV_OTHER,						m_btnHVOther);
  DDX_Control(pDX, IDC_POLE_LV_ABC,							m_btnLVABC);
  DDX_Control(pDX, IDC_POLE_LV_BARE,						m_btnLVBare);
  DDX_Control(pDX, IDC_POLE_LV_OTHER,						m_btnLVOther);
  DDX_Control(pDX, IDOK,												m_btnAction);

  // Variables
  DDX_CBString(pDX, IDC_POLE_STATUS,             m_csPoleStatus);
  DDX_Check(pDX,    IDC_POLE_STAY_REQ,           m_bStayReq);
  DDX_CBString(pDX, IDC_POLE_STAY_TYPE,          m_csPoleStay);
  DDX_Text(pDX,     IDC_POLE_NUMBER,             m_csPoleNumber);
  DDX_Text(pDX,     IDC_POLE_FIELD_NUMBER_NEW,   m_csFieldNumberNew);
	DDX_Text(pDX,     IDC_POLE_FIELD_NUMBER_EXIST, m_csFieldNumberExist);
  DDX_CBString(pDX, IDC_POLE_SIZE,							 m_csPoleSize);
  DDX_CBString(pDX, IDC_POLE_FOOTING,            m_csPoleFooting);
  DDX_Text(pDX,     IDC_POLE_DIA,                m_csPoleDia);
  DDX_Text(pDX,     IDC_POLE_HOLE_DIA,           m_csHoleDia);
  DDX_Text(pDX,     IDC_POLE_HOLE_DEPTH,         m_csHoleDepth);
  DDX_Check(pDX,    IDC_POLE_DISPLAY,            m_bDisplayPoleNumber);
  DDX_Text(pDX,     IDC_POLE_SPAN_LENGTH,        m_csSpanLength);
  DDX_Text(pDX,     IDC_POLE_LINE_DEV_DEG,       m_csLineDevDeg);
  DDX_CBString(pDX, IDC_POLE_LANTERN_BRACKET,    m_csLanternBracket);
  DDX_CBString(pDX, IDC_POLE_LANTERN_STATUS,     m_csLanternStatus);
  DDX_Check(pDX,    IDC_POLE_HV_ABC,             m_bHVABC);
  DDX_Check(pDX,    IDC_POLE_HV_BARE,            m_bHVBare);
  DDX_Check(pDX,    IDC_POLE_HV_OTHER,           m_bHVOther);
  DDX_Text(pDX,     IDC_POLE_HV_SEL,             m_csPoleHV);
  DDX_Check(pDX,    IDC_POLE_LV_ABC,             m_bLVABC);
  DDX_Check(pDX,    IDC_POLE_LV_BARE,            m_bLVBare);
  DDX_Check(pDX,    IDC_POLE_LV_OTHER,           m_bLVOther);
  DDX_Text(pDX,     IDC_POLE_LV_SEL,             m_csPoleLV);
  DDX_Text(pDX,     IDC_POLE_DESCRIPTION,        m_csPoleDesc);
	DDV_MaxChars(pDX, m_csPoleDesc, 40);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPoleDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_POLE_STATUS, OnSelectPoleStatus)
  ON_CBN_SELCHANGE(IDC_POLE_SIZE,   OnSelectPoleSize)
  ON_BN_CLICKED(IDC_POLE_STAY_REQ,  OnClickedStayRequired)
  ON_BN_CLICKED(IDC_POLE_HV_ABC,    OnSelectHVABC)
  ON_BN_CLICKED(IDC_POLE_HV_BARE,   OnSelectHVBare)
  ON_BN_CLICKED(IDC_POLE_HV_OTHER,  OnSelectHVOther)
  ON_BN_CLICKED(IDC_POLE_LV_ABC,    OnSelectLVABC)
  ON_BN_CLICKED(IDC_POLE_LV_BARE,   OnSelectLVBare)
  ON_BN_CLICKED(IDC_POLE_LV_OTHER,  OnSelectLVOther)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_POLE_HV_LIST, OnCheckHVList)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_POLE_LV_LIST, OnCheckLVList)
  ON_BN_CLICKED(IDC_POLE_CLEAR_ALL, OnClearAll)
  ON_NOTIFY(LVN_ENDLABELEDIT, IDC_POLE_HV_LIST, OnLvnEndlabeleditPoleHvList)
  ON_NOTIFY(LVN_ENDLABELEDIT, IDC_POLE_LV_LIST, OnLvnEndlabeleditPoleLvList)
	ON_BN_CLICKED(IDHELP, OnHelp)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPoleDlg::OnInitDialog()
{
  // Call the parent class implementation
  CDialog::OnInitDialog();

  // Create a new bold font
  CFont *pBoldFont = new CFont;
  pBoldFont->CreatePointFont(85, _T("Verdana Bold"));

  // Assign this font to all the static members
  m_stInfoBox.SetFont(pBoldFont);
  m_stConsBox.SetFont(pBoldFont);
  m_stHVBox.SetFont(pBoldFont);
  m_stLVBox.SetFont(pBoldFont);
  m_stLanternBox.SetFont(pBoldFont);

  // Initialize the HV list
  m_lcHV.InsertColumn(0, _T("Value"), LVCFMT_LEFT,   95);
  m_lcHV.InsertColumn(1, _T("Qty"),   LVCFMT_CENTER, 50);
  m_lcHV.InsertColumn(2, _T("Cat"),   LVCFMT_LEFT,    0);
  m_lcHV.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
  m_lcHV.m_ciaEditColumns.Add(1); // Allow editing for the qty column only
  m_lcHV.SetIsHV(TRUE);

  // Initialize the LV list
  m_lcLV.InsertColumn(0, _T("Value"), LVCFMT_LEFT,   95);
  m_lcLV.InsertColumn(1, _T("Qty"),   LVCFMT_CENTER, 50);
  m_lcLV.InsertColumn(2, _T("Cat"),   LVCFMT_LEFT,    0);
  m_lcLV.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
  m_lcLV.m_ciaEditColumns.Add(1); // Allow editing for the qty column only
  m_lcLV.SetIsHV(FALSE);

  // Add the pole types to the combo
  for (int iCtr = 0; iCtr < g_csaPoleTypes.GetSize(); iCtr++) m_cbPoleStatus.AddString(g_csaPoleTypes.GetAt(iCtr));

  // Add the pole stays to the combo
  for (int iCtr = 0; iCtr < g_csaPoleStays.GetSize(); iCtr++) m_cbPoleStay.AddString(g_csaPoleStays.GetAt(iCtr));

  // Add the pole sizes to the combo
  for (int iCtr = 0; iCtr < g_csaPoleSizes.GetSize(); iCtr++) m_cbPoleSize.AddString(g_csaPoleSizes.GetAt(iCtr));

  // Add the pole footings to the combo
  for (int iCtr = 0; iCtr < g_csaPoleFootings.GetSize(); iCtr++) m_cbPoleFooting.AddString(g_csaPoleFootings.GetAt(iCtr));

	// Add the lantern status to the combo
	for (int iCtr = 0; iCtr < m_csaLanternTypes.GetSize(); iCtr++) m_cbLanternStatus.AddString(m_csaLanternTypes.GetAt(iCtr));
	
  // Add the lantern brackets to the combo
  for (int iCtr = 0; iCtr < g_csaPoleBrackets.GetSize(); iCtr++) m_cbLanternBracket.AddString(g_csaPoleBrackets.GetAt(iCtr));

	// Get the maximum of existing pole numbers
	if (!m_bEditing) 
	{
		m_csPoleNumber.Format(_T("%d"), getMaxInArray(m_uiaPoleNumbers) + 1); 
		m_edPoleNumber.SetWindowText(m_csPoleNumber); 
	} 
	else 
	{
		// Disable the pole status as they will never change this
		m_cbPoleStatus.EnableWindow(FALSE);

		// This is to check if the user has changed the pole number during this session (Nothing to do with Asset Number and PF Number)
		m_csExistingPoleNumber = m_csPoleNumber;
	}
			
  // If this is not the first run
  if (m_bFirstRun == FALSE)
  {
		//////////////////////////////////////////////////////////////////////////
    // Show the values for all controls
		//////////////////////////////////////////////////////////////////////////
    int iExistingStatus    = m_cbPoleStatus.SelectString(-1, m_csPoleStatus);
		m_csExistingPoleSymbol = g_csaPoleBlocks.GetAt(iExistingStatus);
		OnSelectPoleStatus();

    m_edPoleNumber.SetWindowText(m_csPoleNumber);
    
		m_edFieldNumberNew.SetWindowText(m_csFieldNumberNew);
		m_edFieldNumberExist.SetWindowText(m_csFieldNumberExist);

    m_btnStay.SetCheck(m_bStayReq); OnClickedStayRequired();
    if (m_bStayReq) m_cbPoleStay.SelectString(-1, m_csPoleStay);
    m_cbPoleSize.SelectString(-1, m_csPoleSize); OnSelectPoleSize();
    m_edPoleDia.SetWindowText(m_csPoleDia);
    m_edHoleDia.SetWindowText(m_csHoleDia);
    m_edHoleDepth.SetWindowText(m_csHoleDepth);
    m_cbPoleFooting.SelectString(-1, m_csPoleFooting);
    m_btnDisplay.SetCheck(m_bDisplayPoleNumber);
    m_edSpanLength.SetWindowText(m_csSpanLength);
    m_edLineDevDeg.SetWindowText(m_csLineDevDeg);
    m_cbLanternBracket.SelectString(-1, m_csLanternBracket);
    m_cbLanternStatus.SelectString(-1, m_csLanternStatus);

    m_btnHVABC.SetCheck(m_bHVABC);     OnSelectHVABC();
    m_btnHVBare.SetCheck(m_bHVBare);   OnSelectHVBare();
    m_btnHVOther.SetCheck(m_bHVOther); OnSelectHVOther();
    
    m_btnLVABC.SetCheck(m_bLVABC);     OnSelectLVABC();
    m_btnLVBare.SetCheck(m_bLVBare);   OnSelectLVBare();
    m_btnLVOther.SetCheck(m_bLVOther); OnSelectLVOther();

    // If we are editing
    if (m_bEditing == TRUE) 
    {
      // Change the window title
      SetWindowText(_T("Modify pole"));

      // Change the label of the "Insert" button
      m_btnAction.SetWindowText(_T("Modify"));
    }
  }

  // Everything is OK
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectPoleStatus
// Description  : Called when the user selects an entry in the "Pole status" combo box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectPoleStatus()
{
  // Get the selected pole status
  int iStatus = m_cbPoleStatus.GetCurSel();
  if (iStatus == CB_ERR) return;

  //////////////////////////////////////////////////////////////////////////
  // As per Change Request #94: Enable disable controls
  //////////////////////////////////////////////////////////////////////////
  m_cbPoleStatus.GetLBText(iStatus, m_csPoleStatus);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mail Request: September 11 th 2009
	// If the user inserts a Replace pole, the user is able to enter the number of the existing pole being replaced. 
	// This is correct. However if the user enters the wrong number and tries to correct it using the modify pole command the field to edit the existing pole number is disabled.
	// This field MUST BE enabled otherwise the user has to delete pole and start again.
	// if (!m_csPoleStatus.CompareNoCase("New") || !m_csPoleStatus.CompareNoCase("Replace"))
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (!m_csPoleStatus.CompareNoCase(_T("New")))
	  m_edFieldNumberExist.EnableWindow(FALSE);
  else
	  m_edFieldNumberExist.EnableWindow(TRUE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Darren mail: When the form is first displayed this field is enabled, however as soon as the user selects a pole status of replace the field is 
	// disabled.  At the moment the work around is to enter the pole number first, however the fix is for the field to remain enabled.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (!m_bEditing) { if (!m_csPoleStatus.CompareNoCase(_T("Replace"))) m_edFieldNumberExist.EnableWindow(TRUE); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectPoleSize
// Description  : Called when the user selects an entry in the "Pole type" combo box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectPoleSize()
{
  // Clear the dependencies
  m_edPoleDia.SetWindowText(_T(""));
  m_edHoleDia.SetWindowText(_T(""));
  m_edHoleDepth.SetWindowText(_T(""));

  // Get the selected pole size
  int iType = m_cbPoleSize.GetCurSel();
  if (iType == CB_ERR) return;

  // Set the dependencies
  CString csPoleDia; csPoleDia.Format(_T("%s"), suppressZero(_tstof(g_csaPoleDiameters.GetAt(iType)) / 1000.0));
  m_edPoleDia.SetWindowText(csPoleDia);
  m_edHoleDia.SetWindowText(suppressZero(_tstof(g_csaPoleDiameters.GetAt(iType))));
  m_edHoleDepth.SetWindowText(suppressZero(_tstof(g_csaPoleDepths.GetAt(iType))));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnClickedStayRequired
// Description  : Called when the user checks the "Pole stay" check box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnClickedStayRequired()
{
  // Get the check state
  m_bStayReq = m_btnStay.GetCheck();

  // If it is required
  if (m_bStayReq == TRUE) m_cbPoleStay.EnableWindow(TRUE); 
  // Otherwise, reset and disable it
  else { m_cbPoleStay.SetCurSel(-1); m_cbPoleStay.EnableWindow(FALSE); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::AddRemoveListItems
// Description  : General function to serve six different check boxes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::AddRemoveListItems(CString csType, CListCtrl& lcList, CButton& btnCheck, CString csCheck, CStringArray& csaCat, CStringArray& csaText)
{
  // Get the status
  BOOL bStatus = btnCheck.GetCheck();

	// Get the previously specified array for the specified list control
	CStringArray csaParams;
	if (csType == _T("HV")) csaParams.Copy(m_csaHVBare); else csaParams.Copy(m_csaLVBare);
	
  // If it is checked
	CString csParam, csValue;
	BOOL bMatched;
  if (bStatus == TRUE)
  {
    // Add the items to the list
    for (int iCtr = 0, iRow = lcList.GetItemCount(); iCtr < csaCat.GetSize(); iCtr++)
    {
      if (csaCat.GetAt(iCtr) == csCheck)
      {
				lcList.InsertItem(iRow, csaText.GetAt(iCtr));
				lcList.SetItemText(iRow, 2, csCheck);

				// Check if this item was checked during the previous instance of the dialog
				bMatched = TRUE;
				for (int iCnt = 0; iCnt < csaParams.GetSize(); iCnt++)
				{
					csParam.Format(_T("%s-%s"), csaText.GetAt(iCtr), csCheck);
					GetParameterValue(csaParams, csParam, csValue, 0);
					if (!csValue.IsEmpty()) 
					{
						lcList.SetItemText(iRow, 1, csValue);
						lcList.SetCheck(iRow, TRUE);
					}
				}
        
        iRow++;
      }
    }
  }
  // If it is unchecked
  else
  {
    // Remove the items from the list
    for (int iRow = lcList.GetItemCount() - 1; iRow > -1; iRow--)
    {
      if (lcList.GetItemText(iRow, 2) == csCheck)
      {
        lcList.DeleteItem(iRow);
      }
    }
  }
  //MessageBox( _T("1"), _T("test"), 0);
  // Call the selection updater
  UpdateSelectedValues(lcList, (lcList == m_lcHV ? IDC_POLE_HV_SEL : IDC_POLE_LV_SEL));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectHVABC
// Description  : Called when the user checks the "ABC" button in the HV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectHVABC()
{
  // Add or remove "ABC" items from the HV list
  AddRemoveListItems(_T("HV"), m_lcHV, m_btnHVABC, _T("ABC"), g_csaPoleHVCat, g_csaPoleHVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectHVBare
// Description  : Called when the user checks the "Bare" button in the HV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectHVBare()
{
  // Add or remove "Bare" items from the HV list
  AddRemoveListItems(_T("HV"), m_lcHV, m_btnHVBare, _T("Bare"), g_csaPoleHVCat, g_csaPoleHVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectHVOther
// Description  : Called when the user checks the "Other" button in the HV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectHVOther()
{
  // Add or remove "Other" items from the HV list
  AddRemoveListItems(_T("HV"), m_lcHV, m_btnHVOther, _T("CCT"), g_csaPoleHVCat, g_csaPoleHVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectLVABC
// Description  : Called when the user checks the "ABC" button in the LV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectLVABC()
{
  // Add or remove "ABC" items from the LV list
  AddRemoveListItems(_T("LV"), m_lcLV, m_btnLVABC, _T("ABC"), g_csaPoleLVCat, g_csaPoleLVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectLVBare
// Description  : Called when the user checks the "Bare" button in the LV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectLVBare()
{
  // Add or remove "Bare" items from the LV list
  AddRemoveListItems(_T("LV"), m_lcLV, m_btnLVBare, _T("Bare"), g_csaPoleLVCat, g_csaPoleLVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnSelectLVOther
// Description  : Called when the user checks the "Other" button in the LV group box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnSelectLVOther()
{
  // Add or remove "Other" items from the LV list
  AddRemoveListItems(_T("LV"), m_lcLV, m_btnLVOther, _T("Other"), g_csaPoleLVCat, g_csaPoleLVText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::UpdateSelectedValues
// Description  : Called to update the values shown below the lists
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::UpdateSelectedValues(CListCtrl& lcList, UINT nID)
{
	// Get the text formatted till now in the edit box
	CString csSel; GetDlgItemText(nID, csSel);

	// Get the texts between the "+" in the string
	CStringArray csaResults; csaResults.RemoveAll(); SplitString(csSel, _T(" + "), csaResults);

	//////////////////////////////////////////////////////////////////////////
	// Modification: UAT 114
	// New code:
	// Initialize the flag array (This array indicates whether, the corresponding element in the Results array has to be included or not
	CUIntArray uiaFlags;
	for (int iFlag = 0; iFlag < csaResults.GetSize(); iFlag++) { uiaFlags.Add(0); }
	//////////////////////////////////////////////////////////////////////////

	// If the edit box has not "+", then the results array will return no items in it, so add the only item to the
	// result array.
	if (!csSel.IsEmpty() && (csSel.Find(_T("+")) == -1)) { csaResults.Add(csSel); uiaFlags.Add(1); }
			
	// For each item in the array check if sufficient numbers of them are present in the array, else add/remove them
	// from the array.
	int iQty;
	int iMatchedAt;
	for (int iCtr = 0; iCtr < lcList.GetItemCount(); iCtr++)
	{
		// Check if the item is present in the array
		iMatchedAt = -1;
		iQty = _ttoi(lcList.GetItemText(iCtr, 1));
		for (int iCnt = 0; iCnt < csaResults.GetSize(); iCnt++)
		{
			if (!csaResults.GetAt(iCnt).CompareNoCase(lcList.GetItemText(iCtr, 0)))
			{
				iQty--; // For each hit, let us reduce the quantity

				//////////////////////////////////////////////////////////////////////////
				// Modification: UAT 114
				// New code:
				// It was a hit so must be included
				uiaFlags.SetAt(iCnt, 1);
				//////////////////////////////////////////////////////////////////////////

				if (!lcList.GetCheck(iCtr) || (iQty < 0))
				{
					// But if this item is not required then...
					csaResults.SetAt(iCnt, _T("#DEL#")); 

					//////////////////////////////////////////////////////////////////////////
					// Modification: UAT 114
					// New code:
					uiaFlags.SetAt(iCnt, 0);
					//////////////////////////////////////////////////////////////////////////
				}

				if (iMatchedAt == -1) iMatchedAt = iCnt;
			}
		}

		// Add the remaining quantity
		if (lcList.GetCheck(iCtr) && (iQty > 0))
		{
			if (iMatchedAt == -1) { iMatchedAt = csaResults.GetSize(); }
			for (int i = 0; i < iQty; i++) 
			{
				//////////////////////////////////////////////////////////////////////////
				// Modification: UAT 114
				// Old: csaResults.InsertAt(iMatchedAt++, lcList.GetItemText(iCtr, 0));
				// New Code
				csaResults.InsertAt(iMatchedAt, lcList.GetItemText(iCtr, 0));
				uiaFlags.InsertAt(iMatchedAt++, 1);
				//////////////////////////////////////////////////////////////////////////
			}
		}
	}

	// Format the items in the array into one string 
	csSel.Empty();
	for (int iCtr = 0; iCtr < uiaFlags.GetSize(); iCtr++) 
	{
		if (!uiaFlags.GetAt(iCtr)) continue;
		if (csSel.IsEmpty()) { csSel = csaResults.GetAt(iCtr); continue; }
		csSel += (_T(" + ") + csaResults.GetAt(iCtr));
	}
	
  SetDlgItemText(nID, csSel);//RC
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnCheckHVList
// Description  : Called when the user checks an item in the HV list
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnCheckHVList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  *pResult = 0;

  // No change
  if (pNMLV->uOldState == 0 && pNMLV->uNewState == 0) return;

  // Old check box state
  BOOL bPrevState = (BOOL)(((pNMLV->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1);  
  if (bPrevState < 0) bPrevState = 0;  // On startup there's no previous state, so assign as false (unchecked)

  // New check box state
  BOOL bChecked = (BOOL)(((pNMLV->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1);   
  if (bChecked < 0) bChecked = 0; // On non-check box notifications, assume false

  // No change in check box
  if (bPrevState == bChecked) return;

  // Call the selection updater
	if (bChecked && m_lcHV.GetItemText(pNMLV->iItem, 1).IsEmpty()) 
		m_lcHV.SetItemText(pNMLV->iItem, 1, _T("1"));
	else if (!bChecked)
		m_lcHV.SetItemText(pNMLV->iItem, 1, _T(""));
  //MessageBox( _T("2"), _T("test"), 0);
  UpdateSelectedValues(m_lcHV, IDC_POLE_HV_SEL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnLvnEndlabeleditPoleHvList
// Description  : Called from edit list control after the quantity of an item is modified in the list.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnLvnEndlabeleditPoleHvList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	// Call the selection updater
	//MessageBox( _T("3"), _T("test"), 0);
	UpdateSelectedValues(m_lcHV, IDC_POLE_HV_SEL);

	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnLvnEndlabeleditPoleLvList
// Description  : Called from edit list control after the quantity of an item is modified in the list.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnLvnEndlabeleditPoleLvList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	
	// Call the selection updater
	//MessageBox( _T("4"), _T("test"), 0);
	UpdateSelectedValues(m_lcLV, IDC_POLE_LV_SEL);

	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnCheckLVList
// Description  : Called when the user checks an item in the LV list
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnCheckLVList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  *pResult = 0;

  // No change
  if (pNMLV->uOldState == 0 && pNMLV->uNewState == 0) return;

  // Old check box state
  BOOL bPrevState = (BOOL)(((pNMLV->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1);  
  if (bPrevState < 0) bPrevState = 0;  // On startup there's no previous state, so assign as false (unchecked)

  // New check box state
  BOOL bChecked = (BOOL)(((pNMLV->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1);   
  if (bChecked < 0) bChecked = 0; // On non-check box notifications, assume false

  // No change in check box
  if (bPrevState == bChecked) return;

  // Call the selection updater
	if (bChecked && m_lcLV.GetItemText(pNMLV->iItem, 1).IsEmpty()) 
		m_lcLV.SetItemText(pNMLV->iItem, 1, _T("1"));
	else if (!bChecked)
		m_lcLV.SetItemText(pNMLV->iItem, 1, _T(""));

	//MessageBox( _T("5"), _T("test"), 0);
  UpdateSelectedValues(m_lcLV, IDC_POLE_LV_SEL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnClearAll()
// Description  : Called when the user clicks on the "Clear All" button. All entries made to controls wiil be undone.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnClearAll()
{
	// Reset all values in the controls
	m_csPoleStatus = _T(""); m_cbPoleStatus.SetCurSel(-1); // Except this
	
	m_csFieldNumberNew   = _T("");    m_edFieldNumberNew.SetWindowText(m_csFieldNumberNew);
	m_csFieldNumberExist = _T("");    m_edFieldNumberExist.SetWindowText(m_csFieldNumberExist);
	m_bStayReq           = FALSE; m_btnStay.SetCheck(m_bStayReq); OnClickedStayRequired();
	m_csPoleStay         = _T("");    m_cbPoleStay.SetCurSel(-1);
	m_csPoleSize         = _T("");    m_cbPoleSize.SetCurSel(-1);     OnSelectPoleSize();
	m_csPoleDia          = _T("");    m_edPoleDia.SetWindowText(m_csPoleDia);
	m_csHoleDia          = _T("");    m_edHoleDia.SetWindowText(m_csHoleDia);
	m_csHoleDepth        = _T("");    m_edHoleDepth.SetWindowText(m_csHoleDepth);
	m_csPoleFooting      = _T("");    m_cbPoleFooting.SetCurSel(-1);
	
	m_csSpanLength     = _T("");      m_edSpanLength.SetWindowText(m_csSpanLength);
	m_csLineDevDeg     = _T("");      m_edLineDevDeg.SetWindowText(m_csLineDevDeg);
	m_csLanternBracket = _T("");      m_cbLanternBracket.SetCurSel(-1);
	m_csLanternStatus  = _T("");      m_cbLanternStatus.SetCurSel(-1);
	m_csPoleDesc       = _T("");      SetDlgItemText(IDC_POLE_DESCRIPTION, m_csPoleDesc);

	m_bHVABC   = FALSE; m_btnHVABC.SetCheck(m_bHVABC);     OnSelectHVABC();
	m_bHVBare  = FALSE; m_btnHVBare.SetCheck(m_bHVBare);   OnSelectHVBare();
	m_bHVOther = FALSE; m_btnHVOther.SetCheck(m_bHVOther); OnSelectHVOther();

	m_bLVABC   = FALSE; m_btnLVABC.SetCheck(m_bLVABC);       OnSelectLVABC();
	m_bLVBare  = FALSE; m_btnLVBare.SetCheck(m_bLVBare);     OnSelectLVBare();
	m_bLVOther = FALSE; m_btnLVOther.SetCheck(m_bLVOther);   OnSelectLVOther();

	SetDlgItemText(IDC_POLE_HV_SEL, _T(""));
	SetDlgItemText(IDC_POLE_LV_SEL, _T(""));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnHelp()
{
	CTime tmStart(CTime::GetCurrentTime());
	while (T)
	{
		/**/ if (!m_bEditing) { if (DisplayHelp((DWORD) _T("Placing__Editing_poles.htm"))) break; }
		else									{ if (DisplayHelp((DWORD) _T("Editing_a_pole.htm"))) break;	}

		CTime tmCur(CTime::GetCurrentTime());
		CTimeSpan tmSpan; tmSpan = tmCur - tmStart;
		if (tmSpan.GetTotalSeconds() > 5) break;
		// acutPrintf(_T("\r\nSecond: %ld"), tmSpan.GetTotalSeconds());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleDlg::OnOK
// Description  : Called when the user clicks on the "Insert" / "Update" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPoleDlg::OnOK()
{
  // Get the data entered by the user
  UpdateData();

	// Check if the specified pole number exists in the drawing
	if (m_bEditing && !m_csPoleNumber.IsEmpty() && m_csPoleNumber.CompareNoCase(m_csExistingPoleNumber) || !m_bEditing)
	{
		// While adding and if the pole number is changed, check for duplication
		if (CheckForDuplication(m_uiaPoleNumbers, _ttoi(m_csPoleNumber)))
		{
			ShowBalloon(_T("Specified pole number already exists in the drawing."),  this, IDC_POLE_NUMBER); 
			return; 
		}
	}

  // At the very least, the pole status is required
  if (m_csPoleStatus.IsEmpty()) { ShowBalloon(_T("Pole status must be selected."), this, IDC_POLE_STATUS); return; }

  // Get the pole block, layer and layer color based on the pole status
  int iStatus = m_cbPoleStatus.GetCurSel();
  m_csPoleSymbol      = g_csaPoleBlocks.GetAt(iStatus);
  m_csPoleLayer       = g_csaPoleLayers.GetAt(iStatus);
  m_csPoleLayerColor  = g_csaPoleLayerColors.GetAt(iStatus);

	// Store the HV values
	m_csaHVBare.RemoveAll();
	CString csFormat;
	for (int iCtr = 0; (m_bHVABC || m_bHVBare || m_bHVOther) && (iCtr < m_lcHV.GetItemCount()); iCtr++) 
	{
		if (m_lcHV.GetCheck(iCtr) == TRUE) 
		{
			csFormat.Format(_T("%s-%s#%s"), m_lcHV.GetItemText(iCtr, 0), m_lcHV.GetItemText(iCtr, 2), m_lcHV.GetItemText(iCtr, 1));
			m_csaHVBare.Add(csFormat);
		}
	}

	// Store the LV values
	m_csaLVBare.RemoveAll();
	for (int iCtr = 0; (m_bLVABC || m_bLVBare || m_bLVOther) && (iCtr < m_lcLV.GetItemCount()); iCtr++) 
	{
		if (m_lcLV.GetCheck(iCtr) == TRUE) 
		{
			csFormat.Format(_T("%s-%s#%s"), m_lcLV.GetItemText(iCtr, 0), m_lcLV.GetItemText(iCtr, 2), m_lcLV.GetItemText(iCtr, 1));
			m_csaLVBare.Add(csFormat);
		}
	}

	// Get the display parameter
	m_bDisplayPoleNumber = IsDlgButtonChecked(IDC_POLE_DISPLAY);

  // Close the dialog
  CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : GetPoleTag
// Description  : Helper to get the pole block attribute tag based on the description
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString GetPoleTag(CString csAttDesc) 
{  
  for (int iCtr = 0; iCtr < g_csaPoleAttDescs.GetSize(); iCtr++) 
  {
    if (!g_csaPoleAttDescs.GetAt(iCtr).CompareNoCase(csAttDesc)) return g_csaPoleAttTags.GetAt(iCtr); 
  }

  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// Function		 : EditAttributesInPole
// Description : Called to edit the values in the pole.
//////////////////////////////////////////////////////////////////////////
BOOL EditAttributesInPole(CPoleDlg& dlgIP, CString csPoleNumber, ads_point ptPoleIns, double dPoleScale, double dPoleRotation, AcDbObjectId& objPole, struct resbuf *rbpXDExist)
{
	// Edit the attributes in the block
	CString csTag;
	AcDbObjectId objAtt;
	AcDbAttribute *pAtt;
	AcDbBlockReference *pInsert;

	// Open the newly inserted block, get the attribute iterator and close the inserted block
	if (acdbOpenObject(pInsert, objPole, AcDb::kForWrite) != Acad::eOk) return FALSE;
	AcDbObjectIterator *pIter = pInsert->attributeIterator();

	// Attaching already prevalent XDATA to the pole while editing
	if (rbpXDExist != NULL)
	{
		pInsert->setXData(rbpXDExist);
		acutRelRb(rbpXDExist);
	}
	pInsert->close();

	// Loop through the attributes
	// BOOL bForLegend = FALSE;
	int iCtr = 0; 

	for (pIter->start(); !pIter->done(); pIter->step())
	{
		// Get the attribute's object ID and open it for writing
		objAtt = pIter->objectId();
		if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

		// Get the attribute's tag
		csTag = pAtt->tag();

		// Set the attribute's value based on the tag
		if (!csTag.CompareNoCase(GetPoleTag(_T("Pole number"))))       
		{
			pAtt->setTextString(csPoleNumber);
			if (dlgIP.m_bDisplayPoleNumber)	pAtt->setInvisible(Adesk::kFalse); else pAtt->setInvisible(Adesk::kTrue);
		}
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Field pole number")))) 
		{
			//////////////////////////////////////////////////////////////////////////
			// PFNumber tag
			//////////////////////////////////////////////////////////////////////////
			/**/ if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Existing"))) { pAtt->setTextString(dlgIP.m_csFieldNumberNew); }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("New")))			{ /* No modification */ }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Replace")))	{ pAtt->setTextString(dlgIP.m_csFieldNumberExist); }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Remove")))		{ pAtt->setTextString(dlgIP.m_csFieldNumberNew); }
		else pAtt->setTextString(dlgIP.m_csFieldNumberExist);
		}
		else if (!csTag.CompareNoCase(GetPoleTag(_T("New Field pole number"))))
		{
			//////////////////////////////////////////////////////////////////////////
			// ASSET_NUM tag
			//////////////////////////////////////////////////////////////////////////
			/**/ if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Existing"))) {	pAtt->setTextString(dlgIP.m_csFieldNumberExist); }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("New"))) 		  {	pAtt->setTextString(dlgIP.m_csFieldNumberNew); }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Replace")))  {	pAtt->setTextString(dlgIP.m_csFieldNumberNew); }
		else if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Remove")))   {	pAtt->setTextString(dlgIP.m_csFieldNumberExist); }
		else	pAtt->setTextString(dlgIP.m_csFieldNumberNew);
		}
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole stay"))))         pAtt->setTextString(dlgIP.m_csPoleStay);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole new"))))          { if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("New")))      pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole existing"))))     { if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Existing"))) pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole remove"))))       { if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Remove")))   pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole replace"))))      { if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Replace")))  pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole relocate"))))     { if (!dlgIP.m_csPoleStatus.CompareNoCase(_T("Relocate"))) pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole type"))))         pAtt->setTextString(dlgIP.m_csPoleSize);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole footing"))))      pAtt->setTextString(dlgIP.m_csPoleFooting);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Hole depth"))))        pAtt->setTextString(dlgIP.m_csHoleDepth);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Hole dia"))))          pAtt->setTextString(dlgIP.m_csHoleDia);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("HV"))))                pAtt->setTextString(dlgIP.m_csPoleHV);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("LV"))))                pAtt->setTextString(dlgIP.m_csPoleLV);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Span"))))              pAtt->setTextString(dlgIP.m_csSpanLength);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Line Dev"))))          pAtt->setTextString(dlgIP.m_csLineDevDeg);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern bracket"))))   pAtt->setTextString(dlgIP.m_csLanternBracket);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Location"))))          pAtt->setTextString(dlgIP.m_csPoleDesc);
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern existing"))))  { if (!dlgIP.m_csLanternStatus.CompareNoCase(_T("Existing"))) pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern remove"))))    { if (!dlgIP.m_csLanternStatus.CompareNoCase(_T("Remove")))   pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern other"))))     { if (!dlgIP.m_csLanternStatus.CompareNoCase(_T("Other")))    pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(_T("Legend")))			    		              { pAtt->setInvisible(Adesk::kTrue); }

		// Close the attribute
		pAtt->close();
	}

	// Add the XDATA to this pole
	ads_name enPole; acdbGetAdsName(enPole, objPole);
	struct resbuf *rbpPole = acutBuildList(AcDb::kDxfRegAppName, XDATA_POLE, 
																					AcDb::kDxfXdInteger16, _ttoi(csPoleNumber), 
																					AcDb::kDxfXdAsciiString, csPoleNumber,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bDisplayPoleNumber,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleStatus,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csFieldNumberExist,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csFieldNumberNew,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleSize,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleStay,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleFooting,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleDia,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csHoleDepth,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csHoleDia,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleHV,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleLV,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csLanternStatus,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csLanternBracket,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csSpanLength,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csLineDevDeg,
																					AcDb::kDxfXdAsciiString, dlgIP.m_csPoleDesc,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bHVABC,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bHVBare,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bHVOther,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bLVABC,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bLVBare,
																					AcDb::kDxfXdInteger16,   dlgIP.m_bLVOther,
																					AcDb::kDxfXdInteger16,   dlgIP.m_csaHVBare.GetSize(), NULL
		);

	// HV data
	struct resbuf *rbpTemp;
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	CString csFormat;
	for (int iCtr = 0; iCtr < dlgIP.m_csaHVBare.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, dlgIP.m_csaHVBare.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	// LV data
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16, dlgIP.m_csaLVBare.GetSize(), NULL);
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < dlgIP.m_csaLVBare.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, dlgIP.m_csaLVBare.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	addXDataToEntity(enPole, rbpPole);
	acutRelRb(rbpPole);

	// Success
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : InsertPoleBlock
// Description  : Called to insert the pole block, based on the values in the passed in dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InsertPoleBlock(CPoleDlg& dlgIP, CString csPoleNumber, ads_point ptPoleIns, double dPoleScale, double dPoleRotation, AcDbObjectId& objPole, struct resbuf *rbpXDExist)
{
  // Insert the pole block with the given parameters
  if (insertBlock(dlgIP.m_csPoleSymbol, dlgIP.m_csPoleLayer, ptPoleIns, dPoleScale, _tstof(g_csPoleTextHeight), dPoleRotation, g_csPoleTextStyle, objPole) == FALSE) return FALSE;
	ChangeAttributeLocation(objPole, GetPoleTag(_T("Pole number")));

	// Edit attributes in block
	return EditAttributesInPole(dlgIP, csPoleNumber, ptPoleIns, dPoleScale, dPoleRotation, objPole, rbpXDExist);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name: GetHVValues()
// Description  : Retreives indivudal values from input string and populates the values to category arrays appropriately.
// Arguments    : 1. [IN]  Consolidated LV value string
//                2. [OUT] Array of ABC catergory values retrieved from the input string
//                3. [OUT] Array of BARE catergory values retrieved from the input string
//                4. [OUT] Array of OTHER catergory values retrieved from the input string
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetHVValues(CString csPoleHV, CStringArray &csaHVABC, CStringArray &csaHVBare, CStringArray &csaHVOther)
{
	//////////////////////////////////////////////////////////////////////////
	// Sample input string (say. EXIST + 22 + 24 + 25C + 25AC + CCT002 + CCT001)
	//////////////////////////////////////////////////////////////////////////

	// Get individual values specified from the text.
	CString csValue;
	CString csValueSize;
	CString csHVValue;
	while (csPoleHV.Find(_T("+")) != -1)
	{
		// One of the values
		csValue = csPoleHV.Mid(0, csPoleHV.Find(_T("+"))); csValue = csValue.Trim();

		// For this value, find out which category it belongs to
		for (int iCtr = 0; iCtr < g_csaPoleHVText.GetSize(); iCtr++)
		{
			if (!g_csaPoleHVText.GetAt(iCtr).CompareNoCase(csValue))
			{
				if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("ABC"))) 
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaHVABC, csValue + _T("-ABC"), csValueSize, 0);
					if (iIndex < 0) { csHVValue.Format(_T("%s-ABC#1"), csValue); csaHVABC.Add(csHVValue);	}
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csHVValue.Format(_T("%s-ABC#%s"), csValue, csValueSize); 
						csaHVABC.SetAt(iIndex, csHVValue);
					}
				}
				else if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("Bare"))) 
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaHVBare, csValue + _T("-Bare"), csValueSize, 0);
					if (iIndex < 0) {	csHVValue.Format(_T("%s-Bare#1"), csValue); csaHVBare.Add(csHVValue);	}
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csHVValue.Format(_T("%s-Bare#%s"), csValue, csValueSize); 
						csaHVBare.SetAt(iIndex, csHVValue);
					}
				}
				else if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("CCT")))  
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaHVOther, csValue + _T("-CCT"), csValueSize, 0);
					if (iIndex < 0) { csHVValue.Format(_T("%s-CCT#1"), csValue); csaHVOther.Add(csHVValue); }
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csHVValue.Format(_T("%s-CCT#%s"), csValue, csValueSize); 
						csaHVOther.SetAt(iIndex, csHVValue);
					}
				}
				else acutPrintf(_T("\nUnknown HVText value [" + csValue + "]. No match found in tblPoleHV."));
			}
		}

		// Snap the string parsed, so that we can continue with extraction and checking of the next value.
		csPoleHV = csPoleHV.Mid(csPoleHV.Find(_T("+")) + 1);
	}

	// The last string without "+" also has to be accounted for.
	csValue = csPoleHV; csValue = csValue.Trim();

	// Check to which category this text belongs
	for (int iCtr = 0; iCtr < g_csaPoleHVText.GetSize(); iCtr++)
	{
		if (!g_csaPoleHVText.GetAt(iCtr).CompareNoCase(csValue))
		{
			if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("ABC"))) 
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaHVABC, csValue + "-ABC", csValueSize, 0);
				if (iIndex < 0) { csHVValue.Format(_T("%s-ABC#1"), csValue); csaHVABC.Add(csHVValue);	}
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csHVValue.Format(_T("%s-ABC#%s"), csValue, csValueSize); 
					csaHVABC.SetAt(iIndex, csHVValue);
				}
			}
			else if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("Bare"))) 
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaHVBare, csValue + "-Bare", csValueSize, 0);
				if (iIndex < 0) {	csHVValue.Format(_T("%s-Bare#1"), csValue); csaHVBare.Add(csHVValue);	}
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csHVValue.Format(_T("%s-Bare#%s"), csValue, csValueSize); 
					csaHVBare.SetAt(iIndex, csHVValue);
				}
			}
			else if (!g_csaPoleHVCat.GetAt(iCtr).CompareNoCase(_T("CCT")))  
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaHVOther, csValue + "-CCT", csValueSize, 0);
				if (iIndex < 0) { csHVValue.Format(_T("%s-CCT#1"), csValue); csaHVOther.Add(csHVValue); }
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csHVValue.Format(_T("%s-CCT#%s"), csValue, csValueSize); 
					csaHVOther.SetAt(iIndex, csHVValue);
				}
			}
			else acutPrintf(_T("\nUnknown HVText value [" + csValue + _T("]. No match found in tblPoleHV.")));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name: GetLVValues()
// Description  : Retreives individual values from input string and populates the values to category arrays appropriately.
// Arguments    : 1. [IN]  Consolidated LV value string
//                2. [OUT] Array of ABC catergory values retrieved from the input string
//                3. [OUT] Array of BARE catergory values retrieved from the input string
//                4. [OUT] Array of OTHER catergory values retrieved from the input string
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetLVValues(CString csPoleLV, CStringArray &csaLVABC, CStringArray &csaLVBare, CStringArray &csaLVOther)
{
	//////////////////////////////////////////////////////////////////////////
	// Sample input string (say. EXIST + 22 + 24 + 25C + 25AC + CCT002 + CCT001)
	//////////////////////////////////////////////////////////////////////////

	// Get individual values specified from the text.
	CString csValue;
	CString csValueSize;
	CString csLVValue;
	while (csPoleLV.Find(_T("+")) != -1)
	{
		// One of the values
		csValue = csPoleLV.Mid(0, csPoleLV.Find(_T("+"))); csValue = csValue.Trim();

		// For this value, find out which category it belongs to
		for (int iCtr = 0; iCtr < g_csaPoleLVText.GetSize(); iCtr++)
		{
			if (!g_csaPoleLVText.GetAt(iCtr).CompareNoCase(csValue))
			{
				if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("ABC"))) 
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaLVABC, csValue + _T("-ABC"), csValueSize, 0);
					if (iIndex < 0) { csLVValue.Format(_T("%s-ABC#1"), csValue); csaLVABC.Add(csLVValue);	}
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csLVValue.Format(_T("%s-ABC#%s"), csValue, csValueSize); 
						csaLVABC.SetAt(iIndex, csLVValue);
					}
				}
				else if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("Bare"))) 
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaLVBare, csValue + "-Bare", csValueSize, 0);
					if (iIndex < 0) {	csLVValue.Format(_T("%s-Bare#1"), csValue); csaLVBare.Add(csLVValue);	}
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csLVValue.Format(_T("%s-Bare#%s"), csValue, csValueSize); 
						csaLVBare.SetAt(iIndex, csLVValue);
					}
				}
				else if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("Other")))  
				{
					// Check if the value is already present in the array
					int iIndex = GetParameterValue(csaLVOther, csValue + _T("-Other"), csValueSize, 0);
					if (iIndex < 0) { csLVValue.Format(_T("%s-Other#1"), csValue); csaLVOther.Add(csLVValue); }
					else 
					{
						csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
						csLVValue.Format(_T("%s-Other#%s"), csValue, csValueSize); 
						csaLVOther.SetAt(iIndex, csLVValue);
					}
				}
				else acutPrintf(_T("\nUnknown LVText value [" + csValue + "]. No match found in tblPoleLV."));
			}
		}

		// Snap the string parsed, so that we can continue with extraction and checking of the next value.
		csPoleLV = csPoleLV.Mid(csPoleLV.Find(_T("+")) + 1);
	}

	// The last string without "+" also has to be accounted for.
	csValue = csPoleLV; csValue = csValue.Trim();

	// Check to which category this text belongs
	for (int iCtr = 0; iCtr < g_csaPoleLVText.GetSize(); iCtr++)
	{
		if (!g_csaPoleLVText.GetAt(iCtr).CompareNoCase(csValue))
		{
			if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("ABC"))) 
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaLVABC, csValue + "-ABC", csValueSize, 0);
				if (iIndex < 0) { csLVValue.Format(_T("%s-ABC#1"), csValue); csaLVABC.Add(csLVValue);	}
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csLVValue.Format(_T("%s-ABC#%s"), csValue, csValueSize); 
					csaLVABC.SetAt(iIndex, csLVValue);
				}
			}
			else if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("Bare"))) 
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaLVBare, csValue + "-Bare", csValueSize, 0);
				if (iIndex < 0) {	csLVValue.Format(_T("%s-Bare#1"), csValue); csaLVBare.Add(csLVValue);	}
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csLVValue.Format(_T("%s-Bare#%s"), csValue, csValueSize); 
					csaLVBare.SetAt(iIndex, csLVValue);
				}
			}
			else if (!g_csaPoleLVCat.GetAt(iCtr).CompareNoCase(_T("Other")))  
			{
				// Check if the value is already present in the array
				int iIndex = GetParameterValue(csaLVOther, csValue + "-Other", csValueSize, 0);
				if (iIndex < 0) { csLVValue.Format(_T("%s-Other#1"), csValue); csaLVOther.Add(csLVValue); }
				else 
				{
					csValueSize.Format(_T("%d"), _ttoi(csValueSize) + 1);
					csLVValue.Format(_T("%s-Other#%s"), csValue, csValueSize); 
					csaLVOther.SetAt(iIndex, csLVValue);
				}
			}
			else acutPrintf(_T("\nUnknown LVText value [" + csValue + "]. No match found in tblPoleLV."));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : GetAttributeValue
// Description  : Retrieves the attribute values from the insert based on the given attribute description
// Arguments    : 1. [IN]  Entity name of the pole to retrieve the attribute value
//                2. [IN]  Attribute description for which the value is retrieved
//							  3. [OUT] Attribute value retrieved
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetAttributeValue(ads_name enPole, CString csAttTag, CString &csAttValue)
{
	csAttValue = _T("");

	// Get the inserts object id
	AcDbObjectId objInsert; acdbGetObjectId(objInsert, enPole);

	// Get the block reference pointer
	AcDbBlockReference *pInsert = NULL;
	if (acdbOpenObject(pInsert, objInsert, AcDb::kForRead) != Acad::eOk) return false;

	//////////////////////////////////////////////////////////////////////////
	// Get the attribute value now
	//////////////////////////////////////////////////////////////////////////
	// Get the attribute iterator for this insert
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	pInsert->close();

	// Parse through all the attributes and get the value assigned to the given tag.
	AcDbAttribute *pAtt = NULL;
	AcDbObjectId objAttId;

	CString csTag;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		objAttId = pIter->objectId();
		acdbOpenObject(pAtt, objAttId, AcDb::kForRead);
		csTag.Format(_T("%s"), pAtt->tag());

		// Get the attribute value from the block
		if (!csTag.CompareNoCase(csAttTag)) csAttValue = pAtt->textString();

		// Close the attribute
		pAtt->close();
	}

	delete pIter;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name : ApplyChangeRequest94
// Description	 : Depending on the status of the symbol edited, appropriate controls are to be updated in the pole dialog.
// Arguments     : 1. [IN]  Entity name of the pole to apply modification.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplyChangeRequest94(ads_name enPole)
{
	//////////////////////////////////////////////////////////////////// /////////////////////
	// Depending on the status get the appropriate values into the XDATA Change request #94
	//////////////////////////////////////////////////////////////////////////////////////////

	// A. If the pole is NEW (Pole_NEW) then the “New Field pole number” on the Modify Pole form will be populated by the ASSET_NUM field 
	// and the “Existing Field pole number” should be blank.
	// B. If the pole is EXISTING (Pole_EXT) then the “New Field pole number” on the Modify Pole form will be blank and the 
	// “Existing Field pole number” will be populated by the ASSET_NUM field.
	// C. If the pole is REPLACED (Pole Replace NEW) then the “New Field pole number” on the Modify Pole form will be populated by the 
	// ASSET_NUM field, and the “Existing Field pole number” will be populated by the PFNumber field.
	// D. If the pole is DELETED (Pole_DEL) then the “New Field pole number” on the Modify Pole form will be blank, and the 
	// “Existing Field pole number” will be populated by the ASSET_NUM field.

	// Get the XDATA from the pole
	struct resbuf *rbpPole = getXDataFromEntity(enPole, XDATA_POLE);
	if (!rbpPole) return;

	//////////////////////////////////////////////////////////////////////////
	// Get the pole status
	//////////////////////////////////////////////////////////////////////////
	// ("IE_TB_POLE" (1070 . 0) (1000 . "") (1070 . 1) (1000 . "Existing") (1000 . "") (1000 . "3KA224") (1000 . "") (1000 . "") (1000 . "") (1000 . "0.00") (1000 . "") (1000 . "") 
	// (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1070 . 0) (1070 . 0) (1070 . 0) (1070 . 0) (1070 . 0) (1070 . 0) (1070 . 0) 
	CString csPoleStatus = rbpPole->rbnext->rbnext->rbnext->rbnext->resval.rstring; 
	CString csFieldNumberNew;		GetAttributeValue(enPole, GetPoleTag(_T("New Field pole number")), csFieldNumberNew); 
	CString csFieldNumberExist; GetAttributeValue(enPole, GetPoleTag(_T("Field pole number")),		 csFieldNumberExist);

	if (!csPoleStatus.CompareNoCase(_T("New")))
	{
		// Point A above
		csFieldNumberExist = _T("");
	}
	else if (!csPoleStatus.CompareNoCase(_T("Existing")) || !csPoleStatus.CompareNoCase(_T("Remove")))
	{
		// Point B/D above
		csFieldNumberExist = csFieldNumberNew;
		csFieldNumberNew   = _T("");
	}
	else if (!csPoleStatus.CompareNoCase(_T("Replace")))
	{
		// Nothing to be done
	}

	// Build the list again...
	int iCtr = 0;
	struct resbuf *rbpTemp;
	struct resbuf *rbpXD = acutBuildList(AcDb::kDxfRegAppName, XDATA_POLE, NULL);
	for (rbpPole = rbpPole->rbnext; rbpPole; rbpPole = rbpPole->rbnext)
	{
		for (rbpTemp = rbpXD; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		switch (rbpPole->restype)
		{
			case AcDb::kDxfXdHandle		   : rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdHandle, rbpPole->resval.rstring, NULL); break;
			case AcDb::kDxfXdInteger16   : rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16, rbpPole->resval.rint, NULL); break;
			case AcDb::kDxfXdAsciiString : 
				/**/ if (iCtr == 4)
					rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csFieldNumberExist, NULL); 
				else if (iCtr == 5)
					rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csFieldNumberNew, NULL); 
				else
					rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, rbpPole->resval.rstring, NULL); 
				break;
			default	: acutPrintf(L"\n[ERROR:%d]: Incorrect type encountered!", __LINE__); break;
		}

		iCtr++;
	}

	// Modify the XDATA accordingly and attach it back to the pole
	//_tcscpy(rbpPole->rbnext->rbnext->rbnext->rbnext->rbnext->resval.rstring,				 csFieldNumberExist);
	//_tcscpy(rbpPole->rbnext->rbnext->rbnext->rbnext->rbnext->rbnext->resval.rstring, csFieldNumberNew);
	addXDataToEntity(enPole, rbpXD);
	acutRelRb(rbpXD);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ConvertToPole
// Description  : Called when the user selects a block to edit it as a pole. The validation of selection &
//                conversion of the same to a valid pole insert from IE_Toolbox takes place here.
// Arguments    : [IN] Entity name of the pole selected.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct resbuf *ConvertToPole(ads_name enPole)
{
	struct resbuf *rbpPole = NULL;

	// Get the block name
	AcDbObjectId objInsert;
	acdbGetObjectId(objInsert, enPole);

	// Get the block reference pointer
	AcDbBlockReference *pInsert = NULL;
	if (acdbOpenObject(pInsert, objInsert, AcDb::kForRead) != Acad::eOk) return (struct resbuf *) NULL;
	AcDbObjectId objTblRcd = pInsert->blockTableRecord();

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (acdbOpenObject(pBlkTblRcd, objTblRcd, AcDb::kForRead) != Acad::eOk) { pInsert->close(); return (struct resbuf *) NULL; }

	// Convert AcString to CString as some of our utility functions does not understand AcString
	AcString acsName; pBlkTblRcd->getName(acsName); CString csName; csName.Format(_T("%s"), acsName.kTCharPtr());
	pBlkTblRcd->close();

	// Check whether this is a valid pole block from its name. Valid pole block names are retrieved from application MDB.
	// If not a valid block kindly exit the function.
	int iIndexMatched;
	if (!CheckForDuplication(g_csaPoleBlocks, csName, iIndexMatched)) { pInsert->close(); return (struct resbuf *) NULL; }

	// Get the attribute iterator for this insert
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	pInsert->close();

	CString csPoleNumber, csFieldNumberExist, csFieldNumberNew, csPoleDia;
	CString csPoleStatus, csPoleStay, csPoleSize, csPoleFooting, csPoleDesc;
	CString csHoleDepth, csHoleDia, csPoleHVValues, csPoleLVValues;
	CString csSpanLength, csLineDevDeg;
	CString csLanternStatus, csLanternBracket, csLanternExisitng, csLanterRemove, csLanternOther;
	bool bDisplayPoleNumber;
	CStringArray csaHVABC, csaHVBare, csaHVOther;
	CStringArray csaLVABC, csaLVBare, csaLVOther;
	CString csTxtValue;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Parse through all the attributes and pick up values required for attaching them as XDATA.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	AcDbAttribute *pAtt = NULL;
	AcDbObjectId objAttId;
	CString csTag;

	CStringArray csaTagIgnored;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		// Get the object Id of the attribute as we iterate and open the attribute for reading
		objAttId = pIter->objectId();
		if (acdbOpenObject(pAtt, objAttId, AcDb::kForRead) != Acad::eOk) continue;

		// Get the attribute value from the block 
		csTag.Format(_T("%s"), pAtt->tag());
		if (!csTag.CompareNoCase(GetPoleTag(_T("Pole number"))))
		{ 
			csPoleNumber       = pAtt->textString(); 
			bDisplayPoleNumber = (pAtt->isInvisible() == Adesk::kTrue) ? false : true; 
		} 
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Field pole number"))))     csFieldNumberExist = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("New Field pole number")))) csFieldNumberNew   = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole stay"))))             csPoleStay         = pAtt->textString();

		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole new"))))      { csTxtValue.Format(_T("%s"), pAtt->textString()); csTxtValue = csTxtValue.Trim(); if (!csTxtValue.CompareNoCase(_T("X"))) csPoleStatus = _T("New");      }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole existing"))))	{	csTxtValue.Format(_T("%s"), pAtt->textString()); csTxtValue = csTxtValue.Trim(); if (!csTxtValue.CompareNoCase(_T("X"))) csPoleStatus = _T("Existing"); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole remove"))))   {	csTxtValue.Format(_T("%s"), pAtt->textString()); csTxtValue = csTxtValue.Trim(); if (!csTxtValue.CompareNoCase(_T("X"))) csPoleStatus = _T("Remove");   }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole replace"))))  {	csTxtValue.Format(_T("%s"), pAtt->textString()); csTxtValue = csTxtValue.Trim(); if (!csTxtValue.CompareNoCase(_T("X"))) csPoleStatus = _T("Replace");  }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole relocate")))) {	csTxtValue.Format(_T("%s"), pAtt->textString()); csTxtValue = csTxtValue.Trim(); if (!csTxtValue.CompareNoCase(_T("X"))) csPoleStatus = _T("Relocate"); }

		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole type"))))       csPoleSize         = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Pole footing"))))    csPoleFooting      = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Hole depth"))))      csHoleDepth        = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Location"))))        csPoleDesc         = pAtt->textString();

		else if (!csTag.CompareNoCase(GetPoleTag(_T("Hole dia"))))      {	csHoleDia = pAtt->textString();	csPoleDia.Format(_T("%.2f"), _tstof(csPoleDia) / 1000); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("HV"))))           	{ csPoleHVValues = pAtt->textString(); GetHVValues(csPoleHVValues, csaHVABC, csaHVBare, csaHVOther); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("LV"))))           	{ csPoleLVValues = pAtt->textString(); GetLVValues(csPoleLVValues, csaLVABC, csaLVBare, csaLVOther); }
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Span"))))						csSpanLength      = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Line Dev"))))				csLineDevDeg      = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern bracket")))) csLanternBracket  = pAtt->textString();
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern existing"))))
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Existing"); 
		}
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern remove"))))      
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Remove"); 
		}
		else if (!csTag.CompareNoCase(GetPoleTag(_T("Lantern other"))))         
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Other"); 
		}
		else csaTagIgnored.Add(csTag);

		// Close the attribute
		pAtt->close();
	}

	delete pIter;

	// Construct the XDATA and attach it to the pole
	rbpPole = acutBuildList(AcDb::kDxfRegAppName,    XDATA_POLE, 
		AcDb::kDxfXdInteger16,   _ttoi(csPoleNumber), 
		AcDb::kDxfXdAsciiString, csPoleNumber,
		AcDb::kDxfXdInteger16,   bDisplayPoleNumber,
		AcDb::kDxfXdAsciiString, csPoleStatus,
		AcDb::kDxfXdAsciiString, csFieldNumberExist,
		AcDb::kDxfXdAsciiString, csFieldNumberNew,
		AcDb::kDxfXdAsciiString, csPoleSize,
		AcDb::kDxfXdAsciiString, csPoleStay,
		AcDb::kDxfXdAsciiString, csPoleFooting,
		AcDb::kDxfXdAsciiString, csPoleDia,
		AcDb::kDxfXdAsciiString, csHoleDepth,
		AcDb::kDxfXdAsciiString, csHoleDia,
		AcDb::kDxfXdAsciiString, csPoleHVValues,
		AcDb::kDxfXdAsciiString, csPoleLVValues,
		AcDb::kDxfXdAsciiString, csLanternStatus,
		AcDb::kDxfXdAsciiString, csLanternBracket,
		AcDb::kDxfXdAsciiString, csSpanLength,
		AcDb::kDxfXdAsciiString, csLineDevDeg,
		AcDb::kDxfXdAsciiString, csPoleDesc,
		AcDb::kDxfXdInteger16,   csaHVABC.GetSize(),
		AcDb::kDxfXdInteger16,   csaHVBare.GetSize(),
		AcDb::kDxfXdInteger16,   csaHVOther.GetSize(),
		AcDb::kDxfXdInteger16,   csaLVABC.GetSize(),
		AcDb::kDxfXdInteger16,   csaLVBare.GetSize(),
		AcDb::kDxfXdInteger16,   csaLVOther.GetSize(),
		AcDb::kDxfXdInteger16,   csaHVABC.GetSize() + csaHVBare.GetSize() + csaHVOther.GetSize(),
		NULL
		);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// HV data
	// (1000 . "EXIST-ABC#1") (1000 . "22-ABC#1") (1000 . "24-ABC#1") (1000 . "25C-Bare#1") ....
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// HV data (ABC) 
	struct resbuf *rbpTemp;
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaHVABC.GetSize(); iCtr++) { rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaHVABC.GetAt(iCtr), NULL);	rbpTemp = rbpTemp->rbnext; }

	// HV data (Bare)
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaHVBare.GetSize(); iCtr++) { rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaHVBare.GetAt(iCtr), NULL); rbpTemp = rbpTemp->rbnext; }

	// HV data (CCT)
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaHVOther.GetSize(); iCtr++) {	rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaHVOther.GetAt(iCtr), NULL);	rbpTemp = rbpTemp->rbnext; }

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// LV data
	// (1000 . "EXIST-ABC#1") (1000 . "22-ABC#1") (1000 . "24-ABC#1") (1000 . "25C-Bare#1") ....
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16, csaLVABC.GetSize() + csaLVBare.GetSize() + csaLVOther.GetSize(), NULL);

	// LV data (ABC)
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaLVABC.GetSize(); iCtr++) { rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaLVABC.GetAt(iCtr), NULL);	rbpTemp = rbpTemp->rbnext; }

	// LV data (Bare)
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaLVBare.GetSize(); iCtr++) { rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaLVBare.GetAt(iCtr), NULL); rbpTemp = rbpTemp->rbnext; }

	// LV data (Other)
	for (rbpTemp = rbpPole; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < csaLVOther.GetSize(); iCtr++) { rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaLVOther.GetAt(iCtr), NULL); rbpTemp = rbpTemp->rbnext; }

	// Add the XData to the pole entity
	addXDataToEntity(enPole, rbpPole);

	// Print tags ignored
	// for (int iCtr = 0; iCtr < csaTagIgnored.GetSize(); iCtr++) { if (!iCtr) acutPrintf("\nTags ignored...");	acutPrintf("[%s]", csaTagIgnored.GetAt(iCtr)); }
	return rbpPole;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_InsertPole
// Description  : Called when the user enters the "IE_AddPole" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_InsertPole()
{
	// Get existing pole number array
	int iMaxNumber;
	CUIntArray uiaPoleNumbers; GetInsertNumbers(XDATA_POLE, uiaPoleNumbers);
	
  // Initialize the pole information dialog
  CPoleDlg dlgIP;
	
	// Get the Lantern details from database
	CQueryTbl tblLantern;
	CString csSQL;
	csSQL.Format(L"SELECT [LanternType], [LanternBlock], [LanternLayer], [LanternLayerColor] FROM [tblLanternStatus] WHERE [LanternBlock] LIKE '%s%%' ORDER BY [ID]", L"SL_LANTERN");
	if (!tblLantern.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, __FUNCTION__)) return;

	tblLantern.GetColumnAt(0, dlgIP.m_csaLanternTypes);
	//tblLantern.GetColumnAt(1, dlgIL.m_csaLanternBlocks);
	//tblLantern.GetColumnAt(2, dlgIL.m_csaLanternLayers);
	//tblLantern.GetColumnAt(3, dlgIL.m_csaLanternLayerColors);

	dlgIP.m_uiaPoleNumbers.Copy(uiaPoleNumbers);
  dlgIP.m_bFirstRun      = TRUE;
  dlgIP.m_bEditing       = FALSE;

	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

  // Do until user cancels
  while (TRUE)
  {
    // Display the pole information dialog
		if (dlgIP.DoModal() == IDCANCEL) 
		{
			acedSetVar(_T("CLAYER"), &rbCLayer); return; 
		}

    // Set the first run flag
    dlgIP.m_bFirstRun = FALSE;

    // Ensure that the layer for this block is created and current
    createLayer(dlgIP.m_csPoleLayer, Adesk::kFalse);

		// Set the layer color
		setLayerColor(dlgIP.m_csPoleLayer, _ttoi(dlgIP.m_csPoleLayerColor));

		int iRet, iCtr = 0;
    ads_name enPole;
    ads_point ptPoleIns;
    AcDbObjectId objPole;
		
    // Do until user returns or cancels
    while (TRUE)
    {
      // Use the previous point as base point, for second point onwards
      iRet = acedGetPoint((iCtr ? ptPoleIns : NULL), _T("\nPick pole location (ENTER to return, ESC to cancel): "), ptPoleIns);

      // If user wants to return to dialog
      if (iRet == RTNONE) break;

      // If user wants to cancel
			if (iRet != RTNORM) {	acedSetVar(_T("CLAYER"), &rbCLayer); return; }

      // Insert the pole block at the given point
			if (InsertPoleBlock(dlgIP, dlgIP.m_csPoleNumber, ptPoleIns, _tstof(g_csPoleDiameter), 0.0, objPole, NULL) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

      // Increment the counters
			dlgIP.m_uiaPoleNumbers.Add(_ttoi(dlgIP.m_csPoleNumber)); 
      iCtr++; 
			
      // Also set the dialog's pole number variable
			if (CheckForDuplication(dlgIP.m_uiaPoleNumbers, _ttoi(dlgIP.m_csPoleNumber) + 1))
			{
				acutPrintf(_T("\nPole number \"%d\" already exists in the drawing."), _ttoi(dlgIP.m_csPoleNumber) + 1);
				acedSetVar(_T("CLAYER"), &rbCLayer);
				return;
			}

			dlgIP.m_csPoleNumber.Format(_T("%d"), _ttoi(dlgIP.m_csPoleNumber) + 1);
    }
  }

	acedSetVar(_T("CLAYER"), &rbCLayer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_EditPole
// Description  : Called when the user enters the "IE_EditPole" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_EditPole()
{
  ads_name enPole;
  ads_point ptDummy;
  struct resbuf *rbpPole = NULL;
	
  // Do until user selects a pole or cancels
  while (TRUE)
  {
    // Ask user to select the pole to be edited
    acedInitGet(RSG_NONULL, _T(""));
    if (acedEntSel(_T("\nSelect pole to edit:"), enPole, ptDummy) == RTCAN) return;
    
    // Check if what has been selected is actually a pole
    if ((rbpPole = getXDataFromEntity(enPole, XDATA_POLE)) == NULL) 
		{
			//////////////////////////////////////////////////////////////////////////
			// This is not a pole added by "Insert_Pole" of IE_Toolbox, so check if
			// this if the block name matches any of the pole blocks in the standard
			// drawing. If it matches, add the relevant XDATA to it recognize the
			// same as an insert from IE_Toolbox.
			//////////////////////////////////////////////////////////////////////////
			rbpPole = ConvertToPole(enPole);
			if (rbpPole == NULL) { acutPrintf(_T("\nThat is not a valid pole.\n")); return; }
			break;
		}
		else { break; }
  }

	// Apply change request 94
	ApplyChangeRequest94(enPole);
	rbpPole = getXDataFromEntity(enPole, XDATA_POLE);

	/*
	("IE_TB_POLE" (1070 . 2) (1000 . "2") (1070 . 1) (1000 . "New") (1000 . "") (1000 . "") (1000 . "9.5/4") (1000 . "") (1000 . "") (1000 . "0.45") (1000 . "1.55")
	(1000 . "450") (1000 . "3001") (1000 . "1001") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1070 . 1) (1070 . 0) (1070 . 0) (1070 . 1) (1070 . 0) 
	(1070 . 0) (1070 . 1) (1000 . "3001-ABC#1") (1000 . "1001-ABC#1"))))
	*/

  // Get the pole information from the XDATA buffer and show the dialog
  CPoleDlg dlgIP;

	// Get the Lantern details from database
	CQueryTbl tblLantern;
	CString csSQL;
	csSQL.Format(L"SELECT [LanternType], [LanternBlock], [LanternLayer], [LanternLayerColor] FROM [tblLanternStatus] WHERE [LanternBlock] LIKE '%s%%' ORDER BY [ID]", L"SL_LANTERN");
	if (!tblLantern.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, __FUNCTION__)) return;
	tblLantern.GetColumnAt(0, dlgIP.m_csaLanternTypes);

  rbpPole = rbpPole->rbnext->rbnext;
  dlgIP.m_csPoleNumber         = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
	CString csExistingPoleNumber = dlgIP.m_csPoleNumber;
	dlgIP.m_bDisplayPoleNumber   = rbpPole->resval.rint;    rbpPole = rbpPole->rbnext;
  dlgIP.m_csPoleStatus         = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
	dlgIP.m_csFieldNumberExist   = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
	dlgIP.m_csFieldNumberNew     = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
	dlgIP.m_csPoleSize					 = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csPoleStay           = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csPoleFooting        = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csPoleDia            = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csHoleDepth          = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csHoleDia            = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
 dlgIP.m_csPoleHV             = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;

   //CString str_m_csPoleHV             = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;

  dlgIP.m_csPoleLV             = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csLanternStatus      = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csLanternBracket     = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csSpanLength         = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csLineDevDeg         = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_csPoleDesc           = rbpPole->resval.rstring; rbpPole = rbpPole->rbnext;
  dlgIP.m_bStayReq             = (dlgIP.m_csPoleStay.IsEmpty() ? FALSE : TRUE);

	// Retrieve HV details
	dlgIP.m_bHVABC   = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	dlgIP.m_bHVBare  = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	dlgIP.m_bHVOther = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	dlgIP.m_bLVABC   = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	dlgIP.m_bLVBare  = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	dlgIP.m_bLVOther = (rbpPole->resval.rint ? TRUE : FALSE); rbpPole = rbpPole->rbnext;
	
	CString csFormat;

	// Retrieve HV details
	int iSizeOfHVArray = rbpPole->resval.rint; rbpPole = rbpPole->rbnext;
	for (int iCtr = 0; iCtr < iSizeOfHVArray; iCtr++) { dlgIP.m_csaHVBare.Add(rbpPole->resval.rstring);   rbpPole = rbpPole->rbnext; }

	// Retrieve LV details
	int iSizeOfLVArray = rbpPole->resval.rint;    rbpPole = rbpPole->rbnext;
	for (int iCtr = 0; iCtr < iSizeOfLVArray; iCtr++) { dlgIP.m_csaLVBare.Add(rbpPole->resval.rstring);   rbpPole = rbpPole->rbnext; }

	// Release the buffer
	acutRelRb(rbpPole);
	
	dlgIP.m_bFirstRun = FALSE;
	dlgIP.m_bEditing  = TRUE;

	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	// Get existing pole number array
	int iMaxNumber;
	CUIntArray uiaPoleNumbers; GetInsertNumbers(XDATA_POLE, uiaPoleNumbers);

	// Call the pole dialog for inputs
	dlgIP.m_uiaPoleNumbers.Copy(uiaPoleNumbers);
	
	// Display the dialog
	if (dlgIP.DoModal() == IDCANCEL) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	// Ensure that the layer for this block is created and current. Also set its BYLAYER color too.
	createLayer(dlgIP.m_csPoleLayer, Adesk::kFalse);
	setLayerColor(dlgIP.m_csPoleLayer, _ttoi(dlgIP.m_csPoleLayerColor));

  // If the pole symbol/status is different from what is edited, a new pole is inserted with the new XDATA. Else, the XDATA attached 
	// to the pole is edited without erasing the existing insert. This is because, there could be XDATA's not concerning IE_TB that 
	// may be attached to the pole symbol edited.
	AcDbObjectId objPole; if (acdbGetObjectId(objPole, enPole) != Acad::eOk) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }
	AcDbBlockReference *pPole; if (acdbOpenObject(pPole, objPole, AcDb::kForWrite) != Acad::eOk) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	// Get all the XDATA that is prevalent in the pole edited.
	struct resbuf *rbpXDExist = pPole->xData(NULL);
	
	// Get the insertion point, scale and rotation angle of the pole
	AcGePoint3d geIns = pPole->position();
	double dScale     = pPole->scaleFactors().sx;
	double dRotation  = pPole->rotation();

	// If the symbol name changes, a new insert has to be drawn for the new symbol. Else edit the XDATA of the pole symbol being edited.
	if (dlgIP.m_csExistingPoleSymbol.CompareNoCase(dlgIP.m_csPoleSymbol))
	{
		// Erase this pole and close it because depending on the input the pole symbol may change.
		pPole->erase();
		pPole->close();

		// Replace it with the new pole block at the given point 
		if (InsertPoleBlock(dlgIP, dlgIP.m_csPoleNumber, asDblArray(geIns), dScale, dRotation, objPole, rbpXDExist) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }
	}
	else
	{
		pPole->close();

		// Get the object id of the pole selected to edit
		acdbGetObjectId(objPole, enPole);
		
		// Simply edit the attributes of the symbol edited and its XDATA
		if (EditAttributesInPole(dlgIP, dlgIP.m_csPoleNumber, asDblArray(geIns), dScale, dRotation, objPole, rbpXDExist) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }
	}

	// Modify the pole schedule table if it is already generated
	ModifyPoleTable(objPole, csExistingPoleNumber, dlgIP.m_csPoleNumber);

	// Restore the layer that was present prior to execution of this command
	acedSetVar(_T("CLAYER"), &rbCLayer);
}

//////////////////////////////////////////////////////////////////////////
// Function name: Command_ConvertPole()
// Description  : Converts all the valid manual pole inserts to IE Pole.
// Called from  : Called from acrxEntryPoint()
//////////////////////////////////////////////////////////////////////////
void Command_ConvertPole()
{
	struct resbuf *rbpFilter = NULL;

	// Exit if if there are no any valid pole symbols in the database
	if (g_csaPoleBlocks.GetSize() <= 0) return;

	// Define a selection set filter of all valid pole insert names
	if (g_csaPoleBlocks.GetSize() == 1)
	{
		// There is only one valid pole symbol
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), 2, g_csaPoleBlocks.GetAt(0), NULL);
	}
	else
	{
		// There are multiple valid pole symbol's. Build them all in the filter list.
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), -4, _T("<NOT"), AcDb::kDxfRegAppName, XDATA_POLE, -4, _T("NOT>"), -4, _T("<OR"), NULL);
		
		struct resbuf *rbpTemp = NULL;
		for (int iCtr = 0; iCtr < g_csaPoleBlocks.GetSize(); iCtr++)
		{
			for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
			rbpTemp->rbnext = acutBuildList(2, g_csaPoleBlocks.GetAt(iCtr), NULL);
		}

		for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);
	}

	// Make a selection set filter of all valid pole insert names
	ads_name ssGet; if (acedSSGet(_T("X"), NULL, NULL, rbpFilter, ssGet) != RTNORM) { acedSSFree(ssGet); acutRelRb(rbpFilter); return; }

	// Cleanup
	acutRelRb(rbpFilter);

	// For each entity in the selection set call the convert pole function
	ads_name enEntity;

	// Get the number of INSERTS that matched the criteria 
	Adesk::Int32 lLength = 0L; acedSSLength(ssGet, &lLength);
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the entity name from the selection set
		acedSSName(ssGet, lCtr, enEntity);

		// Function that does the conversion
		ConvertToPole(enEntity); 
	}

	// Cleanup
	acedSSFree(ssGet);
}

