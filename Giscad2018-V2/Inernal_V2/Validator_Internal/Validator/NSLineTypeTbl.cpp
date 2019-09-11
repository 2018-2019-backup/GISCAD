#include "StdAfx.h"
#include "NSLineTypeTbl.h"

CNSLineTypeTbl::CNSLineTypeTbl(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

CNSLineTypeTbl::~CNSLineTypeTbl(void)
{
}

void CNSLineTypeTbl::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSLineTypeTbl::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}

int CNSLineTypeTbl::validate()
{
	//getting all the linetype properties of std dwg files
	std::map<NSSTRING,CNSLineType*> mapOfStdLineType;
	m_pStdDWGMgr->getLineTypeList(mapOfStdLineType);

	//getting all the linetype properties of local dwg files
	std::map<NSSTRING,CNSLineType*> mapOfLocalLineType;
	m_pLocalDWGMgr->getLineTypeList(mapOfLocalLineType);

	std::map<NSSTRING,CNSLineType*>::iterator LineTypeIter, LineTypeStdIter;
	NSSTRING strLineTypeName;
	TCHAR szFormat[_MAX_PATH + 1];
	for(LineTypeIter = mapOfLocalLineType.begin(); LineTypeIter != mapOfLocalLineType.end(); LineTypeIter++)
	{
		//Get the Linetype name and check it in the std dwg file.
		strLineTypeName = (*LineTypeIter).first;
		LineTypeStdIter = mapOfStdLineType.find(strLineTypeName);
		if(LineTypeStdIter == mapOfStdLineType.end())
		{
			//NSSPRINTF(szFormat,_T("Linetype \'%s\', Name is non-standard"), strLineTypeName.c_str());
			//WriteIDToLog(LOG_CAD, szFormat, ERR_CAD_13);

			WriteErrorRec(LOG_CAD,ERR_CAD_13,_T("LT$"),strLineTypeName.c_str());
		}
	}
	//Delete the objects of the class LineType from the map of both std and local.
	for(LineTypeIter = mapOfLocalLineType.begin(); LineTypeIter != mapOfLocalLineType.end(); LineTypeIter++)
	{
		delete (*LineTypeIter).second;
	}	
	for(LineTypeIter = mapOfStdLineType.begin(); LineTypeIter != mapOfStdLineType.end(); LineTypeIter++)
	{
		delete (*LineTypeIter).second;
	}	
	//Clear the maps
	mapOfLocalLineType.clear();
	mapOfStdLineType.clear();
	return NS_SUCCESS;
}