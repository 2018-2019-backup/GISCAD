#pragma once
#pragma warning( disable: 4251)
#include <vector>
#include "string"
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSenComponentTypes.h"

enum GEO_ELEMENT
{
	ELE_INVALID = -1,
	ELE_CUVRVE = 0,
	ELE_CURVE_MEMEBR = 1,
	ELE_LINEAR = 2,
	ELE_BLK_POINT = 3,
	ELE_TXT_POINT = 4,
};

class _declspec(dllexport) CNSCompositeData
{
public:
	CNSCompositeData(void){/*m_strPoslist = NULL*/;};
public:
	virtual ~CNSCompositeData(void){/*delete []m_strPoslist*/;};
	void setData( NSSTRING strPoslist,  GEO_ELEMENT enType);
	void getData(NSSTRING &strPoslist, GEO_ELEMENT &enType);
	GEO_ELEMENT getType()const;
	void setType(const GEO_ELEMENT enType);
protected:
	//TCHAR* m_strPoslist; 
	NSSTRING m_strPoslist; 
	GEO_ELEMENT m_enType;
};


class _declspec(dllexport) CNSCompositeGeometry
{
public:
	CNSCompositeGeometry(void);
public:
	virtual ~CNSCompositeGeometry(void);
	void addPoslist(GEO_ELEMENT enType, NSSTRING strPoslist, bool bClosed = false);
	void add(CNSCompositeData* pData);
	int getPoslistSize();
	void getAt(int nIndex, NSSTRING &strPoslist, GEO_ELEMENT &enType );
	CNSCompositeData* getAt(int nIndex);
	int getCID()const;
	int getCNO()const;
	void setCID(int nCID);
	void setCNO(int nCNO);
	NSSTRING getLayername()const;
	NSSTRING getName()const;
	COMPONENT_TYPE getCompType()const;
	void setLayername(NSSTRING strLayerName);
	void setName(NSSTRING strName);
	void setCompType(const COMPONENT_TYPE enType);
	bool isClosed();	
	void setCompRepresentation(const short iCompRepresentation);
	short getCompRepresentation() const;
	CNSCompositeGeometry& operator= (CNSCompositeGeometry& pGeo);   // Assignment operator

private:
	std::vector<CNSCompositeData*> m_vectComposites;
	int m_nCID;
	int m_nCNO;
	bool m_bClosed;
	COMPONENT_TYPE m_enComponentType;
	NSSTRING m_strGeoName; // for Cable it is Layer Name , for Block it is Block name
	NSSTRING m_strLayerName;
	short m_iCompRepresentation; 
};

class _declspec(dllexport) CNSCompositeContainer
{
public:
	CNSCompositeContainer(void);
public:
	virtual ~CNSCompositeContainer(void);
	void add(CNSCompositeGeometry*);
	CNSCompositeGeometry* getAt(int nIndex);
	int getSize();
	COMPONENT_TYPE getCompType(int nCompNumber);
private:
	std::vector<CNSCompositeGeometry*> m_vectGeometries;
};
