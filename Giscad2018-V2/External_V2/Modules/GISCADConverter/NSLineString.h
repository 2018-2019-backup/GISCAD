/*!
 @Class		:	CNSLineString
 @Brief		:	This class is responsible for Reading line string from XML and writing 
				line string value into XML.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDirectives.h"
#include "NSTElement.h"
#include "NSGeometry.h"

class CNSLineString:public CNSGeometry
{
public:
	CNSLineString(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	NSSTRING getPosList() const;
	NSSTRING getLineString() const;
	void setComposite(CNSCompositeGeometry* pGeometry);
public:
	~CNSLineString(void);
};
