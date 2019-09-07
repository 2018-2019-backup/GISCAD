#include "StdAfx.h"
#include "NSOwnership.h"
#include "NSConverterUtils.h"

// Constructor
CNSOwnership::CNSOwnership(void)
{
	m_pElement = NULL;
}

// Destructor
CNSOwnership::~CNSOwnership(void)
{
	PAIR_VECT::iterator pairItr;
	for(pairItr = m_vectParent.begin();pairItr != m_vectParent.end();pairItr++)
	{
		delete (*pairItr);
	}
	for(pairItr = m_vectChild.begin();pairItr != m_vectChild.end();pairItr++)
	{
		delete (*pairItr);
	}
}

void CNSOwnership::setElement( CNSTElement &element)
{
	m_pElement = &element;
}

/*!
@Brief			This function reads ownership informaiton from XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSOwnership::readXML()
{
	if(m_pElement->getLength() <= 0) return NS_SUCCESS;
	CNSTElement oNode; 
	for(oNode = m_pElement->begin(); oNode != m_pElement->end(); oNode++)
	{
		CNSPair *pPair = new CNSPair;
		pPair->setElement(oNode);
		pPair->readXML();
		if(oNode.name() == _T("Parent"))
			m_vectParent.push_back(pPair);
		else
			m_vectChild.push_back(pPair);
	}
	return 0;
}

int CNSOwnership::readDWG()
{
	return 0;
}

/*!
@Brief			This function writes ownership informaiton into XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSOwnership::writeXML()
{
	if(m_vectParent.size() == 0 && m_vectChild.size() == 0) return NS_SUCCESS;

	bool bHasValidChildren = false;
	////////////////////////////////////////////////////////////////////////////////////////////////
		PAIR_VECT::iterator parItr;

		for(parItr = m_vectParent.begin();parItr != m_vectParent.end();parItr++)
		{
			int nFID = (*parItr)->getFID();

			// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
			//and FID is positive i.e. existing features
			if (!((  gRelFlag == 1 ) && (nFID > 0)))
			{
				bHasValidChildren  = true;
				break;
			}			
		}

		for(parItr = m_vectChild.begin();parItr != m_vectChild.end();parItr++)
		{
			int nFID = (*parItr)->getFID();

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

		CNSTElement ownershipNode = m_pElement->createNode(_T("Ownership"), _T(""), DEFAULT_NAMESPACE);
		//PAIR_VECT::iterator parItr;

		for(parItr = m_vectParent.begin();parItr != m_vectParent.end();parItr++)
		{
			int nFID = (*parItr)->getFID();

			// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
			//and FID is positive i.e. existing features
			if (!((  gRelFlag == 1 ) && (nFID > 0)))
			{
						CNSTElement parentNode = ownershipNode.createNode(_T("Parent"), _T(""), DEFAULT_NAMESPACE);
						TCHAR szValue[20] = _T(" ");
						NSSPRINTF(szValue, 20, _T("%d"), (*parItr)->getFID());
						parentNode.createNode(_T("FID"), szValue, DEFAULT_NAMESPACE);
						NSSPRINTF(szValue, 20, _T("%d"), (*parItr)->getFNO());
						parentNode.createNode(_T("FNO"), szValue, DEFAULT_NAMESPACE);
			}

		}

		for(parItr = m_vectChild.begin(); parItr != m_vectChild.end();parItr++)
		{
			int nFID = (*parItr)->getFID();

			// To restrict writing to XML when RELATION_FLAG in settings.ini is set to 1 
			//and FID is positive i.e. existing features
			if (!((  gRelFlag == 1 ) && (nFID > 0)))
			{
						CNSTElement childNode = ownershipNode.createNode(_T("Child"), _T(""), DEFAULT_NAMESPACE);
						TCHAR szValue[20] = _T(" ");
						NSSPRINTF(szValue, 20, _T("%d"), (*parItr)->getFID());
						childNode.createNode(_T("FID"), szValue, DEFAULT_NAMESPACE);
						NSSPRINTF(szValue, 20, _T("%d"), (*parItr)->getFNO());
						childNode.createNode(_T("FNO"), szValue, DEFAULT_NAMESPACE);
			}

		}
	}

	return NS_SUCCESS;
}
int CNSOwnership::writeDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			Format comma separated string of FID and FNO values
@Param [in]		XData map Ownership
@Return			none
@Date			27-07-2007
*/
int CNSOwnership::getXDataString(PAIR_VECT &vectNodes, NSSTRING &strXData)
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

void CNSOwnership::getXData(STRING_MAP& mapOfRelationships)
{
	NSSTRING strXData;
	if(NS_SUCCESS == getXDataString(m_vectParent, strXData))
		mapOfRelationships.insert(STRING_MAP::value_type(_T("OWNERSHIP_PARENT"), strXData));
	if(NS_SUCCESS == getXDataString(m_vectChild, strXData))
		mapOfRelationships.insert(STRING_MAP::value_type(_T("OWNERSHIP_CHILD"), strXData));
} 

/*!
@Brief			Read information retrive as XData map from drawing and store
@Param [in]		XData map Ownership
@Return			none
@Date			27-07-2007
*/
void CNSOwnership::setXData(STRING_MAP& mapOfRelationships)
{
	int nSize;
	std::map<NSSTRING, NSSTRING>::iterator xDataIter;
	xDataIter = mapOfRelationships.find(_T("OWNERSHIP_PARENT"));
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
			m_vectParent.push_back(pPair);
		}
	}
	xDataIter = mapOfRelationships.find(_T("OWNERSHIP_CHILD"));
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
			pPair->setFNO(NSATOI(values[i+1]));
			m_vectChild.push_back(pPair);
		}
	}
}