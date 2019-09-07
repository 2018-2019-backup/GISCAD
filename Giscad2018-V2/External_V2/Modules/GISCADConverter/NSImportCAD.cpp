#include "StdAfx.h"

#include "..\..\Include\NSReportWriter.h"

#include "NSImportCAD.h"
#include "NSTElement.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

// Constructor
CNSImportCAD::CNSImportCAD(void)
{
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pElement = NULL;
	m_nModFeatures = 0;
	m_nNewFeatures = 0;
	m_nDelFeatures = 0;
}

// Destructor
CNSImportCAD::~CNSImportCAD(void)
{

}

// This function reads header and feature informaiton from Drawing.
int CNSImportCAD::readDWG()
{
	int nRet=1;// = NS_FAIL;
	m_importHeader.setObject(*m_pDwgMgr); 
	nRet=m_importHeader.readDWG();
switch(nRet)
{
case -1001:return nRet;break;
case -1004:return nRet;break;

}

	m_importFeatContainer.setObject(m_pDbMgr);
	m_importFeatContainer.setObject(m_pDwgMgr);

	//Get drawing number from Title Block set to FB Reference attribute.
	m_pDwgMgr->setDrawingNumber(m_importHeader.getDrawingNumFromMap());

	if(NS_FAIL == m_importFeatContainer.readDWG())
	{
		return NS_FAIL;
	}
	return nRet;
}

/*!
@Brief			This function writes Header information into XML.
@Return			NS_SUCCESS : If writing process successful.
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSImportCAD::writeXML()
{
	CNSTElement headerNode = m_pElement->createNode(_T("Header"), _T(""), DEFAULT_NAMESPACE);
	m_importHeader.setElement(&headerNode);
	// Writing Header information
	if(NS_SUCCESS != m_importHeader.writeXML())
		return NS_FAIL;
	m_importFeatContainer.setElement(m_pElement);
	m_importFeatContainer.setObject(m_pDbMgr);
	// Write feature informaion
	if(NS_SUCCESS != m_importFeatContainer.writeXML())
		return NS_FAIL;

	// Report Name formatting.
	m_strReportLocation = _T("A") + m_importHeader.getDrawingNumber() + 
		_T("_") + m_importHeader.getProjectNumber() + _T("_") + m_importHeader.getExportID() + _T(".log");
	
	// Count for Features
	m_nModFeatures = m_importFeatContainer.getModFeatures();
	m_nNewFeatures = m_importFeatContainer.getNewFeatures();
	m_nDelFeatures = m_importFeatContainer.getDelFeatures();
	return NS_SUCCESS;
}

void CNSImportCAD::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSImportCAD::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}

void CNSImportCAD::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

int CNSImportCAD::getModFeatures() const 
{
	return m_nModFeatures;
}

int CNSImportCAD::getNewFeatures() const 
{
	return m_nNewFeatures;
}

int CNSImportCAD::getDelFeatures() const 
{
	return m_nDelFeatures;
}

NSSTRING CNSImportCAD::getReportLocation() const
{
	return m_strReportLocation;
}