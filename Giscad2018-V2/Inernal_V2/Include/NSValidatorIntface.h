#pragma once
//#include <tchar.h>
//#include <iostream>
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSCredentials.h"
#ifndef interface
#define interface struct
#endif

class CNSDWGMgr;

class __declspec(dllexport) IValidator
{
public:
	IValidator(){};
	virtual int validate(){return 0;};
	virtual void setDWGFilePath(TCHAR* strDWGFileName) = 0;
	virtual void setStdDWGFilePath(TCHAR* strStdDWGFileName) = 0;
	virtual void setReportFilePath(TCHAR* strReportPath) = 0;
	virtual void getReportFilePath(NSSTRING &temp) = 0;
	virtual void setRelationshipFlag(int nRelFlag) = 0;
	virtual void setConnctivityFlag(int nGisFlag) = 0;
	virtual void setSearchAreaFactor(double dSearchAreaFactor, double dSymbolFactor) = 0;
	virtual void setPrecision(int nPrecision) = 0;
	virtual void setGraphicIndication(int nGraphicIndication) = 0;
	virtual void setDatabase(CNSDWGMgr* pDWGMgr) = 0;
	virtual void setMDBLocation(TCHAR* szMDBLocation, bool bStandardMDB = false) = 0;
	virtual void setAssetValidationFlag(bool bFlag) = 0;
	virtual void setCadValidationFlag(bool bFlag) = 0;
	virtual void setGisValidationFlag(bool bFlag) = 0;
	virtual void setUserCredentials(CNSUserCredentials userCredentials)=0;
	virtual void setSearchAreaRadius(double dSearchAreaRadius) = 0;
	virtual void setTolerance(double dTolerance) = 0;
	virtual void setLinearConnOnLayerFlag(int iFlag) = 0;
	virtual void setTemplateVersion(double Version)=0;
	virtual void setValidationRulesErrorFlag(int nFlagP1,int nFlagP2,int nFlagP3,int nFlagP4) = 0;
	/*virtual void setDWGFilePath(NSSTRING strDWGFileName) = 0;
	virtual void setStdDWGFilePath(NSSTRING strStdDWGFileName) = 0;
	virtual void setReportFilePath(NSSTRING strReportPath) = 0;*/

	virtual ~IValidator() {;}
};

//exposed creator
__declspec(dllexport) IValidator* createValidator();