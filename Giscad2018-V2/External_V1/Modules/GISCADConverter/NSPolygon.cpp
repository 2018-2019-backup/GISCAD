#include "StdAfx.h"

#include "..\..\Include\NSDirectives.h"

#include "NSPolygon.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Constructor
CNSPolygon::CNSPolygon(void)
{
	
}

// Destructor
CNSPolygon::~CNSPolygon(void)
{

}

/*!
@Brief			This function reads polygon informaiton from XML file.
@param[in]		element : Geometry node
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSPolygon::readXML(CNSTElement &element)
{
	setType(GIS_POLYGON);
	CNSTElement oExtList = element.subnode(_T("gml:Polygon"));
	if(oExtList.getPosition() == -1)
		return NS_FAIL;
	CNSTElement oTemp, oElement;
	for (CNSTElement oElement = oExtList.begin(); oElement != oExtList.end(); oElement++)
	{
		readPoslist(oElement);
	}
	return NS_SUCCESS;
}

int CNSPolygon::readDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			This function writes polygon informaiton to XML file.
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSPolygon::writeXML()
{
	GEO_ELEMENT enType = ELE_INVALID;
	NSSTRING strList;
	int nSize = m_pCompGeometry->getPoslistSize();
	CNSTElement tempNode;
	CNSTElement node = m_pElement->createAttribute(_T("gml:Polygon"), _T("srsName"), _T("EPSG:28356"));
    tempNode = node.createNode(_T("gml:exterior"), _T(""));
	for(int nListCntr = 0; nListCntr < nSize; nListCntr++)
	{
		m_pCompGeometry->getAt(nListCntr, strList, enType);
		
		if(m_FNONumber ==378) //Commented to avoid the last set of data in arc and added new if condition		
		{
			const std::string s1( strList.begin(), strList.end() );
			
			vector< string > tokens;
			string Buffer;
    
			stringstream ss( s1 );

			while( ss >> Buffer )
				tokens.push_back( Buffer );
	
			string a = tokens.at(0) + " " + tokens.at(1) + " " +tokens.at(2);
			std::wstring wsTmp(a.begin(), a.end());

			if(ELE_LINEAR == enType && nSize == 1)
			{
				strList.append(wsTmp);
			}
		}


		if(ELE_LINEAR == enType && nSize == 1)
		{
			tempNode = tempNode.createNode(_T("gml:LinearRing"), _T(""));
			CNSTElement oPosListNode = tempNode.createAttribute(_T("gml:posList"), _T("srsDimension"), _T("2"));
			oPosListNode.setNodeValue(strList);
		}
		else
		{
			m_curveMember.setType(enType);
			m_curveMember.setPositionList(strList);
            if(nListCntr == 0)
            {
			    tempNode = tempNode.createNode(_T("gml:Ring"), _T(""));
            }
			m_curveMember.setObject(&tempNode);
			m_curveMember.writeXML();
		}
		
	}
	
	return NS_SUCCESS;
}

int CNSPolygon::writeDWG()
{
	return NS_SUCCESS;
}

NSSTRING CNSPolygon::getPositionList() const
{
	return _T("");
}

NSSTRING CNSPolygon::getLineString() const
{
	return _T("");
}

void CNSPolygon::setComposite(CNSCompositeGeometry* pGeometry)
{
	m_pCompGeometry = pGeometry;
}

/*!
@Brief			This function reads posList informaiton from Ring element.
@param[in]		element : Ring node
@Return			NS_SUCCESS : If reading process successful
@Date			12-06-2007
*/
int CNSPolygon::readPoslist(CNSTElement &Ring)
{
	CNSTElement oLinearRingElem = Ring.subnode(_T("gml:LinearRing"));
	GEO_ELEMENT enType = ELE_INVALID;
	NSSTRING strList;
	if(oLinearRingElem.getPosition() != -1)
	{
		enType = ELE_LINEAR;
		strList = oLinearRingElem.subnode(_T("gml:posList")).val();
		m_pCompGeometry->addPoslist(enType, strList, true);
	}
	else
	{
		CNSTElement oCurveElem = Ring.subnode(_T("gml:Ring"));
		for (CNSTElement oElement = oCurveElem.begin(); oElement != oCurveElem.end(); oElement++)
		{
			m_curveMember.setObject(&oElement);
			if(NS_SUCCESS != m_curveMember.readXML())
				return NS_FAIL;
			strList = m_curveMember.getPositionList();
			enType = m_curveMember.getType();
			m_pCompGeometry->addPoslist(enType, strList, true);
		}
	}
	return NS_SUCCESS;
}

NSSTRING CNSPolygon::getName() const
{
	return m_pCompGeometry->getName();
}