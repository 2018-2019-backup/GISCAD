//#include "StdAfx.h"
#include "tchar.h"
#include "..\Include\NSValidationDataCollector.h"
#include <windows.h>

CNSValidationDataCollector::CNSValidationDataCollector(void)
{
	m_nFID = nNumberNotFound;
	m_nFNO = nNumberNotFound;
	bPolyLine = FALSE;
	m_pCircle = NULL;
}

CNSValidationDataCollector::~CNSValidationDataCollector(void)
{
	CNSPoint3D *pTempPoint;
	std::vector<CNSPoint3D*>::iterator ptItr;
	for(ptItr = m_vectOfPolylineVertices.begin(); ptItr != m_vectOfPolylineVertices.end(); ptItr++)
	{
		pTempPoint = *ptItr;
		delete pTempPoint;
		pTempPoint = NULL;
	}
	m_vectOfPolylineVertices.clear();

	if(m_pCircle)
	{
		delete m_pCircle;
		m_pCircle = NULL;
	}
}

long CNSValidationDataCollector::getObjectId()
{
	return m_nAcDbObjectId;
}

int CNSValidationDataCollector::getFID()
{
	return m_nFID;
}

int CNSValidationDataCollector::getFNO()
{
	return m_nFNO;
}

FeatureType CNSValidationDataCollector::getFeatureType()
{
	return m_FeatureType;
}

FeatureStatus CNSValidationDataCollector::getFeatureStatus()
{
	return m_FeatureStatus;
}

NSSTRING CNSValidationDataCollector::getBlockName()
{
	return m_strBlockName;
}

NSSTRING CNSValidationDataCollector::getLayerName()
{
	return m_strLayerName;
}

NSSTRING CNSValidationDataCollector::getLineType()
{
	return m_strLineType;
}

CNSPoint3D CNSValidationDataCollector::getInsertionPoint()
{
	return m_insertionPoint;
}

NSSTRING CNSValidationDataCollector::getInsertionPointString()
{
	NSSTRING strInsPt = _T("");
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T(" X: \'%lf\' Y: \'%lf\'"), m_insertionPoint.m_dXX, m_insertionPoint.m_dYY);
	return strInsPt.append(szFormat);
}

CNSPoint3D CNSValidationDataCollector::getEndPoint()
{
	return m_EndPoint;
}

NSSTRING CNSValidationDataCollector::getEndPointString()
{
	NSSTRING strEndPt = _T("");
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat,_T("At Point X: \'%lf\' Y: \'%lf\'"), m_EndPoint.m_dXX, m_EndPoint.m_dYY);
	return strEndPt.append(szFormat);
}

void CNSValidationDataCollector::getPolylineVertices(std::vector<CNSPoint3D*> &vectOfPolylineVertices)
{
	std::vector<CNSPoint3D*>::iterator vectItr;
	CNSPoint3D *pPoint = NULL;
	for(vectItr = m_vectOfPolylineVertices.begin(); vectItr != m_vectOfPolylineVertices.end(); vectItr++)
	{
		vectOfPolylineVertices.push_back(*vectItr);
	}
}

CNSPoint3D CNSValidationDataCollector::getScaleFactor()
{
	return m_ScaleFactor;
}

void CNSValidationDataCollector::setObjectId(long nObjectId)
{
	m_nAcDbObjectId = nObjectId;
}

void CNSValidationDataCollector::setFID(int nFID)
{
	m_nFID = nFID;
}

void CNSValidationDataCollector::setFNO(int nFNO)
{
	m_nFNO = nFNO;
}

void CNSValidationDataCollector::setFeatureType(FeatureType enFeatureType)
{
	m_FeatureType = enFeatureType;
}

void CNSValidationDataCollector::setFeatureStatus(FeatureStatus enFeatureStatus)
{
	m_FeatureStatus = enFeatureStatus;
}

void CNSValidationDataCollector::setBlockName(NSSTRING strBlkName)
{
	m_strBlockName = strBlkName;
}

void CNSValidationDataCollector::setLayerName(NSSTRING strLayerName)
{
	m_strLayerName = strLayerName;
}

void CNSValidationDataCollector::setLineType(NSSTRING strLineType)
{
	m_strLineType = strLineType;
}

void CNSValidationDataCollector::setInsertionPoint(double dXX, double dYY)
{
	m_insertionPoint.m_dXX = dXX;
	m_insertionPoint.m_dYY = dYY;
}

void CNSValidationDataCollector::setEndPoint(double dXX, double dYY)
{
	m_EndPoint.m_dXX = dXX;
	m_EndPoint.m_dYY = dYY;
}

void CNSValidationDataCollector::addToVectOfPolylineVertices(double dXX, double dYY)
{
	CNSPoint3D* pPolylineVertex = new CNSPoint3D(dXX, dYY);
	m_vectOfPolylineVertices.push_back(pPolylineVertex);
}

void CNSValidationDataCollector::setScaleFactor(double dXX, double dYY)
{
	m_ScaleFactor.m_dXX = dXX;
	m_ScaleFactor.m_dYY = dYY;
}

NSSTRING CNSValidationDataCollector::readXDATA(NSSTRING strKey)
{
	std::map<NSSTRING, NSSTRING>::iterator itrXDATA;
	itrXDATA = m_mapOfRelationsAndConnectivity.find(strKey);
	if(itrXDATA != m_mapOfRelationsAndConnectivity.end())
	{
		return NSSTRING((*itrXDATA).second);
	}
	return _T("");
}

void CNSValidationDataCollector::addXDATA(std::map<NSSTRING, NSSTRING> &mapXDataRead)
{
	std::map<NSSTRING, NSSTRING>::iterator itrXDATA;
	for(itrXDATA = mapXDataRead.begin(); itrXDATA != mapXDataRead.end(); itrXDATA++)
	{
		m_mapOfRelationsAndConnectivity.insert(std::map<NSSTRING, NSSTRING>::value_type(itrXDATA->first, itrXDATA->second));
	}
}

NSSTRING CNSValidationDataCollector::readAttribute(NSSTRING strKey)
{
	std::map<NSSTRING, NSSTRING>::iterator itrXDATA;
	#ifndef _INTERNAL_
		NSSTRUPR((TCHAR*)strKey.c_str(), strKey.size()+1);
	#else
	    NSSTRUPR((TCHAR*)strKey.c_str(), strKey.size()+1);
		//NSSTRUPR((TCHAR*)strKey.c_str());
	#endif
	itrXDATA = m_mapAttributes.find(strKey);
	if(itrXDATA != m_mapAttributes.end())
	{
		return NSSTRING((*itrXDATA).second);
	}
	return _T("");
}

NSSTRING CNSValidationDataCollector::getNotesAppended(NSSTRING strMsg)
{
	NSSTRING strNotes;
	TCHAR szNotes[_MAX_PATH + 1];
	std::map<NSSTRING, NSSTRING>::iterator itrXDATA;
	itrXDATA = m_mapAttributes.find(_T("NOTES"));
	if(itrXDATA != m_mapAttributes.end())
	{
		strNotes.append(strMsg);
		NSSPRINTF(szNotes, _T(" (Notes:%s)"),((*itrXDATA).second).c_str());
		strNotes.append(szNotes);
		return strNotes;
	}
	return strMsg;
}

CNSCircle* CNSValidationDataCollector::getCircle()
{
	return m_pCircle;
}

void CNSValidationDataCollector::setCircle(CNSCircle *pCircle)
{
	m_pCircle = pCircle;
}


