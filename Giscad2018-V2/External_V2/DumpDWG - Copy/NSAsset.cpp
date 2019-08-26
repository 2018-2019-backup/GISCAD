//#include "stdafx.h"
//#include "NSDatabaseMgr.h"
#include "..\Include\nsasset.h"
#include "dbapserv.h"
#include "dbmain.h"
#include "dbsymtb.h"
#include "dbents.h"
#include "tchar.h"

CNSAsset::CNSAsset(void)
{
	NSSTRING m_strAssetNum = _T("");
	NSSTRING m_strLayerName = _T("");
	NSSTRING m_strBlockName = _T("");
	NSSTRING m_strDepot = _T("");
	NSSTRING m_strEquipClass = _T("");
	m_nObjectID = 0;
}

CNSAsset::~CNSAsset(void)
{
}

void CNSAsset::setAssetNumber(NSSTRING strAssetNum)
{
	m_strAssetNum = strAssetNum;
}

NSSTRING CNSAsset::getAssetNumber() const
{
	return m_strAssetNum;
}

void CNSAsset::setObjectID(const long nObjID)
{
	m_nObjectID = nObjID;
}

long CNSAsset::getObjectID() const
{
	return m_nObjectID;
}

void CNSAsset::setBlockName(NSSTRING strBlkName)
{
	m_strBlockName = strBlkName;
}

NSSTRING CNSAsset::getBlockName()
{
	return m_strBlockName;
}

void CNSAsset::setLayerName(NSSTRING strLayerName)
{
	m_strLayerName = strLayerName;
}

NSSTRING CNSAsset::getLayerName()
{
	return m_strLayerName;
}

//bool CNSAsset::validateAsset()
//{
	/*std::map<NSSTRING, NSSTRING> mapofBlkAttbs;
	
	NSSTRING strRequiredAttributes[5] = {_T("StreetName"), _T("SubUrb"), _T("EquipClass"), _T("PlantCode0"), 
		_T("EquipClassif18")};
	readBlockAttributes(mapofBlkAttbs);
	for(int nCntr = 0; nCntr < 5; nCntr++)
	{
		if(NS_SUCCESS != checkAttribute(strRequiredAttributes[nCntr], mapofBlkAttbs))
		{
			return false;
		}
	}
	return true;*/

	// For temporary build
	/*std::map<NSSTRING, NSSTRING> mapofBlkAttbs;
	
	NSSTRING strRequiredAttributes[2] = {_T("StreetName"), _T("SubUrb")};
	readBlockAttributes(mapofBlkAttbs);
	for(int nCntr = 0; nCntr < 2; nCntr++)
	{
		if(NS_SUCCESS != checkAttribute(strRequiredAttributes[nCntr], mapofBlkAttbs))
		{
			return false;
		}
	}
	return true;
}*/

int CNSAsset::checkAttribute(NSSTRING strAttribute/*, std::map<NSSTRING, NSSTRING> & mapOfAttributes*/)
{
	std::map<NSSTRING, NSSTRING>::iterator itr;
	//itr = m_mapOfAttributes.find(NSSTRUPR((TCHAR*)strAttribute.c_str()));
    #ifndef _INTERNAL_
        NSSTRUPR((TCHAR*)strAttribute.c_str(), strAttribute.size()+1);
        itr = m_mapOfAttributes.find((TCHAR*)strAttribute.c_str());
	#else
	    NSSTRUPR((TCHAR*)strAttribute.c_str(), strAttribute.size()+1);
        itr = m_mapOfAttributes.find((TCHAR*)strAttribute.c_str());
		//itr = m_mapOfAttributes.find(NSSTRUPR((TCHAR*)strAttribute.c_str()));
	#endif

	if(itr != m_mapOfAttributes.end())
	{
		NSSTRING strValue = (*itr).second;
		if(NSSTRCMP(strValue.c_str(), _T("")) == 0)
		{
	//		NSPRINT(_T("Mandatory Information not available for asset"));
			return -102;
		}
	}
	else
	{
		//NSPRINT(_T("Mandatory Information not available for asset"));
		return -101;
	}
	return NS_SUCCESS;
}
/*
int CNSAsset::readBlockAttributes(std::map<NSSTRING, NSSTRING> & mapOfAttributes)
{
	Acad::ErrorStatus es;
	AcDbObject *pEntity;
	AcDbObjectId pBlockObjID;
	pBlockObjID.setFromOldId(m_nObjectID);
	es = acdbOpenObject(pEntity, pBlockObjID, AcDb::kForRead);
	if(!pEntity->isKindOf(AcDbBlockReference::desc()))
	{
		pEntity->close();
		return NS_FAIL;
	}
	AcDbBlockReference *pRef = (AcDbBlockReference*)pEntity;
	AcDbObjectIterator *pObjItr = pRef->attributeIterator();
	for(pObjItr->start(); !pObjItr->done(); pObjItr->step())
	{
		AcDbObjectId attID = pObjItr->objectId();
		AcDbAttribute *pAttribute;
		Acad::ErrorStatus es = pRef->openAttribute(pAttribute, attID, AcDb::kForRead);
		if(es == Acad::eOk)
		{
			const TCHAR* szAttName = pAttribute->tag();
			const TCHAR* szValue = pAttribute->textString();	
			mapOfAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, szValue));
		}
		if(pAttribute != NULL)
			pAttribute->close();
	}
	pRef->close();
	pEntity->close();
	delete pObjItr;
	return NS_SUCCESS;
}
*/
NSSTRING CNSAsset::generateAccountCode()
{
	return _T("");
}
/*
NSSTRING CNSAsset::getAccountCode(NSSTRING strDepot, NSSTRING strEquipClass)
{
	CNSDatabaseMgr* pDatabase = CNSDatabaseMgr::getInstance();
	int iRet = pDatabase->openDatabase("", "", "AccountCode");
	if(iRet == NS_FAIL)
		return _T("");
	TCHAR szQuery[_MAX_PATH + 1];
	NSSTRING strActivity, strOrgUnit;
	sprintf(szQuery, _T("select Activity from Activity where EquipClass = \'%s\'"), strEquipClass.c_str());
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vActivity;
		bool bRet = pDatabase->getValue("Activity", vActivity);
		if(bRet != false)
		{
			strActivity = (NSSTRING)((_bstr_t)(vActivity.bstrVal));
		}
	}

	sprintf(szQuery, _T("select OrgUnit from OrgUnit where Depot = \'%s\'"), strDepot.c_str());
	if(-1 != pDatabase->executeQuery(szQuery))
	{
		variant_t vOrgUnit;
		bool bRet = pDatabase->getValue("OrgUnit", vOrgUnit);
		if(bRet != false)
		{
			strOrgUnit = (NSSTRING)((_bstr_t)(vOrgUnit.bstrVal));
		}
	}

	pDatabase->closeDatabase();
	return strOrgUnit.append(_T("0")).append(strActivity);
}
*/
NSSTRING CNSAsset::getEquipNumberD1()
{
	NSSTRING strSubUrb = _T("");
	NSSTRING strStreetName = _T("");
	std::map<NSSTRING, NSSTRING>::iterator it;
	it = m_mapOfAttributes.find(_T("SUBURB"));
	if(it != m_mapOfAttributes.end())
	{
		strSubUrb = (*it).second;
	}
	it = m_mapOfAttributes.find(_T("STREETNAME"));
	if(it != m_mapOfAttributes.end())
	{
		strStreetName = (*it).second;
	}
	NSSTRING strStreet = strStreetName.append(_T("-"));
	return strStreet.append(strSubUrb);
}

int CNSAsset::getFNO() const
{
	return m_nFNO;
}

void CNSAsset::setFNO(const int nFNO)
{
	m_nFNO = nFNO;
}

int CNSAsset::getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue)
{
	std::map<NSSTRING, NSSTRING>::iterator itrMap;
	itrMap = m_mapOfAttributes.find(pszKey);
	if(itrMap != m_mapOfAttributes.end())
	{ 
		pszValue = (*itrMap).second;
		return NS_SUCCESS;
	}
	return NS_FAIL;
}