/*!
 @Class		CNSLogger
 @Brief		Stores and maps the error messages according to the error type
			Also helps to invoke appropraite writeReport() function
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#include "StdAfx.h"
#include "NSLogger.h"
#include "NSHTMLLogger.h"
#include "NSTextLogger.h"

/*!
 @Brief			Populates map with Log types entries as keys and CNSLogCollection as values
 @Date			12 June 2007
*/
CNSLogger::CNSLogger()
{
	m_pReportLogger = NULL;
	m_pCollection = NULL;

	CNSLogCollection *pCollection = NULL;
	for(int nMapCntr = 1; nMapCntr <= MAX_LOGTYPES; nMapCntr++)
	{
		pCollection = new CNSLogCollection;
		m_mapLog.insert(LOGTYPE_MAP::value_type(LOGTYPE(nMapCntr), pCollection));
	}	
}

/*!
 @Brief			Default Destructor
 @Date			12 June 2007
*/
CNSLogger::~CNSLogger(void)
{
	CNSLogCollection *pCollection = NULL;
	LOGTYPE_MAP::iterator theLoggerItr;
	for(theLoggerItr = m_mapLog.begin(); theLoggerItr != m_mapLog.end(); theLoggerItr++)
	{
		pCollection = (*theLoggerItr).second;
		delete pCollection;
		pCollection = NULL;
	}
/*	for(int nMapCntr = 1; nMapCntr <= MAX_LOGTYPES; nMapCntr++)
	{
		pCollection = m_mapLog[LOGTYPE(nMapCntr)];
		delete pCollection;
		pCollection = NULL;
	}*/
	
	m_mapLog.clear();

	if(m_pReportLogger)
	{
		delete m_pReportLogger;
		m_pReportLogger = NULL;
	}

	if(m_pCollection)
	{
		delete m_pCollection;
		m_pCollection = NULL;
	}
}

/*!
 @Brief			Adds the error messages to corresponding vectors
 @Param [in]	LOGTYPE	- log type [LOG_GIS, LOG_CAD, etc.] 
 @Param [in]	TCHAR*	- error message
 @Return		None
 @Date			12 June 2007
*/
int CNSLogger::writeLog(LOGTYPE enLogtype, TCHAR* szMessage)
{
	int nRetVal = -1;
	CNSLogCollection *pCollection = NULL;
	m_mapLogIterator = m_mapLog.find(enLogtype);
	if(m_mapLogIterator != m_mapLog.end())
	{
		pCollection = (*m_mapLogIterator).second;
		if(pCollection != NULL)
		{
			nRetVal = pCollection->AddLog(szMessage);
		}
	}    
	return nRetVal;
}

/*!
 @Brief			Adds the error messages to corresponding vectors
 @Param [in]	LOGTYPE	- log type [LOG_GIS, LOG_CAD, etc.] 
 @Param [in]	TCHAR*	- error message
 @Return		None
 @Date			28 Jan 2009
*/
int CNSLogger::writeLog(ErrRecord er)
{
	int nRetVal = 1;
	if(!m_pCollection)
		m_pCollection = new CNSLogCollection;

	 nRetVal = m_pCollection->AddLog(er);
	return nRetVal;
}

/*!
 @Brief			Creates an object according to the enReportType and calls its writeReport() function
 @Param [in]	REPORTTYPE	- Report type [REPORT_HTML/REPORT_TXT/REPORT_MTEXT]
 @Param [in]	TCHAR*		- path and file name of the report
 @Return		None
 @Date			12 June 2007
*/
bool CNSLogger::writeReport(REPORTTYPE enReportType, TCHAR* szReportPathName, TCHAR* szCSSPathName)
{
	bool bRetVal = false;
	switch(enReportType)
	{
		case REPORT_TXT: m_pReportLogger = new CNSTextLogger;
							break;
		case REPORT_HTML: m_pReportLogger = new CNSHTMLLogger;
							break;
		default: break;		
	}

	if(m_pReportLogger != NULL)
	{
		if(m_pCollection != NULL )
		{

			std::vector<ErrRecord*> vectLogErrRecs ; 
			std::vector<ErrRecord*> vectLogInfoRecs ;

			m_pCollection->getLogErrorVector(vectLogErrRecs);
			m_pCollection->getLogInfoVector(vectLogInfoRecs);

			 bRetVal = m_pReportLogger->writeReport(vectLogErrRecs, vectLogInfoRecs, szReportPathName ,szCSSPathName );			
		}
		else
		{
			bRetVal = m_pReportLogger->writeReport(m_mapLog, szReportPathName);
		} 
	}

	//bRetVal = m_pReportLogger->writeReport(m_mapLog, szReportPathName);

	return bRetVal;
}

/*!
 @Brief			Retrives the number of Logs corresponding to particular log type
 @Return		Number of logs 
 @Date			21 Aug 2007
*/
int CNSLogger::getNumberOfLogs(LOGTYPE enLogType , NSSTRING ErrorSeverity )
{
	int nRetVal = -1;
	//CNSLogCollection *pCollection = NULL;
	//m_mapLogIterator = m_mapLog.find(enLogType);
	//if(m_mapLogIterator != m_mapLog.end())
	//{
	//	pCollection = (*m_mapLogIterator).second;
	//	nRetVal = (int)pCollection->GetSize();
	//}

	if(m_pCollection)
	{
		nRetVal = m_pCollection->getNumberOfLogs(enLogType, ErrorSeverity);
	}
	return nRetVal;
}

/*!
 @Brief			Retrives the number of Logs corresponding to particular log type
 @Return		Number of logs 
 @Date			1 Feb 2009
*/
int CNSLogger::getNumberOfLogsEx(LOGTYPE enLogType)
{
	int nRetVal = -1;
	//CNSLogCollection *pCollection = NULL;
	//m_mapLogIterator = m_mapLog.find(enLogType);
	//if(m_mapLogIterator != m_mapLog.end())
	//{
	//	pCollection = (*m_mapLogIterator).second;
	//	nRetVal = (int)pCollection->GetSize();
	//}

	if(m_pCollection)
	{
		nRetVal = m_pCollection->getNumberOfLogsEx(enLogType);
	}
	return nRetVal;
}



NSSTRING CNSLogger::getLogMessage(LOGTYPE enLogType)
{
	NSSTRING szTitle(_T(""));
	CNSLogCollection *pCollection = NULL;
	m_mapLogIterator = m_mapLog.find(enLogType);
	if(m_mapLogIterator != m_mapLog.end())
	{
		pCollection = (*m_mapLogIterator).second;
		for(int nMsgCntr = 0; nMsgCntr < (int)pCollection->GetSize(); nMsgCntr++)
		{
			TCHAR* szMessage = pCollection->GetLogAt(nMsgCntr);
			szTitle.append(szMessage);
			szTitle.append(_T("\n"));
		}
	}
	return szTitle;
}

/*!
 @Brief			To Write Error Report in Local Drawing Model Layout
 @Note			\ has been replaced \\ via | to handle code crash
 @Return		String fro MText 
 @Date			20 Feb 2009
*/
NSSTRING CNSLogger::getLogMessageEx(LOGTYPE enLogType)
{
	NSSTRING szTitle(_T(""));
	std::vector<ErrRecord*> vectLogRec;

	if(enLogType == LOG_HEADER || enLogType == LOG_OTHER)
	{			
		m_pCollection->getLogInfoVector(vectLogRec);

		std::vector<ErrRecord*>::iterator theIt;
			ErrRecord* tempRec;
			int nRecordCntr = 0;
			for(theIt = vectLogRec.begin(); theIt != vectLogRec.end(); theIt++)
			{
					tempRec = (*theIt);
					if(tempRec)
					{
						if(tempRec->nLogType == enLogType)
						{							
							if((tempRec->strErrorDescription).length() > 0)
							{
								NSSTRING strTempDesc = tempRec->strErrorDescription;
								///////////
								int nLenTag = 0;
								int nLenTagData = 0;
								int nPos = -1;
								nLenTag = strTempDesc.length() ;
									while(nPos < nLenTag)
									{
										nPos = strTempDesc.find(_T("\\"),nPos+1);
										if(nPos == -1)
											break;
										else
										{
										if(nLenTagData >= 0)
											strTempDesc.replace(nPos, 1, _T("|"));
										}
									}
								///////////

								///////////
								nLenTag = strTempDesc.length() ;
									while(nPos < nLenTag)
									{
										nPos = strTempDesc.find(_T("|"),nPos+1);
										if(nPos == -1)
											break;
										else
										{
										if(nLenTagData >= 0)
											strTempDesc.replace(nPos, 1, _T("\\\\"));
										}
									}
								///////////

								szTitle.append(_T("{\\C4"));szTitle.append(strTempDesc);szTitle.append(_T("}\n"));
								
								nRecordCntr++;
							}
						}
					}
			}
	}
	
	else
	{
		m_pCollection->getLogErrorVector(vectLogRec);

		std::vector<ErrRecord*>::iterator theIt;
			ErrRecord* tempRec;
			int nRecordCntr = 0;
			bool bColFlag = false;
			for(theIt = vectLogRec.begin(); theIt != vectLogRec.end(); theIt++)
			{
					tempRec = (*theIt);
					if(tempRec)
					{
						if(tempRec->nLogType == enLogType)
						{
								TCHAR szErrorNo[5] = _T("");
								NSSPRINTF(szErrorNo, _T("%d"), tempRec->nErrorNo);
								NSSTRING strErrorNo(szErrorNo);
								
								NSSTRING strTemp = tempRec->strErrorSeverity.c_str();
								if(strTemp==_T("P1")){szTitle.append(_T("{\\C1"));bColFlag = true;}
								if(strTemp==_T("P2")){szTitle.append(_T("{\\C6"));bColFlag = true;}
								if(strTemp==_T("P3")){szTitle.append(_T("{\\C5"));bColFlag = true;}
								if(strTemp==_T("P4")){szTitle.append(_T("{\\C3"));bColFlag = true;}

								szTitle.append(tempRec->strErrorSeverity.c_str());szTitle.append(_T("\t"));
								szTitle.append(tempRec->strErrorCategory.c_str());szTitle.append(_T("\t\t\t\t ("));
								szTitle.append(strErrorNo);szTitle.append(_T(" ) "));

								//////////////////////////////////////////////////////////////////////////////////
								NSSTRING strTempDesc = tempRec->strErrorDescription;
								///////////
								int nLenTag = 0;
								int nLenTagData = 0;
								int nPos = -1;
								nLenTag = strTempDesc.length() ;
									while(nPos < nLenTag)
									{
										nPos = strTempDesc.find(_T("\\"),nPos+1);
										if(nPos == -1)
											break;
										else
										{
										if(nLenTagData >= 0)
											strTempDesc.replace(nPos, 1, _T("|"));
										}
									}
								///////////

								///////////
								nLenTag = strTempDesc.length() ;
									while(nPos < nLenTag)
									{
										nPos = strTempDesc.find(_T("|"),nPos+1);
										if(nPos == -1)
											break;
										else
										{
										if(nLenTagData >= 0)
											strTempDesc.replace(nPos, 1, _T("\\\\"));
										}
									}
								///////////

								szTitle.append(strTempDesc);								szTitle.append(_T("\n"));
								//////////////////////////////////////////////////////////////////////////////////
								//szTitle.append(tempRec->strErrorDescription.c_str());	szTitle.append(_T("\n"));
								

								if(bColFlag==true)
								{
									szTitle.append(_T("}"));
								}
								bColFlag = false;
						}
					}
			}

	}

	return szTitle;
}



