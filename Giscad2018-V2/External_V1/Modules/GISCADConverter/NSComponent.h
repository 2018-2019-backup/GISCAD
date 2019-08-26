/*!
 @Class			CNSComponent
 @Brief			This class provides functionality to hold
				Component information.
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "NSGeometry.h"
#include "NSTElement.h"
#include "NSAttribute.h"

#include "..\..\Include\NSBlock.h"
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSenComponentTypes.h"
#include "..\..\Include\NSDatabaseMgr.h"

class CNSComponent
{
public:
	CNSComponent(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	int getComponentNumber() const;
	int getComponentID() const;
	int getValue(STRING_MAP& mapOfXData, NSSTRING strType, NSSTRING &strValue);
	void setComponentNumber(const int nCompNo);
	void setComponentID(const int nCompID);
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setElement(CNSTElement* pBlk);
	void setObject(CNSCompositeGeometry *pCompGeometry);
	void getXData(STRING_MAP& mapOfXData);	
	void setXData(STRING_MAP& mapOfXData);
	void setComponentType(const GEO_ELEMENT enType);
	void setCompIdentity(CNSCompositeGeometry *pCompGeometry);
	void setGeoType(NSSTRING strType);
	void createGeometry();
	void setCompRepresentation(const short iCompRepresentation);
	void setFNO(int FNO);

	short getCompRepresentation() const;

	GEO_ELEMENT getComponentType() const;
	COMPONENT_TYPE getCompType() const;
	CNSGeometry* getGeometry();
	
public:
	~CNSComponent(void);
private:
	int m_nCompNo;
	int m_ncompID;
	CNSGeometry* m_pGeometry;
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSTElement *m_pElement;
	CNSCompositeGeometry* m_pCompGeometry;
	GEO_ELEMENT m_enCompType;
	COMPONENT_TYPE m_enComponentType;
	NSSTRING m_strGeoType;
	short m_iCompRepresentation; 

public:
	int m_FNOReference;

private:
	CNSGeometry* createGeometry(NSSTRING strGeoType);
	CNSGeometry* createGeometry(GEO_ELEMENT enType);
};
