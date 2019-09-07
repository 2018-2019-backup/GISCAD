#include "StdAfx.h"

#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSBlock.h"
#include "..\..\Include\NSReportWriter.h"

#include "NSComponent.h"
#include "NSPoint.h"
#include "NSPolygon.h"
#include "NSCompositeCurve.h"
#include "NSCompositeSurface.h"
#include "NSLineString.h"

#define WriteToLog(str,err) CNSReportWriter::getReportWriterInstance()->writeLog(str,err)

// Constructor
CNSComponent::CNSComponent(void)
{
	m_nCompNo = 0;
	m_ncompID = 1;
	m_pGeometry = NULL;
	m_pDwgMgr = NULL;
	m_pDbMgr = NULL;
	m_pElement = NULL;
	m_pCompGeometry = NULL;
	m_iCompRepresentation = 0;
}

// Destructor
CNSComponent::~CNSComponent(void)
{
	delete m_pGeometry;
	m_pGeometry = NULL;
}

/*!
@Brief			This function reads component informaiton from XML.
@Param [in]		element		featureNode
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSComponent::readXML(CNSTElement &element)
{
	// Read Component Number
	m_nCompNo = _wtoi(element.subval(_T("CNO")).c_str());
	// Read Component ID
	m_ncompID = _wtoi(element.subval(_T("CID")).c_str());
	// Get Geometry Subnode
	CNSTElement subelem = element.subnode(_T("Geometry"));
	// Get Point or Polygon or CompositeCurve
	CNSTElement geoelem = subelem.begin();
	// Get Geometry object
	createGeometry();
	if(m_pGeometry == NULL)
		return NS_FAIL;
	if(NS_SUCCESS != m_pGeometry->readXML(subelem))
		return NS_FAIL;
	return NS_SUCCESS;
}

/*!
@Brief			This function reads component informaiton from DWG.
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSComponent::readDWG()
{
	int nPosListSize = m_pCompGeometry->getPoslistSize();
	NSSTRING strPosList;
	GEO_ELEMENT enType = ELE_INVALID;
	m_pCompGeometry->setCNO(m_nCompNo);
	for(int nListCntr = 0; nListCntr < nPosListSize; nListCntr++)
	{
		
		m_pCompGeometry->getAt(nListCntr, strPosList, enType);
		TCHAR szQuery[_MAX_PATH + 1];
		NSSPRINTF(szQuery, _T("Select GeometryType from ComponentGeometryType where CNO = %d"),m_nCompNo);
		int nDBRet = m_pDbMgr->executeQuery(szQuery);
		if(-1 == nDBRet)
			return NS_FAIL;
		variant_t geoType;
		m_pGeometry  = createGeometry(enType); //Read from database..this is temporary
		if(NULL == m_pGeometry )
			return NS_FAIL;
		m_pGeometry->setObject(m_pCompGeometry);
		if(NS_SUCCESS != m_pGeometry->readDWG())
			return NS_FAIL;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes component informaiton into XML.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSComponent::writeXML()
{
	TCHAR strCNO[20] = _T("");
	NSSPRINTF(strCNO,_T("%d"),m_nCompNo);
	m_pElement->createNode(_T("CNO"), strCNO, DEFAULT_NAMESPACE);
	TCHAR strCID[20] = _T("");
	NSSPRINTF(strCID,_T("%d"),m_ncompID);
	m_pElement->createNode(_T("CID"), strCID, DEFAULT_NAMESPACE);
	CNSTElement geoElem = m_pElement->createNode(_T("Geometry"), _T(""), DEFAULT_NAMESPACE);
	m_pGeometry->setElement(&geoElem);
	m_pGeometry->m_FNOReference = m_FNOReference;
	if(NS_SUCCESS != m_pGeometry->writeXML())
		return NS_FAIL;
	return NS_SUCCESS;
}

/*!
@Brief			This function writes component informaiton into DWG.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSComponent::writeDWG()
{
	m_pGeometry->setObject(m_pDbMgr);
	m_pGeometry->setObject(m_pDwgMgr);
	return NS_SUCCESS;

}

void CNSComponent::setComponentNumber(const int nCompNo)
{
	m_nCompNo = nCompNo;
}

int CNSComponent::getComponentNumber() const
{
	return m_nCompNo;
}


void CNSComponent::setComponentID(const int nCompID)
{
	m_ncompID = nCompID;
}

int CNSComponent::getComponentID() const
{
	return m_ncompID;
}


void CNSComponent::setObject(CNSDWGMgr* pDwgMgr)
{
	m_pDwgMgr = pDwgMgr;
}

void CNSComponent::setObject(CNSDatabaseMgr* pDbMgr)
{
	m_pDbMgr = pDbMgr;
}


void CNSComponent::setObject(CNSCompositeGeometry *pCompGeometry)
{
	m_pCompGeometry = pCompGeometry;
}

void CNSComponent::setElement(CNSTElement* pElement)
{
	m_pElement = pElement;
}

void CNSComponent::setFNO(int FNO)
{
	m_FNOReference = FNO;
}

CNSGeometry* CNSComponent::getGeometry()
{
	return m_pGeometry;
}


void CNSComponent::getXData(STRING_MAP& mapOfXData)
{
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%i"), m_ncompID);
	mapOfXData.insert(STRING_MAP::value_type(_T("CID"), szFormat));
	NSSPRINTF(szFormat, _T("%i"), m_nCompNo);
	mapOfXData.insert(STRING_MAP::value_type(_T("CNO"), szFormat));
}

void CNSComponent::setXData(STRING_MAP& mapOfXData)
{
	NSSTRING strTemp;
	if(NS_SUCCESS == getValue(mapOfXData, _T("CID"), strTemp))
		m_ncompID = NSATOI(strTemp.c_str());
	if(NS_SUCCESS == getValue(mapOfXData, _T("CNO"), strTemp))
		m_nCompNo = NSATOI(strTemp.c_str());
}

int CNSComponent::getValue(STRING_MAP& mapOfXData, NSSTRING strType, NSSTRING &strValue)
{
	STRING_MAP::iterator TempItr;
	TempItr = mapOfXData.find(strType);
	if(TempItr == mapOfXData.end())
	{
		return NS_FAIL;
	}
	strValue = (*TempItr).second;
	return NS_SUCCESS;
}



void CNSComponent::setCompIdentity(CNSCompositeGeometry *pCompGeometry)
{
	pCompGeometry->setCID(getComponentID());
	pCompGeometry->setCNO(getComponentNumber());
}

/*!
@Brief			This function creates geometry using geometry tagname
@Param [in]		strGeoType : geometry type
@Return			CNSGeometry*   geometry object
@Date			12-06-2007
*/
CNSGeometry* CNSComponent::createGeometry(NSSTRING strGeoType)
{
	if(NSSTRCMP(strGeoType.c_str(), _T("gml:Point")) == 0)
	{
		m_pGeometry = new CNSPoint();
	}
	else
	{
		if(NSSTRCMP(strGeoType.c_str(), _T("gml:Polygon")) == 0)
		{
			m_pGeometry = new CNSPolygon();
		}
		else if(NSSTRCMP(strGeoType.c_str(), _T("gml:CompositeCurve")) == 0)
		{
			m_pGeometry = new CNSCompositeCurve();
		}
		else if(NSSTRCMP(strGeoType.c_str(), _T("gml:CompositeSurface")) == 0)
		{
			m_pGeometry = new CNSCompositeSurface();
		}
		else if(NSSTRCMP(strGeoType.c_str(), _T("gml:LineString")) == 0)
		{
			m_pGeometry = new CNSLineString();
		}
	}
	m_pGeometry->createComposite();
	return m_pGeometry;
}

/*!
@Brief			This function creates geometry using component number
@Date			12-06-2007
*/
void CNSComponent::createGeometry()
{
	TCHAR szQuery[_MAX_PATH + 1];
    NSSPRINTF(szQuery, _T("Select GeometryType,CADGeometryType from ComponentGeometryType where CNO = %d"),m_nCompNo);
    int nDBRet = m_pDbMgr->executeQuery(szQuery);
	if(-1 == nDBRet)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'ComponentGeometryType\'"));
		return ;
	}
    variant_t geoType;
    bool bGeoType = m_pDbMgr->getValue(_T("GeometryType"), geoType);
	if(!bGeoType)
	{
		TCHAR szMsg[_MAX_PATH + 1];
		NSSPRINTF(szMsg, _T("GeometryType not found from ComponentGeometryType table for ComponentNumber = %d"), m_nCompNo);
		WriteToLog(LOG_OTHER, szMsg);
		return ;
	}

	if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("TextPointGeometry")) == 0)
	{
		m_pGeometry = new CNSPoint();
		m_enComponentType = COMP_TEXTPOINT;
	}
	else if( NSSTRCMP((TCHAR*)geoType.bstrVal, _T("OrientedPointGeometry")) == 0)
	{
		m_pGeometry = new CNSPoint();
		m_enComponentType = COMP_POINT;
	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("CompositePolylineGeometry")) == 0)
	{
        variant_t CADType;
        bool bCADType = m_pDbMgr->getValue(_T("CADGeometryType"), CADType);
        if(!bCADType)
        {
            CADType.bstrVal = _T("");
        }
        if(NSSTRCMP((TCHAR*)CADType.bstrVal,_T("Pline"))==0 || 
            NSSTRCMP((TCHAR*)CADType.bstrVal,_T(""))==0) // Default CADType is polyline
        {
		    m_pGeometry = new CNSCompositeCurve();
		    m_enComponentType = COMP_COMPOSITE_POLYLINE;
        }
        else if(NSSTRCMP((TCHAR*)CADType.bstrVal,_T("Mline"))==0) 
        {
            m_pGeometry = new CNSCompositeCurve();
		    m_enComponentType = COMP_COMPOSITE_MLINE;
        }
        else
        {
            TCHAR szMsg[_MAX_PATH + 1];
		    NSSPRINTF(szMsg, _T("Invalid CADGeometryType value found in ComponentGeometryType table for CNO %d"), m_nCompNo);
		    WriteToLog(LOG_OTHER, szMsg);
		    return ;
        }
	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("BoundaryGeometry")) == 0)
	{
		m_pGeometry = new CNSPolygon();
		m_enComponentType = COMP_BOUNDARY;

	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("PolygonGeometry")) == 0)
	{
		m_pGeometry = new CNSPolygon();
		m_enComponentType = COMP_POLYGON;

	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("CompositePolygonGeometry")) == 0)
	{
        m_pGeometry = new CNSCompositeSurface();
        m_enComponentType = COMP_COMPOSITE_POLYGON;
	}
	if(m_pGeometry)
	{
		m_pGeometry->createComposite();
		CNSCompositeGeometry *pCompGeo = m_pGeometry->getCompositeGeo();
		pCompGeo->setCompType(m_enComponentType);
		pCompGeo->setCID(m_ncompID);
		pCompGeo->setCNO(m_nCompNo);
	}
}

/*!
@Brief			This function creates geometry using element type
@Param [in]		enType : element type
@Return			CNSGeometry*   geometry object
@Date			12-06-2007
*/
CNSGeometry* CNSComponent::createGeometry(GEO_ELEMENT enType)
{
	CNSGeometry  *pGeometry = NULL;
	m_enCompType = enType;
	TCHAR szQuery[_MAX_PATH + 1];
	m_nCompNo = m_pCompGeometry->getCNO();
	m_ncompID = m_pCompGeometry->getCID();
    NSSPRINTF(szQuery, _T("Select GeometryType from ComponentGeometryType where CNO = %d"),m_nCompNo);
    int nDBRet = m_pDbMgr->executeQuery(szQuery);
	if(-1 == nDBRet)
	{
		WriteToLog(LOG_OTHER, _T("DataBase Error : Failed to access table \'ComponentGeometryType\'"));
		return NULL;
	}
    variant_t geoType;
    bool bGeoType = m_pDbMgr->getValue(_T("GeometryType"), geoType);
	if(!bGeoType)
	{
		TCHAR szMsg[_MAX_PATH + 1];
		NSSPRINTF(szMsg, _T("GeometryType not found from ComponentGeometryType table for ComponentNumber = %d"), m_nCompNo);
		WriteToLog(LOG_OTHER, szMsg);
		return NULL;
	}

	if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("TextPointGeometry")) == 0 ||
	   NSSTRCMP((TCHAR*)geoType.bstrVal, _T("OrientedPointGeometry")) == 0)
	{
		pGeometry = new CNSPoint();
	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("CompositePolylineGeometry")) == 0)
	{
		pGeometry = new CNSCompositeCurve();
	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("BoundaryGeometry")) == 0 ||
		    NSSTRCMP((TCHAR*)geoType.bstrVal, _T("PolygonGeometry")) == 0)
	{
		pGeometry = new CNSPolygon();
	}
	else if(NSSTRCMP((TCHAR*)geoType.bstrVal, _T("CompositePolygonGeometry")) == 0)
	{
		pGeometry = new CNSCompositeSurface();
	}
	if(pGeometry)
		pGeometry->createComposite();
	return pGeometry;
}

GEO_ELEMENT  CNSComponent::getComponentType() const
{
	return m_enCompType;
}

COMPONENT_TYPE CNSComponent::getCompType() const
{
	return m_enComponentType;
}
void CNSComponent::setComponentType(const GEO_ELEMENT enCompType)
{
	m_enCompType = enCompType;
}

void CNSComponent::setGeoType(NSSTRING strType)
{
	m_strGeoType = strType;
}

void CNSComponent::setCompRepresentation(const short iCompRepresentation)
{
	m_iCompRepresentation = iCompRepresentation;
}

short CNSComponent::getCompRepresentation() const
{
	return m_iCompRepresentation;
}
