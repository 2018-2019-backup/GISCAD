/*!
 @Class		CNSReportLogger
 @Brief		Base claas of CNSHTMLLogger and CNSTextLogger
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#pragma once
#include <map>
#include <vector>
#include "..\Include\ReportTypeEnum.h"
#include "NSLogCollection.h"

typedef std::map<LOGTYPE, CNSLogCollection*> LOGTYPE_MAP; 

class CNSReportLogger
{
public:
	//methods
	CNSReportLogger(void);
	virtual ~CNSReportLogger(void);

	virtual bool writeReport(LOGTYPE_MAP& LogTypeMap, TCHAR* szReportPathName) = 0;
	virtual bool writeReport(std::vector<ErrRecord*> &m_vectLogErrRecs, std::vector<ErrRecord*> &m_vectLogInfoRecs, TCHAR* szReportPathName,TCHAR* szCSSPathName)= 0;
};
