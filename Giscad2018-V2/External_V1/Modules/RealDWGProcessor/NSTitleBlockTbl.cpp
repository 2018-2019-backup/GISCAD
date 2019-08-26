/*!
 @Class			CNSDimStyleTbl
 @Brief			This class validates by checking the values in the attribute of the 
				title block is not NULL
 @Author		Neilsoft Ltd
 @Date			17-08-2007
 @History 		Change history
*/
#include "StdAfx.h"
#include "NSTitleBlockTbl.h"


//constructor
CNSTitleBlock::CNSTitleBlock(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}
//destructor
CNSTitleBlock::~CNSTitleBlock(void)
{
}

void CNSTitleBlock::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSTitleBlock::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}
/*
@brief		This function checks the value of any title block attribute is not empty 
            except for attribute 'Comment', 'GIS_SUCCESS', 'CAD_SUCCESS'
*/
int CNSTitleBlock::validate(void)
{
	
	TCHAR szFormat[_MAX_PATH + 1];
	std::map<NSSTRING, NSSTRING>mapTitleAttributes;
	std::map<NSSTRING, NSSTRING> mapofPrompValues;
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportID"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectType"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectNumber"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DrawingNumber"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ProjectDescription"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportDate"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportUserName"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("DesignerCompanyName"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("Comments"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportCategories"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("ExportAreaOfInterest"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("GIS_SUCCESS"),_T("")));
	mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CAD_SUCCESS"),_T("")));
    mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("NUM"),_T("")));
    mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CAP_NO"),_T("")));
    mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("IE_NUM"),_T("")));
	m_pLocalDWGMgr->listTitleBlockHeaderAttributes(mapTitleAttributes, mapofPrompValues);


//**//SUBIR  - 10.02.2010 <<MAIL SEARCH STRING = "Invalid drawing unable to continue validation" "Extract ID">>
	NSSTRING ExportID = _T("");
	std::map<NSSTRING, NSSTRING>::iterator it;
	for(it = mapTitleAttributes.begin(); it != mapTitleAttributes.end(); it++)
	{
		NSSTRING key = (*it).first;
		NSSTRING keyVal = (*it).second;
		
        if(NSSTRCMP(key.c_str(), _T("ExportID")) == 0)
			ExportID = (*it).second;

		//Report to see the title block contents
		//WriteErrorRec(LOG_GIS,ERR_GIS_39,_T("GIS"),key.c_str());
		//WriteErrorRec(LOG_GIS,ERR_GIS_39,_T("GIS"),keyVal.c_str());
	}

	if(NSSTRCMP(ExportID.c_str(),_T(""))==0)
	{
		return NS_FAIL;
	}

	if(NSSTRCMP(ExportID.c_str(),_T("0"))==0)
	{
			WriteErrorRec(LOG_GIS,ERR_GIS_39);
	}


//**//SUBIR  - 10.02.2010 <<MAIL SEARCH STRING = "Invalid drawing unable to continue validation" "Extract ID">>

	//New request 9july2015 - check the Title block has values and log validation error
	std::map<NSSTRING, NSSTRING>::iterator vStampItr;
	vStampItr = mapTitleAttributes.find(_T("ProjectNumber"));
	NSSTRING strPN = (*vStampItr).second;
	vStampItr = mapTitleAttributes.find(_T("DrawingNumber"));
	NSSTRING strDN = (*vStampItr).second;
	vStampItr = mapTitleAttributes.find(_T("ProjectDescription"));
	NSSTRING strPD = (*vStampItr).second;
	vStampItr = mapTitleAttributes.find(_T("ExportDate"));
	NSSTRING strED = (*vStampItr).second;
	vStampItr = mapTitleAttributes.find(_T("ExportUserName"));
	NSSTRING strEUN = (*vStampItr).second;
	vStampItr = mapTitleAttributes.find(_T("DesignerCompanyName"));
	NSSTRING strDCN = (*vStampItr).second;
	bool header=NSSTRLEN(strPN.c_str()) && NSSTRLEN(strDN.c_str()) && NSSTRLEN(strED.c_str());
	if(!header)
	{
		int nErrNumber = WriteErrorRec(LOG_CAD,ERR_CAD_19);
	}

	std::map<NSSTRING, NSSTRING>::iterator mapTitleAttributesItr;
	if((int)mapofPrompValues.size() > 0)
	{
		NSSTRING strPrompt;
		for(mapTitleAttributesItr = mapofPrompValues.begin(); mapTitleAttributesItr != mapofPrompValues.end(); mapTitleAttributesItr++)
		{
			strPrompt.append((*mapTitleAttributesItr).second);
			strPrompt.append(_T(","));
		}
		strPrompt =	strPrompt.substr(0, strPrompt.find_last_of(_T(",")));
		if(strPrompt != _T(""))
		{
			//NSSPRINTF(szFormat,_T("The GIS attribute \'%s\' value for block \'%s\' is empty"), strPrompt.c_str(), _T("Title"));
			//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_13);
			
			WriteErrorRecEx(LOG_GIS,ERR_GIS_13,_T("B$"),_T("Title"),_T("A$"),strPrompt.c_str());
		}
	}
	std::map<NSSTRING, NSSTRING> mapTitleAttributesNames;
	std::map<NSSTRING, NSSTRING>::iterator mapTitleAttributesNamesItr;

	m_pLocalDWGMgr->validateTitleBlockAttributes(/*mapTitleAttributes,*/ mapTitleAttributesNames);

	for(mapTitleAttributesNamesItr = mapTitleAttributesNames.begin(); mapTitleAttributesNamesItr != mapTitleAttributesNames.end(); mapTitleAttributesNamesItr++)
	{
		NSSTRING strAttrName = (*mapTitleAttributesNamesItr).first;
		//NSSPRINTF(szFormat,_T("The values for Attribute \'%s\' in different title blocks in drawing is not same"), strAttrName.c_str());
		//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_29);

		WriteErrorRec(LOG_GIS,ERR_GIS_29,_T("A$"),strAttrName.c_str());

	}
	mapTitleAttributesNames.clear();

//	TCHAR szFormat[_MAX_PATH + 1];

    NSSTRING strDrawingNumberValue,strNUMValue, strProjNumberValue,strCAPValue, strProjType,strIENUM;

    std::map<NSSTRING, NSSTRING>::iterator TiltleblockIter;
	for(TiltleblockIter = mapTitleAttributes.begin(); TiltleblockIter != mapTitleAttributes.end(); TiltleblockIter++)
	{
		NSSTRING strAttValuesFirst = (*TiltleblockIter).first;
		NSSTRING strAttValues = (*TiltleblockIter).second;
		if(NSSTRCMP(strAttValuesFirst.c_str(), _T("Comments")) != 0 && 
			NSSTRCMP(strAttValuesFirst.c_str(), _T("GIS_SUCCESS")) != 0 &&
			NSSTRCMP(strAttValuesFirst.c_str(), _T("CAD_SUCCESS")) != 0)
		{
			//19/01/2008 New request by client. Check if DrawingNumber and NUM are same and check if projectnumber
			// is same as CAP_NO or IE_NUM (depending on projectType)

			if(NSSTRCMP(strAttValuesFirst.c_str(), _T("DrawingNumber")) == 0)
				strDrawingNumberValue.append(strAttValues.c_str());
			else if(NSSTRCMP(strAttValuesFirst.c_str(), _T("NUM")) == 0)
				strNUMValue.append(strAttValues.c_str());
			else if(NSSTRCMP(strAttValuesFirst.c_str(), _T("ProjectNumber")) == 0)
				strProjNumberValue.append(strAttValues.c_str());
			else if(NSSTRCMP(strAttValuesFirst.c_str(), _T("CAP_NO")) == 0)
				strCAPValue.append(strAttValues.c_str());
			else if(NSSTRCMP(strAttValuesFirst.c_str(), _T("ProjectType")) == 0)
				strProjType.append(strAttValues.c_str());
			else if(NSSTRCMP(strAttValuesFirst.c_str(), _T("IE_NUM")) == 0)
				strIENUM.append(strAttValues.c_str());

			if(NSSTRCMP(strAttValues.c_str(), _T("")) == 0)
			{
				if(NSSTRCMP(strAttValuesFirst.c_str(), _T("CAP_NO")) != 0 &&
					NSSTRCMP(strAttValuesFirst.c_str(), _T("IE_NUM")) != 0)
				{
					//NSSPRINTF(szFormat,_T("The value of Attribute \'%s\' in Title Block is empty"), strAttValuesFirst.c_str());
					//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_20);

		            WriteErrorRec(LOG_GIS,ERR_GIS_20,_T("A$"),strAttValuesFirst.c_str());
				}
			}
		} 
	}

	if((strCAPValue.length()==0) && (NSSTRCMP(strProjType.c_str(),_T("external"))==0))
	{
		//NSSPRINTF(szFormat,_T("The value of Attribute \'CAP_NO\' in Title Block is empty"));
		//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_21);	

		WriteErrorRec(LOG_GIS,ERR_GIS_21,_T("The value of Attribute \'CAP_NO\' in Title Block is empty"));

	}
	if((strIENUM.length()==0) && (NSSTRCMP(strProjType.c_str(),_T("internal"))==0))
	{
		//NSSPRINTF(szFormat,_T("The value of Attribute \'IE_NUM\' in Title Block is empty"));
		//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_22);	

		WriteErrorRec(LOG_GIS, ERR_GIS_22,_T("The value of Attribute \'IE_NUM\' in Title Block is empty"));
	}
    if(NSSTRCMP(strDrawingNumberValue.c_str(),strNUMValue.c_str())!=0)
    {
        //NSSPRINTF(szFormat,_T("Values of Attribute DrawingNumber and NUM in Title Block are mismatched"));
		//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_23);	

		WriteErrorRec(LOG_GIS,ERR_GIS_23,_T("Values of Attribute DrawingNumber and NUM in Title Block are mismatched"));

    }

    if(NSSTRCMP(strProjType.c_str(),_T("external"))==0)
    {
   //     if(NSSTRCMP(strProjNumberValue.c_str(),strCAPValue.c_str())!=0)
   //     {
   //         //NSSPRINTF(szFormat,_T("Values of Attribute ProjectNumber and CAP_NO in Title Block are mismatched"));
			////WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_24);

			//WriteErrorRec(LOG_GIS,ERR_GIS_24,_T("Values of Attribute ProjectNumber and CAP_NO in Title Block are mismatched"));
   //     }
    }
    else if(NSSTRCMP(strProjType.c_str(),_T("internal"))==0)
    {
        if(NSSTRCMP(strProjNumberValue.c_str(),strIENUM.c_str())!=0)
        {
            //NSSPRINTF(szFormat,_T("Values of Attribute ProjectNumber and IE_NUM in Title Block are mismatched"));
			//WriteIDToLog(LOG_GIS, szFormat,ERR_GIS_25);

			WriteErrorRec(LOG_GIS,ERR_GIS_25,_T("Values of Attribute ProjectNumber and IE_NUM in Title Block are mismatched"));
        }
    }

	//UserName is needed to show it in report hence stored during validation;
	m_strUserName = getValue(_T("ExportUserName"), mapTitleAttributes);
	m_strReportName = _T("A") + getValue(_T("DrawingNumber"), mapTitleAttributes) + _T("_") + getValue(_T("ProjectNumber"), mapTitleAttributes) 
		+ _T("_") + getValue(_T("ExportID"), mapTitleAttributes)+ _T("_Validation") + _T(".html");

	return NS_SUCCESS;
}

NSSTRING CNSTitleBlock::getValue(NSSTRING strKey, std::map<NSSTRING, NSSTRING> &mapTitleAttributes)
{
	std::map<NSSTRING, NSSTRING>::iterator TiltleblockIter;
	for(TiltleblockIter = mapTitleAttributes.begin(); TiltleblockIter != mapTitleAttributes.end(); TiltleblockIter++)
	{
		if(NSSTRCMP(strKey.c_str(), ((*TiltleblockIter).first).c_str()) == 0)
		{
			return (*TiltleblockIter).second;
		}
	}
	

	//TiltleblockIter = mapTitleAttributes.find(strKey);
	//if(TiltleblockIter != mapTitleAttributes.end())
	//{
	//	return (*TiltleblockIter).second;
	//}
	return _T("");
}


NSSTRING CNSTitleBlock::getUserName()const
{
	return m_strUserName;
}

NSSTRING CNSTitleBlock::getReportName()const
{
	return m_strReportName;
}
