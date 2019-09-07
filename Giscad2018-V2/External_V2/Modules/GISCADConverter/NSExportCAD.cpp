#include "StdAfx.h"

#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSReportWriter.h"

#include "NSExportCAD.h"
#include "NSExportHeader.h"
#include "NSExportedFeatureContainer.h"
#include "NSXMLParser.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

// Constructor
CNSExportCAD::CNSExportCAD(void)
{
		m_pDwgMgr = NULL;
		m_pDbMgr = NULL;
		m_nFeatureCount = 0;
}

// Destructor
CNSExportCAD::~CNSExportCAD(void)
{
}

/*!
@Brief			This function reads GISExport informaiton from XML file.
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSExportCAD::readXML()
{
	CNSTElement oGisExportNode = m_pElement->subnode(_T("GISExport"));
	if(oGisExportNode.getPosition() == -1)
	{
		//MessageBox(NULL, _T("HH"),_T("Test"), MB_OK);
		WriteToLog(LOG_OTHER, _T("GISExport element not found."));
		return NS_FAIL;
	}
	//MessageBox(NULL, _T("HH1"), _T("Test"), MB_OK);
	CNSTElement gisExportHeaderNode = oGisExportNode.subnode(_T("Header"));
	CNSTElement exportedFeaturesNode = oGisExportNode.subnode(_T("ExportedFeatures"));
	// Read Header information from XML file
	if(NS_SUCCESS != m_exportHeader.readXML(gisExportHeaderNode))
	{
		WriteToLog(LOG_WARNINGS, _T("Error While reading Header information from XML"));
		return NS_FAIL;
	}
	// Read ExportFeature information from XML file.
	m_exportFeatCont.setObject(m_pDbMgr);
	if(NS_SUCCESS != m_exportFeatCont.readXML(exportedFeaturesNode))
		return NS_FAIL;
	m_strReportName = _T("A") + m_exportHeader.getDrawingNumber() + _T("_") + m_exportHeader.getProjectNumber() 
		+ _T("_") + m_exportHeader.getExportID() + _T(".log");
	return NS_SUCCESS;
}

/*!
@Brief			This function writes Export informaiton into Drawing file.
@Return			NS_SUCCESS : If writing process successful.
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSExportCAD::writeDWG()
{
	// Write Header information to DWG file
	m_exportHeader.setObject(*m_pDwgMgr);
	m_exportHeader.setObject(*m_pDbMgr);
	if(NS_SUCCESS != m_exportHeader.writeDWG())
	{
		WriteToLog(LOG_WARNINGS, _T("Error While Writing Header information into Drawing"));
		return NS_FAIL;
	}
	
	// Read ExportFeature information from XML file.
	m_exportFeatCont.setObject(m_pDwgMgr);
	m_exportFeatCont.setObject(m_pDbMgr);
	int i =0;
	if(NS_SUCCESS != m_exportFeatCont.writeDWG())
	{
		i=1;
	}
		//return NS_FAIL;
	setFeatureCount(m_exportFeatCont.getFeatureCount());
	return NS_SUCCESS;
}

void CNSExportCAD::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSExportCAD::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}

void CNSExportCAD::setObject(CNSTElement* pElement)
{
	m_pElement = pElement;
}

int CNSExportCAD::getFeatureCount() const
{
	return m_nFeatureCount;
}

void CNSExportCAD::setFeatureCount(const int nFeatureCount)
{
	m_nFeatureCount = nFeatureCount;
}

NSSTRING CNSExportCAD::getReportName() const
{
	return m_strReportName;
}