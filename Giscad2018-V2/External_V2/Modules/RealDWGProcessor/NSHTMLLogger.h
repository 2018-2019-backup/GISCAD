/*!
 @Class		CNSTextLogger
 @Brief		Writes HTML report
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#pragma once
#include "NSReportLogger.h"
#include "..\Include\NSDirectives.h"
#include "string"
#include "tchar.h"
#include "windows.h"

#pragma warning( push )
#pragma warning( disable : 4275 )
class __declspec(dllexport)CNSHTMLLogger :public CNSReportLogger
{
public:
	CNSHTMLLogger(void);
	virtual ~CNSHTMLLogger(void);
	bool writeReport(LOGTYPE_MAP& LogTypeMap, TCHAR* szReportPathName);
	bool writeReport(std::vector<ErrRecord*> &m_vectLogErrRecs, std::vector<ErrRecord*> &m_vectInfoRecs, TCHAR* szReportPathName,  TCHAR* szCSSPathName = _T("") );
private:
    NSSTRING addReportTitle(TCHAR* szCSSPathName = _T(""));
    NSSTRING endHTMLReport();
    NSSTRING addReportHeader(CNSLogCollection *pCollection);
	NSSTRING addReportHeaderEx();
	NSSTRING addErrorData(CNSLogCollection *pCollection, LOGTYPE enType);
	NSSTRING addErrorData(std::vector<ErrRecord*> &m_vectLogErrRecs, LOGTYPE enType);
	NSSTRING addInfoData(std::vector<ErrRecord*> &m_vectLogErrRecs);
	NSSTRING addOtherInfoData(std::vector<ErrRecord*> &m_vectLogErrRecs);	
    NSSTRING addTable(BOOL bBorder=TRUE, BOOL bgColor=FALSE);
    NSSTRING endTable();
    NSSTRING addRowInTable(NSSTRING strPTagID = _T(""));
    NSSTRING endRowInTable();
    NSSTRING addColumnInRow(int size=10, NSSTRING color=_T("#000000"), NSSTRING bgcolor = _T("#FFFFFF"), NSSTRING colspan = _T("1"));
    NSSTRING endColumnInRow();
    NSSTRING addEmptyLine();
	void addHeaderRow(NSSTRING &szErrorData);


public:
	 void addTabs  (NSSTRING &szString , int n);
	 void addEnters(NSSTRING &szString, int n);
	 int  m_nTabCount;
};
#pragma warning( pop ) 