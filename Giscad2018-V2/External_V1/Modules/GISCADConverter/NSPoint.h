/*!
 @Class		:	CNSPoint
 @Brief		:	This class is responsible for reading gml:point information
				from XML
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once

#include "NSGeometry.h"
#include "..\..\Include\NSPoint3D.h"
#include "..\..\Include\NSDirectives.h"
#include "NSTElement.h"
class CNSPoint:public CNSGeometry
{
public:
	CNSPoint(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	NSSTRING getLineString() const;
	void getXData(STRING_MAP& mapOfXData);
	NSSTRING getName() const;
	bool getInsertionPoint(CNSPoint3D &pt);

public:
	~CNSPoint(void);
};
