//#include "StdAfx.h"
#include "..\Include\NSCompositeGeometry.h"
#include "..\Include\NSBlock.h"

void CNSCompositeData::setData( NSSTRING strPoslist,  GEO_ELEMENT enType)
{
//	m_strPoslist = new TCHAR[strPoslist.size() + 1];
//	NSSTRCPY(m_strPoslist, strPoslist.size() + 1, strPoslist.c_str());


	m_strPoslist = strPoslist;
	m_enType = enType;
}

void CNSCompositeData::getData(NSSTRING &strPoslist, GEO_ELEMENT &enType)
{
	strPoslist = m_strPoslist ;
	enType = m_enType;
}

GEO_ELEMENT CNSCompositeData::getType()const
{
	return m_enType;
}

void CNSCompositeData::setType(const GEO_ELEMENT enType)
{
	m_enType = enType;
}

CNSCompositeGeometry::CNSCompositeGeometry(void)
{
	m_nCID = 1;
	m_nCNO = 0;
}

CNSCompositeGeometry::~CNSCompositeGeometry(void)
{
	std::vector<CNSCompositeData*>::iterator CompositeDataItr;
	CNSCompositeData *pData = NULL;
	for(CompositeDataItr = m_vectComposites.begin(); CompositeDataItr != m_vectComposites.end(); CompositeDataItr++)
	{
		pData = *CompositeDataItr;
		delete pData;
		pData = NULL;
	}
}

void CNSCompositeGeometry::addPoslist(GEO_ELEMENT enType, NSSTRING strPoslist, bool bClosed)
{
	CNSCompositeData *pData = NULL;
	pData = new CNSCompositeData;
	pData->setData(strPoslist, enType);
	m_bClosed = bClosed;
	m_vectComposites.push_back(pData);
}
void CNSCompositeGeometry::add(CNSCompositeData* pData)
{
	m_vectComposites.push_back(pData);
}

int CNSCompositeGeometry::getPoslistSize(void)
{
	return (int)m_vectComposites.size();
}
void CNSCompositeGeometry::getAt(int nIndex, NSSTRING &strPoslist, GEO_ELEMENT &enType)
{
	if(nIndex >= getPoslistSize()) return;
	m_vectComposites[nIndex]->getData(strPoslist, enType);
}

bool CNSCompositeGeometry::isClosed()
{
	return m_bClosed;
}

CNSCompositeData* CNSCompositeGeometry::getAt(int nIndex)
{
	if(nIndex >= getPoslistSize()) return NULL;
	return m_vectComposites.at(nIndex);
}
int CNSCompositeGeometry::getCID()const
{
	return m_nCID;
}

int CNSCompositeGeometry::getCNO()const
{
	return m_nCNO;
}

void CNSCompositeGeometry::setCID(int nCID)
{
	m_nCID = nCID;
}

void CNSCompositeGeometry::setCNO(int nCNO)
{
	m_nCNO = nCNO;
}

NSSTRING CNSCompositeGeometry::getLayername()const
{
	return m_strLayerName;
}
NSSTRING CNSCompositeGeometry::getName()const
{
	return m_strGeoName;
}

void CNSCompositeGeometry::setLayername(const NSSTRING strLayerName)
{
	m_strLayerName = strLayerName;
}

void CNSCompositeGeometry::setName(const NSSTRING strName)
{
	m_strGeoName = strName;
}

COMPONENT_TYPE CNSCompositeGeometry::getCompType()const
{
	return m_enComponentType;
}

void CNSCompositeGeometry::setCompType(const COMPONENT_TYPE enType)
{
	m_enComponentType = enType;
}

void CNSCompositeGeometry::setCompRepresentation(const short iCompRepresentation)
{
	m_iCompRepresentation = iCompRepresentation;
}

short CNSCompositeGeometry::getCompRepresentation() const
{
	return m_iCompRepresentation;
}

CNSCompositeContainer::CNSCompositeContainer(void)
{
}

CNSCompositeContainer::~CNSCompositeContainer(void)
{
	std::vector<CNSCompositeGeometry*>::iterator CompGeoIter;
	for(CompGeoIter = m_vectGeometries.begin(); CompGeoIter != m_vectGeometries.end(); CompGeoIter++)
	{
		delete *CompGeoIter;
	}
}

void CNSCompositeContainer::add(CNSCompositeGeometry* pCompGeometry)
{
	 m_vectGeometries.push_back(pCompGeometry);
}

CNSCompositeGeometry* CNSCompositeContainer::getAt(int nIndex)
{
	if(nIndex >= getSize()) return NULL;
	return m_vectGeometries.at(nIndex);
}

CNSCompositeGeometry& CNSCompositeGeometry::operator = (CNSCompositeGeometry& pCompGeometry)   // Assignment operator
{
	m_nCID = pCompGeometry.getCID();
	m_nCNO = pCompGeometry.getCNO();
	m_bClosed = pCompGeometry.isClosed();
	m_enComponentType = pCompGeometry.getCompType();
	m_strGeoName = pCompGeometry.getName();
	m_strLayerName = pCompGeometry.getLayername();
	m_iCompRepresentation = pCompGeometry.getCompRepresentation();

	std::vector<CNSCompositeData*>::iterator CompositeDataItr;
	int nSize = pCompGeometry.getPoslistSize();

	CNSCompositeData *pData = NULL;
	CNSCompositeData *pNewData = NULL;
	bool bPointGeo = false;
	for(int nCntr = 0; nCntr < nSize; nCntr++)
	{
		bPointGeo = false;
		pData = pCompGeometry.getAt(nCntr);
		if(pCompGeometry.getCompType() == COMP_POINT || pCompGeometry.getCompType() == COMP_TEXTPOINT)
		{
			pNewData = new CNSBlock;
			bPointGeo = true;
		}
		else
		{
			pNewData = new CNSCompositeData;
		}
		*pNewData = *pData;
		if(bPointGeo == true)
		{
			((CNSBlock*)pNewData)->setAlignment(((CNSBlock*)pData)->getAlignment());
			((CNSBlock*)pNewData)->setRotation(((CNSBlock*)pData)->getRotation());
		}
		add(pNewData);
	}
	return *this;
}

int CNSCompositeContainer::getSize()
{
	return (int)m_vectGeometries.size();
}

COMPONENT_TYPE CNSCompositeContainer::getCompType(int nCompNumber)
{
	if(getSize() > 0)
		return getAt(0)->getCompType();
	return COMP_INVALID;
}