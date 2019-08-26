#include "stdafx.h"
#include "NSErrorCircle.h"
#include "..\Include\NSBlock.h"
#include "..\Include\NSDirectives.h"
#include "NSBaseValidator.h"

CNSErrorCircle::CNSErrorCircle(void)
{
	m_pLocalDWGMgr = NULL;
	m_nGraphicIndication = 1;
}

CNSErrorCircle::~CNSErrorCircle(void)
{
}
void CNSErrorCircle::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}

void CNSErrorCircle::setGraphicIndication(int nGraphicIndication)
{
	m_nGraphicIndication = nGraphicIndication;
}
/*
@brief		Draws a circle with block name 'ERR_CIRCLE' and its attribute 'ERR_NUMBER'
			under layer 'GIS_ERROR' in the given position
@Param(in)	Error number, LOGTYPE and positions
*/
void CNSErrorCircle::drawCircle(int nErrorno,enum LOGTYPE type, int ErrNo, double dX, double dY, double dZ)
{
	
   	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);  // Get the Error type from database
	NSSTRING strErrType = er.strErrorSeverity;

	/*Checking related error checkBox is checked on Dialog and 
	also check notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),strErrType.c_str()) == 0 ||
		CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),strErrType.c_str()) == 0 ||
		CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),strErrType.c_str()) == 0 ||
		CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),strErrType.c_str()) == 0) && er.nErrReq == 0)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		if(type == 2)
		{
			NSSPRINTF(szFormat,_T("GIS_ERR_%i"), nErrorno);
		}
		else if(type == 3)
		{
			NSSPRINTF(szFormat,_T("CAD_ERR_%i"), nErrorno);
		}

		CNSBlock *pBlk = new CNSBlock;
		pBlk->setBlockName(_T("ERR_CIRCLE"));
		pBlk->setInsertionPoint(dX, dY, dZ);
		std::map<NSSTRING, CNSAttributeRefMap*> mapOfAttr;
		STRING_MAP mapOfXData;
		CNSAttributeRefMap *pRefMap = new CNSAttributeRefMap;
		CNSAttributeRef *pRef; 
		pRef = new CNSAttributeRef;
		pRef->setTagName(_T("ERR_NUMBER"));
		pRef->setAttributeValue(szFormat);
		pRefMap->addAttRef(1, pRef);
		mapOfAttr.insert(std::map<NSSTRING, CNSAttributeRefMap*>::value_type(_T("ERR_NUMBER"), pRefMap));
		//Add ERR_TYPE attribute.
		pRefMap = new CNSAttributeRefMap;
		pRef = new CNSAttributeRef;
		pRef->setTagName(_T("ERR_TYPE"));
		pRef->setAttributeValue(strErrType);
		pRefMap->addAttRef(1, pRef);
		mapOfAttr.insert(std::map<NSSTRING, CNSAttributeRefMap*>::value_type(_T("ERR_TYPE"), pRefMap));
		m_pLocalDWGMgr->insertBlockWithAttributes(pBlk, _T("GIS_ERRORS"),mapOfAttr, mapOfXData);
		delete pBlk;
		delete pRefMap;
	}
}
/*
@brief		Gets the error message and stores the message type and error no in a map
			and checks only once that error message is logged in the report else the 
			circle is drawn in the dwg file.
@Param(in)	Message type, error message, LOGTYPE and position
*/
void CNSErrorCircle::logError(enum Err_MSG msgType, TCHAR* szFormat, enum LOGTYPE type, double dXX, double dYY, int nErrID)
{
	int nErrno = 0;
	std::map<enum Err_MSG, int>::iterator mapofErrorItr;
	mapofErrorItr = mapofError.find(msgType);
	if(mapofErrorItr != mapofError.end())
	{
		nErrno = (*mapofErrorItr).second;
	}
	else
	{
        if(nErrID == -1)
        {
		    //nErrno = WriteToLog(type, szFormat);
			nErrno = WriteErrorRec(type,nErrID,szFormat);
        }
        else
        {
           //WriteIDToLog(type, szFormat, nErrID);

		   nErrno = WriteErrorRec(type,nErrID,szFormat); 
        }
		mapofError.insert(std::map<enum Err_MSG, int>::value_type(msgType, nErrno));
	}
	if(m_nGraphicIndication == 1) //indication turened on
		drawCircle(nErrno, type, nErrID, dXX, dYY, 0.0);
}

void CNSErrorCircle::clearErrMap()
{
	mapofError.clear();
}

/*
@brief		Gets the error message and stores the message type and error no in a map
			and checks only once that error message is logged in the report else the 
			circle is drawn in the dwg file.
@Param(in)	Message type, error message, LOGTYPE and position
@Date       2.5.09 ( during Enhancement II by Subir)
*/
void CNSErrorCircle::logErrorEx(enum Err_MSG msgType, TCHAR* szFormat, enum LOGTYPE type, double dXX, double dYY, CNSValidationDataCollector *pCollector, int nErrID)
{
	int nErrno = 0;
	std::map<enum Err_MSG, int>::iterator mapofErrorItr;
	mapofErrorItr = mapofError.find(msgType);
	if(mapofErrorItr != mapofError.end())
	{
		nErrno = (*mapofErrorItr).second;
	}
	else
	{
        if(nErrID == -1)
        {
		    //nErrno = WriteToLog(type, szFormat);
			nErrno = WriteErrorRec(type,nErrID,szFormat);
        }
        else
        {
           //WriteIDToLog(type, szFormat, nErrID);

		   //nErrno = WriteErrorRec(type,nErrID,szFormat); 
		   nErrno = WriteErrorRecEx(type,nErrID,pCollector);
        }
		mapofError.insert(std::map<enum Err_MSG, int>::value_type(msgType, nErrno));
	}
	if(m_nGraphicIndication == 1) //indication turened on
		drawCircle(nErrno, type, nErrID, dXX, dYY, 0.0);
}


