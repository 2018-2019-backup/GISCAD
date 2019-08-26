#include "StdAfx.h"
#include "NSAttribute.h"
#include "sstream"

// Constructor
CNSAttribute::CNSAttribute(void)
{
	m_nANO = 0;
	m_strName = _T("");
	m_pElement = NULL;
}

// Destructor
CNSAttribute::~CNSAttribute(void)
{
}

int CNSAttribute::getAttributeNumber() const
{
	return m_nANO;
}

void CNSAttribute::setAttributeNumber(const int nAttNo)
{
	m_nANO = nAttNo;
}

NSSTRING CNSAttribute::getAttributeName() const
{
	return m_strName;
}

void CNSAttribute::setAttributeName(const NSSTRING strAName)
{
	m_strName = strAName;
}

void CNSAttribute::setDataValue(int nCID, NSSTRING strValue)
{
	m_DataMap.insert(std::map<int, NSSTRING>::value_type(nCID, strValue));
}

void CNSAttribute::getDataValue(int nCID, NSSTRING &strValue)
{
	std::map<int, NSSTRING>::iterator itr;
	itr = m_DataMap.find(nCID);
	if(itr != m_DataMap.end())
	{
		strValue = (*itr).second;
	}
}

void CNSAttribute::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

/*!
@Brief			This function Writes attribute informaiton into XML.
@Return			after completion of writing process returns NS_SUCCESS.
@Date			12-06-2007
*/
int CNSAttribute::writeXML()
{
	CNSTElement node = m_pElement->createNode(_T("Attribute"), _T(""), DEFAULT_NAMESPACE);
	TCHAR strValue[20] = _T("");
	NSSPRINTF(strValue, _T("%d"), m_nANO);
	node.createNode(_T("ANO"), strValue, DEFAULT_NAMESPACE);
	node.createNode(_T("Name"), m_strName, DEFAULT_NAMESPACE);
	std::map<int, NSSTRING>::iterator DataItr;
	NSSTRING strXData;
	for(DataItr = m_DataMap.begin();DataItr != m_DataMap.end(); DataItr++)
	{
		//---------------Add Drawing number to modified features FB. ----
		if(NSSTRCMP(m_strName.c_str(), _T("FB_REFERENCE")) == 0)
	    {
			NSSTRING strFBRef = (*DataItr).second;
			m_vectTokStr vtrToken;
			m_vectTokStr::const_iterator vectItr;
			tokenizeString(strFBRef, _T(","), vtrToken);
			int count = 1;
			std::wstringstream strCIDVal;
			
			for(vectItr = vtrToken.begin(); vectItr!= vtrToken.end(); vectItr++)
			{
				CNSTElement dataNode = node.createNode(_T("Data"), _T(""), DEFAULT_NAMESPACE);
				strCIDVal << count;
				dataNode.createNode(_T("CID"), strCIDVal.str(), DEFAULT_NAMESPACE);
				dataNode.createNode(_T("Value"), (*vectItr).c_str(), DEFAULT_NAMESPACE);
				count++;
				strCIDVal.str(_T(""));
			}
			
	    }
		//---------------------------------------------------------------
		else
		{
			CNSTElement dataNode = node.createNode(_T("Data"), _T(""), DEFAULT_NAMESPACE);
			NSSPRINTF(strValue, _T("%d"), (*DataItr).first);
			dataNode.createNode(_T("CID"), strValue, DEFAULT_NAMESPACE);
			dataNode.createNode(_T("Value"), (*DataItr).second, DEFAULT_NAMESPACE);
		}
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function reads attribute informaiton from XML.
@Param [in]		element		Attributes node
@Return			After successful completion of reading process this function
				returns NS_SUCCESS.
@Date			12-06-2007
*/
int CNSAttribute::readXML(CNSTElement &element)
{
	if(element.getLength() <= 0) return NS_SUCCESS;

	// Read Attribute Number
	m_nANO = _wtoi(element.subval(_T("ANO")).c_str());
	// Read Attribute Name
	m_strName = element.subval(_T("Name"));

	CNSTElement oNode;
	// Read Data values
	int nCID;
	NSSTRING strDataValue;
	for(oNode = element.begin(); oNode != element.end(); oNode++)
	{
		if(oNode.name() == _T("Data"))
		{
			// Read CID value
				nCID = _wtoi(oNode.subval(_T("CID")).c_str());
			// Read Data value
				strDataValue = oNode.subval(_T("Value"));
				m_DataMap.insert(std::map<int, NSSTRING>::value_type(nCID, strDataValue));	
		}
	}
	return NS_SUCCESS;
}

int CNSAttribute::getDataSize() const
{
	return (int)m_DataMap.size();
}

/*!
@Brief			This function retrives data from the DataMap and fills AttributeRefMap
				used for transfer
@Return			Always success
@Date			12-06-2007
*/
int CNSAttribute::getData(CNSAttributeRefMap  *pAttrRefMap, NSSTRING strTagName)
{
	std::map<int, NSSTRING>::iterator DataItr;
	for(DataItr = m_DataMap.begin(); DataItr != m_DataMap.end(); DataItr++)
	{
		CNSAttributeRef* pAttrRef = new CNSAttributeRef;
		pAttrRef->setTagName(strTagName);
		pAttrRef->setAttributeValue((*DataItr).second);
		pAttrRefMap->addAttRef((*DataItr).first, pAttrRef);
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function creates XData comma separated string from Key, Value pair
@Return			Always success
@Date			12-06-2007
*/
NSSTRING CNSAttribute::getXData()
{
	std::map<int, NSSTRING>::iterator DataItr;
	NSSTRING strXData;
	DataItr = m_DataMap.begin();
	for(; DataItr != m_DataMap.end(); DataItr++)
	{
		strXData.append((*DataItr).second);
		if((int)m_DataMap.size() > 1)
			strXData.append(_T(","));
	}
	return strXData;
}

/*!
@Brief			Function to separate the tokanize string.
@Return			Return string
@Date			28 - 05 - 2012
*/

void CNSAttribute::tokenizeString(const NSSTRING& inputStr, const NSSTRING& delims, m_vectTokStr& tokens)
{
    tokens.clear();

    NSSTRING::size_type beg_index , end_index;

    beg_index = inputStr.find_first_not_of(delims);
	if(inputStr.empty())
		tokens.push_back(_T(""));

	while (beg_index != NSSTRING::npos)
    {
        end_index = inputStr.find_first_of(delims,beg_index);
        if (end_index == NSSTRING::npos) end_index = inputStr.length();

        tokens.push_back( inputStr.substr(beg_index,end_index-beg_index) );

        beg_index = inputStr.find_first_not_of(delims,end_index);
    }
}
