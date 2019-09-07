#include "StdAfx.h"
#include "NSExportedFeatureContainer.h"
#include "..\..\Include\NSDirectives.h"
#include "NSFeature.h"
#include "..\..\Include\NSReportWriter.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

// Constructor
CNSExportedFeatureContainer::CNSExportedFeatureContainer(void)
{
	m_nFeatureCount = 0;
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
}

// Destructor
CNSExportedFeatureContainer::~CNSExportedFeatureContainer(void)
{
	std::vector<CNSFeature*>::iterator featureItr;
	for(featureItr = m_vectFeatures.begin(); featureItr != m_vectFeatures.end(); featureItr++)
	{
		delete (*featureItr);
	}
}

/*!
@Brief			This function reads all the exported features information from XML.
@Param [in]		ele			:	ExportedFeatures node
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSExportedFeatureContainer::readXML(CNSTElement &ele)
{
	for (CNSTElement element = ele.begin(); element != ele.end(); element++)
	{
		CNSFeature *pFeature = new CNSFeature;
		pFeature->setObject(m_pDbMgr);
		if(NS_SUCCESS != pFeature->readXML(element))
		{
			TCHAR szMsg[_MAX_PATH];
			NSSPRINTF(szMsg, _T("Error while reading Feature Information of FNO = %d and FID = %d from XML"), pFeature->getFNO(), pFeature->getFID());
			WriteToLog(LOG_WARNINGS, szMsg);
			continue;
		}
		m_vectFeatures.push_back(pFeature);
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes all the exported features information into Drawing.
@Return			NS_SUCCESS : If writing process successful.
@Date			12-06-2007
*/
int CNSExportedFeatureContainer::writeDWG()
{
	CNSFeature *pFeature = NULL;
	std::vector<CNSFeature*>::iterator featureItr;
	for(featureItr = m_vectFeatures.begin(); featureItr != m_vectFeatures.end(); featureItr++)
	{
		pFeature = (*featureItr);
		pFeature->setObject(m_pDwgMgr);
		pFeature->setObject(m_pDbMgr);
		int nResult = pFeature->writeDWG();
		if(NS_SUCCESS == nResult)
			m_nFeatureCount = m_nFeatureCount + 1;
		else
		{
			TCHAR szMsg[_MAX_PATH];
			NSSPRINTF(szMsg, _T("Error while writing Feature with FNO = %d and FID = %d"), pFeature->getFNO(), pFeature->getFID());
			WriteToLog(LOG_WARNINGS, szMsg);
			continue;
		}
	}
	return NS_SUCCESS;
}

std::vector<CNSFeature*> CNSExportedFeatureContainer::getFeatures()
{
	return m_vectFeatures;
}

void CNSExportedFeatureContainer::setObject(CNSDWGMgr* objDwgMgr)
{
	m_pDwgMgr = objDwgMgr;
}

void CNSExportedFeatureContainer::setObject(CNSDatabaseMgr* objDbMgr)
{
	m_pDbMgr = objDbMgr;
}

int CNSExportedFeatureContainer::getFeatureCount() const
{
	return m_nFeatureCount;
}