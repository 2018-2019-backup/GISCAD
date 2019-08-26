/*!
 @Class		:	CNSImportCAD
 @Brief		:	This class is responsible for reading CAD information from DWG
				and write into XML.
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDirectives.h"

#include "NSImportHeader.h"
#include "NSCadData.h"
#include "NSTElement.h"
#include "NSImportedFeatureContainer.h"

class CNSImportCAD:public CNSCadData
{
public:
	CNSImportCAD(void);
	int readDWG();
	int writeXML();
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setElement(CNSTElement* pElement);
	int getModFeatures() const ;
	int getNewFeatures() const ;
	int getDelFeatures() const ;
	NSSTRING getReportLocation() const;
public:
	~CNSImportCAD(void);
private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSImportHeader m_importHeader;
	CNSImportedFeatureContainer m_importFeatContainer;
	CNSTElement* m_pElement;
	int m_nModFeatures;
	int m_nNewFeatures;
	int m_nDelFeatures;
	NSSTRING m_strReportLocation;
};
