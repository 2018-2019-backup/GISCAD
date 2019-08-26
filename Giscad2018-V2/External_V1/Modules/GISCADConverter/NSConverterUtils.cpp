#include "StdAfx.h"
#include "NSConverterUtils.h"
#include "..\..\Include\NSReportWriter.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

// Constructor
CNSConverterUtils::CNSConverterUtils(void)
{
	m_pConfig = NULL;
}

// Destructor
CNSConverterUtils::~CNSConverterUtils(void)
{
	delete m_pConfig;
	m_pConfig = NULL;
}

/*!
@Brief			This function validates the path which read from configuration file for both server and client.
@Param [in]		szServerKey		:	Server standard drawing Key/access database key defined in Configuration file
@Param [in]		szLocalKey		:	Local standard drawing Key/access database key defined in Configuration file
@Return			If configuration file successfully validates then returns true
				in other cases returns false.
@Date			12-06-2007
*/
bool CNSConverterUtils::checkConfigurationFilePathValidity(TCHAR* szServerKey, TCHAR* szLocalKey)
{
	TCHAR szServerFile[_MAX_PATH + 1];
	TCHAR szLocalFile[_MAX_PATH + 1];

	// Check for whether local/server file available
	bool bLocalAvail = checkConfigKeys(szLocalKey, szLocalFile);
	bool bServerAvail = checkConfigKeys(szServerKey, szServerFile);

	bool bRetVal = true;

	// both server and local files available, if timestamps of both files are different then server file 
	// copied to local file location
	if(bServerAvail == true && bLocalAvail == true)
	{
		if(!oValidator.isFileReadOnly(szServerFile))
		{
			TCHAR szError[_MAX_PATH];
			NSSPRINTF(szError, _T("Server file is not Read-Only : %s"), szServerFile);
			WriteToLog(LOG_WARNINGS, szError);
		}
		int nValue = oValidator.checkFileStamps(szServerFile, szLocalFile);
		if(nValue != NS_SUCCESS)
		{
			int nTemp = oValidator.overWriteFile(szServerFile,szLocalFile);
			if(nTemp != NS_SUCCESS)
			{
				bRetVal = false;
			}
			else
			{
				TCHAR szError[_MAX_PATH];
				NSSPRINTF(szError, _T("Server file : \"%s\" is copied to \"%s\""), szServerFile, szLocalFile);
				WriteToLog(LOG_WARNINGS, szError);
			}
			
		}
		if(NSSTRCMP(szServerKey, SRVR_STD_DWG) == 0)
				setStandardDrawingLocation(szLocalFile);
		if(NSSTRCMP(szServerKey, SRVR_MDB_LOCATION) == 0)
				setMDBLocation(szLocalFile);
	}
	//bLocalAvail false means, if local file is not present in the
	// disk or local file location is invalid or local field is empty
	// in the setting.ini file
	else if(bServerAvail == true && bLocalAvail == false)
	{
		// Checking for server Read only property.
		if(!oValidator.isFileReadOnly(szServerFile))
		{
			TCHAR szError[_MAX_PATH];
			NSSPRINTF(szError, _T("Server file is not Read-Only : %s"), szServerFile);
			WriteToLog(LOG_WARNINGS, szError);
		}

		NSSTRING strFileName;
		int nTemp = oValidator.overWriteFile(szServerFile,szLocalFile);
		if(nTemp != NS_SUCCESS)
		{
			strFileName = getDirectoryName(m_szConfigFilePath);
			NSSTRING strServerFileName(szServerFile);
			size_t nIndex = strServerFileName.find_last_of(_T("\\"));
			strServerFileName = strServerFileName.substr(nIndex + 1, strServerFileName.length());

			strFileName = strFileName.append(_T("\\"));
			strFileName = strFileName.append(strServerFileName);
			nTemp = oValidator.overWriteFile(szServerFile,(TCHAR*)strFileName.c_str());
			if(nTemp != NS_SUCCESS)
			{
				TCHAR szError[_MAX_PATH];
				NSSPRINTF(szError, _T("Server file : \"%s\" is not overwritten into local machine."), szServerFile);
				WriteToLog(LOG_WARNINGS, szError);
				bRetVal = false;
			}
			else
			{
				TCHAR szError[_MAX_PATH];
				NSSPRINTF(szError, _T("Server file : \"%s\" is copied to \"%s\""), szServerFile, (TCHAR*)strFileName.c_str());
				WriteToLog(LOG_WARNINGS, szError);
			}
		}
		else
		{
			TCHAR szError[_MAX_PATH];
			NSSPRINTF(szError, _T("Server file : \"%s\" is copied to \"%s\""), szServerFile, szLocalFile);
			WriteToLog(LOG_WARNINGS, szError);
			strFileName = szLocalFile;
		}
		
		
		/**/
		
		if(NSSTRCMP(szServerKey, SRVR_STD_DWG) == 0)
				setStandardDrawingLocation((TCHAR*)strFileName.c_str());
		if(NSSTRCMP(szServerKey, SRVR_MDB_LOCATION) == 0)
				setMDBLocation((TCHAR*)strFileName.c_str());
	}
	// If only local file available, process continues with local file.
	else if(bServerAvail == false && bLocalAvail == true)
	{
		TCHAR szError[_MAX_PATH];
		NSSPRINTF(szError, _T("Local file Used : %s"), szLocalKey);
		WriteToLog(LOG_WARNINGS, szError);
		if(NSSTRCMP(szServerKey, SRVR_STD_DWG) == 0)
				setStandardDrawingLocation(szLocalFile);
		if(NSSTRCMP(szServerKey, SRVR_MDB_LOCATION) == 0)
			setMDBLocation(szLocalFile);
	}
	else
	{
		bRetVal = false;
		setStandardDrawingLocation(_T(""));
		setMDBLocation(_T(""));
	}
	
	return bRetVal;
}


/*!
@Brief			This function validates configuration file.
@Return			If configuration file successfully validates then returns true
				in other cases returns false.
@Date			12-06-2007
*/
bool CNSConverterUtils::validateConfigFile()
{
	// arrays stores server and local keys to standard drawing and access database
	TCHAR *szPath[2][2] = {{ SRVR_STD_DWG, LOCAL_STD_DWG}, 
						   {SRVR_MDB_LOCATION, LOCAL_MDB_LOCATION}};

	for(int nKeyCntr = 0; nKeyCntr < 2; nKeyCntr++)
	{
		if(false == checkConfigurationFilePathValidity(szPath[nKeyCntr][0], szPath[nKeyCntr][1]))
		{
			return false;
		}
	}
	
	return true;
}

/*!
@Brief			This function reads the configuration key values from configuration
				file.
@Param [in]		szKey	:	Key Name
@Param [out]	szValue	:	value read from configuration file.
@Return			If value is not exists then retunrs false
				in other cases returns true.
@Date			13-06-2007
*/
bool CNSConverterUtils::checkConfigKeys(TCHAR* szKey, TCHAR *szValue)
{
	bool bRetVal = true;
	if(m_pConfig != NULL)
		m_pConfig->readString( szValue, szKey);
	else
		return false;
	
	int nExists = 0;
	if(NSSTRCMP(szKey, REPORT_LOCATION) == 0)
	{
		nExists = oValidator.isDirectoryExists(szValue);
	}
	else
    {
		nExists = oValidator.isFilePresent(szValue);

    }
	if(NS_SUCCESS != nExists)
	{
		TCHAR szError[_MAX_PATH];
		NSSPRINTF(szError, _T("Invalid Configuration file parameter : %s"), szKey);
		WriteToLog(LOG_WARNINGS, szError);
		bRetVal = false;
	}
	return bRetVal;
}
/*!
@Brief			This function validates the command line arguments .
@Param [in]		szSource 		 :	source file location
@Param [in]		szDestination	 :	destination file location
@Return			If arguments are valid then return NS_SUCCESS
				If arguments are not valid then return NS_FAIL
@Date			12-06-2007
*/
int CNSConverterUtils::validateArguements(TCHAR* szSource, TCHAR* szDestination)
{
	bool bResult = validateConfigFile();
	if(!bResult)
		return NS_FAIL;
	int nSourceResult = oValidator.isFilePresent(szSource);
	NSSTRING strFileName(szDestination);
	size_t nIndex = strFileName.find_last_of(_T("\\"));
	NSSTRING strDstFileName = strFileName.substr(nIndex + 1, strFileName.length());
	size_t nDstIndex = strDstFileName.find_last_of(_T("."));
	strDstFileName = strDstFileName.substr(nDstIndex + 1, strDstFileName.length());
	if((NSSTRCMP(strDstFileName.c_str(), _T("dwg")) != 0) && (NSSTRCMP(strDstFileName.c_str(), _T("xml")) != 0))
	{
		WriteToLog(LOG_OTHER, _T("Invalid extension for output file"));
		return NS_FAIL;
	}
	strFileName = strFileName.substr(0, nIndex);
	if(-1 == strFileName.find(_T("\\")))
		strFileName.append(_T("\\"));
	TCHAR* direcotryName = (TCHAR*)strFileName.c_str();
	int nDestResult = oValidator.isDirectoryExists(direcotryName);
	if(nSourceResult == NS_SUCCESS)
	{
		if(nDestResult == NS_SUCCESS)
		{
			TCHAR szExtension[_MAX_PATH + 1];
			if(NS_FAIL != oValidator.getFileExtension(szSource, szExtension))
			{
				if(NSSTRCMP(szExtension, _T("xml")) == 0)
					return UTILS_XML2DWG;
				else if(NSSTRCMP(szExtension, _T("dwg")) == 0)
					return UTILS_DWG2XML;
				else
				{
					WriteToLog(LOG_OTHER, _T("Unknown file format \n"));
					return NS_FAIL;
				}
			}
			else
			{
				WriteToLog(LOG_OTHER, _T("File Extension not found for Source file \n"));
				return NS_FAIL;
			}
		}
		else
		{
			WriteToLog(LOG_OTHER, _T("Invalid destination path"));
			return NS_FAIL;
		}
	}
	else 
	{
		WriteToLog(LOG_OTHER, _T("Invalid source file path"));
		return NS_FAIL;
	}
}

/*!
@Brief			ReplaceFileExtension is called to replace the file extension
				with dwg.
@Param [in]		szFileName		:	source file
@Param [out]	szDestFile		:	Destination file
@Return			void
@Date			12-06-2007
*/
void CNSConverterUtils::replaceFileExtension(TCHAR* szFileName, TCHAR *szDestFile)
{
	NSSTRING strFileName(szFileName);
	size_t nResult = strFileName.find_last_of(_T("."));
	NSSTRING strExt = strFileName.substr(nResult + 1, strFileName.length());
	if(NSSTRCMP(strExt.c_str(), _T("xml")) == 0)
		strFileName.replace(nResult + 1, strFileName.length() - nResult, _T("dwg"));
	else
		strFileName.replace(nResult + 1, strFileName.length() - nResult, _T("xml"));
	NSSTRCPY(szDestFile, strFileName.size() + 1, strFileName.c_str());
}
/*!
@Brief			Create ConfigFile Object from the path
@Param [in]		szConfigFilePath	:	Configuration file path
@Return			void
@Date			20-06-2007
*/
void CNSConverterUtils::setConfigFilePath(TCHAR* szConfigFilePath)
{
	NSSTRING strFileName(szConfigFilePath);
	NSSTRCPY(m_szConfigFilePath, strFileName.size() + 1, szConfigFilePath);
	m_pConfig = new ConfigFile(szConfigFilePath);
}

/*!
@Brief			Read which DWG file format to be created
@Param [in]		None
@Return			if format is 2007 return true else return false
@Date			20-06-2007
*/
bool CNSConverterUtils::IsDWGFormat2007()
{
	TCHAR szValue[_MAX_PATH + 1];
	m_pConfig->readString(szValue, DWG_SAVE_FORMAT);
	return ( NSATOI(szValue) == DWG_FORMAT_2007 ? true:false);
}

int CNSConverterUtils::getPrecision()
{
    TCHAR szValue[_MAX_PATH + 1];
    NSSTRCPY(szValue, _MAX_PATH + 1, _T(""));
	m_pConfig->readString(szValue, _T("PRECISION"));
    int nPrecision = -1;
    if(NSSTRCMP(szValue, _T("")) != 0)
       nPrecision  = NSATOI(szValue);
    return nPrecision;

}

double CNSConverterUtils::getTextHeight()
{
	TCHAR szValue[_MAX_PATH + 1];
    NSSTRCPY(szValue, _MAX_PATH + 1, _T(""));
	m_pConfig->readString(szValue, _T("STD_TEXT_HEIGHT"));
    double nTextHeight = 2.5;
    if(NSSTRCMP(szValue, _T("")) != 0)
		nTextHeight = _wtof(szValue);
    return nTextHeight;
}

void CNSConverterUtils::setInputFileName(TCHAR* szLocation)
{
	NSSTRCPY(m_szInputFile, szLocation);
}
TCHAR* CNSConverterUtils::getInputFile()
{
	return m_szInputFile;
}
	
void CNSConverterUtils::setOutputFileName(TCHAR* szLocation)
{
	NSSTRCPY(m_szOutputFile, szLocation);
}
TCHAR* CNSConverterUtils::getOutputFile()
{
	return m_szOutputFile;
}
void CNSConverterUtils::setReportLocation(TCHAR* szLocation)
{
	NSSTRCPY(m_szReportLocation, szLocation);
}
TCHAR* CNSConverterUtils::getReportLocation()
{
	return m_szReportLocation;
}
void CNSConverterUtils::setStandardDrawingLocation(NSSTRING szLocation)
{
	NSSTRCPY(m_szStandardDWGLocation, szLocation.c_str());
}
TCHAR* CNSConverterUtils::getStandardDrawingLocation()
{
	return m_szStandardDWGLocation;
}
void CNSConverterUtils::setMDBLocation(NSSTRING szLocation)
{
	NSSTRCPY(m_szMDBLocation, szLocation.c_str());
}

TCHAR* CNSConverterUtils::getMDBLocation()
{
	return m_szMDBLocation;
}
NSSTRING CNSConverterUtils::getDirectoryName(TCHAR* szFileName)
{
	NSSTRING strFileName(szFileName);
	size_t nIndex = strFileName.find_last_of(_T("\\"));
	strFileName = strFileName.substr(0, nIndex);
	return strFileName;
}


/*!
@Brief			This function reads the RELATION_FLAG value from configuration	file.
@Param [out]	szValue	:	value read from configuration file.
@Return			If value is not exists then retunrs false
				in other cases returns true.
@Date			08-12-2008
*/
bool CNSConverterUtils::getRelFlag( TCHAR *szValue)
{
	bool bRetVal = true;
	if(m_pConfig != NULL)
	{
		m_pConfig->readString( szValue, RELATION_FLAG);
	}
	else
	{
		bRetVal = false;
	}
	return bRetVal;
}