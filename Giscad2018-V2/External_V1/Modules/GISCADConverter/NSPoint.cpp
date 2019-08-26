//#include "StdAfx.h"

//#include "..\..\Include\NSDirectives.h"
//#include "..\..\Include\NSPoint3D.h"
#include "StdAfx.h"
#include "NSPoint.h"

// Constructor
CNSPoint::CNSPoint(void)
{

}

// Destructor
CNSPoint::~CNSPoint(void)
{
}

/*!
@Brief			This function reads block information i.e block insertion point, 
				rotation and orientation from XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSPoint::readXML(CNSTElement &element)
{
	setType(GIS_POINT);
	CNSTElement oPointElem = element.subnode(_T("gml:Point"));
	CNSTElement oPosElem = oPointElem.subnode(_T("gml:pos"));
	NSSTRING val = oPosElem.val();
	CNSBlock *pBlock = new CNSBlock;
	TCHAR* szNextToken;
	double dX,dY,dZ;
	dX = _wtof(wcstok_s ((TCHAR*)val.c_str(), _T(" "), &szNextToken));
	dY = _wtof(wcstok_s (szNextToken, _T(" "), &szNextToken));
	TCHAR* tempToken = wcstok_s (NULL,_T("\0") , &szNextToken);
	if(tempToken == NULL)
      dZ = 0.00;
	else
	  dZ = _wtof(tempToken);//wcstok_s (NULL, _T("\0"), &szNextToken));
	pBlock ->setInsertionPoint(dX, dY,dZ);
	CNSTElement oPointOri = element.subnode(_T("PointOrientation"));
	double nOrientation = _wtof(oPointOri.val().c_str());
	pBlock->setRotation(nOrientation);
	CNSTElement oPointAlign = element.subnode(_T("PointAlignment"));
	int nAlignment = _wtoi(oPointAlign.val().c_str());
	pBlock->setAlignment(nAlignment);
	m_pCompGeometry->add(pBlock);
	return NS_SUCCESS;
}

int CNSPoint::readDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			This function writes block information i.e block insertion point, 
				rotation and orientation into Drawing file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSPoint::writeXML()
{
	int nPosListSize = m_pCompGeometry->getPoslistSize();
	NSSTRING strPosList;
	CNSBlock *pBlock = NULL;
	GEO_ELEMENT enType = ELE_INVALID;
	for(int nListCntr = 0; nListCntr < nPosListSize; nListCntr++)
	{
		pBlock = (CNSBlock*) m_pCompGeometry->getAt(nListCntr);
		pBlock->getData(strPosList, enType);
		CNSTElement oAttrib = m_pElement->createAttribute(_T("gml:Point"), _T("srsName"), _T("EPSG:28356"));
		oAttrib.createNode(_T("gml:pos"), strPosList);
		
		TCHAR strOrien[20] = _T("");
		NSSPRINTF(strOrien, 20, _T("%lf"),pBlock->getRotation());
		m_pElement->createNode(_T("PointOrientation"), strOrien, DEFAULT_NAMESPACE);

		TCHAR strAlign[20] = _T("");
		NSSPRINTF(strAlign, 20, _T("%i"),pBlock->getAlignment());
		m_pElement->createNode(_T("PointAlignment"), strAlign, DEFAULT_NAMESPACE);
	}
	return NS_SUCCESS;
}

int CNSPoint::writeDWG()
{
	return NS_SUCCESS;
}

NSSTRING CNSPoint::getLineString() const
{
	return _T("");
}
void CNSPoint::getXData(STRING_MAP& mapOfXData)
{
	CNSPoint3D Point3d;
	TCHAR szFormat[(_MAX_PATH + 1)];
	NSSPRINTF(szFormat,(_MAX_PATH + 1), _T("%lf,%lf,%lf"), Point3d.m_dXX, Point3d.m_dYY, Point3d.m_dZZ);
	mapOfXData.insert(STRING_MAP::value_type(_T("InsertionPoint"), szFormat));
	
}
NSSTRING CNSPoint::getName() const
{
	CNSBlock* pBlock = (CNSBlock*) m_pCompGeometry->getAt(0);
	return pBlock->getBlockName();
}

bool CNSPoint::getInsertionPoint(CNSPoint3D &pt)
{
	CNSBlock* pBlock = (CNSBlock*) m_pCompGeometry->getAt(0);
	if(!pBlock) return false;
	pBlock->getInsertionpoint(pt);
	return true;
}
