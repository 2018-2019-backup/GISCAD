#include "StdAfx.h"
#include "NSCurveMember.h"

// Constructor
CNSCurveMember::CNSCurveMember(void)
{
	m_strPosList = _T("");
	m_pElement = NULL;
	m_enType = ELE_INVALID;
}

// Destructor
CNSCurveMember::~CNSCurveMember(void)
{
}

void CNSCurveMember::setObject(CNSTElement *pElement)
{
	m_pElement = pElement;
}

GEO_ELEMENT CNSCurveMember::getType() const
{
	return m_enType;
}

void CNSCurveMember::setType(const GEO_ELEMENT enType)
{
	m_enType = enType;
}

NSSTRING CNSCurveMember::getPositionList()const
{
	return m_strPosList;
}

void CNSCurveMember::setPositionList(const NSSTRING strPosList)
{
	m_strPosList = strPosList;
}

/*!
@Brief			This function reads Curve member informaiton from XML file.
@Return			NS_SUCCESS : If reading process successful.
@Date			12-06-2007
*/
int CNSCurveMember::readXML()
{
	CNSTElement curveNode  = m_pElement->subnode(_T("gml:Curve"));
	if(curveNode.getPosition() != -1)
	{
		CNSTElement posListNode = curveNode.subnode(_T("gml:segments"))
			.subnode(_T("gml:Arc")).subnode(_T("gml:posList"));
		m_strPosList = posListNode.val();
		m_enType = ELE_CUVRVE;
	}
	else
	{
		CNSTElement lineStringNode = m_pElement->subnode(_T("gml:LineString"))
		.subnode(_T("gml:posList"));
		m_strPosList = lineStringNode.val();
		m_enType = ELE_LINEAR;
	}
	return NS_SUCCESS;
}

/*!
@Brief			This function writes Curve member informaiton into XML file.
@Return			NS_SUCCESS : If reading process successful.
@Date			12-06-2007
*/
int CNSCurveMember::writeXML()
{
	CNSTElement tempNode = m_pElement->createNode(_T("gml:curveMember"), _T(""));
	if(ELE_CURVE_MEMEBR != m_enType && ELE_CUVRVE != m_enType)
	{
		tempNode = tempNode.createNode(_T("gml:LineString"), _T(""));
	}				
	else
	{
		CNSTElement oPosListNode = tempNode.createAttribute(_T("gml:Curve"), _T("srsName"), _T("EPSG:28356"));
		tempNode = oPosListNode.createNode(_T("gml:segments"), _T(""));
		tempNode = tempNode.createAttribute(_T("gml:Arc"), _T("interpolation"), _T("circularArc3Points"));
	}
	CNSTElement oPosListNode = tempNode.createAttribute(_T("gml:posList"), _T("srsDimension"), _T("2"));
	oPosListNode.setNodeValue(m_strPosList);
	return NS_SUCCESS;
}

NSSTRING CNSCurveMember::getLineString()
{
	return m_strPosList;
}