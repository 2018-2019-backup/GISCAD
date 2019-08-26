#include "StdAfx.h"
#include "..\Include\NSRelationLinker.h"
#include "..\Include\NSDatabaseMgr.h"
#include "NSErrorCircle.h"
#include "NSErrorParser.h"
#include "math.h"

extern CNSErrorCircle gErrorCircle;
extern std::map<LONG_PTR, LONG_PTR> gMapOfValidIDs;
extern double gTolerance;

CNSProposedRelations::CNSProposedRelations(void)
{
	m_nFID = nNumberNotFound;
	m_nOwnershiParentFID = nNumberNotFound;
}

CNSProposedRelations::~CNSProposedRelations(void)
{
}

/*
 @Brief		gets FID of the feature in sting format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getFID()
{
	if(m_nFID != nNumberNotFound)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		NSSPRINTF(szFormat, _T("%i"), m_nFID);	
		return szFormat;
	}
	else 
		return _T("");
}

/*
 @Brief		gets FID of the feature in number format
 @Return	int
*/
int CNSProposedRelations::getFIDNumber()
{
	return m_nFID;
}

/*
 @Brief		gets comma separated OWNERSHIP_PARENTS of a feature in sting format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getOwnershipParentFID()
{
	if(m_nOwnershiParentFID != nNumberNotFound)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		NSSPRINTF(szFormat, _T("%i,%i"), m_nOwnershiParentFID, m_nOwnershipParentFNO);	
		return szFormat;
	}
	else 
		return _T("");
}

int CNSProposedRelations::getOwnershipParentFNO()
{
	return m_nOwnershipParentFNO;
}

/*
 @Brief		gets comma separated OWNERSHIP_CHILDS of a feature in sting format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getOwnershipChildsFID()
{
	NSSTRING strOwnershipChilds(_T(""));
	if(m_mapOwnershipChildFID.size() != 0)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		std::map<int, int>::iterator mapItr;
		for(mapItr = m_mapOwnershipChildFID.begin(); mapItr != m_mapOwnershipChildFID.end(); mapItr++)
		{	
			if(mapItr == m_mapOwnershipChildFID.begin())
				NSSPRINTF(szFormat, _T("%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));
			else
				NSSPRINTF(szFormat, _T(",%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second)); // else comma is added as a prefix

			strOwnershipChilds.append(szFormat);
		}
	}
	return strOwnershipChilds;
}

/*
 @Brief		gets comma separated MANYOWNERSHIP_CHILDS of a feature in sting format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getManyOwnershipFID()
{
	NSSTRING strManyOwnershipChilds(_T(""));
	if(m_mapManyOwnershipChildFID.size() != 0)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		std::map<int, int>::iterator mapItr;
		for(mapItr = m_mapManyOwnershipChildFID.begin(); mapItr != m_mapManyOwnershipChildFID.end(); mapItr++)
		{	
			if(mapItr == m_mapManyOwnershipChildFID.begin())
				NSSPRINTF(szFormat, _T("%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));
			else
				NSSPRINTF(szFormat, _T(",%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));	// else comma is added as a prefix
			strManyOwnershipChilds.append(szFormat);
		}
	}
	return strManyOwnershipChilds;
}

/*
 @Brief		gets comma separated FID and FNO values of features in CONNECTIVITY_NODE1 in string format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getConNode1FID()
{
	NSSTRING strConNode1(_T(""));
	if(m_mapConNode1FID.size() != 0)
	{
		TCHAR szFormat[_MAX_PATH + 1];
		std::map<int, int>::iterator mapItr;
		for(mapItr = m_mapConNode1FID.begin(); mapItr != m_mapConNode1FID.end(); mapItr++)
		{	
			if(mapItr == m_mapConNode1FID.begin())
				NSSPRINTF(szFormat, _T("%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));
			else
				NSSPRINTF(szFormat, _T(",%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));	// else comma is added as a prefix
			strConNode1.append(szFormat);
		}
	}
	return strConNode1;
}

/*
 @Brief		gets comma separated FID and FNO values of features in CONNECTIVITY_NODE2 in string format
 @Return	NSSTRING
*/
NSSTRING CNSProposedRelations::getConNode2FID()
{
	NSSTRING strConNode2(_T(""));
	if(m_mapConNode2FID.size() != 0)
	{
		TCHAR szFormat[_MAX_PATH + 2];
		std::map<int, int>::iterator mapItr;
		for(mapItr = m_mapConNode2FID.begin(); mapItr != m_mapConNode2FID.end(); mapItr++)
		{	
			if(mapItr == m_mapConNode2FID.begin())
				NSSPRINTF(szFormat, _T("%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));
			else
				NSSPRINTF(szFormat, _T(",%i,%i"), ((int)(*mapItr).first), ((int)(*mapItr).second));	// else comma is added as a prefix
			strConNode2.append(szFormat);
		}
	}
	return strConNode2;	
}

/*
 @Brief		gets number of connections at node 1
 @Return	int
*/
int CNSProposedRelations::getNumConAtNode1()
{
	return (int)m_mapConNode1FID.size();
}

/*
 @Brief		gets number of connections at node 2
 @Return	int
*/
int CNSProposedRelations::getNumConAtNode2()
{
	return (int)m_mapConNode2FID.size();
}

/*
 @Brief		sets feature FID
 @Param(in)	int
*/
void CNSProposedRelations::setFID(int nFID)
{
	// Set the Parent FID only if it has not already been set.
	if(m_nFID == nNumberNotFound)
		m_nFID = nFID;
}

/*
 @Brief			sets FID and FNO of the parent feature
 @Param(in)		int		FID of parent feature
 @Param(in)		int		FNO of parent feature
*/
void CNSProposedRelations::setOwnershipParentFID(int nOwenershipParentFID, int nOwenershipParentFNO)
{
	// Set the Parent FID only if it has not already been set.
	if(m_nOwnershiParentFID == nNumberNotFound)
	{
		m_nOwnershiParentFID = nOwenershipParentFID;
		m_nOwnershipParentFNO = nOwenershipParentFNO;
	}
}

/*
 @Brief			sets FID and FNO of the related feature
 @Param(in)		RelationshipConnectivityType	tells in which map the FID and FNO should be stored
 @Param(in)		int		nFID of related feature
 @Param(in)		int		nFNO of related feature
*/
void CNSProposedRelations::setRelConData(RelationshipConnectivityType eRelConType, int nFID, int nFNO)
{
	std::map<int, int> *pTempMapOfRelCon = NULL;

	switch (eRelConType)
	{
		case OWNERSHIP_CHILD_DATA:	
								pTempMapOfRelCon = &(m_mapOwnershipChildFID);
								break;
		case MANYOWNERSHIP_CHILD_DATA:
								pTempMapOfRelCon = &(m_mapManyOwnershipChildFID);
								break;
		case CONNECTIVITY_NODE1_DATA:
								pTempMapOfRelCon = &(m_mapConNode1FID);
								break;
		case CONNECTIVITY_NODE2_DATA:
								pTempMapOfRelCon = &(m_mapConNode2FID);
	}

	std::map<int, int>::iterator mapItr;
	mapItr = pTempMapOfRelCon->find(nFID);
	if(mapItr == pTempMapOfRelCon->end())		// Check to avoid multiple entry of the feature FID
	{
		pTempMapOfRelCon->insert(std::map<int, int>::value_type(nFID, nFNO));
	}
}





/*
 @Brief			CNSRelationLinker default constructor 
				sets m_dSearchAreaFactor to 1.0
*/
CNSRelationLinker::CNSRelationLinker(void)
{
	m_dSearchAreaFactor = 1.0;
	m_dSearchAreaRadius = 1.0;
	m_dSymbolFactor = 5.0;
}

/*
 @Brief		CNSRelationLinker destructor
*/
CNSRelationLinker::~CNSRelationLinker(void)
{
	// Release m_mapProposedRelations
	CNSProposedRelations *pProposedTempObj;
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr;
	for(mapItr = m_mapProposedRelations.begin(); mapItr != m_mapProposedRelations.end(); mapItr++)
	{
		pProposedTempObj = (*mapItr).second;
		delete pProposedTempObj;
		pProposedTempObj = NULL;
	}
	m_mapProposedRelations.clear();

	// Release m_mapBlocksToBeLinked
	CNSValidationDataCollector *pTempBlockObj;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator blockMapItr;
	for(blockMapItr = m_mapBlocksToBeLinked.begin(); blockMapItr != m_mapBlocksToBeLinked.end(); blockMapItr++)
	{
		pTempBlockObj = (*blockMapItr).second;
		delete pTempBlockObj;
		pTempBlockObj = NULL;
	}
	m_mapBlocksToBeLinked.clear();

	// Release m_mapLinearFeaturesToBeLinked
	CNSValidationDataCollector *pTempCableObj;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator cableMapItr;
	for(cableMapItr = m_mapLinearFeaturesToBeLinked.begin(); cableMapItr != m_mapLinearFeaturesToBeLinked.end(); cableMapItr++)
	{
		pTempCableObj = (*cableMapItr).second;
		delete pTempCableObj;
		pTempCableObj = NULL;
	}
	m_mapLinearFeaturesToBeLinked.clear();

	// Release m_mapExtBlocks
	CNSValidationDataCollector *pTempExtObj;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator extMapItr;
	for(extMapItr = m_mapExtBlocks.begin(); extMapItr != m_mapExtBlocks.end(); extMapItr++)
	{
		pTempExtObj = (*extMapItr).second;
		delete pTempExtObj;
		pTempExtObj = NULL;
	}
	m_mapExtBlocks.clear();
}


/*
 @Brief		Sets the drawing database for the CNSRelationLinker's object
 @Param(in) CNSDWGMgr*
*/
void CNSRelationLinker::setLocalDWGObject(CNSDWGMgr* pLocalDWG)
{
	m_pLocalDWGMgr = pLocalDWG;
}


/*
 @Brief		calls functions that retrieve required features from the drawings, 
			decide and validate relationship and connectivity, and write it to the feature XDATA
 @Param(in)	bool	true	: Proposed-to-Exising relationship
					false	: Proposed-to-Proposed relationship
 @Return	int		NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::link(int nRelFlag, double dSearchAreaFactor, double dSearchAreaRadius, double dSymbolFactor, int LinearConnOnLayerFlag)
{
	setSearchAreaFactor(dSearchAreaFactor/2.00);
	m_dSearchAreaRadius = dSearchAreaRadius;
	m_dSymbolFactor = dSymbolFactor;
	
	bool bProposedToExisting = false;
	if(nRelFlag > 0)// previous code was if(nRelFlag == 1)// this is to generalize both value 1 & 2 for RELATION_FLAG
		bProposedToExisting = true;

	initializeFeatures(bProposedToExisting, LinearConnOnLayerFlag);

	establishRelationship(bProposedToExisting, LinearConnOnLayerFlag);

	validateCables(); //doPostConnectionvalidation
	validateOwnershipParent();
	validateManyOwnershipChilds();
	validateBlockFeatures();
	
	postasXDATA();

	return NS_SUCCESS;
}

/*
 @Brief		This function retrieves features from the drawing
			assigns FNO and FID to these features
			Finds features in proximity and sorts them according to distance
 @Param(in)	bool	true	: Proposed-to-Exising relationship
					false	: Proposed-to-Proposed relationship
 @Return	int		NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::initializeFeatures(bool bProposedToExisting, int LinearConnOnLayerFlag)
{
	// Propsed features  -- NEW and MOD
	m_pLocalDWGMgr->getFeaturesToBeValidated(m_mapBlocksToBeLinked, m_mapLinearFeaturesToBeLinked, true, true, _T("_NEW"));
	// Do not take cables on MODlayers
	m_pLocalDWGMgr->getFeaturesToBeValidated(m_mapBlocksToBeLinked, m_mapLinearFeaturesToBeLinked, true, false, _T("_MOD"));
	if(bProposedToExisting)
	{
		m_pLocalDWGMgr->getFeaturesToBeValidated(m_mapExtBlocks, m_mapExtCablesToBeLinked, true, true, _T("_EXT"));
	}

	asignFNOAndFID();
	removeInvalidFeatures();

	//We need to clear the XData of new features if it already there
	clearExistingXDataForNew();

	searchBusBar();
	assignFeaturesInProximity(LinearConnOnLayerFlag);
	// sort block features in proximity
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator valDataItr;
	for(valDataItr = m_mapBlocksToBeLinked.begin(); valDataItr != m_mapBlocksToBeLinked.end(); valDataItr++)
	{
		CNSValidationDataCollector* pTempFeature = (*valDataItr).second;
		sortFeaturesByDistance(pTempFeature);
	}	

	return NS_SUCCESS;
}

/*
 @Brief		Finds whether a feature is a busbar or not
			A linear feature with ConnectingMethod as 3 is treated as busbar
*/
void CNSRelationLinker::searchBusBar()
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator featuteItr;
	for(featuteItr = m_mapLinearFeaturesToBeLinked.begin();  featuteItr!= m_mapLinearFeaturesToBeLinked.end(); featuteItr++)
	{
		CNSValidationDataCollector *pTempFeature = (*featuteItr).second;
		int nConMethod = nNumberNotFound;
		getConnectionMethod(pTempFeature, nConMethod);
		if(nConMethod == 3)
		{
			pTempFeature->setFeatureType(FEATURE_BUSBAR);
		}
	}

	for(featuteItr = m_mapExtCablesToBeLinked.begin();  featuteItr!= m_mapExtCablesToBeLinked.end(); featuteItr++)
	{
		CNSValidationDataCollector *pTempFeature = (*featuteItr).second;
		int nConMethod = nNumberNotFound;
		getConnectionMethod(pTempFeature, nConMethod);
		if(nConMethod == 3)
		{
			pTempFeature->setFeatureType(FEATURE_BUSBAR);
		}
	}
}

/*
 @Brief			sets m_dSearchAreaFactor
 @Param(double)	double dSearchAreaFactor	
*/
void CNSRelationLinker::setSearchAreaFactor(double dSearchAreaFactor)
{
	m_dSearchAreaFactor = dSearchAreaFactor;
}

/*
 @Brief		Establishes Relationship and Connectivity	
*/
void CNSRelationLinker::establishRelationship(bool bProposedToExisting, int LinearConnOnLayerFlag)
{
	populateProposedRelMap();

	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator blkMapItr;
	for(blkMapItr = m_mapBlocksToBeLinked.begin(); blkMapItr != m_mapBlocksToBeLinked.end(); blkMapItr++)
	{
		CNSValidationDataCollector* pTempFeature = (*blkMapItr).second;
		
		assignOwnershipParentChild(pTempFeature);
		assignOwnershipChild(pTempFeature);
		assignManyOwnershipChilds(pTempFeature);
		assignElectricalConnections(pTempFeature,bProposedToExisting,LinearConnOnLayerFlag);
	}

	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator cableMapItr;
	for(cableMapItr = m_mapLinearFeaturesToBeLinked.begin(); cableMapItr != m_mapLinearFeaturesToBeLinked.end(); cableMapItr++)
	{
		CNSValidationDataCollector* pTempFeature = (*cableMapItr).second;
		
		if(pTempFeature->getFeatureType() == FEATURE_ARC)
			assignOwnershipParentChild(pTempFeature);
		assignManyOwnershipChilds(pTempFeature);
		assignElectricalConnections(pTempFeature,bProposedToExisting,LinearConnOnLayerFlag);
	}
}

/*
 @Brief		Finds features in proximity
*/
void CNSRelationLinker::assignFeaturesInProximity(int LinearConnOnLayerFlag)
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator FeatureItr1, FeatureItr2, FeatureItr3, FeatureItr4, FeatureItr5, FeatureItr6, FeatureItr7, FeatureItr8, FeatureItr9, FeatureItr10;

	for(FeatureItr1 = m_mapBlocksToBeLinked.begin();  FeatureItr1!= m_mapBlocksToBeLinked.end(); FeatureItr1++)
	{
		// Find Block NEW/MOD features in proximity of a NEW/MOD block feature
		for(FeatureItr2 = m_mapBlocksToBeLinked.begin();  FeatureItr2!= m_mapBlocksToBeLinked.end(); FeatureItr2++)
		{
			if((*FeatureItr1).first == (*FeatureItr2).first) // Do not check overlap for self
				continue;
			checkIfBlockWithinBlockBoundary((*FeatureItr1).second, (*FeatureItr2).second);
		}

		// Find NEW Linear features in proximity of a NEW/MOD block feature
		for(FeatureItr3 = m_mapLinearFeaturesToBeLinked.begin();  FeatureItr3!= m_mapLinearFeaturesToBeLinked.end(); FeatureItr3++)
		{
			checkIfBlockWithinBlockBoundary((*FeatureItr1).second, (*FeatureItr3).second);
		}

		// Find EXT features in proximity of a NEW/MOD block feature
		for(FeatureItr7 = m_mapExtBlocks.begin();  FeatureItr7!= m_mapExtBlocks.end(); FeatureItr7++)
		{
			checkIfBlockWithinBlockBoundary((*FeatureItr1).second, (*FeatureItr7).second);
		}

		// Find EXT cables in proximity of a NEW/MOD block feature
		for(FeatureItr9 = m_mapExtCablesToBeLinked.begin();  FeatureItr9!= m_mapExtCablesToBeLinked.end(); FeatureItr9++)
		{
			checkIfBlockWithinBlockBoundary((*FeatureItr1).second, (*FeatureItr9).second);
		}
	}
	
	for(FeatureItr4 = m_mapLinearFeaturesToBeLinked.begin();  FeatureItr4!= m_mapLinearFeaturesToBeLinked.end(); FeatureItr4++)
	{
		// Find Liner features in proximity of a linear feature
		for(FeatureItr5 = m_mapLinearFeaturesToBeLinked.begin();  FeatureItr5!= m_mapLinearFeaturesToBeLinked.end(); FeatureItr5++)
		{
			if((*FeatureItr4).first == (*FeatureItr5).first) // Do not check overlap for self
				continue;
			checkIfBlockWithinBlockBoundary((*FeatureItr4).second, (*FeatureItr5).second);
		}

		// Find NEW/MOD Block features in proximity of a linear feature
		for(FeatureItr6 = m_mapBlocksToBeLinked.begin();  FeatureItr6!= m_mapBlocksToBeLinked.end(); FeatureItr6++)
		{
			if((*FeatureItr4).first == (*FeatureItr6).first) // Do not check overlap for self
				continue;
			checkIfBlockWithinBlockBoundary((*FeatureItr4).second, (*FeatureItr6).second);
		}

		// Find EXT features in proximity of a linear feature
		for(FeatureItr8 = m_mapExtBlocks.begin();  FeatureItr8!= m_mapExtBlocks.end(); FeatureItr8++)
		{
			checkIfBlockWithinBlockBoundary((*FeatureItr4).second, (*FeatureItr8).second);
		}

		// Find EXT cables in proximity of a linear feature
		for(FeatureItr10 = m_mapExtCablesToBeLinked.begin();  FeatureItr10!= m_mapExtCablesToBeLinked.end(); FeatureItr10++)
		{
			checkIfBlockWithinBlockBoundary((*FeatureItr4).second, (*FeatureItr10).second, LinearConnOnLayerFlag);
		}
	}
}


/*
@Brief			In following cases pFeature is First entity and pFeatureToBeLinked is Second entity
				The function determines if pFeatureToBeLinked is in proximity of pFeature
				case 1 Block : Block
				case 2 Block : Start(Insertion) Point of linear feature
				case 3 Block : End Point of linear feature
				case 4 Insertion Point of linear feature : Block
				case 5 Insertion Point of linear feature : Insertion Point of linear feature
				case 6 Insertion Point of linear feature : End Point of linear feature
				case 7 End Point of linear feature : Block
				case 8 End Point of linear feature : Insertion Point of linear feature
				case 9 End Point of linear feature : End Point of linear feature
				case 10 BusBar : a linear feature or a block feature
				case 11 Polyline : a block feature on polyline vertices (other than the start and end points of the poly lines)
				case 12 Arc and linear feature connection, Arc centre point will be connection point
				case 13 Arc and block feature, Arc centre point and block insertion point to be considered
 @Param(out)	CNSValidationDataCollector* pFeature
 @Param(out)	CNSValidationDataCollector* pFeatureToBeLinked
*/
void CNSRelationLinker::checkIfBlockWithinBlockBoundary(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked, int LinearConnOnLayerFlag)
{
	//case 11
	if((FEATURE_LINEAR == pFeature->getFeatureType()) && pFeature->bPolyLine && (FEATURE_BLOCK == pFeatureToBeLinked->getFeatureType()))
	{
		checkOnPolylineVertices(pFeature, pFeatureToBeLinked);
		//return;
	}
	
	//case 10
    // As per client request, for BUSBAR objects connectivity is not required. Hence
    // this code is commented out.
	if(FEATURE_BUSBAR == pFeature->getFeatureType())
	{
		//checkInBusBarBoundary(pFeature, pFeatureToBeLinked);
		return;
	}

	//case 12
	if((FEATURE_ARC == pFeature->getFeatureType() && FEATURE_LINEAR == pFeatureToBeLinked->getFeatureType())
		||(FEATURE_LINEAR == pFeature->getFeatureType() && FEATURE_ARC == pFeatureToBeLinked->getFeatureType()))
	{
		checkInCircularArea(pFeature, pFeatureToBeLinked,LinearConnOnLayerFlag);
		return;
	}
	else if((FEATURE_ARC == pFeature->getFeatureType() && FEATURE_BLOCK == pFeatureToBeLinked->getFeatureType())
		||(FEATURE_BLOCK == pFeature->getFeatureType() && FEATURE_ARC == pFeatureToBeLinked->getFeatureType()))
	{
		checkIfBlockInCircularArea(pFeature, pFeatureToBeLinked);
		return;
	}

	double dSearchAreaFactor = m_dSearchAreaFactor;
	if(FEATURE_BLOCK == pFeature->getFeatureType() && FEATURE_BLOCK == pFeatureToBeLinked->getFeatureType())
	{
		//Provide the ability for the validation to ignore spatial creation of parent/child relationships
		//through attribute settings. eg: Substation switchgear is rarely drawn close enough for
		//validation to correctly determine the parent/child relation, thus provide a mechanism where
		//the user can manually define the relationship through attributes. Eg: PARENT attribute
		//currently used for Asset Numbering.
		
		//This if condition commented on 03-May-2014, for solving performance issue
		/////////////////////////////////if(checkBlocksBasedOnAttribites(pFeature, pFeatureToBeLinked))
		/////////////////////////////////{
			pFeature->m_vectFeaturesInProximityByCmnAttVal.push_back(pFeatureToBeLinked->getObjectId());
			//pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//return;
		/////////////////////////////////}
		dSearchAreaFactor = m_dSymbolFactor;
	}

	CNSPoint3D ptpFeatureInsertionPoint = pFeature->getInsertionPoint();
	//CNSPoint3D ptpFeatureScaleFactor = pFeature->getScaleFactor();
	CNSPoint3D ptpFeatureScaleFactor(dSearchAreaFactor, dSearchAreaFactor);

	CNSPoint3D ptLowerLeftInsr((ptpFeatureInsertionPoint.m_dXX - ptpFeatureScaleFactor.m_dXX), (ptpFeatureInsertionPoint.m_dYY - ptpFeatureScaleFactor.m_dYY));
	CNSPoint3D ptTopRightInsr((ptpFeatureInsertionPoint.m_dXX + ptpFeatureScaleFactor.m_dXX), (ptpFeatureInsertionPoint.m_dYY + ptpFeatureScaleFactor.m_dYY));
		
	CNSPoint3D ptTestIP = pFeatureToBeLinked->getInsertionPoint();

	// Covers case 1, case 2, case 4 and case 5
	if((ptTestIP.m_dXX >= ptLowerLeftInsr.m_dXX) && (ptTestIP.m_dYY >= ptLowerLeftInsr.m_dYY) && (ptTestIP.m_dXX <= ptTopRightInsr.m_dXX) && (ptTestIP.m_dYY <= ptTopRightInsr.m_dYY))
	{
		pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
	}

	// Covers case 3, case 6
	if(FEATURE_LINEAR == pFeatureToBeLinked->getFeatureType())
	{
		CNSPoint3D ptTestEP = pFeatureToBeLinked->getEndPoint();
		if((ptTestEP.m_dXX >= ptLowerLeftInsr.m_dXX) && (ptTestEP.m_dYY >= ptLowerLeftInsr.m_dYY) && (ptTestEP.m_dXX <= ptTopRightInsr.m_dXX) && (ptTestEP.m_dYY <= ptTopRightInsr.m_dYY))
		{
			pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
		}
	}


	if(FEATURE_LINEAR == pFeature->getFeatureType())
	{
		CNSPoint3D ptpFeatureEndPoint = pFeature->getEndPoint();
		CNSPoint3D ptLowerLeftEnd((ptpFeatureEndPoint.m_dXX - ptpFeatureScaleFactor.m_dXX), (ptpFeatureEndPoint.m_dYY - ptpFeatureScaleFactor.m_dYY));
		CNSPoint3D ptTopRightEnd((ptpFeatureEndPoint.m_dXX + ptpFeatureScaleFactor.m_dXX), (ptpFeatureEndPoint.m_dYY + ptpFeatureScaleFactor.m_dYY));
		
		// Covers case 7 and case 8
		if((ptTestIP.m_dXX >= ptLowerLeftEnd.m_dXX) && (ptTestIP.m_dYY >= ptLowerLeftEnd.m_dYY) && (ptTestIP.m_dXX <= ptTopRightEnd.m_dXX) && (ptTestIP.m_dYY <= ptTopRightEnd.m_dYY))
		{
			pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
		}

		// Covers case 9
		if(FEATURE_LINEAR == pFeatureToBeLinked->getFeatureType())
		{
			CNSPoint3D ptTestEP = pFeatureToBeLinked->getEndPoint();
			if((ptTestEP.m_dXX >= ptLowerLeftEnd.m_dXX) && (ptTestEP.m_dYY >= ptLowerLeftEnd.m_dYY) && (ptTestEP.m_dXX <= ptTopRightEnd.m_dXX) && (ptTestEP.m_dYY <= ptTopRightEnd.m_dYY))
			{
				pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			}
		}
	}
}

/*
 @Brief			finds features in proximity of a busbar
 @Param(in)		CNSValidationDataCollector		busbar feature
 @Param(in)		CNSValidationDataCollector		feature in busbar proximity
*/
void CNSRelationLinker::checkInBusBarBoundary(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked)
{
	if(FEATURE_BUSBAR != pFeature->getFeatureType())
		return;
	
	bool bWithinLineSegment = false;
	double dDistance;

	dDistance = getPointToLineDistance(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint(), bWithinLineSegment);
	if((dDistance <= m_dSearchAreaFactor) && (bWithinLineSegment == true))
	{
		pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
		
		if(FEATURE_BUSBAR == pFeatureToBeLinked->getFeatureType())
			pFeatureToBeLinked->m_vectFeaturesInProximityAtNode2.push_back(pFeature->getObjectId());
		else
			pFeatureToBeLinked->m_vectFeaturesInProximity.push_back(pFeature->getObjectId());

	}

	// Find if end point of a linear feature is in the proximity of a busbar feature
	if(FEATURE_LINEAR == pFeatureToBeLinked->getFeatureType())
	{
		bWithinLineSegment = false;
		double dDistanceFromEndPoint = getPointToLineDistance(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint(), bWithinLineSegment);
		if((dDistanceFromEndPoint <= m_dSearchAreaFactor) && (bWithinLineSegment == true))
		{
			pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			pFeatureToBeLinked->m_vectFeaturesInProximityAtNode2.push_back(pFeature->getObjectId());
		}
	}
}

double CNSRelationLinker::getSquareDist(CNSPoint3D pt1, CNSPoint3D pt2)
{
	return ((pt1.m_dXX - pt2.m_dXX) * (pt1.m_dXX - pt2.m_dXX) +
						 (pt1.m_dYY - pt2.m_dYY) * (pt1.m_dYY - pt2.m_dYY));
}

/*
 @Brief			Checks if point in circle area 
 @Param(in)		Circle center point, point to be checked ,radius, search factor
 @Param(out)	true if within area, else false	
*/
bool CNSRelationLinker::checkPointInCircle(CNSPoint3D centrePt, CNSPoint3D givenPt, double dRadius, double dSearchFactor)
{
	//calculate square 2D distance between the two points
	double dDistSquare = getSquareDist(centrePt, givenPt);
	if(dDistSquare > (dSearchFactor * dSearchFactor * dRadius * dRadius))
		return false;
	return true;
}

/*
 @Brief			Checks if point in circle area 
 @Param(in)		Circle center point, point to be checked ,radius, search factor
 @Param(out)	true if within area, else false	
*/
bool CNSRelationLinker::checkPointOnLine(CNSPoint3D startPt, CNSPoint3D endPt, CNSPoint3D givenPt)
{
	double dStartDistSq = getSquareDist(startPt, givenPt);
	double dEndDistSq = getSquareDist(endPt, givenPt);
	double dLineDistSq = getSquareDist(startPt, endPt);
	double dTempDist = sqrt(dStartDistSq) + sqrt(dEndDistSq);  
	if(fabs(fabs(sqrt(dLineDistSq))  - fabs(dTempDist)) < gTolerance)
		return true;
	return false;
}


/*
 @Brief			checks if the layers are having same usage
 @date			7.11.09
*/

bool CNSRelationLinker::CheckIfUsageIsMatching(CNSValidationDataCollector* pFeature1, CNSValidationDataCollector* pFeature2 )
{	
	if(getUsage(pFeature1) == getUsage(pFeature2) )
		return true;
	else
		return false;
}

NSSTRING CNSRelationLinker::getUsage(CNSValidationDataCollector* pFeature)
{
	NSSTRING strLayType = getLayerType(pFeature->getLayerName());

	TCHAR szQuery[MAX_PATH+1];

	if(pFeature->getFNO() != nNumberNotFound)
	{
		if(strLayType == _T("NEW"))
		{
			NSSTRING strLayer = getQueryString(pFeature->getLayerName());
			NSSPRINTF(szQuery, _T("Select Usage from NewLinearFeatures where FNO = %i and NewLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}

		if(strLayType == _T("EXT"))
		{
			NSSTRING strLayer = getQueryString(pFeature->getLayerName());
			NSSPRINTF(szQuery, _T("Select Usage from LinearFeatures where FNO = %d and ExtLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}
		
	}
	else
		return _T("");
	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	variant_t vUsage;
	bool bUsage;
	if(nDBRet != -1)
	{
		bUsage = CNSDatabaseMgr::getInstance()->getValue(_T("Usage"), vUsage);
	}
	
	NSSTRING strUsage ;
	if(bUsage)
	{
		#ifndef _INTERNAL_
			strUsage = vUsage.bstrVal;
		#else
			strUsage = vUsage.bstrVal;
		/*	char* strLayer = _com_util::ConvertBSTRToString(vUsage.bstrVal);
			strUsage = strLayer;*/
		#endif

		return strUsage;
	}


	return _T("");
}

NSSTRING CNSRelationLinker::getLayerType(NSSTRING strLayName)
{
	int nIndex1 = m_pLocalDWGMgr->rFindString(strLayName, _T("_NEW"));
		if (nIndex1 != -1 )//-1
			return _T("NEW");

	int nIndex2 = m_pLocalDWGMgr->rFindString(strLayName, _T("_EXT"));
		if (nIndex2 != -1 )//-1
			return _T("EXT");	

	return _T("");
}


/*
 @Brief			finds features in proximity of a bond arc 
 @Param(in)		CNSValidationDataCollector		arc feature
 @Param(in)		CNSValidationDataCollector		feature in arc proximity
*/
void CNSRelationLinker::checkInCircularArea(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked, int LinearConnOnLayerFlag)
{
	//In the new implementation of Arc and Linear feature connectivity, 
	//application will establish connectivity between a polyline (cable feature) and 
	//an arc (bond arc feature) based on their layer names. That means, 
	
	//i.	Application will trim the layer name by omitting the suffix after _NEW 
	//		( example: If layer name for cable is MAINS_LV_NEW_C27 and 
	//		for bond is MAINS_LV_NEW_HardWired, then application will remove _C27 and  _HardWired  from these layers respectively. 

	//ii.	Application will then try to match the above layer name parts and 
	//		if they are found matching it will establish connectivity between cable and arc. 
	//		In above example, connectivity will be established as layer name parts are matching. 

	int nIndex1 = m_pLocalDWGMgr->rFindString(pFeature->getLayerName(), _T("_NEW"));
	NSSTRING strTemp1 = pFeature->getLayerName();
	NSSTRING strComp1;
	if((int)strTemp1.size() >= nIndex1) 
		strComp1 = strTemp1.substr(0, nIndex1);  //Trim the name of this feature
	else return;

	int nIndex2 = m_pLocalDWGMgr->rFindString(pFeatureToBeLinked->getLayerName(), _T("_NEW"));
	NSSTRING strTemp2 = pFeatureToBeLinked->getLayerName();
	NSSTRING strComp2;
	if((int)strTemp2.size() >= nIndex2) 
		strComp2 = strTemp2.substr(0, nIndex2);	//Trim the name of the feature to be linked
	else return;

	if(LinearConnOnLayerFlag == 0)
	{		
		if(NSSTRCMP(strComp1.c_str(), strComp2.c_str()) != 0) return; //No match, then return
	}
	else
	{
		bool bUsageMatched = CheckIfUsageIsMatching(pFeature,pFeatureToBeLinked);

		if(!bUsageMatched)
			return;
	}

	//Below we consider two condition as the ARC with LINEAR feature and
	//LINEAR with ARC connectivity should be treated as diferent ones
	bool bInCircle = false;
	if(pFeature->getFeatureType() == FEATURE_ARC && pFeatureToBeLinked->getFeatureType() == FEATURE_LINEAR)
	{
		CNSCircle  *pCircle = pFeature->getCircle();
		if(pCircle == NULL) return;
		
		//A new configuration variable SEARCH_RADIUS will be provided in ini file 
		//and its value will be used to find the cable end point in the vicinity of the arc end point. 
		//The connectivity is to be established by considering center point of arc

		//Check if the point in lies in Search area.
		
		if(true == checkPointInCircle(pCircle->m_Centre, pFeatureToBeLinked->getInsertionPoint(), pCircle->m_dRadius, m_dSearchAreaRadius))
		{
			bInCircle = true;

			//if(getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getInsertionPoint())
			//	- getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint()) < gTolerance)
			//{
			//	if(getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getEndPoint())
			//		< getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint()))
			//	{
			//		pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//	}
			//	else
			//	{
			//		pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			//	}
			//}
			////Check the start or end point of arc which is nearer to connect
			//else if(getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getInsertionPoint())
			//	< getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint()))
			//{
			//	pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//}
			//else
			//{
			//	pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			//}
		}
		else if(true == checkPointInCircle(pCircle->m_Centre, pFeatureToBeLinked->getEndPoint(), pCircle->m_dRadius, m_dSearchAreaRadius))
		{
			bInCircle = true;

			//	if(getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getEndPoint())
			//	- getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint()) < gTolerance)
			//{
			//	if(getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getInsertionPoint())
			//		< getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint()))
			//	{
			//		pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//	}
			//	else
			//	{
			//		pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			//	}
			//}
			//else if(getSquareDist(pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint())
			//	< getSquareDist(pFeature->getInsertionPoint(), pFeatureToBeLinked->getEndPoint()))
			//{
			//	pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			//}
			//else
			//{
			//	pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//}
		}
		//Check if arc's start point lies on the line, if yes add to Node1
		if(bInCircle == true)
		{
			if(checkPointOnLine(pFeatureToBeLinked->getInsertionPoint(), pFeatureToBeLinked->getEndPoint(), pFeature->getInsertionPoint()))
			{
				pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			}
			//Check if arc's end point lies on the line, if yes add to Node2
			else if(checkPointOnLine(pFeatureToBeLinked->getInsertionPoint(), pFeatureToBeLinked->getEndPoint(), pFeature->getEndPoint()))
			{
				pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			}
		}
	}
	else if(pFeature->getFeatureType() == FEATURE_LINEAR && pFeatureToBeLinked->getFeatureType() == FEATURE_ARC)
	{
		CNSCircle  *pCircle = pFeatureToBeLinked->getCircle();
		if(pCircle == NULL) return;

		//check if lines insertion point in circular area
		if(true == checkPointInCircle(pCircle->m_Centre, pFeature->getInsertionPoint(), pCircle->m_dRadius, m_dSearchAreaRadius))
		{
			//check if the start point of the arc really lies on the line, if not ignore it
			if(checkPointOnLine(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint())
				|| checkPointOnLine(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint()))
			{
				pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			}
		}
		//check if lines end point in circular area
        else if(true == checkPointInCircle(pCircle->m_Centre, pFeature->getEndPoint(), pCircle->m_dRadius, m_dSearchAreaRadius))
		{
			//check if the end point of the arc really lies on the line, if not ignore it
           	if(checkPointOnLine(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getInsertionPoint())
				|| checkPointOnLine(pFeature->getInsertionPoint(), pFeature->getEndPoint(), pFeatureToBeLinked->getEndPoint()))
			{
				pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			}
		}
	}
}

/*
 @Brief			finds features in proximity of a bond arc 
 @Param(in)		CNSValidationDataCollector		arc feature
 @Param(in)		CNSValidationDataCollector		feature in arc proximity
*/
void CNSRelationLinker::checkIfBlockInCircularArea(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked)
{
	CNSValidationDataCollector *pArcFeature = NULL;
	CNSValidationDataCollector *pLinearFeature = NULL;

	CNSCircle  *pCircle = NULL;
	CNSPoint3D pt;
	if((pFeature->getFeatureType() == FEATURE_ARC) && (pFeatureToBeLinked->getFeatureType() == FEATURE_BLOCK))
	{
		pCircle = pFeature->getCircle();
		pt = pFeatureToBeLinked->getInsertionPoint();
	}
	else if((pFeature->getFeatureType() == FEATURE_BLOCK) && (pFeatureToBeLinked->getFeatureType() == FEATURE_ARC))
	{
		pCircle = pFeatureToBeLinked->getCircle();
		pt = pFeature->getInsertionPoint();
	}
	else
	{
		return;
	}
	if(!pCircle) return;
	
	
	if(true == checkPointInCircle(pCircle->m_Centre, pt, pCircle->m_dRadius, m_dSearchAreaRadius))
	{
		pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
	}
}

/*
 @Brief			finds block features in proximity of polyline vertices
 @Param(in)		CNSValidationDataCollector		polyline feature
 @Param(in)		CNSValidationDataCollector		feature in polyline proximity
*/
void CNSRelationLinker::checkOnPolylineVertices(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked)
{
	std::vector<CNSPoint3D*> vectVertices;
	pFeature->getPolylineVertices(vectVertices);

	std::vector<CNSPoint3D*>::iterator vectItr;
	for(vectItr = vectVertices.begin(); vectItr != vectVertices.end(); vectItr++)
	{
		CNSPoint3D polyineVertex = **vectItr;
		//CNSPoint3D ptpFeatureScaleFactor = pFeature->getScaleFactor();
		CNSPoint3D ptpFeatureScaleFactor(m_dSearchAreaFactor, m_dSearchAreaFactor);
		CNSPoint3D ptLowerLeftInsr((polyineVertex.m_dXX - ptpFeatureScaleFactor.m_dXX), (polyineVertex.m_dYY - ptpFeatureScaleFactor.m_dYY));
		CNSPoint3D ptTopRightInsr((polyineVertex.m_dXX + ptpFeatureScaleFactor.m_dXX), (polyineVertex.m_dYY + ptpFeatureScaleFactor.m_dYY));
			
		CNSPoint3D ptTestIP = pFeatureToBeLinked->getInsertionPoint();

		if((ptTestIP.m_dXX >= ptLowerLeftInsr.m_dXX) && (ptTestIP.m_dYY >= ptLowerLeftInsr.m_dYY) && (ptTestIP.m_dXX <= ptTopRightInsr.m_dXX) && (ptTestIP.m_dYY <= ptTopRightInsr.m_dYY))
		{
			//if(vectItr == vectVertices.begin())
			//{
			//	pFeature->m_vectFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			//	pFeatureToBeLinked->m_vectFeaturesInProximity.push_back(pFeature->getObjectId());
			//}

			//if(vectItr == vectVertices.begin())
			//{
			//	pFeature->m_vectFeaturesInProximityAtNode2.push_back(pFeatureToBeLinked->getObjectId());
			//	pFeatureToBeLinked->m_vectFeaturesInProximity.push_back(pFeature->getObjectId());
			//}

			pFeature->m_vectInterPolyFeaturesInProximity.push_back(pFeatureToBeLinked->getObjectId());
			pFeatureToBeLinked->m_vectInterPolyFeaturesInProximity.push_back(pFeature->getObjectId());
		}
	}
}

/*
 @Brief			gets the shortest distance between a point and a line
 @Param(in)		CNSPoint3D		first point of a line
 @Param(in)		CNSPoint3D		second point of a line
 @Param(in)		CNSPoint3D		any point in the plane of line say C
 @Param(out)	bool			say P is the point on AB closest 
*/
double CNSRelationLinker::getPointToLineDistance(CNSPoint3D ptA, CNSPoint3D ptB, CNSPoint3D ptC, bool &bWithinLineSegment)
{
	double dAx = ptA.m_dXX;
	double dAy = ptA.m_dYY;
	double dBx = ptB.m_dXX;
	double dBy = ptB.m_dYY;
	double dCx = ptC.m_dXX;
	double dCy = ptC.m_dYY;
	double dDistanceSegment, dDistanceLine;

//    Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
//    Let P be the point of perpendicular projection of C on AB.  The parameter
//    dR, which indicates P's position along AB, is computed dBy the dot product 
//    of AC and AB divided dBy the square of the length of AB:
//    
//    (1)     AC dot AB
//        dR = ---------  
//            ||AB||^2
//    
//    dR has the following meaning:
//    
//        dR=0      P = A
//        dR=1      P = B
//        dR<0      P is on the backward extension of AB
//        dR>1      P is on the forward extension of AB
//        0<dR<1    P is interior to AB
//    
//    The length of a line segment in d dimensions, AB is computed dBy:
//    
//        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 + ... + (Bd-Ad)^2)
//
//    so in 2D:   
//    
//        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )
//    
//    and the dot product of two vectors in d dimensions, U dot V is computed:
//    
//        D = (Ux * Vx) + (Uy * Vy) + ... + (Ud * Vd)
//    
//    so in 2D:   
//    
//        D = (Ux * Vx) + (Uy * Vy) 
//    
//    So (1) expands to:
//    
//            (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
//        dR = -------------------------------
//                          L^2
//
//    The point P can then be found:
//
//        Px = Ax + dR(Bx-Ax)
//        Py = Ay + dR(By-Ay)
//
//    And the distance from A to P = dR*L.
//
//    Use another parameter s to indicate the location along PC, with the 
//    following meaning:
//           s<0      C is left of AB
//           s>0      C is right of AB
//           s=0      C is on AB
//
//    Compute s as follows:
//
//            (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
//        s = -----------------------------
//                        L^2
//
//    Then the distance from C to P = |s|*L.

	double dRNumerator = (dCx-dAx)*(dBx-dAx) + (dCy-dAy)*(dBy-dAy);
	double dRDenomenator = (dBx-dAx)*(dBx-dAx) + (dBy-dAy)*(dBy-dAy);
	double dR = dRNumerator / dRDenomenator;

    double dPx = dAx + dR*(dBx-dAx);
    double dPy = dAy + dR*(dBy-dAy);
     
    double s =  ((dAy-dCy)*(dBx-dAx)-(dAx-dCx)*(dBy-dAy) ) / dRDenomenator;

	dDistanceLine = fabs(s)*sqrt(dRDenomenator);

// (dXX,dyy) is the point on the lineSegment closest to (dCx,dCy)

	double dXX = dPx;
	double dyy = dPy;

	if ( (dR >= 0) && (dR <= 1) )
	{
		dDistanceSegment = dDistanceLine;
		bWithinLineSegment = true;
	}
	else
	{
		bWithinLineSegment = false;
		double dDist1 = (dCx-dAx)*(dCx-dAx) + (dCy-dAy)*(dCy-dAy);
		double dDist2 = (dCx-dBx)*(dCx-dBx) + (dCy-dBy)*(dCy-dBy);
		if (dDist1 < dDist2)
		{
			dXX = dAx;
			dyy = dAy;
			dDistanceSegment = sqrt(dDist1);
		}
		else
		{
			dXX = dBx;
			dyy = dBy;
			dDistanceSegment = sqrt(dDist2);
		}
	}
	return dDistanceSegment;
}

/*
 @Brief		assigns FID to new features
			retrieves FNO for each entry of CNSValidationDataCollector from MDB
 @Return	NS_FAIL/NS_SUCCESS
*/
int  CNSRelationLinker::asignFNOAndFID()
{
	int nFID = -1;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator blocksToBeLinkedItr, cablesToBeLinkedItr, extBlockItr, extCableItr;
	CNSValidationDataCollector* pValidationDataCollectorObj;

	for(blocksToBeLinkedItr = m_mapBlocksToBeLinked.begin(); blocksToBeLinkedItr != m_mapBlocksToBeLinked.end(); blocksToBeLinkedItr++)
	{
		pValidationDataCollectorObj = (*blocksToBeLinkedItr).second;
		if(STATUS_MOD == pValidationDataCollectorObj->getFeatureStatus())
		{
			NSSTRING strTempFID(pValidationDataCollectorObj->readXDATA(_T("FID")).c_str());
			if(NSSTRCMP(strTempFID.c_str(), _T("")) != 0)
			{
				pValidationDataCollectorObj->setFID(NSATOI(strTempFID.c_str()));
			}
		}

		int nFNO = nNumberNotFound;
		retrieveFNOFromDatabase(pValidationDataCollectorObj, nFNO);
		pValidationDataCollectorObj->setFNO(nFNO);

		if(nFNO == nNumberNotFound)
			continue;


		if(STATUS_NEW == pValidationDataCollectorObj->getFeatureStatus())
		{
			pValidationDataCollectorObj->setFID(nFID);
			nFID--;
		}
		
		
	}

	for(cablesToBeLinkedItr = m_mapLinearFeaturesToBeLinked.begin(); cablesToBeLinkedItr != m_mapLinearFeaturesToBeLinked.end(); cablesToBeLinkedItr++)
	{
		pValidationDataCollectorObj = (*cablesToBeLinkedItr).second;
		pValidationDataCollectorObj->setFID(nFID);
		nFID--;		
		
		int nFNO = nNumberNotFound;
		retrieveFNOFromDatabase(pValidationDataCollectorObj, nFNO);
		pValidationDataCollectorObj->setFNO(nFNO);
	}

	for(extBlockItr = m_mapExtBlocks.begin(); extBlockItr != m_mapExtBlocks.end(); extBlockItr++)
	{
		pValidationDataCollectorObj = (*extBlockItr).second;
		pValidationDataCollectorObj->setFID(NSATOI((pValidationDataCollectorObj->readXDATA(_T("FID")).c_str())));
		pValidationDataCollectorObj->setFNO(NSATOI((pValidationDataCollectorObj->readXDATA(_T("FNO")).c_str())));			
	}

	for(extCableItr = m_mapExtCablesToBeLinked.begin(); extCableItr != m_mapExtCablesToBeLinked.end(); extCableItr++)
	{
		pValidationDataCollectorObj = (*extCableItr).second;
		pValidationDataCollectorObj->setFID(NSATOI((pValidationDataCollectorObj->readXDATA(_T("FID")).c_str())));
		pValidationDataCollectorObj->setFNO(NSATOI((pValidationDataCollectorObj->readXDATA(_T("FNO")).c_str())));
	}

	return NS_SUCCESS;
}

/*
 @Brief			This function sorts features in proximity by distance
 @Param(out)	CNSValidationDataCollector* ValidationDataCollectorObj
 @Return		int		NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::sortFeaturesByDistance(CNSValidationDataCollector* ValidationDataCollectorObj)
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator BlocksToBeLinkedItr;
	std::vector<double> vectDist;

	//Get the vectors of objectids from the CNSValidationDataCollector class
	std::vector<LONG_PTR> vectFeaturesInProximity = ValidationDataCollectorObj->m_vectFeaturesInProximity;
	std::vector<LONG_PTR>::iterator ObjIdIter;
	
	//Create a temporary object
	CNSValidationDataCollector* pVDCollectorObj;

	//Get the position of the main object from where distance of other objects has to be taken
	CNSPoint3D Mainpoint =  ValidationDataCollectorObj->getInsertionPoint();
	double dXMainPt = Mainpoint.m_dXX;
	double dYMainPt = Mainpoint.m_dYY;

	//Get the distance by searching the objectid in the map and then getting their position 
	//and calculating the distance and storing the distance values in a vector
	for(ObjIdIter = vectFeaturesInProximity.begin(); ObjIdIter != vectFeaturesInProximity.end(); ObjIdIter++)
	{
		LONG_PTR lObjId = *ObjIdIter;
		BlocksToBeLinkedItr = m_mapBlocksToBeLinked.find(lObjId);
		if(BlocksToBeLinkedItr == m_mapBlocksToBeLinked.end())
		{
			return NS_FAIL;
		}
		else
		{
			pVDCollectorObj = (*BlocksToBeLinkedItr).second;
			CNSPoint3D localPoint = pVDCollectorObj->getInsertionPoint();
			double dXLocalPt = localPoint.m_dXX;
			double dYLocalPt = localPoint.m_dYY;
			double dDeltaX = (dXLocalPt - dXMainPt) * (dXLocalPt - dXMainPt);
			double dDeltaY = (dYLocalPt - dYMainPt) * (dYLocalPt - dYMainPt);
			double dDistance = dDeltaX + dDeltaY;
			dDistance = sqrt(dDistance);

			vectDist.push_back(dDistance);
		}
	}

	//Sort the Objectid by sorting the distance vector in ascending order
	std::vector<double>::iterator DistIter;
	LONG_PTR lTmp;
	double dTmpDist;
	int nflag = 1;
	int nSize = (int)vectDist.size();
	for (int nVectCnt1 = 1; nVectCnt1 <= nSize; nVectCnt1++)  
	{
		nflag = 0;
		for (int nVectCnt2 = 0; nVectCnt2 < nSize-1; nVectCnt2++) 
		{
			if (vectDist[nVectCnt2+1] < vectDist[nVectCnt2])
			{
				dTmpDist = vectDist[nVectCnt2];
				vectDist[nVectCnt2] = vectDist[nVectCnt2+1];
				vectDist[nVectCnt2+1] = dTmpDist;

				lTmp = ValidationDataCollectorObj->m_vectFeaturesInProximity[nVectCnt2];
				ValidationDataCollectorObj->m_vectFeaturesInProximity[nVectCnt2] = ValidationDataCollectorObj->m_vectFeaturesInProximity[nVectCnt2+1];
				ValidationDataCollectorObj->m_vectFeaturesInProximity[nVectCnt2+1] = lTmp;
				nflag = 1;
			}
		}
     }
	return NS_SUCCESS;
}

/*
 @Brief			used to bulid query string for retrieving FNO from MDB
 @Param(out)	NSSTRING_MAP
 @Param(in)		TCHAR			Query string	
*/
NSSTRING CNSRelationLinker::getQueryString(NSSTRING_MAP & mapAttributes, TCHAR szQuery[_MAX_PATH])
{
	NSSTRING strQuery(szQuery);
	std::map<NSSTRING, NSSTRING> mapofColumns;
	std::map<NSSTRING, NSSTRING>::iterator mapItr;
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SUBSTATION_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("SWITCH_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CCP_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("LANTERN_TYPE"), _T("")));
	mapofColumns.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ACTUAL_STATUS"), _T("")));

	NSSTRING_MAP::iterator itr;
	for(itr = mapAttributes.begin(); itr != mapAttributes.end(); itr++)
	{
		NSSTRING strAttName = (*itr).first;
		#ifndef _INTERNAL_
			NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
		#else
		    NSSTRUPR((TCHAR*)strAttName.c_str(), strAttName.size()+1);
			//NSSTRUPR((TCHAR*)strAttName.c_str());
		#endif		
		mapItr = mapofColumns.find(strAttName);
		if(mapItr != mapofColumns.end())
		{
			NSSTRING strAttValue = getQueryString((*itr).second);
			TCHAR szTemp[_MAX_PATH + 1];
			NSSPRINTF(szTemp, _T(" and %s %s"), strAttName.c_str(), strAttValue.c_str());
			strQuery.append(szTemp);
		}

	}
	return strQuery;
}

int findString(NSSTRING strBaseString, NSSTRING strStringToBeFind)
{
	#ifndef _INTERNAL_
		NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );	
	#else
		NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );	
		/*NSSTRUPR((TCHAR*)strBaseString.c_str());
		NSSTRUPR((TCHAR*)strStringToBeFind.c_str());*/	
	#endif
	return (int)strBaseString.find(strStringToBeFind);
}

NSSTRING CNSRelationLinker::getUsageOrntBlkName(CNSValidationDataCollector* pFeature)
{
	NSSTRING strBlockColumn;
	NSSTRING strUsage, strOrientation, strBlockName;
	strBlockName = pFeature->getBlockName();
	size_t foundNew;
	foundNew = findString(strBlockName, _T("NEW"));
	//foundNew = strBlockName.find(_T("NEW"));
	if(foundNew!=NSSTRING::npos)
	{
		strBlockColumn =  _T("NewBlock");
		strUsage = pFeature->readAttribute(_T("USAGE"));
		strOrientation = pFeature->readAttribute(_T("ORIENTATION"));
	}
	else
	{
		size_t foundExt;
		foundExt = findString(strBlockName, _T("NEW"));
		//foundExt = strBlockName.find(_T("EXT"));
		if(foundExt != NSSTRING::npos)
		{
			strBlockColumn =  _T("ExtBlock");
			strUsage = pFeature->readXDATA(_T("USAGE"));
			strOrientation = pFeature->readXDATA(_T("ORIENTATION"));
		}
		//else 
		//	return NS_FAIL;
	}

	NSSTRING strFinalUsage = getQueryString(strUsage);
	NSSTRING strFinalOrientation = getQueryString(strOrientation);

	TCHAR szQuery[MAX_PATH+1];
	NSSPRINTF(szQuery, _T("%S = \'%S\' and USAGE %S and ORIENTATION %S"),
	strBlockColumn.c_str(), strBlockName.c_str(), strFinalUsage.c_str(), strFinalOrientation.c_str());
	NSSTRING strUsageOrntBlkNmae = szQuery;
	return strUsageOrntBlkNmae;
}

/*
 @Brief			Takes FNO from the Database for pFeature
 @Param(in)		CNSValidationDataCollector* pFeature
 @Param(out)	int *nFNO
 @Return		int	NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::retrieveFNOFromDatabase(CNSValidationDataCollector* pFeature, int &nFNO)
{
	bool bFNO = false;
	int nDBRet = -1;
	TCHAR szQuery[MAX_PATH+1];
	if(FEATURE_BLOCK == pFeature->getFeatureType())
	{
		//// To get FNO we need usage orientation and block name
		//NSSTRING strBlockColumn;
		//NSSTRING strUsage, strOrientation, strBlockName;
		//strBlockName = pFeature->getBlockName();
		//size_t foundNew;
		//foundNew = strBlockName.find(_T("NEW"));
		//if(foundNew!=NSSTRING::npos)
		//{
		//	strBlockColumn =  _T("NewBlock");
		//	strUsage = pFeature->readAttribute(_T("USAGE"));
		//	strOrientation = pFeature->readAttribute(_T("ORIENTATION"));
		//}
		//else
		//{
		//	size_t foundExt;
		//	foundExt = strBlockName.find(_T("EXT"));
		//	if(foundExt != NSSTRING::npos)
		//	{
		//		strBlockColumn =  _T("ExtBlock");
		//		strUsage = pFeature->readXDATA(_T("USAGE"));
		//		strOrientation = pFeature->readXDATA(_T("ORIENTATION"));
		//	}
		//	else 
		//		return NS_FAIL;
		//}

		//NSSTRING strFinalUsage = getQueryString(strUsage);
		//NSSTRING strFinalOrientation = getQueryString(strOrientation);
	
		//NSSPRINTF(szQuery, _T("Select FNO from BlockFeatures where %s = \'%s\' and USAGE %s and ORIENTATION %s"),
		//	strBlockColumn.c_str(), strBlockName.c_str(), strFinalUsage.c_str(), strFinalOrientation.c_str());	
		//
		
		NSSTRING strUsageOrntBlkNmae = getUsageOrntBlkName(pFeature);
		NSSPRINTF(szQuery, _T("Select FNO from BlockFeatures where %S"), strUsageOrntBlkNmae.c_str());	
		NSSTRING strFinalQuery = getQueryString(pFeature->m_mapAttributes, szQuery);
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(strFinalQuery.c_str());
	}
	
	if(FEATURE_LINEAR == pFeature->getFeatureType() || FEATURE_ARC == pFeature->getFeatureType())
	{
		NSSTRING strLayerName = pFeature->getLayerName();
		/*if(NSSTRCMP(_T("DEVELOPER_PLAN"), strLayerName.c_str()) == 0)
		{
			return NS_FAIL;
		}*/
		if(STATUS_NEW == pFeature->getFeatureStatus())
			NSSPRINTF(szQuery, _T("Select FNO from NewLinearFeatures where NewLayer = \'%S\'"), strLayerName.c_str());
		
		nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	}

	variant_t vFNO;
	if(nDBRet != -1)
	{
		bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("FNO"), vFNO);
	}
	
	if(bFNO)
	{
		nFNO = (int) vFNO;
		return NS_SUCCESS;
	}
	else
		return NS_FAIL;
}

int CNSRelationLinker::retrieveLinearFeatOrientation(CNSValidationDataCollector* pFeature, NSSTRING &strOrientation)
{
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];	

	// to allow Existing features to be able to connect with new features
	/*if(STATUS_NEW != pFeature->getFeatureStatus())
		return NS_FAIL;*/
	
	if(FEATURE_LINEAR != pFeature->getFeatureType() && FEATURE_ARC != pFeature->getFeatureType())
		return NS_FAIL;

	if(pFeature->getFNO() != nNumberNotFound)
	{
		NSSTRING strLayer = getQueryString(pFeature->getLayerName());

		if(STATUS_NEW == pFeature->getFeatureStatus())
		{
			NSSPRINTF(szQuery, _T("Select Orientation from NewLinearFeatures where FNO = %i and NewLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}
		else if(STATUS_EXT == pFeature->getFeatureStatus())
		{
			NSSPRINTF(szQuery, _T("Select Orientation from LinearFeatures where FNO = %i and ExtLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}
		
	}
	else
		return NS_FAIL;

	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	variant_t vOrientation;
	if(nDBRet != -1)
	{
		bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("Orientation"), vOrientation);

	}
	
	if(bFNO)
	{
		#ifndef _INTERNAL_
			strOrientation = vOrientation.bstrVal;
		#else
			strOrientation = vOrientation.bstrVal;
			/*char* szOrientation = _com_util::ConvertBSTRToString(vOrientation.bstrVal);
			strOrientation = szOrientation;*/
		#endif

		return NS_SUCCESS;
	}
	else
		return NS_FAIL;
}

int CNSRelationLinker::retrieveLinearFeatUsage(CNSValidationDataCollector* pFeature, NSSTRING &strUsage)
{
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];	
	
	// to allow Existing features to be able to connect with new features
	/*if(STATUS_NEW != pFeature->getFeatureStatus())
		return NS_FAIL;*/
	
	if(FEATURE_LINEAR != pFeature->getFeatureType() && FEATURE_ARC != pFeature->getFeatureType())
		return NS_FAIL;

	if(pFeature->getFNO() != nNumberNotFound)
	{
		NSSTRING strLayer = getQueryString(pFeature->getLayerName());

		if(STATUS_NEW == pFeature->getFeatureStatus())
		{
			NSSPRINTF(szQuery, _T("Select Usage from NewLinearFeatures where FNO = %i and NewLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}
		else if(STATUS_EXT == pFeature->getFeatureStatus())
		{
			NSSPRINTF(szQuery, _T("Select Usage from LinearFeatures where FNO = %i and ExtLayer %S"), pFeature->getFNO(), strLayer.c_str());
		}
		
	}
	else
		return NS_FAIL;

	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	variant_t vUsage;
	if(nDBRet != -1)
	{
		bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("Usage"), vUsage);

	}
	
	if(bFNO)
	{
		#ifndef _INTERNAL_
			strUsage = vUsage.bstrVal;
		#else
			strUsage = vUsage.bstrVal;
			/*char* szUsage = _com_util::ConvertBSTRToString(vUsage.bstrVal);
			strUsage = szUsage;*/
		#endif

		return NS_SUCCESS;
	}
	else
		return NS_FAIL;
}
/*
 @Brief			Takes Parent FNOs from the Database for pFeature
 @Param(in)		CNSValidationDataCollector* pFeature
 @Param(out)	std::vector<int> vectOwnershipParents
 @Return		int	NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::getOwnershipParentsFromMDB(CNSValidationDataCollector* pFeature, std::vector<int> &vectOwnershipParents)
{
	int nFeatureFNO = pFeature->getFNO();
	
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];
	NSSPRINTF(szQuery, _T("Select OWNERFNO from ParentChild where SOURCEFNO = %d"), nFeatureFNO);	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	std::vector<variant_t> vectVarientOwnerParent;
	if(nDBRet != -1)
	{
		CNSDatabaseMgr::getInstance()->getValue(_T("OWNERFNO"), vectVarientOwnerParent);
	}

	std::vector<variant_t>::iterator varientVectItr;
	for(varientVectItr = vectVarientOwnerParent.begin(); varientVectItr != vectVarientOwnerParent.end(); varientVectItr++)
	{
		vectOwnershipParents.push_back((int)(*varientVectItr));
	}
	return NS_SUCCESS;
}


/*
 @Brief			Takes child FNOs from the Database for pFeature
 @Param(in)		CNSValidationDataCollector* pFeature
 @Param(out)	std::vector<int> vectOwenershipChilds
 @Return		int	NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::getOwnershipChilds(CNSValidationDataCollector* pFeature, std::vector<int> &vectOwnershipChilds)
{
	int nFeatureFNO = pFeature->getFNO();
	
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];
	NSSPRINTF(szQuery, _T("Select SOURCEFNO from ParentChild where OWNERFNO = %d"), nFeatureFNO);	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	std::vector<variant_t> vectVarientOwnershipchild;
	if(nDBRet != -1)
	{
		CNSDatabaseMgr::getInstance()->getValue(_T("SOURCEFNO"), vectVarientOwnershipchild);
	}

	std::vector<variant_t>::iterator varientVectItr;
	for(varientVectItr = vectVarientOwnershipchild.begin(); varientVectItr != vectVarientOwnershipchild.end(); varientVectItr++)
	{
		vectOwnershipChilds.push_back((int)(*varientVectItr));
	}
	return NS_SUCCESS;
}


/*
 @Brief			Takes mant ownership_child FNOs from the Database for pFeature
 @Param(in)		CNSValidationDataCollector* pFeature
 @Param(out)	std::vector<int> vectManyOwenershipChilds
 @Return		int	NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::getManyOwnershipChilds(CNSValidationDataCollector* pFeature, std::vector<int> &vectManyOwnershipChilds)
{
	int nFeatureFNO = pFeature->getFNO();
	
	bool bFNO = false;
	TCHAR szQuery1[MAX_PATH+1];
	TCHAR szQuery2[MAX_PATH+1];
	
	// Get Source FNOs
	NSSPRINTF(szQuery1, _T("Select SOURCEFNO from ManyOwnership where OWNERFNO = %d"), nFeatureFNO);	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery1);
	std::vector<variant_t> vectVarientManyOwnershipchild;
	if(nDBRet != -1)
	{
		CNSDatabaseMgr::getInstance()->getValue(_T("SOURCEFNO"), vectVarientManyOwnershipchild);
	}

	std::vector<variant_t>::iterator varientVectItr;
	for(varientVectItr = vectVarientManyOwnershipchild.begin(); varientVectItr != vectVarientManyOwnershipchild.end(); varientVectItr++)
	{
		vectManyOwnershipChilds.push_back((int)(*varientVectItr));
	}
	

	//Also get Owener FNOs
	NSSPRINTF(szQuery2, _T("Select OWNERFNO from ManyOwnership where SOURCEFNO = %d"), nFeatureFNO);	
	int nDBRet2 = CNSDatabaseMgr::getInstance()->executeQuery(szQuery2);
	std::vector<variant_t> vectVarientManyOwnershipchild2;
	if(nDBRet2 != -1)
	{
		CNSDatabaseMgr::getInstance()->getValue(_T("OWNERFNO"), vectVarientManyOwnershipchild2);
	}

	for(varientVectItr = vectVarientManyOwnershipchild2.begin(); varientVectItr != vectVarientManyOwnershipchild2.end(); varientVectItr++)
	{
		vectManyOwnershipChilds.push_back((int)(*varientVectItr));
	}

	return NS_SUCCESS;
}


/*
 @Brief			Takes mant FNOs from the Database for pFeature
 @Param(in)		CNSValidationDataCollector* pFeature
 @Param(out)	std::vector<int> vectManyOwnershipChilds
 @Return		int	NS_FAIL/NS_SUCCESS
*/
int CNSRelationLinker::getConnectableAssets(CNSValidationDataCollector* pFeature, std::vector<int> &vectAssetsAtNode1)
{
	int nFeatureFNO = pFeature->getFNO();
	
	TCHAR szQuery[MAX_PATH+1];
	NSSPRINTF(szQuery, _T("Select SOURCEFNO from Connectivity where CONNECTINGFNO = %d"), nFeatureFNO);	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	std::vector<variant_t> vectVarientassetsAtNode1;
	if(nDBRet != -1)
	{
		CNSDatabaseMgr::getInstance()->getValue(_T("SOURCEFNO"), vectVarientassetsAtNode1);
	}

	std::vector<variant_t>::iterator varientVectItr;
	for(varientVectItr = vectVarientassetsAtNode1.begin(); varientVectItr != vectVarientassetsAtNode1.end(); varientVectItr++)
	{
		vectAssetsAtNode1.push_back((int)(*varientVectItr));
	}
	return NS_SUCCESS;
}

/*
 @Brief			This function gets the Node (Node/Node2) of a Block feature to which the linear feature (pFeature) should be connected
 @Param(in)		CNSValidationDataCollector		Cable feature
 @Param(out)	int &nNode						gets the node number of a block to which cable should be connected
 @Return		NS_SUCCESS/NS_FAIL
*/
int CNSRelationLinker::getConnectionNode(CNSValidationDataCollector* pFeature, int &nNode)
{
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];	
	TCHAR szFinalQuery[MAX_PATH+1];	
	NSSTRING strTemp;
	
	if(/*FEATURE_LINEAR != pFeature->getFeatureType() ||*/ (STATUS_NEW != pFeature->getFeatureStatus()))
		return NS_FAIL;

	if(pFeature->getFNO() != nNumberNotFound)
	{
		if(FEATURE_LINEAR == pFeature->getFeatureType())
		//NSSPRINTF(szQuery, _T("Select Node from NewLinearFeatures where FNO = %i"), pFeature->getFNO());
			NSSPRINTF(szQuery, _T("Select Node from NewLinearFeatures where NewLayer = \'%S\'"), pFeature->getLayerName().c_str());
		else if(FEATURE_BLOCK == pFeature->getFeatureType())
		{
			NSSTRING strUsageOrntBlkNmae = getUsageOrntBlkName(pFeature);
			NSSPRINTF(szFinalQuery, _T("Select Node from BlockFeatures where %S"), strUsageOrntBlkNmae.c_str());
			strTemp = getQueryString(pFeature->m_mapAttributes, szFinalQuery);

			#ifndef _INTERNAL_
				NSSTRCPY(szQuery, strTemp.c_str());
			#else
				NSSTRCPY(szQuery,strTemp.c_str());
			#endif
		}
	}
	else
		return NS_FAIL;
	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	variant_t vFNO;
	if(nDBRet != -1)
	{
		bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("Node"), vFNO);
	}
	
	if(bFNO)
	{
		nNode = (int) vFNO;
		return NS_SUCCESS;
	}
	else
		return NS_FAIL;
}

/*
 @Brief		Gets the number of connection nodes of a feature
 @Param(in)		CNSValidationDataCollector		Block feature
 @Param(out)	int &nNode						number of connection nodes
 @Return		NS_SUCCESS/NS_FAIL
*/
int CNSRelationLinker::getNumberOfNodes(CNSValidationDataCollector* pFeature, int &nNode)
{
	bool bFNO = false, bLinear = false;
	TCHAR szQuery[MAX_PATH+1];
	NSSTRING strFinalQuery;
	
	if(FEATURE_LINEAR == pFeature->getFeatureType() || FEATURE_BUSBAR == pFeature->getFeatureType() || FEATURE_ARC == pFeature->getFeatureType())
	{
		//nNode = 2;
		//return NS_SUCCESS;
		bLinear = true;
		NSSTRING strLayer = getQueryString(pFeature->getLayerName());
		NSSPRINTF(szQuery, _T("Select NumberOfNodes from NewLinearFeatures where FNO = %i and NewLayer %S"), pFeature->getFNO(), strLayer.c_str());
		//strFinalQuery = getQueryString(szQuery);	
		strFinalQuery = szQuery;
	}

	if(!bLinear && (pFeature->getFNO() != nNumberNotFound) && (FEATURE_LINEAR != pFeature->getFeatureType()))
	{
		NSSTRING strUsageOrntBlkNmae = getUsageOrntBlkName(pFeature);
		NSSPRINTF(szQuery, _T("Select NumOfNodes from BlockFeatures where %S"), strUsageOrntBlkNmae.c_str());		
		strFinalQuery = getQueryString(pFeature->m_mapAttributes, szQuery);	
	}
	//else
		//return NS_FAIL;

	//int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(strFinalQuery.c_str());
	variant_t vNumOfNodes;
	if(nDBRet != -1)
	{
		if(bLinear)
			bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("NumberOfNodes"), vNumOfNodes);
		else
			bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("NumOfNodes"), vNumOfNodes);
	}
	
	if(bFNO)
	{
		nNode = (int) vNumOfNodes;
		return NS_SUCCESS;
	}
	else
		return NS_FAIL;

}

/*
 @Brief			Gets connection method of the block feature
				method 0 - Atleast one cable is connected to node of a block, if the block can connect to two or more cables, irrespective of the cable type
				method 1 - Node1 and Node2 of a block can connect to only certain type of cables
 @Param(in)		CNSValidationDataCollector		Block feature
 @Param(out)	int &nNode						Connection method
 @Return		NS_SUCCESS/NS_FAIL
*/
int CNSRelationLinker::getConnectionMethod(CNSValidationDataCollector* pFeature, int &nNode)
{
	bool bFNO = false;
	TCHAR szQuery[MAX_PATH+1];
	NSSTRING strFinalQuery;
	
	if((pFeature->getFNO() != nNumberNotFound) && (FEATURE_BLOCK == pFeature->getFeatureType()))
	{
		NSSTRING strUsageOrntBlkNmae = getUsageOrntBlkName(pFeature);
		//NSSPRINTF(szQuery, _T("Select NumOfNodes from BlockFeatures where %s"), strUsageOrntBlkNmae.c_str());		
		NSSPRINTF(szQuery, _T("Select ConnectingMethod from BlockFeatures where %S"), strUsageOrntBlkNmae.c_str());
		
		strFinalQuery = getQueryString(pFeature->m_mapAttributes, szQuery);	
		//NSSPRINTF(szQuery, _T("Select ConnectingMethod from BlockFeatures where FNO = %i"), pFeature->getFNO());
	}
	else if((pFeature->getFNO() != nNumberNotFound) && (FEATURE_LINEAR == pFeature->getFeatureType()))
	{
		if(pFeature->getFeatureStatus() == STATUS_NEW)
		{
			NSSTRING strLayer = getQueryString(pFeature->getLayerName());
			NSSPRINTF(szQuery, _T("Select ConnectingMethod from NewLinearFeatures where FNO = %i and NewLayer %S"), pFeature->getFNO(), strLayer.c_str());
			
			strFinalQuery = szQuery;
		}
		/*else------- Need to decide how to identify the existing BUSBAR
		{
			NSSTRING strLayer = getQueryString(pFeature->getLayerName());
			NSSPRINTF(szQuery, _T("Select ConnectingMethod from LinearFeatures where FNO = %i and Layer %s"), pFeature->getFNO(), strLayer.c_str());
			strFinalQuery = szQuery;
		}*/
	}

	//int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(strFinalQuery.c_str());
	variant_t vConnectionMethod;
	if(nDBRet != -1)
	{
		bFNO = CNSDatabaseMgr::getInstance()->getValue(_T("ConnectingMethod"), vConnectionMethod);
	}
	
	if(bFNO)
	{
		nNode = (int) vConnectionMethod;
		return NS_SUCCESS;
	}
	else
		return NS_FAIL;
}


NSSTRING CNSRelationLinker::getCableOrientation(CNSValidationDataCollector* pFeature)
{
	bool bOrientation = false;
	TCHAR szQuery[MAX_PATH+1];	
	
	if(FEATURE_LINEAR != pFeature->getFeatureType() || (STATUS_NEW != pFeature->getFeatureStatus()))
		return _T("");

	if(pFeature->getFNO() != nNumberNotFound)
	{
		NSSPRINTF(szQuery, _T("Select ORIENTATION from NewLinearFeatures where NewLayer = \'%S\'"), pFeature->getLayerName().c_str());
	}
	else
		return _T("");
	
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
	variant_t vOrientation;
	if(nDBRet != -1)
	{
		bOrientation = CNSDatabaseMgr::getInstance()->getValue(_T("ORIENTATION"), vOrientation);
	}
	
	if(bOrientation)
	{
		NSSTRING strOrientation = (NSSTRING)((_bstr_t)(vOrientation.bstrVal));
		return strOrientation;
	}
	else
		return _T("");
}

/*
 @Brief		Stores parent and Child FID in m_mapProposedRelations
 @Param(in)	CNSValidationDataCollector*
*/
void CNSRelationLinker::assignOwnershipParentChild(CNSValidationDataCollector* pFeature)
{
	int nParentFID = nNumberNotFound;
	CNSValidationDataCollector *pParentFeature;

	std::vector<int>vectMDBOwnershipParents;
	getOwnershipParentsFromMDB(pFeature, vectMDBOwnershipParents);

	if(vectMDBOwnershipParents.size() == 0)
		return;

	// check the vectMDBOwnershipParents has corresponding FNO in vectFeaturesInproximity
	// Do this search operation in separate function
	if(pFeature->m_vectFeaturesInProximity.size() != 0)
	{
		std::vector<LONG_PTR>::iterator vectProxItr;
		std::vector<int>::iterator vectMDBFeatureItr;
		
		for(vectProxItr = pFeature->m_vectFeaturesInProximity.begin(); vectProxItr != pFeature->m_vectFeaturesInProximity.end(); vectProxItr++)
		{
			for(vectMDBFeatureItr = vectMDBOwnershipParents.begin(); vectMDBFeatureItr != vectMDBOwnershipParents.end(); vectMDBFeatureItr++)
			{
				LONG_PTR nProxObjectId = (*vectProxItr);
				CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

				if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
				{
					nParentFID = pProxFeature->getFID();
					pParentFeature = pProxFeature;
					break;
				}
			}
			// Get out of this loop also, beacause there can be only one parent for a feature
			if(nParentFID != nNumberNotFound)
				break;
		}
	}

	if(pFeature->m_vectFeaturesInProximityByCmnAttVal.size() != 0)
	{
		std::vector<LONG_PTR>::iterator vectProxItr;
		std::vector<int>::iterator vectMDBFeatureItr;
		
		for(vectProxItr = pFeature->m_vectFeaturesInProximityByCmnAttVal.begin(); vectProxItr != pFeature->m_vectFeaturesInProximityByCmnAttVal.end(); vectProxItr++)
		{
			for(vectMDBFeatureItr = vectMDBOwnershipParents.begin(); vectMDBFeatureItr != vectMDBOwnershipParents.end(); vectMDBFeatureItr++)
			{
				LONG_PTR nProxObjectId = (*vectProxItr);
				CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

				if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
				{
					nParentFID = pProxFeature->getFID();
					pParentFeature = pProxFeature;
					break;
				}
			}
			// Get out of this loop also, beacause there can be only one parent for a feature
			if(nParentFID != nNumberNotFound)
				break;
		}
	}
	
	if(nParentFID != nNumberNotFound)
	{
		// First make Owenership_Parent entry for pFeature
		//
		LONG_PTR lObjectId = pFeature->getObjectId();
		std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 

		mapItr = m_mapProposedRelations.find(lObjectId);
		if(mapItr != m_mapProposedRelations.end())
		{
			CNSProposedRelations *pTempProposedRelations = (*mapItr).second;
			pTempProposedRelations->setOwnershipParentFID(nParentFID, pParentFeature->getFNO());
		}

		// Now make Owenership_Child entry for pParentFeature 
		//
		LONG_PTR lParentObjectId = pParentFeature->getObjectId();

		mapItr = m_mapProposedRelations.find(lParentObjectId);
		if(mapItr != m_mapProposedRelations.end())
		{
			CNSProposedRelations *pTempProposedRelationsCounterPart = (*mapItr).second;
			//pTempProposedRelationsCounterPart->setOwnershipChildsFID(pFeature->getFID(), pFeature->getFNO());
			pTempProposedRelationsCounterPart->setRelConData(OWNERSHIP_CHILD_DATA, pFeature->getFID(), pFeature->getFNO());
		}			
	}
}

/*
 @Brief		Stores Child FID in m_mapProposedRelations
			Though assignOwnershipParentChild() makes both OWENERSHIP_PARENT and OWNERSHIP_CHILD entries for the correspomding features,
			this function is required because the above functions does not make OWNERSHIP_CHILD entry of an EXT Block (say EXT Street Light)
			into a NEW Block feature (say NEW Pole)
 @Param(in)	CNSValidationDataCollector*
*/
void CNSRelationLinker::assignOwnershipChild(CNSValidationDataCollector* pFeature)
{
	int nChildFID = nNumberNotFound;
	CNSValidationDataCollector *pChildFeature;	

	std::vector<int> vectMDBOwnershipChilds;
	getOwnershipChilds(pFeature, vectMDBOwnershipChilds);

	if(vectMDBOwnershipChilds.size() == 0)
		return;

	// check the vectMDBOwnershipChilds has corresponding FNO in vectFeaturesInproximity
	// Do this search operation in separate function
	std::vector<LONG_PTR>::iterator vectProxItr;
	std::vector<int>::iterator vectMDBFeatureItr;
	
	for(vectProxItr = pFeature->m_vectFeaturesInProximity.begin(); vectProxItr != pFeature->m_vectFeaturesInProximity.end(); vectProxItr++)
	{
		for(vectMDBFeatureItr = vectMDBOwnershipChilds.begin(); vectMDBFeatureItr != vectMDBOwnershipChilds.end(); vectMDBFeatureItr++)
		{
			LONG_PTR nProxObjectId = (*vectProxItr);
			CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

			if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
			{
				nChildFID = pProxFeature->getFID();
				pChildFeature = pProxFeature;

				// First make Owenership_Child entry for pFeature
				//
				LONG_PTR lObjectId = pFeature->getObjectId();
				std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 

				mapItr = m_mapProposedRelations.find(lObjectId);
				if(mapItr != m_mapProposedRelations.end())
				{
					CNSProposedRelations *pTempProposedRelations = (*mapItr).second;
					//pTempProposedRelations->setOwnershipChildsFID(nChildFID, pChildFeature->getFNO());
					pTempProposedRelations->setRelConData(OWNERSHIP_CHILD_DATA, nChildFID, pChildFeature->getFNO());
				}				

				//// Now make ManyOwenership_Child entry for pManyOwnershipRelative 
				////
				//long lParentObjectId = pManyOwnershipRelative->getObjectId();

				//mapItr = m_mapProposedRelations.find(lParentObjectId);
				//if(mapItr != m_mapProposedRelations.end())
				//{
				//	CNSProposedRelations *pTempProposedRelationsCounterPart = (*mapItr).second;
				//	pTempProposedRelationsCounterPart->setManyOwnershipChildsFID(pFeature->getFID(), pFeature->getFNO());
				//}
				break;
			}
		}
	}
}

/*
 @Brief		Stores manyownership childs FID in m_mapProposedRelations
 @Param(in)		CNSValidationDataCollector*
*/
void CNSRelationLinker::assignManyOwnershipChilds(CNSValidationDataCollector* pFeature)
{
	int nManyOwenershipChild = nNumberNotFound;
	CNSValidationDataCollector *pManyOwnershipRelative;

	std::vector<int>vectMDBManyOwenershipChilds;
	getManyOwnershipChilds(pFeature, vectMDBManyOwenershipChilds);
	
	if(vectMDBManyOwenershipChilds.size() != 0)
	{
		// check the vectMDBManyOwenershipChilds has corresponding FNO in vectFeaturesInproximity
		// Do this search operation in separate function
		std::vector<LONG_PTR>::iterator vectProxItr;
		std::vector<int>::iterator vectMDBFeatureItr;
		
		for(vectProxItr = pFeature->m_vectFeaturesInProximity.begin(); vectProxItr != pFeature->m_vectFeaturesInProximity.end(); vectProxItr++)
		{
			for(vectMDBFeatureItr = vectMDBManyOwenershipChilds.begin(); vectMDBFeatureItr != vectMDBManyOwenershipChilds.end(); vectMDBFeatureItr++)
			{
				LONG_PTR nProxObjectId = (*vectProxItr);
				CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

				if(pFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pFeature).c_str(), _T("OH")) != 0)
						continue;
				}
				if(pProxFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pProxFeature).c_str(), _T("OH")) != 0)
						continue;
				} 

				if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
				{
					nManyOwenershipChild = pProxFeature->getFID();
					pManyOwnershipRelative = pProxFeature;

					// First make ManyOwenership_Child entry for pFeature
					//
					LONG_PTR lObjectId = pFeature->getObjectId();
					std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 

					mapItr = m_mapProposedRelations.find(lObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelations = (*mapItr).second;
						//pTempProposedRelations->setManyOwnershipChildsFID(nManyOwenershipChild, pManyOwnershipRelative->getFNO());
						 pTempProposedRelations->setRelConData(MANYOWNERSHIP_CHILD_DATA, nManyOwenershipChild, pManyOwnershipRelative->getFNO());
					}				

					// Now make ManyOwenership_Child entry for pManyOwnershipRelative 
					//
					LONG_PTR lParentObjectId = pManyOwnershipRelative->getObjectId();

					mapItr = m_mapProposedRelations.find(lParentObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelationsCounterPart = (*mapItr).second;
						//pTempProposedRelationsCounterPart->setManyOwnershipChildsFID(pFeature->getFID(), pFeature->getFNO());
						pTempProposedRelationsCounterPart->setRelConData(MANYOWNERSHIP_CHILD_DATA, pFeature->getFID(), pFeature->getFNO());
					}
					break;
				}
			}
		}

		//
		for(vectProxItr = pFeature->m_vectFeaturesInProximityAtNode2.begin(); vectProxItr != pFeature->m_vectFeaturesInProximityAtNode2.end(); vectProxItr++)
		{
			for(vectMDBFeatureItr = vectMDBManyOwenershipChilds.begin(); vectMDBFeatureItr != vectMDBManyOwenershipChilds.end(); vectMDBFeatureItr++)
			{
				LONG_PTR nProxObjectId = (*vectProxItr);
				CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);
				
				if(pFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pFeature).c_str(), _T("OH")) != 0)
						continue;
				}
				if(pProxFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pProxFeature).c_str(), _T("OH")) != 0)
						continue;
				}

				if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
				{
					nManyOwenershipChild = pProxFeature->getFID();
					pManyOwnershipRelative = pProxFeature;

					// First make ManyOwenership_Child entry for pFeature
					//
					LONG_PTR lObjectId = pFeature->getObjectId();
					std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 

					mapItr = m_mapProposedRelations.find(lObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelations = (*mapItr).second;
						//pTempProposedRelations->setManyOwnershipChildsFID(nManyOwenershipChild, pManyOwnershipRelative->getFNO());
						pTempProposedRelations->setRelConData(MANYOWNERSHIP_CHILD_DATA, nManyOwenershipChild, pManyOwnershipRelative->getFNO());
					}				

					// Now make ManyOwenership_Child entry for pManyOwnershipRelative 
					//
					LONG_PTR lParentObjectId = pManyOwnershipRelative->getObjectId();

					mapItr = m_mapProposedRelations.find(lParentObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelationsCounterPart = (*mapItr).second;
						//pTempProposedRelationsCounterPart->setManyOwnershipChildsFID(pFeature->getFID(), pFeature->getFNO());
						pTempProposedRelationsCounterPart->setRelConData(MANYOWNERSHIP_CHILD_DATA, pFeature->getFID(), pFeature->getFNO());
					}
					break;
				}
			}
		}

		//
		for(vectProxItr = pFeature->m_vectInterPolyFeaturesInProximity.begin(); vectProxItr != pFeature->m_vectInterPolyFeaturesInProximity.end(); vectProxItr++)
		{
			for(vectMDBFeatureItr = vectMDBManyOwenershipChilds.begin(); vectMDBFeatureItr != vectMDBManyOwenershipChilds.end(); vectMDBFeatureItr++)
			{
				LONG_PTR nProxObjectId = (*vectProxItr);
				CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

				if(pFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pFeature).c_str(), _T("OH")) != 0)
						continue;
				}
				if(pProxFeature->bPolyLine)
				{
					if(NSSTRCMP(getCableOrientation(pProxFeature).c_str(), _T("OH")) != 0)
						continue;
				}


				if(pProxFeature->getFNO() == (*vectMDBFeatureItr))
				{
					nManyOwenershipChild = pProxFeature->getFID();
					pManyOwnershipRelative = pProxFeature;

					// First make ManyOwenership_Child entry for pFeature
					//
					LONG_PTR lObjectId = pFeature->getObjectId();
					std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 

					mapItr = m_mapProposedRelations.find(lObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelations = (*mapItr).second;
						//pTempProposedRelations->setManyOwnershipChildsFID(nManyOwenershipChild, pManyOwnershipRelative->getFNO());
						 pTempProposedRelations->setRelConData(MANYOWNERSHIP_CHILD_DATA, nManyOwenershipChild, pManyOwnershipRelative->getFNO());
					}				

					// Now make ManyOwenership_Child entry for pManyOwnershipRelative 
					//
					LONG_PTR lParentObjectId = pManyOwnershipRelative->getObjectId();

					mapItr = m_mapProposedRelations.find(lParentObjectId);
					if(mapItr != m_mapProposedRelations.end())
					{
						CNSProposedRelations *pTempProposedRelationsCounterPart = (*mapItr).second;
						//pTempProposedRelationsCounterPart->setManyOwnershipChildsFID(pFeature->getFID(), pFeature->getFNO());
						pTempProposedRelationsCounterPart->setRelConData(MANYOWNERSHIP_CHILD_DATA, pFeature->getFID(), pFeature->getFNO());
					}
					break;
				}
			}
		}
		//
	}
}


/*
 @Brief		Stores electrical connections in m_mapProposedRelations
 @Param		CNSValidationDataCollector*
*/
void CNSRelationLinker::assignElectricalConnections(CNSValidationDataCollector* pFeature, bool bProposedToExisting, int LinearConnOnLayerFlag)
{
	int nConFeatureFID = nNumberNotFound;
	CNSValidationDataCollector *pEleConnectable = NULL;

	std::vector<int>vectMDBEleCon;
	getConnectableAssets(pFeature, vectMDBEleCon);
	if(vectMDBEleCon.size() == 0)
		return;

	int nNumberOfNodes = 0;
	//if(NS_FAIL == getNumberOfNodes(pFeature, nNumberOfNodes))
	// give error log 
	getNumberOfNodes(pFeature, nNumberOfNodes);
		

	if((nNumberOfNodes >= 1) && ((pFeature->m_vectFeaturesInProximity.size() != 0)))
	{
		// check the vectMDBEleCon has corresponding FNO in vectFeaturesInproximity
		std::vector<LONG_PTR>::iterator vectProxItr;
		std::vector<int>::iterator vectMDBFeatureItr;
		
		for(vectProxItr = pFeature->m_vectFeaturesInProximity.begin(); vectProxItr != pFeature->m_vectFeaturesInProximity.end(); vectProxItr++)
		{
			LONG_PTR nProxObjectId = (*vectProxItr);
			CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

			if(!bProposedToExisting)
			{
				if(STATUS_EXT == pProxFeature->getFeatureStatus())
					continue; //To avoid Proposed-To-Existing connectivity
			}

			// this code to restrict connectivity between freature with mismatching orientation
			//if(!checkforOrientation(pFeature , pProxFeature))
			//{
			//	continue; //To avoid UG-OH Connectivity
			//}
			// this code to restrict connectivity between freature with mismatching orientation
			
			
			//To avoid Block-To-Block connectivity
			/*	if(FEATURE_BLOCK == pFeature->getFeatureType() && FEATURE_BLOCK == pProxFeature->getFeatureType())
				continue;*/
			
			if(FEATURE_LINEAR == pFeature->getFeatureType() && FEATURE_LINEAR == pProxFeature->getFeatureType())
			{
				if(LinearConnOnLayerFlag == 0)
				{							
					if(NSSTRCMP(pFeature->getLayerName().c_str(), pProxFeature->getLayerName().c_str()) != 0)
						continue;  //To avoid connecivity of linear features on different layers
				}				
			}

			for(vectMDBFeatureItr = vectMDBEleCon.begin(); vectMDBFeatureItr != vectMDBEleCon.end(); vectMDBFeatureItr++)
			{
				if(pProxFeature->getFNO() != (*vectMDBFeatureItr)) 
					continue;

				nConFeatureFID = pProxFeature->getFID();
				pEleConnectable = pProxFeature;

				// For features like Sub-Station or Zone-Sub-Station linear features are connected to particular node
				//NSSTRING strNodeSeparator = pFeature->readAttribute(_T("NODESEPARATOR"));
				int nNodeSeparator = 0;		// by default we take general case
				//if(strNodeSeparator.size() != 0)
				//	nNodeSeparator = NSATOI(strNodeSeparator.c_str());
				getConnectionMethod(pFeature, nNodeSeparator);

				std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 
				mapItr = m_mapProposedRelations.find(pFeature->getObjectId());
				if(mapItr != m_mapProposedRelations.end())
				{
					CNSProposedRelations* pTempProposedRelations = (*mapItr).second;
					if(nNodeSeparator == 1 && (FEATURE_ARC != pProxFeature->getFeatureType()))
					{
						int nNode = 1;
						// Get the Node(1/2) of a block to which the linear feature in proximity should be connected
						if((FEATURE_BLOCK == pFeature->getFeatureType() /*&& (FEATURE_LINEAR == pProxFeature->getFeatureType()*/))
							getConnectionNode(pProxFeature, nNode);

						if(nNode == 1) 
						{
							//pTempProposedRelations->setConNode1FID(nConFeatureFID, pEleConnectable->getFNO());
							pTempProposedRelations->setRelConData(CONNECTIVITY_NODE1_DATA, nConFeatureFID, pEleConnectable->getFNO());
						}
						else if(nNode == 2) 
						{
							//pTempProposedRelations->setConNode2FID(nConFeatureFID, pEleConnectable->getFNO());
							pTempProposedRelations->setRelConData(CONNECTIVITY_NODE2_DATA, nConFeatureFID, pEleConnectable->getFNO());
						}
					}
					else if(nNodeSeparator == 2 && (FEATURE_ARC != pProxFeature->getFeatureType()))
					{
						//This is the case where the feature will checked for usage values
						int nNode = 1;
						//getConnectionNode(pProxFeature, nNode);
						bool bUsage = checkforUsage(pFeature , pProxFeature);
						if(bUsage == true)
						{
							int nConNode1 = pTempProposedRelations->getNumConAtNode1();
							int nConNode2 = pTempProposedRelations->getNumConAtNode2();

							if((FEATURE_BLOCK == pFeature->getFeatureType())&& (nNumberOfNodes == 2) && (nConNode1 >=1) && (nConNode2 == 0))
							{
								//pTempProposedRelations->setConNode2FID(nConFeatureFID, pEleConnectable->getFNO());
								pTempProposedRelations->setRelConData(CONNECTIVITY_NODE2_DATA, nConFeatureFID, pEleConnectable->getFNO());
							}
							else // It is a linear feature
							{
								//pTempProposedRelations->setConNode1FID(nConFeatureFID, pEleConnectable->getFNO());
								pTempProposedRelations->setRelConData(CONNECTIVITY_NODE1_DATA, nConFeatureFID, pEleConnectable->getFNO());
							}
						}
					}
					//Newly introduced connecting method =4. This indicates that we need to first make the connectivity
					//based onm USAGE and if usages are matching for two features then we look for NODE column value of 
					//features in vicinity.
					else if(nNodeSeparator == 4 && (FEATURE_ARC != pProxFeature->getFeatureType()))
					{
						//This is the case where the feature will checked for usage values
						int nNode = 1;
						//getConnectionNode(pProxFeature, nNode);
						bool bUsage = checkforUsage(pFeature , pProxFeature);
						if(bUsage == true)
						{
							int nNode = 1;
							// Get the Node(1/2) of a block to which the linear feature in proximity should be connected
							if((FEATURE_BLOCK == pFeature->getFeatureType() /*&& (FEATURE_LINEAR == pProxFeature->getFeatureType()*/))
								getConnectionNode(pProxFeature, nNode);

							if(nNumberOfNodes == 1)
							{
								pTempProposedRelations->setRelConData(CONNECTIVITY_NODE1_DATA, nConFeatureFID, pEleConnectable->getFNO());
							}
							else if(nNode == 1) 
							{
								//pTempProposedRelations->setConNode1FID(nConFeatureFID, pEleConnectable->getFNO());
								pTempProposedRelations->setRelConData(CONNECTIVITY_NODE1_DATA, nConFeatureFID, pEleConnectable->getFNO());
							}
							else if(nNode == 2) 
							{
								//pTempProposedRelations->setConNode2FID(nConFeatureFID, pEleConnectable->getFNO());
								pTempProposedRelations->setRelConData(CONNECTIVITY_NODE2_DATA, nConFeatureFID, pEleConnectable->getFNO());
							}
						}
					}
					else
					{	// In a case, where a Block has two nodes and whether a cable should be connected to node1 or node 2 is not clear,
						// One cable is connected to Node 1 and one cable is connected to node 2
						// If there are more than 2 cables remaining cables are connected to node 1
						// If pFeature is a cable, then pEleConnectable is connected to Node1
						int nConNode1 = pTempProposedRelations->getNumConAtNode1();
						int nConNode2 = pTempProposedRelations->getNumConAtNode2();
						if((FEATURE_BLOCK == pFeature->getFeatureType()) && (nNumberOfNodes == 2) && (nConNode1 >=1) && (nConNode2 == 0))
						{
							//pTempProposedRelations->setConNode2FID(nConFeatureFID, pEleConnectable->getFNO());
							pTempProposedRelations->setRelConData(CONNECTIVITY_NODE2_DATA, nConFeatureFID, pEleConnectable->getFNO());
						}
						else // It is a linear feature
						{
							//pTempProposedRelations->setConNode1FID(nConFeatureFID, pEleConnectable->getFNO());
							pTempProposedRelations->setRelConData(CONNECTIVITY_NODE1_DATA, nConFeatureFID, pEleConnectable->getFNO());
						}
					}
					break;
				}
			}
		}
	}
	
	// Following code is only for linear features
	// ** Only linear features will have populated m_vectFeaturesInProximityAtNode2
	if((nNumberOfNodes == 2) && ((pFeature->m_vectFeaturesInProximityAtNode2.size()) != 0))
	{
		std::vector<LONG_PTR>::iterator vectProxItr;
		std::vector<int>::iterator vectMDBFeatureItr;
		
		for(vectProxItr = pFeature->m_vectFeaturesInProximityAtNode2.begin(); vectProxItr != pFeature->m_vectFeaturesInProximityAtNode2.end(); vectProxItr++)
		{
			LONG_PTR nProxObjectId = (*vectProxItr);
			CNSValidationDataCollector* pProxFeature = getFeature(nProxObjectId);

			if(!bProposedToExisting)
			{
				if(STATUS_EXT == pProxFeature->getFeatureStatus())
					continue; //To avoid Proposed-To-Existing connectivity
			}

			// this code to restrict connectivity between freature with mismatching orientation
			//if(!checkforOrientation(pFeature , pProxFeature))
			//{
			//	continue; //To avoid UG-OH Connectivity
			//}
			// this code to restrict connectivity between freature with mismatching orientation

			
			if(FEATURE_LINEAR == pFeature->getFeatureType() && FEATURE_LINEAR == pProxFeature->getFeatureType())
			{
				if(LinearConnOnLayerFlag == 0)
				{							
					if(NSSTRCMP(pFeature->getLayerName().c_str(), pProxFeature->getLayerName().c_str()) != 0)
						continue;  //To avoid connecivity of linear features on different layers
				}					
			}


            int nConnMthd = -1;
			if((FEATURE_ARC == pFeature->getFeatureType() && FEATURE_LINEAR == pProxFeature->getFeatureType())
			||(FEATURE_LINEAR == pFeature->getFeatureType() && FEATURE_ARC == pProxFeature->getFeatureType()))
			{
			}
			else
			{
				getConnectionMethod(pFeature,nConnMthd);
				if(nConnMthd == 2)
				{
					bool bUsage = checkforUsage(pFeature , pProxFeature);
		    		if(bUsage == false)
					{
						continue;
					}
				}
			}

			for(vectMDBFeatureItr = vectMDBEleCon.begin(); vectMDBFeatureItr != vectMDBEleCon.end(); vectMDBFeatureItr++)
			{
				if(pProxFeature->getFNO() != (*vectMDBFeatureItr))
					continue;

				std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr; //m_mapProposedRelations 
				mapItr = m_mapProposedRelations.find(pFeature->getObjectId());
              
                if(mapItr != m_mapProposedRelations.end() /*&& (nConnMthd!=2)*/)
				{
					CNSProposedRelations* pTempProposedRelations = (*mapItr).second;
					//pTempProposedRelations->setConNode2FID(pProxFeature->getFID(), pProxFeature->getFNO());
					pTempProposedRelations->setRelConData(CONNECTIVITY_NODE2_DATA, pProxFeature->getFID(), pProxFeature->getFNO());
				}
				break; // Connection complete, Get next Feature in proximity
			}
		}
	}
}


/*
 @Brief		Searches features in m_mapBlocksToBeLinked and in m_mapLinearFeaturesToBeLinked
 @Param(in)	long lObjectId
 @Return	CNSValidationDataCollector*
*/
CNSValidationDataCollector* CNSRelationLinker::getFeature(LONG_PTR lObjectId)
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator mapItr;
	mapItr = m_mapBlocksToBeLinked.find(lObjectId);

	if(mapItr != m_mapBlocksToBeLinked.end())
	{
		return ((CNSValidationDataCollector*) (*mapItr).second);
	}
	else
	{
		mapItr = m_mapLinearFeaturesToBeLinked.find(lObjectId);
		if(mapItr != m_mapLinearFeaturesToBeLinked.end())
		{
			return ((CNSValidationDataCollector*) (*mapItr).second);
		}
		else 
		{
			mapItr = m_mapExtBlocks.find(lObjectId);
			if(mapItr != m_mapExtBlocks.end())
			{
				return ((CNSValidationDataCollector*) (*mapItr).second);
			}
			else 
			{
				mapItr = m_mapExtCablesToBeLinked.find(lObjectId);
				if(mapItr != m_mapExtCablesToBeLinked.end())
				{
					return ((CNSValidationDataCollector*) (*mapItr).second);
				}
				else 
					return NULL;
			}
		}
	}
}

/*
 @Brief		Sends the FID and the proposed relations to be written as XDATA to the addXDATA()
*/
void CNSRelationLinker::postasXDATA()
{
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapItr;
	for(mapItr = m_mapProposedRelations.begin(); mapItr != m_mapProposedRelations.end(); mapItr++)
	{
		LONG_PTR lObjectId = (*mapItr).first;
		CNSProposedRelations* pProposedRelations = (*mapItr).second;

		NSSTRING strOParent(pProposedRelations->getOwnershipParentFID());
		NSSTRING strOChilds(pProposedRelations->getOwnershipChildsFID());
		NSSTRING strMOChilds(pProposedRelations->getManyOwnershipFID());
		NSSTRING strConNode1(pProposedRelations->getConNode1FID());
		NSSTRING strConNode2(pProposedRelations->getConNode2FID());
		//NSSTRING strConNode2(_T("007"));
		NSSTRING strFID(pProposedRelations->getFID());

		STRING_MAP mapXDATA;
		mapXDATA.insert(STRING_MAP::value_type(_T("OWNERSHIP_PARENT"), strOParent));
		mapXDATA.insert(STRING_MAP::value_type(_T("OWNERSHIP_CHILD"), strOChilds));
		mapXDATA.insert(STRING_MAP::value_type(_T("MANYOWNERSHIP_CHILD"), strMOChilds));
		mapXDATA.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE1"), strConNode1));
		mapXDATA.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE2"), strConNode2));
		mapXDATA.insert(STRING_MAP::value_type(_T("FID"), strFID));

		//// For MOD features preserve original XDATA 
		//CNSValidationDataCollector* pTempFeature = getFeature(lObjectId);
		//STRING_MAP::iterator originalXdataItr, NewXdataItr;
		//for(originalXdataItr = pTempFeature->m_mapOfRelationsAndConnectivity.begin();
		//	originalXdataItr != pTempFeature->m_mapOfRelationsAndConnectivity.end();
		//	originalXdataItr++)
		//{
		//	NSSTRING strKey = (*originalXdataItr).first;
		//	NewXdataItr = mapXDATA.find(strKey);
		//	if(NewXdataItr == mapXDATA.end())
		//	{
		//		mapXDATA.insert(STRING_MAP::value_type(strKey, (*originalXdataItr).second));
		//	}
		//}
		m_pLocalDWGMgr->addXDATA(lObjectId, mapXDATA);
		// release memory of mapXDATA
	}
}

NSSTRING CNSRelationLinker::getQueryString(NSSTRING strQueryString)
{
	NSSTRING strOutput;
	if(NSSTRCMP(strQueryString.c_str(), _T("")) == 0)
	{
		strOutput = _T(" is null");
	}
	else
	{
		strOutput = _T(" = \'") + strQueryString + _T("\'");
	}
	return strOutput;
}

/*
 @Brief		Logs an error if a proposed feature can have an OWNERSHIP_PARENT entry according to database  but doesn't have one.
*/
void CNSRelationLinker::validateOwnershipParent()
{
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapPropRelItr;
	for(mapPropRelItr = m_mapProposedRelations.begin(); mapPropRelItr != m_mapProposedRelations.end(); mapPropRelItr++)
	{
		LONG_PTR lObjectId = (*mapPropRelItr).first;
		CNSValidationDataCollector *pTempFeature = getFeature(lObjectId);

		std::vector<int> vectMDBOwnershipParents;
		getOwnershipParentsFromMDB(pTempFeature, vectMDBOwnershipParents);

		if(vectMDBOwnershipParents.size() == 0)
			continue;

		CNSProposedRelations *pTempPropRel = (*mapPropRelItr).second;
		int nParentFNO = pTempPropRel->getOwnershipParentFNO();

		bool bParentFound = false;
		std::vector<int>::iterator prentItr;
		for(prentItr = vectMDBOwnershipParents.begin(); prentItr != vectMDBOwnershipParents.end(); prentItr++)
		{
			if(((int)(*prentItr)) == nParentFNO)
			{
				bParentFound = true;
				break;
			}
		}

		if(bParentFound == false)
		{
			int nErr = 0;
			TCHAR szFormat[_MAX_PATH + 1];
			CNSPoint3D pt = pTempFeature->getInsertionPoint();
			if(pTempFeature->getFeatureType() == FEATURE_BLOCK)
			{
				//NSSPRINTF(szFormat,_T("Parent not found for block \'%s\' on layer \'%s\'. FID: \'%s\' %s"), pTempFeature->getBlockName().c_str(), pTempFeature->getLayerName().c_str(), pTempPropRel->getFID().c_str(), pTempFeature->getInsertionPointString().c_str());
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_17);

				nErr = WriteErrorRec(LOG_GIS,ERR_GIS_17,pTempFeature,pTempPropRel);
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_17, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
			else
			{
				//NSSPRINTF(szFormat,_T("Parent not found for feature with FID: \'%s\' on layer \'%s\' %s"), pTempPropRel->getFID().c_str(), pTempFeature->getLayerName().c_str(),  pTempFeature->getInsertionPointString().c_str());
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_32);

				nErr = WriteErrorRec(LOG_GIS,ERR_GIS_32,pTempFeature,pTempPropRel);
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_32, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
			
		}
	}
}

/*
 @Brief		If a cables is not connected at any of the ends it logs an error
*/
void CNSRelationLinker::validateCables()
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator mapCablesItr;
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapPropRelItr;
	for(mapCablesItr = m_mapLinearFeaturesToBeLinked.begin(); mapCablesItr != m_mapLinearFeaturesToBeLinked.end(); mapCablesItr++)
	{
		LONG_PTR lObjectId = (*mapCablesItr).first;
		CNSValidationDataCollector *pTempFeature = getFeature(lObjectId);

		mapPropRelItr = m_mapProposedRelations.find(lObjectId);
		CNSProposedRelations *pTempPropRel = (*mapPropRelItr).second;
		
		int nNumConAtNode1, nNumConAtNode2,nNumOfNodes=0;
		getNumberOfNodes(pTempFeature, nNumOfNodes);
		if(nNumOfNodes == 0)
			continue;
		nNumConAtNode1 = pTempPropRel->getNumConAtNode1();
		nNumConAtNode2 = pTempPropRel->getNumConAtNode2();

		TCHAR szFormat[_MAX_PATH + 1] = _T("");
		TCHAR strInsPt[_MAX_PATH + 1] = _T("");
		TCHAR strEndPt[_MAX_PATH + 1] = _T("");
		bool bNotConnected = false;
		bool bAtStart = false;
		bool bAtEnd = false;
		if((nNumConAtNode1 < 1) && (FEATURE_BUSBAR != pTempFeature->getFeatureType()))
		{
			bNotConnected = true;
			#ifndef _INTERNAL_
				NSSTRCPY(strInsPt, (pTempFeature->getInsertionPointString().size())+1, pTempFeature->getInsertionPointString().c_str());
			#else
				NSSTRCPY(strInsPt,pTempFeature->getInsertionPointString().c_str());
			#endif
			bAtStart = true;
		}
		
		if( nNumConAtNode2 < 1)
		{
			bNotConnected = true;
			#ifndef _INTERNAL_
				NSSTRCPY(strEndPt, (pTempFeature->getEndPointString().size())+1, pTempFeature->getEndPointString().c_str());
			#else
				NSSTRCPY(strEndPt, pTempFeature->getEndPointString().c_str());
			#endif
			bAtEnd = true;
		}

		if(bNotConnected && (FEATURE_BUSBAR != pTempFeature->getFeatureType()))
		{
			int nErr = 0;
		//	NSSPRINTF(szFormat,_T("Inadaquate electrical connectivity. FID: \'%s\' %s %s"), pTempPropRel->getFID().c_str(), strInsPt, strEndPt);
		//	NSSPRINTF(szFormat,_T("Inadaquate electrical connectivity. Required number of nodes for feature with FID: \'%s\' is \'%i\'"), pTempPropRel->getFID().c_str(), 2);
			CNSPoint3D pt = pTempFeature->getInsertionPoint();
            //NSSPRINTF(szFormat,_T("Inadaquate electrical connectivity at location %lf, %lf on layer \'%s\'. Required number of nodes for feature with FID: \'%s\' is \'%i\'"),  
			//pt.m_dXX, pt.m_dYY,pTempFeature->getLayerName().c_str(), pTempPropRel->getFID().c_str(), nNumOfNodes);
			//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_18);
			//int nErr = WriteToLog(LOG_GIS, szFormat);
		     

			 TCHAR strNumOfNodes[_MAX_PATH + 1] = _T("");
			 NSSPRINTF(strNumOfNodes,_T("%i"), nNumOfNodes);
			 nErr = WriteErrorRecEx(LOG_GIS,ERR_GIS_18,pTempFeature,pTempPropRel,_T("N$"),strNumOfNodes);


			 
			//nErr draw error circle
			if(bAtStart == true)
			{
				pt = pTempFeature->getInsertionPoint();
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_18, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
			if(bAtEnd == true)
			{
				pt = pTempFeature->getEndPoint();
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_18, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
		}
	}
}

/*
 @Brief		If a Block feature is not connected it logs an error
*/
void CNSRelationLinker::validateBlockFeatures()
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator mapBlockFeatureItr;
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapPropRelItr;
	for(mapBlockFeatureItr = m_mapBlocksToBeLinked.begin(); mapBlockFeatureItr != m_mapBlocksToBeLinked.end(); mapBlockFeatureItr++)
	{
		LONG_PTR lObjectId = (*mapBlockFeatureItr).first;
		CNSValidationDataCollector *pTempFeature = getFeature(lObjectId);
		
		mapPropRelItr = m_mapProposedRelations.find(lObjectId);
		CNSProposedRelations *pTempPropRel = (*mapPropRelItr).second;
		
		int nNumOfNodes = 0;
		int nNumConAtNode1 = 0;
		int nNumConAtNode2 = 0; 
		getNumberOfNodes(pTempFeature, nNumOfNodes);
		nNumConAtNode1 = pTempPropRel->getNumConAtNode1();
		nNumConAtNode2 = pTempPropRel->getNumConAtNode2();

		TCHAR szFormat[_MAX_PATH + 1] = _T("");
		TCHAR strInsPt[_MAX_PATH + 1] = _T("");

		bool bNotConnected = false;
		
		if((nNumOfNodes>= 1) && (nNumConAtNode1 < 1))
		{
			bNotConnected = true;
			#ifndef _INTERNAL_
				NSSTRCPY(strInsPt, (pTempFeature->getInsertionPointString().size())+1, pTempFeature->getInsertionPointString().c_str());
			#else
				NSSTRCPY(strInsPt,pTempFeature->getInsertionPointString().c_str());
			#endif
		}
		
		if((bNotConnected == false) && (nNumOfNodes == 2) && (nNumConAtNode2 < 1))
		{
			bNotConnected = true;
			#ifndef _INTERNAL_
				NSSTRCPY(strInsPt, (pTempFeature->getInsertionPointString().size())+1, pTempFeature->getInsertionPointString().c_str());
			#else
				NSSTRCPY(strInsPt,pTempFeature->getInsertionPointString().c_str());
			#endif
		}

		if(bNotConnected)
		{
			int nErr;
			CNSPoint3D pt = pTempFeature->getInsertionPoint();
			//NSSPRINTF(szFormat,_T("Inadaquate electrical connectivity for block \'%s\' at location %lf, %lf. Required number of nodes for feature with FID: \'%s\' is \'%i\'"), 
			//pTempFeature->getBlockName().c_str(), pt.m_dXX, pt.m_dYY, pTempPropRel->getFID().c_str(), nNumOfNodes);
		    
			//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_19);

			TCHAR strNumOfNodes[_MAX_PATH + 1] = _T("");
			NSSPRINTF(strNumOfNodes,_T("%i"), nNumOfNodes);
			nErr = WriteErrorRecEx(LOG_GIS,ERR_GIS_19,pTempFeature,pTempPropRel,_T("N$"),strNumOfNodes);

			gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_19, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
		}
	}
}


/*
 @Brief		Removes Invalid features
*/
void CNSRelationLinker::removeInvalidFeatures()
{	
	std::map<LONG_PTR, LONG_PTR> mapOfInvalidIds;
	std::map<LONG_PTR, LONG_PTR>::iterator mapValidIdItr, mapInvalidIdItr;
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator mapPropBlockItr, mapCableItr, mapExtBlkItr, mapExtCableItr;

	// Find Invalid Ids by comparing Ids in maps in Relationlinker with gMapOfValidIDs
	//
	for(mapPropBlockItr = m_mapBlocksToBeLinked.begin(); mapPropBlockItr != m_mapBlocksToBeLinked.end(); mapPropBlockItr++)
	{
		LONG_PTR lBlkId = (*mapPropBlockItr).first;
		mapValidIdItr = gMapOfValidIDs.find(lBlkId);
		if(mapValidIdItr == gMapOfValidIDs.end())
		{
			mapOfInvalidIds.insert(std::map<LONG_PTR, LONG_PTR>::value_type(lBlkId, nNumberNotFound)); // second value in yhe map is dummy and is not used anywhere
		}
	}
	for(mapCableItr = m_mapLinearFeaturesToBeLinked.begin(); mapCableItr != m_mapLinearFeaturesToBeLinked.end(); mapCableItr++)
	{
		LONG_PTR lBlkId = (*mapCableItr).first;
		mapValidIdItr = gMapOfValidIDs.find(lBlkId);
		if(mapValidIdItr == gMapOfValidIDs.end())
		{
			// As per client request, linear features which have NumOfNodes=0 must be excluded from conn/reln
			// We were assuming that all cables will by deafult have NumOfNodes =0 which is not the case

			mapOfInvalidIds.insert(std::map<LONG_PTR, LONG_PTR>::value_type(lBlkId, nNumberNotFound)); // second value in yhe map is dummy and is not used anywhere
		}
	}
	for(mapExtBlkItr = m_mapExtBlocks.begin(); mapExtBlkItr != m_mapExtBlocks.end(); mapExtBlkItr++)
	{
		LONG_PTR lBlkId = (*mapExtBlkItr).first;
		mapValidIdItr = gMapOfValidIDs.find(lBlkId);
		if(mapValidIdItr == gMapOfValidIDs.end())
		{
			mapOfInvalidIds.insert(std::map<LONG_PTR, LONG_PTR>::value_type(lBlkId, nNumberNotFound)); // second value in yhe map is dummy and is not used anywhere
		}
	}
	for(mapExtCableItr = m_mapExtCablesToBeLinked.begin(); mapExtCableItr != m_mapExtCablesToBeLinked.end(); mapExtCableItr++)
	{
		LONG_PTR lBlkId = (*mapExtCableItr).first;
		mapValidIdItr = gMapOfValidIDs.find(lBlkId);
		if(mapValidIdItr == gMapOfValidIDs.end())
		{
			mapOfInvalidIds.insert(std::map<LONG_PTR, LONG_PTR>::value_type(lBlkId, nNumberNotFound)); // second value in yhe map is dummy and is not used anywhere
		}
	}
	//
	
	
	// Remove the Invalid ids
	//
	std::map<LONG_PTR, LONG_PTR>::iterator mapInvalidFtItr;
	for(mapInvalidIdItr = mapOfInvalidIds.begin(); mapInvalidIdItr != mapOfInvalidIds.end(); mapInvalidIdItr++)
	{	
		LONG_PTR lInvalidID = (*mapInvalidIdItr).first;
		bool bIdFound = false;
		
		mapPropBlockItr = m_mapBlocksToBeLinked.find(lInvalidID);
		if(mapPropBlockItr != m_mapBlocksToBeLinked.end())
		{
			m_mapBlocksToBeLinked.erase(mapPropBlockItr);			// Invald Id found, Erase it
			bIdFound = true;
		}

		if(!bIdFound)
		{
			mapCableItr = m_mapLinearFeaturesToBeLinked.find(lInvalidID);
			if(mapCableItr != m_mapLinearFeaturesToBeLinked.end())
			{
				m_mapLinearFeaturesToBeLinked.erase(mapCableItr);	// Invald Id found, Erase it
				bIdFound = true;
			}
		}

		if(!bIdFound)
		{
			mapExtBlkItr = m_mapExtBlocks.find(lInvalidID);
			if(mapExtBlkItr != m_mapExtBlocks.end())
			{
				m_mapExtBlocks.erase(mapExtBlkItr);					// Invald Id found, Erase it
			}
		}

		if(!bIdFound)
		{
			mapExtCableItr = m_mapExtCablesToBeLinked.find(lInvalidID);
			if(mapExtCableItr != m_mapExtCablesToBeLinked.end())
			{
				m_mapExtCablesToBeLinked.erase(mapExtCableItr);					// Invald Id found, Erase it
			}
		}
	}
	//
	
	// Remove features not having FIDs and FNOs
}


/*
 @Brief		populates this map with the objectIds and CNSProposedRelations objects
			the objectIds are of features to which relationship XDATA is to be written
*/
void CNSRelationLinker::populateProposedRelMap()
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator blocksToBeLinkedItr, cablesToBeLinkedItr;
	CNSValidationDataCollector* pValidationDataCollectorObj;
	
	for(blocksToBeLinkedItr = m_mapBlocksToBeLinked.begin(); blocksToBeLinkedItr != m_mapBlocksToBeLinked.end(); blocksToBeLinkedItr++)
	{
		pValidationDataCollectorObj  = (*blocksToBeLinkedItr).second;
		CNSProposedRelations *pTempProposedRel = new CNSProposedRelations();
		
		pTempProposedRel->setFID(pValidationDataCollectorObj->getFID());
		
		m_mapProposedRelations.insert(std::map<LONG_PTR, CNSProposedRelations*>::value_type(pValidationDataCollectorObj->getObjectId(), pTempProposedRel));
	}

	for(cablesToBeLinkedItr = m_mapLinearFeaturesToBeLinked.begin(); cablesToBeLinkedItr != m_mapLinearFeaturesToBeLinked.end(); cablesToBeLinkedItr++)
	{
		pValidationDataCollectorObj  = (*cablesToBeLinkedItr).second;

		CNSProposedRelations *pTempProposedRel = new CNSProposedRelations();
		
		pTempProposedRel->setFID(pValidationDataCollectorObj->getFID());
		m_mapProposedRelations.insert(std::map<LONG_PTR, CNSProposedRelations*>::value_type(pValidationDataCollectorObj->getObjectId(), pTempProposedRel));
	}
}

bool CNSRelationLinker::checkforOrientation(CNSValidationDataCollector *pFeature , CNSValidationDataCollector *pProxFeature)
{
	if(pFeature == NULL || pProxFeature == NULL)
		return false;
    NSSTRING strFeatureOrientation, strProxyFeatureOrientation; 
    if(pFeature->getFeatureType() == FEATURE_BLOCK)
    {
	    strFeatureOrientation = pFeature->readAttribute(_T("ORIENTATION"));
    }
    else if(pFeature->getFeatureType() == FEATURE_LINEAR || pFeature->getFeatureType() == FEATURE_ARC)
    {
	     retrieveLinearFeatOrientation(pFeature, strFeatureOrientation);
    }
    
    if(pProxFeature->getFeatureType() == FEATURE_BLOCK)
    {
	    strProxyFeatureOrientation = pProxFeature->readAttribute(_T("ORIENTATION"));
    }
    else if(pProxFeature->getFeatureType() == FEATURE_LINEAR || pProxFeature->getFeatureType() == FEATURE_ARC)
    {
	     retrieveLinearFeatOrientation(pProxFeature, strProxyFeatureOrientation);
    }

	//	NSSTRUPR((TCHAR*)strFeatureOrientation.c_str());
	//	NSSTRUPR((TCHAR*)strProxyFeatureOrientation.c_str());

	if(NSSTRCMP(strFeatureOrientation.c_str(), strProxyFeatureOrientation.c_str()) != 0)//Orientation is not same
			return false; // to open if required

	return true;
}

bool CNSRelationLinker::checkforUsage(CNSValidationDataCollector *pFeature , CNSValidationDataCollector *pProxFeature)
{
	if(pFeature == NULL || pProxFeature == NULL)
		return false;
    NSSTRING strFeatureUsage, strProxyFeatureUsage; 
    if(pFeature->getFeatureType() == FEATURE_BLOCK)
    {
	    strFeatureUsage = pFeature->readAttribute(_T("USAGE"));
    }
    else if(pFeature->getFeatureType() == FEATURE_LINEAR || pFeature->getFeatureType() == FEATURE_ARC )
    {
	     retrieveLinearFeatUsage(pFeature, strFeatureUsage);
    }
    
    if(pProxFeature->getFeatureType() == FEATURE_BLOCK)
    {
	    strProxyFeatureUsage = pProxFeature->readAttribute(_T("USAGE"));
    }
    else if(pProxFeature->getFeatureType() == FEATURE_LINEAR || pProxFeature->getFeatureType() == FEATURE_ARC )
    {
	     retrieveLinearFeatUsage(pProxFeature, strProxyFeatureUsage);
    }

//	NSSTRUPR((TCHAR*)strFeatureUsage.c_str());
//	NSSTRUPR((TCHAR*)strProxyFeatureUsage.c_str());
	if(NS_FAIL == compareTokenValues(strFeatureUsage , strProxyFeatureUsage))
		return false;
	//if(NSSTRCMP(strFeatureUsage.c_str(), strProxyFeatureUsage.c_str()) != 0)//Usage is not same
	//		return false;
	return true;
}

int CNSRelationLinker::compareTokenValues(NSSTRING strActValue, NSSTRING strAllowedValue)
{
	STRING_MAP::iterator ActValueItr, AllowedValueItr;
	STRING_MAP mapOfActValues, mapOfAllowedValues;
	if(strActValue.length() == 0)
	{
		mapOfActValues.insert(STRING_MAP::value_type(_T(""), _T("")));
	}
	tokenize(strActValue, mapOfActValues);
	tokenize(strAllowedValue, mapOfAllowedValues);
	for(ActValueItr = mapOfActValues.begin(); ActValueItr != mapOfActValues.end(); ActValueItr++)
	{
		AllowedValueItr = mapOfAllowedValues.find((*ActValueItr).first);
		if(AllowedValueItr != mapOfAllowedValues.end())
		{
			return NS_SUCCESS;
		}
	}
	return NS_FAIL;
}

int CNSRelationLinker::tokenize(NSSTRING strInputString, STRING_MAP &mapOfValues)
{
	TCHAR* pszTemp = NULL;
	TCHAR* pszSep = _T(",");
	CNSPoint3D ptTemp;
	int nPointCounter = 0;
	int nCordinateCounter = 1;

	// Establish string and get the first token
	TCHAR* pszNextToken;   //dummy 
	# ifndef _INTERNAL_
		pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep, &pszNextToken);
	#else
		pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep, &pszNextToken);
		//pszTemp = NSSTRTOK((TCHAR*)strInputString.c_str(), pszSep);
	#endif
	
	// While there are tokens in "string"
	while( pszTemp != NULL )
	{
		NSSTRING strTemp = pszTemp;
		#ifndef _INTERNAL_
			NSSTRUPR(pszTemp, strTemp.size()+1);
		#else
			NSSTRUPR(pszTemp, strTemp.size()+1);
			//NSSTRUPR(pszTemp);
		#endif		
		
		mapOfValues.insert(STRING_MAP::value_type(pszTemp, _T("")));
		// Get next token
		# ifndef _INTERNAL_
			pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
		#else
			pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
			//pszTemp = NSSTRTOK( NULL, pszSep);
		#endif

	}
	return NS_SUCCESS;
}

/*
 @Brief		Logs an error if a proposed feature can have an OWNERSHIP_PARENT entry according to database  but doesn't have one.
 			developed by Subir on 10/12/2008
*/
void CNSRelationLinker::validateManyOwnershipChilds()
{
	std::map<LONG_PTR, CNSProposedRelations*>::iterator mapPropRelItr;
	for(mapPropRelItr = m_mapProposedRelations.begin(); mapPropRelItr != m_mapProposedRelations.end(); mapPropRelItr++)
	{
		LONG_PTR lObjectId = (*mapPropRelItr).first;
		CNSValidationDataCollector *pTempFeature = getFeature(lObjectId);

		std::vector<int> vectMDBManyOwnershipChilds;
		getManyOwnershipChilds(pTempFeature,  vectMDBManyOwnershipChilds);

		if(vectMDBManyOwnershipChilds.size() == 0)
			continue;

		CNSProposedRelations *pTempPropRel = (*mapPropRelItr).second;
		NSSTRING strManyOwnershipFID = pTempPropRel->getManyOwnershipFID();

		if (strManyOwnershipFID.empty())
		{
			int nErr = 0;
			TCHAR szFormat[_MAX_PATH + 1];
			CNSPoint3D pt = pTempFeature->getInsertionPoint();
			if(pTempFeature->getFeatureType() == FEATURE_BLOCK)
			{
				/*if(NSSTRCMP(pTempFeature->readAttribute(_T("ORIENTATION")).c_str(), _T("UG")) == 0)
						continue;*/

                nErr = WriteErrorRec(LOG_GIS,ERR_GIS_33,pTempFeature,pTempPropRel);
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_33, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
			else
			{
				if(NSSTRCMP(getCableOrientation(pTempFeature).c_str(), _T("UG")) == 0)
						continue;

				nErr = WriteErrorRec(LOG_GIS,ERR_GIS_34,pTempFeature,pTempPropRel);
				gErrorCircle.drawCircle(nErr, LOG_GIS, ERR_GIS_34, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			}
			
		}
	}
}

/*
 @Brief		To establish relationship between blocks based on common attribute values
 			developed by Subir on 10/12/2008
*/

bool  CNSRelationLinker::checkBlocksBasedOnAttribites(CNSValidationDataCollector* pFeature, CNSValidationDataCollector* pFeatureToBeLinked)
{
	bool bAttBasedRelFlag = false;
	NSSTRING strParentID,strParent;

	int iFeatureFNO = pFeature->getFNO();
	int iFeatureToBeLinkedFNO =  pFeatureToBeLinked->getFNO();

	TCHAR szQuery[_MAX_PATH + 1];
	NSSPRINTF(szQuery, _T("Select COMMON_ATT_NAME from ParentChild where SOURCEFNO = %d and OWNERFNO  = %d"),iFeatureFNO,iFeatureToBeLinkedFNO);        
	int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
    
	NSSTRING strCMN_ATT_NEM;
	if(-1 != nDBRet)
	{
		    NSSTRING strCOMMON_ATT_NAME  = _T("COMMON_ATT_NAME");

			bool bval = false;
			variant_t valCOMMON_ATT_NAME;
			 
			bval = CNSDatabaseMgr::getInstance()->getValue(strCOMMON_ATT_NAME.c_str(), valCOMMON_ATT_NAME);	

			int iErrNo = -1;
			
			if(bval)
			{
				#ifndef _INTERNAL_
						strCMN_ATT_NEM  = valCOMMON_ATT_NAME.bstrVal;						
				#else						
						strCMN_ATT_NEM  = valCOMMON_ATT_NAME.bstrVal;
						/*char *strvalue ;				        					
						strvalue = _com_util::ConvertBSTRToString(valCOMMON_ATT_NAME.bstrVal); strCMN_ATT_NEM = strvalue;*/
				#endif
			}
			else
			{
				NSSPRINTF(szQuery, _T("Select COMMON_ATT_NAME from ParentChild where SOURCEFNO = %d and OWNERFNO  = %d"),iFeatureToBeLinkedFNO,iFeatureFNO); 

				int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);

				if(-1 != nDBRet)
				{
						NSSTRING strCOMMON_ATT_NAME  = _T("COMMON_ATT_NAME");

						bool bval;
						variant_t valCOMMON_ATT_NAME;
						 
						bval = CNSDatabaseMgr::getInstance()->getValue(strCOMMON_ATT_NAME.c_str(), valCOMMON_ATT_NAME);	

						int iErrNo = -1;
						
						if(bval)
						{
							#ifndef _INTERNAL_
									strCMN_ATT_NEM  = valCOMMON_ATT_NAME.bstrVal;						
							#else						
									strCMN_ATT_NEM  = valCOMMON_ATT_NAME.bstrVal;	
									/*char *strvalue ;				        					
									strvalue = _com_util::ConvertBSTRToString(valCOMMON_ATT_NAME.bstrVal); strCMN_ATT_NEM = strvalue;*/
							#endif
						}
				}


			}
	}
	


	strParentID = pFeature->readAttribute(strCMN_ATT_NEM);
	strParent   = pFeatureToBeLinked->readAttribute(strCMN_ATT_NEM);


	if(!(strParentID.empty()))
	{
		if(!(strParent.empty()))
		{
			if(NSSTRCMP(strParentID.c_str(),strParent.c_str()) == 0)
			{
				bAttBasedRelFlag = true;
			}
		}
	}

	/*strParentID = pFeatureToBeLinked->readAttribute(strCMN_ATT_NEM);
	strParent   = pFeature->readAttribute(strCMN_ATT_NEM);


	if(!(strParentID.empty()))
	{
		if(!(strParent.empty()))
		{
			if(NSSTRCMP(strParentID.c_str(),strParent.c_str()) == 0)
			{
				bAttBasedRelFlag = true;
			}
		}
	}	*/

return bAttBasedRelFlag;

}

/*
 @Brief	:	need to clear the XData(Relationship and connectivity only
			of new features if it already there
*/

void  CNSRelationLinker::clearExistingXDataForNew()
{
	std::map<LONG_PTR, CNSValidationDataCollector*>::iterator valDataItr;
	//Clear for new blocks to be linked
	for(valDataItr = m_mapBlocksToBeLinked.begin(); valDataItr != m_mapBlocksToBeLinked.end(); valDataItr++)
	{
		CNSValidationDataCollector* pTempFeature = (*valDataItr).second;
		if(pTempFeature)
		{
			LONG_PTR lObjectId = pTempFeature->getObjectId();
			STRING_MAP mapXDATA;
			initializeMap(mapXDATA);
			m_pLocalDWGMgr->addXDATA(lObjectId, mapXDATA);
		}
	}
	//Clear for new linear features to be linked
	for(valDataItr = m_mapLinearFeaturesToBeLinked.begin(); valDataItr != m_mapLinearFeaturesToBeLinked.end(); valDataItr++)
	{
		CNSValidationDataCollector* pTempFeature = (*valDataItr).second;
		if(pTempFeature)
		{
			LONG_PTR lObjectId = pTempFeature->getObjectId();
			STRING_MAP mapXDATA;
			initializeMap(mapXDATA);
			m_pLocalDWGMgr->addXDATA(lObjectId, mapXDATA);
		}
	}
}

/*
 @Brief		Initialize the XData map
*/
void CNSRelationLinker::initializeMap(STRING_MAP &mapXDATA)
{
	mapXDATA.clear();
	mapXDATA.insert(STRING_MAP::value_type(_T("OWNERSHIP_PARENT"), _T("")));
	mapXDATA.insert(STRING_MAP::value_type(_T("OWNERSHIP_CHILD"), _T("")));
	mapXDATA.insert(STRING_MAP::value_type(_T("MANYOWNERSHIP_CHILD"), _T("")));
	mapXDATA.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE1"), _T("")));
	mapXDATA.insert(STRING_MAP::value_type(_T("CONNECTIVITY_NODE2"), _T("")));
}
