/*!
 @Class		:	CNSPolygon
 @Brief		:	This class is responsible for reading gml:polygon information
				from XML
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "NSGeometry.h"
#include "NSTElement.h"
#include "NSLineString.h"
#include "NSCurveMember.h"

class CNSPolygon:public CNSGeometry
{
public:
	CNSPolygon(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	NSSTRING getPositionList() const;
	NSSTRING getLineString() const;
	int readPoslist(CNSTElement &element);
	void setComposite(CNSCompositeGeometry* pGeometry);
	NSSTRING getName() const;
public:
	~CNSPolygon(void);

public:
	int m_FNONumber;

private:
	CNSCurveMember m_curveMember;

};
