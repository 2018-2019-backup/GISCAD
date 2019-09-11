/*!
 @Class		CNSLogCollection
 @Brief		Maintains vector of error messages
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#include <iostream>
#include "StdAfx.h"
#include "nslogcollection.h"
#include "..\..\Include\NSDirectives.h"
/*
#ifdef _UNICODE
#define STRINGCOPY wcscpy
#else
#define STRINGCOPY strcpy
#endif*/

/*!
 @Brief			Default Constructor
 @Date			12 June 2007
*/
CNSLogCollection::CNSLogCollection()
{
}

/*!
 @Brief			Clears memory allocated for vectors storing error messages
 @Date			12 June 2007
*/
CNSLogCollection::~CNSLogCollection()
{	
	std::vector<TCHAR*>::iterator CollectionItr;
	TCHAR* pMessage = NULL;
	for(CollectionItr =  m_vectLogMessages.begin(); CollectionItr != m_vectLogMessages.end(); CollectionItr++)
	{
		pMessage = *CollectionItr;
		delete []pMessage;
	}
	m_vectLogMessages.clear();

	std::vector<ErrRecord*>::iterator CollectionItr1;
	ErrRecord* pEr1 = NULL;
	for(CollectionItr1 =  m_vectLogErrRecs.begin(); CollectionItr1 != m_vectLogErrRecs.end(); CollectionItr1++)
	{
		pEr1 = *CollectionItr1;
		delete pEr1;
	}
	m_vectLogErrRecs.clear();

	std::vector<ErrRecord*>::iterator CollectionItr2;
	ErrRecord* pEr2 = NULL;
	for(CollectionItr2 =  m_vectInfoMessages.begin(); CollectionItr2 != m_vectInfoMessages.end(); CollectionItr2++)
	{
		pEr2 = *CollectionItr2;
		delete pEr2;
	}
	m_vectInfoMessages.clear();


}

/*!
 @Brief			Adds error message to a vector
 @Param [in]	TCHAR*		- error message
 @Return		None
 @Date			12 June 2007
*/
int CNSLogCollection::AddLog(TCHAR* szLogMessage)
{
	TCHAR *pMessage = new TCHAR[NSSTRLEN(szLogMessage) + 1];
	NSSTRING strMsg(szLogMessage);
	#ifdef _INTERNAL_
		//NSSTRCPY(pMessage, szLogMessage);
		NSSTRCPY(pMessage, strMsg.size() + 1, szLogMessage);
	#else
		NSSTRCPY(pMessage, strMsg.size() + 1, szLogMessage);
	#endif
		
	m_vectLogMessages.push_back(pMessage);
	return (int)m_vectLogMessages.size();
}
/*!
 @Brief			Adds error message to a vector
 @Param [in]	TCHAR*		- error message
 @Return		None
 @Date			27 Jan 2009
*/
int CNSLogCollection::AddLog(ErrRecord &er)
{
	ErrRecord *new_er = new ErrRecord;
	new_er->nErrorNo = er.nErrorNo ;
	new_er->strErrorSeverity = er.strErrorSeverity ;
	new_er->strErrorCategory = er.strErrorCategory ;
	new_er->strErrorDescription = er.strErrorDescription ;
    new_er->nLogType = er.nLogType;
	if(	er.nLogType == LOG_GIS || er.nLogType == LOG_CAD)
	{
		m_vectLogErrRecs.push_back(new_er);
		new_er->nErrorID = (int)m_vectLogErrRecs.size();
		return (int)m_vectLogErrRecs.size();
	}
	else // LOG_HEADER = 1 and LOG_OTHER = 4
	{
		m_vectInfoMessages.push_back(new_er);
	}
	return 0;
}

/*!
 @Brief			Retrieves error message at given position
 @Param [in]	int		- vector element postion
 @Return		TCHAR*	- error message
 @Date			12 June 2007
*/
TCHAR* CNSLogCollection::GetLogAt(int iLogAt)
{
	size_t size = GetSize();
	if(iLogAt <  (int)size && iLogAt >= 0)
		return m_vectLogMessages.at(iLogAt);
	else
		return NULL;
}

/*!
 @Brief			Gives size of the vector
 @Return		int	- vector size
 @Date			12 June 2007
*/
size_t CNSLogCollection::GetSize()
{
	return m_vectLogMessages.size();
}

/*!
 @Brief			Gives size of the number of errors based on log type & ErrorSeverity
 @Return		int	- vector size
 @Date			11 Sept 2009
*/
int CNSLogCollection::getNumberOfLogs(LOGTYPE enLogType, NSSTRING ErrorSeverity)
{
	std::vector<ErrRecord*>::iterator theIt;
	ErrRecord* tempRec;
	int nRecordCntr = 0;
	for(theIt = m_vectLogErrRecs.begin(); theIt != m_vectLogErrRecs.end(); theIt++)
	{
		tempRec = (*theIt);
		if(tempRec)
		{
			if(ErrorSeverity == _T(""))
			{
				if(tempRec->nLogType == enLogType)
				{
					nRecordCntr++;
				}
			}
			else
			{
				if(tempRec->nLogType == enLogType)
				{
					if(tempRec->strErrorSeverity == ErrorSeverity)
					{
						nRecordCntr++;
					}
				}
			}
		}
	}
	return nRecordCntr;
}

/*!
 @Brief			Gives size of the number of errors based on log type
 @Return		int	- vector size
 @Date			16 Feb 2009
*/
int CNSLogCollection::getNumberOfLogsEx(LOGTYPE enLogType)
{
	std::vector<ErrRecord*>::iterator theIt;
	ErrRecord* tempRec;
	int nRecordCntr = 0;
	for(theIt = m_vectInfoMessages.begin(); theIt != m_vectInfoMessages.end(); theIt++)
	{
		tempRec = (*theIt);
		if(tempRec)
		{
			if(tempRec->nLogType == enLogType)
			{
				nRecordCntr++;
			}
		}
	}
	return nRecordCntr;
}

/*!
 @Brief			Gives size of the number of errors based on log type
 @Return		int	- vector size
 @Date			16 Feb 2009
*/
void CNSLogCollection::getLogErrorVector(std::vector<ErrRecord*> &vectLogErrRecs)
{
	vectLogErrRecs = m_vectLogErrRecs;
}

/*!
 @Brief			Gives size of the number of errors based on log type
 @Return		int	- vector size
 @Date			16 Feb 2009
*/
void CNSLogCollection::getLogInfoVector(std::vector<ErrRecord*> &vectLogErrRecs)
{
	vectLogErrRecs = m_vectInfoMessages;
}