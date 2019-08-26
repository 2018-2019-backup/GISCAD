////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : EE Toolbox
// Client name      : AEC Systems
// File name        : acrxEntryPoint.cpp
// Description      : ObjectARX entry point implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"

#define szRDS _RXST(_T(""))

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables used to hold database information, so that it need not be repeatedly accessed.
/////////////////////////////////////////////////////////////////////////////////////////////////////

bool g_bCallWipeOut;
CString g_csBaseFolder;
CString g_csPoleTextStyle, g_csPoleTextHeight, g_csPoleDiameter;
CString g_csSchTextStyle,  g_csSchTextHeight;
CStringArray g_csaPoleHVCat, g_csaPoleHVText, g_csaPoleLVCat, g_csaPoleLVText;
CStringArray g_csaPoleStays, g_csaPoleFootings, g_csaPoleBrackets;
CStringArray g_csaPoleSizes, g_csaPoleDepths, g_csaPoleDiameters;
CStringArray g_csaPoleTypes, g_csaPoleBlocks, g_csaPoleLayers, g_csaPoleLayerColors;
CStringArray g_csaPoleAttDescs, g_csaPoleAttTags;

CStringArray g_csaLanternCharges, g_csaLanternCategories;
CStringArray g_csaLanternBrackets, g_csaLanternBracketCodes;
CStringArray g_csaLanternMasts, g_csaLanternMastCodes;
CStringArray g_csaLanternOutreaches, g_csaLanternOutreachCodes;

CStringArray g_csaRagboltType,g_csaRagboltTypeCodes;
CStringArray g_csaFoundationType,g_csaFoundationTypeCodes;

CStringArray g_csaLanternTypes, g_csaLanternBlocks, g_csaLanternLayers, g_csaLanternLayerColors;
CStringArray g_csaColumnTypes, g_csaColumnBlocks, g_csaColumnLayers, g_csaColumnLayerColors;

CStringArray g_csaLanternSch1Types, g_csaLanternSch1Lamps, g_csaLanternSch1LampCodes, g_csaLanternSch1LanternCodes, g_csaLanternSch1RateCodes;
CStringArray g_csaLanternSch2Types, g_csaLanternSch2Lamps, g_csaLanternSch2LampCodes, g_csaLanternSch2LanternCodes, g_csaLanternSch2RateCodes;
CStringArray g_csaLanternSch3Types, g_csaLanternSch3Lamps, g_csaLanternSch3LampCodes, g_csaLanternSch3LanternCodes, g_csaLanternSch3RateCodes;
CStringArray g_csaLanternSch4Types, g_csaLanternSch4Lamps, g_csaLanternSch4LampCodes, g_csaLanternSch4LanternCodes, g_csaLanternSch4RateCodes;
CStringArray g_csaLanternSch5Types, g_csaLanternSch5Lamps, g_csaLanternSch5LampCodes, g_csaLanternSch5LanternCodes, g_csaLanternSch5RateCodes;
CStringArray g_csaLanternSchUnRegulatedTypes, g_csaLanternSchUnRegulatedLamps, g_csaLanternSchUnRegulatedLampCodes, g_csaLanternSchUnRegulatedLanternCodes, g_csaLanternSchUnRegulatedRateCodes;

CStringArray g_csaReplaceLanternSch1Types, g_csaReplaceLanternSch1Lamps, g_csaReplaceLanternSch1LampCodes, g_csaReplaceLanternSch1LanternCodes, g_csaReplaceLanternSch1RateCodes;
CStringArray g_csaReplaceLanternSch2Types, g_csaReplaceLanternSch2Lamps, g_csaReplaceLanternSch2LampCodes, g_csaReplaceLanternSch2LanternCodes, g_csaReplaceLanternSch2RateCodes;

CStringArray g_csaLightAttDescs, g_csaLightAttTags;
CStringArray g_csaColumnAttDescs, g_csaColumnAttTags;

CStringArray g_csaConductorNames, g_csaConductorLTypes, g_csaConductorLayers, g_csaConductorLayerColors;
CStringArray g_csaLightTblTitles, g_csaPoleTblTitles;

CString g_csMDBLocation;
CString g_csDWGLocation;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command functions implemented externally
extern void Command_ConvertPole();    // PoleDlg.cpp
extern void Command_InsertPole();     // PoleDlg.cpp
extern void Command_EditPole();       // PoleDlg.cpp

extern void Command_ConvertLight();   // LightDlg.cpp
extern void Command_InsertLight();    // LightDlg.cpp
extern void Command_EditLight();      // LightDlg.cpp

extern void Command_AttachCable();    // AttachCableDlg.cpp
extern void Command_AddBond();        // OtherCommands.cpp
extern void Command_TerminateCable(); // OtherCommands.cpp

extern void Command_RenumberPoles();  // OtherCommands.cpp
extern void Command_ResizePoleDia();  // OtherCommands.cpp
extern void Command_ResizePoleText(); // OtherCommands.cpp
extern void Command_Legend();         // OtherCommands.cpp

extern void Command_Circuit();        // OtherCommands.cpp
extern void Command_Help();           // OtherCommands.cpp

extern void Command_PoleTable();      // TableColumnsDlg.cpp
extern void Command_LightTable();     // TableColumnsDlg.cpp

extern void Command_Options();        // OptionsDlg.cpp

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ReadINIFile
// Description  : Reads the "Settings.ini" file for default settings
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ReadINIFile(CString csBaseFolder)
{
	CString csError, csErrorTitle = _T("EE Toolbox: Error in INI");

	// Check if the "Settings.ini" file is present in this location
	CString csINIFile; 
	csINIFile.Format(_T("%s\\Settings.ini"), csBaseFolder);
	if (_taccess(csINIFile, 00) == -1) 
	{ 
		csError.Format(_T("Unable to read default settings from\n%s\n\nThe file does not exist."), csINIFile);
		MessageBox(NULL, csError, csErrorTitle, MB_OK); 
		return FALSE;    
	}

	// Open the file for reading
	CString csLine;
	CStdioFile cfIni;
	CFileException cfe;
		
	if (cfIni.Open(csINIFile, CFile::modeRead, &cfe) == FALSE)
	{
		TCHAR szError[1024];
		cfe.GetErrorMessage(szError, 1024);
		csError.Format(_T("Error opening file for reading\n%s\n\n%s"), csINIFile, szError);
		MessageBox(NULL, csError, csErrorTitle, MB_OK); 
		return FALSE;
	}

	// Read the INI file for all the default values
	while (cfIni.ReadString(csLine) == TRUE)
	{
		// If this first character of the line is ';' or if this is an empty line, continue
		if ((csLine.GetAt(0) == _T(';')) || (csLine.IsEmpty() == TRUE)) continue;

		// Depending on what is the line read, set the appropriate variable while reading the following...
		// SRVR_STD_DWG=X:\CADGIS\Database\IE_CAD_Standards.dwg
		// SRVR_MDB_LOCATION=X:\CADGIS\Database\IE_Toolbox.mdb

		csLine = csLine.Trim();
		if (csLine.Mid(0,  12) == _T("SRVR_STD_DWG"))      { g_csDWGLocation  = csLine.Mid(csLine.Find(_T('=')) + 1); g_csDWGLocation.Trim(); } 
		if (csLine.Mid(0,  17) == _T("SRVR_MDB_LOCATION")) { g_csMDBLocation  = csLine.Mid(csLine.Find(_T('=')) + 1); g_csMDBLocation.Trim(); }
	}
	
	// Close the file
	cfIni.Close();

	// Success
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint class
//////////////////////////////////////////////////////////////////////////
class CAECSmartPoleApp : public AcRxArxApp 
{
	public:

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // The one and only constructor
	CAECSmartPoleApp() : AcRxArxApp() { }

	virtual AcRx::AppRetCode On_kLoadDwgMsg(void *pkt)
	{
		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);

		// Call the conversion utility to collect all the poles and attach XDATA reflecting the attribute in the pole
		Command_ConvertPole();

		// Call the conversion utility to collect all the lights and attach XDATA reflecting the attribute in the pole
		Command_ConvertLight();

		// Return the status
		return (retCode);
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // This method will be called by AutoCAD when the ARX is loaded
	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) 
  {
		CString csSQL, csFunc = _T("On_kInitAppMsg");

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// Get the location from which IE_ToolBox.arx is loaded
		TCHAR sDllPath[_MAX_PATH];
		GetModuleFileName(_hdllInstance, sDllPath, _MAX_PATH);
		CString csBaseFolder = sDllPath; csBaseFolder.MakeUpper();
		if (csBaseFolder.ReverseFind(_T('\\')) > 0) csBaseFolder = csBaseFolder.Mid(0, csBaseFolder.ReverseFind(_T('\\')));

		// Read the settings.ini file to find out the location of .DWG and .MDB files.
		if (!ReadINIFile(csBaseFolder)) return retCode;

		// Show a message
		acutPrintf(_T("\nVerifying standards drawing...\n"));
		if (checkFileAccess(g_csDWGLocation, 00) == FALSE) return retCode;

		g_csBaseFolder = csBaseFolder;

		// Show a message
		acutPrintf(_T("Pre-loading configuration data...[%s]\n"), g_csMDBLocation);
		if (checkFileAccess(g_csMDBLocation, 00) == FALSE) return retCode;
		if (setupDataSource(DSN_IETB, g_csMDBLocation) == FALSE) return retCode;
    		
		// Show a message
		acutPrintf(_T("Pre-loading configuration data...\n"));
		if (checkFileAccess(g_csMDBLocation, 00) == FALSE) return (retCode);
		if (setupDataSource(DSN_IETB, g_csMDBLocation) == FALSE) return (retCode);
    		
    // Read the configuration data and populate the global arrays
    CQueryTbl tblData;
    
    // Configuration
    csSQL.Format(_T("SELECT [PNTextStyle], [PNTextHeight], [PoleDia], [SchTextStyle], [SchTextHeight] FROM [tblConfig]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    g_csPoleTextStyle  = tblData.GetRowAt(0)->GetAt(0);
    g_csPoleTextHeight = tblData.GetRowAt(0)->GetAt(1);
    g_csPoleDiameter   = tblData.GetRowAt(0)->GetAt(2);
    g_csSchTextStyle   = tblData.GetRowAt(0)->GetAt(3);
    g_csSchTextHeight  = tblData.GetRowAt(0)->GetAt(4);

	  // Pole types
    csSQL.Format(_T("SELECT [PoleType], [PoleBlock], [PoleLayer], [PoleLayerColor] FROM [tblPoleStatus] WHERE [PoleType] <> 'Stay' ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleTypes);
    tblData.GetColumnAt(1, g_csaPoleBlocks);
    tblData.GetColumnAt(2, g_csaPoleLayers);
    tblData.GetColumnAt(3, g_csaPoleLayerColors);

    // Pole sizes
    csSQL.Format(_T("SELECT [PoleSize], [HoleDepth], [HoleDia] FROM [tblPoleTypes] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleSizes);
    tblData.GetColumnAt(1, g_csaPoleDepths);
    tblData.GetColumnAt(2, g_csaPoleDiameters);

    // Pole stays
    csSQL.Format(_T("SELECT [Stay] FROM [tblPoleStays] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleStays);

    // Pole footings
    csSQL.Format(_T("SELECT [Footing] FROM [tblPoleFootings] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleFootings);

    // Pole brackets
    csSQL.Format(_T("SELECT [Bracket] FROM [tblPoleBrackets] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleBrackets);

    // Pole HV values
    csSQL.Format(_T("SELECT [HVCategory], [HVText] FROM [tblPoleHV] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleHVCat);
    tblData.GetColumnAt(1, g_csaPoleHVText);

    // Pole LV values
    csSQL.Format(_T("SELECT [LVCategory], [LVText] FROM [tblPoleLV] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleLVCat);
    tblData.GetColumnAt(1, g_csaPoleLVText);

    // Pole attributes
    csSQL.Format(_T("SELECT [AttDesc], [AttTag] FROM [tblAttributes] WHERE [Block] = 'Pole'"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaPoleAttDescs); 
    tblData.GetColumnAt(1, g_csaPoleAttTags);

    // Lantern types
    csSQL.Format(_T("SELECT [LanternType], [LanternBlock], [LanternLayer], [LanternLayerColor] FROM [tblLanternStatus] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		
		tblData.GetColumnAt(0, g_csaLanternTypes);
		tblData.GetColumnAt(1, g_csaLanternBlocks);
		tblData.GetColumnAt(2, g_csaLanternLayers);
		tblData.GetColumnAt(3, g_csaLanternLayerColors);
		
    // Lantern brackets and codes
    csSQL.Format(_T("SELECT [Bracket], [Code] FROM [tblLanternBrackets] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternBrackets);
    tblData.GetColumnAt(1, g_csaLanternBracketCodes);

    // Lantern masts and codes
    csSQL.Format(_T("SELECT [Mast], [Code] FROM [tblLanternMasts] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternMasts);
    tblData.GetColumnAt(1, g_csaLanternMastCodes);

    // Lantern outreaches and codes
    csSQL.Format(_T("SELECT [Outreach], [Code] FROM [tblLanternOutreaches] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternOutreaches);
    tblData.GetColumnAt(1, g_csaLanternOutreachCodes);

	//Rag Bolt Type
	csSQL.Format(_T("SELECT [Description], [Code] FROM [tblRagBolt] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaRagboltType);
    tblData.GetColumnAt(1, g_csaRagboltTypeCodes);

	//Foundation Type
	csSQL.Format(_T("SELECT [Description], [Code] FROM [tblFoundation] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0,g_csaFoundationType);
    tblData.GetColumnAt(1,g_csaFoundationTypeCodes);


    // Lantern categories
    csSQL.Format(_T("SELECT [Category] FROM [tblLanternCategories] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternCategories);

    // Lantern charges
    csSQL.Format(_T("SELECT [ChargeTo] FROM [tblLanternCharges] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternCharges);

    // Column types
    csSQL.Format(_T("SELECT [ColumnType], [ColumnBlock], [ColumnLayer], [ColumnLayerColor] FROM [tblColumnStatus] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaColumnTypes);
    tblData.GetColumnAt(1, g_csaColumnBlocks);
    tblData.GetColumnAt(2, g_csaColumnLayers);
    tblData.GetColumnAt(3, g_csaColumnLayerColors);

    //// Lantern Schedule 1 details
    //csSQL.Format(_T("SELECT [Type], [Lamp], [LampCode], [LanternCode], [RateCode] FROM [tblLanternSchedule1] ORDER BY [ID]"));
    //if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    //tblData.GetColumnAt(0, g_csaLanternSch1Types);
    //tblData.GetColumnAt(1, g_csaLanternSch1Lamps);
    //tblData.GetColumnAt(2, g_csaLanternSch1LampCodes);
    //tblData.GetColumnAt(3, g_csaLanternSch1LanternCodes);
    //tblData.GetColumnAt(4, g_csaLanternSch1RateCodes);

// Tariff Class 1 details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClass1] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSch1Types);
    tblData.GetColumnAt(1, g_csaLanternSch1Lamps);
    tblData.GetColumnAt(2, g_csaLanternSch1LampCodes);
    tblData.GetColumnAt(3, g_csaLanternSch1LanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSch1RateCodes);

		// Replace Lantern Schedule 1 details
		csSQL.Format(_T("SELECT [Type], [Lamp], [LampCode], [LanternCode], [RateCode] FROM [tblReplaceLanternSchedule1] ORDER BY [ID]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		tblData.GetColumnAt(0, g_csaReplaceLanternSch1Types);
		tblData.GetColumnAt(1, g_csaReplaceLanternSch1Lamps);
		tblData.GetColumnAt(2, g_csaReplaceLanternSch1LampCodes);
		tblData.GetColumnAt(3, g_csaReplaceLanternSch1LanternCodes);
		tblData.GetColumnAt(4, g_csaReplaceLanternSch1RateCodes);

    //// Lantern Schedule 2 details
    //csSQL.Format(_T("SELECT [Type], [Lamp], [LampCode], [LanternCode], [RateCode] FROM [tblLanternSchedule2] ORDER BY [ID]"));
    //if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    //tblData.GetColumnAt(0, g_csaLanternSch2Types);
    //tblData.GetColumnAt(1, g_csaLanternSch2Lamps);
    //tblData.GetColumnAt(2, g_csaLanternSch2LampCodes);
    //tblData.GetColumnAt(3, g_csaLanternSch2LanternCodes);
    //tblData.GetColumnAt(4, g_csaLanternSch2RateCodes);

	// Tariff Class 2 details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClass2] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSch2Types);
    tblData.GetColumnAt(1, g_csaLanternSch2Lamps);
    tblData.GetColumnAt(2, g_csaLanternSch2LampCodes);
    tblData.GetColumnAt(3, g_csaLanternSch2LanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSch2RateCodes);

		// Replace Lantern Schedule 2 details
		csSQL.Format(_T("SELECT [Type], [Lamp], [LampCode], [LanternCode], [RateCode] FROM [tblReplaceLanternSchedule2] ORDER BY [ID]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		tblData.GetColumnAt(0, g_csaReplaceLanternSch2Types);
		tblData.GetColumnAt(1, g_csaReplaceLanternSch2Lamps);
		tblData.GetColumnAt(2, g_csaReplaceLanternSch2LampCodes);
		tblData.GetColumnAt(3, g_csaReplaceLanternSch2LanternCodes);
		tblData.GetColumnAt(4, g_csaReplaceLanternSch2RateCodes);

// Tariff Class 3 details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClass3] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSch3Types);
    tblData.GetColumnAt(1, g_csaLanternSch3Lamps);
    tblData.GetColumnAt(2, g_csaLanternSch3LampCodes);
    tblData.GetColumnAt(3, g_csaLanternSch3LanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSch3RateCodes);

	// Tariff Class 4 details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClass4] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSch4Types);
    tblData.GetColumnAt(1, g_csaLanternSch4Lamps);
    tblData.GetColumnAt(2, g_csaLanternSch4LampCodes);
    tblData.GetColumnAt(3, g_csaLanternSch4LanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSch4RateCodes);

// Tariff Class 5 details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClass5] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSch5Types);
    tblData.GetColumnAt(1, g_csaLanternSch5Lamps);
    tblData.GetColumnAt(2, g_csaLanternSch5LampCodes);
    tblData.GetColumnAt(3, g_csaLanternSch5LanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSch5RateCodes);

// Tariff Class Unregulated details
    csSQL.Format(_T("SELECT [Description], [LampType], [LampCode], [EEStockCode], [Code] FROM [tblTariffClassUnregulated] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLanternSchUnRegulatedTypes);
    tblData.GetColumnAt(1, g_csaLanternSchUnRegulatedLamps);
    tblData.GetColumnAt(2, g_csaLanternSchUnRegulatedLampCodes);
    tblData.GetColumnAt(3, g_csaLanternSchUnRegulatedLanternCodes);
    tblData.GetColumnAt(4, g_csaLanternSchUnRegulatedRateCodes);
    // Lantern attributes
    csSQL.Format(_T("SELECT [AttDesc], [AttTag] FROM [tblAttributes] WHERE [Block] = 'Light'"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaLightAttDescs);
    tblData.GetColumnAt(1, g_csaLightAttTags);

		// Column attributes
		csSQL.Format(_T("SELECT [AttDesc], [AttTag] FROM [tblAttributes] WHERE [Block] = 'Column'"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		tblData.GetColumnAt(0, g_csaColumnAttDescs);
		tblData.GetColumnAt(1, g_csaColumnAttTags);

    // Conductor details
    csSQL.Format(_T("SELECT [ConName], [ConLinetype], [ConLayer], [ConLayerColor] FROM [tblConductors] ORDER BY [ID]"));
    if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
    tblData.GetColumnAt(0, g_csaConductorNames);
    tblData.GetColumnAt(1, g_csaConductorLTypes); 
    tblData.GetColumnAt(2, g_csaConductorLayers);
    tblData.GetColumnAt(3, g_csaConductorLayerColors);

		// Light table column details
		csSQL.Format(_T("SELECT [ColumnTitle] & '#' & [Width] FROM [tblStreetLightTable] ORDER BY [ID]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		tblData.GetColumnAt(0, g_csaLightTblTitles);
		
		// Pole table column details
		csSQL.Format(_T("SELECT [GroupName] & '-' & [ColumnTitle] & '#' & [Width] FROM [tblPoleTable] ORDER BY [ID]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, csFunc)) return (retCode);
		tblData.GetColumnAt(0, g_csaPoleTblTitles);

		// Load these line types into the drawing
		acutPrintf(_T("Pre-loading required line types...\n"));

		// if (checkFileAccess(g_csBaseFolder + "\\IE_ToolBox.lin", 00) == FALSE) return (retCode);
		for (int iCtr = 0; iCtr < g_csaConductorLTypes.GetSize(); iCtr++) setLineType(g_csaConductorLTypes.GetAt(iCtr));

		// Attach database reactor for draw order purpose
		// acutPrintf(_T("Initializing drawing order..."));
		// attachReactor();

		/*
		// To call the WIPEOUTFIX command!
		acutPrintf(_T("\nLoading wipeout library...\n"));
		CString csWipeOutPath;  csWipeOutPath.Format(L"%s\\GISCAD.dll", g_csBaseFolder);
		CString csWipeOutPathU; csWipeOutPathU.Format(L"%s\\GISCADU.dll", g_csBaseFolder);

		int iRet;
		g_bCallWipeOut = false;
		if ((_taccess(csWipeOutPath, 00) != -1) && (_taccess(csWipeOutPathU, 00) != -1))
		{
			iRet = acedCommandS(RTSTR, L".NETLOAD", RTSTR, csWipeOutPath,  NULL); 
			if (iRet == RTNORM) 
			{
				g_bCallWipeOut = true;

				iRet = acedCommandS(RTSTR, L".NETLOAD", RTSTR, csWipeOutPathU, NULL); 
				if (iRet != RTNORM) { g_bCallWipeOut = false; acutPrintf(L"\nUnable to load \"%s\"!", csWipeOutPathU); }
			}
			else acutPrintf(L"\nUnable to load \"%s\"!", csWipeOutPath); 
		}
		else
			acutPrintf(L"\nOne or more wipeout libraries not found!");
		*/
		
    // Show a message 
    acutPrintf(_T("\nPre-load complete.\nEE ToolBox is ready for use!\n"));
				
    // Return the status
    return (retCode);
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // This method will be called by AutoCAD when the ARX is unloaded
	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
  {
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// Detach the reactors
		//clearReactor();

    // Return the status
		return (retCode);
	}
	
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // This method will be called by AutoCAD for COM servers
	virtual void RegisterServerComponents() 
  {
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Command methods
	static void AECSmartPole_IE_ConvertPole(void) { setOptionParameters(); Command_ConvertPole();		}
	static void AECSmartPole_IE_AddPole(void)     { setOptionParameters(); Command_InsertPole();     /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */} 
	static void AECSmartPole_IE_EditPole(void)    { setOptionParameters(); Command_EditPole();       /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}

	static void AECSmartPole_IE_AddLight(void)    { setOptionParameters(); Command_InsertLight();    /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}
	static void AECSmartPole_IE_EditLight(void)   { setOptionParameters(); Command_EditLight();      /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}

	static void AECSmartPole_IE_AttachCable(void) { setOptionParameters(); Command_AttachCable();    /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}
	static void AECSmartPole_IE_AddBond(void)     { setOptionParameters(); Command_AddBond();        /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}
	static void AECSmartPole_IE_AddTerm(void)     { setOptionParameters(); Command_TerminateCable(); /* if (g_bCallWipeOut) acedCommandS(RTSTR, L"WIPEOUTFIX", NULL); */}

	static void AECSmartPole_IE_Renumber(void)    { setOptionParameters(); Command_RenumberPoles();		}
	static void AECSmartPole_IE_ResizeDia(void)   { setOptionParameters(); Command_ResizePoleDia();		}
	static void AECSmartPole_IE_ResizeText(void)  { setOptionParameters(); Command_ResizePoleText();	}

	static void AECSmartPole_IE_PoleTable(void)   { setOptionParameters(); Command_PoleTable();			}
	static void AECSmartPole_IE_LightTable(void)  { setOptionParameters(); Command_LightTable();		}

	static void AECSmartPole_IE_Options(void)     { Command_Options();									}
	static void AECSmartPole_IE_Legend(void)	  { setOptionParameters(); Command_Legend();			}

	static void AECSmartPole_IE_Circuit(void)     { setOptionParameters(); Command_Circuit();			}

	static void AECSmartPole_IE_Help(void)        { Command_Help();										}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entry point declaration macro
IMPLEMENT_ARX_ENTRYPOINT(CAECSmartPoleApp)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command declaration macros

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_AddPole,     EE_AddPole,     ACRX_CMD_MODAL,  NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_EditPole,    EE_EditPole,    ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_AddLight,    EE_AddLight,    ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_EditLight,   EE_EditLight,   ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_AttachCable, EE_AttachCable, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_AddBond,     EE_AddBond,     ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_AddTerm,     EE_AddTerm,     ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_Renumber,    EE_Renumber,    ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_ResizeDia,   EE_ResizeDia,   ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_ResizeText,  EE_ResizeText,  ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_PoleTable,   EE_PoleTable,   ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_LightTable,  EE_LightTable,  ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_Options,     EE_Options,    ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_Legend,      EE_Legend,     ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_Circuit,     EE_Circuit,    ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CAECSmartPoleApp, AECSmartPole, _IE_Help,        EE_Help,       ACRX_CMD_MODAL, NULL)
