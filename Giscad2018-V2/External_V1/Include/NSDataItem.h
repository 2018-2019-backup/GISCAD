#pragma once
#include "map"
#include "tchar.h"
class __declspec(dllexport) CNSDataItem
{
public:
	CNSDataItem(void);
	~CNSDataItem(void);
public:
	void insertIntoMap(TCHAR* szKey, TCHAR* szValue);
public:
	TCHAR* getValueFromMap(TCHAR* szKey);
	int getSize();
private:
	std::map<TCHAR*, TCHAR*> m_mapOfParameters;
};
