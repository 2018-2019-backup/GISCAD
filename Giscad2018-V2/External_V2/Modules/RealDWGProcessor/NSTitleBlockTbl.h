#pragma once
#include "..\Include\NSDWGMgr.h"
class CNSTitleBlock
{
public:
	CNSTitleBlock(void);
public:
	~CNSTitleBlock(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(void);
	NSSTRING getUserName()const;
	NSSTRING getValue(NSSTRING strKey, std::map<NSSTRING, NSSTRING> &mapTitleAttributes);
	NSSTRING getReportName()const;
private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
	NSSTRING m_strUserName;
	NSSTRING m_strReportName;
};
