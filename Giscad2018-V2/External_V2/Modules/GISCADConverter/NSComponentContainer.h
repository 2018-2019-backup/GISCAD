/*!
 @Class			CNSComponentContainer
 @Brief			This class provides functionality to hold
				Components information and read and write components information
				into XML.
 @Author		Neilsoft Ltd
 @Date			07-06-2007
 @History 		Change history
 */
#pragma once
#include "vector"

#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSBlock.h"

#include "NSComponent.h"
#include "NSTElement.h"
#include "NSAttribute.h"


class CNSComponentContainer
{
public:
	CNSComponentContainer(void);
	int readXML(CNSTElement &element);
	int readDWG();
	int writeXML();
	int writeDWG();
	void setObject(CNSDWGMgr* pDWGMgr);
	void setObject(CNSDatabaseMgr* pDBMgr);
	void setObject(CNSBlock* pBlk);
	void setElement(CNSTElement* pElement);
	void setXData(STRING_MAP& mapOfXData);
	void setObject(CNSCompositeContainer *pCompositeContainer);
	void AddComponent(CNSCompositeGeometry *pGeometry, int nCompNo, int nCompID);
	void setFNO(const int nFNO);
	void setDualRepresent(const int nRep);
	bool getInsertionPoint(CNSPoint3D &insertionPt);
	bool setCollinearPoints(std::vector<CNSPoint3D> &vectNode1Pts, std::vector<CNSPoint3D> &vectNode2Pts);
	//NSSTRING getName();
	std::vector<CNSComponent*> & getComponents();
public:
	~CNSComponentContainer(void);
private:
	bool getSecondaryComponents(std::map<int, int> &mapOfComponents);

private:
	std::vector<CNSComponent*> m_components;
	CNSDWGMgr* m_pDwgMgr;
	CNSDatabaseMgr* m_pDbMgr;
	CNSBlock* m_pBlk;
	CNSTElement* m_pElement;
	CNSCompositeContainer *m_pContainer;
	int m_nFNO;
	int m_nNumRep;

public:
	int m_FNOReference;
};
