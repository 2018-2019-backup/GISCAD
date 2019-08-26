#include "StdAfx.h"
#include "NSConnectivity.h"
#include "NSConverterUtils.h"

// Constructor
CNSConnectivity::CNSConnectivity(void)
{
}

// Destructor
CNSConnectivity::~CNSConnectivity(void)
{
	PAIR_VECT::iterator pairItr;
	for(pairItr = m_vectNode1.begin(); pairItr != m_vectNode1.end(); pairItr++)
	{
		delete (*pairItr);
	}
	for(pairItr = m_vectNode2.begin(); pairItr != m_vectNode2.end(); pairItr++)
	{
		delete (*pairItr);
	}
}

void CNSConnectivity::setElement( CNSTElement &element)
{
	m_pElement = &element;
}

/*!
@Brief			This function reads Connectivity informaiton from XML file.
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSConnectivity::readXML()
{
	if(m_pElement->getLength() <= 0) return NS_SUCCESS;
	CNSTElement oNode; 
	for(oNode = m_pElement->begin(); oNode != m_pElement->end(); oNode++)
	{
		CNSPair *pPair = new CNSPair;
		pPair->setElement(oNode);
		pPair->readXML();
		if(oNode.name() == _T("Node1"))
			m_vectNode1.push_back(pPair);
		else
			m_vectNode2.push_back(pPair);
	}
	return NS_SUCCESS;
}

int CNSConnectivity::readDWG()
{
	return 0;
}

/*!
@Brief			This function writes Connectivity informaiton into XML file.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSConnectivity::writeXML()
{
	if(m_vectNode1.size() == 0 && m_vectNode2.size() == 0) return NS_SUCCESS;
	
		bool bHasValidChildren = false;
		////////////////////////////////////////////////////////////////////////////////////////////////
		PAIR_VECT::iterator iterPair;

		for(iterPair = m_vectNode1.begin();iterPair != m_vectNode1.end();iterPair++)
		{
			int nFID = (*iterPair)->getFID();

			// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
			//and FID is positive i.e. existing features
			if (!((  gRelFlag == 1 ) && (nFID > 0)))
			{
				bHasValidChildren  = true;
				break;
			}
			
		}

		for(iterPair = m_vectNode2.begin();iterPair != m_vectNode2.end();iterPair++)
		{
			int nFID = (*iterPair)->getFID();

			// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
			//and FID is positive i.e. existing features
			if (!((  gRelFlag == 1 ) && (nFID > 0)))
			{
				bHasValidChildren  = true;
				break;
			}
			
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
	
	
if(bHasValidChildren)
{	
	
	CNSTElement connectivityNode = m_pElement->createNode(_T("Connectivity"), _T(""), DEFAULT_NAMESPACE);
	//PAIR_VECT::iterator iterPair;
	for(iterPair = m_vectNode1.begin();iterPair != m_vectNode1.end();iterPair++)
	{
		int nFID = (*iterPair)->getFID();
		// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
		//and FID is positive i.e. existing features
		if (!((  gRelFlag == 1 ) && (nFID > 0)))
		{
			CNSTElement node1Node = connectivityNode.createNode(_T("Node1"), _T(""), DEFAULT_NAMESPACE);
			TCHAR szValue[20] = _T(" ");
			NSSPRINTF(szValue, 20, _T("%d"), (*iterPair)->getFID());
			node1Node.createNode(_T("FID"), szValue, DEFAULT_NAMESPACE);
			NSSPRINTF(szValue, 20, _T("%d"), (*iterPair)->getFNO());
			node1Node.createNode(_T("FNO"), szValue, DEFAULT_NAMESPACE);
		}
	}
	for(iterPair = m_vectNode2.begin();iterPair != m_vectNode2.end();iterPair++)
	{
		int nFID = (*iterPair)->getFID();
		// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
		//and FID is positive i.e. existing features
		if (!((  gRelFlag == 1 ) && (nFID > 0)))
		{
			CNSTElement node1Node = connectivityNode.createNode(_T("Node2"), _T(""), DEFAULT_NAMESPACE);
			TCHAR szValue[20] = _T("");
			NSSPRINTF(szValue, _T("%d"), (*iterPair)->getFID());
			node1Node.createNode(_T("FID"), szValue, DEFAULT_NAMESPACE);
			NSSPRINTF(szValue, _T("%d"), (*iterPair)->getFNO());
			node1Node.createNode(_T("FNO"), szValue, DEFAULT_NAMESPACE);
		}
	}
}
	return 0;
}

int CNSConnectivity::writeDWG()
{
	return 0;
}

/*!
@Brief			create Xdata comma separated string for vector of nodes
@Param [in]		map of connectivity Nnode XData information
@Return			none
@Date			12-06-2007
*/
int CNSConnectivity::getXDataString(PAIR_VECT &vectNodes, NSSTRING &strXData)
{
	TCHAR szFormat[_MAX_PATH + 1];
	strXData = _T("");
	int nSize = (int)vectNodes.size();
	if(nSize == 0) return NS_FAIL;
	for (int nListCntr = 0; nListCntr < nSize - 1; nListCntr++)
	{
		NSSPRINTF(szFormat, _T("%i,%i,"), vectNodes[nListCntr]->getFID(), vectNodes[nListCntr]->getFNO());
		strXData.append(szFormat);
	}
	NSSPRINTF(szFormat, _T("%i,%i"), vectNodes[nSize - 1]->getFID(), vectNodes[nSize - 1]->getFNO());
	strXData.append(szFormat);

	return NS_SUCCESS;
}

/*!
@Brief			create Xdata string for Node1 and Node2
@Param [in]		
@Return			map of connectivity XData information
@Date			12-06-2007
*/
void CNSConnectivity::getXData(STRING_MAP& mapOfRelationships)
{
	NSSTRING strXData;
	if(NS_SUCCESS == getXDataString(m_vectNode1, strXData))
		mapOfRelationships.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE1"), strXData));
	if(NS_SUCCESS == getXDataString(m_vectNode2, strXData))
		mapOfRelationships.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE2"), strXData));
} 

/*!
@Brief			The XData information of connectivity read from DWG is converted to class members
@Param [in]		map of connectivity XData information
@Return			none
@Date			12-06-2007
*/
void CNSConnectivity::setXData(STRING_MAP& mapOfRelationships)
{
	int nSize;
	std::map<NSSTRING, NSSTRING>::iterator xDataIter;
	xDataIter = mapOfRelationships.find(_T("CONNECTIVITY_NODE1"));
	if(xDataIter != mapOfRelationships.end())
	{
		NSSTRING strOwnershipParent = (*xDataIter).second;
		TCHAR* szNextToken;
		TCHAR* szToken = wcstok_s((TCHAR*)strOwnershipParent.c_str(), _T(","), &szNextToken);
		std::vector<TCHAR*> values;
		while(szToken != NULL)
		{
			values.push_back(szToken);
			szToken = wcstok_s (NULL, _T(","), &szNextToken);
		}
		nSize = (int)values.size();
		for(int i = 0; i < nSize; i = i + 2)
		{
			CNSPair *pPair = new CNSPair;
			pPair->setFID(NSATOI(values[i]));
			if(i < nSize - 1)
				pPair->setFNO(NSATOI(values[i+1]));
			m_vectNode1.push_back(pPair);
		}
	}
	xDataIter = mapOfRelationships.find(_T("CONNECTIVITY_NODE2"));
	if(xDataIter != mapOfRelationships.end())
	{
		NSSTRING strOwnershipParent = (*xDataIter).second;
		TCHAR* szNextToken;
		TCHAR* szToken = wcstok_s((TCHAR*)strOwnershipParent.c_str(), _T(","), &szNextToken);
		std::vector<TCHAR*> values;
		while(szToken != NULL)
		{
			values.push_back(szToken);
			szToken = wcstok_s (NULL, _T(","), &szNextToken);
		}
		nSize = (int)values.size();
		for(int i = 0; i < nSize; i = i + 2)
		{
			CNSPair *pPair = new CNSPair;
			pPair->setFID(NSATOI(values[i]));
			if(i < nSize - 1)
				pPair->setFNO(NSATOI(values[i+1]));
			m_vectNode2.push_back(pPair);
		}
	}
}

/*!
@Brief			The FIDs of connectable node features are retrived
@Param [in]		Node1 FID vector, Node2 FID vector
@Return			true/false
@Date			12-06-2007
*/
bool CNSConnectivity::getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2)
{
	PAIR_VECT::iterator pairItr;
	if(m_vectNode1.size() == 0 && m_vectNode2.size() == 0 ) return false;
	for(pairItr = m_vectNode1.begin(); pairItr != m_vectNode1.end(); pairItr++)
	{
		vectNode1.push_back((*pairItr)->getFID());
	}
	for(pairItr = m_vectNode2.begin(); pairItr != m_vectNode2.end(); pairItr++)
	{
		vectNode2.push_back((*pairItr)->getFID());
	}
	return true;
}