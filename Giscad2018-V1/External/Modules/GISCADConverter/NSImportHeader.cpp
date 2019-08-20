#include "StdAfx.h"
#include "..\..\Include\NSDirectives.h"

#include "NSImportHeader.h"
#include "NSTElement.h"


#include "shlwapi.h"
#include "urlmon.h"
#include "Wininet.h"
#include "adesk.h"
#include "dbsymtb.h"
#include "dbents.h"
#include "dbelipse.h"
#include "dbspline.h"
#include "dblead.h"
#include "dbray.h"
#include "dbxline.h"
#include "dbmline.h"
#include "dbbody.h"
#include "dbregion.h"
#include "dbsol3d.h"
#include "acgi.h"
#include "acestext.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "rxregsvc.h"

#include "acdbxref.h"
#include "dbacis.h"

#include "windows.h"

#include "dbents.h"
#include "dbhatch.h"
#include "dbimage.h"
#include "imgdef.h"
#include "imgent.h"
#include "dbole.h"
#include "dbfcf.h"
#include "dbproxy.h"


#include "dbapserv.h"
#include "dbmstyle.h"
#include "gelnsg3d.h"
#include "tchar.h"

#include "StdAfx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "tchar.h"
//#include "dbsymtb.h"
#include "dbsymutl.h"
#include "dbid.h" 
#include "dbapserv.h"
#include "dbmain.h"
#include "acdbabb.h"
#include "dblayout.h"
#include "AcDbLMgr.h"
#include "dbents.h"
#include "dbdict.h"
#include "dbMPolygon.h"
#include "dbidmap.h"
#include "summinfo.h"
#include "dbxrecrd.h"
#include "sstream"
#include <atlstr.h>
#include "dbdynblk.h"

#include "dbregion.h"
#include "dblead.h"
#include "dbmleader.h"
#include "dbmleaderstyle.h"
#include "acgiviewport.h"
#include "dbtable.h"
#include "dbsol3d.h"

#include "dbmstyle.h"
//#include "acedads.h"

#include "acdb.h"

// Multiline change request start
#include "dbmline.h"

#include <dbobjptr.h>

#ifndef _INTERNAL_
#include "dbDictUtil.h"
#endif



// Constructor
CNSImportHeader::CNSImportHeader(void)
{
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pElement = NULL;
}

// Destructor
CNSImportHeader::~CNSImportHeader(void)
{
}


/*!
@Brief			This function reads Header information from drawing..
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/

int CNSImportHeader::readDWG()
{
	std::map<NSSTRING, NSSTRING> mapOfPromptValues;
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportID"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectType"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectNumber"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DrawingNumber"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectDescription"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportDate"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportUserName"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DesignerCompanyName"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("Comments"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportCategories"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportAreaOfInterest"),_T("")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("GIS_SUCCESS"),_T("FAIL")));
	m_mapOfTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CAD_SUCCESS"),_T("FAIL")));
	m_pDwgMgr->listTitleBlockHeaderAttributes(m_mapOfTitleAttributes, mapOfPromptValues);

	std::map<NSSTRING, NSSTRING>::iterator vStampItr;
	vStampItr = m_mapOfTitleAttributes.find(_T("GIS_SUCCESS"));
	NSSTRING strGIS = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("CAD_SUCCESS"));
	NSSTRING strCAD = (*vStampItr).second;
	

	vStampItr = m_mapOfTitleAttributes.find(_T("ProjectNumber"));
	NSSTRING strPN = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("DrawingNumber"));
	NSSTRING strDN = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("ProjectDescription"));
	NSSTRING strPD = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("ExportDate"));
	NSSTRING strED = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("ExportUserName"));
	NSSTRING strEUN = (*vStampItr).second;
	vStampItr = m_mapOfTitleAttributes.find(_T("DesignerCompanyName"));
	NSSTRING strDCN = (*vStampItr).second;

	if((NSSTRCMP(strGIS.c_str(), _T("FAIL")) == 0) && (NSSTRCMP(strCAD.c_str(), _T("FAIL")) == 0))
	{
		return -1001;
	}
	if((NSSTRCMP(strGIS.c_str(), _T("0")) == 0))
	{
		return -1002;
	}
	bool header=NSSTRLEN(strPN.c_str()) && NSSTRLEN(strDN.c_str()) && NSSTRLEN(strED.c_str());
	if(!header)
	{
		return -1004;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes Header information into xml.
@Return			NS_SUCCESS : If reading process successful.
@Date			12-06-2007
*/
int CNSImportHeader::writeXML()
{
	std::map<NSSTRING, NSSTRING>::iterator it;
	it = m_mapOfTitleAttributes.find(_T("ExportID"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ExportID"), (*it).second, DEFAULT_NAMESPACE);
		m_strExportID = (*it).second;
	}
	it = m_mapOfTitleAttributes.find(_T("ProjectType"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ProjectType"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("ProjectNumber"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ProjectNumber"), (*it).second, DEFAULT_NAMESPACE);
		m_strProjectNumber = (*it).second;
	}
	it = m_mapOfTitleAttributes.find(_T("DrawingNumber"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("DrawingNumber"), (*it).second, DEFAULT_NAMESPACE);
		m_strDrawingNumber = (*it).second;
	}
	it = m_mapOfTitleAttributes.find(_T("ProjectDescription"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ProjectDescription"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("ExportDate"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ExportDate"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("ExportUserName"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("ExportUserName"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("DesignerCompanyName"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("DesignerCompanyName"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("Comments"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		m_pElement->createNode(_T("Comments"), (*it).second, DEFAULT_NAMESPACE);
	}
	it = m_mapOfTitleAttributes.find(_T("ExportCategories"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		NSSTRING strPosList = (*it).second;
		TCHAR* szNextToken;
		TCHAR* szToken = wcstok_s ((TCHAR*)strPosList.c_str(), _T(","), &szNextToken);
		std::vector<TCHAR*> vCategories;
		while(szToken != NULL)
		{
			vCategories.push_back(szToken);
			szToken = wcstok_s (NULL, _T(","), &szNextToken);
		}
		CNSTElement oExportCat = m_pElement->createNode(_T("ExportCategories"), _T(""), DEFAULT_NAMESPACE);
		std::vector<TCHAR*>::iterator iterCate;
		for(iterCate = vCategories.begin(); iterCate != vCategories.end(); iterCate++)
		{
			oExportCat.createNode(_T("ExportCategory"), (*iterCate), DEFAULT_NAMESPACE);
		}

	}
	it = m_mapOfTitleAttributes.find(_T("ExportAreaOfInterest"));
	if(m_mapOfTitleAttributes.end() != it)
	{
		NSSTRING strPosList = (*it).second;
		CNSTElement element;
		element = m_pElement->createNode(_T("ExportAreaOfInterest"), _T(""), DEFAULT_NAMESPACE);
		element = element.createAttribute(_T("gml:Polygon"), _T("srsName"), _T("EPSG:28356"));
		element = element.createNode(_T("gml:exterior"), _T("")).createNode(_T("gml:LinearRing"), _T(""));
		element = element.createAttribute(_T("gml:posList"), _T("srsDimension"), _T("2"));
		element.setNodeValue(strPosList);
		
	}
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	NSSTRING strDateTime;
	strDateTime = parseDateTime(systemTime.wYear) + _T("-") + parseDateTime(systemTime.wMonth)
		+ _T("-") + parseDateTime(systemTime.wDay) + _T("T") + parseDateTime(systemTime.wHour) +
		+ _T(":") + parseDateTime(systemTime.wMinute) + _T(":") + parseDateTime(systemTime.wSecond);
	m_pElement->createNode(_T("ImportConversionDate"), strDateTime, DEFAULT_NAMESPACE);
	return NS_SUCCESS;
}

// This function parse the date and time for XML file.
NSSTRING CNSImportHeader::parseDateTime(int value)
{
	NSSTRING strMonth;
	TCHAR szMonth[10];
	NSSPRINTF(szMonth, _T("%d"), value);
	strMonth = szMonth;
	if(value < 10)
	{	
		strMonth = _T("0") + strMonth;
	}
	return strMonth;
}
void CNSImportHeader::setObject(CNSDWGMgr& dwgMgr)
{
	m_pDwgMgr = &dwgMgr;
}

void CNSImportHeader::setObject(CNSDatabaseMgr& dbMgr)
{
	m_pDbMgr = &dbMgr;
}

void CNSImportHeader::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

/*!
@Brief			This function retrives Drawing number from map of title block.
@Return			NSSTRING : Return current drawing number.
@Date			20-04-2012
*/

NSSTRING CNSImportHeader::getDrawingNumFromMap()
{
	std::map<NSSTRING, NSSTRING>::iterator it;
	it = m_mapOfTitleAttributes.find(_T("DrawingNumber"));
	if(m_mapOfTitleAttributes.end() != it)
		m_strDrawingNumber = (*it).second;
	return m_strDrawingNumber;
}

NSSTRING CNSImportHeader::getDrawingNumber() const
{
	return m_strDrawingNumber;
}

NSSTRING CNSImportHeader::getProjectNumber() const
{
	return m_strProjectNumber;
}

NSSTRING CNSImportHeader::getExportID() const
{
	return m_strExportID;
}