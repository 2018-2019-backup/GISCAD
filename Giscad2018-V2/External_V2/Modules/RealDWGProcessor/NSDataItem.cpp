#include "StdAfx.h"
#include "../include/nsdataitem.h"
#include "../include/nsdirectives.h"

CNSDataItem::CNSDataItem(void)
{	
}

CNSDataItem::~CNSDataItem(void)
{
    std::map<TCHAR*, TCHAR*>::iterator it;
    for(it = m_mapOfParameters.begin(); it != m_mapOfParameters.end(); it++)
    {
        TCHAR* szKey = (*it).first;
        delete[] szKey;
        TCHAR* szValue = (*it).second;
        delete[] szValue;
    }
    m_mapOfParameters.clear();
}

void CNSDataItem::insertIntoMap(TCHAR* szKey, TCHAR* szValue)
{
	// Instead of using _MAX_PATH use the length og key and value.
	// Changed this code to fix External validation problem which was working in
	// case of internal validation.
    //TCHAR* szTempKey = new TCHAR[_MAX_PATH + 1];
    //TCHAR* szTempValue = new TCHAR[_MAX_PATH + 1];
	TCHAR* szTempKey = new TCHAR[NSSTRLEN(szKey) + 1];
	TCHAR* szTempValue = new TCHAR[NSSTRLEN(szValue) + 1];
#ifdef _INTERNAL_
	/*NSSTRCPY(szTempKey, szKey);
    NSSTRCPY(szTempValue, szValue);*/
	NSSTRCPY(szTempKey, NSSTRLEN(szKey) + 1, szKey);
   	NSSTRCPY(szTempValue, NSSTRLEN(szValue) + 1, szValue);
#else
    //NSSTRCPY(szTempKey, _MAX_PATH + 1, szKey);
	NSSTRCPY(szTempKey, NSSTRLEN(szKey) + 1, szKey);
    //NSSTRCPY(szTempValue, _MAX_PATH + 1, szValue);
	NSSTRCPY(szTempValue, NSSTRLEN(szValue) + 1, szValue);
#endif
	m_mapOfParameters.insert(std::map<TCHAR*, TCHAR*>::value_type(szTempKey, szTempValue));
}

TCHAR* CNSDataItem::getValueFromMap(TCHAR* szKey)
{
	std::map<TCHAR*, TCHAR*>::iterator it;
	for(it = m_mapOfParameters.begin(); it != m_mapOfParameters.end(); it++)
	{
		TCHAR* key = (*it).first;
		//if(stricmp(key, szKey) == 0)
        if(NSSTRCMP(key, szKey) == 0)
			return (*it).second;
	}
	/*it = m_mapOfParameters.find(szKey);
	if(it != m_mapOfParameters.end())
	{
		return (*it).second;
	}*/
	return _T("");

}

int CNSDataItem::getSize()
{
	int size = m_mapOfParameters.size();
	return size;
}
