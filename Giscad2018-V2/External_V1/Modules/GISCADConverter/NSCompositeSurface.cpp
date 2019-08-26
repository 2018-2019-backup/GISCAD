#include "StdAfx.h"
#include "NSCompositeSurface.h"

// Constructor
CNSCompositeSurface::CNSCompositeSurface(void)
{
	m_pElement = NULL;
}

// Destructor
CNSCompositeSurface::~CNSCompositeSurface(void)
{

}

/*!
@Brief			This function reads compositeSurface informaiton from XML file.
@Param [in]		element			:	Geometry  Node
@Return			NS_SUCCESS : If reading process successful
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSCompositeSurface::readXML(CNSTElement &element)
{
	CNSTElement surfaceMemberElem = element.subnode(_T("gml:CompositeSurface"));
	if(surfaceMemberElem.getPosition() == -1)
		return NS_FAIL;
	for (CNSTElement oElement = surfaceMemberElem.begin(); oElement != surfaceMemberElem.end(); oElement++)
	{
		m_polygon.setComposite(m_pCompGeometry);
		if(NS_FAIL == m_polygon.readXML(oElement))
			return NS_FAIL;
	}
	return NS_SUCCESS;
}

int CNSCompositeSurface::readDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			This function writes compositeSurface informaiton into XML file.
@Return			NS_SUCCESS : If writing process successful
				NS_FAIL    : If writing process not successful.
@Date			12-06-2007
*/
int CNSCompositeSurface::writeXML()
{
	CNSTElement surfaceNode = m_pElement->createAttribute(_T("gml:CompositeSurface"), _T("srsName"), _T("EPSG:28356"));
	CNSTElement tempNode = surfaceNode.createNode(_T("gml:surfaceMember"), _T(""));		
	m_polygon.setElement(&tempNode);
	m_polygon.setComposite(m_pCompGeometry);
	m_polygon.m_FNONumber = m_FNOReference;
	m_polygon.writeXML();
	return NS_SUCCESS;
}

int CNSCompositeSurface::writeDWG()
{
	return NS_SUCCESS;
}

NSSTRING CNSCompositeSurface::getLineString()
{
	return _T("");
}