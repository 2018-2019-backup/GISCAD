/*!
 @Class		:	CNSExportHeader
 @Brief		:	This class provides the functionality to read Header information
				from XML and write the information into title block.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "vector"
#include "map"

#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"

#include "NSHeader.h"
#include "NSTElement.h"
#include "NSPolygon.h"

typedef std::map<NSSTRING, NSSTRING> HEADER_MAP;

class CNSExportHeader:public CNSHeader
{
public:
	CNSExportHeader(void);
	int readXML(CNSTElement &element);
	int writeDWG();
	void setObject(CNSDWGMgr& dwgMgr);
	void setObject(CNSDatabaseMgr& dbMgr);
	NSSTRING getDrawingNumber() const;
	NSSTRING getProjectNumber() const;
	NSSTRING getExportID() const;
public:
	~CNSExportHeader(void);
private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	NSSTRING m_strExportID;
	NSSTRING m_strProjectType;
	NSSTRING m_strProjectNumber;
	NSSTRING m_strDrawingNumber;
	NSSTRING m_strProjectDes;
	NSSTRING m_strExportDate;
	NSSTRING m_strExportUserName;
	NSSTRING m_strDesignerCompanyName;
	NSSTRING m_strComments;
	NSSTRING m_strExportCategores;
	CNSPolygon* m_pPolygon;
};
