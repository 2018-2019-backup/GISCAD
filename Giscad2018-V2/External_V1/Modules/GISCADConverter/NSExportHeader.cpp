#include "StdAfx.h"
#include "..\..\Include\NSDirectives.h"
#include "NSExportHeader.h"

// Constructor
CNSExportHeader::CNSExportHeader(void)
{
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pPolygon = NULL;
	m_strExportID = _T("");
	m_strProjectType = _T("");
	m_strProjectNumber = _T("");
	m_strDrawingNumber = _T("");
	m_strProjectDes = _T("");
	m_strExportDate = _T("");
	m_strExportUserName = _T("");
	m_strDesignerCompanyName = _T("");
	m_strComments = _T("");

}

// Destructor
CNSExportHeader::~CNSExportHeader(void)
{
	delete m_pPolygon;
}

/*!
@Brief			This function reads ExportHeader information from XML.
@Param [in]		headerNode			:	Header Node
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSExportHeader::readXML(CNSTElement &headerNode)
{
	m_strExportID = headerNode.subval(_T("ExportID"));
	m_strProjectType = headerNode.subval(_T("ProjectType"));
	m_strProjectNumber = headerNode.subval(_T("ProjectNumber"));
	m_strDrawingNumber = headerNode.subval(_T("DrawingNumber"));
	m_strProjectDes = headerNode.subval(_T("ProjectDescription"));
	m_strExportDate = headerNode.subval(_T("ExportDate"));
	m_strExportUserName = headerNode.subval(_T("ExportUserName"));
	m_strDesignerCompanyName = headerNode.subval(_T("DesignerCompanyName"));
	m_strComments = headerNode.subval(_T("Comments"));
	// Read Export Categories
	CNSTElement oExportCategories = headerNode.subnode(_T("ExportCategories"));
	NSSTRING strExportCategories;
	for (CNSTElement element = oExportCategories.begin(); element != oExportCategories.end(); element++)
	{
		m_strExportCategores.append(element.val());
		m_strExportCategores.append(_T(","));
	}
	
	// Read Area of Interest
	CNSTElement oExportAreaofInterest = headerNode.subnode(_T("ExportAreaOfInterest"));
	m_pPolygon = new CNSPolygon;
	m_pPolygon->createComposite();
	if(NS_SUCCESS != m_pPolygon->readXML(oExportAreaofInterest))
		return NS_FAIL;
	
	return NS_SUCCESS;
}

/*!
@Brief			This function writes ExportHeader information into Drawing.
@Return			NS_SUCCESS : If writing process successful.
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSExportHeader::writeDWG()
{
	HEADER_MAP mapHeader;
	mapHeader.insert(HEADER_MAP::value_type(_T("ExportID"), m_strExportID));
	mapHeader.insert(HEADER_MAP::value_type(_T("ProjectType"), m_strProjectType));
    mapHeader.insert(HEADER_MAP::value_type(_T("ProjectNumber"), m_strProjectNumber));
    if(NSSTRCMP(m_strProjectType.c_str(),_T("external"))==0)
	    mapHeader.insert(HEADER_MAP::value_type(_T("CAP_NO"), m_strProjectNumber));
    else
    {
        if(NSSTRCMP(m_strProjectType.c_str(),_T("internal"))==0)
            mapHeader.insert(HEADER_MAP::value_type(_T("IE_NUM"), m_strProjectNumber));
    }
	mapHeader.insert(HEADER_MAP::value_type(_T("DrawingNumber"), m_strDrawingNumber));
    mapHeader.insert(HEADER_MAP::value_type(_T("NUM"), m_strDrawingNumber));
	mapHeader.insert(HEADER_MAP::value_type(_T("ProjectDescription"), m_strProjectDes));
	mapHeader.insert(HEADER_MAP::value_type(_T("ExportDate"), m_strExportDate));
	mapHeader.insert(HEADER_MAP::value_type(_T("ExportUserName"), m_strExportUserName));
	mapHeader.insert(HEADER_MAP::value_type(_T("DesignerCompanyName"), m_strDesignerCompanyName));
	mapHeader.insert(HEADER_MAP::value_type(_T("Comments"), m_strComments));
	mapHeader.insert(HEADER_MAP::value_type(_T("ExportCategories"), m_strExportCategores));
	CNSCompositeData* pData = m_pPolygon->getCompositeGeo()->getAt(0);
	NSSTRING strPosList;
	GEO_ELEMENT enEle;
	if(pData != NULL)
		pData->getData(strPosList, enEle);
	mapHeader.insert(HEADER_MAP::value_type(_T("ExportAreaOfInterest"), strPosList));
	//if(NS_SUCCESS != m_pDwgMgr->setTitleBlockAttributes(mapHeader))
	if(NS_SUCCESS != m_pDwgMgr->setTitleBlockAttributes(mapHeader,1))
		return NS_FAIL;
	return NS_SUCCESS;
}

void CNSExportHeader::setObject(CNSDWGMgr& dwgMgr)
{
	m_pDwgMgr = &dwgMgr;
}

void CNSExportHeader::setObject(CNSDatabaseMgr& dbMgr)
{
	m_pDbMgr = &dbMgr;
}

NSSTRING CNSExportHeader::getDrawingNumber() const
{
	return m_strDrawingNumber;
}

NSSTRING CNSExportHeader::getProjectNumber() const
{
	return m_strProjectNumber;
}

NSSTRING CNSExportHeader::getExportID() const
{
	return m_strExportID;
}