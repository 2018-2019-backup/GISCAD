/*!
 @Class		CNSLogger
 @Brief		Stores and maps the error messages according to the error type
			Also helps to invoke appropraite writeReport() function
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#include "StdAfx.h"
#include "tchar.h"
#include "..\Include\NSDirectives.h"
#include "NSTextLogger.h"

/*!
 @Brief			Default Constructor
 @Date			12 June 2007
*/
CNSTextLogger::CNSTextLogger(void)
{
}

/*!
 @Brief			Default Destructor
 @Date			12 June 2007
*/
CNSTextLogger::~CNSTextLogger(void)
{
}

/*!
 @Brief			Retrives the values from LOGTYPE_MAP and writes them to a TXT file.
 @Param [in]	LOGTYPE_MAP - Map of Error Logs
 @Param [in]	TCHAR*		- path and file name of the report
 @Return		None
 @Date			12 June 2007
*/
bool CNSTextLogger::writeReport(LOGTYPE_MAP& LogTypeMap, TCHAR* szReportPathName)
{
	bool bRetVal = true;
	bool bIndex = false;
	FILE* pFile = NULL;
	int nIndexCntr = 0;
	int nSize = 0;

	#ifdef _INTERNAL_
		//pFile = NSFOPEN(szReportPathName, _T("w"));
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#else
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#endif

	if(pFile != NULL)
	{
		//Write Report Header
		CNSLogCollection *pCollection = NULL;

		for(int nMapCntr = 1; nMapCntr <= MAX_LOGTYPES; nMapCntr++)
		{
			pCollection = LogTypeMap[LOGTYPE(nMapCntr)];
			switch(LOGTYPE(nMapCntr))
			{
			case LOG_WARNINGS:
					NSFPRINTF(pFile, _T("\nWarning(s) Found During Conversion : %d\n"), pCollection->GetSize());
					NSFPRINTF(pFile, _T("-----------------------------------------------------------------------------------------------\n"));
					break;
			case LOG_OTHER:
					NSFPRINTF(pFile, _T("\nError(s) Found During Conversion : %d\n"), pCollection->GetSize());
					NSFPRINTF(pFile, _T("-----------------------------------------------------------------------------------------------\n"));
					break;
			default:break;
			}

			if(LOGTYPE(nMapCntr) == LOG_WARNINGS || LOGTYPE(nMapCntr) == LOG_OTHER)
			{
				bIndex = true;
			}

			nIndexCntr = 0;
			for(int nMsgCntr = 0; nMsgCntr < (int)pCollection->GetSize(); nMsgCntr++)
			{
				TCHAR* szMessage = pCollection->GetLogAt(nMsgCntr);
				if(bIndex == true)
					nSize = NSFPRINTF(pFile, _T("%d.%s \n"), ++nIndexCntr, szMessage);
				else
					nSize = NSFPRINTF(pFile, _T("%s \n"), szMessage);
				if(nSize < 0)	bRetVal = false;
			}
		}

		if(fclose(pFile) != 0)
			bRetVal = false;
	}
	return bRetVal;
}