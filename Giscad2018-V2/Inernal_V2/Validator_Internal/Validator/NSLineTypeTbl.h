/*!
 @Class			CNSLineTypeTbl
 @Brief			This class validates by comparing the different linetype properties 
				of std dwg file and local dwg file.
 @Author		Neilsoft Ltd
 @Date			06-08-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\NSDWGMgr.h"
class CNSLineTypeTbl
{
public:
	CNSLineTypeTbl(void);
public:
	~CNSLineTypeTbl(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(void);

private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
};
