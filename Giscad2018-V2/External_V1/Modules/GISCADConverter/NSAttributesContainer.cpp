#include "StdAfx.h"
#include "NSAttributesContainer.h"

//Constructor
CNSAttributesContainer::CNSAttributesContainer(void)
{
	m_pElement = NULL;
}

// Destructor
CNSAttributesContainer::~CNSAttributesContainer(void)
{
	std::map<int, CNSAttribute*>::iterator it;
	CNSAttribute *pAttribute =  NULL;
	for(it = m_mapOfAttributes.begin(); it != m_mapOfAttributes.end(); it++)
	{
		pAttribute = (*it).second;
		delete pAttribute;
		pAttribute = NULL;
	}	
}

/*!
@Brief			This function reads attributes information strored and return the ANO, attribure pair map.
@Param [in]		
@Return			Atribute map.
@Date			12-06-2007
*/
void CNSAttributesContainer::getAttributes(std::map<int, CNSAttribute*> & mapAttributes)
{
	std::map<int, CNSAttribute*>::iterator AttrIter;
	for(AttrIter = m_mapOfAttributes.begin(); AttrIter != m_mapOfAttributes.end(); AttrIter++)
	{
		mapAttributes.insert(std::map<int, CNSAttribute*>::value_type((*AttrIter).first, (*AttrIter).second));
	}
}

int CNSAttributesContainer::getSize()const
{
	return (int)m_mapOfAttributes.size();
}

/*!
@Brief			This function reads attributes informaiton from XML.
@Param [in]		elem		featureNode
@Return			After successful completion of reading process this function
				returns NS_SUCCESS.
@Date			12-06-2007
*/
int CNSAttributesContainer::readXML(CNSTElement &elem)
{
	for (CNSTElement element = elem.begin(); element != elem.end(); element++)
	{
		CNSAttribute *pAttribute = new CNSAttribute;
		pAttribute->readXML(element);
		m_mapOfAttributes.insert(std::map<int, CNSAttribute*>::value_type(pAttribute->getAttributeNumber(), pAttribute));
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function Writes attributes informaiton into XML.
@Return			after completion of writing process returns NS_SUCCESS.
@Date			12-06-2007
*/
int CNSAttributesContainer::writeXML()
{
	CNSTElement oAttrsNode = m_pElement->createNode(_T("Attributes"), _T(""), DEFAULT_NAMESPACE);
	std::map<int, CNSAttribute*>::iterator attIter;
	CNSAttribute *pAttribute = NULL;
	for(attIter = m_mapOfAttributes.begin(); attIter != m_mapOfAttributes.end(); attIter++)
	{
		pAttribute = (*attIter).second;
		if(pAttribute)
		{
			pAttribute->setElement(&oAttrsNode);
			if(NS_SUCCESS != pAttribute->writeXML())
				return NS_FAIL;
		}
	}
	return NS_SUCCESS;
}
void CNSAttributesContainer::setElement(CNSTElement *pElement)
{
	m_pElement = pElement;
}

/*!
@Brief			Adds attributes to the attribute map.
@Param [in]		nCID-ComponentNumber, nANO-AttributeNumber, strName-AttributeName, strValue-AttributeValue
@Return			None
@Date			12-06-2007
*/
void CNSAttributesContainer::addAttribute(int nCID, int nANO, NSSTRING strName, NSSTRING strValue)
{
	std::map<int, CNSAttribute*>::iterator it;
	CNSAttribute *pAtt = new CNSAttribute;
	pAtt->setAttributeName(strName);
	pAtt->setAttributeNumber(nANO);
	pAtt->setDataValue(nCID, strValue);
	m_mapOfAttributes.insert(std::map<int, CNSAttribute*>::value_type(nANO, pAtt));
}

/*!
@Brief			Create string XData map entries from the attributes map.
@Param [in]		
@Return			String map of attributes used to post as XData in drawing
@Date			12-06-2007
*/
void CNSAttributesContainer::getXData(STRING_MAP& mapOfXData)
{
	std::map<int, CNSAttribute*>::iterator it;
	CNSAttribute *pAttribute = NULL;
	NSSTRING strXData;
	for(it = m_mapOfAttributes.begin(); it != m_mapOfAttributes.end(); it++)
	{
		pAttribute = (*it).second;
		if(pAttribute)
		{
			strXData = pAttribute->getXData();
			mapOfXData.insert(STRING_MAP::value_type(pAttribute->getAttributeName(), strXData));
		}
	}
}