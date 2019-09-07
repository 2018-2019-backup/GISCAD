/*!
 @Class		:	CNSOwnership
 @Brief		:	This class is responsible for reading Ownership information from
				XML and writing into DWG as a xdata.
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include "NSPair.h"
typedef std::vector<CNSPair*> PAIR_VECT;
class CNSOwnership
{
public:
	CNSOwnership(void);
public:
	~CNSOwnership(void);
	int readXML();
	int readDWG();
	int writeXML();
	int writeDWG();
	void setElement(CNSTElement &element);
	int getXDataString(PAIR_VECT &vectNodes, NSSTRING &strXData);
	void getXData(STRING_MAP& mapOfRelationships);
	void setXData(STRING_MAP& mapOfRelationships);
private:
	CNSTElement *m_pElement;
	PAIR_VECT m_vectParent;
	PAIR_VECT m_vectChild;
};