/*!
 @Class		CNSLogger
 @Brief		Stores and maps the error messages according to the error type
			Also helps to invoke appropraite writeReport() function
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#pragma once

#include "NSReportLogger.h"
#include "..\Include\NSDirectives.h"
class CNSLogCollection;

class CNSLogger
{
public:
	CNSLogger();
	virtual ~CNSLogger(void);

	//methods
	//int  writeLog(LOGTYPE , TCHAR*);
	int  writeLog(LOGTYPE enLogtype, TCHAR* szMessage);
	int writeLog(ErrRecord er);
	bool writeReport(REPORTTYPE enReportType, TCHAR* szReportPathName,TCHAR* szCSSPathName = _T(""));	
	int getNumberOfLogs(LOGTYPE enLogType, NSSTRING ErrorSeverity = _T(""));
	int getNumberOfLogsEx(LOGTYPE enLogType);
	NSSTRING getLogMessage(LOGTYPE enLogType);
	NSSTRING getLogMessageEx(LOGTYPE enLogType);

private:
	//members
	LOGTYPE_MAP m_mapLog;	//This map stores Error type and Error message
	LOGTYPE_MAP::iterator m_mapLogIterator;
	CNSReportLogger *m_pReportLogger;
	CNSLogCollection *m_pCollection;
};
