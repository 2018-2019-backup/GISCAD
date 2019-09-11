#include "StdAfx.h"
#include "io.h"
#include <time.h>
#include <sys/stat.h>
#include "windows.h"
#include "TCHAR.H"
#include <ODBCINST.H>
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSValidator.h"

// Constuctor
CNSValidator::CNSValidator(void)
{
}

// Destructor
CNSValidator::~CNSValidator(void)
{
}

/*!
 @Brief			This function provides the functionality to check the time stamps
				of server and local files.
 @Param [in]	szServerFileName	:	Server file Name
 @Param [in]	szLocalFileName		:	Local file Name
 @Return		It returns one of the following values
				NS_FAIL		:	if two files having different timestamp.
				NS_SUCCESS	:	if two files having same timestamp.
 @Date			13-06-2007
*/

int CNSValidator::checkFileStamps(TCHAR *szServerFileName, TCHAR *szLocalFileName)
{
	if((NSACCESS(szServerFileName, 0) == -1) || (NSACCESS(szLocalFileName, 0) == -1))
		return VALIDATOR_INVALID_FILE_PATH;

	__time64_t serverFileLastMod = getFileLastModified(szServerFileName);
	__time64_t localFileLastMod = getFileLastModified(szLocalFileName);
	double diff = difftime(serverFileLastMod, localFileLastMod);
	if(diff == 0.0)
		return NS_SUCCESS;
	return NS_FAIL;

}

/*!
 @Brief			:	This function returns last modified date of the given file.
 @Param [in]	:	szFileName	:	file name to find last modified date
 @Return		:	retuns last modified date
 @Date			:	13-06-2007
*/
__time64_t CNSValidator::getFileLastModified(TCHAR *szFileName)
{
	struct NSSTATSTRUCT stFileInfo;
	NSSTATFUNC(szFileName, &stFileInfo);
	NSTIME_T serverFileLastMod = stFileInfo.st_mtime;
    struct tm stClock;
# ifndef _INTERNAL_
	NSGMTIME(&stClock,&serverFileLastMod);

#else
	NSGMTIME(&stClock,&serverFileLastMod);
    //NSGMTIME(&serverFileLastMod);
#endif
	return serverFileLastMod;
}
/*!
 @Brief			This function provides the functionality to overwirte the local
				file with server file if time difference found in server and local
				files.
 @Param [in]	szServerFileName	Server file name
				szLocalFileName		Local file name
 @Return		It returns one of the following values
				VALIDATOR_INVALID_FILE_PATH if given path is not valid
				VALIDATOR_SUCCESS			if both files having same time stamps
				VALIDATOR_FAIL				if both files having different time stamps
 @Date			13-06-2007
*/

int CNSValidator::overWriteFile(TCHAR *szServerFileName, TCHAR *szLocalFileName)
{
      int nResult = NS_FAIL;
      /*if(checkFileStamps(szServerFileName, szLocalFileName) != NS_SUCCESS)
      {*/
            ::SetFileAttributes(szLocalFileName,FILE_ATTRIBUTE_NORMAL);
            if(1 == CopyFile((NS_LP_STR)szServerFileName, (NS_LP_STR)szLocalFileName, false))
                  nResult = NS_SUCCESS;
      /*}*/
      return nResult;
}

/*!
 @Brief			This function provides the functionality to check
				given file is preset or not.
 @Param [in]	szFileName	file name to check present or not
 @Return		It returns one of the following values
				NS_SUCCESS			if file is present in the specified location
				VALIDATOR_INVALID_FILE_PATH	if file is not present in the specified location

 @Date			13-06-2007
*/

int CNSValidator::isFilePresent(TCHAR *szFileName)
{
	struct NSSTATSTRUCT stFileInfo;
	if(NSACCESS(szFileName, 0) != -1)
	{
		NSSTATFUNC(szFileName, &stFileInfo);
		if(stFileInfo.st_mode & _S_IFREG)
			return NS_SUCCESS;
	}
	return VALIDATOR_INVALID_FILE_PATH;
}
/*!
 @Brief			This function provides the functionality to check
				given directory is preset or not.
 @Param [in]	szFileName	directory name to check present or not
 @Return		It returns one of the following values
				NS_SUCCESS			if file is present in the specified location
				VALIDATOR_INVALID_FILE_PATH	if file is not present in the specified location

 @Date			13-06-2007
*/

int CNSValidator::isDirectoryExists(TCHAR *szFileName)
{
	TCHAR* szReport = szFileName;
	/*NSSTRING strReportLoc(szFileName);
	if(strReportLoc.find_first_of(_T("\\")) != strReportLoc.find_last_of(_T("\\")))
	{
		size_t value = strReportLoc.find_last_of(_T("\\"));
		if(value == strReportLoc.length() - 1)
		{
			strReportLoc = strReportLoc.substr(0, strReportLoc.length() - 1);
			szReport = (TCHAR*)strReportLoc.c_str();
		}
	}*/

	struct NSSTATSTRUCT stFileInfo;
	if(NSACCESS(szReport, 0) != -1)
	{
		int value = NSSTATFUNC(szReport, &stFileInfo);	
		if(value != -1)
		{
			if(stFileInfo.st_mode & _S_IFDIR )
				return NS_SUCCESS;
		}
	}
	return VALIDATOR_INVALID_FILE_PATH;
}


/*!
 @Brief			This function provides the functionality to check
				given file is read only or not
 @Param [in]	szFileName	file name to check read-only.
 @Return		If file is readonly it returns true else returns false.
 @Date			13-06-2007
*/

bool CNSValidator::isFileReadOnly(TCHAR *szFileName)
{
	if(NSACCESS(szFileName, 2) == -1)
		return true;
	else 
		return false;
}

/*!
 @Brief			This function provides the functionality to return
				the extension of the given file
 @Param [in]	szFileName	file name 
 @Param [out]	szExtension	extension of the szFileName
 @Return		it stores the extension in the out parameter szExtension
 @Date			13-06-2007
*/

 int CNSValidator::getFileExtension(TCHAR *szFileName, TCHAR szExtension[])
{
	//if(-1 == NSACCESS(szFileName, 0))
		//return NS_FAIL;
	NSSTRING strFileName(szFileName);
	size_t result = strFileName.find_last_of(_T("."));
	if(result != -1){
		strFileName = strFileName.substr(result + 1, strFileName.length());
	#ifdef _INTERNAL_
		//NSSTRCPY(szExtension, (TCHAR*)strFileName.c_str());
		NSSTRCPY(szExtension, strFileName.size() + 1, (TCHAR*)strFileName.c_str());
	#else
		NSSTRCPY(szExtension, strFileName.size() + 1, (TCHAR*)strFileName.c_str());
	#endif

	return NS_SUCCESS;
	} 
	return NS_FAIL;
}

 /*!
@Brief			This function creates the DSN, if DSN is not available.
@Param [in]		szMDBLoc		database location 
@Return			return true if DSN added successful else return false.
@Date			12-06-2007
*/
bool CNSValidator::createDSN(TCHAR* szMDBLoc)
{
	HKEY hKey;
	bool bCreateDSN = false;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\ODBC\\ODBC.INI\\GISCAD"), 0L, KEY_QUERY_VALUE, &hKey))
	{
		TCHAR strValue[_MAX_PATH + 1];
		DWORD dwLength = sizeof(strValue);
		LONG ret = RegQueryValueEx(hKey, L"DBQ", NULL, NULL, (LPBYTE)strValue,
			&dwLength);

		if (ret != 0 && (NSACCESS(szMDBLoc, 0) != -1))
		bCreateDSN = true;

		else
		{
			int nAccess = NSACCESS(szMDBLoc, 0);
			if((NSSTRCMP(szMDBLoc, strValue) != 0) && (nAccess != -1))
			{
				bCreateDSN = true;
			}
		}
	} 
	else
	{
		if(NSACCESS(szMDBLoc, 0) != -1)
			bCreateDSN = true;
	}

	if(bCreateDSN)
	{
		TCHAR szDesc[_MAX_PATH + 1];
		TCHAR szAttributes[_MAX_PATH + 1];
		//Use Hexadecimal 'FF' (=255) as temporary place holder
		wsprintf(szDesc, _T("DSN=GISCAD \xFF DESCRIPTION=TEST DES \xFF DBQ=%s\xFF \xFF "), szMDBLoc);
		int mlen = (int)NSSTRLEN(szDesc);
		int i = 0;
		int j = 0;
		//Loop to replace "FF" by "\0"(so as to store multiple strings into one):
		while(i<mlen-1)
		{
			#ifdef _INTERNAL_
				if ((szDesc[i] == '\xFF') && (szDesc[i+1] == ' '))
			#else
				if ((szDesc[i] == L'\xFF') && (szDesc[i+1] == L' '))
			#endif
//			if ((szDesc[i] == L'\xFF') && (szDesc[i+1] == L' '))
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
		bool bRtnCode = false;
		#ifdef _INTERNAL_
				//return SQLConfigDataSource(NULL, ODBC_ADD_SYS_DSN, "Microsoft Access Driver (*.mdb)\0", (LPCSTR)szAttributes);
		bRtnCode = SQLConfigDataSourceW(NULL, ODBC_ADD_SYS_DSN, L"Microsoft Access Driver (*.mdb, *.accdb)\0", (LPCWSTR)szAttributes);
		#else
		bRtnCode =  SQLConfigDataSourceW(NULL, ODBC_ADD_SYS_DSN, L"Microsoft Access Driver (*.mdb)\0", (LPCWSTR)szAttributes);
		#endif
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
				return bRtnCode;
	}
	else
	{
		return false;
	}
}