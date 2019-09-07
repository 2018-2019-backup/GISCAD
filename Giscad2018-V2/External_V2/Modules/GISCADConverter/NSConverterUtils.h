/*!
 @Class			CNSConverterUtils
 @Brief			This class provides supporting functionality to GISCAD
				Converter.
 @Author		Neilsoft Ltd
 @Date			13-06-2007
 @History 		Change history
 */
#pragma once
#include "stdafx.h"
#include <sys/stat.h>
#include "..\..\Include\NSValidator.h"
#include "..\..\Include\NSConfigFileMgr.h"
#include "..\..\Include\NSReportWriter.h"
#include "..\..\Include\ReportTypeEnum.h"
#include "..\..\Include\NSDirectives.h"

// Configuration keys
#define LOCAL_STD_DWG _T("LOCAL_STD_DWG")
#define SRVR_STD_DWG _T("SRVR_STD_DWG")
#define MDB_LOCATION _T("MDB_LOCATION")
#define REPORT_LOCATION _T("REPORT_LOCATION")
#define SRVR_MDB_LOCATION _T("SRVR_MDB_LOCATION")
#define LOCAL_MDB_LOCATION _T("LOCAL_MDB_LOCATION")
#define DWG_SAVE_FORMAT _T("DWG_SAVE_FORMAT")
#define RELATION_FLAG _T("RELATION_FLAG")

#define DWG_FORMAT_2007 2007
#define DWG_DEFAULT_FORMAT 2004

extern int gRelFlag ; // global variable to store the value of RELATION_FLAG as in Settings.ini configuration file

class CNSConverterUtils
{
private:
	CNSValidator oValidator; // validator object for accssing validator methods
	TCHAR m_szInputFile[_MAX_PATH]; // to store input file for further processing
	TCHAR m_szOutputFile[_MAX_PATH]; // to store output file for further processing
	TCHAR m_szReportLocation[_MAX_PATH]; // to store report location.
	TCHAR m_szStandardDWGLocation[_MAX_PATH]; // to store standard drawing file location.
	TCHAR m_szConfigFilePath[_MAX_PATH]; // to store standard drawing file location.
	TCHAR m_szMDBLocation[_MAX_PATH];
	ConfigFile *m_pConfig;// to retrive config parameters inforamtion.

public:
	CNSConverterUtils(void);
	~CNSConverterUtils(void);
	//Member Functions
	bool validateConfigFile();
	int validateArguements(TCHAR* szSource, TCHAR* szDestination);
	void replaceFileExtension(TCHAR* szFileName, TCHAR *szDestFile);
	bool checkConfigKeys(TCHAR* szKey, TCHAR* szValue);
	bool getRelFlag( TCHAR* szValue);
	bool checkConfigurationFilePathValidity(TCHAR* szServerKey, TCHAR* szLocalKey);
	void setInputFileName(TCHAR* szInputFile);
	TCHAR* getInputFile();
	void setOutputFileName(TCHAR* szOutputFile);
	TCHAR* getOutputFile();
	void setReportLocation(TCHAR* szReportLocation);
	TCHAR* getReportLocation();
	void setStandardDrawingLocation(NSSTRING szStandardDWGLocation);
	TCHAR* getStandardDrawingLocation();
	NSSTRING getDirectoryName(TCHAR* szFileName);
	void setMDBLocation(NSSTRING szLocation);
	TCHAR* getMDBLocation();
	bool IsDWGFormat2007();
	void setConfigFilePath(TCHAR* szConfigFilePath);
    int getPrecision();
	double getTextHeight();
	enum{
		UTILS_XML2DWG	= 201,
		UTILS_DWG2XML	= 202
	};
};
