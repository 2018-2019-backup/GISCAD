/*!
 @Class			CNSExtraValidator
 @Brief			This class validates specific CAD validation rules
 @Author		Neilsoft Ltd
 @Date			28-08-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"

class CNSExtraValidator
{
public:
	CNSExtraValidator(void);
public:
	~CNSExtraValidator(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	int validate(bool m_bCadValidation, bool m_bGisValidation);
private:
	int compareAttribute(CNSAttributeRef* pStdAtt, CNSAttributeRef* pLocalAtt);
	void releaseAttributeMap(std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*>& attrMap);

private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
};
