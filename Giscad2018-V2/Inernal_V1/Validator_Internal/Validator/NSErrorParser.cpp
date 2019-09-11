#include "StdAfx.h"
#include "NSErrorParser.h"
#include "..\..\Include\NSDatabaseMgr.h"
#include "..\..\Include\NSRelationLinker.h"




CNSErrorParser* CNSErrorParser::pErrorParser = NULL;


CNSErrorParser::CNSErrorParser(void)
{
	
}

CNSErrorParser::~CNSErrorParser(void)
{
}


/*!
 @Brief			Parse the $ tags
 @Various overloads to tackle various vatiations
 @Param [in]	ErrRecord	- unparsed error records
 @Return		ErrRecord   - parsed error records
 @Date			27 Jan 2009
*/
ErrRecord CNSErrorParser::ParseDesc(ErrRecord er)
{	
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	TCHAR szFormat[_MAX_PATH + 1];

	er.strErrorDescription = strTemp; 

return er;

}


ErrRecord CNSErrorParser::ParseDesc(ErrRecord er , CNSValidationDataCollector *pTempFeature, CNSProposedRelations *pTempPropRel)
{	
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,_T("B$"),pTempFeature->getBlockName().c_str());
	strTemp = ReplaceTag(strTemp,_T("L$"),pTempFeature->getLayerName().c_str());

	//if ( pTempPropRel != NULL ){strTemp = ReplaceTag(strTemp,_T("F$"),pTempPropRel->getFID().c_str());}
	TCHAR szFormat[_MAX_PATH + 1];
	int nFID = pTempFeature->getFID();
	if(nFID!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFID);
	strTemp = ReplaceTag(strTemp,_T("F$"),szFormat);
	}

	strTemp = ReplaceTag(strTemp,_T("P$"),pTempFeature->getInsertionPointString().c_str());

	strTemp = ReplaceTag(strTemp,_T("NOTE$"),pTempFeature->getNotesAppended(_T("")));

	int nFNO = pTempFeature->getFNO();
	if(nFNO!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFNO);
	strTemp = ReplaceTag(strTemp,_T("FNO$"),szFormat);
	}


	er.strErrorDescription = strTemp; 

return er;


}

ErrRecord CNSErrorParser::ParseDesc(ErrRecord er,CNSAsset *pAsset, NSSTRING strMsg )
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,_T("B$"),pAsset->getBlockName().c_str());

	strTemp = ReplaceTag(strTemp,_T("L$"),pAsset->getLayerName().c_str());
    strTemp = ReplaceTag(strTemp,_T("AN$"),pAsset->getAssetNumber().c_str());
	strTemp = ReplaceTag(strTemp,_T("FT$"),strMsg); 

	
	TCHAR szFormat[_MAX_PATH + 1];
	int nFNO = pAsset->getFNO();
	if(nFNO!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFNO);
	strTemp = ReplaceTag(strTemp,_T("FNO$"),szFormat);
	}
	

	CNSPoint3D pt; 
	CNSDWGMgr tempMgr;	tempMgr.GetInsertionPoint(pAsset->getObjectID(), pt);
	NSSTRING strInsPt = _T("");	
	NSSPRINTF(szFormat, _T("X: \'%lf\' Y: \'%lf\'"), pt.m_dXX, pt.m_dYY);
	strInsPt.append(szFormat);
	strTemp = ReplaceTag(strTemp,_T("P$"),strInsPt);

	//strTemp = ReplaceTag(strTemp,_T("F$"),pAsset->getFID().c_str());
	//strTemp = ReplaceTag(strTemp,_T("P$"),pAsset->getInsertionPointString().c_str());

	er.strErrorDescription = strTemp; 

    return er;
}

ErrRecord CNSErrorParser::ParseDesc(ErrRecord er,NSSTRING pszTag, NSSTRING strData )
{

	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,pszTag,strData);


	er.strErrorDescription = strTemp; 
    return er;
}

ErrRecord CNSErrorParser::ParseDesc(ErrRecord er,NSSTRING strTag,NSSTRING locLay, NSSTRING stdLay )
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,_T("LT$"),strTag);

    strTemp = ReplaceTag(strTemp,_T("SL$"),locLay);
	strTemp = ReplaceTag(strTemp,_T("L$"),stdLay); 


	er.strErrorDescription = strTemp; 

    return er;

}

ErrRecord CNSErrorParser::ParseDesc(ErrRecord er,NSSTRING PropName, NSSTRING Stylename, NSSTRING CurrVal, NSSTRING StdVal )
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,_T("PROP$"),PropName);

	strTemp = ReplaceTag(strTemp,_T("DS$"),Stylename);
	strTemp = ReplaceTag(strTemp,_T("TS$"),Stylename);
	strTemp = ReplaceTag(strTemp,_T("L$"),Stylename);
	strTemp = ReplaceTag(strTemp,_T("LT$"),Stylename);

    strTemp = ReplaceTag(strTemp,_T("CV$"),CurrVal);
	strTemp = ReplaceTag(strTemp,_T("SV$"),StdVal); 


	er.strErrorDescription = strTemp; 

    return er;
}

ErrRecord CNSErrorParser::ParseDescEx(ErrRecord er,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2 )
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,Tag1,Data1);
	strTemp = ReplaceTag(strTemp,Tag2,Data2);




	er.strErrorDescription = strTemp; 

    return er;
}

ErrRecord  CNSErrorParser::ParseDescEx(ErrRecord er,CNSValidationDataCollector *pTempFeature,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2  , NSSTRING Tag3, NSSTRING Data3 )
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,Tag1,Data1);
	strTemp = ReplaceTag(strTemp,Tag2,Data2);
	strTemp = ReplaceTag(strTemp,Tag3,Data3);


	strTemp = ReplaceTag(strTemp,_T("B$"),pTempFeature->getBlockName().c_str());
	strTemp = ReplaceTag(strTemp,_T("L$"),pTempFeature->getLayerName().c_str());

	strTemp = ReplaceTag(strTemp,_T("NOTE$"),pTempFeature->getNotesAppended(_T("")));

	strTemp = ReplaceTag(strTemp,_T("P$"),pTempFeature->getInsertionPointString().c_str());


	TCHAR szFormat[_MAX_PATH + 1];
	int nFNO = pTempFeature->getFNO();
	if(nFNO!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFNO);
	strTemp = ReplaceTag(strTemp,_T("FNO$"),szFormat);
	}

	int nFID = pTempFeature->getFID();
	if(nFID!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFID);
	strTemp = ReplaceTag(strTemp,_T("F$"),szFormat);
	}


	er.strErrorDescription = strTemp; 

    return er;

}

ErrRecord CNSErrorParser::ParseDescEx(ErrRecord er,CNSValidationDataCollector *pTempFeature,CNSProposedRelations *pTempPropRel , NSSTRING Tag1, NSSTRING Data1)
{
	NSSTRING strTemp = er.strErrorDescription;

	strTemp = ReplaceTag(strTemp,_T("DWG$"),strDWG$);
	strTemp = ReplaceTag(strTemp,_T("dwg$"),strdwg$);

	strTemp = ReplaceTag(strTemp,Tag1,Data1);

	strTemp = ReplaceTag(strTemp,_T("B$"),pTempFeature->getBlockName().c_str());
	strTemp = ReplaceTag(strTemp,_T("L$"),pTempFeature->getLayerName().c_str());

	strTemp = ReplaceTag(strTemp,_T("NOTE$"),pTempFeature->getNotesAppended(_T("")));

	strTemp = ReplaceTag(strTemp,_T("P$"),pTempFeature->getInsertionPointString().c_str());


	TCHAR szFormat[_MAX_PATH + 1];
	int nFNO = pTempFeature->getFNO();
	if(nFNO!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFNO);
	strTemp = ReplaceTag(strTemp,_T("FNO$"),szFormat);
	}

	int nFID = pTempFeature->getFID();
	if(nFID!=0)
	{
	NSSPRINTF(szFormat, _T("%i"), nFID);
	strTemp = ReplaceTag(strTemp,_T("F$"),szFormat);
	}


	er.strErrorDescription = strTemp; 

    return er;


}

/*!
 @Brief			Replace the $ tags with specified strings
 @Param [in]	strDescFromDB			- Description string from Database 
 @Param [in]	strSmartTag				- predefined $ tag say L$ for layername
 @Param [in]	strTagReplacementData	- actual string to be placed in parsed string say "MYLAYER_NEW"
 @Date			27 Jan 2009
*/
NSSTRING CNSErrorParser::ReplaceTag(NSSTRING strDescFromDB,NSSTRING strSmartTag,NSSTRING strTagReplacementData)
{

int nLenTag = 0;
int nLenTagData = 0;
int nPos = -1;

std::vector<int> nPositions;

nLenTag = strDescFromDB.length() ;
nLenTagData = strTagReplacementData.length() ;

while(nPos < nLenTag)
{
	nPos = strDescFromDB.find(strSmartTag,nPos+1);
	if(nPos == -1)
		break;
	else
	{
		//nPositions.push_back(nPos);
	   if(nLenTagData >= 0)
		 strDescFromDB.replace(nPos, strSmartTag.length(), strTagReplacementData);
	   //else
	   //{
		  // NSSTRING strTagStart =  _T("<span style=\"background-color: #FF0000\">");
		  // NSSTRING strTagEnd =  _T("</span>");

		  // NSSTRING strTagString = strTagStart + strSmartTag + strTagEnd ;

		  // strDescFromDB.replace(nPos, strSmartTag.length(), strTagString);
	   //}
	}
}

if(nPos != -1 && nLenTag > nPos )
   if(nLenTagData != 0)
	 strDescFromDB.replace(nPos, strSmartTag.length(), strTagReplacementData);
   else
   {
	   NSSTRING strTagStart =  _T("<span style=\"background-color: #FF0000\">");
	   NSSTRING strTagEnd =  _T("</span>");

	   NSSTRING strTagString = strTagStart + strSmartTag + strTagEnd ;

	   strDescFromDB.replace(nPos, strSmartTag.length(), strTagString);
   }
     
	
	return strDescFromDB;
}



//void CNSErrorParser::SetTag(NSSTRING strSmartTag,NSSTRING strTagReplacementData)
//{
//
//	TagMap.insert( std::map<NSSTRING, NSSTRING>::value_type(strSmartTag,strTagReplacementData));
//	
//
//	return ;
//}






CNSErrorParser* CNSErrorParser::getInstance()
{
    if(pErrorParser == NULL)
    {
        pErrorParser = new CNSErrorParser();
    }
    return pErrorParser;
}