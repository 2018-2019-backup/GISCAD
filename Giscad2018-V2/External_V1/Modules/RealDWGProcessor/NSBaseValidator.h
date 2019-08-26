/*!
 @Class			CNSBaseValidator 
 @Brief			This class calls the different function for validation of dwg files
 @Author		Neilsoft Ltd
 @Date			06-08-2007
 @History 		Change history
*/
#pragma once
#include "stdafx.h"
#include "tchar.h"
#include <string>
#include "..\Include\NSValidatorIntface.h"
#include "..\Include\NSAssetMgrIFace.h"
#include "..\Include\NSDirectives.h"
#include "..\Include\NSDWGMgr.h"


//extern CNSDWGMgr cnMgr;

class CNSBaseValidator:public IValidator//__declspec(dllexport) 
{
public:
	CNSBaseValidator(void);
public:
	//virtual ~CNSBaseValidator(void);
	virtual ~CNSBaseValidator(void);
public:
	//int setUserCredentials();
	/*void setDWGFilePath(NSSTRING strDWGFileName);
	void setStdDWGFilePath(NSSTRING strStdDWGFileName);
	void setReportFilePath(NSSTRING strReportPath);*/

	void setDWGFilePath(TCHAR* strDWGFileName);
	void setStdDWGFilePath(TCHAR* strStdDWGFileName);
	void setReportFilePath(TCHAR* strReportPath);
	void getReportFilePath(NSSTRING &temp);
 //NSSTRING getReportFilePath(NSSTRING &temp);

	void setRelationshipFlag(int nRelFlag);
	void setConnctivityFlag(int nGisFlag);
	void setSearchAreaFactor(double dSearchAreaFactor, double dSymbolFactor);
	void setPrecision(int nPrecision);
	void setGraphicIndication(int nGraphicIndication);
	void setDatabase(CNSDWGMgr* pDWGMgr);
	int validate(void);
	//int validateTextStyle();
	void setMDBLocation(TCHAR* szMDBLocation, bool bStandardMDB = false);
	void setUserCredentials(CNSUserCredentials userCredentials);
	void setAssetValidationFlag(bool bFlag);
	void setCadValidationFlag(bool bFlag);
	void setGisValidationFlag(bool bFlag);
	void setSearchAreaRadius(double dSearchAreaRadius);
	void setTolerance(double dTolerance);
	void setLinearConnOnLayerFlag(int iFlag);
	void setTemplateVersion(double Version);
	void setValidationRulesErrorFlag(int nFlagP1,int nFlagP2,int nFlagP3,int nFlagP4);
	int ReadDwgHeader();
private:
	int openDrawing(NSSTRING strFileName, CNSDWGMgr &dwgMgr,bool bMakeCurrentActive = false);
	void checkReportPath();
	NSSTRING getTokenizedPath(TCHAR*);

private:
	/*NSSTRING m_strDWGFilename;
	NSSTRING m_strStdDWGFilename;
	NSSTRING m_strReportPath;*/
	TCHAR *m_strDWGFileversion;
	TCHAR *m_strDWGFilename;
	TCHAR *m_strStdDWGFilename;
	TCHAR *m_strReportPath;
	TCHAR *m_szMDBLocation;
	TCHAR *m_szStdMDBLocation;
	NSSTRING m_strReportFileName;
	int m_nRelFlag;
	int m_nGisFlag;
	double m_dSearchAreaFactor;
	double m_dSymbolFactor;
	//CNSDWGMgr localDWGObj;
	CNSUserCredentials m_UserCredentials;
	bool m_bAssetValidation;
	bool m_bCadValidation;
	bool m_bGisValidation;
	double m_dSearchAreaRadius;
	double templateVersion;

public:
	CNSDWGMgr localDWGObj;
	int NewFeatCnt ;
	int ModFeatCnt ;
	int DelFeatCnt ;
	static int m_nP1ErrorFlag;
	static int m_nP2ErrorFlag;
    static int m_nP3ErrorFlag;
	static int m_nP4ErrorFlag;
	
	NSSTRING report;
};
