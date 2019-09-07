#include "StdAfx.h"
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSReportWriter.h"

#include "NSComponentContainer.h"
#include "NSAttribute.h"
#include "NSPoint.h"
#include "NSCompositecurve.h"

//Constructor
CNSComponentContainer::CNSComponentContainer(void)
{
		m_pDwgMgr = NULL;
		m_pDbMgr = NULL;
		m_pBlk = NULL;
		m_pElement = NULL;
		m_pContainer = NULL;
		m_nNumRep = 0;
}

// Destructor
CNSComponentContainer::~CNSComponentContainer(void)
{
	std::vector<CNSComponent*>::iterator componentItr;
	for(componentItr = m_components.begin(); componentItr != m_components.end(); componentItr++)
	{
		delete (*componentItr);
	}
}

/*!
@Brief			This function reads components informaiton from drawing file.
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSComponentContainer::readDWG()
{
	TCHAR szQuery[_MAX_PATH + 1];
	NSSPRINTF(szQuery, _T("Select CNO from FeatureComponents where FNO = %d and Required = 1"), m_nFNO);
	int nDBRet = m_pDbMgr->executeQuery(szQuery);
	if(-1 != nDBRet)
	{
		std::vector<variant_t> vCNO;
		std::vector<variant_t>::iterator vCNOItr;
		bool bIsOrientedPointFound = false;
		m_pDbMgr->getValue(_T("CNO"), vCNO);
		if(vCNO.size() == 0) return NS_FAIL;

		std::map<int, int> mapOfComponents;
		std::map<int, int>::iterator CompItr;
		int nRepresentation = 0;
		for(vCNOItr = vCNO.begin(); vCNOItr != vCNO.end(); vCNOItr++)
		{
			mapOfComponents.insert(std::map<int, int>::value_type((*vCNOItr), 0));
		}
		bool bResult = getSecondaryComponents(mapOfComponents);
		for(vCNOItr = vCNO.begin(); vCNOItr != vCNO.end(); vCNOItr++)
		{
			int nCNO = (*vCNOItr);
			NSSPRINTF(szQuery, _T("Select GeometryType from ComponentGeometryType where CNO = %d"), nCNO);
			nDBRet = m_pDbMgr->executeQuery(szQuery);
			if(-1 != nDBRet)
			{
				variant_t vGeoType;
				bool bRes = m_pDbMgr->getValue(_T("GeometryType"), vGeoType);
				if(bRes)
				{
					NSSTRING strGeoType = (NSSTRING)(vGeoType.bstrVal);
					if(NSSTRCMP(strGeoType.c_str(), _T("OrientedPointGeometry")) == 0)
					{
						CompItr = mapOfComponents.find(nCNO);
						if(CompItr != mapOfComponents.end())
						{
							nRepresentation = (*CompItr).second;
						}
						if(bIsOrientedPointFound == true)
						{
							continue;
						}
						//If nothing is mentioned add only first component encountered
						if(bResult == false)	
							bIsOrientedPointFound = true;
						//Skip all the other components if Primary/Secondary components found
						else if(bResult == true && nRepresentation == 0)	
							continue;
					}
					if(NSSTRCMP(strGeoType.c_str(), _T("TextPointGeometry")) != 0 || m_nFNO == 369)
					{
						CNSComponent *pComponent = new CNSComponent();
						CNSCompositeGeometry *pNewCompGeo = NULL;
						CNSCompositeGeometry *pCompGeo = m_pContainer->getAt(0);
						pNewCompGeo = new  CNSCompositeGeometry;
						*pNewCompGeo = *pCompGeo;
						pComponent->setObject(pNewCompGeo);
						pComponent->setObject(m_pDbMgr);
						pComponent->setObject(m_pDwgMgr);
						int nCNO = (*vCNOItr);
						pComponent->setComponentNumber(nCNO);
						pComponent->setComponentID(m_pContainer->getAt(0)->getCID());
						pComponent->setGeoType(strGeoType);
						if(NS_SUCCESS != pComponent->readDWG())
							return NS_FAIL;
						m_components.push_back(pComponent);
					}
				}
			}	
			else
			{
				WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'ComponentGeometryType\'"));
				return NS_FAIL;
			}
		}

	}
	else
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'FeatureComponents\'"));
		return NS_FAIL;
	}
    if(m_components.size() == 0) 
    {
        TCHAR szMsg[_MAX_PATH + 1];
        NSSPRINTF(szMsg, _T("Required component not found for FNO = %d"), m_nFNO);
		WriteToLog(LOG_OTHER, szMsg);
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function reads components informaiton from XML file.
@Param [in]		element			:	Feature Node
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/

int CNSComponentContainer::readXML(CNSTElement &element)
{
	for (CNSTElement elem = element.begin(); elem != element.end(); elem++)
	{
		CNSComponent *pComponent = new CNSComponent();
		pComponent->setObject(m_pDbMgr);
		if(NS_SUCCESS != pComponent->readXML(elem))
			return NS_FAIL;
		m_components.push_back(pComponent);
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes components informaiton into DWG.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSComponentContainer::writeDWG()
{
	std::vector<CNSComponent*>::iterator compIt;
	for(compIt = m_components.begin(); compIt < m_components.end(); compIt++)
	{
		(*compIt)->setObject(m_pDbMgr);
		(*compIt)->setObject(m_pDwgMgr);
		if(NS_SUCCESS != (*compIt)->writeDWG())
			return NS_FAIL;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes components informaiton into XML.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSComponentContainer::writeXML()
{

	CNSTElement compsNode = m_pElement->createNode(_T("Components"), _T(""), DEFAULT_NAMESPACE);
	std::vector<CNSComponent*>::iterator compIter;
	for(compIter = m_components.begin(); compIter != m_components.end(); compIter++)
	{
		CNSTElement compNode = compsNode.createNode(_T("Component"), _T(""), DEFAULT_NAMESPACE);
		(*compIter)->setElement(&compNode);
		(*compIter)->setFNO(m_FNOReference);
		(*compIter)->writeXML();
		//if((*compIter)->getComponentType() == ELE_BLK_POINT && m_nNumRep == 1)
		//{
		//	CNSTElement compNode = compsNode.createNode(_T("Component"), _T(""), DEFAULT_NAMESPACE);
		//	(*compIter)->setElement(&compNode);
		//	(*compIter)->setComponentID((*compIter)->getComponentID() + 1);
		//	(*compIter)->writeXML();
		//}
	}

	return NS_SUCCESS;
}

std::vector<CNSComponent*>& CNSComponentContainer::getComponents()
{
	return m_components;
}

void CNSComponentContainer::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSComponentContainer::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}

void CNSComponentContainer::setObject(CNSBlock* pBlk)
{
	m_pBlk = pBlk;
}

void CNSComponentContainer::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

void CNSComponentContainer::setObject(CNSCompositeContainer *pCompositeContainer)
{
	m_pContainer = pCompositeContainer;
}

void CNSComponentContainer::setXData(STRING_MAP& mapOfXData)
{
	std::vector<CNSComponent*>::iterator compIter;
	CNSComponent* pComp = NULL;
	for(compIter = m_components.begin(); compIter != m_components.end(); compIter++)
	{
		pComp = *compIter;
		pComp->setXData(mapOfXData);
	}
}

void CNSComponentContainer::setFNO(const int nFNO)
{
	m_nFNO = nFNO;
}

/*!
@Brief			Add component in the container with specifies geometry , comp. number, comp. ID
@Param [in]		enType : element type
@Return			CNSGeometry*   geometry object
@Date			12-06-2007
*/
void CNSComponentContainer::AddComponent(CNSCompositeGeometry *pCompGeometry, int nCompNo, int nCompID)
{
	CNSComponent *pComponent = new CNSComponent();
	pComponent->setComponentNumber(nCompNo);
	pComponent->setComponentID(nCompID);
	pComponent->setObject(m_pDbMgr);
	pComponent->createGeometry();
	CNSGeometry *pGeometry = pComponent->getGeometry();
	if(pGeometry)
	{
		CNSCompositeGeometry *pCompGeo = pGeometry->getCompositeGeo();
		*pCompGeo = *pCompGeometry;
		m_components.push_back(pComponent);
	}
}

void CNSComponentContainer::setDualRepresent(const int nRep)
{
	m_nNumRep = nRep;
}

/*!
@Brief			Retrives the insertion point of OrientedPointGeometry component, 
				used for Cable end points as block insertion points 
@Param [in]		
@Return			insertion point of OrientedPointGeometry component
@Date			12-06-2007
*/
bool CNSComponentContainer::getInsertionPoint(CNSPoint3D &insertionPt)
{
	std::vector<CNSComponent*>::iterator compIter;
	CNSComponent* pComp = NULL;
	CNSPoint *pPoint = NULL;
	for(compIter = m_components.begin(); compIter != m_components.end(); compIter++)
	{
		pComp = *compIter;
		if(ELE_BLK_POINT == pComp->getComponentType())
		{
			pPoint = (CNSPoint*)((*compIter)->getGeometry());
			if(pPoint != NULL)
				return pPoint->getInsertionPoint(insertionPt);
		}
	}
	return false;
}

/*!
@Brief			The connectble end points of the block to which cable is attached at both ends
				used for Cable end points as block insertion points conversion
@Param [in]		vetor of insertion points at both the ends
@Return			true/false
@Date			12-06-2007
*/
bool CNSComponentContainer::setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts)
{
	std::vector<CNSComponent*>::iterator compIter;
	CNSComponent* pComp = NULL;
	CNSCompositeCurve *pCurve = NULL;
	for(compIter = m_components.begin(); compIter != m_components.end(); compIter++)
	{
		pComp = *compIter;
		if(ELE_LINEAR == pComp->getComponentType())
		{
			pCurve = (CNSCompositeCurve*)((*compIter)->getGeometry());
			if(pCurve != NULL)
				return pCurve->setCollinearPoints(vectNode1Pts, vectNode2Pts);
		}
	}
	return true;
}

bool CNSComponentContainer::getSecondaryComponents(std::map<int, int> &mapOfComponents)
{
	bool bResult = false;
	std::map<int, int>::iterator CompItr;
	for(CompItr = mapOfComponents.begin(); CompItr != mapOfComponents.end(); CompItr++)
	{
		TCHAR szQuery[_MAX_PATH + 1];
		NSSPRINTF(szQuery, _T("Select CompRepresentation from FeatureComponents where FNO = %d and CNO = %d"), m_nFNO, (*CompItr).first);
		int nDBRet = m_pDbMgr->executeQuery(szQuery);
		if(-1 != nDBRet)
		{
			variant_t  vCompRepresentation;
			bool bRes = m_pDbMgr->getValue(_T("CompRepresentation"), vCompRepresentation);
			if(bRes)
			{
				if(vCompRepresentation.iVal == 1 || vCompRepresentation.iVal == 2)
				{
					bResult = true;
				}
				(*CompItr).second = vCompRepresentation.iVal;
			}
		}
	}
	return bResult;
}


