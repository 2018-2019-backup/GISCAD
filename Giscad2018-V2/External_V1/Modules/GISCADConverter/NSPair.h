/*!
 @Class		:	CNSPair
 @Brief		:	This class holds the pair information of FID and FNO
 @Author	:	Neilsoft Ltd
 @Date		:	16-07-2007
 @History 	:	Change history
 */
#pragma once
#include <vector>
#include <map>
#include "string"

#include "..\..\Include\NSDirectives.h"

#include "NSTElement.h"

typedef std::map<NSSTRING, NSSTRING> STRING_MAP;

class CNSPair
{
public:
	CNSPair(void);
	~CNSPair(void);
	void setElement(CNSTElement &element);
	int getFID()const;
	int getFNO()const;
	void setFID(const int nFID);
	void setFNO(const int nFNO);

	int readXML();
	int readDWG();
	int writeXML();
	int writeDWG();	

private:
	int  m_nFID; 
    int  m_nFNO;
	CNSTElement *m_pElement;
};
