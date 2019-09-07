/*!
 @Class		CNSReportWriter
 @Brief		Root class for storing error messages and writing HTML or Text reports
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#pragma once

//#include "stdafx.h"
#include "ReportTypeEnum.h"
#include "..\Include\NSDirectives.h"
#include "string.h"

class CNSLogger;

class __declspec(dllexport) CNSReportWriter
{

private:
	//Members
	CNSLogger *m_pErrorLog;			// all errors are logged in object of CNSLogger.
	CNSReportWriter(void);          // Make the constructor private to make this class singleton
    static CNSReportWriter* reportWriterInstance; // Single instance to be used across the application 
public:
	~CNSReportWriter(void);

	//Methods
    static CNSReportWriter* getReportWriterInstance();
	int writeLog(LOGTYPE enLogType, TCHAR* szMessage);
	int writeLogEx(ErrRecord er);
	bool writeReport(REPORTTYPE enReportType, TCHAR* szReportPathName,TCHAR* szCSSPathName = _T(""));				// report type
	int getNumberOfLogs(LOGTYPE enLogType, NSSTRING ErrorSeverity = _T("")) const;
	int getNumberOfLogsEx(LOGTYPE enLogType) const;
	void release();
	NSSTRING getLogAt(LOGTYPE enLogType);
	NSSTRING getLogAtEx(LOGTYPE enLogType);	
	

	//int writeLogEx(LOGTYPE enLogType, NSSTRING strErrorString);
};
