/*!
 @Class			NSTextHeightTbl
 @Brief			This class validates by comparing the TextHeight values with the std values.
 @Author		Neilsoft Ltd
 @Date			20-08-2007
 @History 		Change history
*/
#pragma once
#include "..\Include\NSDWGMgr.h"

class  __declspec(dllexport) CNSTextHeightTbl
{
public:
	CNSTextHeightTbl(void);
	~CNSTextHeightTbl(void){};
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(void);
private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
};