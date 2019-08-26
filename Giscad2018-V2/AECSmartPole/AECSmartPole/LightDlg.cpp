////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : LightDlg.cpp
// Created          : 26th April 2007
// Description      : Light Information dialog implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h" 
#include "Resource.h"
#include "LightDlg.h"

//////////////////////////////////////////////////////////////////////////
// Externally defined functions
//////////////////////////////////////////////////////////////////////////
// Defined in OtherCommands.cpp
extern void ModifyLightTable(AcDbObjectId objLight, CString csLightNumber);
extern bool GetAttributeValue(ads_name enPole, CString csAttDesc, CString &csAttValue);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : GetLightTag
// Description  : Helper to get the light block attribute tag based on attribute description
// Arguments    : [IN] Attribute description to get its tag
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString GetLightTag(CString csAttDesc) 
{  
	for (int iCtr = 0; iCtr < g_csaLightAttDescs.GetSize(); iCtr++) 
	{
		if (!g_csaLightAttDescs.GetAt(iCtr).CompareNoCase(csAttDesc)) return g_csaLightAttTags.GetAt(iCtr); 
	}
	return _T("");
}

//////////////////////////////////////////////////////////////////////////////
// Function name : ApplyChangeRequest94ForLights
// Description	 : Depending on the status of the symbol edited, appropriate controls are to be updated in the light dialog.
// Arguments     : 1. [IN]  Entity name of the light to apply modification.
//////////////////////////////////////////////////////////////////////////////
void ApplyChangeRequest94ForLights(ads_name enLight)
{
	//////////////////////////////////////////////////////////////////// /////////////////////
	// Depending on the status get the appropriate values into the XDATA Change request #94
	//////////////////////////////////////////////////////////////////////////////////////////
	// Get the XDATA from the light
	struct resbuf *rbpLight = getXDataFromEntity(enLight, XDATA_LIGHT);
	if (!rbpLight) return;

	// Get the attribute value 
	CString csAttTag = GetLightTag(_T("Lantern number"));
	CString csLanternNumber = L""; 
	GetAttributeValue(enLight, csAttTag, csLanternNumber);

	// Build the list again...
	int iCtr = 0;
	struct resbuf *rbpTemp;
	struct resbuf *rbpXD = acutBuildList(AcDb::kDxfRegAppName, XDATA_LIGHT, NULL);
	for (rbpLight = rbpLight->rbnext; rbpLight; rbpLight = rbpLight->rbnext)
	{
		for (rbpTemp = rbpXD; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

		switch (rbpLight->restype)
		{
		case AcDb::kDxfXdHandle		   : rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdHandle, rbpLight->resval.rstring, NULL); break;
		case AcDb::kDxfXdInteger16   : rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16, rbpLight->resval.rint, NULL); break;
		case AcDb::kDxfXdAsciiString : 
			{
				if (iCtr == 22)
					rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csLanternNumber, NULL); 
				else
					rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, rbpLight->resval.rstring, NULL); 
				break;
			}
		default											 : acutPrintf(L"\n[ERROR:%d]: Incorrect type encountered!", __LINE__); break;
		}

		iCtr++;
	}

	addXDataToEntity(enLight, rbpXD);
	acutRelRb(rbpXD);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Light Information dialog
IMPLEMENT_DYNAMIC(CLightDlg, CDialog)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::CLightDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLightDlg::CLightDlg(CWnd* pParent /*=NULL*/)	: CDialog(CLightDlg::IDD, pParent)
{
	m_bEditing     = FALSE;
	m_bFirstRun    = FALSE;
	m_bColumnReq   = FALSE;
	m_csLanternColumn = _T("");
	m_bNonStandard = FALSE;
	m_csLanternSchedule = _T("1"); //_T("Schedule 1");
	m_csLightNumber = _T("");
	m_lcCharge.m_bIsPoleDlg = false;
	m_csExistingLanternSymbol = _T("");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::~CLightDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLightDlg::~CLightDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Group boxes
	DDX_Control(pDX, IDC_STATIC_LANTERN_INFO, m_stInfoBox);
	DDX_Control(pDX, IDC_STATIC_CHARGE,       m_stChargeBox);
	DDX_Control(pDX, IDC_STATIC_ASSET,        m_stAssetBox);

	// Controls
	DDX_Control(pDX, IDC_LIGHT_LANTERN_SCHEDULE, m_cbLanternSchedule);
	DDX_Control(pDX, IDC_LIGHT_LANTERN_STATUS,   m_cbLanternStatus);
	DDX_Control(pDX, IDC_LIGHT_REPLACE,          m_btnReplace);
	DDX_Control(pDX, IDC_LIGHT_COLUMN_STATUS,    m_cbLanternColumn);
	DDX_Control(pDX, IDC_LIGHT_TYPE,             m_cbLanternType);
	DDX_Control(pDX, IDC_LIGHT_LAMP,             m_cbLanternLamp);
	DDX_Control(pDX, IDC_LIGHT_MAST,             m_cbLanternMast);
	DDX_Control(pDX, IDC_LIGHT_OUTREACH,         m_cbLanternOutreach);
	DDX_Control(pDX, IDC_LIGHT_BRACKET,          m_cbLanternBracket);
	DDX_Control(pDX, IDC_LIGHT_LANTERN_STOCK,    m_edLanternStockCode);
	DDX_Control(pDX, IDC_LIGHT_LAMP_STOCK,       m_edLampStockCode);
	DDX_Control(pDX, IDC_LIGHT_RATE_CODE,        m_edRateCode);
	DDX_Control(pDX, IDC_LIGHT_KINGBOLT,         m_edKingbolt);
	DDX_Control(pDX, IDC_LIGHT_MAST_STOCK,       m_edMastCode);
	DDX_Control(pDX, IDC_LIGHT_OUTREACH_STOCK,   m_edOutreachCode);
	DDX_Control(pDX, IDC_LIGHT_BRACKET_STOCK,    m_edBracketCode);
	DDX_Control(pDX, IDC_LIGHT_COLUMN_REQ,       m_btnInsertColumn);
	DDX_Control(pDX, IDC_LIGHT_CHARGE_LIST,      m_lcCharge);
	DDX_Control(pDX, IDC_LIGHT_ASSET_CATEGORY,   m_cbLanternCategory);
	DDX_Control(pDX, IDC_LIGHT_ASSET_COLUMN,     m_edAssetColumn);
	DDX_Control(pDX, IDC_LIGHT_ASSET_LANTERN,    m_edLanternNumber);
	DDX_Control(pDX, IDC_LIGHT_LANTERN_LOCATION, m_edLanternLocation);
	DDX_Control(pDX, IDOK,                       m_btnAction);
	DDX_Control(pDX, IDC_Rag_Bolt,				 m_cbRagBolt);
	DDX_Control(pDX, IDC_Foundation_Type,		 m_cbFoundationType);

	DDX_Control(pDX, IDC_MAST_RATE,				 m_edMASTRATE);
	DDX_Control(pDX, IDC_Outreach_Rate,			 m_edOutreachRate);
	DDX_Control(pDX, IDC_Bracket_Rate,			 m_edBracketRate);
	DDX_Control(pDX, IDC_Mounting_Height,		 m_edMountingHeight);

	// Variables
	DDX_Check(pDX,    IDC_LIGHT_COLUMN_REQ,       m_bColumnReq);
	DDX_Check(pDX,    IDC_LIGHT_NON_STANDARD,     m_bNonStandard);
	DDX_CBString(pDX, IDC_LIGHT_LANTERN_SCHEDULE, m_csLanternSchedule);
	DDX_CBString(pDX, IDC_LIGHT_LANTERN_STATUS,   m_csLanternStatus);
	DDX_CBString(pDX, IDC_LIGHT_COLUMN_STATUS,    m_csLanternColumn);
	DDX_CBString(pDX, IDC_LIGHT_TYPE,             m_csLanternType);
	DDX_CBString(pDX, IDC_LIGHT_LAMP,             m_csLanternLamp);
	DDX_CBString(pDX, IDC_LIGHT_MAST,             m_csLanternMast);
	DDX_CBString(pDX, IDC_LIGHT_OUTREACH,         m_csLanternOutreach);
	DDX_CBString(pDX, IDC_LIGHT_BRACKET,          m_csLanternBracket);
	DDX_CBString(pDX, IDC_LIGHT_ASSET_CATEGORY,   m_csLanternCategory);

	DDX_CBString(pDX, IDC_Rag_Bolt,				  m_csRagBolt);
	DDX_CBString(pDX, IDC_Foundation_Type,		  m_csFoundationType);


	DDX_CBString(pDX, IDC_MAST_RATE,			 m_csMASTRATE);
	DDX_CBString(pDX, IDC_Outreach_Rate,		 m_csOutreachRate);
	DDX_CBString(pDX, IDC_Bracket_Rate,			 m_csBracketRate);
	DDX_CBString(pDX, IDC_Mounting_Height,		 m_csMountingHeight);

	DDX_Text(pDX,			IDC_LIGHT_NUMBER,	  m_csLightNumber);
	DDX_Text(pDX,     IDC_LIGHT_LANTERN_STOCK,    m_csLanternStockCode);
	DDX_Text(pDX,     IDC_LIGHT_LAMP_STOCK,       m_csLampStockCode);
	DDX_Text(pDX,     IDC_LIGHT_RATE_CODE,        m_csRateCode);
	DDX_Text(pDX,     IDC_LIGHT_KINGBOLT,         m_csKingbolt);
	DDX_Text(pDX,     IDC_LIGHT_MAST_STOCK,       m_csMastCode);
	DDX_Text(pDX,     IDC_LIGHT_OUTREACH_STOCK,   m_csOutreachCode);
	DDX_Text(pDX,     IDC_LIGHT_BRACKET_STOCK,    m_csBracketCode);
	DDX_Text(pDX,     IDC_LIGHT_ASSET_COLUMN,     m_csAssetColumn);
	DDX_Text(pDX,     IDC_LIGHT_ASSET_LANTERN,    m_csLanternNumber);
	DDX_Text(pDX,     IDC_LIGHT_LANTERN_LOCATION, m_csLanternLocation);
	DDV_MaxChars(pDX, m_csLanternLocation, 40);
	DDX_Control(pDX, IDC_LIGHT_NUMBER, m_edLightNumber);
	DDX_Text(pDX, IDC_LIGHT_NUMBER, m_csLightNumber);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLightDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_LIGHT_LANTERN_STATUS, OnSelectStatus)
	ON_CBN_SELCHANGE(IDC_LIGHT_SCHEDULE,  OnSelectSchedule)
	ON_CBN_SELCHANGE(IDC_LIGHT_TYPE,      OnSelectType)
	ON_CBN_SELCHANGE(IDC_LIGHT_MAST,      OnSelectMast)
	ON_CBN_SELCHANGE(IDC_LIGHT_OUTREACH,  OnSelectOutreach)
	ON_CBN_SELCHANGE(IDC_LIGHT_BRACKET,   OnSelectBracket)
	ON_BN_CLICKED(IDC_LIGHT_NON_STANDARD, OnClickNonStandard)
	ON_BN_CLICKED(IDC_LIGHT_COLUMN_REQ,   OnClickColumn)
	ON_BN_CLICKED(IDC_LIGHT_REPLACE,      OnClickReplace)
	ON_BN_CLICKED(IDHELP,                 OnHelp)
	ON_BN_CLICKED(IDC_CLEARALL,						OnBnClickedClearall)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLightDlg::OnInitDialog()
{
	// Call the parent class implementation
	CDialog::OnInitDialog();

	// Create a new bold font
	CFont *pBoldFont = new CFont;
	pBoldFont->CreatePointFont(85, _T("Verdana Bold"));

	// Assign this font to all the static members
	m_stInfoBox.SetFont(pBoldFont);
	m_stChargeBox.SetFont(pBoldFont);
	m_stAssetBox.SetFont(pBoldFont);

	// Initialize the HV list
	m_lcCharge.InsertColumn(0, _T("Charge to"), LVCFMT_LEFT,   300);
	m_lcCharge.InsertColumn(1, _T("%"),         LVCFMT_CENTER,  50);
	m_lcCharge.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	m_lcCharge.m_ciaEditColumns.Add(1); // Allow editing for the % column only

	// Populate the status combo
	for (int iCtr = 0; iCtr < m_csaLanternTypes.GetSize(); iCtr++) m_cbLanternStatus.AddString(m_csaLanternTypes.GetAt(iCtr));

	// Check for standard
	CheckDlgButton(IDC_LIGHT_NON_STANDARD, m_bNonStandard);
	OnClickNonStandard();

	// Add the column types to the combo
	for (int iCtr = 0; iCtr < g_csaColumnTypes.GetSize(); iCtr++) m_cbLanternColumn.AddString(g_csaColumnTypes.GetAt(iCtr));

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

	// Get the maximum of existing light numbers
	if (!m_bEditing) 
	{
		m_csLightNumber.Format(_T("L%d"), getMaxInArray(m_uiaLightNumbers) + 1); 
		m_edLightNumber.SetWindowText(m_csLightNumber); 
	} 
	else 
	{
		// Disable the light status as they will never change this
		m_cbLanternStatus.EnableWindow(FALSE);

		// This is to check if the user has changed the pole number during this session (Nothing to do with Asset Number and PF Number)
		m_csExistingLightNumber = m_csLightNumber;
	}

	// If this is not the first run
	if (m_bFirstRun == FALSE)
	{
		//////////////////////////////////////////////////////////////////////////
		// Show the values for all controls
		//////////////////////////////////////////////////////////////////////////
		int iExistingStatus				= m_cbLanternStatus.SelectString(-1, m_csLanternStatus); 
		if (iExistingStatus != CB_ERR)
		{
			m_csExistingLanternSymbol = m_csaLanternBlocks.GetAt(iExistingStatus);
			OnSelectStatus();
			m_bReplaceData = true;
		}

		m_edLightNumber.SetWindowText(m_csLightNumber);

		if (!m_bNonStandard)
		{
			// Select "Schedule 1" by default
			if (m_csLanternSchedule.IsEmpty()) m_csLanternSchedule = _T("1");// _T("Schedule 1");
			m_cbLanternSchedule.SelectString(-1, m_csLanternSchedule); 

			OnSelectSchedule();

			m_cbLanternType.SelectString(-1, m_csLanternType);         OnSelectType();
			m_cbLanternLamp.SelectString(-1, m_csLanternLamp);
			m_cbLanternMast.SelectString(-1, m_csLanternMast);         OnSelectMast();
			m_cbLanternOutreach.SelectString(-1, m_csLanternOutreach); OnSelectOutreach();
			m_cbLanternBracket.SelectString(-1, m_csLanternBracket);   OnSelectBracket();
			m_cbLanternCategory.SelectString(-1, m_csLanternCategory);
			m_cbRagBolt.SelectString(-1,m_csRagBolt);
			m_cbFoundationType.SelectString(-1, m_csFoundationType);
		}
		else
		{
			m_cbLanternType.SetWindowText(m_csLanternType);
			m_cbLanternLamp.SetWindowText(m_csLanternLamp);
			m_cbLanternMast.SetWindowText(m_csLanternMast);
			m_cbLanternOutreach.SetWindowText(m_csLanternOutreach);
			m_cbLanternBracket.SetWindowText(m_csLanternBracket);
			m_cbLanternCategory.SetWindowText(m_csLanternCategory);
			m_cbRagBolt.SetWindowText(m_csRagBolt);
			m_cbRagBolt.SetWindowText(m_csFoundationType);
		}

		if (!m_csLanternColumn.IsEmpty())
		{
			CheckDlgButton(IDC_LIGHT_COLUMN_REQ, TRUE);
			m_cbLanternColumn.SelectString(-1, m_csLanternColumn);
			m_cbLanternColumn.EnableWindow(TRUE);
		}

		m_edLanternStockCode.SetWindowText(m_csLanternStockCode);
		m_edLampStockCode.SetWindowText(m_csLampStockCode);
		m_edRateCode.SetWindowText(m_csRateCode);
		m_edKingbolt.SetWindowText(m_csKingbolt);
		m_edMastCode.SetWindowText(m_csMastCode);
		m_edOutreachCode.SetWindowText(m_csOutreachCode);
		m_edBracketCode.SetWindowText(m_csBracketCode);
		m_edLanternLocation.SetWindowText(m_csLanternLocation);
		m_edAssetColumn.SetWindowText(m_csAssetColumn);
		m_edLanternNumber.SetWindowText(m_csLanternNumber);

		m_edMASTRATE.SetWindowText(m_csMASTRATE);
		m_edOutreachRate.SetWindowText(m_csOutreachRate);
		m_edBracketRate.SetWindowText(m_csBracketRate);
		m_edMountingHeight.SetWindowText(m_csMountingHeight);

		// If we are editing
		if (m_bEditing == TRUE) 
		{
			// Change the window title
			SetWindowText(_T("Modify street light"));

			// Change the label of the "Insert" button
			m_btnAction.SetWindowText(_T("Modify"));
		}
	}
	else OnSelectSchedule();

	// Existing light number


	// Everything is OK
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::RecreateComboBox
// Description  : Recreate the combo box by copying styles etc, and list items and applying them to a newly created control
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLightDlg::RecreateComboBox(CComboBox* pCombo, LPVOID lpParam/*=0*/)
{
	if (pCombo == NULL)	return FALSE;
	if (pCombo->GetSafeHwnd() == NULL) return FALSE;

	CWnd* pParent = pCombo->GetParent();
	if (pParent == NULL) return FALSE;

	// Get current attributes
	DWORD dwStyle = pCombo->GetStyle();
	DWORD dwStyleEx = pCombo->GetExStyle();
	CRect rc;
	pCombo->GetDroppedControlRect(&rc);
	pParent->ScreenToClient(&rc);	// map to client co-ords
	UINT nID = pCombo->GetDlgCtrlID();
	CFont* pFont = pCombo->GetFont();
	CWnd* pWndAfter = pCombo->GetNextWindow(GW_HWNDPREV);

	// Get the currently selected text (and whether it is a valid list selection)
	CString sCurText;
	int nCurSel = pCombo->GetCurSel();
	BOOL bItemSelValid = nCurSel != -1;
	if (bItemSelValid)
		pCombo->GetLBText(nCurSel, sCurText);
	else
		pCombo->GetWindowText(sCurText);
	pCombo->ModifyStyle(0, CBS_DROPDOWN);

	// Copy the combo box items into a temp combo box (not sorted) along with each item's data
	CComboBox comboNew;
	if (!comboNew.CreateEx(dwStyleEx, _T("COMBOBOX"), _T(""), dwStyle, rc, pParent, nID, lpParam)) return FALSE;
	comboNew.SetFont(pFont);
	int nNumItems = pCombo->GetCount();
	for (int n = 0; n < nNumItems; n++)
	{
		CString sText;
		pCombo->GetLBText(n, sText);
		int nNewIndex = comboNew.AddString(sText);
		comboNew.SetItemData(nNewIndex, pCombo->GetItemData(n));
	}

	// Re-set selected text
	if (bItemSelValid)
		comboNew.SetCurSel(comboNew.FindStringExact(-1, sCurText));
	else
		comboNew.SetWindowText(sCurText);

	// Destroy the existing window, then attach the new one
	pCombo->DestroyWindow(); 
	HWND hwnd = comboNew.Detach();
	pCombo->Attach(hwnd);

	// Position correctly in z-order
	pCombo->SetWindowPos(pWndAfter == NULL ? &CWnd::wndBottom : pWndAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnClickNonStandard
// Description  : Called when the user clicks the "Non standard" check box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnClickNonStandard()
{
	m_bNonStandard = IsDlgButtonChecked(IDC_LIGHT_NON_STANDARD);
	if (m_bNonStandard)
	{
		// Turn the combo's from drop down to simple type.
		m_cbLanternType.SwitchToSimple(); 
		m_cbLanternMast.SwitchToSimple();
		m_cbLanternLamp.SwitchToSimple();
		m_cbLanternOutreach.SwitchToSimple();
		m_cbLanternBracket.SwitchToSimple();
		m_cbLanternCategory.SwitchToSimple();
		m_cbRagBolt.SwitchToSimple();
		m_cbFoundationType.SwitchToSimple();
		// Disable the lantern schedule
		m_csLanternSchedule = _T(""); m_cbLanternSchedule.SetCurSel(-1); m_cbLanternSchedule.EnableWindow(FALSE);
	}
	else 
	{
		m_cbLanternType.SwitchToDropDownList();
		m_cbLanternMast.SwitchToDropDownList();
		m_cbLanternLamp.SwitchToDropDownList();
		m_cbLanternOutreach.SwitchToDropDownList();
		m_cbLanternBracket.SwitchToDropDownList();
		m_cbLanternCategory.SwitchToDropDownList();
		m_cbRagBolt.SwitchToDropDownList();
		m_cbFoundationType.SwitchToDropDownList();

		m_cbRagBolt.ResetContent();
		m_cbFoundationType.ResetContent();

		// Add the lantern types to the combo
		for (int iCtr = 0; iCtr < g_csaLanternSch1Types.GetSize(); iCtr++) m_cbLanternType.AddString(g_csaLanternSch1Types.GetAt(iCtr));

		// Add the mast types to the combo
		for (int iCtr = 0; iCtr < g_csaLanternMasts.GetSize(); iCtr++) m_cbLanternMast.AddString(g_csaLanternMasts.GetAt(iCtr));

		// Add the outreach types to the combo
		for (int iCtr = 0; iCtr < g_csaLanternOutreaches.GetSize(); iCtr++) m_cbLanternOutreach.AddString(g_csaLanternOutreaches.GetAt(iCtr));

		// Add the bracket types to the combo
		for (int iCtr = 0; iCtr < g_csaLanternBrackets.GetSize(); iCtr++) m_cbLanternBracket.AddString(g_csaLanternBrackets.GetAt(iCtr));

		// Add the categories to the combo
		for (int iCtr = 0; iCtr < g_csaLanternCategories.GetSize(); iCtr++) m_cbLanternCategory.AddString(g_csaLanternCategories.GetAt(iCtr));

		//Add the RagBolt to Combo
		for (int iCtr = 0; iCtr < g_csaRagboltType.GetSize(); iCtr++) m_cbRagBolt.AddString(g_csaRagboltType.GetAt(iCtr));

		//Add the Foundation Type to Combo
		for (int iCtr = 0; iCtr < g_csaFoundationType.GetSize(); iCtr++) m_cbFoundationType.AddString(g_csaFoundationType.GetAt(iCtr));



		m_cbLanternSchedule.SelectString(-1, m_csLanternSchedule);
		m_cbLanternSchedule.EnableWindow(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectStatus
// Description  : Called when the user selects the light status
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectStatus()
{
	// Get the selected status
	int iStatus = m_cbLanternStatus.GetCurSel();
	if (iStatus == CB_ERR) return;
	CString csStatus; m_cbLanternStatus.GetLBText(iStatus, csStatus);

	// If we are replacing a light, enable the button and set the flag
	BOOL bReplace = (csStatus == _T("Replace"));
	m_btnReplace.EnableWindow(bReplace);
	m_bReplace = bReplace;
	m_bReplaceData = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnClickReplace
// Description  : Called when the user clicks on the "Replace" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnClickReplace()
{
	// Show the replace light dialog
	if (m_dlgRL.DoModal() == IDCANCEL) return;

	// Set the flag
	m_bReplaceData = TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnClickColumn
// Description  : Called when the user clicks on the "Insert column" check box
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnClickColumn()
{
	// Get the check status
	m_bColumnReq = m_btnInsertColumn.GetCheck();

	// If it is required
	if (m_bColumnReq == TRUE) m_cbLanternColumn.EnableWindow(TRUE); 
	// Otherwise, reset and disable it
	else { m_cbLanternColumn.SetCurSel(-1); m_cbLanternColumn.EnableWindow(FALSE); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectSchedule
// Description  : Called when the user selects a schedule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectSchedule()
{
	if (m_bNonStandard) return;

	// Clear the dependencies
	m_cbLanternType.ResetContent(); m_edLanternStockCode.SetWindowText(_T(""));
	m_cbLanternLamp.ResetContent(); m_edLampStockCode.SetWindowText(_T(""));

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectType
// Description  : Called when the user selects a type
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectType()
{
	// Get the type
	int iType = m_cbLanternType.GetCurSel();
	if (iType == CB_ERR) return;


	// Set the dependencies based on the schedule
	if (m_iSchedule == 0) 
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSch1LanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSch1LampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSch1RateCodes.GetAt(iType));
	}
	else if(m_iSchedule == 1) 
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSch2LanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSch2LampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSch2RateCodes.GetAt(iType));
	}
	else if(m_iSchedule == 2) 
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSch3LanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSch3LampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSch3RateCodes.GetAt(iType));
	}
	else if(m_iSchedule == 3) 
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSch4LanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSch4LampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSch4RateCodes.GetAt(iType));
	}
	else if(m_iSchedule == 4) 
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSch5LanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSch5LampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSch5RateCodes.GetAt(iType));
	}
	else
	{
		m_cbLanternLamp.SetCurSel(iType);
		m_edLanternStockCode.SetWindowText(g_csaLanternSchUnRegulatedLanternCodes.GetAt(iType));
		m_edLampStockCode.SetWindowText(g_csaLanternSchUnRegulatedLampCodes.GetAt(iType));
		m_edRateCode.SetWindowText(g_csaLanternSchUnRegulatedRateCodes.GetAt(iType));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectMast
// Description  : Called when the user selects a mast
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectMast()
{
	// Get the mast
	int iMast = m_cbLanternMast.GetCurSel();
	if (iMast == CB_ERR) return;

	// Set the mast stock code
	m_edMastCode.SetWindowText(g_csaLanternMastCodes.GetAt(iMast));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectOutreach
// Description  : Called when the user selects an outreach
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectOutreach()
{
	// Get the Outreach
	int iOutreach = m_cbLanternOutreach.GetCurSel();
	if (iOutreach == CB_ERR) return;

	// Set the Outreach stock code
	m_edOutreachCode.SetWindowText(g_csaLanternOutreachCodes.GetAt(iOutreach));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnSelectBracket
// Description  : Called when the user selects a bracket
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnSelectBracket()
{
	// Get the Bracket
	int iBracket = m_cbLanternBracket.GetCurSel();
	if (iBracket == CB_ERR) return;

	// Set the Bracket stock code
	m_edBracketCode.SetWindowText(g_csaLanternBracketCodes.GetAt(iBracket));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnBnClickedClearall()
// Description  : Clears all the user inputs sp far specified in this session of the dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnBnClickedClearall()
{
	m_bNonStandard = false;					   OnClickNonStandard();
	m_cbLanternStatus.SetCurSel(-1);   OnSelectStatus(); m_btnReplace.EnableWindow(false); 
	m_cbLanternType.SetCurSel(-1);     OnSelectType();
	m_csLanternStockCode.Empty();      m_edLanternStockCode.SetWindowText(m_csLanternStockCode);
	m_csRateCode.Empty();              m_edRateCode.SetWindowText(m_csLanternStockCode);
	m_cbLanternMast.SetCurSel(-1);	   OnSelectMast();
	m_cbLanternOutreach.SetCurSel(-1); OnSelectOutreach();
	m_cbLanternBracket.SetCurSel(-1);  OnSelectBracket();
	m_csLanternLocation.Empty();       m_edLanternLocation.SetWindowText(m_csLanternLocation);
	m_cbLanternSchedule.SetCurSel(0);  OnSelectSchedule();

	m_bColumnReq = false;							 OnClickColumn();
	m_cbLanternLamp.SetCurSel(-1);		
	m_csLampStockCode.Empty();         m_edLampStockCode.SetWindowText(m_csLampStockCode);
	m_csKingbolt.Empty();              m_edKingbolt.SetWindowText(m_csKingbolt);
	m_csMastCode.Empty();							 m_edMastCode.SetWindowText(m_csMastCode);
	m_csOutreachCode.Empty();					 m_edOutreachCode.SetWindowText(m_csOutreachCode);
	m_csBracketCode.Empty();					 m_edBracketCode.SetWindowText(m_csBracketCode);

	m_csMASTRATE.Empty();	m_edMASTRATE.SetWindowText(m_csMASTRATE);
	m_csOutreachRate.Empty();	m_edOutreachRate.SetWindowText(m_csOutreachRate);
	m_csBracketRate.Empty();	m_edBracketRate.SetWindowText(m_csBracketRate);
	m_csMountingHeight.Empty();	m_edMountingHeight.SetWindowText(m_csBracketRate);
	// Remove the entries in the list control 
	CString csValue;
	m_csaChargeTo.RemoveAll();
	for (int iCtr = 0; iCtr < g_csaLanternCharges.GetSize(); iCtr++)
	{
		m_lcCharge.SetCheck(iCtr, false);
		m_lcCharge.SetItemText(iCtr, 1, _T(""));
	}

	m_cbLanternCategory.SetCurSel(-1);
	m_csAssetColumn.Empty();  m_edAssetColumn.SetWindowText(m_csAssetColumn);
	m_csLanternNumber.Empty(); m_edLanternNumber.SetWindowText(m_csLanternNumber);

	// Replace dialog variables
	m_dlgRL.m_csLanternType.Empty();
	m_dlgRL.m_csLanternLamp.Empty();
	m_dlgRL.m_csRateCode.Empty();
	m_dlgRL.m_csLanternCategory.Empty();
	m_dlgRL.m_csaChargeTo.RemoveAll();
	m_dlgRL.m_csAssetColumn.Empty();
	m_dlgRL.m_csAssetLantern.Empty();
	m_dlgRL.m_bNonStandard = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnHelp()
{
	// if (!m_bEditing) DisplayHelp((DWORD) _T("Adding_a_street_light.htm")); else DisplayHelp((DWORD) _T("Editing_a_street_light.htm"));

	CTime tmStart(CTime::GetCurrentTime());
	while (T)
	{
		/**/ if (!m_bEditing) { if (DisplayHelp((DWORD) _T("Adding_a_street_light.htm")))  break; }
		else									{ if (DisplayHelp((DWORD) _T("Editing_a_street_light.htm"))) break;	}

		CTime tmCur(CTime::GetCurrentTime());
		CTimeSpan tmSpan; tmSpan = tmCur - tmStart;
		if (tmSpan.GetTotalSeconds() > 5) break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CLightDlg::OnOK
// Description  : Called when the user clicks on the "Insert" / "Update" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLightDlg::OnOK()
{
	// Get the data entered by the user
	UpdateData();

	// Check if the light number specified is prefixed with "L"
	if (m_csLightNumber.IsEmpty() || (m_csLightNumber.Mid(0, 1) != _T("L")) || (_ttoi(m_csLightNumber.Mid(1)) <= 0))
	{
		ShowBalloon(_T("Specified light number is not valid."),  this, IDC_LIGHT_NUMBER);
		return;
	}

	// Check if the specified light number exists in the drawing
	if (m_bEditing && m_csLightNumber.CompareNoCase(m_csExistingLightNumber) || !m_bEditing)
	{
		// While adding and if the pole number is changed, check for duplication
		if (CheckForDuplication(m_uiaLightNumbers, _ttoi(m_csLightNumber.Mid(1))))
		{
			ShowBalloon(_T("Specified light number already exists in the drawing."), this, IDC_LIGHT_NUMBER);
			return; 
		}
	}

	// At the very least, the lantern status is required
	if (m_csLanternStatus.IsEmpty()) { ShowBalloon(_T("Lantern status must be selected."), this, IDC_LIGHT_LANTERN_STATUS); return; }

	// If the status is "Replace", then the replace details must have been provided
	if ((m_bReplace == TRUE) && (m_bReplaceData == FALSE)) { ShowBalloon(_T("The details of the light being\nreplaced must be provided."), this, IDC_LIGHT_REPLACE); return; }

	// Get the light block, layer and layer color based on the light status
	int iLightStatus = m_cbLanternStatus.GetCurSel();
	m_csLanternSymbol      = m_csaLanternBlocks.GetAt(iLightStatus);
	m_csLanternLayer       = m_csaLanternLayers.GetAt(iLightStatus);
	m_csLanternLayerColor  = m_csaLanternLayerColors.GetAt(iLightStatus);

	// Get the column block, layer and layer color
	int iColumnStatus = m_cbLanternColumn.GetCurSel();
	if (iColumnStatus != CB_ERR)
	{
		m_cbLanternColumn.GetLBText(iColumnStatus, m_csLanternColumn);
		m_csColumnSymbol      = g_csaColumnBlocks.GetAt(iColumnStatus);
		m_csColumnLayer       = g_csaColumnLayers.GetAt(iColumnStatus);
		m_csColumnLayerColor  = g_csaColumnLayerColors.GetAt(iColumnStatus);
	}

	int iRagBolt = m_cbRagBolt.GetCurSel();
	if(iRagBolt != CB_ERR)
	{
		m_cbRagBolt.GetLBText(iRagBolt, m_csRagBolt);
	}

	int iFoundationType = m_cbFoundationType.GetCurSel();
	if(iFoundationType	!=CB_ERR)
	{
		m_cbFoundationType.GetLBText(iFoundationType,m_csFoundationType);
	}



	// Store the charges into the array
	m_csaChargeTo.RemoveAll();
	CString csChargeToFormat;
	for (int iCtr = 0; iCtr < m_lcCharge.GetItemCount(); iCtr++) 
	{
		if (m_lcCharge.GetCheck(iCtr) == TRUE) 
		{
			csChargeToFormat.Format(_T("%s#%s"), m_lcCharge.GetItemText(iCtr, 0), m_lcCharge.GetItemText(iCtr, 1));
			m_csaChargeTo.Add(csChargeToFormat);
		}
	}

	// Close the dialog
	CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : GetColumnTag
// Description  : Helper to get the column block's attribute tag based on the attribute description
// Arguments    : [IN] Attribute description to get its tag
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString GetColumnTag(CString csAttDesc) 
{  
	for (int iCtr = 0; iCtr < g_csaColumnAttDescs.GetSize(); iCtr++) { if (g_csaColumnAttDescs.GetAt(iCtr) == csAttDesc) return g_csaColumnAttTags.GetAt(iCtr); }
	return _T("");
}

//////////////////////////////////////////////////////////////////////////
// Function		 : EditAttributesInLight
// Description : Called to edit the values in the light.
// Arguments   : 1. [IN]  Instance of the Light dialog
//							 2. [IN]  Light number to be assigned for the light
//							 3. [IN]  Insertion point of the light block
//               4. [IN]  Insertion scale factor for the light block
//							 5. [IN]  Rotation angle to insert the light block
//               6. [OUT] Object Id of the newly inserted light symbol
//               7. [OUT] Object Id of the newly inserted light number symbol
//               8. [OUT] Object Id of the newly inserted replace light number symbol
//							 9. [IN]  XDATA's to be attached to the light. This will have a valid linked list when a light block is edited.  
//////////////////////////////////////////////////////////////////////////
BOOL EditAttributesInLight(CLightDlg& dlgIL, CString csLightNumber, ads_point ptLightIns, double dLightScale, double dLightRotation, AcDbObjectId objLight, AcDbObjectId objLightNum, AcDbObjectId objRepLightNum, struct resbuf *rbpXDExist)
{
	// Edit the attributes in the block
	CString csTag;
	AcDbObjectId objAtt;
	AcDbAttribute *pAtt;
	AcDbBlockReference *pInsert;

	// Open the newly inserted block, get the attribute iterator and close the inserted block
	if (acdbOpenObject(pInsert, objLight, AcDb::kForWrite) != Acad::eOk) return FALSE;
	AcDbObjectIterator *pIter = pInsert->attributeIterator();

	// Attaching already prevalent XDATA to the light while editing
	if (rbpXDExist != NULL)
	{
		pInsert->setXData(rbpXDExist);
		acutRelRb(rbpXDExist);
	}
	pInsert->close();

	//////////////////////////////////////////////////////////////////////////
	// Loop through the attributes
	//////////////////////////////////////////////////////////////////////////
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		// Get the attribute's object ID and open it for writing
		objAtt = pIter->objectId();
		if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

		// Get the attribute's tag
		csTag = pAtt->tag();

		// Set the attribute's value based on the tag
		// /**/ if (!csTag.CompareNoCase(GetLightTag(_T("Light number"))))        pAtt->setTextString(_T("0"));
		/**/ if (!csTag.CompareNoCase(GetLightTag(_T("Light number"))))        pAtt->setTextString(dlgIL.m_csLightNumber);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern number"))))      pAtt->setTextString(dlgIL.m_csLanternNumber);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light new"))))           { if (dlgIL.m_csLanternStatus    == _T("New"))      pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light existing"))))      { if (dlgIL.m_csLanternStatus    == _T("Existing")) pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light remove"))))        { if (dlgIL.m_csLanternStatus    == _T("Remove"))   pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light replace"))))       { if (dlgIL.m_csLanternStatus    == _T("Replace"))  pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern type"))))        pAtt->setTextString(dlgIL.m_csLanternType);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern stock code"))))  pAtt->setTextString(dlgIL.m_csLanternStockCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lamp type"))))           pAtt->setTextString(dlgIL.m_csLanternLamp);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lamp stock code"))))     pAtt->setTextString(dlgIL.m_csLampStockCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Rate code"))))           pAtt->setTextString(dlgIL.m_csRateCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Kingbolt height"))))     pAtt->setTextString(dlgIL.m_csKingbolt);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Mast type"))))           pAtt->setTextString(dlgIL.m_csLanternMast);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Mast stock code"))))     pAtt->setTextString(dlgIL.m_csMastCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Outreach type"))))       pAtt->setTextString(dlgIL.m_csLanternOutreach);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Outreach stock code")))) pAtt->setTextString(dlgIL.m_csOutreachCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Bracket type"))))        pAtt->setTextString(dlgIL.m_csLanternBracket);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Bracket stock code"))))  pAtt->setTextString(dlgIL.m_csBracketCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern category"))))    pAtt->setTextString(dlgIL.m_csLanternCategory);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Column/pole number"))))  pAtt->setTextString(dlgIL.m_csAssetColumn);

		else if(!csTag.CompareNoCase(GetLightTag(_T("RagBolt"))))  
			pAtt->setTextString(dlgIL.m_csRagBolt);
		else if(!csTag.CompareNoCase(GetLightTag(_T("FoundationType"))))  pAtt->setTextString(dlgIL.m_csFoundationType);
		else if(!csTag.CompareNoCase(GetLightTag(_T("MastRate"))))  pAtt->setTextString(dlgIL.m_csMASTRATE);
		else if(!csTag.CompareNoCase(GetLightTag(_T("OutReachRate"))))  pAtt->setTextString(dlgIL.m_csOutreachRate);
		else if(!csTag.CompareNoCase(GetLightTag(_T("BracketRate"))))  pAtt->setTextString(dlgIL.m_csBracketRate);
		else if(!csTag.CompareNoCase(GetLightTag(_T("Mounting Height"))))  pAtt->setTextString(dlgIL.m_csMountingHeight);

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 1"))) && (dlgIL.m_csaChargeTo.GetSize() > 0))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(0).Mid(0, dlgIL.m_csaChargeTo.GetAt(0).Find(_T("#"))));

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 2"))) && (dlgIL.m_csaChargeTo.GetSize() > 1))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(1).Mid(0, dlgIL.m_csaChargeTo.GetAt(1).Find(_T("#"))));

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 3"))) && (dlgIL.m_csaChargeTo.GetSize() > 2))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(2).Mid(0, dlgIL.m_csaChargeTo.GetAt(2).Find(_T("#"))));

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 1 percent"))) && (dlgIL.m_csaChargeTo.GetSize() > 0))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(0).Mid(dlgIL.m_csaChargeTo.GetAt(0).Find(_T("#")) + 1));

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 2 percent"))) && (dlgIL.m_csaChargeTo.GetSize() > 1))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(1).Mid(dlgIL.m_csaChargeTo.GetAt(1).Find(_T("#")) + 1));

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 3 percent"))) && (dlgIL.m_csaChargeTo.GetSize() > 2))
			pAtt->setTextString(dlgIL.m_csaChargeTo.GetAt(2).Mid(dlgIL.m_csaChargeTo.GetAt(2).Find(_T("#")) + 1));

		// These attributes will appear if this is a "Replace" lamp only
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lantern type"))))          pAtt->setTextString(dlgIL.m_dlgRL.m_csLanternType);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lamp type"))))             pAtt->setTextString(dlgIL.m_dlgRL.m_csLanternLamp);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Rate code"))))             pAtt->setTextString(dlgIL.m_dlgRL.m_csRateCode);
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lantern category"))))      pAtt->setTextString(dlgIL.m_dlgRL.m_csLanternCategory);

		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 1"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 0))
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(0).Mid(0, dlgIL.m_dlgRL.m_csaChargeTo.GetAt(0).Find(_T("#"))));
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 2"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 1))
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(1).Mid(0, dlgIL.m_dlgRL.m_csaChargeTo.GetAt(1).Find(_T("#"))));
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 3"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 2))
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(2).Mid(0, dlgIL.m_dlgRL.m_csaChargeTo.GetAt(2).Find(_T("#"))));
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 1 percent"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 0))      
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(0).Mid(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(0).Find(_T("#")) + 1));
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 2 percent"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 1))
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(1).Mid(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(1).Find(_T("#")) + 1));
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 3 percent"))) && (dlgIL.m_dlgRL.m_csaChargeTo.GetSize() > 2))
			pAtt->setTextString(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(2).Mid(dlgIL.m_dlgRL.m_csaChargeTo.GetAt(2).Find(_T("#")) + 1));
		else if (!csTag.CompareNoCase(_T("Legend")))	  { pAtt->setInvisible(Adesk::kTrue); }

		// Close the attribute
		pAtt->close();
	}

	//////////////////////////////////////////////////////////////////////////
	// Add the XDATA to this light
	//////////////////////////////////////////////////////////////////////////
	// Get the handle of the light to attach to Light Number and Replace Light Number blocks
	ads_name enLight; acdbGetAdsName(enLight,	objLight);
	AcDbHandle dbHandle = objLight.handle();

	ACHAR szLightHandle[17]; 
	memset((void *)&szLightHandle, '\0', sizeof(ACHAR) * 17);
	dbHandle.getIntoAsciiBuffer(szLightHandle);

	// Get the handle of the light number to attach to light block
	ads_name enLightNum; acdbGetAdsName(enLightNum, objLightNum);
	dbHandle = objLightNum.handle();
	ACHAR szLightNumHandle[17]; 
	memset((void *)&szLightNumHandle, '\0', sizeof(ACHAR) * 17);
	dbHandle.getIntoAsciiBuffer(szLightNumHandle);

	// Get the handle of the replace light number to attach to light block
	ads_name enRepLightNum; 
	ACHAR szRepLightNumHandle[17]; 
	memset((void *)&szRepLightNumHandle, '\0', sizeof(ACHAR) * 17);

	if (dlgIL.m_bReplace == TRUE)
	{
		acdbGetAdsName(enRepLightNum, objRepLightNum);
		dbHandle = objRepLightNum.handle();
		dbHandle.getIntoAsciiBuffer(szRepLightNumHandle);
	}

	struct resbuf *rbpLight = acutBuildList(AcDb::kDxfRegAppName,    XDATA_LIGHT, 
		AcDb::kDxfXdHandle,      szLightNumHandle,
		AcDb::kDxfXdHandle,      szRepLightNumHandle,
		AcDb::kDxfXdInteger16,   _ttoi(csLightNumber.Mid(1)), 
		AcDb::kDxfXdAsciiString, csLightNumber,
		AcDb::kDxfXdInteger16,   dlgIL.m_bNonStandard, 
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternSchedule,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternStatus,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternType,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternStockCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternLamp,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLampStockCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csRateCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csKingbolt,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternMast,
		AcDb::kDxfXdAsciiString, dlgIL.m_csMastCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternOutreach,
		AcDb::kDxfXdAsciiString, dlgIL.m_csOutreachCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternBracket,
		AcDb::kDxfXdAsciiString, dlgIL.m_csBracketCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternLocation,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternCategory,
		AcDb::kDxfXdAsciiString, dlgIL.m_csAssetColumn,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternNumber,
		AcDb::kDxfXdAsciiString, dlgIL.m_csLanternColumn,
		AcDb::kDxfXdAsciiString, dlgIL.m_szColumnHandle,

		AcDb::kDxfXdAsciiString, dlgIL.m_csRagBolt,
		AcDb::kDxfXdAsciiString, dlgIL.m_csFoundationType,
		AcDb::kDxfXdAsciiString, dlgIL.m_csMASTRATE,
		AcDb::kDxfXdAsciiString, dlgIL.m_csOutreachRate,
		AcDb::kDxfXdAsciiString, dlgIL.m_csBracketRate,	
		AcDb::kDxfXdAsciiString, dlgIL.m_csMountingHeight,

		AcDb::kDxfXdInteger16,	 dlgIL.m_csaChargeTo.GetSize(),	NULL);

	struct resbuf *rbpTemp;
	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

	CString csChargeToFormat;
	for (int iCtr = 0; iCtr < dlgIL.m_csaChargeTo.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, dlgIL.m_csaChargeTo.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

	rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16,   dlgIL.m_dlgRL.m_bNonStandard, 
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csLanternSchedule,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csLanternType,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csLanternLamp,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csRateCode,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csLanternCategory,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csAssetColumn,
		AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csAssetLantern,
		AcDb::kDxfXdInteger16,	 dlgIL.m_dlgRL.m_csaChargeTo.GetSize(),
		NULL
		);

	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
	for (int iCtr = 0; iCtr < dlgIL.m_dlgRL.m_csaChargeTo.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, dlgIL.m_dlgRL.m_csaChargeTo.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	if (acdbEntGet(enLight)) addXDataToEntity(enLight, rbpLight);
	acutRelRb(rbpLight);
	
	// Add the light number also as XDATA to the insert. This is required to modify the light table when lights are edited
	rbpLight = acutBuildList(AcDb::kDxfRegAppName, csLightNumber, AcDb::kDxfXdInteger16, 1, NULL);
	addXDataToEntity(enLight, rbpLight);	

	//////////////////////////////////////////////////////////////////////////
	// Process the light number block now
	//////////////////////////////////////////////////////////////////////////
	if (objLightNum.isValid())
	{
		// Get the attribute iterator to edit the attributes in the symbol
		if (acdbOpenObject(pInsert, objLightNum, AcDb::kForRead) != Acad::eOk) return FALSE;
		AcDbObjectIterator *pIterNum = pInsert->attributeIterator();
		pInsert->close();

		// Loop through the attributes
		for (pIterNum->start(); !pIterNum->done(); pIterNum->step())
		{
			// Get the attribute's object ID and open it for writing
			objAtt = pIterNum->objectId();
			if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

			// Get the attribute's tag
			csTag = pAtt->tag();

			// Set the attribute's value based on the tag
			/**/ if (!csTag.CompareNoCase(_T("SNUMBER"))) pAtt->setTextString(_T("SL") + dlgIL.m_csLanternNumber);
			else if (!csTag.CompareNoCase(_T("CNUMBER"))) pAtt->setTextString(dlgIL.m_csAssetColumn); 
			else if (!csTag.CompareNoCase(_T("LEGEND"))) { pAtt->setInvisible(Adesk::kTrue); }

			// Close the attribute
			pAtt->close();
		}

		// Add the handle of the light to the light number block
		rbpLight = acutBuildList(AcDb::kDxfRegAppName, XDATA_LIGHT_NUM, AcDb::kDxfXdHandle, szLightHandle, NULL);
		addXDataToEntity(enLightNum, rbpLight);
	}

	//////////////////////////////////////////////////////////////////////////
	// Process the replace block now
	//////////////////////////////////////////////////////////////////////////
	if (dlgIL.m_bReplace == TRUE) 
	{
		// Get the attribute iterator to edit the attributes in the symbol
		if (acdbOpenObject(pInsert, objRepLightNum, AcDb::kForRead) != Acad::eOk) return FALSE;
		AcDbObjectIterator *pIterRepNum = pInsert->attributeIterator();
		pInsert->close();

		// Loop through the attributes
		for (pIterRepNum->start(); !pIterRepNum->done(); pIterRepNum->step())
		{
			// Get the attribute's object ID and open it for writing
			objAtt = pIterRepNum->objectId();
			if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

			// Get the attribute's tag
			csTag = pAtt->tag();

			// Set the attribute's value based on the tag
			/**/ if (!csTag.CompareNoCase(_T("SNUMBER"))) pAtt->setTextString(_T("SL") + dlgIL.m_dlgRL.m_csAssetLantern);
			else if (!csTag.CompareNoCase(_T("CNUMBER"))) pAtt->setTextString(dlgIL.m_dlgRL.m_csAssetColumn); 
			else if (!csTag.CompareNoCase(_T("Legend"))) { pAtt->setInvisible(Adesk::kTrue); }

			// Close the attribute
			pAtt->close();
		}

		// Add the XDATA
		addXDataToEntity(enRepLightNum, rbpLight); 
	}

	// Release the buffer
	acutRelRb(rbpLight);

	// Success
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : InsertLightBlock
// Description  : Called to insert the light block, based on the values in the passed in dialog
// Arguments    : 1. [IN]  IE Light dialog instance.
//								2. [IN]  Light number to be assigned for the light
//								3. [IN]  Insertion point of the light block
//                4. [IN]  Insertion scale factor for the light block
//								5. [IN]  Rotation angle to insert the light block
//                6. [OUT] Object Id of the newly inserted light symbol
//								7. [IN]  XDATA's to be attached to the light. This will have a valid linked list when a light block is edited.  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InsertLightBlock(CLightDlg& dlgIL, CString csLightNumber, ads_point ptLightIns, double dLightScale, double dLightRotation, AcDbObjectId& objLight, struct resbuf *rbpXDExist)
{
	CString csTag;
	AcDbObjectId objAtt;
	AcDbAttribute *pAtt = NULL;
	AcDbBlockReference *pInsert = NULL;

	// Insert the light block with the given parameters
	/*ads_point ptLightInsNewPos;
	ptLightInsNewPos[X]=ptLightIns[X]+15;
	ptLightInsNewPos[Y]=ptLightIns[Y]+5;
	ptLightInsNewPos[Z]=ptLightIns[Z];*/

	if (insertBlock(dlgIL.m_csLanternSymbol, dlgIL.m_csLanternLayer, ptLightIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLight) == FALSE) return FALSE;
	//if (insertBlock(dlgIL.m_csLanternSymbol, dlgIL.m_csLanternLayer, ptLightInsNewPos, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLight) == FALSE) return FALSE;//Commented on 04-03-2014, since light or column should not jump only label should jump
	ChangeAttributeLocation(objLight, GetLightTag(_T("Lantern number")));

	//////////////////////////////////////////////////////////////////////////
	// Insert the light number block with the given parameters
	//////////////////////////////////////////////////////////////////////////
	AcDbObjectId objLightNum;
	ads_point ptLightNumIns; acutPolar(ptLightIns, DTR(45.0), 10 * dLightScale / 4, ptLightNumIns);
	// if (insertBlock(_T("SLANTERN_NUM"), _T("NOTES_SP_SL_EXT"), ptLightNumIns, dLightScale, atof(g_csPoleTextHeight), 0.0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;


	ads_point ptLightNumInsPos;
	ptLightNumInsPos[X]=ptLightNumIns[X]-15;
	ptLightNumInsPos[Y]=ptLightNumIns[Y]-5;
	ptLightNumInsPos[Z]=ptLightNumIns[Z];


	// Darren msg: March/April 09
	ads_name enRepLightNum;
	ACHAR szRepLightNumHandle[17]; 
	AcDbObjectId objRepLightNum;
	/**/ if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("NEW")))
	{
		//Changed on 08-Feb-2014 to keep the circle horizonatal always
		//if (insertBlock(_T("SLANTERN_NUM_NEW"), _T("NOTES_SL_EXT"), ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;

		//Commented on 04-03-2014, since light or column should not jump only label should jump
		//if (insertBlock(_T("SLANTERN_NUM_NEW"), _T("NOTES_SL_EXT"), ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
		
		//Commented on 13-03-2014, to change the layer for New Streetlight label
		//if (insertBlock(_T("SLANTERN_NUM_NEW"), _T("NOTES_SL_EXT"), ptLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
		if (insertBlock(_T("SLANTERN_NUM_NEW"), _T("Equip_SP_SL_NEW"), ptLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
	}
	else if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("EXISTING")))
	{
		//Changed on 08-Feb-2014 to keep the circle horizonatal always
		//if (insertBlock(_T("SLANTERN_NUM_EXT"), _T("NOTES_SL_EXT"), ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
		
		//Commented on 04-03-2014, since light or column should not jump only label should jump
		//if (insertBlock(_T("SLANTERN_NUM_EXT"), _T("NOTES_SL_EXT"), ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
		if (insertBlock(_T("SLANTERN_NUM_EXT"), _T("NOTES_SL_EXT"), ptLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
	}
	else if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("REPLACE")))
	{
		//Changed on 08-Feb-2014 to keep the circle horizonatal always
		//if (insertBlock(_T("SLANTERN_NUM"),dlgIL.m_csLanternLayer, ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;

		//Commented on 04-03-2014, since light or column should not jump only label should jump
		//if (insertBlock(_T("SLANTERN_NUM"),dlgIL.m_csLanternLayer, ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
		
		if (insertBlock(_T("SLANTERN_NUM"),dlgIL.m_csLanternLayer, ptLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), 0, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;

		

		//////////////////////////////////////////////////////////////////////////
		// If this is a replace lamp, one more symbol has to be inserted.
		//////////////////////////////////////////////////////////////////////////
		if (dlgIL.m_bReplace == TRUE)
		{
			ads_point ptRepLightNumIns; acutPolar(ptLightNumIns, 0.0, 16 * dLightScale / 10 , ptRepLightNumIns);

			ads_point ptRepLightNumInsPos;
	ptRepLightNumInsPos[X]=ptLightNumIns[X]+15;
	ptRepLightNumInsPos[Y]=ptLightNumIns[Y]+5;
	ptRepLightNumInsPos[Z]=ptLightNumIns[Z];

			//Commented on 13-03-2014, to change the block and position for replace
			//if (insertBlock(_T("SLANTERN_EXIST"), _T("NOTES_SL_EXT"), ptRepLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objRepLightNum) == FALSE) return FALSE;
			if (insertBlock(_T("SLANTERN_NUM_EXT"), _T("NOTES_SL_EXT"), ptRepLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objRepLightNum) == FALSE) return FALSE;
		}
	}
	//Commented on 04-03-2014, since light or column should not jump only label should jump
	//else if (insertBlock(_T("SLANTERN_NUM"), _T("NOTES_SL_EXT"), ptLightNumIns, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;
	else if (insertBlock(_T("SLANTERN_NUM"), _T("NOTES_SL_EXT"), ptLightNumInsPos, dLightScale, _tstof(g_csPoleTextHeight), dLightRotation, g_csPoleTextStyle, objLightNum) == FALSE) return FALSE;


	// Edit attributes in block
	return EditAttributesInLight(dlgIL, csLightNumber, ptLightIns, dLightScale, dLightRotation, objLight, objLightNum, objRepLightNum, rbpXDExist);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : InsertColumnBlock
// Description  : Called to insert the column block, based on the values in the passed in dialog.
// Arguments    : 1. [IN]  IE Light dialog instance.
//								2. [IN]  Lantern number to be assigned for the column
//								3. [IN]  Column number to be assigned for the column
//								4. [IN]  Insertion point of the column block
//                5. [IN]  Insertion scale factor for the column block
//								6. [IN]  Rotation angle to insert the column block
//                7. [OUT] Object Id of the newly inserted column symbol
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL InsertColumnBlock(CLightDlg& dlgIL, CString csLanternNumber, CString csColumnNumber, ads_point ptColumnIns, double dColumnScale, double dColumnRotation, AcDbObjectId& objColumn)
{
	// Insert the column block with the given parameters

	//Position value changed on 08-Feb-2014, for fixing the issue in visibility of light
	/*ads_point pColumn;
	pColumn[X] = ptColumnIns[X]+15;
	pColumn[Y] = ptColumnIns[Y]+5;
	pColumn[Z] = ptColumnIns[Z];*/

	if (insertBlock(dlgIL.m_csColumnSymbol, dlgIL.m_csColumnLayer, ptColumnIns, dColumnScale, _tstof(g_csPoleTextHeight), dColumnRotation, g_csPoleTextStyle, objColumn) == FALSE) return FALSE;
	//if (insertBlock(dlgIL.m_csColumnSymbol, dlgIL.m_csColumnLayer, pColumn, dColumnScale, _tstof(g_csPoleTextHeight), dColumnRotation, g_csPoleTextStyle, objColumn) == FALSE) return FALSE;//Commented on 04-03-2014, since light or column should not jump only label should jump

	// Relocate the column tag appropriately so that they don't overlap with the insert's graphics
	ChangeAttributeLocation(objColumn, GetColumnTag(_T("Column number")));

	CString csTag;
	AcDbObjectId objAtt;
	AcDbAttribute *pAtt;
	AcDbBlockReference *pInsert;

	// Open the newly inserted block, get the attribute iterator and close the inserted block
	if (acdbOpenObject(pInsert, objColumn, AcDb::kForWrite) != Acad::eOk) return FALSE;
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	AcDbHandle dbLightHandle; pInsert->getAcDbHandle(dbLightHandle);
	ACHAR szLightHandle[17]; dbLightHandle.getIntoAsciiBuffer(szLightHandle);
	pInsert->close();

	// Loop through the attributes and set attribute values for each
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		// Get the attribute's object ID and open it for writing
		objAtt = pIter->objectId();
		if (acdbOpenObject(pAtt, objAtt, AcDb::kForWrite) != Acad::eOk) continue;

		// Get the attribute's tag
		csTag = pAtt->tag();

		// Set the attribute's value based on the tag
		if (!csTag.CompareNoCase(GetColumnTag(_T("Lantern number"))))      pAtt->setTextString(csLanternNumber);
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Column number"))))      { if (dlgIL.m_bColumnReq) pAtt->setTextString(csColumnNumber); }
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Column new"))))         { if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("New")))     pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Column exist"))))       { if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("Exist")))   pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Column remove"))))      { if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("Remove")))  pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Column replace"))))     { if (!dlgIL.m_csLanternStatus.CompareNoCase(_T("Replace"))) pAtt->setTextString(_T("X")); }
		else if (!csTag.CompareNoCase(GetColumnTag(_T("Legend"))))					   { pAtt->setTextString(_T("")); }

		// Close the attribute
		pAtt->close();
	}

	// Get the entity name of the newly inserted column
	ads_name enColumn; acdbGetAdsName(enColumn, objColumn);

	// Add the XDATA to this column
	struct resbuf *rbpColumn = acutBuildList(AcDb::kDxfRegAppName, XDATA_COLUMN, AcDb::kDxfXdInteger16, _ttoi(csColumnNumber.Mid(1)), NULL);
	addXDataToEntity(enColumn, rbpColumn);

	// Cleanup
	acutRelRb(rbpColumn);

	// Success
	return TRUE;
}

/////////////////////////////////////////////////////
// Function name: GetBlockName()
// Description  :
// Arguments    :
// Called by    :
/////////////////////////////////////////////////////
CString GetBlockName(ads_name enLight)
{
	// Get the block name from the insert
	AcDbObjectId objInsert;
	acdbGetObjectId(objInsert, enLight);

	// Get the block reference pointer
	AcDbBlockReference *pInsert = NULL;
	if (acdbOpenObject(pInsert, objInsert, AcDb::kForRead) != Acad::eOk) return L"";
	AcDbObjectId objTblRcd = pInsert->blockTableRecord();
	pInsert->close();

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (acdbOpenObject(pBlkTblRcd, objTblRcd, AcDb::kForRead) != Acad::eOk) return L"";

	// Convert AcString to CString as some of our utility functions does not understand AcString
	AcString acsName; pBlkTblRcd->getName(acsName); 
	CString csName; csName.Format(_T("%s"), acsName.kTCharPtr());
	pBlkTblRcd->close();

	return csName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ConvertToLight
// Description  : Called when the user selects a block to edit it as a light. The validation of selection and
//                conversion of the same to a valid light insert from IE_Toolbox takes place here.
// Arguments    : 1. [IN] Entity name of an INSERT to convert to IE Light
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct resbuf *ConvertToLight(ads_name enLight)
{
	// Get the block name from the insert
	AcDbObjectId objInsert;
	acdbGetObjectId(objInsert, enLight);

	// Get the block reference pointer
	AcDbBlockReference *pInsert = NULL;
	if (acdbOpenObject(pInsert, objInsert, AcDb::kForRead) != Acad::eOk) return (struct resbuf *) NULL;
	AcDbObjectId objTblRcd = pInsert->blockTableRecord();

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (acdbOpenObject(pBlkTblRcd, objTblRcd, AcDb::kForRead) != Acad::eOk) { pInsert->close(); return (struct resbuf *) NULL; }

	// Convert AcString to CString as some of our utility functions does not understand AcString
	AcString acsName; pBlkTblRcd->getName(acsName); 
	CString csName; csName.Format(_T("%s"), acsName.kTCharPtr());
	pBlkTblRcd->close();

	// Check whether this is a valid light block from its name. Valid pole block names are retrieved from application MDB.
	int iIndexMatched;
	if (!CheckForDuplication(g_csaLanternBlocks, csName, iIndexMatched)) { pInsert->close(); return (struct resbuf *) NULL; }

	// Get the attributes attached to this insert
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	pInsert->close();

	// Change the location of the specified attribute.
	AcDbAttribute *pAtt = NULL;
	AcDbObjectId objAttId;

	CString csLightNumber, csLanternStatus, csLanternNumber, csLanternType, csLanternStockCode, csLanternLamp, csLampStockCode;
	CString csRateCode, csKingbolt, csMastType, csMastStockCode, csOutreachType, csOutreachStockCode;
	CString csBracketType, csBracketStockCode, csLanternCategory, csAssetColumn, csCharge1, csCharge1Precent;
	CString csCharge2, csCharge2Precent, csCharge3, csCharge3Precent;
	CString csTxtValue;
	CString csRLanternType, csRLanternLamp, csRRateCode, csRLanternCategory, csRCharge1, csRCharge1Precent;
	CString csRCharge2, csRCharge2Precent, csRCharge3, csRCharge3Precent;
	CStringArray csaChargeTo, csaRChargeTo;

	CString csRagBolt, csFoundationType, csMASTRATE, csOutreachRate, csBracketRate, csMountingHeight;

	// Get the status of this light from the symbol name
	int iIndex = -1;
	csLanternStatus = _T("New");
	if (CheckForDuplication(g_csaLanternBlocks, csName, iIndex)) csLanternStatus = g_csaLanternTypes.GetAt(iIndex);

	// Iterate through the attributes and get data for the light symbol
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		// Get the object Id of the attribute as we iterate and open the attribute for reading
		objAttId = pIter->objectId();
		if (acdbOpenObject(pAtt, objAttId, AcDb::kForRead) != Acad::eOk) continue;

		// Get the attribute value from the block 
		CString csTag; csTag.Format(_T("%s"), pAtt->tag());
		/**/ if (!csTag.CompareNoCase(GetLightTag(_T("Light number"))))    csLightNumber   = pAtt->textString(); 
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern number"))))  csLanternNumber = pAtt->textString(); 
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light new"))))  
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("New"); 
		}
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light existing"))))
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Existing"); 
		}
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light remove"))))       
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Remove"); 
		}
		else if (!csTag.CompareNoCase(GetLightTag(_T("Light replace"))))       
		{
			csTxtValue.Format(_T("%s"), pAtt->textString()); 
			csTxtValue = csTxtValue.Trim();
			if (!csTxtValue.CompareNoCase(_T("X"))) csLanternStatus = _T("Replace"); 
		}
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern type"))))         csLanternType       = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern stock code"))))   csLanternStockCode  = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lamp type"))))            csLanternLamp       = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lamp stock code"))))      csLampStockCode     = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Rate code"))))            csRateCode          = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Kingbolt height"))))      csKingbolt          = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Mast type"))))            csMastType          = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Mast stock code"))))      csMastStockCode     = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Outreach type"))))        csOutreachType      = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Outreach stock code"))))  csOutreachStockCode = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Bracket type"))))         csBracketType       = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Bracket stock code"))))   csBracketStockCode  = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Lantern category"))))     csLanternCategory   = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Column/pole number"))))   csAssetColumn       = pAtt->textString();

		else if(!csTag.CompareNoCase(GetLightTag(_T("RagBolt"))))  csRagBolt =pAtt->textString();
		else if(!csTag.CompareNoCase(GetLightTag(_T("FoundationType"))))  csFoundationType=pAtt->textString();
		else if(!csTag.CompareNoCase(GetLightTag(_T("MastRate"))))  csMASTRATE=pAtt->textString();
		else if(!csTag.CompareNoCase(GetLightTag(_T("OutReachRate"))))  csOutreachRate=pAtt->textString();
		else if(!csTag.CompareNoCase(GetLightTag(_T("BracketRate"))))  csBracketRate=pAtt->textString();
		else if(!csTag.CompareNoCase(GetLightTag(_T("Mounting Height"))))  csMountingHeight=pAtt->textString();

		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 1"))))             csCharge1           = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 1 percent"))))     csCharge1Precent    = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 2"))))             csCharge2           = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 2 percent"))))     csCharge2Precent    = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 3"))))             csCharge3           = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Charge 3 percent"))))     csCharge3Precent    = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lantern type"))))     csRLanternType     = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lamp type"))))        csRLanternLamp     = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Rate code"))))        csRRateCode        = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Lantern category")))) csRLanternCategory = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 1"))))         csRCharge1         = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 1 percent")))) csRCharge1Precent  = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 2"))))         csRCharge2         = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 2 percent")))) csRCharge2Precent  = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 3"))))         csRCharge3         = pAtt->textString();
		else if (!csTag.CompareNoCase(GetLightTag(_T("Replace Charge 3 percent")))) csRCharge3Precent  = pAtt->textString();

		// Close the attribute
		pAtt->close();
	}

	delete pIter;

	/*
	//////////////////////////////////////////////////////////////////////////
	// Check if any other light inserts has this lantern number assigned
	//////////////////////////////////////////////////////////////////////////
	CUIntArray uiaLightNumbers;  GetInsertNumbers(XDATA_LIGHT, uiaLightNumbers);
	int iLightNumber  = getMaxInArray (uiaLightNumbers)  + 1;
	if (!csLanternNumber.IsEmpty())
	{
	// Check if the light number is in the valid format
	csLanternNumber = csLanternNumber.MakeUpper();
	if ((csLanternNumber.Find("L") == -1) || !atoi((csLanternNumber.Mid(1))))
	{
	// acutPrintf("\nThe symbols lantern number \"%s\" is not in the format. Say L1,L2 etc.", csLanternNumber);
	// return false;
	csLanternNumber.Format("L%d", iLightNumber);
	}
	else if (CheckForDuplication(uiaLightNumbers, atoi(csLanternNumber.Mid(1))))
	{
	acutPrintf("\nDuplicate numbering of lantern number '%s' in drawing.", csLanternNumber);
	return false;
	}
	}
	else
	{
	// acutPrintf("\nAsigning lantern number '%d' in drawing.", iLightNumber + 1);
	csLanternNumber.Format("L%d", iLightNumber);
	}
	*/

	// Get the schedule for the lantern
	bool bNonStandard = false; // Assuming this always
	CString csLanternSchedule = _T("");
	if (!bNonStandard)
	{
		//	/**/ if (CheckForDuplication(g_csaLanternSch1Types, csRLanternType)) csRLanternSchedule = _T("Schedule 1");
		//else if (CheckForDuplication(g_csaLanternSch2Types, csRLanternType)) csRLanternSchedule = _T("Schedule 2");
		/**/ if (CheckForDuplication(g_csaLanternSch1Types, csLanternType)) csLanternSchedule =_T("1"); //_T("Schedule 1");
		else if (CheckForDuplication(g_csaLanternSch2Types, csLanternType)) csLanternSchedule = _T("2");// _T("Schedule 2");
		else if (CheckForDuplication(g_csaLanternSch3Types, csLanternType)) csLanternSchedule = _T("3");
		else if (CheckForDuplication(g_csaLanternSch4Types, csLanternType)) csLanternSchedule = _T("4");
		else if (CheckForDuplication(g_csaLanternSch5Types, csLanternType)) csLanternSchedule = _T("5");
		else if (CheckForDuplication(g_csaLanternSchUnRegulatedTypes, csLanternType)) csLanternSchedule = _T("UnRegulated");
	}

	// Get the schedule for the lantern for replaced types
	bool bRNonStandard = false; // Assuming this always
	CString csRLanternSchedule = _T("");
	if (!bRNonStandard)
	{
		//	/**/ if (CheckForDuplication(g_csaLanternSch1Types, csRLanternType)) csRLanternSchedule = _T("Schedule 1");
		//else if (CheckForDuplication(g_csaLanternSch2Types, csRLanternType)) csRLanternSchedule = _T("Schedule 2");
		if (CheckForDuplication(g_csaLanternSch1Types, csLanternType)) csLanternSchedule =_T("1"); //_T("Schedule 1");
		else if (CheckForDuplication(g_csaLanternSch2Types, csLanternType)) csLanternSchedule = _T("2");// _T("Schedule 2");
		else if (CheckForDuplication(g_csaLanternSch3Types, csLanternType)) csLanternSchedule = _T("3");
		else if (CheckForDuplication(g_csaLanternSch4Types, csLanternType)) csLanternSchedule = _T("4");
		else if (CheckForDuplication(g_csaLanternSch5Types, csLanternType)) csLanternSchedule = _T("5");
		else if (CheckForDuplication(g_csaLanternSchUnRegulatedTypes, csLanternType)) csLanternSchedule = _T("UnRegulated");
	}

	// Add information to Charges array
	CString csFormat;
	if (!csCharge1.IsEmpty()) { csFormat.Format(_T("%s#%s"), csCharge1, csCharge1Precent); csaChargeTo.Add(csFormat); }
	if (!csCharge2.IsEmpty()) { csFormat.Format(_T("%s#%s"), csCharge2, csCharge2Precent); csaChargeTo.Add(csFormat); }
	if (!csCharge3.IsEmpty()) { csFormat.Format(_T("%s#%s"), csCharge3, csCharge3Precent); csaChargeTo.Add(csFormat); }

	// Add information to Replace Charges array
	if (!csRCharge1.IsEmpty()) { csFormat.Format(_T("%s#%s"), csRCharge1, csRCharge1Precent); csaRChargeTo.Add(csFormat); }
	if (!csRCharge2.IsEmpty()) { csFormat.Format(_T("%s#%s"), csRCharge2, csRCharge2Precent); csaRChargeTo.Add(csFormat); }
	if (!csRCharge3.IsEmpty()) { csFormat.Format(_T("%s#%s"), csRCharge3, csRCharge3Precent); csaRChargeTo.Add(csFormat); }

	//////////////////////////////////////////////////////////////////////////
	// Build the XDATA to attach to this light
	//////////////////////////////////////////////////////////////////////////
	/*
	("IE_TB_LIGHT" (1005 . "0") (1005 . "0") (1070 . 894) (1000 
	. "L894") (1070 . 0) (1000 . "") (1000 . "Replace") (1000 . "") (1000 . "") 
	(1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . 
	"") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") 
	(1000 . "") (1000 . "") (1000 . "") (1070 . 0) (1070 . 0) (1000 . "") (1000 . 
	"") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1000 . "") (1070 . 0)) 
	*/
	struct resbuf *rbpLight = acutBuildList(AcDb::kDxfRegAppName, XDATA_LIGHT, 
		AcDb::kDxfXdHandle,      _T(""),
		AcDb::kDxfXdHandle,      _T(""),
		AcDb::kDxfXdInteger16,   _ttoi(csLightNumber.Mid(1)), 
		AcDb::kDxfXdAsciiString, csLightNumber,
		AcDb::kDxfXdInteger16,   bNonStandard, 
		AcDb::kDxfXdAsciiString, csLanternSchedule,
		AcDb::kDxfXdAsciiString, csLanternStatus,
		AcDb::kDxfXdAsciiString, csLanternType,
		AcDb::kDxfXdAsciiString, csLanternStockCode,
		AcDb::kDxfXdAsciiString, csLanternLamp,
		AcDb::kDxfXdAsciiString, csLampStockCode,
		AcDb::kDxfXdAsciiString, csRateCode,
		AcDb::kDxfXdAsciiString, csKingbolt,
		AcDb::kDxfXdAsciiString, csMastType,
		AcDb::kDxfXdAsciiString, csMastStockCode,
		AcDb::kDxfXdAsciiString, csOutreachType,
		AcDb::kDxfXdAsciiString, csOutreachStockCode,
		AcDb::kDxfXdAsciiString, csBracketType,
		AcDb::kDxfXdAsciiString, csBracketStockCode,
		AcDb::kDxfXdAsciiString, _T(""),
		AcDb::kDxfXdAsciiString, csLanternCategory,
		AcDb::kDxfXdAsciiString, csAssetColumn,
		AcDb::kDxfXdAsciiString, _T(""),
		AcDb::kDxfXdAsciiString, _T(""),
		AcDb::kDxfXdAsciiString, _T(""),

		AcDb::kDxfXdAsciiString, csRagBolt,
		AcDb::kDxfXdAsciiString, csFoundationType,
		AcDb::kDxfXdAsciiString, csMASTRATE,
		AcDb::kDxfXdAsciiString, csOutreachRate,
		AcDb::kDxfXdAsciiString, csBracketRate,	
		AcDb::kDxfXdAsciiString, csMountingHeight,

		AcDb::kDxfXdInteger16,	 csaChargeTo.GetSize(), NULL);

	struct resbuf *rbpTemp;
	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

	for (int iCtr = 0; iCtr < csaChargeTo.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaChargeTo.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

	rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdInteger16,   bRNonStandard, 
		AcDb::kDxfXdAsciiString, csRLanternSchedule,
		AcDb::kDxfXdAsciiString, csRLanternType,
		AcDb::kDxfXdAsciiString, csRLanternLamp,
		AcDb::kDxfXdAsciiString, csRRateCode,
		AcDb::kDxfXdAsciiString, csRLanternCategory,
		AcDb::kDxfXdAsciiString, _T(""),
		AcDb::kDxfXdAsciiString, _T(""),
		AcDb::kDxfXdInteger16,	 csaRChargeTo.GetSize(),
		NULL
		);

	for (rbpTemp = rbpLight; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);

	for (int iCtr = 0; iCtr < csaRChargeTo.GetSize(); iCtr++)
	{
		rbpTemp->rbnext = acutBuildList(AcDb::kDxfXdAsciiString, csaRChargeTo.GetAt(iCtr), NULL);
		rbpTemp = rbpTemp->rbnext;
	}

	//////////////////////////////////////////////////////////////////////////
	// Add the light XDATA to the block. This marks the conversion to IE Light
	//////////////////////////////////////////////////////////////////////////
	addXDataToEntity(enLight, rbpLight);

	//////////////////////////////////////////////////////////////////////////
	// Add the lantern number XDATA to the converted block 
	// (This is required for table generation)
	//////////////////////////////////////////////////////////////////////////
	struct resbuf *rbpXD = acutBuildList(AcDb::kDxfRegAppName, csLightNumber, AcDb::kDxfXdInteger16, 1, NULL);
	addXDataToEntity(enLight, rbpXD); acutRelRb(rbpXD);

	// All is well so return the light XDATA
	return rbpLight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_InsertLight
// Description  : Called when the user enters the "IE_AddLight" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_InsertLight()
{
	// Get existing pole number array
	CUIntArray uiaLightNumbers;  GetInsertNumbers(XDATA_LIGHT,  uiaLightNumbers);	  int iLightNumber  = getMaxInArray (uiaLightNumbers)  + 1;
	CUIntArray uiaColumnNumbers; GetInsertNumbers(XDATA_COLUMN, uiaColumnNumbers);	int iColumnNumber = getMaxInArray (uiaColumnNumbers) + 1;

	//Display the light information dialog
	CLightDlg dlgIL;
	dlgIL.m_uiaLightNumbers.Copy(uiaLightNumbers);
	dlgIL.m_bEditing  = FALSE;
	dlgIL.m_bFirstRun = TRUE;

	// Get the Lantern details from database
	CQueryTbl tblLantern;
	CString csSQL;
	csSQL.Format(L"SELECT [LanternType], [LanternBlock], [LanternLayer], [LanternLayerColor] FROM [tblLanternStatus] WHERE [LanternBlock] LIKE '%s%%' ORDER BY [ID]", L"SL_LANTERN");
	if (!tblLantern.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, __FUNCTION__)) return;

	tblLantern.GetColumnAt(0, dlgIL.m_csaLanternTypes);
	tblLantern.GetColumnAt(1, dlgIL.m_csaLanternBlocks);
	tblLantern.GetColumnAt(2, dlgIL.m_csaLanternLayers);
	tblLantern.GetColumnAt(3, dlgIL.m_csaLanternLayerColors);

	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	// Create the layer if it is not there (If NOT there set the COLOR to YELLOW!)
	AcDbLayerTable *pLayerTbl;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	bool bLayerExists = true;
	if (!pLayerTbl->has(_T("NOTES_SL_EXT"))) bLayerExists = false; 
	pLayerTbl->close();

	// Do until user cancels
	while (TRUE)
	{
		// Display the light information dialog
		// dlgIL.m_csLightNumber.Format(_T("L%d"), iLightNumber); 
		if (dlgIL.DoModal() == IDCANCEL) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

		// Modification on 22.04.08 by mail
		// The layer name for the symbol must be "NOTES_SP_SL_EXT" hard coded. Earlier this was placed on "IEC_Overhead"
		// createLayer(_T("NOTES_SP_SL_EXT"), Adesk::kFalse, Adesk::kFalse);
		// Modification on 24.10.08 by mail

		// The layer name for the symbol must be "NOTES_SL_EXT" hard coded. Earlier this was placed on "IEC_Overhead"
		createLayer(_T("NOTES_SL_EXT"), Adesk::kFalse, Adesk::kFalse);
		if (!bLayerExists) setLayerColor(_T("NOTES_SL_EXT"), 2); // 03.08.12

		// Set the first run flag 
		dlgIL.m_bFirstRun = FALSE;

		int iRet, iCtr = 0;
		ads_name enLight;
		ads_point ptLightIns;
		CString csLightNumber, csColumnNumber;
		AcDbObjectId objLight, objColumn;
		double dRotAngle = 0.0;
		CString csRotation = L"0";
		int iRetROT;

		// Do until user returns or cancels
		while (TRUE)
		{
			csColumnNumber.Format(_T("C%d"), iColumnNumber);

			// Use the previous point as base point, for second point onwards
			iRet = acedGetPoint((iCtr ? ptLightIns : NULL), _T("\nPick street light location (ENTER to return, ESC to cancel): "), ptLightIns);

			// If user wants to return to dialog
			if (iRet == RTNONE) break;

			// If user wants to cancel
			if (iRet != RTNORM) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

			// Get the rotation angle
			csRotation.Format(L"%s", suppressZero(RTD(dRotAngle)));
			iRetROT = acedGetAngle(NULL, L"\nRotation angle <" + csRotation + L">: ", &dRotAngle);
			/**/ if (iRetROT != RTNORM) dRotAngle = DTR(_tstof(csRotation));

			// Insert the column block (if required)
			if (dlgIL.m_bColumnReq) 
			{
				// Ensure that the layer for the column block is create and is current     
				createLayer(dlgIL.m_csColumnLayer, Adesk::kFalse); 

				// Set the layer color
				setLayerColor(dlgIL.m_csColumnLayer, _ttoi(dlgIL.m_csColumnLayerColor));

				// Insert it here
				if (InsertColumnBlock(dlgIL, dlgIL.m_csLightNumber, csColumnNumber, ptLightIns, _tstof(g_csPoleDiameter), dRotAngle, objColumn) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

				// Get its handle
				AcDbHandle dbColumnHandle = objColumn.handle();
				dbColumnHandle.getIntoAsciiBuffer(dlgIL.m_szColumnHandle);
				iColumnNumber++;

				// Calculate the new point for light block
				// acutPolar(ptLightIns, MinusPIby2, 0.48 * _tstof(g_csPoleDiameter), ptLightIns);
			}

			// Ensure that the layer for this block is created and current
			createLayer(dlgIL.m_csLanternLayer, Adesk::kFalse);

			// Set the layer color
			setLayerColor(dlgIL.m_csLanternLayer, _ttoi(dlgIL.m_csLanternLayerColor));

			// Insert the light block at the given point
			if (InsertLightBlock(dlgIL, dlgIL.m_csLightNumber, ptLightIns, _tstof(g_csPoleDiameter), dRotAngle, objLight, NULL) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

			// Increment the counters
			iCtr++; 
			dlgIL.m_uiaLightNumbers.Add(_ttoi(dlgIL.m_csLightNumber.Mid(1)));
			// iLightNumber++;

			// Check if the next light number in series already exists in the drawing
			if (CheckForDuplication(dlgIL.m_uiaLightNumbers, _ttoi(dlgIL.m_csLightNumber.Mid(1)) + 1))
			{
				acutPrintf(_T("\nLight number \"%d\" already exists in the drawing."), _ttoi(dlgIL.m_csLightNumber.Mid(0, 1)) + 1);
				acedSetVar(_T("CLAYER"), &rbCLayer);
				return;
			}

			// Set the next light number to the next in series
			dlgIL.m_csLightNumber.Format(_T("L%d"), _ttoi(dlgIL.m_csLightNumber.Mid(1)) + 1);
		}
	}

	// Set the layer that was current prior to this command
	acedSetVar(_T("CLAYER"), &rbCLayer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_EditLight
// Description  : Called when the user enters the "IE_EditLight" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_EditLight()
{
	ads_name enLight;
	ads_point ptDummy;
	struct resbuf *rbpLight = NULL;

	// Do until user selects a light or cancels
	while (TRUE)
	{
		// Ask user to select the light to be edited
		acedInitGet(RSG_NONULL, _T(""));
		if (acedEntSel(_T("\nSelect street light to edit:"), enLight, ptDummy) == RTCAN) return;

		// Check if what has been selected is a valid IE Light
		if ((rbpLight = getXDataFromEntity(enLight, XDATA_LIGHT)) == NULL) 
		{
			//////////////////////////////////////////////////////////////////////////
			// This is not a light added by "Insert_Light" of IE_Toolbox, so check if
			// its block name matches any of the light blocks in the standard database. 
			// If it matches, add XDATA_LIGHT XDATA to it recognize the same as an insert
			// from IE_Toolbox.
			//////////////////////////////////////////////////////////////////////////
			rbpLight = ConvertToLight(enLight);

			// The selected symbol did not match any of the valid light symbols
			if (rbpLight == NULL) { acutPrintf(_T("\nThat is not a valid light.\n")); return; }
			break;
		}
		else { break; }
	}

	// Apply change request 94
	ApplyChangeRequest94ForLights(enLight);
	rbpLight = getXDataFromEntity(enLight, XDATA_LIGHT);
	if (!rbpLight) return;

	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	//////////////////////////////////////////////////////////////////////////
	// Get the light information from the XDATA buffer and show the dialog
	//////////////////////////////////////////////////////////////////////////
	CLightDlg dlgIL;

	// Get the block name of the selected insert
	CString csSymbolName = GetBlockName(enLight);
	csSymbolName = csSymbolName.Mid(0, csSymbolName.ReverseFind(L'_'));

	// Get the Lantern details from database
	CQueryTbl tblLantern;
	CString csSQL;
	csSQL.Format(L"SELECT [LanternType], [LanternBlock], [LanternLayer], [LanternLayerColor] FROM [tblLanternStatus] WHERE [LanternBlock] LIKE '%s%%' ORDER BY [ID]", csSymbolName);
	if (!tblLantern.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, __FUNCTION__)) { acutRelRb(rbpLight); return; }

	tblLantern.GetColumnAt(0, dlgIL.m_csaLanternTypes);
	tblLantern.GetColumnAt(1, dlgIL.m_csaLanternBlocks);
	tblLantern.GetColumnAt(2, dlgIL.m_csaLanternLayers);
	tblLantern.GetColumnAt(3, dlgIL.m_csaLanternLayerColors);

	rbpLight = rbpLight->rbnext;
	CString csLightNumHandle          = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1005 . "4B0CF")
	CString csRepLightNumHandle       = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1005 . "0")
	int iLightNumber                  = rbpLight->resval.rint;    rbpLight = rbpLight->rbnext; // (1070 . 4)
	dlgIL.m_csLightNumber             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "L4")
	CString csExistingLightNumber			= dlgIL.m_csLightNumber;

	dlgIL.m_bNonStandard              = rbpLight->resval.rint;    rbpLight = rbpLight->rbnext; // (1070 . 0)
	dlgIL.m_csLanternSchedule         = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "Schedule 1")
	dlgIL.m_csLanternStatus           = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "Existing")
	dlgIL.m_csLanternType             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternStockCode        = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternLamp             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLampStockCode           = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csRateCode                = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csKingbolt                = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternMast             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csMastCode                = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternOutreach         = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csOutreachCode            = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternBracket          = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csBracketCode             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternLocation         = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternCategory         = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")

	dlgIL.m_csAssetColumn             = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternNumber           = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_csLanternColumn           = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "Existing")
	_tcscpy(dlgIL.m_szColumnHandle, rbpLight->resval.rstring);     rbpLight = rbpLight->rbnext; // (1000 . "4B0C2")


	dlgIL.m_csRagBolt    = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;
	dlgIL.m_csFoundationType    = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;
	dlgIL.m_csMASTRATE   = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;
	dlgIL.m_csOutreachRate   = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;
	dlgIL.m_csBracketRate   = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;
	dlgIL.m_csMountingHeight = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext;


	// Append to charge array
	CString csChargeToItem;
	int iSizeOfChargeToArray					= rbpLight->resval.rint;    rbpLight = rbpLight->rbnext; // (1070 . 0)
	for (int iCtr = 0; iCtr < iSizeOfChargeToArray; iCtr++)
	{
		dlgIL.m_csaChargeTo.Add(rbpLight->resval.rstring); rbpLight = rbpLight->rbnext; 
	}

	dlgIL.m_dlgRL.m_bNonStandard      = rbpLight->resval.rint;    rbpLight = rbpLight->rbnext; // (1070 . 0)
	dlgIL.m_dlgRL.m_csLanternSchedule = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "Schedule 1")
	dlgIL.m_dlgRL.m_csLanternType     = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_dlgRL.m_csLanternLamp     = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_dlgRL.m_csRateCode        = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_dlgRL.m_csLanternCategory = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_dlgRL.m_csAssetColumn     = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")
	dlgIL.m_dlgRL.m_csAssetLantern    = rbpLight->resval.rstring; rbpLight = rbpLight->rbnext; // (1000 . "")

	// Append to charge array
	iSizeOfChargeToArray		    			= rbpLight->resval.rint;    rbpLight = rbpLight->rbnext; // (1070 . 0)
	for (int iCtr = 0; iCtr < iSizeOfChargeToArray; iCtr++)
	{
		dlgIL.m_dlgRL.m_csaChargeTo.Add(rbpLight->resval.rstring); rbpLight = rbpLight->rbnext;
	}

	// if (rbpLight) acutRelRb(rbpLight);

	dlgIL.m_bFirstRun = FALSE;
	dlgIL.m_bEditing  = TRUE;

	// Get existing pole number array
	int iMaxNumber;
	CUIntArray uiaLightNumbers; GetInsertNumbers(XDATA_LIGHT, uiaLightNumbers);

	// Call the pole dialog for inputs
	dlgIL.m_uiaLightNumbers.Copy(uiaLightNumbers);

	// Display the light dialog for inputs
	if (dlgIL.DoModal() == IDCANCEL) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	//////////////////////////////////////////////////////////////////////////
	// Create the layer to place street light number block.
	//////////////////////////////////////////////////////////////////////////
	// Modification as  mail on 22.04.08
	// The layer name for the symbol must be "NOTES_SP_SL_EXT" hard coded.". Earlier it was hard coded as "IEC_Overhead".
	// createLayer(_T("NOTES_SP_SL_EXT"), Adesk::kFalse, Adesk::kFalse);
	// Modification on 24.10.08 by mail
	// The layer name for the symbol must be "NOTES_SL_EXT" hard coded. Earlier this was placed on "IEC_Overhead"
	createLayer(_T("NOTES_SL_EXT"), Adesk::kFalse, Adesk::kFalse);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If the light symbol/status is different from what is edited, a new light is inserted with the new XDATA. Else, the XDATA attached 
	// to the pole is edited without erasing the existing insert. This is because, there could be XDATA's not concerning IE_TB that may 
	// be attached to the light symbol edited.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Get the object id from the selected entity name
	AcDbObjectId objLight; 
	if (acdbGetObjectId(objLight, enLight) != Acad::eOk) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	// Open the INSERT and get present position, scale and rotation angle, so that the new symbol can be placed with these parameters
	AcDbBlockReference *pLight; 
	if (acdbOpenObject(pLight, objLight, AcDb::kForWrite) != Acad::eOk) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	// Get the insertion point, scale and rotation angle of the Light
	AcGePoint3d geIns = pLight->position();
	double dScale     = pLight->scaleFactors().sx;
	double dRotation  = pLight->rotation();

	// Get all the XDATA that is prevalent in the light edited.
	struct resbuf *rbpXDExist = pLight->xData(NULL);

	// Get the object ID of light number block
	ads_name enLightNum; 
	AcDbObjectId objLightNum;
	if (acdbHandEnt(csLightNumHandle, enLightNum) == RTNORM) { acdbGetObjectId(objLightNum, enLightNum); }

	// Get the object ID of replace light number block
	ads_name enRepLightNum; 
	AcDbObjectId objRepLightNum;
	if (acdbHandEnt(csRepLightNumHandle, enRepLightNum) == RTNORM) { acdbGetObjectId(objRepLightNum, enRepLightNum); }

	// If the symbol name changes, a new insert has to be drawn for the new symbol. Else edit the XDATA of the light symbol being edited.
	if (!dlgIL.m_csExistingLanternSymbol.CompareNoCase(dlgIL.m_csLanternSymbol))
	{
		pLight->close();

		// Get the object id of the light selected to edit
		acdbGetObjectId(objLight, enLight);

		// Get the object id of the light number selected to edit
		acdbGetObjectId(objLight, enLight);

		// Simply edit the attributes in the symbol selected and its XDATA
		if (EditAttributesInLight(dlgIL, dlgIL.m_csLightNumber, asDblArray(geIns), dScale, dRotation, objLight, objLightNum, objRepLightNum, rbpXDExist) == FALSE) 
		{
			acedSetVar(_T("CLAYER"), &rbCLayer); 
			return; 
		}
	}
	else
	{
		// Erase this Light
		pLight->erase();
		pLight->close();

		// Erase the light number block
		if (objLightNum.isValid())
		{
			AcDbBlockReference *pLightNum;
			if (acdbOpenObject(pLightNum, objLightNum, AcDb::kForWrite) == Acad::eOk)
			{
				pLightNum->erase();
				pLightNum->close();
			} 
		}

		// Erase the replace light number
		if (objRepLightNum.isValid())
		{	
			AcDbBlockReference *pRepLightNum;
			if (acdbOpenObject(pRepLightNum, objRepLightNum, AcDb::kForWrite) == Acad::eOk)
			{
				pRepLightNum->erase();
				pRepLightNum->close();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Erase the column number block that is edited
		//////////////////////////////////////////////////////////////////////////
		ads_name enColumn; 
		AcDbObjectId objColumn;
		CString csColumnNumber; csColumnNumber.Empty();
		if (acdbHandEnt(dlgIL.m_szColumnHandle, enColumn) == RTNORM)
		{
			if (acdbGetObjectId(objColumn, enColumn) == Acad::eOk)
			{
				AcDbBlockReference *pColumn;
				if (acdbOpenObject(pColumn, objColumn, AcDb::kForWrite) == Acad::eOk)
				{
					struct resbuf *rbpColumnXD = pColumn->xData(XDATA_COLUMN);
					if (rbpColumnXD != NULL) csColumnNumber.Format(_T("C%d"), rbpColumnXD->rbnext->resval.rint);

					pColumn->erase();
					pColumn->close();
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// If column is required, insert the column block
		//////////////////////////////////////////////////////////////////////////
		ads_point ptLightIns; acutPolar(asDblArray(geIns), 0.0, 0.0, ptLightIns);
		if (dlgIL.m_bColumnReq)
		{
			// Ensure that the layer for the column block is create and is current     
			createLayer(dlgIL.m_csColumnLayerColor, Adesk::kFalse);

			// Set the layer color
			setLayerColor(dlgIL.m_csColumnLayer, _ttoi(dlgIL.m_csColumnLayerColor));

			if (InsertColumnBlock(dlgIL, dlgIL.m_csLightNumber, csColumnNumber, ptLightIns, dScale, dRotation, objColumn) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

			// Get its handle
			AcDbHandle dbColumnHandle = objColumn.handle();
			dbColumnHandle.getIntoAsciiBuffer(dlgIL.m_szColumnHandle);

			// Calculate the new point for light block
			acutPolar(ptLightIns, MinusPIby2, 0.48 * _tstof(g_csPoleDiameter), ptLightIns);
		}

		// Ensure that the layer for this block is created and current. Also set its BYLAYER color too,
		createLayer(dlgIL.m_csLanternLayer, Adesk::kFalse);
		setLayerColor(dlgIL.m_csLanternLayer, _ttoi(dlgIL.m_csLanternLayerColor));

		// Replace it with the new light block at the given point (it could be the same symbol but let's do it anyway)
		if (InsertLightBlock(dlgIL, dlgIL.m_csLightNumber, ptLightIns, dScale, dRotation, objLight, rbpXDExist) == FALSE) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }
	}

	// Modify the light schedule table if it is already generated
	// ModifyLightTable(objLight, suppressZero(iLightNumber));

	//Commented since this crashes autocad when ragbolt modified.. //19-03-2014
	//ModifyLightTable(objLight, csExistingLightNumber.Mid(1));

	// Restore the layer that was present prior to execution of this command
	acedSetVar(_T("CLAYER"), &rbCLayer);
} // rbpLight

//////////////////////////////////////////////////////////////////////////////////////
// Function name: Command_ConvertLight()
// Description  : Converts all the valid manually placed light inserts to IE Light
// Called from  : acrxEntryPoint()
//////////////////////////////////////////////////////////////////////////////////////
void Command_ConvertLight()
{
	struct resbuf *rbpFilter = NULL;

	// Exit if if there are no any valid light symbols in the database
	if (g_csaLanternBlocks.GetSize() <= 0) return;

	// Define a selection set filter of all valid light insert names
	if (g_csaLanternBlocks.GetSize() == 1)
	{
		// There is only one valid light symbol
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), 2, g_csaLanternBlocks.GetAt(0), NULL);
	}
	else
	{
		// There are multiple valid light symbol's. Build them all in the filter list.
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), -4, _T("<NOT"), AcDb::kDxfRegAppName, XDATA_LIGHT, -4, _T("NOT>"), -4, _T("<OR"), NULL);

		struct resbuf *rbpTemp = NULL;
		for (int iCtr = 0; iCtr < g_csaLanternBlocks.GetSize(); iCtr++)
		{
			for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
			rbpTemp->rbnext = acutBuildList(2, g_csaLanternBlocks.GetAt(iCtr), NULL);
		}

		for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);
	}

	// Make the selection set of all INSERTS that has a valid light symbol name
	ads_name ssGet; if (acedSSGet(_T("X"), NULL, NULL, rbpFilter, ssGet) != RTNORM) { acedSSFree(ssGet); acutRelRb(rbpFilter); return; }

	// Cleanup
	acutRelRb(rbpFilter);

	//////////////////////////////////////////////////////////////////////////
	// For each INSERT in the selection set call the convert light function
	//////////////////////////////////////////////////////////////////////////
	ads_name enEntity;

	// Get the number of INSERTS that matched the criteria 
	Adesk::Int32 lLength = 0L; acedSSLength(ssGet, &lLength);

	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the entity name from the selection set
		acedSSName(ssGet, lCtr, enEntity);

		// Function that does the conversion
		ConvertToLight(enEntity); 
	}

	// Cleanup
	acedSSFree(ssGet);
}

//////////////////////////////////////////////////////////////////////////
// Utility function: For Developer use
// Description     : Removes all LIGHT XDATA from inserts in the drawing.
//                   Rename this to "ConvertToLight()" before use.
//////////////////////////////////////////////////////////////////////////
void removeAllXDATAForLight()
{
	ads_name ssLight;
	struct resbuf *rbpFilt = acutBuildList(AcDb::kDxfRegAppName, XDATA_LIGHT, NULL);
	struct resbuf *rbpGet;
	CString csLNo;
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssLight) == RTNORM)
	{
		Adesk::Int32 lLength = 0L; acedSSLength(ssLight, &lLength);
		ads_name enLight;
		for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
		{
			acedSSName(ssLight, lCtr, enLight);
			rbpGet = getXDataFromEntity(enLight, XDATA_LIGHT);
			addXDataToEntity(enLight, rbpFilt);

			// Remove all numbered XDATA
			for (int iNo = 0; iNo < 100; iNo++)
			{
				csLNo.Format(_T("L%d"), iNo);
				if (getXDataFromEntity(enLight, csLNo))
				{
					struct resbuf *rbpLNo = acutBuildList(AcDb::kDxfRegAppName, csLNo, NULL);
					addXDataToEntity(enLight, rbpLNo);
					acutRelRb(rbpLNo);
				}
			}
		}
	}

	acedSSFree(ssLight);
	acutRelRb(rbpFilt);
}

