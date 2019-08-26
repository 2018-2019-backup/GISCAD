// GISCADConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "time.h"
#include "windows.h"
#include "shellapi.h"
#include <ODBCINST.H>

#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSReportWriter.h"

#include "NSImportCAD.h"
#include "NSXMLParser.h"
#include "NSConverterUtils.h"
#include "NSExportCAD.h"


//--------------------------------------------------
//#include "afxwin.h"
#include "shlwapi.h"
#include "urlmon.h"
#include "Wininet.h"
#include "adesk.h"
#include "dbsymtb.h"
#include "dbents.h"
#include "dbelipse.h"
#include "dbspline.h"
#include "dblead.h"
#include "dbray.h"
#include "dbxline.h"
#include "dbmline.h"
#include "dbbody.h"
#include "dbregion.h"
#include "dbsol3d.h"
#include "acgi.h"

#include "acestext.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "rxregsvc.h"

#include "acdbxref.h"
#include "dbacis.h"

#include "windows.h"

#include "dbents.h"
#include "dbhatch.h"
#include "dbimage.h"
#include "imgdef.h"
#include "imgent.h"
#include "dbole.h"
#include "dbfcf.h"
#include "dbproxy.h"

#include "dbapserv.h"
#include "dbmstyle.h"
#include "gelnsg3d.h"
#include "tchar.h"

#include "StdAfx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "tchar.h"
#include "dbsymutl.h"
#include "dbid.h" 
#include "dbapserv.h"
#include "dbmain.h"
#include "acdbabb.h"
#include "dblayout.h"
#include "AcDbLMgr.h"
#include "dbents.h"
#include "dbdict.h"
#include "dbMPolygon.h"
#include "dbidmap.h"
#include "summinfo.h"
#include "dbxrecrd.h"
#include "sstream"
#include <atlstr.h>
#include "dbdynblk.h"

#include "dbregion.h"
#include "dblead.h"
#include "dbmleader.h"
#include "dbmleaderstyle.h"
#include "acgiviewport.h"
#include "dbtable.h"
#include "dbsol3d.h"

#include "dbmstyle.h"
//#include "acedads.h"

#include "acdb.h"

#define DEFAULT_TEXT_HEIGHT 1.8

// Multiline change request start
#include "dbmline.h"

#include <dbobjptr.h>


//--------------------------------------------------



#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

#define CONFIG_NAME _T("\\settings.ini")
#define SCHEMA_NAME _T("\\giscadloader ie_giscadloader.xsd")

int nExportedFeatures;
int nImportedNewFeatures;
int nImportedModFeatures;
int nImportedDelFeatures;
NSSTRING strReportName;

int gRelFlag = 2;


/*!
@Brief			This function returns the Schema path
@Return			Schema Location
@Date			12-06-2007
*/
NSSTRING getSchemaPath()
{
	// get configuration file path
	TCHAR szAppPath[_MAX_PATH + 1];
	GetModuleFileName( NULL, szAppPath, _MAX_PATH );
	NSSTRING strXSDPath(szAppPath);
	size_t nIndex = strXSDPath.find_last_of(_T("\\"));
	strXSDPath = strXSDPath.substr(0, nIndex);
	strXSDPath = strXSDPath.append(SCHEMA_NAME);
	return strXSDPath;
}

/*!
@Brief			This function converts from input XML file to output DWG file
@Param [in]		szSourceFileName		input XML file name
@Param [in]		szDestFileName		    name of the output DWG file
@Param [in]		nPrecision		        precision to be set in the generated drawing.
@Param [in]		bSaveAs2007		        flag to be save generated drawing in 2007 format.
@Return			NS_SUCCESS : If Conversion process successful
				NS_FAIL    : If conversion process not successful
				ERR_XML_LOADING : Problem while loading the xml file.
@Date			12-06-2007
*/
int XMLToDWG(TCHAR* szSourceFileName, TCHAR *szDestFileName, TCHAR* szDwgName, int nPrecision, double dTextHeight, bool bSaveAs2007)
{
	CNSDWGMgr DWGMgr;
	CNSDatabaseMgr *pDatabase = CNSDatabaseMgr::getInstance();
	if(NS_SUCCESS != pDatabase->openDatabase(_T(""), _T(""), _T("DSN=GISCAD")))
	{
		WriteToLog(LOG_OTHER, _T("Could not establish database connection."));
		return NS_FAIL;
	}
	int nRetVal = NS_FAIL;
	CNSExportCAD oExport;
	oExport.setObject(&DWGMgr);
	oExport.setObject(pDatabase);
	oExport.setDestinationFileName(szDestFileName);
	oExport.setSourceFileName(szSourceFileName);
	
	CNSXMLParser xmlParser;
	if(true != xmlParser.loadFile(szSourceFileName))
	{
		WriteToLog(LOG_OTHER, _T("XML file not loaded"));
		return NS_FAIL;
	}
	
	NSSTRING strXSDPath;
	strXSDPath = getSchemaPath();
	if(false == xmlParser.validate(strXSDPath))
	{
		return ERR_XML_LOADING;
	}
	
	CNSTElement *pRoot = xmlParser.getRootElement();
	if(NULL == pRoot)
	{
		return NS_FAIL;
	}
	oExport.setObject(pRoot);
	// if XML file is not read successfully then return
	if(NS_SUCCESS != oExport.readXML())
		return nRetVal;
	if(NS_SUCCESS == DWGMgr.openDWG(szDwgName, true))
	{
		if(nPrecision != -1)
			DWGMgr.setDatabasePrecision(nPrecision);
		DWGMgr.setTextHeight(dTextHeight);
		int j=0;
		if(NS_SUCCESS != oExport.writeDWG())
			j=1;//return nRetVal;
		nExportedFeatures = oExport.getFeatureCount();
		//DWGMgr.purge();
		nRetVal = DWGMgr.saveCurrentDWG(szDestFileName,false);// !bSaveAs2007);
		DWGMgr.closeDatabase();
	}
	strReportName = oExport.getReportName();
	pDatabase->closeDatabase();
	if(pRoot) 
	{
		delete pRoot;
		pRoot = NULL;
	}
	return nRetVal;	
}

/*!
@Brief			This function returns the configuration file location
@Return			NSSTRING INI file location
@Date			01-08-2007
*/
NSSTRING getConfigFileLocation()
{
	CNSConverterUtils oConverterUtils;
	TCHAR szAppPath[_MAX_PATH + 1];
	GetModuleFileName( NULL, szAppPath, _MAX_PATH );
	NSSTRING strAppDirName = oConverterUtils.getDirectoryName(szAppPath);
	NSSTRING strConfigPath(strAppDirName);
	strConfigPath = strConfigPath.append(CONFIG_NAME);
	return strConfigPath;
}

/*!
@Brief			This function creates the DSN, if DSN is not available.
@Param [in]		szMDBLoc		database location 
@Return			return true if DSN added successful else return false.
@Date			12-06-2007
*/
BOOL createDSN(TCHAR* szMDBLoc)
{
	TCHAR szDesc[_MAX_PATH + 1];
	TCHAR szAttributes[_MAX_PATH + 1];
	//Use Hexadecimal 'FF' (=255) as temporary place holder
	wsprintf(szDesc, L"DSN=GISCAD \xFF DESCRIPTION=TEST DES \xFF DBQ=%s\xFF \xFF ", szMDBLoc);
	int mlen = (int)wcslen(szDesc);
	int i = 0;
	int j = 0;
	//Loop to replace "FF" by "\0"(so as to store multiple strings into one):
	while (i < mlen - 1)
	{
		if ((szDesc[i] == L'\xFF') && (szDesc[i + 1] == L' '))
		{
			szAttributes[j] = '\0';
			i++;
		}
		else
		{
			szAttributes[j] = szDesc[i];
		}
		i++;
		j++;
	}
	BOOL bRtnCode = SQLConfigDataSourceW(NULL, ODBC_ADD_SYS_DSN, L"Microsoft Access Driver (*.mdb, *.accdb)\0", (LPCWSTR)szAttributes);
	if (!bRtnCode)
	{
		WORD iError = 1;
		DWORD pfErrorCode;
		TCHAR lpszErrorMsg[301];
		WORD cbErrorMsgMax = 300;
		RETCODE rc;

		while (SQL_NO_DATA != (rc = SQLInstallerError(iError++,
			&pfErrorCode, lpszErrorMsg, cbErrorMsgMax, NULL)))
			if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
				MessageBox(NULL, lpszErrorMsg, _T("SQL Error"), MB_OK);
	}
	return 0;
}

/*!
@Brief			This function add the report path in SUMMARY.txt file.
@Param [in]		szDestPath		    Destination file path
@Param [in]		strReportLoc		Log file location.
@Return			void
@Date			01-08-2007
*/

void AddLogPathToSummaryReport(NSSTRING szDestPath, NSSTRING strReportLoc)
{
	szDestPath = szDestPath.substr(0,szDestPath.find_last_of(_T("\\")));
	NSSTRING szSummaryFile;
	FILE* fp=NULL;

	int nIndex = (int)szDestPath.find_last_of(_T("\\"));
	int nLength = (int)szDestPath.length();

	if(nIndex != nLength - 1)
		szDestPath=szDestPath+_T("\\");
	szSummaryFile=szDestPath + (_T("SUMMARY.TXT"));	
	NSFILEOPEN(&fp,szSummaryFile.c_str(),_T("a+"));
	if(fp!=NULL)
	{
		fputws(strReportLoc.c_str(),fp);
		fputws(_T("\n"),fp);
	}
	if(fp != NULL)
		fclose(fp);
}

/*!
@Brief			This function converts from input DWG file into output XML file
@Param [in]		szSourceFileName		input Drawing file name
@Param [in]		szDestFileName		    output xml file name to generate. 
@Return			NS_SUCCESS  :  If Conversion process successful
				NS_FAIL     :  If Conversion process not successful
@Date			12-06-2007
*/

int DWGToXML(TCHAR* szSouceFileName, TCHAR *szDestFileName)
{
	CNSImportCAD oImportCAD;
	CNSXMLParser oParser;
	bool bPartialSuccess = false;

	oImportCAD.setDestinationFileName(szDestFileName);
	oImportCAD.setSourceFileName(szSouceFileName);
	CNSDWGMgr DWGMgr;
	CNSDatabaseMgr *pDatabase = CNSDatabaseMgr::getInstance();
	if(NS_SUCCESS != pDatabase->openDatabase(_T(""), _T(""), _T("DSN=GISCAD")))
	{
		WriteToLog(LOG_OTHER, _T("Could not establish database connection."));
		return NS_FAIL;
	}

	oImportCAD.setObject(&DWGMgr);
	oImportCAD.setObject(pDatabase);

	// Read DWG
	if(NS_SUCCESS == DWGMgr.openDWG(szSouceFileName, true))
	{	
		int nRetVal = oImportCAD.readDWG();
		//Commented by kumanan - issue - this should be taken care by validation
		if(-1001 == nRetVal)
		{
			WriteToLog(LOG_OTHER, _T("Validation stamp is not available"));
			//DWGMgr.closeDatabase();
			//return -1001;
		}
		else if(-1002 == nRetVal)
		{
			//TCHAR szFormat[_MAX_PATH + 1];
			//NSSPRINTF(szFormat, _T("The drawing:\'%s\' is not GIS compliant. Do you want to continue?"), szSouceFileName);
			//if(IDNO == MessageBox(NULL, szFormat, _T("DWGtoXML"), MB_ICONWARNING|MB_YESNO))
			//{
			//	WriteToLog(LOG_OTHER, _T("Drawing is not GIS compliant."));
			//	return NS_FAIL; // User don't want to continue
			//}
			//else 
			//	bPartialSuccess = true;
			WriteToLog(LOG_WARNINGS, _T("Drawing is not GIS compliant."));
		}
		else if(-1004== nRetVal)
		{
			if(MessageBox(NULL,  _T("Drawing is missing header information required for GIS Import - Do you want to Write XML?"),_T("Converting..."), MB_OKCANCEL)==IDOK)
			{
			}
			else
			{
				return NS_FAIL;
			}
		}
		DWGMgr.ReleaseHostDwg();
		DWGMgr.closeDatabase();		
		
	}
	else
	{
		TCHAR szFormat[_MAX_PATH + 1];
		NSSPRINTF(szFormat, _T("File sharing violation: %s"), szSouceFileName);
		WriteToLog(LOG_OTHER, szFormat);
		DWGMgr.closeDatabase();
		return NS_FAIL;
	}
	// Write XML
	oParser.createFile();
	oParser.createRootElement(_T("IEGISCADInterchange"),_T("http://www.integral.com.au/giscadloader"));
	CNSTElement *pRoot = NULL;
	pRoot = oParser.getRootElement();
	pRoot->setAttribute(_T("xmlns:gml"), _T("http://www.opengis.net/gml"));
	pRoot->setAttribute(_T("xmlns:xsi"), _T("http://www.w3.org/2001/XMLSchema-instance"));
	pRoot->setAttribute(_T("xsi:schemaLocation"), _T("http://www.integral.com.au/giscadloader ie_giscadloader.xsd"));
	CNSTElement oImportNode = pRoot->createNode(_T("GISImport"),_T(""), DEFAULT_NAMESPACE);
	oImportCAD.setElement(&oImportNode);
	if(NS_SUCCESS != oImportCAD.writeXML())
	{
		WriteToLog(LOG_OTHER, _T("Problem in Writing XML"));
		return NS_FAIL;
	}
	NSSTRING strXSDPath;
	strXSDPath = getSchemaPath();
	::SetFileAttributes(szDestFileName,FILE_ATTRIBUTE_NORMAL);
	oParser.save(szDestFileName);
	nImportedDelFeatures = oImportCAD.getDelFeatures();
	nImportedModFeatures = oImportCAD.getModFeatures();
	nImportedNewFeatures = oImportCAD.getNewFeatures();

	strReportName = oImportCAD.getReportLocation();
	delete pRoot;
	if(bPartialSuccess == true)
		return -1003;
	return NS_SUCCESS;
}

/*!
@Brief			This function writes header information into Report
@Param [in]		szSourcePath			XML file name
@Param [in]		szDestinationPath		name of dwg file to generate.
@Param [in]		szStdDwgLoc				standard drawing location.
@Param [in]		nConversion				conversion whether it is XML2DWG or DWG2XML
@Return			void
@Date			15-06-2007
*/
void writeHeaderInformation(TCHAR* szSourcePath, TCHAR* szDestinationPath, TCHAR* szStdDwgLoc, int nConversion)
{
	TCHAR szDate[_MAX_PATH + 1];
	TCHAR szTime[_MAX_PATH + 1];
	NSDATE(szDate);
	NSTIME(szTime);
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("Report Creation date and time          : %s %s "), szDate, szTime);
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Source File                             : %s "), szSourcePath);
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Destination File                       : %s "), szDestinationPath);
	WriteToLog(LOG_HEADER, szFormat);
	NSSPRINTF(szFormat, _T("Standard Drawing file location         : %s "), szStdDwgLoc);
	WriteToLog(LOG_HEADER, szFormat);
	if(CNSConverterUtils::UTILS_XML2DWG == nConversion)
	{
		NSSPRINTF(szFormat, _T("Total Number of Features created       : %d "), nExportedFeatures);
		WriteToLog(LOG_HEADER, szFormat);
	}
	else if(CNSConverterUtils::UTILS_DWG2XML == nConversion)
	{
		NSSPRINTF(szFormat, _T("Number of New Features Imported        : %d "), nImportedNewFeatures);
		WriteToLog(LOG_HEADER, szFormat);
		NSSPRINTF(szFormat, _T("Number of Modified Features Imported   : %d "), nImportedModFeatures);
		WriteToLog(LOG_HEADER, szFormat);
		NSSPRINTF(szFormat, _T("Number of Deleted Features Imported    : %d "), nImportedDelFeatures);
		WriteToLog(LOG_HEADER, szFormat);
	}
	WriteToLog(LOG_HEADER, _T("-----------------------------------------------------------------------------------------------"));
}

// Main Function
int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL); 
	TCHAR* szSourcePath;
	TCHAR szDestinationPath[_MAX_PATH + 1]; 
	CNSConverterUtils oConverterUtils;
	CNSValidator oValidator;
	bool bFlag = false;
	
	//MessageBox(NULL, _T("1"), _T("Test"), MB_OKCANCEL);
	
	// get configuration file path
	TCHAR szAppPath[_MAX_PATH + 1];
	GetModuleFileName( NULL, szAppPath, _MAX_PATH );
	NSSTRING strAppDirName = oConverterUtils.getDirectoryName(szAppPath);
	NSSTRING strConfigPath(strAppDirName);
	strConfigPath = strConfigPath.append(CONFIG_NAME);

	// get default report location path
	NSSTRING strDefReportName(_T("\\ErrorReport.log"));
	NSSTRING strDefReportLoc(strAppDirName);
	strDefReportLoc.append(strDefReportName);


	//Check for config file presence
	if(NS_SUCCESS != oValidator.isFilePresent((TCHAR*)strConfigPath.c_str()))
	{
		writeHeaderInformation(_T(""), _T(""), _T(""), -1);
		WriteToLog(LOG_OTHER, _T("Configuration file is not available."));
		CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)strDefReportLoc.c_str());
		return NS_FAIL;
	}

	oConverterUtils.setConfigFilePath((TCHAR*)strConfigPath.c_str());

	TCHAR szRelFlag[_MAX_PATH + 1];
	if(true == oConverterUtils.getRelFlag(szRelFlag))
	{
		gRelFlag = NSATOI(szRelFlag);
	}

	// to check the valid range of the RELATION_FLAG with in 0 to 2 
	// otherwise set it to default value 2 to proceed
	if ((gRelFlag > 2) || (gRelFlag < 0))
		gRelFlag = 2;

	TCHAR szReportDirectory[_MAX_PATH + 1];
	bool bRet = oConverterUtils.checkConfigKeys(REPORT_LOCATION, szReportDirectory);

	NSSTRING strReportLoc;
	if(!bRet)
	{
		NSSTRCPY(szReportDirectory, (TCHAR*)strAppDirName.c_str());
		oConverterUtils.setReportLocation((TCHAR*)strDefReportLoc.c_str());
	}
	else
	{
		strReportLoc = szReportDirectory + strDefReportName;
		oConverterUtils.setReportLocation((TCHAR*)strReportLoc.c_str());
	}
	//Check for number of arguments
	if(argc < 2 || argc > 3)
	{
		writeHeaderInformation(_T(""), _T(""), _T(""), -1);
		WriteToLog(LOG_OTHER, _T("Invalid number of arguments."));
		CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)oConverterUtils.getReportLocation());
		return NS_FAIL;
	}
	//Source file path
	szSourcePath = argv[1];

	//Get destination file path
	if(argc == 2)
	{
		oConverterUtils.replaceFileExtension(szSourcePath, szDestinationPath);
	}
	else 
	{
		NSSTRING strDest(argv[2]);
		// Destination file is valid, Extract destination file name
		NSSTRING strDestFileName = strDest.substr((int)strDest.find_last_of(_T("\\")) + 1, strDest.length());
		if(-1 == strDestFileName.find_last_of(_T(".")))
		{
			TCHAR szExt[10];
			// Get source file extension
			oValidator.getFileExtension(szSourcePath, szExt);
			if(NS_SUCCESS == oValidator.isDirectoryExists((TCHAR*)strDest.c_str()))
			{

				NSSTRING strFileName(szSourcePath);
				size_t nResult = strFileName.find_last_of(_T("."));
				NSSTRING strExt = strFileName.substr(0, nResult);
				nResult = strExt.find_last_of(_T("\\"));
				strExt = strExt.substr(nResult + 1, strExt.length());
				if(NSSTRCMP(szExt, _T("xml")) == 0)
				{	
					strExt = strExt.append(_T(".dwg"));
				}
				else
				{
					strExt = strExt.append(_T(".xml"));
				}
				if(strDest.find_last_of(_T("\\")) == (strDest.length() - 1))
					strDest.append(strExt);
				else
				{
					strDest.append(_T("\\"));
					strDest.append(strExt);
				}

			}
			else
			{
				if((NSSTRCMP(strDestFileName.c_str(), _T("")) != 0) && ((int)strDestFileName.find(_T("\"")) == -1))
				{
					if(NSSTRCMP(szExt, _T("xml")) == 0)
					{	
						strDest = strDest.append(_T(".dwg"));
					}
					else
					{
						strDest = strDest.append(_T(".xml"));
					}
				}
				else
				{
					writeHeaderInformation(szSourcePath, _T(""), _T(""), -1);
					TCHAR szMsg[_MAX_PATH + 1];
					NSSPRINTF(szMsg, _T("Invalid Destination path : %S"), strDest.c_str());
					WriteToLog(LOG_OTHER, szMsg);
					CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)oConverterUtils.getReportLocation());
					return NS_FAIL;
				}
			}

		}

		NSSTRCPY(szDestinationPath, (TCHAR*)strDest.c_str());
	}

	//Validate arguments/config parameters
	int nConversion = oConverterUtils.validateArguements(szSourcePath, szDestinationPath);

	int nReturnVal = NS_FAIL;

	// Setting DSN
	HKEY hKey;
	bool bCreateDSN = false;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ODBC\\ODBC.INI\\GISCAD"), 0L, KEY_QUERY_VALUE, &hKey))
	{ 
		
		TCHAR strValue[_MAX_PATH + 1]; 
		DWORD dwLength = sizeof(strValue);
		LONG ret = RegQueryValueEx(hKey,L"DBQ", NULL, NULL, (LPBYTE)strValue,
			&dwLength);

		if(ret != 0 && (NSACCESS(oConverterUtils.getMDBLocation(), 0) != -1))	
			bCreateDSN = true;
		else
		{
			int nAccess = NSACCESS(oConverterUtils.getMDBLocation(), 0);
			if((NSSTRCMP(oConverterUtils.getMDBLocation(), strValue) != 0) && (nAccess != -1))
			{
				bCreateDSN = true;
			}
		}
	} 
	else
	{
		if(NSACCESS(oConverterUtils.getMDBLocation(), 0) != -1)
			bCreateDSN = true;
	}

	if(bCreateDSN == true)
	{
		if(!createDSN(oConverterUtils.getMDBLocation()))
		{
			WriteToLog(LOG_WARNINGS, _T("Failed to Add Data Source Name \"GISCAD\" "));
			nConversion = NS_FAIL ; //= return NS_FAIL;
		}
		else
		{
			WriteToLog(LOG_WARNINGS, _T("Data Source Name \"GISCAD\" added successfully"));
		}
	}
	TCHAR szMsg[_MAX_PATH + 1];
	NSSPRINTF(szMsg, _T("Invalid Destination path : %d"), nConversion);
	//MessageBox(NULL, szMsg, _T("test"), MB_OK);

	//Start Conversion
	TCHAR szFormat[_MAX_PATH + 1];
	if(CNSConverterUtils::UTILS_XML2DWG == nConversion)
	{
		bool bSaveas2007 = oConverterUtils.IsDWGFormat2007();
		int nPrecision = oConverterUtils.getPrecision();
		double dTextHeight = oConverterUtils.getTextHeight();
		int nRetVal = XMLToDWG(szSourcePath, szDestinationPath, oConverterUtils.getStandardDrawingLocation(), nPrecision, dTextHeight, bSaveas2007);


		if(nRetVal == ERR_XML_LOADING)
			strReportName = strDefReportName;
		else if(NSSTRCMP(strReportName.c_str(), _T("")) == 0)
			strReportName = _T("\\ErrorReport.log");
		else
			strReportName = _T("\\") + strReportName;
		strReportLoc = szReportDirectory + strReportName;
		oConverterUtils.setReportLocation((TCHAR*)strReportLoc.c_str());
		switch(nRetVal)
		{
		case NS_SUCCESS:	
			{
				oConverterUtils.setOutputFileName(szDestinationPath); 
				nReturnVal = NS_SUCCESS;
			}
			break;
		case NS_SHARING_VIOLATION: 
			{
				NSSPRINTF(szFormat, _T("File sharing violation: %s"), szDestinationPath);
				NSSTRCPY(szDestinationPath, _T(""));
				WriteToLog(LOG_OTHER, szFormat);
				nReturnVal = NS_FAIL;
			}
			break;
		case NS_FAIL: 
			{
				NSSPRINTF(szFormat, _T("Failed to create file: %s"), szDestinationPath);
				WriteToLog(LOG_OTHER, szFormat);		
				writeHeaderInformation(szSourcePath, _T(" "), oConverterUtils.getStandardDrawingLocation(), -1);
				CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)oConverterUtils.getReportLocation());
				return NS_FAIL;
			}
			break;
		}
	}
	else if(CNSConverterUtils::UTILS_DWG2XML == nConversion)
	{
		int nRet = DWGToXML(szSourcePath, szDestinationPath);

		if(nRet == NS_FAIL)
		{
			nReturnVal = NS_FAIL;
			bFlag = true;
		}
		else
		{
			if(NSSTRCMP(strReportName.c_str(), _T("")) == 0)
				strReportName = _T("\\ErrorReport.log");
			strReportName = _T("\\") + strReportName;
			strReportLoc = szReportDirectory + strReportName;
			oConverterUtils.setReportLocation((TCHAR*)strReportLoc.c_str());
			if(-1001 == nRet)
				nReturnVal = -1001;
			else if(-1002 == nRet)
				nReturnVal = -1002;
			else if(-1003 == nRet)
				nReturnVal = -1003;
			else
				nReturnVal = NS_SUCCESS;
		}
	}
	TCHAR szMsg1[_MAX_PATH + 1];
	NSSPRINTF(szMsg1, _T("Invalid Destination path : %d"), nConversion);
	if(nConversion == NS_FAIL)
	{
		TCHAR szExt[10];
		oValidator.getFileExtension(szDestinationPath, szExt);
		if(NSSTRCMP(szExt, _T("xml")) == 0)
			AddLogPathToSummaryReport(szDestinationPath, oConverterUtils.getReportLocation());
		writeHeaderInformation(szSourcePath, _T(""), oConverterUtils.getStandardDrawingLocation(), -1);
		CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)oConverterUtils.getReportLocation());
		return NS_FAIL;
	}
	else
	{
		if(CNSConverterUtils::UTILS_DWG2XML == nConversion)	
			AddLogPathToSummaryReport(szDestinationPath, oConverterUtils.getReportLocation());
		if(bFlag)
			NSSTRCPY(szDestinationPath, _T(""));
		writeHeaderInformation(szSourcePath, szDestinationPath, oConverterUtils.getStandardDrawingLocation(), nConversion);
		CNSReportWriter::getReportWriterInstance()->writeReport(REPORT_TXT, (TCHAR*)oConverterUtils.getReportLocation());
	}
	
	CoUninitialize();
	
	return nReturnVal;
}

