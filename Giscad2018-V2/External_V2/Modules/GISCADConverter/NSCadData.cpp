#include "StdAfx.h"
#include "NSCadData.h"

// Constructor
CNSCadData::CNSCadData(void)
{
}

// Destructor
CNSCadData::~CNSCadData(void)
{
}

void CNSCadData::setSourceFileName(const TCHAR* szFileName)
{
	m_szSourceFileName = szFileName;
}

void CNSCadData::setDestinationFileName(const TCHAR* szFileName)
{
	m_szDestinationFileName = szFileName;
}

const TCHAR* CNSCadData::getSourceFileName()
{
	return m_szSourceFileName;
}

const TCHAR* CNSCadData::getDestinationFileName()
{
	return m_szDestinationFileName;
}