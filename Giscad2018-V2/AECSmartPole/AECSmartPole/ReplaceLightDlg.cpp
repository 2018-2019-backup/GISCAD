////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : ReplaceLightDlg.cpp
// Created          : 11th June 2007
// Description      : Replace light Information dialog implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Resource.h"
#include "ReplaceLightDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Replace light Information dialog
IMPLEMENT_DYNAMIC(CReplaceLightDlg, CDialog)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::CReplaceLightDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CReplaceLightDlg::CReplaceLightDlg(CWnd* pParent /*=NULL*/)	: CDialog(CReplaceLightDlg::IDD, pParent)
{
  m_bNonStandard = FALSE;
  m_csLanternSchedule =_T("1");// _T("Schedule 1");
	m_lcCharge.m_bIsPoleDlg = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::~CReplaceLightDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CReplaceLightDlg::~CReplaceLightDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  // Group boxes
  DDX_Control(pDX, IDC_STATIC_REPLACE_CHARGE,          m_stChargeBox);
  DDX_Control(pDX, IDC_STATIC_REPLACE_ASSET,           m_stAssetBox);

  // Controls
  DDX_Control(pDX, IDC_LIGHT_REPLACE_NON_STANDARD,     m_btnNonStandard);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_SCHEDULE,         m_cbLanternSchedule);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_TYPE,             m_cbLanternType);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_LAMP,             m_cbLanternLamp);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_RATE_CODE,        m_edRateCode);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_CHARGE_LIST,      m_lcCharge);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_ASSET_CATEGORY,   m_cbLanternCategory);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_ASSET_COLUMN,     m_edAssetColumn);
  DDX_Control(pDX, IDC_LIGHT_REPLACE_ASSET_LANTERN,    m_edAssetLantern);

  // Variables
  DDX_Check(pDX,    IDC_LIGHT_REPLACE_NON_STANDARD,     m_bNonStandard);
  DDX_CBString(pDX, IDC_LIGHT_REPLACE_SCHEDULE,         m_csLanternSchedule);
  DDX_CBString(pDX, IDC_LIGHT_REPLACE_TYPE,             m_csLanternType);
  DDX_CBString(pDX, IDC_LIGHT_REPLACE_LAMP,             m_csLanternLamp);
  DDX_Text(pDX,     IDC_LIGHT_REPLACE_RATE_CODE,        m_csRateCode);
  DDX_CBString(pDX, IDC_LIGHT_REPLACE_ASSET_CATEGORY,   m_csLanternCategory);
  DDX_Text(pDX,     IDC_LIGHT_REPLACE_ASSET_COLUMN,     m_csAssetColumn);
  DDX_Text(pDX,     IDC_LIGHT_REPLACE_ASSET_LANTERN,    m_csAssetLantern);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CReplaceLightDlg, CDialog)
  ON_BN_CLICKED(IDC_LIGHT_REPLACE_NON_STANDARD, OnClickNonStandard)
  ON_CBN_SELCHANGE(IDC_LIGHT_REPLACE_SCHEDULE,  OnSelectSchedule)
  ON_CBN_SELCHANGE(IDC_LIGHT_REPLACE_TYPE,      OnSelectType)
	ON_BN_CLICKED(IDHELP,													OnHelp)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CReplaceLightDlg::OnInitDialog()
{
  // Call the parent class implementation
  CDialog::OnInitDialog();

  // Create a new bold font
  CFont *pBoldFont = new CFont;
  pBoldFont->CreatePointFont(85, _T("Verdana Bold"));

  // Assign this font to all the static members
  m_stChargeBox.SetFont(pBoldFont);
  m_stAssetBox.SetFont(pBoldFont);

  // Initialize the HV list
  m_lcCharge.InsertColumn(0, _T("Charge to"), LVCFMT_LEFT,   300);
  m_lcCharge.InsertColumn(1, _T("%"),         LVCFMT_CENTER,  50);
  m_lcCharge.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
  m_lcCharge.m_ciaEditColumns.Add(1); // Allow editing for the % column only

  // Add the categories to the combo
  for (int iCtr = 0; iCtr < g_csaLanternCategories.GetSize(); iCtr++) m_cbLanternCategory.AddString(g_csaLanternCategories.GetAt(iCtr));

	// Check for standard
	CheckDlgButton(IDC_LIGHT_NON_STANDARD, m_bNonStandard);
	OnClickNonStandard();

  // Add the charge values to the list
	CString csValue;
  for (int iCtr = 0; iCtr < g_csaLanternCharges.GetSize(); iCtr++)
	{
		m_lcCharge.InsertItem(iCtr, g_csaLanternCharges.GetAt(iCtr));

		// Get the value of this charge if specified already
		GetParameterValue(m_csaChargeTo, g_csaLanternCharges.GetAt(iCtr), csValue, 0);
		if (!csValue.IsEmpty()) 
		{
			m_lcCharge.SetCheck(iCtr, TRUE);
			m_lcCharge.SetItemText(iCtr, 1, csValue);
		}
	}

	 // Select the status, schedule, type, lamp and category
	if (!m_bNonStandard)
	{
		if (m_csLanternSchedule.IsEmpty()) m_csLanternSchedule = _T("1");// _T("Schedule 1");
		m_cbLanternSchedule.SelectString(-1, m_csLanternSchedule); OnSelectSchedule();
		m_cbLanternType.SelectString(-1, m_csLanternType); OnSelectType();
		m_cbLanternLamp.SelectString(-1, m_csLanternLamp);
		m_cbLanternCategory.SelectString(-1, m_csLanternCategory);
	}
	else
	{
		m_cbLanternType.SetWindowText(m_csLanternType);
		m_cbLanternLamp.SetWindowText(m_csLanternLamp);
		m_cbLanternCategory.SetWindowText(m_csLanternCategory);
	}

  // Everything is OK
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::OnClickNonStandard
// Description  : Called when the user clicks the "Non standard" check box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::OnClickNonStandard()
{
	m_bNonStandard = IsDlgButtonChecked(IDC_LIGHT_REPLACE_NON_STANDARD);
	if (m_bNonStandard)
	{
		// Turn the combo's from drop down to simple type.
		m_cbLanternType.SwitchToSimple(); 
		m_cbLanternLamp.SwitchToSimple();
		m_cbLanternCategory.SwitchToSimple();

		// Disable the lantern schedule
		m_csLanternSchedule = _T(""); m_cbLanternSchedule.SetCurSel(-1); m_cbLanternSchedule.EnableWindow(FALSE);
	}
	else 
	{
		m_cbLanternType.SwitchToDropDownList();
		m_cbLanternLamp.SwitchToDropDownList();
		m_cbLanternCategory.SwitchToDropDownList();

		// Add the lantern types to the combo
		for (int iCtr = 0; iCtr < g_csaReplaceLanternSch1Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaReplaceLanternSch1Types.GetAt(iCtr));
		
		// Add the categories to the combo
		for (int iCtr = 0; iCtr < g_csaLanternCategories.GetSize(); iCtr++) m_cbLanternCategory.AddString(g_csaLanternCategories.GetAt(iCtr));

		m_cbLanternSchedule.SelectString(-1, m_csLanternSchedule);
		m_cbLanternSchedule.EnableWindow(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::OnSelectSchedule
// Description  : Called when the user selects a schedule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::OnSelectSchedule()
{
	if (m_bNonStandard) return;

  // Clear the dependencies
  m_cbLanternType.ResetContent();
  m_cbLanternLamp.ResetContent();

  // Get the selected schedule
  m_iSchedule = m_cbLanternSchedule.GetCurSel();
  if (m_iSchedule == CB_ERR) return;

  // Fill the combo from the appropriate array
  if (m_iSchedule == 0) // 1
  {
		for (int iCtr = 0; iCtr < g_csaLanternSch1Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch1Types.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSch1Lamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch1Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSch1Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSch1Lamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSch1Lamps.GetAt(iCtr));
		}
  }
  else if (m_iSchedule == 1)// 2
  {
		for (int iCtr = 0; iCtr < g_csaLanternSch2Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch2Types.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSch2Lamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSch2Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSch2Lamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
		}
  }
  else if (m_iSchedule == 2)// 3
  {
	  	for (int iCtr = 0; iCtr < g_csaLanternSch3Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch3Types.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSch3Lamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSch3Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSch3Lamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSch3Lamps.GetAt(iCtr));
		}
  }
  else if (m_iSchedule == 3)// 4
  {
	  	for (int iCtr = 0; iCtr < g_csaLanternSch4Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch4Types.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSch4Lamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSch4Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSch4Lamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSch4Lamps.GetAt(iCtr));
		}
  }
  else if (m_iSchedule == 4)// 5
  {
	  for (int iCtr = 0; iCtr < g_csaLanternSch5Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch5Types.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSch5Lamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSch5Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSch5Lamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSch5Lamps.GetAt(iCtr));
		}
  }
  else //Unregulated
  {
	for (int iCtr = 0; iCtr < g_csaLanternSchUnRegulatedTypes.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSchUnRegulatedTypes.GetAt(iCtr));
		for (int iCtr = 0; iCtr < g_csaLanternSchUnRegulatedLamps.GetSize(); iCtr++) 
		{
			// m_cbLanternLamp.AddString(g_csaLanternSch2Lamps.GetAt(iCtr));
			// Mail: 06.06.12
			if (!g_csaLanternSchUnRegulatedLamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaLanternSchUnRegulatedLamps.GetAt(iCtr)) == CB_ERR)) m_cbLanternLamp.AddString(g_csaLanternSchUnRegulatedLamps.GetAt(iCtr));
		}
  }
  //// Fill the combo from the appropriate array
  //if (m_iSchedule == 0) 
  //{
  //  for (int iCtr = 0; iCtr < g_csaReplaceLanternSch1Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaReplaceLanternSch1Types.GetAt(iCtr));
  //  for (int iCtr = 0; iCtr < g_csaReplaceLanternSch1Lamps.GetSize(); iCtr++) 
		//{
		//	// m_cbLanternLamp.AddString(g_csaReplaceLanternSch1Lamps.GetAt(iCtr));
		//	// Mail: 06.06.12
		//	if (!g_csaReplaceLanternSch1Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaReplaceLanternSch1Lamps.GetAt(iCtr)) == CB_ERR)) 
		//		m_cbLanternLamp.AddString(g_csaReplaceLanternSch1Lamps.GetAt(iCtr));
		//}
  //}
  //else 
  //{
  //  for (int iCtr = 0; iCtr < g_csaReplaceLanternSch2Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaReplaceLanternSch2Types.GetAt(iCtr));
  //  for (int iCtr = 0; iCtr < g_csaReplaceLanternSch2Lamps.GetSize(); iCtr++) 
		//{
		//	// m_cbLanternLamp.AddString(g_csaReplaceLanternSch2Lamps.GetAt(iCtr));
		//	// Mail: 06.06.12
		//	if (!g_csaReplaceLanternSch2Lamps.GetAt(iCtr).IsEmpty() && (m_cbLanternLamp.SelectString(-1, g_csaReplaceLanternSch2Lamps.GetAt(iCtr)) == CB_ERR)) 
		//		m_cbLanternLamp.AddString(g_csaReplaceLanternSch2Lamps.GetAt(iCtr));
		//}
  //}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectType
// Description  : Called when the user selects a type
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::OnSelectType()
{
  // Get the type
  int iType = m_cbLanternType.GetCurSel();
  if (iType == CB_ERR) return;

  // Set the dependencies based on the schedule
	CString csSQL;
  if (m_iSchedule == 0) 
  {
		// Mail: 06-06-12
    // m_cbLanternLamp.SetCurSel(iType);
		if (m_cbLanternLamp.SelectString(-1, g_csaReplaceLanternSch1Lamps.GetAt(iType)) == CB_ERR) m_cbLanternLamp.SetCurSel(-1); 
    m_edRateCode.SetWindowText(g_csaReplaceLanternSch1RateCodes.GetAt(iType));
  }
  else
  {
    // m_cbLanternLamp.SetCurSel(iType);
		if (m_cbLanternLamp.SelectString(-1, g_csaReplaceLanternSch2Lamps.GetAt(iType)) == CB_ERR) m_cbLanternLamp.SetCurSel(-1); 
    m_edRateCode.SetWindowText(g_csaReplaceLanternSch2Lamps.GetAt(iType));
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::OnHelp() 
{
	// DisplayHelp((DWORD) _T("Replace_stree_light_dialog_box.htm")); 

	CTime tmStart(CTime::GetCurrentTime());
	while (T)
	{
		if (DisplayHelp((DWORD) _T("Replace_stree_light_dialog_box.htm"))) break;

		CTime tmCur(CTime::GetCurrentTime());
		CTimeSpan tmSpan; tmSpan = tmCur - tmStart;
		if (tmSpan.GetTotalSeconds() > 5) break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CReplaceLightDlg::OnOK
// Description  : Called when the user clicks on the _T("OK") button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CReplaceLightDlg::OnOK()
{
  // Get the data entered by the user
  UpdateData();

  // Store the charges into the array
	m_csaChargeTo.RemoveAll();
	CString csChargeToFormat;
  for (int iCtr = 0; iCtr < m_lcCharge.GetItemCount(); iCtr++) 
  {
    if (m_lcCharge.GetCheck(iCtr) == TRUE)
    {
      // m_csaChargeTo.Add(m_lcCharge.GetItemText(iCtr, 0));
      // m_csaChargeTo.Add(m_lcCharge.GetItemText(iCtr, 1));

			csChargeToFormat.Format(_T("%s#%s"), m_lcCharge.GetItemText(iCtr, 0), m_lcCharge.GetItemText(iCtr, 1));
			m_csaChargeTo.Add(csChargeToFormat);
    }
  }
	
  // Fill the array with blanks for the rest
  // for (int iCtr = m_csaChargeTo.GetUpperBound(); iCtr < m_lcCharge.GetItemCount(); iCtr++) { m_csaChargeTo.Add(_T("")); m_csaChargeTo.Add(_T("")); }

  // Close the dialog
  CDialog::OnOK();
}
