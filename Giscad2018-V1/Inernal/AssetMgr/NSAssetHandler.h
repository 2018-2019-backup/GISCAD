#pragma once
#include "tchar.h"
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSAssetMgrIFace.h"
#include "..\..\Include\NSReportWriter.h"
#include "..\..\Include\NSEllipseConnector.h"
#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)
typedef std::vector<CNSAsset*> ASSET_VECTOR;
typedef std::vector<LONG_PTR> OBJECTID_VECTOR ;
typedef std::map<NSSTRING, OBJECTID_VECTOR*> SWITCH_MAP;

class CNSAssetHandler:public IAssetMgr
{
public:
	CNSAssetHandler(void);
	~CNSAssetHandler(void);
    int scan();	
	int scanTitleBlock(CNSTitleBlkInfo &titleBlk);
    void setDatabase(CNSDWGMgr* pDWGMgr);
	int getNumOfAssets(enum ASSET_TYPE);
	void setUserCredentials(CNSUserCredentials userCredentials);
	int generateNumbers();
	int validate();
	bool IsValidDepotCode(NSSTRING szDepotCode);
	bool GetErrorsOccuredFlag();
	void SetErrorsOccuredFlag(bool bFlag);
private:
	int getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue, std::map<NSSTRING, NSSTRING> &mapofAttributes);
	int filterAssets(ASSET_VECTOR &vectOfAssets);
	void setReplyParameters(std::vector<TCHAR*> & replyVect);
	void setRequestParameters(std::map<TCHAR*, TCHAR*> & mapOfRequest, NSSTRING strAssetType, CNSAsset* pAsset);
	void setLinkToProject(std::map<TCHAR*, TCHAR*> & mapofCredentials, TCHAR* szEquipNumber);
	int setProjectLink(NSSTRING strService, std::vector<TCHAR*> & replyVect, std::map<TCHAR*, TCHAR*> & mapOfRequest, std::map<TCHAR*, TCHAR*> & mapofCredentials, TCHAR* szEquipNumber, bool bFlag);
	int validateAsset(CNSAsset* pAsset, NSSTRING strAssetType, bool bDisplayErrorMsg = true);
	NSSTRING getAccountCode(NSSTRING strDepot, NSSTRING strEquipClass);
	int prepareSOAPMessage(std::map<TCHAR*, TCHAR*> & mapofCredentials, std::vector<TCHAR*> & replyVect, std::map<TCHAR*, TCHAR*> & mapOfRequest);
	int scanSubStationAssets(std::map<LONG_PTR, SWITCH_MAP*> & mapofSwitches);
	int scanServicePillars(std::map<LONG_PTR, ASSET_VECTOR*> & mapOfPillars);
	TCHAR* getValueFromMap(std::map<NSSTRING, NSSTRING> & mapOfAttributes, TCHAR* szKey);
	void generateAssetsForPillars(std::map<TCHAR*, TCHAR*> & mapofCredentials);
	NSSTRING getPlantNo(TCHAR* szPlantNo);
	void generateAssetsForSwitches(std::map<TCHAR*, TCHAR*> & mapofCredentials);
	NSSTRING getEquipGrpID(TCHAR* szSwitchType);
	NSSTRING getQueryString(NSSTRING strQueryString);
	NSSTRING getQueryString(CNSAsset* pAsset, TCHAR szQuery[_MAX_PATH]);
	int assignOPIds(std::map<TCHAR*, TCHAR*> &mapofCredentials, STRING_MAP &mapOfValues, bool bGroupFlag = false );
	int tokenize(NSSTRING strInputString, NSSTRING strEquipNo, STRING_MAP &mapOfValues, bool bGroupFlag);
	int	setAssetNumbers(STRING_MAP &mapOfValues);
	int generateAssetForSubSwitches(std::map<TCHAR*, TCHAR*> &mapofCredentials);
	int generateAssetForServicePillars(std::map<TCHAR*, TCHAR*> &mapofCredentials);
	CNSAsset* getAsset(LONG_PTR nObjectID);
	int processErrMessages(std::vector<TCHAR*> &vectMsgs);
	void leftTrim(NSSTRING &src, TCHAR ch);


private:
	CNSDWGMgr m_LocalDWGObj;
	CNSUserCredentials m_UserCredentials;
	std::map<NSSTRING, ASSET_VECTOR*> m_mapOfAssets;
	CNSEllipseConnector m_ellipseConnector;
	CNSTitleBlkInfo m_titleBlk;
	bool m_ErrorsOccured;
	std::map<LONG_PTR, NSSTRING> mapOfNumAssets;
	std::map<LONG_PTR, NSSTRING> m_mapOfEquipNoIDs;
//	std::map<NSSTRING, NSSTRING> m_mapOfEquipNoAssets;
	std::map<LONG_PTR, SWITCH_MAP*> mapofSwitches;
	std::map<LONG_PTR, ASSET_VECTOR*> mapOfPillars;
	std::map<LONG_PTR, NSSTRING> m_mapOfSwitchType;
};
