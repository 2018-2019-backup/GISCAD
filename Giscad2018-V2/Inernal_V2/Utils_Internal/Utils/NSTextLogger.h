/*!
 @Class		CNSTextLogger
 @Brief		Writes Text report
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#pragma once
#include "NSReportLogger.h"

class CNSTextLogger :public CNSReportLogger
{
public:
	CNSTextLogger(void);
	virtual ~CNSTextLogger(void);

	bool writeReport(LOGTYPE_MAP& LogTypeMap, TCHAR* szReportPathName);
	bool writeReport(std::vector<ErrRecord*> &m_vectLogErrRecs, std::vector<ErrRecord*> &m_vectInfoRecs, TCHAR* szReportPathName, TCHAR* szCSSPathName)
	{ return true;};
};
