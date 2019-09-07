/*!
 @Class			CNSCurveMember
 @Brief			This class provides the functionality to read Curve member 
				information from XML or write into DWG.
 @Author		Neilsoft Ltd
 @Date			06-07-2007
 @History 		Change history
 */
#pragma once
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSCompositeGeometry.h"
#include "NSTElement.h"

class CNSCurveMember
{
public:
	CNSCurveMember(void);
	NSSTRING getPositionList()const;
	int readXML();
	int writeXML();
	void setPositionList(const NSSTRING strPosList);
	void setObject(CNSTElement *pElement);
	void setType(const GEO_ELEMENT enType);
	GEO_ELEMENT getType() const;
	NSSTRING getLineString();
public:
	~CNSCurveMember(void);
private:
	NSSTRING m_strPosList;
	CNSTElement* m_pElement;
	GEO_ELEMENT m_enType;
};
