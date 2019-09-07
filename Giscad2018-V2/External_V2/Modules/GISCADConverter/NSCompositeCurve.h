/*!
 @Class			CNSCompositeCurve
 @Brief			This class provides the functionality to read and write Composite
				curve information from/into XML or DWG 
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "NSGeometry.h"
#include "NSTElement.h"
#include "NSCurveMember.h"
#include "NSLineString.h"
#include "NSAttribute.h"

class CNSCompositeCurve:public CNSGeometry
{
public:
	CNSCompositeCurve(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	bool setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts);
	int convertString2Vertices(NSSTRING strPolyLineVertices, std::vector<CNSPoint3D>& vctPolyLineVertices);
	bool getClosestPoint(CNSPoint3D insertionPt, std::vector<CNSPoint3D> &vectPts, int &nIndex, double &dDistance);
	NSSTRING getPosString(double dX, double dY);
public:
	~CNSCompositeCurve(void);
private:
	CNSLineString m_LineString;
};
