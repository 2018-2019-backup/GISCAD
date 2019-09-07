/*!
 @Class			CNSCompositeSurface
 @Brief			This class provides the functionality to read and write Composite
				surface information from/into XML or DWG 
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "NSGeometry.h"
#include "NSTElement.h"
#include "NSPolygon.h"

class CNSCompositeSurface:public CNSGeometry
{
public:
	CNSCompositeSurface(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	NSSTRING getLineString();

public:
	~CNSCompositeSurface(void);

public:
	int m_FNORefNumber;

private:
	CNSPolygon m_polygon;
};
