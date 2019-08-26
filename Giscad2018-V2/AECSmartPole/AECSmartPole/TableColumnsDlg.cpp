////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : AECSmartPole
// Client name      : AEC Systems
// File name        : TableColumnsDlg.cpp
// Created          : 4th May 2007
// Created by       : S.Jaisimha
// Description      : Output column selection dialog implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "TableColumnsDlg.h"
#include "PoleInfo.h"
#include "LightInfo.h"

void sortPoleInfo(std:: vector <CPoleInfo> &poleInfo);
void sortLightInfo(std:: vector <CLightInfo> &lightInfo);
void sortLightInfoOnStatus(std:: vector <CLightInfo> &lightInfo);
void consolidateLightInfo (std:: vector <CLightInfo> &lightInfo_Vector);
extern bool GetAttributeValue(ads_name enPole, CString csAttDesc, CString &csAttValue);
extern void ApplyChangeRequest94(ads_name enPole);
void ApplyChangeRequest94ForLights(ads_name enLight);

// Defined in PoleDlg.cpp
extern struct resbuf *ConvertToLight(ads_name enLight);
extern struct resbuf *ConvertToPole (ads_name enPole);
extern struct resbuf * BuildFilterListForPole();
extern struct resbuf * BuildFilterListForLight();

double g_LegendLinearLen = 20;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Pole Information dialog
IMPLEMENT_DYNAMIC(CTableColumnsDlg, CDialog)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::CTableColumnsDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableColumnsDlg::CTableColumnsDlg(CWnd* pParent /*=NULL*/)	: CDialog(CTableColumnsDlg::IDD, pParent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::~CTableColumnsDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTableColumnsDlg::~CTableColumnsDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TABLE_LIST,   m_lcColumns);
  DDX_Control(pDX, IDC_TABLE_LAYOUT, m_cbLayouts);
  DDX_Control(pDX, IDC_TABLE_STYLE,  m_cbStyles);
  DDX_Control(pDX, IDC_TABLE_TEXTHT, m_edHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTableColumnsDlg, CDialog)
  ON_BN_CLICKED(IDC_SELECT_ALL, OnBnClickedSelectAll)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnBnClickedRemoveAll)
	ON_BN_CLICKED(IDHELP, OnHelp)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TABLE_LIST, &CTableColumnsDlg::OnLvnItemchangedTableList)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTableColumnsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Initialize the list control
  m_lcColumns.InsertColumn(0, _T("Column"), LVCFMT_LEFT, 200);
  m_lcColumns.SetExtendedStyle(LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
  
	CQueryTbl tblData;
	CString csSQL; 
	CStringArray csaDefaults;
	CString csValue;
  if (m_iCalledFor == 1)
  {
		// Add the output columns names for pole schedule table
    m_lcColumns.InsertItem(0, _T("Footing"));
    m_lcColumns.InsertItem(1, _T("Stay"));
    m_lcColumns.InsertItem(2, _T("Pole type"));
    m_lcColumns.InsertItem(3, _T("Hole details"));
    m_lcColumns.InsertItem(4, _T("Construction details"));
    m_lcColumns.InsertItem(5, _T("Lantern details"));
    m_lcColumns.InsertItem(6, _T("Field pole number"));

		// Check the rows that must appear as defaults as said in database.
		csSQL.Format(_T("SELECT [GroupName] & '#' & [Default] FROM [tblPoleTable] WHERE [Default] = 1"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, _T("CTableColumnsDlg::OnInitDialog()"))) return TRUE;
		if (tblData.GetRows() > 0)
		{
			tblData.GetColumnAt(0, csaDefaults);
			if (GetParameterValue(csaDefaults, _T("Footing"),              csValue, 0) >= 0) m_lcColumns.SetCheck(0, TRUE);
			if (GetParameterValue(csaDefaults, _T("Stay"),                 csValue, 0) >= 0) m_lcColumns.SetCheck(1, TRUE);
			if (GetParameterValue(csaDefaults, _T("Pole type"),            csValue, 0) >= 0) m_lcColumns.SetCheck(2, TRUE);
			if (GetParameterValue(csaDefaults, _T("Hole details"),         csValue, 0) >= 0) m_lcColumns.SetCheck(3, TRUE);
			if (GetParameterValue(csaDefaults, _T("Construction details"), csValue, 0) >= 0) m_lcColumns.SetCheck(4, TRUE);
			if (GetParameterValue(csaDefaults, _T("Lantern details"),      csValue, 0) >= 0) m_lcColumns.SetCheck(5, TRUE);
			if (GetParameterValue(csaDefaults, _T("Field pole number"),    csValue, 0) >= 0) m_lcColumns.SetCheck(6, TRUE);
		}
  }
  else if (m_iCalledFor == 2)
  {
		// Add the output columns names for the light schedule table
	  m_lcColumns.InsertItem(0,  _T("Luminaire details"));
		m_lcColumns.InsertItem(1,  _T("Column/Pole details"));
		m_lcColumns.InsertItem(2,  _T("Bracket/Outreach code"));
		m_lcColumns.InsertItem(3,  _T("Charge to"));
		m_lcColumns.InsertItem(4,  _T("Lighting category"));
		/*m_lcColumns.InsertItem(0,  _T("Lantern details"));
		m_lcColumns.InsertItem(1,  _T("Column details"));
		m_lcColumns.InsertItem(2,  _T("Type code"));
		m_lcColumns.InsertItem(3,  _T("Rate code"));
		m_lcColumns.InsertItem(4,  _T("Lighting category"));
		m_lcColumns.InsertItem(5,  _T("Charge to"));
		m_lcColumns.InsertItem(6,  _T("Pole details"));*/

	
		// Check the rows that must appear as defaults as said in database.
		csSQL.Format(_T("SELECT [GroupName] & '#' & [Default] FROM [tblStreetLightTable] WHERE [Default] = 1"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, _T("CTableColumnsDlg::OnInitDialog()"))) return TRUE;
		if (tblData.GetRows() > 0)
		{
			tblData.GetColumnAt(0, csaDefaults);
		
			if (GetParameterValue(csaDefaults, _T("Luminaire details"),   csValue, 0) >= 0) m_lcColumns.SetCheck(0, TRUE);
			if (GetParameterValue(csaDefaults, _T("Column/Pole details"),    csValue, 0) >= 0) m_lcColumns.SetCheck(1, TRUE);
			if (GetParameterValue(csaDefaults, _T("Bracket/Outreach details"),         csValue, 0) >= 0) m_lcColumns.SetCheck(2, TRUE);
			if (GetParameterValue(csaDefaults, _T("Charge to"),         csValue, 0) >= 0) m_lcColumns.SetCheck(3, TRUE);
			if (GetParameterValue(csaDefaults, _T("Lighting category"), csValue, 0) >= 0) m_lcColumns.SetCheck(4, TRUE);

			m_lcColumns.SetCheck(0, TRUE);
			m_lcColumns.SetCheck(1, TRUE);
			m_lcColumns.SetCheck(2, TRUE);
			m_lcColumns.SetCheck(3, TRUE);
			m_lcColumns.SetCheck(4, TRUE);
			/*if (GetParameterValue(csaDefaults, _T("Lantern details"),   csValue, 0) >= 0) m_lcColumns.SetCheck(0, TRUE);
			if (GetParameterValue(csaDefaults, _T("Column details"),    csValue, 0) >= 0) m_lcColumns.SetCheck(1, TRUE);
			if (GetParameterValue(csaDefaults, _T("Type code"),         csValue, 0) >= 0) m_lcColumns.SetCheck(2, TRUE);
			if (GetParameterValue(csaDefaults, _T("Rate code"),         csValue, 0) >= 0) m_lcColumns.SetCheck(3, TRUE);
			if (GetParameterValue(csaDefaults, _T("Lighting category"), csValue, 0) >= 0) m_lcColumns.SetCheck(4, TRUE);
			if (GetParameterValue(csaDefaults, _T("Charge to"),         csValue, 0) >= 0) m_lcColumns.SetCheck(5, TRUE);
			if (GetParameterValue(csaDefaults, _T("Pole details"),      csValue, 0) >= 0) m_lcColumns.SetCheck(6, TRUE);*/
		}
  }
	
  // Show all layouts in the drawing
  ShowLayoutsInDrawing();

  // Show all text styles in the drawing
  ShowTextStylesInDrawing();

  // Show the default values
  m_cbStyles.SelectString(-1, g_csSchTextStyle);
  m_edHeight.SetWindowText(g_csSchTextHeight);

  // Everything is OK
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::ShowLayoutsInDrawing
// Description  : Populates the "Layouts" combo with all layouts in the drawing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::ShowLayoutsInDrawing()
{
  // Get the block table of the drawing and create a new iterator
  AcDbBlockTable *pBT; acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBT, AcDb::kForRead);
  AcDbBlockTableIterator* pIter; pBT->newIterator(pIter);
  pBT->close();

  // Loop through the iterator
  for( ; !pIter->done(); pIter->step())
  {
    // Get the block table record
    AcDbBlockTableRecord* pBTR; pIter->getRecord(pBTR, AcDb::kForRead);

    // If this is a layout
    if (pBTR->isLayout())
    {
      // Get the layout's object ID and from there its name
      AcDbObjectId layoutId = pBTR->getLayoutId();
      AcDbLayout *pLayout; acdbOpenAcDbObject((AcDbObject*&)pLayout, layoutId, AcDb::kForRead);
      ACHAR *pLayoutName; pLayout->getLayoutName(pLayoutName);
      CString csLayoutName = pLayoutName;
      pLayout->close();

      // If this is not the "Model" layout
      if (csLayoutName != _T("Model")) m_cbLayouts.AddString(csLayoutName);
    }

    // Close the block table record
    pBTR->close();
  }

	// Set the deault layout
	m_cbLayouts.SetCurSel(0);

  // Delete the iterator
  delete pIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::ShowTextStylesInDrawing
// Description  : Populates the "Text styles" combo with all layouts in the drawing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::ShowTextStylesInDrawing()
{
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
    m_cbStyles.AddString(csTSName);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::OnBnClickedSelectAll
// Description  : Called when the user clicks on the "Select all" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::OnBnClickedSelectAll()
{
  // Check everything in the list
  for (int iCtr = 0; iCtr < m_lcColumns.GetItemCount(); iCtr++) m_lcColumns.SetCheck(iCtr, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::OnBnClickedRemoveAll
// Description  : Called when the user clicks on the "Remove all" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::OnBnClickedRemoveAll()
{
  // Check everything in the list
  for (int iCtr = 0; iCtr < m_lcColumns.GetItemCount(); iCtr++) m_lcColumns.SetCheck(iCtr, FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::OnHelp() 
{
	if (m_iCalledFor == 1) DisplayHelp((DWORD) _T("Generating_pole_schedule_table.htm")); else  DisplayHelp((DWORD) _T("Generating_light_schedule_tabl.htm"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CTableColumnsDlg::OnOK
// Description  : Called when the user clicks on the "OK" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableColumnsDlg::OnOK()
{
  m_csaColumns.RemoveAll();

  // Get the selected columns
  for (int iCtr = 0; iCtr < m_lcColumns.GetItemCount(); iCtr++) 
  {
    if (m_lcColumns.GetCheck(iCtr) == TRUE)
    {
      m_csaColumns.Add(m_lcColumns.GetItemText(iCtr, 0));
    }
  }

  // Get the selected layout
  int iLayout = m_cbLayouts.GetCurSel();
  if (iLayout == CB_ERR) { ShowBalloon(_T("Layout must be selected."), this, IDC_TABLE_LAYOUT); return; }
  m_cbLayouts.GetLBText(iLayout, m_csLayout);

  // Get the selected text style
  int iStyle = m_cbStyles.GetCurSel();
  if (iStyle == CB_ERR) { ShowBalloon(_T("Text style must be selected."), this, IDC_TABLE_STYLE); return; }
  m_cbStyles.GetLBText(iStyle, m_csStyle);

  // Get the text height
  m_edHeight.GetWindowText(m_csHeight);
  if (_tstof(m_csHeight) <= 0.0) { ShowBalloon(_T("Text height must be greater than zero."), this, IDC_TABLE_TEXTHT); return; }

  // Close the dialog
  CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : setTextString
// Description  : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setTextString(AcDbTable *pTable, int iRow, int iCol, AcDbObjectId objIDTxtStyle, AcDb::RotationAngle rotAngle, CString csText, CString csTxtHt)
{
	pTable->setTextString(iRow, iCol, csText); pTable->setTextRotation(iRow, iCol, rotAngle);
	pTable->setTextHeight(iRow, iCol, _tstof(csTxtHt));
	pTable->setTextRotation(iRow, iCol, rotAngle);

	// Set the row alignment
	pTable->setAlignment(iRow, iCol, AcDb::kMiddleCenter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_PoleTable
// Description  : Called when the user enters the "IE_PoleTable" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_PoleTable()
{
	// Ensure that the layer for this block is created and current
	createLayer(_T("NOTES"), Adesk::kFalse);

	// Ask user to select poles to output
	ads_name ssPoles; 
	Adesk::Int32 lLength = 0L;
	// struct resbuf *rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), AcDb::kDxfRegAppName, XDATA_POLE, NULL);
	struct resbuf *rbpFilter = BuildFilterListForPole();

	if (acedSSGet(NULL, NULL, NULL, rbpFilter, ssPoles) != RTNORM) return;
	if ((acedSSLength(ssPoles, &lLength) != RTNORM) && (lLength < 1L)) return;

	// Collect the pole data into arrays
	ads_name enPole;
	struct resbuf *rbpPoleXD = NULL;
	AcDbObjectId objPole;
	AcDbHandle dbPoleHandle;
	ACHAR szPoleHandle[17];

	std::vector <CPoleInfo> poleInfo_Vector;
	for (long lCtr = 0L; lCtr < lLength; lCtr++)
	{
		// Get the entity name
		acedSSName(ssPoles, lCtr, enPole);

		// If it have the pole XDATA convert it now
		// if ((rbpPoleXD = getXDataFromEntity(enPole, XDATA_POLE)) == NULL) continue;
		if ((rbpPoleXD = getXDataFromEntity(enPole, XDATA_POLE)) == NULL)	{	if (!(rbpPoleXD = ConvertToPole(enPole))) continue;	}

		// Apply Change request to the pole
		ApplyChangeRequest94(enPole);
		rbpPoleXD = getXDataFromEntity(enPole, XDATA_POLE);

		CPoleInfo poleInfo;	
		rbpPoleXD = rbpPoleXD->rbnext->rbnext;                                                    
		poleInfo.m_csPoleNumber       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext->rbnext; 
		if (poleInfo.m_csPoleNumber.Trim().IsEmpty()) continue;
		poleInfo.m_csPoleStatus       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;					
		poleInfo.m_csFieldNumberExist	= rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;
		poleInfo.m_csFieldNumberNew		= rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;

		//////////////////////////////////////////////////////////////////////////////////////////
		// Modification: Mail 13.11.2007: The user will run a utility called "Asset Numbering"
		//               that will populate "Asset_Num" attribute automatically for all poles
		//               and street lights (New Field pole number). 
		//               User will then generate the schedule table.
		// Old code
		//poleInfo.m_csFieldNumberExist = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext; 
		//poleInfo.m_csFieldNumberNew   = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext; 

		// New code
		//poleInfo.m_csFieldNumberExist = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext->rbnext; 
		//if (!GetAttributeValue(enPole, GetPoleTag("New Field pole number"), poleInfo.m_csFieldNumberNew))
		//{
			//acutPrintf("\nUnable to retrieve attribute value for \"New Field pole number\".");
		//}
		//////////////////////////////////////////////////////////////////////////////////////////
		
		poleInfo.m_csPoleSize         = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csPoleStay         = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csPoleFooting      = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csPoleDia          = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;					
		poleInfo.m_csHoleDepth        = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;					
		poleInfo.m_csHoleDia          = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csPoleHV           = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csPoleLV           = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;					
		poleInfo.m_csLanternStatus    = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csLanternBracket   = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csSpanLength       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         
		poleInfo.m_csLineDevDeg       = rbpPoleXD->resval.rstring; rbpPoleXD = rbpPoleXD->rbnext;         

		// Add this to the vector for sort
		poleInfo_Vector.push_back(poleInfo);
	}

	// Free the selection set
	acutRelRb(rbpPoleXD);
	acedSSFree(ssPoles);

	// If there are no lights whose details are to be displayed, exit the command
	if (!poleInfo_Vector.size())  { appMessage(_T("No poles selected (or) the poles selected do not have pole numbers."), 00); return; }

	// Sort the pole array
	sortPoleInfo(poleInfo_Vector);

	CStringArray csaPoleNumbers, csaPoleStatus, csaFieldNumbersExist, csaFieldNumbersNew, csaPoleSizes,csaPoleStays, csaPoleFootings;
	CStringArray csaHoleDepths, csaHoleDias, csaLanternStatus, csaLanternBrackets, csaSpanLengths, csaLineDevDegs;
	CStringArray csaPoleDias, csaPoleHVs, csaPoleLVs;

	for (int i = 0; i < poleInfo_Vector.size(); i++)
	{
		CPoleInfo poleInfo = poleInfo_Vector.at(i);
		csaPoleNumbers      .Add(poleInfo.m_csPoleNumber); 
		csaPoleStatus       .Add(poleInfo.m_csPoleStatus);
		csaFieldNumbersExist.Add(poleInfo.m_csFieldNumberExist); 
		csaFieldNumbersNew  .Add(poleInfo.m_csFieldNumberNew); 

		csaPoleSizes				.Add(poleInfo.m_csPoleSize); 
		csaPoleStays				.Add(poleInfo.m_csPoleStay); 
		csaPoleFootings			.Add(poleInfo.m_csPoleFooting); 
		csaPoleDias					.Add(poleInfo.m_csPoleDia); 
		csaHoleDepths				.Add(poleInfo.m_csHoleDepth); 
		csaHoleDias					.Add(poleInfo.m_csHoleDia); 
		csaPoleHVs					.Add(poleInfo.m_csPoleHV); 
		csaPoleLVs					.Add(poleInfo.m_csPoleLV); 
		csaLanternStatus		.Add(poleInfo.m_csLanternStatus); 
		csaLanternBrackets	.Add(poleInfo.m_csLanternBracket); 
		csaSpanLengths			.Add(poleInfo.m_csSpanLength); 
		csaLineDevDegs			.Add(poleInfo.m_csLineDevDeg); 
	}
	
	// Get the columns to be present in the table
	CTableColumnsDlg dlgTC;
	dlgTC.m_iCalledFor = 1;
	if (dlgTC.DoModal() == IDCANCEL) return;

	// Store the specified text height and calculate the column widths
	double dTextHeight = _tstof(dlgTC.m_csHeight);
	double dRowHeight  = dTextHeight * 3.0;
	double dCWidth1    = dRowHeight;
	double dCWidth2    = dRowHeight * 2.0;
	double dCWidth3    = dRowHeight * 3.0;
	double dCWidth4    = dRowHeight * 4.0;

	// Calculate the number of columns required in the output (minimum will be 6 columns for the basic pole information)
	BOOL bHasFooting   = (CheckForDuplication(dlgTC.m_csaColumns, _T("Footing"))              ? TRUE : FALSE);
	BOOL bHasStay      = (CheckForDuplication(dlgTC.m_csaColumns, _T("Stay"))                 ? TRUE : FALSE);
	BOOL bHasType      = (CheckForDuplication(dlgTC.m_csaColumns, _T("Pole type"))            ? TRUE : FALSE);
	BOOL bHasHoleData  = (CheckForDuplication(dlgTC.m_csaColumns, _T("Hole details"))         ? TRUE : FALSE);
	BOOL bHasConstData = (CheckForDuplication(dlgTC.m_csaColumns, _T("Construction details")) ? TRUE : FALSE);
	BOOL bHasLantern   = (CheckForDuplication(dlgTC.m_csaColumns, _T("Lantern details"))      ? TRUE : FALSE);
	BOOL bHasFieldPole = (CheckForDuplication(dlgTC.m_csaColumns, _T("Field pole number"))    ? TRUE : FALSE);

	// Create a new TABLE entity
	AcDbTable *pTableLegend = new AcDbTable();
	pTableLegend->setLayer(_T("0"));
	pTableLegend->setNumRows(csaPoleNumbers.GetSize() + 2);
	pTableLegend->setRowHeight(dRowHeight);
			
	// Set text style in the table to "Standard"
	AcDbObjectId objIDTxtStyle;
	AcDbTextStyleTable *pTxtStyle = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTxtStyle, AcDb::kForRead) != Acad::eOk) { pTableLegend->close(); return; }
	if (pTxtStyle->getAt(dlgTC.m_csStyle, objIDTxtStyle) != Acad::eOk) { pTableLegend->close(); pTxtStyle->close(); return; }
	pTxtStyle->close();

	//////////////////////////////////////////////////////////////////////////
	// Modify the "Standard" table style to represent set the flow direction
	//////////////////////////////////////////////////////////////////////////

	AcDbDictionary *pDict = NULL; 
	AcDbObjectId styleId;      
	acdbHostApplicationServices()->workingDatabase()->getTableStyleDictionary(pDict, AcDb::kForRead);  
	Acad::ErrorStatus es;
	pDict->getAt(_T("Standard"), styleId); 
	pDict->close();

	AcDbTableStyle *pTableStyleTbl;
	if (acdbOpenObject(pTableStyleTbl, styleId, AcDb::kForWrite) == Acad::eOk)
	{
		pTableStyleTbl->setFlowDirection(AcDb::kBtoT);
		pTableStyleTbl->close();
	}

	pTableLegend->setTextStyle(objIDTxtStyle, AcDb::kAllRows);
	pTableLegend->setAlignment(AcDb::kMiddleCenter, AcDb::kAllRows);
	pTableLegend->generateLayout(); // Very very important, else expect crashes later on

	// Row 2: Headers for the columns
	double  dWidth = 1; CString csWidth;
	pTableLegend->setRowHeight(1, dRowHeight * 3.0);
	int iCol = 0;

	if (bHasFieldPole)
	{ 
		int iFNumberCol = iCol;

		dWidth = dCWidth1;
		if (GetParameterValue(g_csaPoleTblTitles, "Field pole number-NEW", csWidth, 0) >= 0) dWidth = _tstof(csWidth);
		pTableLegend->insertColumns(iCol, dWidth);
		// KMK: pTableLegend->setTextString(1, iCol, _T("NEW")); pTableLegend->setTextRotation(1, iCol, AcDb::kDegrees090); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("NEW"), dlgTC.m_csHeight);
		iCol++; 

		dWidth = dCWidth1;
		if (GetParameterValue(g_csaPoleTblTitles, _T("Field pole number-EXIST"), csWidth, 0) >= 0) dWidth = _tstof(csWidth);
		pTableLegend->insertColumns(iCol, dWidth);
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("EXISTING"), dlgTC.m_csHeight);
		iCol++; 

		pTableLegend->mergeCells(0, 0, iFNumberCol,  iCol - 1);
		setTextString(pTableLegend, 0, 0, objIDTxtStyle, AcDb::kDegrees000, _T("FIELD POLE\\PNUMBER"), dlgTC.m_csHeight);
	}

	if (bHasLantern)  
	{  
		int iLantCol = iCol;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Lantern details-NEW"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("NEW"), dlgTC.m_csHeight); iCol++; 
		
		if (GetParameterValue(g_csaPoleTblTitles, _T("Lantern details-REMOVE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("REMOVE"), dlgTC.m_csHeight); iCol++; 

		if (GetParameterValue(g_csaPoleTblTitles, _T("Lantern details-EXISTING"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("EXISTING"), dlgTC.m_csHeight); iCol++; 

		// ***** HERE *****
		if (GetParameterValue(g_csaPoleTblTitles, _T("Lantern details-REPLACE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("REPLACE"), dlgTC.m_csHeight); iCol++; 
		// *********

		if (GetParameterValue(g_csaPoleTblTitles, _T("Lantern details-BRACKET"), csWidth, 0) >= 0) 
		{
			dWidth = _tstof(csWidth); 
		}
		else 
		{
			dWidth =  dCWidth1;
		}
		
	/*	CString tempStr; 
		tempStr.Format(_T("%d %s %d %d"),dWidth,csWidth, dCWidth1,wcstod(csWidth,NULL));
		MessageBox(NULL, tempStr, _T("Test"), MB_OK); */

		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("BRACKET"), dlgTC.m_csHeight); iCol++; 

		pTableLegend->mergeCells(0, 0, iLantCol,  iCol - 1);
		setTextString(pTableLegend, 0, iLantCol, objIDTxtStyle, AcDb::kDegrees000, _T("LANTERNS"), dlgTC.m_csHeight);
	}

	if (bHasConstData)
	{
		int iConstCol = iCol;
		
		if (GetParameterValue(g_csaPoleTblTitles, _T("Construction details-SPAN LENGTH"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("SPAN\\PLENGTH"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Construction details-LINE DEV DEGREES"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("LINE DEV\\PDEGREES"), dlgTC.m_csHeight);  iCol++;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Construction details-HV"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("HV"), dlgTC.m_csHeight);  iCol++;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Construction details-LV"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("LV"), dlgTC.m_csHeight);  iCol++;
		
		pTableLegend->mergeCells(0,0, iConstCol, iCol - 1);
		setTextString(pTableLegend, 0, iConstCol, objIDTxtStyle, AcDb::kDegrees000, _T("CONSTRUCTION"), dlgTC.m_csHeight);
	}

	if (bHasHoleData) 
	{ 
		int iHoleCol = iCol;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Hole details-DIA mm"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("DIA\\Pmm"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaPoleTblTitles, _T("Hole details-DEPTH mm"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("DEPTH\\Pm"), dlgTC.m_csHeight); iCol++;

		pTableLegend->mergeCells(0, 0, iHoleCol, iCol - 1);
		setTextString(pTableLegend, 0, iHoleCol, objIDTxtStyle, AcDb::kDegrees000, _T("HOLE"), dlgTC.m_csHeight);
	}
	
	int iPoleCol = iCol;
	if (bHasType)     
	{
		if (GetParameterValue(g_csaPoleTblTitles, _T("Pole type-TYPE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("TYPE"), dlgTC.m_csHeight); iCol++;
	}

	if (bHasStay) 
	{
		if (GetParameterValue(g_csaPoleTblTitles, _T("Stay-STAY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("STAY"), dlgTC.m_csHeight); iCol++;
	}

	if (bHasFooting) 
	{
		if (GetParameterValue(g_csaPoleTblTitles, _T("Footing-FOOTING"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("FOOTING"), dlgTC.m_csHeight); iCol++;
	}

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-RELOCATE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("RELOCATE"), dlgTC.m_csHeight); iCol++;

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-REPLACE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("REPLACE"), dlgTC.m_csHeight); iCol++;

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-NEW"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("NEW"), dlgTC.m_csHeight); iCol++;

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-EXISTING"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("EXISTING"), dlgTC.m_csHeight); iCol++;

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-REMOVE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("REMOVE"), dlgTC.m_csHeight); iCol++;

	if (GetParameterValue(g_csaPoleTblTitles, _T("Pole details-NUMBER"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); 
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("NUMBER"), dlgTC.m_csHeight); iCol++;
	
	pTableLegend->mergeCells(0, 0, iPoleCol, iCol - 1);
	setTextString(pTableLegend, 0, iPoleCol, objIDTxtStyle, AcDb::kDegrees000, _T("POLE"), dlgTC.m_csHeight);
	pTableLegend->deleteColumns(iCol);

	// Populate the table rows
	CString csPoleNumber, csPoleStatus, csFieldNumberExist, csFieldNumberNew, csPoleSize, csPoleStay, csPoleFooting;
	CString csHoleDepth, csHoleDia, csLanternStatus, csLanternBracket, csLV, csHV, csSpanLength, csLineDevDeg;
	for (int iCtr = 0, iRow = 2; iCtr < csaPoleNumbers.GetSize(); iCtr++, iRow++)
	{
  	// Get the row data
		csPoleNumber       = csaPoleNumbers      .GetAt(iCtr); 
		csPoleStatus       = csaPoleStatus       .GetAt(iCtr);
		csFieldNumberExist = csaFieldNumbersExist.GetAt(iCtr);
		csFieldNumberNew   = csaFieldNumbersNew  .GetAt(iCtr);
		csPoleSize         = csaPoleSizes        .GetAt(iCtr);
		csPoleStay         = csaPoleStays        .GetAt(iCtr);
		csPoleFooting      = csaPoleFootings     .GetAt(iCtr);
		csHoleDepth        = csaHoleDepths       .GetAt(iCtr);
		csHoleDia          = csaHoleDias         .GetAt(iCtr);	
		csLanternStatus    = csaLanternStatus    .GetAt(iCtr);
		csLanternBracket   = csaLanternBrackets  .GetAt(iCtr);
		csSpanLength       = csaSpanLengths      .GetAt(iCtr);
		csLineDevDeg       = csaLineDevDegs      .GetAt(iCtr);
		csHV               = csaPoleHVs          .GetAt(iCtr); 
		csLV               = csaPoleLVs          .GetAt(iCtr);

		int iCol = 0;
		if (bHasFieldPole) 
		{
			setTextString(pTableLegend, iRow, iCol + 0, objIDTxtStyle, AcDb::kDegrees000, csFieldNumberNew, dlgTC.m_csHeight);
			setTextString(pTableLegend, iRow, iCol + 1, objIDTxtStyle, AcDb::kDegrees000, csFieldNumberExist, dlgTC.m_csHeight);

			iCol = iCol + 2; 
		}

		if (bHasLantern)
		{ 
			if (csLanternStatus == _T("New"))			 setTextString(pTableLegend, iRow, iCol + 0, objIDTxtStyle, AcDb::kDegrees000,  _T("X"), dlgTC.m_csHeight);  
			if (csLanternStatus == _T("Remove"))   setTextString(pTableLegend, iRow, iCol + 1, objIDTxtStyle, AcDb::kDegrees000,  _T("X"), dlgTC.m_csHeight);  
			if (csLanternStatus == _T("Existing")) setTextString(pTableLegend, iRow, iCol + 2, objIDTxtStyle, AcDb::kDegrees000,  _T("X"), dlgTC.m_csHeight);  
			// ****
			if (csLanternStatus == _T("Replace"))  setTextString(pTableLegend, iRow, iCol + 3, objIDTxtStyle, AcDb::kDegrees000,  _T("X"), dlgTC.m_csHeight);  
			// ****
			// iCol = iCol + 3; 
			iCol = iCol + 4; 
			// ****
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csLanternBracket, dlgTC.m_csHeight);
		}

		if (bHasConstData) 
		{
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csSpanLength, dlgTC.m_csHeight);
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csLineDevDeg, dlgTC.m_csHeight);
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csHV, dlgTC.m_csHeight);
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csLV, dlgTC.m_csHeight);
		}

		if (bHasHoleData)
		{
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csHoleDia, dlgTC.m_csHeight);
			setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csHoleDepth, dlgTC.m_csHeight);
		}

		// Set the row data
		if (bHasType)        setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csPoleSize, dlgTC.m_csHeight);
		if (bHasStay)        setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csPoleStay, dlgTC.m_csHeight);
		if (bHasFooting)     setTextString(pTableLegend, iRow, iCol++, objIDTxtStyle, AcDb::kDegrees000, csPoleFooting, dlgTC.m_csHeight);

		if (csPoleStatus == _T("Relocate")) { setTextString(pTableLegend, iRow, iCol + 0, objIDTxtStyle, AcDb::kDegrees000, _T("X"), dlgTC.m_csHeight);  }
		if (csPoleStatus == _T("Replace"))  { setTextString(pTableLegend, iRow, iCol + 1, objIDTxtStyle, AcDb::kDegrees000, _T("X"), dlgTC.m_csHeight);  }
		if (csPoleStatus == _T("New"))      { setTextString(pTableLegend, iRow, iCol + 2, objIDTxtStyle, AcDb::kDegrees000, _T("X"), dlgTC.m_csHeight);  }
		if (csPoleStatus == _T("Existing")) { setTextString(pTableLegend, iRow, iCol + 3, objIDTxtStyle, AcDb::kDegrees000, _T("X"), dlgTC.m_csHeight);  }
		if (csPoleStatus == _T("Remove"))   { setTextString(pTableLegend, iRow, iCol + 4, objIDTxtStyle, AcDb::kDegrees000, _T("X"), dlgTC.m_csHeight);  }

		iCol = iCol + 5; 

		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csPoleNumber, dlgTC.m_csHeight);
	}
		
	// Switch to the appropriate Layout in paper space
	acdbHostApplicationServices()->layoutManager()->setCurrentLayout(dlgTC.m_csLayout);

	// Add the table to the drawing
	AcDbBlockTable *pBlkTbl = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) { pTableLegend->close(); return; }

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (pBlkTbl->getAt( ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite) != Acad::eOk) { pBlkTbl->close(); pTableLegend->close(); return; }
	pBlkTbl->close();

	double dTableWidth  = pTableLegend->width();
	double dTableHeight = pTableLegend->height();
	pBlkTblRcd->appendAcDbEntity(pTableLegend);

	// Register the application and add the XDATA to the table.
	struct resbuf *rbpXData = acutBuildList(AcDb::kDxfRegAppName, _T("POLE_SCHEDULE"), AcDb::kDxfXdInteger16, 1, NULL);
	acdbRegApp(_T("POLE_SCHEDULE")); pTableLegend->setXData(rbpXData); acutRelRb(rbpXData);

	// Attach XData to table about the pole numbers in the table
	for (int iCtr = 0; iCtr < csaPoleNumbers.GetSize(); iCtr++)
	{
		struct resbuf *rbpPoleNumber = acutBuildList(AcDb::kDxfRegAppName, csaPoleNumbers.GetAt(iCtr), AcDb::kDxfXdInteger16, 1, NULL);

		// Register the application and add the xdata to the table.
		acdbRegApp(csaPoleNumbers.GetAt(iCtr));	pTableLegend->setXData(rbpPoleNumber); acutRelRb(rbpPoleNumber);
	}

	// Register the application and add the xdata to the table.
	struct resbuf *rbpTblCfg = acutBuildList(AcDb::kDxfRegAppName, _T("TABLECFG"), AcDb::kDxfXdInteger16, bHasFooting,
																					 AcDb::kDxfXdInteger16, bHasStay,
																					 AcDb::kDxfXdInteger16, bHasType,
																					 AcDb::kDxfXdInteger16, bHasHoleData,
																					 AcDb::kDxfXdInteger16, bHasLantern,
																					 AcDb::kDxfXdInteger16, bHasFieldPole,
																					 AcDb::kDxfXdInteger16, bHasConstData,
																					 NULL
																					);

	// Register the application and add the XDATA to the table.
	acdbRegApp(_T("TABLECFG")); pTableLegend->setXData(rbpTblCfg); acutRelRb(rbpTblCfg);
	pBlkTblRcd->close();

	// If in Model space, switch to Model Space
	struct resbuf rbTileMode; rbTileMode.restype = RTSHORT; rbTileMode.resval.rint = 0; acedSetVar(_T("TILEMODE"), &rbTileMode);
	acedCommandS(RTSTR, _T(".PSPACE"), NULL);

	int iRet;
	ads_point ptTable;
	acedInitGet(RSG_NONULL, _T(""));
	while (T)
	{
		iRet = acedGetPoint(NULL, _T("\nSpecify insertion point: "), ptTable);
		if (iRet == RTCAN) { pTableLegend->erase(); pTableLegend->close(); return; }
		else if (iRet == RTNORM) break;
	}

	pTableLegend->setPosition(AcGePoint3d(ptTable[X] - dTableWidth, ptTable[Y], 0.0));
	pTableLegend->setLayer(_T("NOTES"));
	pTableLegend->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_LightTable
// Description  : Called when the user enters the "SPLightTable" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_LightTable()
{
	// Ensure that the layer for this block is created and current
	createLayer(_T("NOTES"), Adesk::kFalse);

	// Ask user to select lights to output
	ads_name ssLights; 
	Adesk::Int32 lLength = 0L;

	// KMK: 18.09.12: ****
	 struct resbuf *rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), AcDb::kDxfRegAppName, XDATA_LIGHT, NULL);
	//struct resbuf *rbpFilter = BuildFilterListForLight();
	// *** KMK: 18.09.12

	if (acedSSGet(NULL, NULL, NULL, rbpFilter, ssLights) != RTNORM) return;
	acutRelRb(rbpFilter);	

	if ((acedSSLength(ssLights, &lLength) != RTNORM) && (lLength < 1L)) { return; }
	
	// Collect the light data into arrays
	CString csValue;
	ads_name enLight;
	struct resbuf *rbpLightXD = NULL;

	CStringArray csaLightNumberXD;
	std::vector <CLightInfo> lightInfo_Vector;
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		acedSSName(ssLights, lCtr, enLight);

		// 18.09.12***
		// if ((rbpLightXD = getXDataFromEntity(enLight, XDATA_LIGHT)) == NULL) continue;
		if ((rbpLightXD = getXDataFromEntity(enLight, XDATA_LIGHT)) == NULL) 
		{
			if (!(rbpLightXD = ConvertToLight(enLight))) continue;	
		}
		
		// Apply Change request to the pole
		// rbpLightXD = ApplyChangeRequest94ForLights(enLight);
		ApplyChangeRequest94ForLights(enLight);
		rbpLightXD = getXDataFromEntity(enLight, XDATA_LIGHT);
		if (!rbpLightXD) continue;
		
		CLightInfo lightInfo;	
		rbpLightXD = rbpLightXD->rbnext->rbnext;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_iLightNumber	      = rbpLightXD->resval.rint; 
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLightNumber      = rbpLightXD->resval.rstring; 

		if (lightInfo.m_csLightNumber.IsEmpty() || lightInfo.m_csLightNumber.Trim().IsEmpty()) 
		{
			// if (rbpLightXD != NULL) acutRelRb(rbpLightXD); // Gives a GP!
			continue; 
		}

		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csIsStandard       = suppressZero(rbpLightXD->resval.rint);
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternSchedule  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternStatus    = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternType		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternStockCode = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternLamp		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLampStockCode    = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csRateCode			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csKingbolt			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternMast    	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMastCode       	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternOutreach  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csOutreachCode		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternBracket	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csBracketCode   	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternLocation  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternCategory  = rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csAssetColumn		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csAssetLantern		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csLanternColumn    = rbpLightXD->resval.rstring;
		
		rbpLightXD = rbpLightXD->rbnext->rbnext; lightInfo.m_csRagBolt	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csFoundationType	= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMASTRATE			= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csOutreachRate		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csBracketRate		= rbpLightXD->resval.rstring;
		rbpLightXD = rbpLightXD->rbnext; lightInfo.m_csMountingHeight	= rbpLightXD->resval.rstring;

		//////////////////////////////////////////////////////////////////////////
		// Charge To options
		//////////////////////////////////////////////////////////////////////////
		//rbpLightXD = rbpLightXD->rbnext->rbnext; 
		rbpLightXD = rbpLightXD->rbnext; 
		int iSizeOfChargeTo = rbpLightXD->resval.rint;
		for (int iCtr = 0; iCtr < iSizeOfChargeTo; iCtr++) 
		{
			rbpLightXD = rbpLightXD->rbnext; csValue.Format(_T("%s"), rbpLightXD->resval.rstring);
			if (!lightInfo.m_csChargeTo.IsEmpty())
				lightInfo.m_csChargeTo.Format(_T("%s\\P%s(%s%%)"), lightInfo.m_csChargeTo, csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
			else
				lightInfo.m_csChargeTo.Format(_T("%s(%s%%)"), csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
		}
		
		//////////////////////////////////////////////////////////////////////////
		// If the light status is Replaced, add the replaced light information too
		//////////////////////////////////////////////////////////////////////////
		if (!lightInfo.m_csLanternStatus.CompareNoCase(_T("Replace")))
		{
			// Add this to the vector for sort
			lightInfo.m_csLanternStatus = _T("New");			

			//////////////////////////////////////////////////////////////////////////
			// Replace light information
			//////////////////////////////////////////////////////////////////////////
			CLightInfo lightInfoReplace;
			lightInfoReplace.m_csLanternStatus = _T("Remove");
			lightInfoReplace.m_iLightNumber    = _ttoi(lightInfo.m_csLightNumber.Mid(1));
			lightInfoReplace.m_csLightNumber   = lightInfo.m_csLightNumber; 
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csIsStandard.Format(_T("%d"), rbpLightXD->resval.rint); 
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternSchedule   = rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternType     	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternLamp     	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csRateCode        	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csLanternCategory  	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csAssetColumn      	= rbpLightXD->resval.rstring;
			rbpLightXD = rbpLightXD->rbnext; lightInfoReplace.m_csAssetLantern     	= rbpLightXD->resval.rstring;

			//////////////////////////////////////////////////////////////////////////
			// Replace - Charge To
			//////////////////////////////////////////////////////////////////////////
			rbpLightXD = rbpLightXD->rbnext;
			iSizeOfChargeTo	= rbpLightXD->resval.rint; 
			for (int iCtr = 0; iCtr < iSizeOfChargeTo; iCtr++)
			{
				rbpLightXD = rbpLightXD->rbnext; csValue.Format(_T("%s"), rbpLightXD->resval.rstring);
				if (!lightInfoReplace.m_csChargeTo.IsEmpty())
					lightInfoReplace.m_csChargeTo.Format(_T("%s\\P%s(%s%%)"), lightInfoReplace.m_csChargeTo, csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
				else
					lightInfoReplace.m_csChargeTo.Format(_T("%s(%s%%)"), csValue.Mid(0, csValue.Find(_T("#"))), csValue.Mid(csValue.Find(_T("#")) + 1));
			}

			// Add this to the vector for sort
			lightInfo_Vector.push_back(lightInfoReplace);
		}

		csaLightNumberXD.Add(lightInfo.m_csLightNumber.Mid(1));
		lightInfo_Vector.push_back(lightInfo);
	}

	// Free the selection set
	acutRelRb(rbpLightXD);
	acedSSFree(ssLights);

	// If there are no lights whose details are to be displayed, exit the command
	if (!lightInfo_Vector.size()) { appMessage(_T("No lights selected (or) the lights selected do not have light numbers."), 00); return; }

	// Sort the light array
	sortLightInfo(lightInfo_Vector);
		
	CStringArray csaLightNumber, csaLanternStatus, csaLanternType, csaLanternStockCode, csaLaternLamp;
	CStringArray csaLampStockCode, csaLanternMast, csaMastCode, csaLanternOutreach, csaOutreachCode, csaTypeCode, csaRateCode, csaLanternCategory;
	CStringArray csaChargeTo, csaQty, csaColumnQty;
	CStringArray csaLanternBracket, csaBracketCode, csaKBH;
	
	CStringArray csaTariffClass, csaLuminaireRate, csaPoleColumnRate, csaOutreachBracketRate;
	CStringArray csaRagbolt, csaFoundationType, csaOutreachRate, csaAssetColumn, csAssetLantern;
	CStringArray csaMASTRATE;

	// Consolidate the information of lights by combining the identical lights
	consolidateLightInfo(lightInfo_Vector);
			
	// Put the consolidated information into individual arrays for tabulating them
	CString csQty;
	for (int i = 0; i < lightInfo_Vector.size(); i++)
	{
		csaLightNumber.     Add(lightInfo_Vector.at(i).m_csLightNumber); 
		csaLanternStatus.   Add(lightInfo_Vector.at(i).m_csLanternStatus);
		csaLanternType.     Add(lightInfo_Vector.at(i).m_csLanternType);
		csaLanternStockCode.Add(lightInfo_Vector.at(i).m_csLanternStockCode);
		csaLaternLamp.      Add(lightInfo_Vector.at(i).m_csLanternLamp);
		csaLampStockCode.   Add(lightInfo_Vector.at(i).m_csLampStockCode);
		csaLanternMast.     Add(lightInfo_Vector.at(i).m_csLanternMast);
		csaMastCode.        Add(lightInfo_Vector.at(i).m_csMastCode);
		csaLanternOutreach. Add(lightInfo_Vector.at(i).m_csLanternOutreach);
		csaOutreachCode.    Add(lightInfo_Vector.at(i).m_csOutreachCode);
		csaRateCode.        Add(lightInfo_Vector.at(i).m_csRateCode);
		csaLanternCategory. Add(lightInfo_Vector.at(i).m_csLanternCategory);
		csaChargeTo.        Add(lightInfo_Vector.at(i).m_csChargeTo);
		csQty.Format(_T("%d"), lightInfo_Vector.at(i).m_iQty); csaQty.Add(csQty);
		csaLanternBracket.  Add(lightInfo_Vector.at(i).m_csLanternBracket);
		csaBracketCode.	  	Add(lightInfo_Vector.at(i).m_csBracketCode);
		csaKBH.				Add(lightInfo_Vector.at(i).m_csKingbolt);
		csaTariffClass.		Add(lightInfo_Vector.at(i).m_csTariffClass);
		csaLuminaireRate.   Add(lightInfo_Vector.at(i).m_csRateCode); 
		csaPoleColumnRate.  Add(lightInfo_Vector.at(i).m_csPoleColumnRate);
		csaOutreachBracketRate.Add(lightInfo_Vector.at(i).m_csBracketRate);
		csaRagbolt.			Add(lightInfo_Vector.at(i).m_csRagBolt);
		csaFoundationType.	Add(lightInfo_Vector.at(i).m_csFoundationType);
		csaOutreachRate.	Add(lightInfo_Vector.at(i).m_csOutreachRate);
		csaAssetColumn.		Add(lightInfo_Vector.at(i).m_csAssetColumn);
		csAssetLantern.		Add(lightInfo_Vector.at(i).m_csAssetLantern);
		csaMASTRATE.		Add(lightInfo_Vector.at(i).m_csMASTRATE);

		//if( lightInfo_Vector.at(i).m_iColumnQty>0)
		//{
		//	csQty.Format(_T("%d"), lightInfo_Vector.at(i).m_iColumnQty+1);
		//}
		//else
		//{
		if(lightInfo_Vector.at(i).m_csLanternColumn != _T(""))
		{
			csQty.Format(_T("%d"), lightInfo_Vector.at(i).m_iColumnQty+1);
		}
		else
			csQty.Format(_T("%d"), lightInfo_Vector.at(i).m_iColumnQty);
		//}

		csaColumnQty.		Add(csQty);
	}
		
  // Get the columns to be present in the table
  CTableColumnsDlg dlgTC;
  dlgTC.m_iCalledFor = 2;
  if (dlgTC.DoModal() == IDCANCEL) return;
	
	// Store the specified text height and calculate the column widths
	double dTextHeight = _tstof(dlgTC.m_csHeight);
	double dRowHeight  = dTextHeight * 3.0;
	double dCWidth1    = dRowHeight;
	double dCWidth2    = dRowHeight * 2.0;
	double dCWidth3    = dRowHeight * 3.0;
	double dCWidth4    = dRowHeight * 4.0; 

	

	// Calculate the number of columns required in the output (minimum will be 6 columns for the basic pole information)
	BOOL bHasLaternDetails = (CheckForDuplication(dlgTC.m_csaColumns, _T("Luminaire details"))   ? TRUE : FALSE);
	BOOL bHasColumnDetails = (CheckForDuplication(dlgTC.m_csaColumns, _T("Column/Pole details"))    ? TRUE : FALSE);
	BOOL bHasPoleDetails   = (CheckForDuplication(dlgTC.m_csaColumns, _T("Bracket/Outreach details"))      ? TRUE : FALSE);
	//BOOL bHasTypeCode      = (CheckForDuplication(dlgTC.m_csaColumns, _T("Type code"))         ? TRUE : FALSE);
	//BOOL bHasRateCode      = (CheckForDuplication(dlgTC.m_csaColumns, _T("Rate code"))         ? TRUE : FALSE);
	BOOL bHasCategory      = (CheckForDuplication(dlgTC.m_csaColumns, _T("Lighting category")) ? TRUE : FALSE);
	BOOL bHasCharge        = (CheckForDuplication(dlgTC.m_csaColumns, _T("Charge to"))         ? TRUE : FALSE);

	bHasLaternDetails=TRUE;
	bHasColumnDetails = TRUE;
	bHasPoleDetails   = TRUE;
	bHasCategory      = TRUE;
	bHasCharge        = TRUE;

	// Create a new TABLE entity
	AcDbTable *pTableLegend = new AcDbTable();
	pTableLegend->setLayer(_T("0"));
	pTableLegend->setNumRows(csaLightNumber.GetSize() + 3);
	pTableLegend->setRowHeight(dRowHeight);

	// Get the objectID of the text style
	AcDbObjectId objIDTxtStyle;
	AcDbTextStyleTable *pTxtStyle = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTxtStyle, AcDb::kForWrite) != Acad::eOk) return;
	if (pTxtStyle->getAt(g_csSchTextStyle, objIDTxtStyle) != Acad::eOk) { pTxtStyle->close(); return; }
	pTxtStyle->close();
	
	//////////////////////////////////////////////////////////////////////////
	// Modify the "Standard" table style to represent set the flow direction
	//////////////////////////////////////////////////////////////////////////

	AcDbDictionary *pDict = NULL; 
	AcDbObjectId styleId;      
	acdbHostApplicationServices()->workingDatabase()->getTableStyleDictionary(pDict, AcDb::kForRead);  
	Acad::ErrorStatus es;
	pDict->getAt(_T("Standard"), styleId); 
	pDict->close();

	AcDbTableStyle *pTableStyleTbl;
	if (acdbOpenObject(pTableStyleTbl, styleId, AcDb::kForWrite) == Acad::eOk)
	{
		//pTableStyleTbl->setFlowDirection(AcDb::kBtoT);
		pTableStyleTbl->setFlowDirection(AcDb::kTtoB);
		pTableStyleTbl->close();
	}

	pTableLegend->setTextStyle(objIDTxtStyle, AcDb::kAllRows);
	pTableLegend->setAlignment(AcDb::kMiddleCenter, AcDb::kAllRows);
	pTableLegend->generateLayout(); // Very very important, else expect crashes later on

	// Row 2: Headers for the columns
	double  dWidth = 1; CString csWidth;
	int iCol = 1;
	int iSubCol = 0;

	pTableLegend->setRowHeight(0, dRowHeight * 1.0);
	pTableLegend->setRowHeight(1, dRowHeight * 1.0);
	pTableLegend->setRowHeight(2, dRowHeight * 3.0);
	

	// Row 1: Label
	pTableLegend->mergeCells(1, 2, 0, 0); 
	if (GetParameterValue(g_csaLightTblTitles, _T("ITEM ID"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->setColumnWidth(0, dWidth); setTextString(pTableLegend, 1, 0, objIDTxtStyle,AcDb::kDegrees000, _T("ITEM ID"), dlgTC.m_csHeight);

	// Row 1: Label
	if (GetParameterValue(g_csaLightTblTitles, _T("STATE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol);   setTextString(pTableLegend, 1, iCol, objIDTxtStyle,AcDb::kDegrees000, _T("STATE"), dlgTC.m_csHeight); iCol++;

	//SL NUMBER	
	
	if (GetParameterValue(g_csaLightTblTitles, _T("NEW"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth);
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("SL NUMBER"), dlgTC.m_csHeight);
	setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("NEW"), dlgTC.m_csHeight); iCol++;


	//SUPPORT NUMBER	
	
	if (GetParameterValue(g_csaLightTblTitles, _T("EXISTING"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth);
	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("SUPPORT NUMBER"), dlgTC.m_csHeight);
	setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("EXISTING"), dlgTC.m_csHeight); iCol++;

	if (bHasLaternDetails)
	{
		// Sub columns
		iSubCol = iCol;

		if (GetParameterValue(g_csaLightTblTitles, _T("DESCRIPTION"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("DESCRIPTION"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("PART NO"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("PART NO"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("QTY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("QTY"), dlgTC.m_csHeight); iCol++;					     

		if (GetParameterValue(g_csaLightTblTitles, _T("RATE CODE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("RATE CODE"), dlgTC.m_csHeight);	iCol++; 
/*
		if (GetParameterValue(g_csaLightTblTitles, _T("Existing"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("Existing"), dlgTC.m_csHeight); iCol++;							 
		
		if (GetParameterValue(g_csaLightTblTitles, _T("RMVD"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("RMVD"), dlgTC.m_csHeight); iCol++;	*/						 

		// Row 1: Label 
		pTableLegend->mergeCells(1, 1, iSubCol, iCol - 1);
		setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("LUMINAIRE"), dlgTC.m_csHeight);
		//setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("LANTERN"), dlgTC.m_csHeight);
	}

	if (bHasColumnDetails)
	{
		// Sub columns
		iSubCol = iCol;
		if (GetParameterValue(g_csaLightTblTitles, _T("DESCRIPTION"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("DESCRIPTION"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("PART NO"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("PART NO"), dlgTC.m_csHeight);	iCol++; 

		if (GetParameterValue(g_csaLightTblTitles, _T("FOOTING (BOLT/FOUNDATION)"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;		
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("FOOTING (BOLT/FOUNDATION)"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("QTY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("QTY"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("RATE CODE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("RATE CODE"), dlgTC.m_csHeight); iCol++;

		// Row 1: Label
		pTableLegend->mergeCells(1, 1, iSubCol, iCol - 1); 
		setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("COLUMN/POLE"), dlgTC.m_csHeight);
		//setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("COLUMN"), dlgTC.m_csHeight);
	}

	if (bHasPoleDetails)
	{
		// Sub columns
		iSubCol = iCol;
		//if (GetParameterValue(g_csaLightTblTitles, "TYPE", csWidth, 0) >= 0) dWidth = atof(csWidth); else dWidth =  dCWidth1;
		//pTableLegend->insertColumns(iCol, dWidth); setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("TYPE")); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("DESCRIPTION"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("DESCRIPTION"), dlgTC.m_csHeight);	iCol++; 

		if (GetParameterValue(g_csaLightTblTitles, _T("PART NO"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("PART NO"), dlgTC.m_csHeight); iCol++;

		if (GetParameterValue(g_csaLightTblTitles, _T("RATE CODE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("RATE CODE"), dlgTC.m_csHeight); iCol++;

		/*if (GetParameterValue(g_csaLightTblTitles, _T("MH"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); 
		setTextString(pTableLegend, 2, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("MH"), dlgTC.m_csHeight); iCol++;*/

		// Row 1: Label
		pTableLegend->mergeCells(1, 1, iSubCol, iCol - 1); 
		setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("BRACKET/OUTREACH"), dlgTC.m_csHeight);
		//setTextString(pTableLegend, 1, iSubCol, objIDTxtStyle, AcDb::kDegrees000, _T("POLE"), dlgTC.m_csHeight);
	}

	//// Row 1: Label
	//pTableLegend->mergeCells(1, 2, 0, 0); 
	//if (GetParameterValue(g_csaLightTblTitles, _T("ITEM ID"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//pTableLegend->setColumnWidth(0, dWidth); setTextString(pTableLegend, 1, 0, objIDTxtStyle,AcDb::kDegrees090, _T("ITEM ID"), dlgTC.m_csHeight);

	// Row 1: Label
	//if (GetParameterValue(g_csaLightTblTitles, _T("QTY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol);   setTextString(pTableLegend, 1, iCol, objIDTxtStyle,AcDb::kDegrees090, _T("QTY"), dlgTC.m_csHeight); iCol++;

	//if (bHasTypeCode)
	//{
	//	if (GetParameterValue(g_csaLightTblTitles, _T("TARIFF CLASS"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("TARIFF CLASS"), dlgTC.m_csHeight); iCol++;
	//}

	//if (bHasRateCode) 
	//{
	//	/*if (GetParameterValue(g_csaLightTblTitles, _T("RATE CODE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("RATE CODE"), dlgTC.m_csHeight); iCol++;*/

	//	if (GetParameterValue(g_csaLightTblTitles, _T("LUMINAIRE RATE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("LUMINAIRE RATE"), dlgTC.m_csHeight); iCol++;

	//	if (GetParameterValue(g_csaLightTblTitles, _T("POLE/COLUMN RATE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("POLE/COLUMN RATE"), dlgTC.m_csHeight); iCol++;

	//	if (GetParameterValue(g_csaLightTblTitles, _T("OUTREACH/BRACKET RATE"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("OUTREACH/BRACKET RATE"), dlgTC.m_csHeight); iCol++;
	//}

	//if (bHasCategory)
	//{
	//	if (GetParameterValue(g_csaLightTblTitles, _T("LIGHTING CAT."), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	//	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); 
	//	setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees090, _T("LIGHTING CAT."), dlgTC.m_csHeight); iCol++;
	//}

	if (GetParameterValue(g_csaLightTblTitles, _T("UPCAST"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
	pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("UPCAST"), dlgTC.m_csHeight); iCol++;

	if (bHasCategory)
	{
		if (GetParameterValue(g_csaLightTblTitles, _T("CATEGORY"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("CATEGORY"), dlgTC.m_csHeight); iCol++;
	}
	if (bHasCharge)
	{
		if (GetParameterValue(g_csaLightTblTitles, _T("CHARGE TO"), csWidth, 0) >= 0) dWidth = _tstof(csWidth); else dWidth =  dCWidth1;
		pTableLegend->insertColumns(iCol, dWidth); pTableLegend->mergeCells(1, 2, iCol, iCol); 
		setTextString(pTableLegend, 1, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("CHARGE TO"), dlgTC.m_csHeight); iCol++;
	}

	// Row 1: Table Header
	pTableLegend->mergeCells(0, 0, 0, pTableLegend->numColumns() - 1);
	//setTextString(pTableLegend, 0, 0, objIDTxtStyle, AcDb::kDegrees000, _T("NON-CONTESTABLE LIGHTING SCHEDULE"), dlgTC.m_csHeight);
	setTextString(pTableLegend, 0, 0, objIDTxtStyle, AcDb::kDegrees000, _T("LIGHTING EQUIPMENT SCHEDULE"), dlgTC.m_csHeight);

	int iSize;
	int iStart = 0;

	CString csLightNumber;
	for (int iCtr = 0, iRow = 3; iCtr < csaLightNumber.GetSize(); iCtr++, iRow++)
	{

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Light numbers are to be consolidated like L1,L3-L5 etc. In the string right now it would be in the format. Say L1,L3,L5 etc.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Get all the light numbers (i.e the integer part) into an array
		int *array;	array = new int[_ttoi(csaQty.GetAt(iCtr))];
		csLightNumber = csaLightNumber.GetAt(iCtr);
		iSize = 0;
		while (csLightNumber.Find(_T(",")) != -1)
		{
			array[iSize]  = _ttoi(csLightNumber.Mid(1, csLightNumber.Find(_T(",")) - 1)); 
			iSize++;
			csLightNumber = csLightNumber.Mid(csLightNumber.Find(_T(",")) + 1);
		}
		array[iSize] = _ttoi(csLightNumber.Mid(1)); iSize++;
		
		// Now consolidate them like L1,L3-L5 etc. (if any)
		csLightNumber.Empty();
		int iThis, iNext;
		bool bAdded = false;
		iStart = array[0];
		CString csLTemp;

		int iPrev;
		csLightNumber.Format(_T("L%d%s"), iStart, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")));
		for (int iCnt = 1; iCnt < iSize; iCnt++)
		{
			iThis = array[iCnt];
			iPrev = array[iCnt - 1];
			if (iThis > (iPrev + 1)) 
			{ 
				//////////////////////////////////////////////////////////////////////////
				// Not in sequence
				//////////////////////////////////////////////////////////////////////////
				if (iPrev == iStart) // 57 < 52
					csLTemp.Format(_T(", L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 
				else 
					csLTemp.Format(_T(" - %d%s, L%d%s"), iPrev, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")), 
					iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T("")) 
					); 

				iStart = iThis;
				csLightNumber += csLTemp;
			}
			else if (iCnt == iSize - 1)
			{
				// If we are are at the end of the loop add the last number
				if (iThis == iPrev + 1)
					csLTemp.Format(_T("-L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 
				else
					csLTemp.Format(_T(", L%d%s"), iThis, ((!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove"))) ? _T("r") : _T(""))); 

				csLightNumber += csLTemp;
			}
		}
		delete[] array;

		//////////////////////////////////////////////////////////////////////////
		// Write the light number text finally
		//////////////////////////////////////////////////////////////////////////
		iCol = 0;
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csLightNumber, dlgTC.m_csHeight); iCol++;
		csLTemp.Format(_T("%d"),iCtr+1) ;
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csLTemp, dlgTC.m_csHeight); iCol++;// ITEM ID

		//STATE
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternStatus.GetAt(iCtr), dlgTC.m_csHeight); iCol++;

		//SL NUMBER (NEW)		
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLightNumber.GetAt(iCtr), dlgTC.m_csHeight); iCol++;//_T("SL NUMBER (NEW)")

		//Support Number
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csAssetLantern.GetAt(iCtr), dlgTC.m_csHeight); iCol++;

		//////////////////////////////////////////////////////////////////////////
		// Write the other columns now
		//////////////////////////////////////////////////////////////////////////
		if (bHasLaternDetails)
		{
			// Description
			if (csaLanternStatus.GetAt(iCtr) == _T("Existing"))
			{
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); 
			}			
			else if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), dlgTC.m_csHeight); 
			else 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); 
			iCol++;

			// Part No
			if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternStockCode.GetAt(iCtr), dlgTC.m_csHeight); 
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); 
			iCol++;

			// QTY
			/*if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLaternLamp.GetAt(iCtr), dlgTC.m_csHeight);
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); */

			//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); //Since no value QTY is left blank
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaQty.GetAt(iCtr) , dlgTC.m_csHeight); 
			iCol++;

			// Rate code
			/*if (csaLanternStatus.GetAt(iCtr) != _T("Remove")) 
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLampStockCode.GetAt(iCtr), dlgTC.m_csHeight); 
			else
				setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); */
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLuminaireRate.GetAt(iCtr), dlgTC.m_csHeight);

			iCol++;

			//iCol++;

			//// RMVD
			//if (csaLanternStatus.GetAt(iCtr) == _T("Remove"))
			//	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternType.GetAt(iCtr), dlgTC.m_csHeight); 
			//else
			//	setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); 
			//iCol++;
		}

		if (bHasColumnDetails)
		{
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternMast.GetAt(iCtr), dlgTC.m_csHeight); iCol++;//DESCRIPTION
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaMastCode.GetAt(iCtr), dlgTC.m_csHeight); iCol++;//PART NO
			CString temp;
			temp.Format(csaRagbolt.GetAt(iCtr) + "/" + csaFoundationType.GetAt(iCtr));
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, temp, dlgTC.m_csHeight); iCol++;			
			//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T(""), dlgTC.m_csHeight); iCol++;//QTY
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000,csaColumnQty.GetAt(iCtr) , dlgTC.m_csHeight); iCol++;//QTY
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaMASTRATE.GetAt(iCtr), dlgTC.m_csHeight); iCol++;//RATE CODE
		}

		if (bHasPoleDetails)
		{		
			
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternOutreach.GetAt(iCtr), dlgTC.m_csHeight); iCol++;
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachCode.GetAt(iCtr), dlgTC.m_csHeight); iCol++;
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachRate.GetAt(iCtr) , dlgTC.m_csHeight); iCol++;
			//iCol++;
		}

		//UPCAST
		//Default value for upcast is 5°
		setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, _T("5°"), dlgTC.m_csHeight); 
		iCol++;

		//CATEGORY
		if (bHasCategory) 
		{
			setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternCategory.GetAt(iCtr), dlgTC.m_csHeight); 
			iCol++;
		}
		//// Quantity
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, (!csaLanternStatus.GetAt(iCtr).CompareNoCase(_T("Remove")) ? _T("-") : _T("")) + csaQty.GetAt(iCtr), dlgTC.m_csHeight); iCol++;
		
		////Tariff Class
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaTariffClass.GetAt(iCtr), dlgTC.m_csHeight); 
		//iCol++;

		////Luminaire Rate
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLuminaireRate.GetAt(iCtr), dlgTC.m_csHeight); 
		//iCol++;

		////Pole/Column Rate
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaPoleColumnRate.GetAt(iCtr), dlgTC.m_csHeight); 
		//iCol++;

		////Outreach/Bracket Rate
		//setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaOutreachBracketRate.GetAt(iCtr), dlgTC.m_csHeight); 
		//iCol++;

		//if (bHasCategory) { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaLanternCategory.GetAt(iCtr), dlgTC.m_csHeight); iCol++; }

	
		if (bHasCharge)   { setTextString(pTableLegend, iRow, iCol, objIDTxtStyle, AcDb::kDegrees000, csaChargeTo.GetAt(iCtr), dlgTC.m_csHeight); iCol++; }
	}

	// Switch to the appropriate Layout in paper space
	acdbHostApplicationServices()->layoutManager()->setCurrentLayout(dlgTC.m_csLayout);
	
	// Add the table to the drawing
	AcDbBlockTable *pBlkTbl = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) { pTableLegend->close(); return; }

	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	if (pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite) != Acad::eOk) { pBlkTbl->close(); pTableLegend->close(); return; }
	pBlkTbl->close();

	double dTableHeight = pTableLegend->height();
	pBlkTblRcd->appendAcDbEntity(pTableLegend);
	double dTableWidth  = pTableLegend->width();
	
	// Attach XData to table so that it could be erased while removing frame numbers
	struct resbuf *rbpXData = acutBuildList(AcDb::kDxfRegAppName, _T("LIGHT_SCHEDULE"), AcDb::kDxfXdInteger16, 1, NULL);
	acdbRegApp(_T("LIGHT_SCHEDULE")); pTableLegend->setXData(rbpXData); acutRelRb(rbpXData);

	// Attach XData to table about the pole numbers in the table
	for (int iCtr = 0; iCtr < csaLightNumberXD.GetSize(); iCtr++)
	{
		struct resbuf *rbpLightNumber = acutBuildList(AcDb::kDxfRegAppName, csaLightNumberXD.GetAt(iCtr), AcDb::kDxfXdInteger16, 1, NULL);

		// Register the application and add the XDATA to the table.
		acdbRegApp(csaLightNumberXD.GetAt(iCtr));	pTableLegend->setXData(rbpLightNumber); acutRelRb(rbpLightNumber);
	}

	// Register the application and add the XDATA to the table.
	struct resbuf *rbpTblCfg = acutBuildList(AcDb::kDxfRegAppName, _T("TABLECFG"), 
																					 AcDb::kDxfXdInteger16, bHasLaternDetails,
																					 AcDb::kDxfXdInteger16, bHasColumnDetails,
																					 AcDb::kDxfXdInteger16, bHasPoleDetails,
																					// AcDb::kDxfXdInteger16, bHasTypeCode,
																					// AcDb::kDxfXdInteger16, bHasRateCode,
																					 AcDb::kDxfXdInteger16, bHasCategory,
																					 AcDb::kDxfXdInteger16, bHasCharge,
																					 NULL);

	// Register the application and add the XDATA to the table.
	acdbRegApp(_T("TABLECFG")); pTableLegend->setXData(rbpTblCfg); acutRelRb(rbpTblCfg);
	pBlkTblRcd->close();

	// If in Model space, switch to Model Space
	struct resbuf rbTileMode; rbTileMode.restype = RTSHORT; rbTileMode.resval.rint = 0; acedSetVar(_T("TILEMODE"), &rbTileMode);
	acedCommandS(RTSTR, _T(".PSPACE"), NULL);

	int iRet;
	ads_point ptTable;
	acedInitGet(RSG_NONULL, _T("")); 
	while (T)
	{
		iRet = acedGetPoint(NULL, _T("\nSpecify insertion point: "), ptTable);
		if (iRet == RTCAN) { pTableLegend->erase(); pTableLegend->close(); return; }
		else if (iRet == RTNORM) break;
	}

	pTableLegend->setPosition(AcGePoint3d(ptTable[X] - dTableWidth, ptTable[Y], 0.0));
	pTableLegend->setLayer(_T("NOTES"));
	pTableLegend->close();
}


void CTableColumnsDlg::OnLvnItemchangedTableList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
