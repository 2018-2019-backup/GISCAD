////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : TableColumnsDlg.cpp
// Created          : 4th May 2007
// Description      : Output column selection dialog implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "OptionsDlg.h" 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Pole Information dialog
IMPLEMENT_DYNAMIC(COptionsDlg, CDialog) 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::COptionsDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)	: CDialog(COptionsDlg::IDD, pParent) { }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::~COptionsDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
COptionsDlg::~COptionsDlg() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  // Controls
  DDX_Control(pDX, IDC_POLE_TEXT_STYLE,  m_cbPoleStyle);
  DDX_Control(pDX, IDC_POLE_TEXT_HEIGHT, m_edPoleTextHeight);
  DDX_Control(pDX, IDC_POLE_DIAMETER,    m_edPoleDia);
  DDX_Control(pDX, IDC_SCH_TEXT_STYLE,   m_cbSchStyle);
  DDX_Control(pDX, IDC_SCH_TEXT_HEIGHT,  m_edSchTextHeight);

  // Variables
  DDX_Text(pDX, IDC_POLE_TEXT_STYLE,  m_csPoleStyle);
  DDX_Text(pDX, IDC_POLE_TEXT_HEIGHT, m_csPoleTextHeight);
  DDX_Text(pDX, IDC_POLE_DIAMETER,    m_csPoleDia);
  DDX_Text(pDX, IDC_SCH_TEXT_STYLE,   m_csSchStyle);
  DDX_Text(pDX, IDC_SCH_TEXT_HEIGHT,  m_csSchTextHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDHELP, OnHelp)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL COptionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Get the text table of the drawing and create a new iterator
  AcDbTextStyleTable *pTST; acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTST, AcDb::kForRead);
  AcDbTextStyleTableIterator *pIter; pTST->newIterator(pIter);
  pTST->close();

  // Loop through the iterator
  for( ; !pIter->done(); pIter->step())
  {
    // Get the text style table record
    AcDbTextStyleTableRecord* pTSTR; pIter->getRecord(pTSTR, AcDb::kForRead);
    ACHAR *pTSName; pTSTR->getName(pTSName);
    pTSTR->close();
    CString csTSName = pTSName;

		if (!csTSName.IsEmpty())
		{
			m_cbPoleStyle.AddString(csTSName);
			m_cbSchStyle.AddString(csTSName);
		}
  }

	// Retrieve the current values for the drawings XRECORD
	struct resbuf *rbpOptions = getXRecordFromDictionary(DICT_IETB, XREC_IETB);
	if (rbpOptions != NULL)
	{
		g_csPoleTextStyle  = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csPoleTextHeight = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csPoleDiameter   = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csSchTextStyle   = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csSchTextHeight  = rbpOptions->resval.rstring;
	}
	else
	{
		// If the XRECRORD is not there, take it from the MDB's
		CQueryTbl tblData;
		CString csSQL;
		csSQL.Format(_T("SELECT [PNTextStyle], [PNTextHeight], [PoleDia], [SchTextStyle], [SchTextHeight] FROM [tblConfig]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, _T("COptionsDlg::OnInitDialog()"))) return FALSE;
		g_csPoleTextStyle  = tblData.GetRowAt(0)->GetAt(0);
		g_csPoleTextHeight = tblData.GetRowAt(0)->GetAt(1);
		g_csPoleDiameter   = tblData.GetRowAt(0)->GetAt(2);
		g_csSchTextStyle   = tblData.GetRowAt(0)->GetAt(3);
		g_csSchTextHeight  = tblData.GetRowAt(0)->GetAt(4);
	}

  // Show the current values
	if (!g_csPoleTextStyle.IsEmpty()) 
	{
		if (m_cbPoleStyle.SelectString(-1, g_csPoleTextStyle) == CB_ERR) m_cbPoleStyle.SetCurSel(0);
	}
  m_edPoleTextHeight.SetWindowText(g_csPoleTextHeight);
  m_edPoleDia.SetWindowText(g_csPoleDiameter);

	if (!g_csSchTextStyle.IsEmpty())
	{
		if (m_cbSchStyle.SelectString(-1, g_csSchTextStyle) == CB_ERR) m_cbSchStyle.SetCurSel(0);
	}
  m_edSchTextHeight.SetWindowText(g_csSchTextHeight);

  // Success
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COptionsDlg::OnHelp() { DisplayHelp((DWORD) _T("IE_Toobox_Options_dialog_box.htm")); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : COptionsDlg::OnOK
// Description  : Called when the user clicks on the "OK" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COptionsDlg::OnOK()
{
  // Get the data entered by the user
  UpdateData();

  // Ensure everything is valid
  if (m_csPoleStyle.IsEmpty())           { ShowBalloon(_T("Default pole numbering text style must be selected."),           this, IDC_POLE_TEXT_STYLE);  return; }
  if (_tstof(m_csPoleTextHeight) <= 0.0) { ShowBalloon(_T("Default pole text height must be greater than zero."),           this, IDC_POLE_TEXT_HEIGHT); return; }
  if (_tstof(m_csPoleDia) <= 0.0)        { ShowBalloon(_T("Default pole diameter must be greater than zero."),              this, IDC_POLE_TEXT_HEIGHT); return; }
  if (m_csSchStyle.IsEmpty())            { ShowBalloon(_T("Default schedule table text style must be selected."),           this, IDC_POLE_TEXT_HEIGHT); return; }
  if (_tstof(m_csSchTextHeight) <= 0.0)  { ShowBalloon(_T("Default schedule table text height must be greater than zero."), this, IDC_POLE_TEXT_HEIGHT); return; }

	// Close the dialog
  CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_Options
// Description  : Called when the user enters the "SPOptions" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_Options()
{
  // Display the Options dialog
  COptionsDlg dlgOpt;
	if (dlgOpt.DoModal() == IDCANCEL) return;

	// Save the specified information to the drawings xrecord
	struct resbuf *rbpOptions = acutBuildList(AcDb::kDxfText, dlgOpt.m_csPoleStyle, 
																						AcDb::kDxfText, dlgOpt.m_csPoleTextHeight,
																						AcDb::kDxfText, dlgOpt.m_csPoleDia,
																						AcDb::kDxfText, dlgOpt.m_csSchStyle,
																						AcDb::kDxfText, dlgOpt.m_csSchTextHeight, NULL
																						);

	addXRecordToDictionary(DICT_IETB, XREC_IETB, rbpOptions);
	acutRelRb(rbpOptions);
}