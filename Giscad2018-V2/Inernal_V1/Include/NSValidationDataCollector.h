#pragma once
#pragma warning( disable: 4251)
#include <vector>
#include <map>
#include "string"
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSPoint3D.h"

// nNumberNotFound and nNumberFound have been defined assuming that more than 36999 features will not be processed at a time
#define nNumberNotFound 0

typedef std::map<NSSTRING, NSSTRING> NSSTRING_MAP;

enum FeatureType
{
	FEATURE_INVALID = -1,
	FEATURE_BLOCK = 1,
	FEATURE_LINEAR = 2,
	FEATURE_BUSBAR = 3,
	FEATURE_ARC = 4
};

enum FeatureStatus
{
	STATUS_INVALID = -1,
	STATUS_NEW = 1,
	STATUS_MOD = 2,
	STATUS_EXT = 3,
};


class CNSCircle
{
public:
	CNSCircle(){m_dRadius = 0; m_Centre.m_dXX = m_Centre.m_dYY = m_Centre.m_dZZ = 0.0;}
	~CNSCircle(){};
	void setCenterPt(const double dX, const double dY, const double dZ)
	{m_Centre.m_dXX = dX; m_Centre.m_dYY = dY; m_Centre.m_dZZ = dZ;}
public:
	CNSPoint3D m_Centre;
	double m_dRadius;
};

/*!
 @Class		CNSValidationDataCollector
 @Brief		stores the data required for validation of an object
 @Author	Neilsoft Ltd
 @Date		August 2007
 @History 	
 */
 class _declspec(dllexport) CNSValidationDataCollector
{
public:
	CNSValidationDataCollector(void);
	virtual ~CNSValidationDataCollector(void);

public:
	LONG_PTR getObjectId();
	int getFID();
	int getFNO();
	FeatureType getFeatureType();
	FeatureStatus getFeatureStatus();
	NSSTRING getBlockName();
	NSSTRING getLayerName();
	NSSTRING getLineType();
	CNSPoint3D  getInsertionPoint();
	NSSTRING getInsertionPointString();
	CNSPoint3D getEndPoint();
	NSSTRING getEndPointString();
	void getPolylineVertices(std::vector<CNSPoint3D*> &vectOfPolylineVertices);
	CNSPoint3D getScaleFactor();
	CNSCircle* getCircle();

	void setObjectId(LONG_PTR);
	void setFID(int);
	void setFNO(int);
	void setFeatureType(FeatureType enFeatureType);
	void setFeatureStatus(FeatureStatus enFeatureStatus);
	void setBlockName(NSSTRING strBlkName);
	void setLayerName(NSSTRING strLayerName);
	void setLineType(NSSTRING strLineType);
	void setInsertionPoint(double dXX, double dYY);
	void setEndPoint(double dXX, double dYY);
	void addToVectOfPolylineVertices(double dXX, double dYY);
	void setScaleFactor(double dXX, double dYY);
	void setCircle(CNSCircle *pCircle);

	NSSTRING readXDATA(NSSTRING strKey);
	void addXDATA(std::map<NSSTRING, NSSTRING> &mapXDataRead);
	NSSTRING readAttribute(NSSTRING strKey);
	NSSTRING getNotesAppended(NSSTRING strMsg);

private:
	LONG_PTR m_nAcDbObjectId;
	int m_nFID;
	int m_nFNO;
	FeatureType m_FeatureType;
	FeatureStatus m_FeatureStatus; 
	NSSTRING m_strBlockName;
	NSSTRING m_strLayerName;
	NSSTRING m_strLineType;
	CNSPoint3D m_insertionPoint;// In case of a linear feature this is taken as the start point
	CNSPoint3D m_EndPoint;		// used only in case of a linear feature
	std::vector<CNSPoint3D*> m_vectOfPolylineVertices;
	CNSPoint3D m_ScaleFactor;	// referred to decide features in proximity
	CNSCircle *m_pCircle;
	

public:	
	std::vector<LONG_PTR> m_vectFeaturesInProximity;
	std::vector<LONG_PTR> m_vectFeaturesInProximityByCmnAttVal;
	std::vector<LONG_PTR> m_vectInterPolyFeaturesInProximity; // vector to store intermediate features on polyline verices
	std::vector<LONG_PTR> m_vectFeaturesInProximityAtNode2;  // Used in case of Linear feature	
	NSSTRING_MAP m_mapAttributes;	// For New features take USAGE, ORIENTATION  from m_mapAttributes
	NSSTRING_MAP m_mapOfRelationsAndConnectivity; //USAGE, ORIENTATION, NODE1, NODE2, OWNERSHIP_PARENT, OWNERSHIP_CHILD, MANYOWENERSHIP_CHILD
	bool bPolyLine;
};

typedef std::map<LONG_PTR, CNSValidationDataCollector*> VALIDATE_COL_MAP_TYPE;