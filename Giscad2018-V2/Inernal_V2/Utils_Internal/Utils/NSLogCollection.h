/*!
 @Class		CNSLogCollection
 @Brief		Maintains vector of error messages
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */
#pragma once

#include <vector>
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\reporttypeenum.h"

class CNSLogCollection
{
public:
	//methods
	CNSLogCollection(void);
	~CNSLogCollection(void);
	
	TCHAR* GetLogAt(int iLogAt);
	int AddLog(TCHAR* szLogMessage);
	int AddLog(ErrRecord &er);
	size_t GetSize();
	int CNSLogCollection::getNumberOfLogs(LOGTYPE enLogType, NSSTRING ErrorSeverity = _T(""));
	int getNumberOfLogsEx(LOGTYPE enLogType);
	void getLogErrorVector(std::vector<ErrRecord*> &vectLogErrRecs);
	void getLogInfoVector(std::vector<ErrRecord*> &vectLogErrRecs);

private:
	//members
	std::vector<TCHAR*> m_vectLogMessages;
	std::vector<ErrRecord*> m_vectLogErrRecs;
	std::vector<ErrRecord*> m_vectInfoMessages;
};
