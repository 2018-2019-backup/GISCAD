/*!
 @Class		:	CNSLayerBlockValidator
 @Brief		:	This class provides the functionality validating feaures against layers and reticulation
 @Author	:	Neilsoft Ltd
 @Date		:	05-08-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"

struct IDCollector{std::vector<LONG_PTR> nObjectIds;};
typedef std::map<NSSTRING, IDCollector*> STRING_ID_TYPE;
typedef std::map<LONG_PTR, LONG_PTR> ID_TYPE;

enum RET_TYPE 
{
	RET_INVALID = -1,
	RET_EXT = 1,
	RET_MOD = 2,
	RET_DEL = 3,
	RET_NEW = 4,
	RET_REP = 5
};

class CNSLayerBlockValidator
{
public:
	CNSLayerBlockValidator(void);
public:
	~CNSLayerBlockValidator(void);
public:
	void setStdDWGObject(CNSDWGMgr* pStdDWGMgr);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	void setObject(CNSDWGMgr* pLocalDWGMgr);
	void setSearchAreaFactor(double dFactor);

	int validate(int &NewFeatCnt,int &ModFeatCnt,int &DelFeatCnt);
private:
	int getValue(STRING_MAP* mapOfXData, NSSTRING strType, NSSTRING &strValue);
	int validateReticulationBlks(RET_TYPE	enType, int &FeaturesCount);
	int validateBlkLayers(NSSTRING strLayerName, IDCollector *pIDCollector, VALIDATE_COL_MAP_TYPE &mapOfBlocks, RET_TYPE enType);
	//int getLayerName(CNSValidationDataCollector* pCollector , NSSTRING &strExtLayer, NSSTRING &strDelLayer, NSSTRING &strDelBlk);
	int getLayerName(CNSValidationDataCollector* pCollector , NSSTRING &strLayerName, RET_TYPE enLayerType, RET_TYPE enBlockType);
//	int checkRelation(CNSValidationDataCollector* pCollector , IDCollector &nonExtCollector, VALIDATE_COL_MAP_TYPE &mapOfBlocks, NSSTRING strBlkName, long &nObjID, RET_TYPE enLayerType);
	int checkRelation(std::vector<CNSValidationDataCollector*> &vectExtCollector , IDCollector &nonExtCollector, VALIDATE_COL_MAP_TYPE &mapOfBlocks, LONG_PTR &nObjID, RET_TYPE enLayerType);
	int getBlkNameFromType(NSSTRING strExtName, RET_TYPE enType, NSSTRING &strNewName);
	int getLayerNameFromLineType(CNSValidationDataCollector* pCollector , NSSTRING &strLayerName, RET_TYPE enLayerType, RET_TYPE enBlockType);
	int validateFIDforEXTBlock(CNSValidationDataCollector *pCollector, NSSTRING strFeatureType);

	NSSTRING getExtFromType(RET_TYPE enType);
	int checkReplacedWithNew(CNSValidationDataCollector* pCollector,  VALIDATE_COL_MAP_TYPE &mapOfBlocks);
	int checkBlkLayer(CNSValidationDataCollector *pCollector, NSSTRING &strLayerName, RET_TYPE enType);
	bool CheckForAdditionalAllowedLayers(CNSValidationDataCollector *pCollector, RET_TYPE enType,NSSTRING &strLayerName);
	int checkNonBlkLayer(CNSValidationDataCollector *pCollector, NSSTRING &strLayerName, RET_TYPE enType);
	int validateExisitingFeatures();
	int validateNewFeatures(int &NewFeaturesCount);
	void clearCollection(VALIDATE_COL_MAP_TYPE &mapOfCollection);
	RET_TYPE getTypeFromBlkName(NSSTRING strExtName);
	int isPointOverlapped(CNSPoint3D firstPt, CNSPoint3D secondPt);
	NSSTRING getQueryString(NSSTRING strQueryString);
	int validateArrtibutes(CNSValidationDataCollector *pCollector);
	int compareAttributes(NSSTRING strActValue, NSSTRING strAllowedValue);
	int tokenize(NSSTRING strInputString, STRING_MAP &mapOfValues);
	NSSTRING getQueryString(STRING_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH]);
	int checkGISPrompt(CNSValidationDataCollector *pCollector);

	bool isExtPole(RET_TYPE enType , CNSValidationDataCollector *pCollector  );

private:
	CNSDWGMgr *m_pStdDWGMgr;
	CNSDWGMgr *m_pLocalDWGMgr;
	double m_dAreaFactor;
	ID_TYPE m_mapOfPairs;
	STRING_ID_TYPE m_mapOfNewFeautres;
	VALIDATE_COL_MAP_TYPE m_OfDataCollector;
};
