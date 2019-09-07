/*!
 @Class			CNSAttribute
 @Brief			This class provides functionality to hold
				data related to Attribute.
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "string"
#include <map>

#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSBlock.h"

#include "NSTElement.h"
typedef std::vector<NSSTRING> m_vectTokStr;


class CNSAttribute
{
public:
	CNSAttribute(void);
	int getAttributeNumber() const;
	void setAttributeNumber(const int nAno);
	NSSTRING getAttributeName() const;
	void setAttributeName(const NSSTRING strAName);
	void setDataValue(int nCID, NSSTRING strValue);
	void getDataValue(int nCID, NSSTRING &strValue);
	NSSTRING getXData();
	void setElement(CNSTElement* pElement);
	int writeXML();
	int readXML(CNSTElement &pElement);
	int getDataSize() const;
	int getData(CNSAttributeRefMap  *pAttrRefMap, NSSTRING strTagName);
	void tokenizeString(const NSSTRING& inputStr, const NSSTRING& delims, m_vectTokStr& tokens);

public:
	~CNSAttribute(void);
private:
	int m_nANO;
	NSSTRING m_strName;
	std::map<int, NSSTRING> m_DataMap; 
	CNSTElement* m_pElement;
};
