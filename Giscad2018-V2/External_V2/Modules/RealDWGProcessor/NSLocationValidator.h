#pragma once
#include "..\Include\NSDWGMgr.h"
enum LocationType
{
	LOC_INVALID = -1,
	LOC_POINT = 1,
	LOC_OTHER = 2
};

class CNSLocationValidator
{
public:
	CNSLocationValidator(void);
public:
	~CNSLocationValidator(void);
public:
	//void setObject(CNSFeatureContainer* pFeatureContainer);
	void setObject(CNSValidationDataCollector* pCollector);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	void setType(LocationType enLocType);
	int validate(void);
private:
	int getValue(NSSTRING strType, NSSTRING &strValue);
	int validatePolylines();
	CNSPoint3D getPoint(NSSTRING strPt);
	int isPointOverlapped(CNSPoint3D firstPt, CNSPoint3D secondPt);
    NSSTRING getAttributeValue(STRING_MAP &mapOfAttr, NSSTRING strAttr,  bool &bAssetNumPresent);
    
private:
	//CNSFeatureContainer* m_pFeatureContainer;
	CNSValidationDataCollector *m_pCollector;
	CNSDWGMgr *m_pLocalDWGMgr;
	LocationType m_enLocType;
	std::map<NSSTRING, int> m_mapOfCommonErrors[2];
};
