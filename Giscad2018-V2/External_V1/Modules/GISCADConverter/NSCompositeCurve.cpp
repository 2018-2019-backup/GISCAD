#include "StdAfx.h"
#include "math.h"
#include "NSCompositeCurve.h"
#include "..\..\Include\NSDirectives.h"

// Constructor
CNSCompositeCurve::CNSCompositeCurve(void)
{
}

// Destructor
CNSCompositeCurve::~CNSCompositeCurve(void)
{
	
}

/*!
@Brief			This function reads information from CompositeCurve node in the input XML file.
@Param [in]		element		:	Geometry node in the XML file to find CompositeCurve node
@Return			NS_SUCCESS : If reading successful
				NS_FAIL    : If reading not successful.
@Date			06-07-2007
*/
int CNSCompositeCurve::readXML(CNSTElement &element)
{
	CNSCurveMember curveMember;
	GEO_ELEMENT enType;
	setType(GIS_COMPOSITE_CURVE);
	CNSTElement compCurveNode = element.subnode(_T("gml:CompositeCurve"));
	if(compCurveNode.getPosition() != -1)
	{
		for (CNSTElement oElement = compCurveNode.begin(); oElement != compCurveNode.end(); oElement++)
		{
			curveMember.setObject(&oElement);
			if(NS_SUCCESS == curveMember.readXML())
			{
				NSSTRING strList = curveMember.getPositionList();
				enType = curveMember.getType();
				m_pCompGeometry->addPoslist(enType, strList);
			}
			else{
				return NS_FAIL;
			}
		}
	}
	else
	{
		m_LineString.setComposite(m_pCompGeometry);
		if(NS_FAIL == m_LineString.readXML(element))
			return NS_FAIL;
	}
	return NS_SUCCESS;
}

int CNSCompositeCurve::readDWG()
{
	return NS_SUCCESS;
}

/*!
@Brief			This function writes composite curve information into XML file.
@Return			NS_SUCCESS : If writing successful
				NS_FAIL    : If writing not successful.
@Date			06-07-2007
*/
int CNSCompositeCurve::writeXML()
{
	CNSCurveMember curveMember;
	GEO_ELEMENT enType = ELE_INVALID;
	NSSTRING strList;
	int nSize = m_pCompGeometry->getPoslistSize();
	CNSTElement node = m_pElement->createAttribute(_T("gml:CompositeCurve"), _T("srsName"), _T("EPSG:28356"));
	for(int nListCntr = 0; nListCntr < nSize; nListCntr++)
	{
		m_pCompGeometry->getAt(nListCntr, strList, enType);
		curveMember.setObject(&node);
		curveMember.setType(enType);
		curveMember.setPositionList(strList);
		curveMember.writeXML();
	}
	return NS_SUCCESS;
}

int CNSCompositeCurve::writeDWG()
{
	return NS_SUCCESS;
}
/*!
@Brief			The connectble end points of the block to which cable is attached at both ends
				used for Cable end points as block insertion points conversion
@Param [in]		vetor of insertion points at both the ends
@Return			true/false
@Date			12-06-2007
*/
bool CNSCompositeCurve::setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts)
{
	int nSize = m_pCompGeometry->getPoslistSize();
	if(nSize > 1) return false;
	CNSCompositeData *pData = m_pCompGeometry->getAt(0);
	NSSTRING strPosList;
	GEO_ELEMENT enType;
	pData->getData(strPosList, enType);
	std::vector<CNSPoint3D> vectOfVertices;
	convertString2Vertices(strPosList, vectOfVertices);
	if(vectOfVertices.size() != 2) return false;

	int nIndex1 = 0, nIndex2 = 0;
	double dDistance1 = 0.0, dDistance2 = 0.0;
	CNSPoint3D startPt, endPt;
	
	int nNode1Size = (int)vectNode1Pts.size();
	int nNode2Size = (int)vectNode2Pts.size();
	
	if(nNode1Size == 0 && nNode2Size == 0) return false;
	else if(nNode1Size > 0 && nNode2Size == 0)
	{
		getClosestPoint(vectOfVertices[0], vectNode1Pts, nIndex1, dDistance1);
		startPt = vectNode1Pts[nIndex1];
		endPt = vectOfVertices[1];
	}
	else if(nNode1Size == 0 && nNode2Size > 0)
	{
		getClosestPoint(vectOfVertices[0], vectNode2Pts, nIndex2, dDistance2);
		endPt = vectNode2Pts[nIndex2];
		startPt = vectOfVertices[0];
	}
	else
	{
		getClosestPoint(vectOfVertices[0], vectNode1Pts, nIndex1, dDistance1);
		getClosestPoint(vectOfVertices[0], vectNode2Pts, nIndex2, dDistance2);
		if(dDistance1 < dDistance2)
		{
			startPt = vectNode1Pts[nIndex1];
			getClosestPoint(vectOfVertices[1], vectNode2Pts, nIndex2, dDistance2);
			endPt = vectNode2Pts[nIndex2];
		}
		else
		{
			startPt = vectNode2Pts[nIndex2];
			getClosestPoint(vectOfVertices[0], vectNode1Pts, nIndex1, dDistance1);
			endPt = vectNode1Pts[nIndex2];
		}
	}
	NSSTRING strTemp;
	strPosList = getPosString(startPt.m_dXX, startPt.m_dYY);
	strPosList.append(_T(" "));
	strTemp = getPosString(endPt.m_dXX, endPt.m_dYY);
	strPosList.append(strTemp);
	pData->setData(strPosList, enType);
	return true;
}

bool CNSCompositeCurve::getClosestPoint(CNSPoint3D insertionPt, std::vector<CNSPoint3D> &vectPts, int &nIndex, double &dDistance)
{
	std::vector<CNSPoint3D>::iterator DistItr;
	CNSPoint3D tempPt;
	int nVectCntr = -1;
	double dTempDist = 0.0;
	if(vectPts.size() == 0) return false;
	for(DistItr = vectPts.begin(); DistItr != vectPts.end(); DistItr++)
	{
		nVectCntr++;
		tempPt = *DistItr;
		dTempDist = sqrt((insertionPt.m_dXX - tempPt.m_dXX) * (insertionPt.m_dXX - tempPt.m_dXX) 
						+(insertionPt.m_dYY - tempPt.m_dYY) * (insertionPt.m_dYY - tempPt.m_dYY));

		if(nVectCntr == 0)
		{
			dDistance = dTempDist;
			nIndex = nVectCntr;
		}
		else if(dTempDist < dDistance)
		{
			dDistance = dTempDist;
			nIndex = nVectCntr;
		}
	}
	return true;	
}

/*!
@Brief			This function formats the Position string for Composite curve to write into XML file.
@Return			formatted string
@Date			06-07-2007
*/
NSSTRING CNSCompositeCurve::getPosString(double dX, double dY)
{
	TCHAR szFormat[(_MAX_PATH + 1)];
	NSSPRINTF(szFormat,(_MAX_PATH + 1), _T("%lf %lf"), dX, dY);
	return szFormat;
}

int CNSCompositeCurve::convertString2Vertices(NSSTRING strPolyLineVertices, std::vector<CNSPoint3D>& vctPolyLineVertices)
{
	TCHAR* pszTemp = NULL;
	TCHAR* pszSep = _T(" ");
	CNSPoint3D ptTemp;
	int nPointCounter = 0;
	int nCordinateCounter = 1;

	// Establish string and get the first token (coordinate):
	TCHAR* pszNextToken;   //dummy 
	pszTemp = NSSTRTOK((TCHAR*)strPolyLineVertices.c_str(), pszSep, &pszNextToken);

	// While there are tokens in "string"
	while( pszTemp != NULL )
	{
		//Get x-coordinate
		if (nCordinateCounter%2 != 0)
		{			
			ptTemp.m_dXX = NSATOF(pszTemp);
			nCordinateCounter++;
		}
		else	// Get y-coordinate
		{
			ptTemp.m_dYY = NSATOF(pszTemp);
			nCordinateCounter++;

			ptTemp.m_dZZ = 0.0;
			// Add point to the vector
			vctPolyLineVertices.push_back(ptTemp);

			nPointCounter++;
		}
		// Get next token: (coordinate)
		pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
	}

	// If total number coordinates is not even
	if(nCordinateCounter%2 != 0)
		return NS_FAIL;

	return NS_SUCCESS;
}