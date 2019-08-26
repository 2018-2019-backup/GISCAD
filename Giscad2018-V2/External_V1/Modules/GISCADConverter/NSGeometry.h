/*!
 @Class		:	CNSGeometry
 @Brief		:	This class provides the functionality to read geometry informaiton
				from XMl and write into DWG.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once

#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSBlock.h"
#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSCompositeGeometry.h"

#include "NSTElement.h"
#include "NSAttribute.h"


enum GEOMETRY_TYPE
{
	GIS_POINT = 1001,
	GIS_POLYGON = 1002,
	GIS_COMPOSITE_CURVE = 1003,
	GIS_COMPOSITE_SURFACE = 1004,
	GIS_LINESTRING = 1005
};
class CNSGeometry
{
public:
	CNSGeometry(void);
	virtual int readXML(CNSTElement &element) = 0;
	virtual int readDWG();
	virtual int writeXML();
	virtual int writeDWG();
	virtual NSSTRING getLineString();
	void setType(GEOMETRY_TYPE enGeoType);
	enum GEOMETRY_TYPE getType();
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setObject(CNSBlock* pBlk);
	void setElement(CNSTElement* pElement);
	void setObject(CNSCompositeGeometry* pCompGeometry);
	virtual NSSTRING getName(){return m_pCompGeometry->getName();};
	virtual CNSCompositeGeometry* getCompositeGeo(){return m_pCompGeometry;};
	virtual void createComposite();
public:
	~CNSGeometry(void);
public:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSBlock* m_pBlk;
	CNSTElement *m_pElement;
	int m_FNOReference;

protected:
	CNSCompositeGeometry* m_pCompGeometry;

private:
	enum GEOMETRY_TYPE m_type;
};
