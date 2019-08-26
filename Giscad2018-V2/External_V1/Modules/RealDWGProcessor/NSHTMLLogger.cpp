/*!
 @Class		CNSHTMLLogger
 @Brief		Writes HTML report
 @Author	Neilsoft Ltd
 @Date		12 June 2007
 @History 	
 */

#include "StdAfx.h"
#include "tchar.h"
#include "nshtmllogger.h"
#include "..\Include\NSDirectives.h"
#include "..\Include\NSReportWriter.h"
#include "NSDatabaseMgr.h"
#include "map"

#include <iostream>
#include <algorithm>
#include <vector>

/*!
 @Brief			Custom function for multifield sorting of STL vector of Error Record Structure 1st by severity then by  catagory
 @Param [in]	Pointer of Error Record Structure
 @Param [in]	Pointer of Error Record Structure
 @Return		bool
 @Date			29 Jan 2009
*/
//bool MultiColumnSort (ErrRecord *i,ErrRecord *j)
//{
//	if(i->strErrorSeverity < j->strErrorSeverity)
//	{
//		  return true;
//	}
//  	else 
//	{
//		  if(i->strErrorSeverity > j->strErrorSeverity)
//		  {
//			  return false;
//		  }
//		  else 
//		  {
//			  if(i->strErrorCategory < j->strErrorCategory)
//			  {
//				  return true;
//			  }
//			  else
//			  {
//				  return false;
//			  }
//		  }
//	}
//}

/*!
 @Brief			Custom function for multifield sorting of STL vector of Error Record Structure - 1st by catagory then by severity
 @Param [in]	Pointer of Error Record Structure
 @Param [in]	Pointer of Error Record Structure
 @Return		bool
 @Date			17 Feb 2009
*/
bool MultiColumnSort (ErrRecord *i,ErrRecord *j)
{
	if(i->strErrorCategory < j->strErrorCategory)
	{
		  return true;
	}
  	else 
	{
		  if(i->strErrorCategory > j->strErrorCategory)
		  {
			  return false;
		  }
		  else 
		  {
			  if(i->strErrorSeverity < j->strErrorSeverity)
			  {
				  return true;
			  }
			  else
			  {
				  //---Sort By Error Id -----------------------
				  if(i->strErrorSeverity > j->strErrorSeverity)
				  {
				    return false;
				  }
				  else
				  {
					  if(i->nErrorID < j->nErrorID)
					 {
						return true;
					 }
					 else
					 {
						return false;
					 }
				  }
				  //--------------------------------------
			  }
		  }
	}
}

/*!
 @Brief			Default Constructor
 @Date			12 June 2007
*/
CNSHTMLLogger::CNSHTMLLogger(void)
{
}

/*!
 @Brief			Default Destructor
 @Date			12 June 2007
*/
CNSHTMLLogger::~CNSHTMLLogger(void)
{
}

/*!
 @Brief			Retrives the values from LOGTYPE_MAP and writes them to an HTML file.
 @Param [in]	LOGTYPE_MAP - Map of Error Logs
 @Param [in]	TCHAR*		- path and file name of the report
 @Return		None
 @Date			12 June 2007
*/
bool CNSHTMLLogger::writeReport(LOGTYPE_MAP& LogTypeMap, TCHAR* szReportPathName)
{
    bool bRetVal = true;
    bool bIndex = false;
    FILE* pFile = NULL;
    int nIndexCntr = 0;
    int nSize = 0;

	#ifdef _INTERNAL_
		//pFile = NSFOPEN(szReportPathName, _T("w"));
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#else
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#endif
    if(pFile != NULL)
    {
        NSFPRINTF(pFile,addReportTitle().c_str()); 	

        std::map<NSSTRING,NSSTRING> headerData;
        
        CNSLogCollection *pCollection = NULL;
        LOGTYPE_MAP::iterator LogIter;
        LogIter = LogTypeMap.find(LOG_HEADER);
        if(LogIter != LogTypeMap.end())
        {
            pCollection = (*LogIter).second;
        }

		NSFPRINTF(pFile,addReportHeader(pCollection).c_str()); 		

        NSFPRINTF(pFile,addEmptyLine().c_str());
        NSFPRINTF(pFile,addEmptyLine().c_str()); 

		
		LogIter = LogTypeMap.find(LOG_OTHER);
        if(LogIter != LogTypeMap.end())
        {
            pCollection = (*LogIter).second;
	      //  NSFPRINTF(pFile,AddCADErrorData(pCollection).c_str());
			NSFPRINTF(pFile,addErrorData(pCollection, LOG_OTHER).c_str());
	        NSFPRINTF(pFile,addEmptyLine().c_str()); 
        }

        LogIter = LogTypeMap.find(LOG_CAD);
        if(LogIter != LogTypeMap.end())
        {
            pCollection = (*LogIter).second;
	      //  NSFPRINTF(pFile,AddCADErrorData(pCollection).c_str());
			NSFPRINTF(pFile,addErrorData(pCollection, LOG_CAD).c_str());
	        NSFPRINTF(pFile,addEmptyLine().c_str()); 
        }

        LogIter = LogTypeMap.find(LOG_GIS);
        if(LogIter != LogTypeMap.end())
        {
            pCollection = (*LogIter).second;
	        NSFPRINTF(pFile,addErrorData(pCollection, LOG_GIS).c_str());
	        NSFPRINTF(pFile,addEmptyLine().c_str()); 
        }

        NSFPRINTF(pFile,endHTMLReport().c_str());

        fclose(pFile);
        return TRUE;

	    if(fclose(pFile) != 0)
		    bRetVal = false;
    }
    return bRetVal;
}


/*!
 @Brief			Retrives the values from m_vectLogErrRecs and writes them to an HTML file.
 @Param [in]	m_vectLogErrRecs - Vector of Error ( CAD & GIS ) Logs
 @Param [in]	m_vectInfoRecs	 - Vector of Header & Other Logs
 @Param [in]	szReportPathName - path and file name of the report
 @Return		None
 @Date			29 Jan 2009
*/
bool CNSHTMLLogger::writeReport(std::vector<ErrRecord*> &m_vectLogErrRecs, std::vector<ErrRecord*> &m_vectInfoRecs, TCHAR* szReportPathName,  TCHAR* szCSSPathName)
{
	bool bRetVal = true;
    bool bIndex = false;
    FILE* pFile = NULL;
    int nIndexCntr = 0;
    int nSize = 0;

	#ifdef _INTERNAL_
		//pFile = NSFOPEN(szReportPathName, _T("w"));
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#else
		NSFOPEN(&pFile, szReportPathName, _T("w"));
	#endif
    if(pFile != NULL)
    {
		NSFPRINTF(pFile,addReportTitle(szCSSPathName).c_str()); 	


		NSFPRINTF(pFile,addReportHeaderEx().c_str()); 	

		NSFPRINTF(pFile,addInfoData(m_vectInfoRecs).c_str());

        NSFPRINTF(pFile,addEmptyLine().c_str());
        NSFPRINTF(pFile,addEmptyLine().c_str()); 
		
		NSFPRINTF(pFile,addOtherInfoData(m_vectInfoRecs).c_str());

		NSFPRINTF(pFile,addEmptyLine().c_str());
        NSFPRINTF(pFile,addEmptyLine().c_str()); 

		NSFPRINTF(pFile,addErrorData(m_vectLogErrRecs,LOG_CAD).c_str());

		NSFPRINTF(pFile,addEmptyLine().c_str());
        NSFPRINTF(pFile,addEmptyLine().c_str()); 

		NSFPRINTF(pFile,addErrorData(m_vectLogErrRecs,LOG_GIS).c_str());

        NSFPRINTF(pFile,endHTMLReport().c_str());

        fclose(pFile);
        return TRUE;

	    if(fclose(pFile) != 0)
		    bRetVal = false;
    }
}
/*!
 @Brief			add empty line in HTML file.
 @Return		empty line
 @Date			23 Aug 2007
*/
NSSTRING CNSHTMLLogger::addEmptyLine()
{
    NSSTRING szEmpty(_T(""));
    szEmpty.append(addTable(FALSE));
		szEmpty.append(addRowInTable());
			szEmpty.append(addColumnInRow(15));
			szEmpty.append(endColumnInRow());
		szEmpty.append(endRowInTable());
    szEmpty.append(endTable());
    return szEmpty;
}
/*!
 @Brief			Add "Validation Report" html title
 @Param [in]	none
 @Return		report title string
 @Date			23 Aug 2007
*/
NSSTRING CNSHTMLLogger::addReportTitle(TCHAR* szCSSPathName )
{
    NSSTRING szTitle(_T(""));
    m_nTabCount = 0;
	CNSHTMLLogger::addTabs(szTitle,m_nTabCount);	szTitle.append(_T("<HTML>"));							CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<HEAD>"));							CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<TITLE>Validation Report</TITLE>"));	CNSHTMLLogger::addEnters(szTitle,1);
	
	szTitle.append(_T("<link rel=\"stylesheet\" href=\""));
	szTitle.append(szCSSPathName);
	szTitle.append(_T(" type=\"text/css\">"));
	  
	CNSHTMLLogger::addEnters(szTitle,1);

	CNSHTMLLogger::addTabs(szTitle,--m_nTabCount);	szTitle.append(_T("</HEAD>"));							CNSHTMLLogger::addEnters(szTitle,2);

    return szTitle;
}

/*!
 @Brief			Add report header html title
 @Param [in]	none
 @Return		report header string
 @Date			23 Aug 2007
*/
NSSTRING CNSHTMLLogger::addReportHeader(CNSLogCollection *pCollection )
{
    NSSTRING szTitle(_T(""));

	CNSHTMLLogger::addTabs(szTitle,m_nTabCount);	szTitle.append(_T("<BODY bgcolor=\"ffffff\">"));				CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<P align=center>"));							CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<FONT face=Arial color=#000099 size=5>"));	CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<B>Validation Report</B>"));					CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,--m_nTabCount);	szTitle.append(_T("</FONT>"));									CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,--m_nTabCount);	szTitle.append(_T("</P>"));										CNSHTMLLogger::addEnters(szTitle,3);

    szTitle.append(addTable());
    //std::map<NSSTRING,NSSTRING>::iterator headerItr;
        
    int nIndexCntr = 0;
	for(int nMsgCntr = 0; nMsgCntr < (int)pCollection->GetSize(); nMsgCntr++)
	{
		TCHAR* szMessage = pCollection->GetLogAt(nMsgCntr);
		szTitle.append(addRowInTable());
			szTitle.append(addColumnInRow());
				szTitle.append(szMessage);
			szTitle.append(endColumnInRow());
		szTitle.append(endRowInTable());
    }
  
    szTitle.append(endTable());
    return szTitle;
}

/*!
 @Brief			Add report header html title
 @Param [in]	none
 @Return		report header string
 @Date			29 Jan 2009
*/
NSSTRING CNSHTMLLogger::addReportHeaderEx()
{
    NSSTRING szTitle(_T(""));

	CNSHTMLLogger::addTabs(szTitle,m_nTabCount);	szTitle.append(_T("<BODY bgcolor=\"ffffff\">"));				CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<P align=center>"));							CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<FONT face=Arial color=#000099 size=5>"));	CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,++m_nTabCount);	szTitle.append(_T("<B>Validation Report</B>"));					CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,--m_nTabCount);	szTitle.append(_T("</FONT>"));									CNSHTMLLogger::addEnters(szTitle,1);
	CNSHTMLLogger::addTabs(szTitle,--m_nTabCount);	szTitle.append(_T("</P>"));										CNSHTMLLogger::addEnters(szTitle,3);

    return szTitle;
}


/*!
 @Brief			Add error table
 @Param [in]	Collection of error messages  and type of error
 @Return		report error table string
 @Date			23 Aug 2007
*/
NSSTRING CNSHTMLLogger::addErrorData(CNSLogCollection *pCollection, LOGTYPE enType)
{
	int nCollectionSize = (int)pCollection->GetSize();
	if(nCollectionSize == 0) return _T("");
    NSSTRING szErrorData;
    szErrorData.append(_T(""));
    szErrorData.append(addTable(FALSE,TRUE));
    szErrorData.append(addRowInTable());
    szErrorData.append(addColumnInRow(10,_T("ff0000")));
	if(enType == LOG_CAD)
		szErrorData.append(_T("CAD Errors"));
	else if(enType == LOG_GIS)
		szErrorData.append(_T("GIS Errors"));
	else if(enType == LOG_OTHER)
		szErrorData.append(_T("Errors"));
    szErrorData.append(endColumnInRow());
    szErrorData.append(endRowInTable());
    szErrorData.append(endTable());

    addEmptyLine();
    addEmptyLine();
    addEmptyLine();

    szErrorData.append(addTable());
    int nIndexCntr = 0;
	for(int nMsgCntr = 0; nMsgCntr < nCollectionSize; nMsgCntr++)
	{
        TCHAR* szMessage = pCollection->GetLogAt(nMsgCntr);
        szErrorData.append(addRowInTable());
        szErrorData.append(addColumnInRow());
		if(enType == LOG_CAD)
			szErrorData.append(_T("CAD_ERR_"));
		if(enType == LOG_GIS)
			szErrorData.append(_T("GIS_ERR_"));
		else if(enType == LOG_OTHER)
			szErrorData.append(_T("Errors"));
		//szErrorData.append(_T("CAD_ERR_"));
       	TCHAR id[20];
        #ifdef _UNICODE 
			NSITOW(nMsgCntr + 1,id,20,10);
		#else
			NSITOW(nMsgCntr + 1,id,10);
		#endif
        szErrorData.append(id);
        szErrorData.append(endColumnInRow());

        szErrorData.append(addColumnInRow());
			szErrorData.append(szMessage);
        szErrorData.append(endColumnInRow());
    }
    szErrorData.append(endTable());
    return szErrorData;
}

/*!
 @Brief			Add Info table
 @Param [in]	Collection of info records
 @Return		report info table string
 @Date			16 Feb 2009
*/
NSSTRING CNSHTMLLogger::addInfoData(std::vector<ErrRecord*> &m_vectLogErrRecs)
{
	NSSTRING szErrorData = _T("");

	std::sort (m_vectLogErrRecs.begin(), m_vectLogErrRecs.end(), MultiColumnSort);	

    szErrorData.append(addTable());


	for(std::vector<ErrRecord*>::iterator theIt = m_vectLogErrRecs.begin(); theIt !=  m_vectLogErrRecs.end(); theIt++)
	{
		
	ErrRecord *er = *theIt ; 

	if(er->nLogType != LOG_HEADER)
		continue;

	int iErrorNo					= er->nErrorNo;
	
	TCHAR szErrorNo[5] = _T("");
	NSSPRINTF(szErrorNo, _T("%d"), iErrorNo);
	NSSTRING strErrorNo(szErrorNo);

	NSSTRING FORECOL = _T("000000");
	NSSTRING BGCOL = _T("FFFFFF");

	NSSTRING strErrorDescription	= er->strErrorDescription;

	szErrorData.append(addRowInTable());


		szErrorData.append(addColumnInRow(10,FORECOL,BGCOL));	
			szErrorData.append(strErrorDescription); 
		szErrorData.append(endColumnInRow());


    szErrorData.append(endRowInTable());

	}


szErrorData.append(endTable());

	



return szErrorData;
}



/*!
 @Brief			Add OTHER Info table
 @Param [in]	Collection of info records
 @Return		report info table string
 @Date			16 Feb 2009
*/
NSSTRING CNSHTMLLogger::addOtherInfoData(std::vector<ErrRecord*> &m_vectLogErrRecs)
{

	NSSTRING szErrorData = _T("");

	int nOtherLogs = CNSReportWriter::getReportWriterInstance()->getNumberOfLogsEx(LOG_OTHER);

	if( nOtherLogs > 0) 
	{

	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	szErrorData.append(_T("<P align=left>"));							CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	szErrorData.append(_T("<FONT face=Arial color=#000099 size=5>"));	CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	szErrorData.append(_T("<B>SYSTEM ERRORS</B>"));					CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,--m_nTabCount);	szErrorData.append(_T("</FONT>"));									CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,--m_nTabCount);	szErrorData.append(_T("</P>"));	

	std::sort (m_vectLogErrRecs.begin(), m_vectLogErrRecs.end(), MultiColumnSort);	

    szErrorData.append(addTable());


	for(std::vector<ErrRecord*>::iterator theIt = m_vectLogErrRecs.begin(); theIt !=  m_vectLogErrRecs.end(); theIt++)
	{
		
	ErrRecord *er = *theIt ; 

	if(er->nLogType != LOG_OTHER)
		continue;

	int iErrorNo					= er->nErrorNo;
	
	TCHAR szErrorNo[5] = _T("");
	NSSPRINTF(szErrorNo, _T("%d"), iErrorNo);
	NSSTRING strErrorNo(szErrorNo);

	NSSTRING FORECOL = _T("000000");
	NSSTRING BGCOL = _T("FFFFFF");

	NSSTRING strErrorDescription	= er->strErrorDescription;

	szErrorData.append(addRowInTable());


		szErrorData.append(addColumnInRow(10,FORECOL,BGCOL));	
			szErrorData.append(strErrorDescription); 
		szErrorData.append(endColumnInRow());


    szErrorData.append(endRowInTable());

	}


szErrorData.append(endTable());

	
	}


return szErrorData;
}


/*!
 @Brief			Add error table
 @Param [in]	Collection of error records
 @Return		report error table string
 @Date			29 Jan 2009
*/
NSSTRING CNSHTMLLogger::addErrorData(std::vector<ErrRecord*> &m_vectLogErrRecs, LOGTYPE enType)
{
	
	NSSTRING szErrorData = _T("");

	int nErrLogs = CNSReportWriter::getReportWriterInstance()->getNumberOfLogs(enType);

	if( nErrLogs > 0)  
	{

	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	szErrorData.append(_T("<P align=left>"));							CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	szErrorData.append(_T("<FONT face=Arial color=#000099 size=3>"));	CNSHTMLLogger::addEnters(szErrorData,1);

	CNSHTMLLogger::addTabs(szErrorData,++m_nTabCount);	

		szErrorData.append(_T("<B><u>"));

			switch ( enType )
			{

			  case LOG_CAD : 
				szErrorData.append(_T("CAD ERRORS"));
				break;

			  case LOG_GIS : 
				szErrorData.append(_T("GIS ERRORS"));
				break;

			  //default : 

			}
		szErrorData.append(_T("</u></B>"));	

	CNSHTMLLogger::addEnters(szErrorData,1);

	CNSHTMLLogger::addTabs(szErrorData,--m_nTabCount);	szErrorData.append(_T("</FONT>"));									CNSHTMLLogger::addEnters(szErrorData,1);
	CNSHTMLLogger::addTabs(szErrorData,--m_nTabCount);	szErrorData.append(_T("</P>"));										CNSHTMLLogger::addEnters(szErrorData,3);


	std::sort (m_vectLogErrRecs.begin(), m_vectLogErrRecs.end(), MultiColumnSort);	

    szErrorData.append(addTable());

    

	NSSTRING strCurrErrorCategory		= _T("");
	for(std::vector<ErrRecord*>::iterator theIt = m_vectLogErrRecs.begin(); theIt !=  m_vectLogErrRecs.end(); theIt++)
	{

	ErrRecord *er = *theIt ; 

	if(er->nLogType != enType)
		continue;

	int iErrorNo					= er->nErrorNo;
	
	TCHAR szErrorNo[5] = _T("");
	NSSPRINTF(szErrorNo, _T("%d"), iErrorNo);
	NSSTRING strErrorNo(szErrorNo);

	NSSTRING strFORECOL = _T("000000");
	NSSTRING strBGCOL = _T("FFFFFF");



	NSSTRING strErrorSeverity		= er->strErrorSeverity;
	NSSTRING strErrorCategory		= er->strErrorCategory;
	NSSTRING strErrorDescription	= er->strErrorDescription;

	NSSTRING strErrorID	= _T("");


	switch ( enType )
	{

	  case LOG_CAD : 
		strErrorID	= _T("CAD_ERR_");
		break;

	  case LOG_GIS : 
		strErrorID	= _T("GIS_ERR_");
		break;

	  //default : 

	}



	int iErrorID	= er->nErrorID;

	TCHAR szFormat[(_MAX_PATH + 1)];
	
	TCHAR id[20];

        #ifdef _UNICODE 
			NSITOW(iErrorID,id,20,10);
		#else
			NSITOW(iErrorID,id,10);
		#endif

    strErrorID.append(id);


	if(strCurrErrorCategory != strErrorCategory)
	{
		strCurrErrorCategory = strErrorCategory;
		szErrorData.append(addRowInTable());			
			szErrorData.append(addColumnInRow(10,_T("000000"),_T("FFFFFF"),_T("3")));	
				szErrorData.append(_T("<p align=\"left\"><B>")); 
				szErrorData.append(strCurrErrorCategory); 
				szErrorData.append(_T("</B></p>")); 
			szErrorData.append(endColumnInRow());
		szErrorData.append(endRowInTable());

		addHeaderRow(szErrorData);
	}

	NSSTRING strPTagID = _T("id =\'")+strErrorSeverity+_T("\'");

	szErrorData.append(addRowInTable(strPTagID));


		szErrorData.append(addColumnInRow(10));	
			szErrorData.append(_T("<p align=\"center\">"));
				szErrorData.append(strErrorID); 
			szErrorData.append(_T("</p>"));
		szErrorData.append(endColumnInRow());

		szErrorData.append(addColumnInRow(10));	
			szErrorData.append(_T("<p align=\"center\">"));
				szErrorData.append(strErrorSeverity); 
			szErrorData.append(_T("</p>"));
		szErrorData.append(endColumnInRow());

		//szErrorData.append(addColumnInRow(10,FORECOL,BGCOL));	
		//	szErrorData.append(_T("<p align=\"center\">"));
		//		szErrorData.append(strErrorCategory); 
		//	szErrorData.append(_T("</p>"));
		//szErrorData.append(endColumnInRow());


		szErrorData.append(addColumnInRow(10 ));	
			szErrorData.append(_T("<p align=\"left\">("));
				szErrorData.append(strErrorNo); 
			szErrorData.append(_T(") \t "));
			szErrorData.append(strErrorDescription); 
			szErrorData.append(_T("</p>"));
		szErrorData.append(endColumnInRow());


    szErrorData.append(endRowInTable());

	}


szErrorData.append(endTable());

	}



return szErrorData;

}
//
/*!
 @Brief			Add table in html report
 @Param [in]	border and color
 @Return		table string
 @Date			23 Aug 2007
*/
NSSTRING CNSHTMLLogger::addTable(BOOL bBorder, BOOL bgColor)
{
    NSSTRING szTable(_T("\n"));
    if(bBorder)
    {
        if(bgColor)
            szTable.append(_T("<TABLE style=\"BORDER-COLLAPSE: collapse\" cellSpacing=0 cellPadding=3 bgColor=\"#33C3E0\" borderColor=#1012F1 border=1>"));
        else
            szTable.append(_T("<TABLE style=\"BORDER-COLLAPSE: collapse\" cellSpacing=0 cellPadding=3 borderColor=#1012F1 border=1>"));
    }
    else
    {
        if(bgColor)
            szTable.append(_T("<TABLE style=\"BORDER-COLLAPSE: collapse\" cellSpacing=0 cellPadding=3 bgColor=\"#33C3E0\" borderColor=#1012F1 border=0>"));
        else
            szTable.append(_T("<TABLE style=\"BORDER-COLLAPSE: collapse\" cellSpacing=0 cellPadding=3 borderColor=#1012F1 border=0>"));
    }
    szTable.append(_T("\n\n<TBODY>"));
    return szTable;
}

NSSTRING CNSHTMLLogger::endTable()
{
    NSSTRING szTable(_T(""));
    szTable.append(_T("</TBODY>\n"));
	szTable.append(_T("\n"));
    szTable.append(_T("</TABLE>\n\n"));
    return szTable;
}
NSSTRING CNSHTMLLogger::addRowInTable(NSSTRING strPTagID)
{
    NSSTRING szRow(_T(""));
	CNSHTMLLogger::addEnters(szRow,1);
	CNSHTMLLogger::addTabs (szRow,m_nTabCount++);
    szRow.append(_T("<TR "));
	szRow.append(strPTagID);
	szRow.append(_T(">"));
    return szRow;
}
NSSTRING CNSHTMLLogger::endRowInTable()
{
    NSSTRING szRow(_T(""));
	szRow.append(_T("\n"));
	CNSHTMLLogger::addTabs (szRow,--m_nTabCount);
    szRow.append(_T("</TR>"));
	szRow.append(_T("\n"));
    return szRow;
}
NSSTRING CNSHTMLLogger::addColumnInRow(int size, NSSTRING color, NSSTRING bgcolor, NSSTRING colspan)
{
    NSSTRING szRow(_T(""));
	CNSHTMLLogger::addEnters(szRow,1);
	CNSHTMLLogger::addTabs (szRow,m_nTabCount++);

    szRow.append(_T("<TD"));


	if(NSSTRCMP(color.c_str(),_T("1")))
    {
        szRow.append(_T(" colspan="));
        szRow.append(colspan);
        szRow.append(_T(" "));
    }

    szRow.append(_T("><FONT face=Times New Roman"));

    if(size!=10)
    {
        szRow.append(_T(" size="));
      		
		TCHAR szSize[20];
        #ifdef _UNICODE 
			NSITOW(size + 1,szSize,20,10);
		#else
		    NSITOW(size + 1,szSize,10);
		#endif

        szRow.append(szSize);
        szRow.append(_T(" "));
    }
    szRow.append(_T(">"));
    return szRow;
}
NSSTRING CNSHTMLLogger::endColumnInRow()
{
    NSSTRING szRow(_T(""));
    szRow.append(_T("</FONT>"));
	//szRow.append(_T("\n"));
	szRow.append(_T("</TD>"));
	m_nTabCount--;
    return szRow;
}
NSSTRING CNSHTMLLogger::endHTMLReport()
{
    NSSTRING szHtmlEnd(_T(""));
    szHtmlEnd.append(_T("</BODY>"));
	szHtmlEnd.append(_T("\n"));
	szHtmlEnd.append(_T("</HTML>"));
    return szHtmlEnd;
}


void CNSHTMLLogger::addTabs(NSSTRING &szString, int n)
{
	for(int i = 0; i<n; i++)
		szString.append(_T("\t"));
}

void CNSHTMLLogger::addEnters(NSSTRING &szString, int n)
{
	for(int i = 0; i<n; i++)
		szString.append(_T("\n"));
}


/*!
 @Brief			Write HTML string for Table Column Header
 @Param [in]	szErrorData - HTML string
 @Param [in/out]szErrorData - HTML string
 @Return		bool
 @Date			16 Feb 2009
*/

void CNSHTMLLogger::addHeaderRow(NSSTRING &szErrorData)
{
	szErrorData.append(addRowInTable(_T("id = P0")));
		szErrorData.append(addColumnInRow(10,_T("FFFF00"),_T("000000")));	szErrorData.append(_T("<p align=\"center\"><B>ErrorID</B></p>")); szErrorData.append(endColumnInRow());
		szErrorData.append(addColumnInRow(10,_T("FFFF00"),_T("000000")));	szErrorData.append(_T("<p align=\"center\"><B>ErrorSeverity</B></p>")); szErrorData.append(endColumnInRow());
		//szErrorData.append(addColumnInRow(10,_T("FFFF00"),_T("000000")));	szErrorData.append(_T("<p align=\"center\"><B>ErrorCatagory</B></p>")); szErrorData.append(endColumnInRow());
		szErrorData.append(addColumnInRow(10,_T("FFFF00"),_T("000000")));	
			szErrorData.append(_T("<p align=\"center\"><B>ErrorDescription</B></p>"));	
		szErrorData.append(endColumnInRow());
	szErrorData.append(endRowInTable());
}


