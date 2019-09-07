/*!
 @Class		:	CNSImportHeader
 @Brief		:	This class is responsible for Reading title block information from
				Drawing and writing into XML.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "NSHeader.h"
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"
#include "NSTElement.h"


class CNSImportHeader:public CNSHeader
{
public:
	CNSImportHeader(void);
	int readDWG();
	int writeXML();
	void setObject(CNSDWGMgr& dwgMgr);
	void setObject(CNSDatabaseMgr& dbMgr);
	void setElement(CNSTElement* pElement);
	NSSTRING getDrawingNumber() const;
	NSSTRING getProjectNumber() const;
	NSSTRING getExportID() const;
	NSSTRING parseDateTime(int nValue);
	NSSTRING getDrawingNumFromMap();
public:
	~CNSImportHeader(void);

private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	std::map<NSSTRING, NSSTRING> m_mapOfTitleAttributes;	
	CNSTElement* m_pElement;
	NSSTRING m_strDrawingNumber;
	NSSTRING m_strProjectNumber;
	NSSTRING m_strExportID;
	AcDbDatabase* m_CurrentDatabase;
};
