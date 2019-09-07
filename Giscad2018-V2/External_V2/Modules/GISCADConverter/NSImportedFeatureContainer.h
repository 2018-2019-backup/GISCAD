/*!
 @Class		:	CNSImportedFeatureContainer
 @Brief		:	This class is responsible for reading CAD information from DWG
				and write into XML.
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include "..\..\Include\NSDirectives.h"
#include "..\..\Include\NSDWGMgr.h"
#include "..\..\Include\NSDatabaseMgr.h"
#include "NSTElement.h"
#include "NSFeature.h"

class CNSImportedFeatureContainer
{
public:
	CNSImportedFeatureContainer(void);
	int readDWG();
	int writeXML();
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	void setElement(CNSTElement* pElement);
	int getModFeatures();
	int getNewFeatures();
	int getDelFeatures();
public:
	~CNSImportedFeatureContainer(void);
private:
	int createFeatures(NSSTRING strLayerType, NSSTRING strFeatureType, FeatureCategory enCategory, std::vector<CNSFeatureContainer*> &vectFeatureContainer, std::vector<CNSFeature*> &vectFeatures);
	int writeXMLFeatureNode(CNSTElement* pElement, CNSFeature* pFeature);
	int createDelPolylines(std::vector<CNSFeatureContainer*> &vectDelCableContainer);
	void modifyCableEndConnections();
private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
private:
	CNSTElement *m_pElement;
	std::vector<CNSFeature*> m_vectModifiedFeatures;
	std::vector<CNSFeature*> m_vectNewFeatures;
	std::vector<CNSCompositeGeometry*> m_vectNewLinearFeatures;
	std::map<NSSTRING, NSSTRING> m_mapDelFeatures;
	int m_nNewFeatures;
	int m_nModFeatures;
	int m_nDelFeatures;
	int m_nFeatureNumber;

	std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer;	


	bool isNestedFeature(CNSFeatureContainer* aNewFeature);
	NSSTRING getATTVal(NSSTRING tagname, CNSFeatureContainer* aNewFeature);
	NSSTRING getdataOf(NSSTRING strKey,STRING_MAP *pDataMap );
	int getBitVal(int iDesimalValue, int iBitPos);
	void getNestedFeatures(std::vector<CNSFeatureContainer*> vectNewContainer, std::map<NSSTRING, CNSFeatureContainer*> &mapNestedFeatureContainer);
	void normalizeSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> &mapNestedFeatureContainer);
	void updateSubFeaturesInterRelationShip(NSSTRING strParentFID, CNSRelationInfo* pRelationInfoOfSubFeat);
	void replaceAllInstance(NSSTRING &stringToRepalce, NSSTRING strSearchChar ,NSSTRING stringToRepalceWith);
	void updateFeatures(std::vector<CNSFeatureContainer*> &vectFeatureContainer);
	void getFIDFNOs(NSSTRING strXDATA,std::map<NSSTRING,NSSTRING> &mapOfFIDFNOs);
	void update(NSSTRING strFID, NSSTRING strXDATA,enum RelationType RelType, CNSFeatureContainer *pFeatureContainer);

	void createSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer);
	int createSubFeature(NSSTRING strLayerType, NSSTRING strFeatureType, FeatureCategory enCategory, std::vector<CNSFeatureContainer*> &vectFeatureContainer, std::vector<CNSFeature*> &vectFeatures);
	void finalizeSubFeatures(std::map<NSSTRING, CNSFeatureContainer*> mapNestedFeatureContainer);
};
