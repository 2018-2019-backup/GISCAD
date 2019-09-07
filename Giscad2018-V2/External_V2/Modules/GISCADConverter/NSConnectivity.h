/*!
 @Class			CNSConnectivity
 @Brief			This class provides the functionality to read connectivity information from
				XML or write connectivity information into DWG.
 @Author		Neilsoft Ltd
 @Date			12-07-2007
 @History 		Change history
 */

#pragma once
#include "NSPair.h"

typedef std::vector<CNSPair*> PAIR_VECT;

class CNSConnectivity
{
public:
	CNSConnectivity(void);
public:
	~CNSConnectivity(void);
	int readXML();
	int readDWG();
	int writeXML();
	int writeDWG();
	int getXDataString(PAIR_VECT &vectNodes, NSSTRING &strXData);
	void setElement(CNSTElement &element);
	void getXData(STRING_MAP& mapOfRelationships);
	void setXData(STRING_MAP& mapOfRelationships);
	bool getConnectivityData(std::vector<int> &vectNode1, std::vector<int> &vectNode2);

private:
	CNSTElement* m_pElement;
	PAIR_VECT m_vectNode1;
	PAIR_VECT m_vectNode2;
};
