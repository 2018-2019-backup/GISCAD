#include "StdAfx.h"
#include "NSLineString.h"

// Constructor
CNSLineString::CNSLineString(void)
{
}

// Destructor
CNSLineString::~CNSLineString(void)
{
}

NSSTRING CNSLineString::getPosList() const
{
	return _T("");
}

int CNSLineString::readDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			This function reads line string information from XML.
@Return			NS_SUCCESS : If reading process successful.
				NS_FAIL    : If reading process not successful.
@Date			12-06-2007
*/
int CNSLineString::readXML(CNSTElement &element)
{
	CNSTElement lineStringElem = element.subnode(_T("gml:LineString"))
		.subnode(_T("gml:posList"));
	m_pCompGeometry->addPoslist(ELE_LINEAR, lineStringElem.val());
	return NS_SUCCESS;
}

int CNSLineString::writeDWG()
{
	return NS_SUCCESS;
}

int CNSLineString::writeXML()
{
	return NS_SUCCESS;
}

NSSTRING CNSLineString::getLineString() const
{
	return _T("");
}

void CNSLineString::setComposite(CNSCompositeGeometry* pGeometry)
{
	m_pCompGeometry = pGeometry;
}