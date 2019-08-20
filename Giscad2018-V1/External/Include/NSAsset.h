#pragma once

#include "map"
#include "vector"
#include "..\Include\NSDirectives.h"
class CNSAsset//__declspec(dllexport)
{
public:
	CNSAsset(void);
	virtual ~CNSAsset(void);
public:
	void setAssetNumber(NSSTRING strAssetNum);
	NSSTRING getAssetNumber() const;
	void setObjectID(const LONG_PTR nObjID);
	LONG_PTR getObjectID() const;
	void setBlockName(NSSTRING strBlkName);
	NSSTRING getBlockName();
	void setLayerName(NSSTRING strLayerName);
	NSSTRING getLayerName();
//	bool validateAsset();
	NSSTRING generateAccountCode();
	//NSSTRING getAccountCode(NSSTRING strDepot, NSSTRING strEquipClass);
	NSSTRING getEquipNumberD1();
	int getFNO() const;
	int getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue);
	void setFNO(const int nFNO);
	int checkAttribute(NSSTRING strAttribute/*, std::map<NSSTRING, NSSTRING> & mapOfAttributes*/);

private:
	//int readBlockAttributes(std::map<NSSTRING, NSSTRING> & mapOfAttributes);
private:
	LONG_PTR m_nObjectID;
	int m_nFNO;
	NSSTRING m_strAssetNum;
	NSSTRING m_strLayerName;
	NSSTRING m_strBlockName;
	NSSTRING m_strDepot;
	NSSTRING m_strEquipClass;
public:
	std::map<NSSTRING,NSSTRING> m_mapOfAttributes;
	
};
