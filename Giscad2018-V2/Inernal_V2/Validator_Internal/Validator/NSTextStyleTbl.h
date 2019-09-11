/*!
 @Class			CNSTextStyleTbl
 @Brief			This class validates by comparing the different TextStyle properties 
				of std dwg file and local dwg file.
 @Author		Neilsoft Ltd
 @Date			06-08-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\NSDWGMgr.h"
class CNSTextStyleTbl
{
public:
	CNSTextStyleTbl(void);
public:
	~CNSTextStyleTbl(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(void);

	int CheckFontName(std::map<NSSTRING,CNSTextStyle*>::iterator TextStyleStdIter,std::map<NSSTRING,CNSTextStyle*>::iterator TextStyleIter );


private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
};