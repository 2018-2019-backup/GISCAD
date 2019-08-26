#include "StdAfx.h"
#include "NSTextStyleTbl.h"

CNSTextStyleTbl::CNSTextStyleTbl(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

CNSTextStyleTbl::~CNSTextStyleTbl(void)
{
}
void CNSTextStyleTbl::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}
void CNSTextStyleTbl::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}
int CNSTextStyleTbl::validate(void)
{
	//getting all the TextStyle properties of local dwg files 
	std::map<NSSTRING, CNSTextStyle*> mapOfStdTextStyle;
	m_pStdDWGMgr->getTextStyleProperties(mapOfStdTextStyle);

	//getting all the TextStyle properties of local dwg files 
	std::map<NSSTRING,CNSTextStyle*> mapOfLocalTextStyle;
	m_pLocalDWGMgr->getTextStyleProperties(mapOfLocalTextStyle);
	CNSTextStyle *pLocalTextStyle, *pStdTextStyle ;
	std::map<NSSTRING,CNSTextStyle*>::iterator TextStyleIter, TextStyleStdIter;
	NSSTRING strTextStyleName;
	TCHAR szFormat[_MAX_PATH + 1];
	for(TextStyleIter = mapOfLocalTextStyle.begin(); TextStyleIter != mapOfLocalTextStyle.end(); TextStyleIter++)
	{
		//Get the TextStyle name and check it in the std dwg file.
		strTextStyleName = (*TextStyleIter).first;
		TextStyleStdIter = mapOfStdTextStyle.find(strTextStyleName);
		if(TextStyleStdIter == mapOfStdTextStyle.end())
		{
			WriteErrorRec(LOG_CAD,ERR_CAD_14,_T("TS$"),strTextStyleName.c_str());
		}
		else
		{
			CheckFontName(TextStyleStdIter,TextStyleIter);			
		}
	}
	//Delete the objects of the class TextStyle from the map of both std and local.
	for(TextStyleIter = mapOfLocalTextStyle.begin(); TextStyleIter != mapOfLocalTextStyle.end(); TextStyleIter++)
	{
		delete (*TextStyleIter).second;
	}
    for(TextStyleIter = mapOfStdTextStyle.begin(); TextStyleIter != mapOfStdTextStyle.end(); TextStyleIter++)
	{
		delete (*TextStyleIter).second;
	}
	//Clear the maps
	mapOfLocalTextStyle.clear();
	mapOfStdTextStyle.clear();
	return NS_SUCCESS;
}

int CNSTextStyleTbl::CheckFontName(std::map<NSSTRING,CNSTextStyle*>::iterator TextStyleStdIter,std::map<NSSTRING,CNSTextStyle*>::iterator TextStyleIter )
{
			CNSTextStyle *pLocalTextStyle, *pStdTextStyle ;

			pStdTextStyle = (*TextStyleStdIter).second;
			pLocalTextStyle = (*TextStyleIter).second;

			NSSTRING strStdFontName = pStdTextStyle->getFontName();
			NSSTRING strLocalFontName =  pLocalTextStyle->getFontName();

			if(strStdFontName != strLocalFontName)
			{
				NSSTRING  strTextStyleName = (*TextStyleIter).first;
				WriteErrorRec(LOG_CAD,ERR_CAD_15,_T("Font Name"), strTextStyleName.c_str(), strLocalFontName.c_str(), strStdFontName.c_str());
				return NS_FAIL;
			}

			return NS_SUCCESS;
}
