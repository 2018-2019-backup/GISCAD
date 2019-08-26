/*!
 @Class			CNSAttributesContainer
 @Brief			This class provides functionality to hold
				attributes information for one feature.
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "vector"
#include "map"

#include "NSTElement.h"
#include "NSAttribute.h"

typedef std::vector<CNSAttribute*> ATTRIBVECTOR;
typedef std::map<NSSTRING, NSSTRING> STRING_MAP;

class CNSAttributesContainer
{
public:
	CNSAttributesContainer(void);
	void getAttributes( std::map<int, CNSAttribute*> & mapOfAttributes);

public:
	~CNSAttributesContainer(void);
	int getSize()const;
	int readXML(CNSTElement &element);
	int writeXML();
	void setElement(CNSTElement *pElement);
	void addAttribute(int nCID, int nANO, NSSTRING strName, NSSTRING strValue);
	void getXData(STRING_MAP& mapOfXData);

private:
	std::map<int, CNSAttribute*> m_mapOfAttributes;
	CNSTElement* m_pElement;

};
	