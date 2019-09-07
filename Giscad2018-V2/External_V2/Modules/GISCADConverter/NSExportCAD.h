/*!
 @Class		:	CNSExportCAD
 @Brief		:	This class provides the functionality to read ExportCAD
				information from XML.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"

#include "NSCadData.h"
#include "NSExportHeader.h"
#include "NSExportedFeatureContainer.h"
#include "NSTElement.h"

class CNSExportCAD:public CNSCadData
{
public:
	CNSExportCAD(void);
	int readXML();
	int writeDWG();
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setObject(CNSTElement* pElement);
	int getFeatureCount() const;
	void setFeatureCount(int nFeatureCount);
	NSSTRING getReportName() const;
public:
	~CNSExportCAD(void);
private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSExportHeader m_exportHeader;
	CNSExportedFeatureContainer m_exportFeatCont;
	int m_nFeatureCount;
	CNSTElement* m_pElement;
	NSSTRING m_strReportName;
};
