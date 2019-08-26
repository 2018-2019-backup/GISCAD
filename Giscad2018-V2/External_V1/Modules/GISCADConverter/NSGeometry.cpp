#include "StdAfx.h"

#include "NSGeometry.h"

#include "..\..\Include\NSDirectives.h"

// Constructor
CNSGeometry::CNSGeometry(void)
{
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pCompGeometry = NULL;
	m_pBlk = NULL;
	m_pElement = NULL;
}

// Destructor
CNSGeometry::~CNSGeometry(void)
{
	if(m_pCompGeometry != NULL) 
	{
		delete m_pCompGeometry;
		m_pCompGeometry = NULL;
	}
}

int CNSGeometry::readDWG()
{
	return NS_SUCCESS;
}

int CNSGeometry::writeXML()
{
	return NS_SUCCESS;
}

int CNSGeometry::writeDWG()
{
	return NS_SUCCESS;
}

void CNSGeometry::setType(GEOMETRY_TYPE geoType)
{
	m_type = geoType;
}

enum GEOMETRY_TYPE CNSGeometry::getType()
{
	return m_type;
}


void CNSGeometry::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSGeometry::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}

void CNSGeometry::setObject(CNSBlock* pBlk)
{
	m_pBlk = pBlk;
}

NSSTRING CNSGeometry::getLineString()
{
	return _T("");
}

void CNSGeometry::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

void CNSGeometry::createComposite()
{
	m_pCompGeometry = new CNSCompositeGeometry; 
}
void CNSGeometry::setObject(CNSCompositeGeometry *pCompGeometry)
{
	m_pCompGeometry = pCompGeometry;
}