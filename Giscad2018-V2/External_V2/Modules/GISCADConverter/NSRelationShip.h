/*!
 @Class		:	CNSRelationShip
 @Brief		:	This class is responsible for reading relationship information
				from XML and writing the same information into DWG.
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include "NSTElement.h"
#include "NSConnectivity.h"
#include "NSOwnership.h"
#include "NSManyOwnership.h"
typedef std::map<NSSTRING, NSSTRING> STRING_MAP;
class CNSRelationShip
{
public:
	CNSRelationShip(void);
public:
	~CNSRelationShip(void);
	int readXML();
	int readDWG();
	int writeXML();
	int writeDWG();
	void setElement(CNSTElement &element);
	void getXData(STRING_MAP& mapOfRelationships);
	void setXData(STRING_MAP& mapOfRelationships);
	bool getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2);

private:
	CNSTElement *m_pElement;
	CNSConnectivity m_connectivity;
	CNSOwnership m_ownerShip;
	CNSManyOwnership m_manyOwnership;
};
