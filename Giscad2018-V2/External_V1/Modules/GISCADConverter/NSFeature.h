/*!
 @Class		:	CNSFeature
 @Brief		:	This class provides the functionality to read Feature information
				from XML and write the same information into DWG.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSBlock.h"

#include "NSComponentContainer.h"
#include "NSTElement.h"
#include "NSAttribute.h"
#include "NSAttributesContainer.h"
#include "NSRelationship.h"

typedef std::map<NSSTRING, NSSTRING> ATTRIBUTE_MAP;
enum FeatureCategory{CAT_INVALID = -1,CAT_BLOCK = 0, CAT_SEGMENT = 1, CAT_LABEL = 2};

class CNSFeature
{
public:
	

	CNSFeature(void);
	int readXML(CNSTElement &obj);
	int readDWG();
	int writeXML();
	int writeDWG();
	void setFID(const int nFID);
	void setFNO(const int nFNO);
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setObject(CNSCompositeContainer* pCompositeContainer);
	void setElement(CNSTElement* pElement);
	void setFeatureType(NSSTRING strFeatType);
	NSSTRING getFeatureType() const;
	void setXData(STRING_MAP& mapOfXData);
	int setAttributes(std::map<NSSTRING, NSSTRING> &mapofBlkAtt);
	int setAttributes(ATT_MAP &mapOfBlkAtt);
	void setXData(ATT_MAP &mapOfAttrRefs);
	int getValue(STRING_MAP& mapOfXData, NSSTRING strType, NSSTRING &strValue);
	void setCategory(const FeatureCategory enCategory);
	FeatureCategory getCategory() const;
	int getFNO() const;
	int getFID() const;
	std::wstring getUsage();
	bool isNestedFeature()
	{ return m_bNestedFeature;}
	void setNestedFeature(bool bNested) {m_bNestedFeature = bNested; }
	bool getInsertionPoint(CNSPoint3D &insertionPt);
	bool getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2);	
	bool setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts);

	CNSRelationShip getRelationship();
	NSSTRING getQueryString(NSSTRING strQueryString);
	NSSTRING getQueryString(std::map<int, CNSAttribute*> & mapAttributes, TCHAR szQuery[_MAX_PATH]);
	NSSTRING getQueryString(ATTRIBUTE_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH]);

public:
	~CNSFeature(void);
private:
	//int getBlockAttributes(std::map<NSSTRING, NSSTRING> & blockAttribs, ATT_MAP &mapAttrRef, std::map<int, NSSTRING> attMap);
	int getBlockAttributes(ATT_MAP &mapAttrRef, std::map<int, CNSAttribute*> &attMap);
	void getXData(STRING_MAP& mapOfXData);
	
	int getValue(ATT_MAP& mapOfAttrRefs, NSSTRING strType, NSSTRING &strValue);
	
	void setBlockAttributes(ATT_MAP &mapAttrRef);
	int assignRepresentation(CNSComponent *pComponent);
	//int createLinearAttributes(NSSTRING strLayerName, int nFNO);
	int createAttributes(NSSTRING strFeatureName, int nFNO);

	int getFinalBlockName(NSSTRING& strBlkName,std::map<int, CNSAttribute*>& mapAttributes,int nFNO,NSSTRING& strUsage,NSSTRING& strOri);
	bool readCriteria(NSSTRING strCriteria);

private:
	CNSComponentContainer m_compContainer;
	CNSAttributesContainer m_attributeContainer;
	CNSRelationShip m_relationship;
	int m_nFID;
	int m_nFNO;
	NSSTRING m_strUsage;
	NSSTRING m_strOrientation;
	NSSTRING m_strState;
    NSSTRING m_strTextLabel;
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSCompositeContainer *m_pContainer;
	CNSTElement *m_pElement;
	NSSTRING m_strFeatureType;
	FeatureCategory m_enCategory;
	ATTRIBUTE_MAP m_mapAttribute;
	bool m_bNestedFeature;

};
