#pragma once
#pragma warning( disable: 4251)

#include "NSPoint3D.h"
#include <map>
#include "string"
#include "..\Include\NSDirectives.h"
#include "..\Include\NSCompositeGeometry.h"
typedef std::map<NSSTRING, NSSTRING> STRING_MAP;
#define NS_ERR_GHOST_BLOCK -2
#define NS_GHOST_BLOCK_NOT_FOUND -3
#define NS_NEW_LAYER_ADDED -4
#define NS_BYLAYER_SET -5
#define NS_ERR_CIRCLE_NOT_FOUND -6
#define NS_ERR_PAPERSPACE -7

enum RelationType
{
	REL_NODE1 = 0,
	REL_NODE2 = 1,
	REL_PARENT = 2,
	REL_CHILD = 3,
	REL_MANYOWNERSHIP = 4
};

class CNSFeatureContainer ; // Forward declarartion 
class CNSRelationInfo ; // Forward declarartion 

// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft
class CNSNestedBlockInfo
{
public:
	CNSNestedBlockInfo(void){}
public:
	~CNSNestedBlockInfo(void){;}

public:
//Methods to find/add record in the map
	
//int createFeatures(NSSTRING strLayerType, NSSTRING strFeatureType, FeatureCategory enCategory, std::vector<CNSFeatureContainer*> &vectFeatureContainer, std::vector<CNSFeature*> &vectFeatures);
void createAnewSubFeature(NSSTRING strFID ,CNSFeatureContainer *pSubFeatContainer);
std::map<NSSTRING, CNSFeatureContainer *> getSubFeatureMap();

private:
	std::map<NSSTRING, CNSFeatureContainer *> m_SubFeaturesMap;
	//std::map<long, long> m_SubFeaturesMap;

};

class _declspec(dllexport) CNSRelationInfo
{
	public:
	CNSRelationInfo();
	~CNSRelationInfo();

	private:
	NSSTRING m_strFID;
	NSSTRING m_strFNO;
	NSSTRING m_strConnectivtyNode1;
	NSSTRING m_strConnectivtyNode2;
	NSSTRING m_strParent;
	NSSTRING m_strChild;
	NSSTRING m_strManyOwnership;

	//Get/Set methods to retrieve/store the info
public:

	bool isParentReferenced(enum RelationType) ;
	NSSTRING getIndexAtPNode2();
	void replaceParentReferenc(enum RelationType,const NSSTRING strActualStringtoReplaceWith, NSSTRING strPNodeIndex = _T("")) ;
	void getFIDFNOs(NSSTRING strXDATA, std::map<NSSTRING,NSSTRING> &mapOfFIDFNOs);

	NSSTRING getFID();
	NSSTRING getFNO();
	NSSTRING getConnectivtyNode1();
	NSSTRING getConnectivtyNode2();
	NSSTRING getParent();
	NSSTRING getChild();
	NSSTRING getManyOwnership();

	void setFID(const NSSTRING strFID);
	void setFNO(const NSSTRING strFNO);
	void setConnectivtyNode1(const NSSTRING strConnectivtyNode1);
	void setConnectivtyNode2(const NSSTRING strConnectivtyNode2);
	void setParent(const NSSTRING strParent);
	void setChild(const NSSTRING strChild);
	void setManyOwnership(const NSSTRING strManyOwnership);

	//Methods:
	//RelationType 			
    //{REL_NODE1,REL_NODE2,REL_PARENT,REL_CHILD,REL_MANYOWNERSHIP}

	
};

// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft

class  _declspec(dllexport) CNSAttributeRef
{
public:
	CNSAttributeRef(void){m_pCompGeometry = NULL;};
	~CNSAttributeRef(void){};
public:
	void setAttributeValue(const NSSTRING strAttrValue){m_strValue = strAttrValue;};
	NSSTRING getAttributeValue() const{return m_strValue;};
	CNSCompositeGeometry* getCompGeometry(){return m_pCompGeometry;};
	NSSTRING getTagName(){return m_strTagName;};
	bool getIsVisible(){return m_bIsVisible;};

	void setCompGeometry(CNSCompositeGeometry *pCompGeometry){m_pCompGeometry = pCompGeometry;};
	void setTagName(const NSSTRING strTagName){m_strTagName = (NSSTRING)strTagName;};
	void setIsVisible(bool bIsVisible){m_bIsVisible = bIsVisible;};
private:
	NSSTRING m_strValue;
	CNSCompositeGeometry *m_pCompGeometry;
	NSSTRING m_strTagName;
	bool m_bIsVisible;
};
typedef std::map<int, CNSAttributeRef*> ATTREF_MAP;
class  _declspec(dllexport) CNSAttributeRefMap
{
public:
	CNSAttributeRefMap(void);
	~CNSAttributeRefMap(void);
public:
	void addAttRef(int nCID, CNSAttributeRef *pAttrRef);
	CNSAttributeRef* getAttrRef(int nCID);
	int getSize();
	CNSAttributeRef* getAt(int nIndex);
	int getAttrReferences(ATTREF_MAP &MapOfRefs);
private:
	ATTREF_MAP m_AttrRefMap;
};

typedef std::map<NSSTRING, CNSAttributeRefMap*> ATT_MAP;

class _declspec(dllexport) CNSFeatureContainer
{
public:
	// methods
	CNSFeatureContainer(void);
	~CNSFeatureContainer(void){};
public:
	void setComponentContainer(CNSCompositeContainer* container);
	void setBlockAttributes(STRING_MAP* mapAttributes);
	void setXData(STRING_MAP* mapXData);
	void setAttributeMap(ATT_MAP* mapOfAtt);
	CNSCompositeContainer* getComponentContainer();
	ATT_MAP* getAttributeMap();
	STRING_MAP* getBlockAttributes();
	STRING_MAP* getXData();
	void setLayerName(NSSTRING strLayer);
	void setTypeName(NSSTRING strType);
	NSSTRING getLayerName();
	NSSTRING getTypeName();
//	void addAttributeRef(NSSTRING strKey, CNSAttributeRef* pAttrRef);
	void addAttributeRef(NSSTRING strKey, int nCID, CNSAttributeRef* pAttrRef);
	void updateAttributeRef(NSSTRING strKey, int nCID, CNSAttributeRef* pAttrRef);

//	CNSAttributeRef* findAttributeRef(NSSTRING strKey);
	CNSAttributeRef* findAttributeRef(NSSTRING strKey, int nCID);
	void release();

// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft
	void createAnewSubFeature(CNSFeatureContainer *pSubFeatContainer);
	void getSubFeatureMap(std::map<NSSTRING, CNSFeatureContainer *> &mapOfSubFeatures);
// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft
	COMPONENT_TYPE getCompType(int nCompNumber);
	

public:
	CNSCompositeContainer *m_pContainer;
//	STRING_MAP m_mapBlkAtt;
	ATT_MAP *m_mapOfBlkAtt;
	STRING_MAP *m_mapOfXData;
	STRING_MAP m_mapOfParentAttributes;

private:
	bool m_bIsNestedFeature;
	bool m_bIsMultipleFeature;

private:
	NSSTRING m_strlayerName;
	NSSTRING m_strType;

// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft
private:
	CNSNestedBlockInfo *m_pNestedBlockInfo;
	CNSRelationInfo* m_pRelationInfo;

public:
	void setRelation(NSSTRING strAppName, NSSTRING strAppValue, CNSFeatureContainer *pNestedFeatContainer);
	int isParentReferenced();
	NSSTRING getIndexAtPNode2();
	void replaceParentReferenc(enum RelationType RelType,const NSSTRING strActualStringtoReplaceWith, NSSTRING iPNodeIndex = _T("") );
	CNSRelationInfo*  getRelationInfo();

	NSSTRING getXData(NSSTRING strAppName);
	void setXData(NSSTRING strAppName, NSSTRING strAppValue);
	void SetAssetNumberFromParent();
	bool getNestedFeatureFlag();
	bool getMultipleFeatureFlag();
	void setNestedFeatureFlag(bool bNewVal);
	void setMultipleFeatureFlag(bool bNewVal);
// introduced in Phase II enhancement for Nested Block on 09.04.09 by Subir @ Neilsoft

};


class _declspec(dllexport) CNSBlock: public CNSCompositeData//_declspec(dllexport)
{
public:
	// methods
	CNSBlock(void);
	~CNSBlock(void);

	CNSBlock(NSSTRING pszBlockName, CNSPoint3D ptBlockInsertionPoint, double dRotation);

	NSSTRING getBlockName();
	void getInsertionpoint(CNSPoint3D& Pt);
	double getRotation();
	int getAttributeValue(NSSTRING pszKey, NSSTRING& pszValue);
	int getAlignment();

	void setBlockName(NSSTRING pszBlockName);
	void setInsertionPoint(CNSPoint3D ptBlockInsertionPoint);
	void setInsertionPoint(double dX, double dY, double dZ = 0);
	void setRotation(double dRotation);
	void addAttribute(NSSTRING pszKey, NSSTRING pszValue);
	void setAlignment(int nAlignment);
	//void addXDATA(std::map<NSSTRING, NSSTRING> &mapXDataRead);

	//Private:
	// Members
	NSSTRING	m_pszBlockName;
//	CNSPoint3D	m_ptBlockInsertionPoint;
	double		m_dRotation;				// Block rotation angle in radian
	int		m_nAlignment;
//	std::map<NSSTRING, NSSTRING> m_mapAttributes;
//	std::map<NSSTRING, NSSTRING> m_mapXDATA;

};


class _declspec(dllexport)CNSTextInfo
{
public:
	// methods
	CNSTextInfo(void)
    {
        m_InsertionPt.m_dXX = 0;
        m_InsertionPt.m_dYY = 0;
        m_nAlignment = 0;
        m_dRotation = 0;
    }
    ~CNSTextInfo(void){};
    CNSPoint3D m_InsertionPt;
    NSSTRING m_strText;
    int m_nAlignment;
    double m_dRotation;
};

typedef std::vector<CNSTextInfo*> TEXTINFO_VECTOR;


//#pragma once


