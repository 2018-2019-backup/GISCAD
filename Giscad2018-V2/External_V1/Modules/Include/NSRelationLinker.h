#pragma once
#include "..\Include\NSDirectives.h"
#include "..\Include\NSDWGMgr.h"

enum RelationshipConnectivityType
{
	DATA_INVALID = -1,
	OWNERSHIP_PARENT_DATA = 1,
	OWNERSHIP_CHILD_DATA = 2,
	MANYOWNERSHIP_CHILD_DATA = 3,
	CONNECTIVITY_NODE1_DATA = 4,
	CONNECTIVITY_NODE2_DATA = 5,
};

/*
 @Class		CNSProposedRelations
 @Brief		Store Relationship and connectivity information, which is to be written as XDATA
 @Author	Neilsoft Ltd
 @Date		August 2007
 @History
*/
class CNSProposedRelations
{
	public:
		CNSProposedRelations(void);
		~CNSProposedRelations(void);

	public:
		NSSTRING getFID();
		int getFIDNumber();
		NSSTRING getOwnershipParentFID();
		int getOwnershipParentFNO();
		NSSTRING getOwnershipChildsFID();
		NSSTRING getManyOwnershipFID();
		NSSTRING getConNode1FID();
		NSSTRING getConNode2FID();

		int getNumConAtNode1();
		int getNumConAtNode2();

		void setFID(int nFID);
		void setOwnershipParentFID(int, int);
		void setRelConData(enum RelationshipConnectivityType, int nFID, int nFNO);

	private:
		int					m_nFID;
		int					m_nOwnershiParentFID;
		int					m_nOwnershipParentFNO;
		// First integer represents FID and second integer represents FNO of related feature in following maps
		std::map<int, int>	m_mapOwnershipChildFID; 
		std::map<int, int>	m_mapManyOwnershipChildFID;
		std::map<int, int>	m_mapConNode1FID;
		std::map<int, int>	m_mapConNode2FID;
};


/*
 @Class		CNSRelationLinker
 @Brief		Stores all the data required for relationship and connectivity
 @Author	Neilsoft Ltd
 @Date		August 2007
 @History
*/
	
class CNSRelationLinker
{
public:
	CNSRelationLinker(void);
	virtual ~CNSRelationLinker(void);

public:
	void setLocalDWGObject(CNSDWGMgr*);
	int link(int nRelFlag, double dSearchAreaFactor, double dSearchAreaRadius, double dSymbolFactor, int LinearConnOnLayerFlag);
	
private:
	int initializeFeatures(bool bProposedToExisting, int LinearConnOnLayerFlag);
	void searchBusBar();
	void setSearchAreaFactor(double dSearchAreaFactor);

	void populateProposedRelMap();
	void establishRelationship(bool bProposedToExisting, int LinearConnOnLayerFlag);
	void assignOwnershipParentChild(CNSValidationDataCollector* pFeature);
	void assignOwnershipChild(CNSValidationDataCollector* pFeature);
	void assignManyOwnershipChilds(CNSValidationDataCollector* pFeature);
	void assignElectricalConnections(CNSValidationDataCollector* pFeature, bool bProposedToExisting, int LinearConnOnLayerFlag);

	void postasXDATA();
	
	// Functions retrieving Data from MDB
	int retrieveFNOFromDatabase(CNSValidationDataCollector* pFeature, int &nFNO);
	int getOwnershipParentsFromMDB(CNSValidationDataCollector* pFeature, std::vector<int> &vectOwnershipParents);
	int getOwnershipChilds(CNSValidationDataCollector* pFeature, std::vector<int> &vectOwnershipChilds);
	int getManyOwnershipChilds(CNSValidationDataCollector* pFeature, std::vector<int> &vectManyOwnershipChilds);
	int getConnectableAssets(CNSValidationDataCollector* pFeature, std::vector<int> &vectAssetsAtNode1);
	int getConnectionNode(CNSValidationDataCollector* pFeature, int &nNode);
	int getNumberOfNodes(CNSValidationDataCollector* pFeature, int &nNumberOfNodes);
	int getConnectionMethod(CNSValidationDataCollector* pFeature, int &nConMethod);
	NSSTRING getCableOrientation(CNSValidationDataCollector* pFeature);
	int retrieveLinearFeatUsage(CNSValidationDataCollector* pFeature, NSSTRING &strUsage);
	int retrieveLinearFeatOrientation(CNSValidationDataCollector* pFeature, NSSTRING &strOrientation);
	//

	//Should be called in the same sequence in initialize() function
	int asignFNOAndFID();
	void assignFeaturesInProximity(int LinearConnOnLayerFlag);
	int sortFeaturesByDistance(CNSValidationDataCollector *ValidationDataCollectorObj);
	void checkIfBlockWithinBlockBoundary(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked, int LinearConnOnLayerFlag  = 0);
	void checkInBusBarBoundary(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked);
	void checkOnPolylineVertices(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked);
	bool checkforUsage(CNSValidationDataCollector *pFeature , CNSValidationDataCollector *pProxFeature);
	bool checkforOrientation(CNSValidationDataCollector *pFeature , CNSValidationDataCollector *pProxFeature);
	void checkInCircularArea(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked,int LinearConnOnLayerFlag );
	bool checkPointInCircle(CNSPoint3D centrePt, CNSPoint3D givenPt, double dRadius, double dSearchFactor);
	double getSquareDist(CNSPoint3D pt1, CNSPoint3D pt2);
	void checkIfBlockInCircularArea(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked);
	int compareTokenValues(NSSTRING strActValue, NSSTRING strAllowedValue);
	int tokenize(NSSTRING strInputString, STRING_MAP &mapOfValues);
	bool checkPointOnLine(CNSPoint3D startPt, CNSPoint3D endPt, CNSPoint3D givenPt);
	bool checkBlocksBasedOnAttribites(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked);

	bool CheckIfUsageIsMatching(CNSValidationDataCollector* pFeature1, CNSValidationDataCollector* pFeature2 );
	NSSTRING getLayerType(NSSTRING strLayName);
	NSSTRING getUsage(CNSValidationDataCollector* pFeature2);




	//

	// Utility functions
	CNSValidationDataCollector* getFeature(LONG_PTR lObjectId); // gets feature from m_mapBlocksToBeLinked or m_mapLinearFeaturesToBeLinked
	NSSTRING getQueryString(NSSTRING strQueryString);
	NSSTRING getQueryString(NSSTRING_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH]);
	NSSTRING getUsageOrntBlkName(CNSValidationDataCollector* pFeature);
	double getPointToLineDistance(CNSPoint3D ptLineStart, CNSPoint3D ptLineEnd, CNSPoint3D, bool &bWithinLineSegment);
	//double getPointToPointDistance(CNSPoint3D, CNSPoint3D);
	//bool checkPointInSearchArea(CNSPoint3D ptLowerLeft, CNSPoint3D ptUpperRight, CNSPoint3D ptInProximity);
	//
	
	//validateBlock();		// Connectivity and Relationship related validation
	void removeInvalidFeatures();
	void validateOwnershipParent();
	void validateManyOwnershipChilds();
	void validateCables();
	void validateBlockFeatures();
	void clearExistingXDataForNew();
	void initializeMap(STRING_MAP &mapXDATA);

	
	
public:
	std::map<LONG_PTR, CNSProposedRelations*> m_mapProposedRelations;			// this information is to be sent to postXDATA 
private:
	double m_dSearchAreaFactor;												// retrieved from settings.ini, and is used to search features in proximity
	double m_dSearchAreaRadius;												// retrieved from settings.ini, and is usedfor arc features in proximity
	double m_dSymbolFactor;
	CNSDWGMgr *m_pLocalDWGMgr;												// fetch map data through object of CNSDWGMgr 
	std::map<LONG_PTR, CNSValidationDataCollector*> m_mapBlocksToBeLinked;		// NEW and MOD blocks are stored in this map
	std::map<LONG_PTR, CNSValidationDataCollector*> m_mapLinearFeaturesToBeLinked; // NEW linear features(i.e. cables) are stored in this map
	std::map<LONG_PTR, CNSValidationDataCollector*> m_mapExtBlocks;				// Only EXT Blocks
	std::map<LONG_PTR, CNSValidationDataCollector*> m_mapExtCablesToBeLinked;	// Only EXT cables
};
