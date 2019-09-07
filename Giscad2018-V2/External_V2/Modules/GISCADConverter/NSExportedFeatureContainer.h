/*!
 @Class		:	CNSExportedFeatureContainer
 @Brief		:	This class provides the functionality to read Exported Feature
				information from XML.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
#include "vector"

#include "..\..\Include\NSDatabaseMgr.h"

#include "NSTElement.h"
#include "NSFeature.h"


class CNSExportedFeatureContainer
{
public:
	CNSExportedFeatureContainer(void);
	int readXML(CNSTElement &element);
	int writeDWG();
	std::vector<CNSFeature*> getFeatures();
	void setObject(CNSDWGMgr* pDwgMgr);
	void setObject(CNSDatabaseMgr* pDbMgr);
	int getFeatureCount() const;
public:
	~CNSExportedFeatureContainer(void);
private:
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	std::vector<CNSFeature*> m_vectFeatures;
	int m_nFeatureCount;
};
