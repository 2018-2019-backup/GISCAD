#include "StdAfx.h"
#include "NSPair.h"

// Constructor
CNSPair::CNSPair(void)
{
	m_nFID = 0;
	m_nFNO = 0;
	m_pElement = NULL;
}

// Destructor
CNSPair::~CNSPair(void)
{
}

void CNSPair::setElement(CNSTElement &element)
{
	m_pElement = &element;
}


int CNSPair::readXML()
{
	m_nFID = NSATOI((m_pElement->subval(_T("FID"))).c_str());
	m_nFNO = NSATOI((m_pElement->subval(_T("FNO"))).c_str());
	return 0;
}

int CNSPair::readDWG()
{
	return 0;
}

int CNSPair::writeXML()
{
	return 0;
}

int CNSPair::writeDWG()
{
	return 0;
}

int CNSPair::getFID()const
{
	return m_nFID;
}

int CNSPair::getFNO()const
{
	return m_nFNO;
}

void CNSPair::setFID(const int nFID)
{
	m_nFID = nFID;
}

void CNSPair::setFNO(const int nFNO)
{
	m_nFNO = nFNO;
}
