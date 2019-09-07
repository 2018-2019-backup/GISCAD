#pragma once
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSCredentials.h"
#ifndef interface
#define interface class
#endif

class CNSDWGMgr;

class __declspec(dllexport) IAssetMgr
{
public:
    virtual int scan()=0;
	virtual int generateNumbers()= 0;
	virtual int scanTitleBlock(CNSTitleBlkInfo &titleBlk)=0;
	virtual int getNumOfAssets(enum ASSET_TYPE)=0;
	virtual void setUserCredentials(CNSUserCredentials userCredentials)=0;
//	virtual void setDWGFilePath(TCHAR* strDWGFileName) = 0;
//	virtual void setStdDWGFilePath(TCHAR* strStdDWGFileName) = 0;
//	virtual void setReportFilePath(TCHAR* strReportPath) = 0;
//	virtual void getReportFilePath(NSSTRING &temp) = 0;
//	virtual void setRelationshipFlag(int nRelFlag) = 0;
//	virtual void setSearchAreaFactor(double dSearchAreaFactor) = 0;
//	virtual void setPrecision(int nPrecision) = 0;
//	virtual void setGraphicIndication(int nGraphicIndication) = 0;
	virtual void setDatabase(CNSDWGMgr* pDWGMgr) = 0;
//	virtual void setMDBLocation(TCHAR* szMDBLocation) = 0;
	virtual int validate() = 0;
	IAssetMgr() {}
	virtual ~IAssetMgr() {;}
	virtual bool IsValidDepotCode(NSSTRING szDepotCode)=0;
	virtual bool GetErrorsOccuredFlag()=0;
};

//exposed creator
__declspec(dllexport) IAssetMgr* createAssetMgr();