/*!
 @Class			CNSLayerTbl
 @Brief			This class validates by comparing the different layer properties 
				of std dwg file and local dwg file.
 @Author		Neilsoft Ltd
 @Date			06-08-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\NSDWGMgr.h"
class CNSLayerTbl
{
public:
	CNSLayerTbl(void);
public:
	~CNSLayerTbl(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(void);
private:
	NSSTRING getLineWtString(int nNumber);
private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
};
