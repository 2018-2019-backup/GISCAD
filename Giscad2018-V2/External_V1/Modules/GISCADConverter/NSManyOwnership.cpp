#include "StdAfx.h"
#include "NSManyOwnership.h"
#include "NSConverterUtils.h"



// Constructor
CNSManyOwnership::CNSManyOwnership(void)
{
	m_pElement = NULL;
}

// Destructor
CNSManyOwnership::~CNSManyOwnership(void)
{
	PAIR_VECT::iterator pairItr;
	for(pairItr = m_vectChild.begin(); pairItr != m_vectChild.end(); pairItr++)
	{
		delete (*pairItr);
	}
}

void CNSManyOwnership::setElement( CNSTElement &element)
{
	m_pElement = &element;
}

/*!
@Brief			This function reads ownership informaiton from XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSManyOwnership::readXML()
{
	if(m_pElement->getLength() <= 0) return NS_SUCCESS;
	CNSTElement node; 
	for(node = m_pElement->begin(); node != m_pElement->end(); node++)
	{
		CNSPair *pPair = new CNSPair;
		pPair->setElement(node);
		pPair->readXML();
		m_vectChild.push_back(pPair);
	}
	return NS_SUCCESS;
}

int CNSManyOwnership::readDWG()
{
	return NS_SUCCESS;
}
/*!
@Brief			This function writes ownership informaiton into XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSManyOwnership::writeXML()
{
	if(m_vectChild.size() > 0)
	{
		bool bHasValidChildren = false;
		////////////////////////////////////////////////////////////////////////////////////////////////
		PAIR_VECT::iterator iterPair;
		for(iterPair = m_vectChild.begin();iterPair != m_vectChild.end();iterPair++)
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
			CNSTElement manyOwnerShipNode = m_pElement->createNode(_T("ManyOwnership"), _T(""), DEFAULT_NAMESPACE);
			//PAIR_VECT::iterator iterPair;
			for(iterPair = m_vectChild.begin();iterPair != m_vectChild.end();iterPair++)
			{
				int nFID = (*iterPair)->getFID();

				// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
				//and FID is positive i.e. existing features
				if (!((  gRelFlag == 1 ) && (nFID > 0)))
				{
						CNSTElement childNode = manyOwnerShipNode.createNode(_T("Child"), _T(""), DEFAULT_NAMESPACE);
						TCHAR szValue[20] = _T(" ");
						NSSPRINTF(szValue, 20, _T("%d"), (*iterPair)->getFID());
						childNode.createNode(_T("FID"), szValue, DEFAULT_NAMESPACE);
						NSSPRINTF(szValue, 20, _T("%d"), (*iterPair)->getFNO());
						childNode.createNode(_T("FNO"), szValue, DEFAULT_NAMESPACE);
				}
				
			}
		}
	}
	return NS_SUCCESS;
}
int CNSManyOwnership::writeDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			Format comma separated string of FID and FNO values
@Param [in]		XData map manyOwnership
@Return			none
@Date			27-07-2007
*/
int CNSManyOwnership::getXDataString(PAIR_VECT &vectNodes, NSSTRING &strXData)
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

void CNSManyOwnership::getXData(STRING_MAP& mapOfRelationships)
{
	NSSTRING strXData;
	if(NS_SUCCESS == getXDataString(m_vectChild, strXData))
		mapOfRelationships.insert(STRING_MAP::value_type(_T("MANYOWNERSHIP_CHILD"), strXData));
} 

/*!
@Brief			Read information retrive as XData map from drawing and store
@Param [in]		XData map manyOwnership
@Return			none
@Date			27-07-2007
*/
void CNSManyOwnership::setXData(STRING_MAP& mapOfRelationships)
{
	std::map<NSSTRING, NSSTRING>::iterator xDataIter;
	xDataIter = mapOfRelationships.find(_T("MANYOWNERSHIP_CHILD"));
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
		int nSize = (int)values.size();
		for(int i = 0; i < nSize; i = i + 2)
		{
			CNSPair *pPair = new CNSPair;
			pPair->setFID(NSATOI(values[i]));
			if(i < nSize - 1)
				pPair->setFNO(NSATOI(values[i+1]));
			m_vectChild.push_back(pPair);
		}
	}
}