/*!
 @Class		CNSReportWriter
 @Brief		Root class for storing error messages and writing HTML or Text reports
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#include "StdAfx.h"
#include <io.h>
#include "..\Include\NSDirectives.h"
#include "..\Include\NSReportWriter.h"
#include "NSLogger.h"

// initialize the singleton instance
CNSReportWriter* CNSReportWriter::reportWriterInstance = NULL;

/*!
 @Brief			Default Constructor
 @Date			12 June 2007
*/
CNSReportWriter::CNSReportWriter(void)
{
	m_pErrorLog = new CNSLogger();	//Create an object of type CNSLogger
}

/*!
@Brief			Destructor
 @Date			12 June 2007
*/
CNSReportWriter::~CNSReportWriter(void)
{
	 delete m_pErrorLog;
	 m_pErrorLog = NULL;
	 reportWriterInstance = NULL;
}

/*!
 @Brief			Call CNSLogger's writeLog() function
 @Param [in]	LOGTYPE	- log type [LOG_GIS, LOG_CAD, etc.] 
 @Param [in]	TCHAR*	- error message
 @Return		None
 @Date			12 June 2007
*/
int CNSReportWriter::writeLog(LOGTYPE enLogType, TCHAR* szMessage)
{
	int nRetVal = -1;

	if(m_pErrorLog == NULL)
	{
		m_pErrorLog = new CNSLogger();	//Create an object of type CNSLogger
	}
	nRetVal = m_pErrorLog->writeLog(enLogType, szMessage);
	
	return nRetVal;
}

/*!
 @Brief			Call CNSLogger's writeLog() function
 @Param [in]	LOGTYPE	- log type [LOG_GIS, LOG_CAD, etc.] 
 @Param [in]	TCHAR*	- error message
 @Return		None
 @Date			27 Jan 2009
*/
int CNSReportWriter::writeLogEx(ErrRecord er)
{
	int nRetVal = -1;

	if(m_pErrorLog == NULL)
	{
		m_pErrorLog = new CNSLogger();	//Create an object of type CNSLogger
	}
	nRetVal = m_pErrorLog->writeLog(er);
	
	return nRetVal;
}

///********************14-Feb-2014****************/
//int CNSReportWriter::writeLogEx(LOGTYPE enLogType, NSSTRING strErrorString)
//{
//	int nRetVal = -1;
//
//	ErrRecord er ;
//	er.nLogType = enLogType;
//	er.strErrorDescription = strErrorString;	
//
//	if(m_pErrorLog == NULL)
//	{
//		m_pErrorLog = new CNSLogger();	//Create an object of type CNSLogger
//	}
//	nRetVal = m_pErrorLog->writeLog(er);
//	
//	return nRetVal;
//}


/*!
 @Brief			Call CNSLogger's writeReport() function
 @Param [in]	REPORTTYPE  - Report type [REPORT_HTML/REPORT_TXT/REPORT_MTEXT]
 @Param [in]	TCHAR*		- path and file name of the report
 @Return		None
 @Date			12 June 2007
*/
bool CNSReportWriter::writeReport(REPORTTYPE enReportType, TCHAR* szReportPathName, TCHAR* szCSSPathName)
{
	bool bRelVal = false;
	if(m_pErrorLog != NULL)
	{
		bRelVal = m_pErrorLog->writeReport(enReportType, szReportPathName, szCSSPathName);
	}	
	return bRelVal;
}

/*!
 @Brief			Gets the single available instance of ReportWriter class
 @Return		Sigle available instance of ReportWriter 
 @Date			20 July 2007
*/
CNSReportWriter* CNSReportWriter::getReportWriterInstance()
{
    if(reportWriterInstance == NULL)
    {
        reportWriterInstance = new CNSReportWriter();
    }
    return reportWriterInstance;
}

/*!
 @Brief			Retrives the number of Logs corresponding to particular log type
 @Return		Number of logs 
 @Date			21 Aug 2007
*/
int CNSReportWriter::getNumberOfLogs(LOGTYPE enLogType , NSSTRING ErrorSeverity ) const
{
	int nRelVal = -1;
	if(m_pErrorLog != NULL)
	{
		nRelVal = m_pErrorLog->getNumberOfLogs(enLogType,ErrorSeverity );
	}	
	return nRelVal;
}

/*!
 @Brief			Retrives the number of Logs corresponding to particular log type
 @Return		Number of logs 
 @Date			16 Feb 2009
*/
int CNSReportWriter::getNumberOfLogsEx(LOGTYPE enLogType) const
{
	int nRelVal = -1;
	if(m_pErrorLog != NULL)
	{
		nRelVal = m_pErrorLog->getNumberOfLogsEx(enLogType);
	}	
	return nRelVal;
}

void CNSReportWriter::release()
{
	 delete m_pErrorLog;
	 m_pErrorLog = NULL;

}

NSSTRING CNSReportWriter::getLogAt(LOGTYPE enLogType)//, NSSTRING &strFormat)
{
	NSSTRING strFormat;
	if(m_pErrorLog != NULL)
	{
		strFormat = m_pErrorLog->getLogMessage(enLogType);
	}
	return strFormat;
}

NSSTRING CNSReportWriter::getLogAtEx(LOGTYPE enLogType)//, NSSTRING &strFormat)
{
	NSSTRING strFormat;
	if(m_pErrorLog != NULL)
	{
		strFormat = m_pErrorLog->getLogMessageEx(enLogType);
	}
	return strFormat;
}

