#include "stdafx.h"
//#include "..\Include\NSErrorCodes.h"
#include "..\Include\NSErrorCodes.h"
//#include "..\validator\validator\NSErrorParser.h"
#include "..\Include\NSDatabaseMgr.h"
//#include "..\validator\validator\NSBaseValidator.h"
#include "NSErrorParser.h"
#include "NSBaseValidator.h"

/*!
 @Brief			Fetchs the Error record from the standard database table
 @Param [in]	ErrNo	- error number eg 420
 @Param [out]	ErrRecord	- error record
 @Return		None
 @Date			27 Jan 2009
*/
bool GetErrRecord(int ErrNo, ErrRecord &ErrRec)
{

	HRESULT hr = S_OK;
	bool retval = false ;
	
		TCHAR szQuery[_MAX_PATH + 1];
		NSSPRINTF(szQuery, _T("Select * from tblValidationErrors where fldErrorNo = %d"),ErrNo);        
		int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery((TCHAR*)szQuery);

		if(-1 != nDBRet)
		{
            NSSTRING strErrNo  = _T("fldErrorNo");
			NSSTRING strErrorSeverity  = _T("fldErrorSeverity");
			NSSTRING strErrorCategory = _T("fldErrorCategory");
			NSSTRING strErrorDescription  = _T("fldErrorDescription");
			//Read notRequired column value from database (tblValidationErrors table.)
			NSSTRING strErrorIsRequired  = _T("notRequired");         

			bool bFNO1,bFNO2,bFNO3,bFNO4,bFNO5 ;
			variant_t valErrNo,valErrSev,valErrCat,valErrDesc, valErrReq;
			 
			bFNO1 = CNSDatabaseMgr::getInstance()->getValue(strErrNo.c_str(), valErrNo);
			bFNO2 = CNSDatabaseMgr::getInstance()->getValue(strErrorSeverity.c_str(), valErrSev);
			bFNO3 = CNSDatabaseMgr::getInstance()->getValue(strErrorCategory.c_str(), valErrCat);
			bFNO4 = CNSDatabaseMgr::getInstance()->getValue(strErrorDescription.c_str(), valErrDesc);
			bFNO5 = CNSDatabaseMgr::getInstance()->getValue(strErrorIsRequired.c_str(), valErrReq);

			int iErrNo = -1, iErrReq = -1;
			NSSTRING strErrSev,strErrCat,strErrDesc;
			if(bFNO1 && bFNO2 && bFNO3 && bFNO4 && bFNO5)
			{
				retval = true;

					#ifndef _INTERNAL_
						iErrNo   = valErrNo.intVal ;
						strErrSev  = valErrSev.bstrVal;
						strErrCat  = valErrCat.bstrVal;
						strErrDesc = valErrDesc.bstrVal;
						iErrReq = valErrReq.intVal;

						ErrRec.nErrorNo = iErrNo;
						ErrRec.strErrorSeverity = strErrSev;
						ErrRec.strErrorCategory = strErrCat;
						ErrRec.strErrorDescription = strErrDesc;
						ErrRec.nErrReq = iErrReq;

					#else

						int ivalue;
						char *strvalue1, *strvalue2, *strvalue3 ;

						iErrNo   = valErrNo.intVal ;
						strErrSev  = valErrSev.bstrVal;
						strErrCat  = valErrCat.bstrVal;
						strErrDesc = valErrDesc.bstrVal;
						iErrReq = valErrReq.intVal;

						ErrRec.nErrorNo = iErrNo;
						ErrRec.strErrorSeverity = strErrSev;
						ErrRec.strErrorCategory = strErrCat;
						ErrRec.strErrorDescription = strErrDesc;
						ErrRec.nErrReq = iErrReq;
				        /*ivalue = valErrNo.intVal ;iErrNo = ivalue;						
						strvalue1 = _com_util::ConvertBSTRToString(valErrSev.bstrVal);strErrSev = strvalue1;
						strvalue2 = _com_util::ConvertBSTRToString(valErrCat.bstrVal);strErrCat = strvalue2;
						strvalue3 = _com_util::ConvertBSTRToString(valErrDesc.bstrVal);strErrDesc = strvalue3;

						ErrRec.nErrorNo = ivalue;
						ErrRec.strErrorSeverity = strvalue1;
						ErrRec.strErrorCategory = strvalue2;
						ErrRec.strErrorDescription = strvalue3;*/

					#endif
			}

		}

	return retval;
}

int WriteIDToLog(LOGTYPE enLogType, TCHAR* szMessage, int nErrorID)
{
    if(nErrorID != -1)
    {
        TCHAR szTemp[_MAX_PATH + 1];
        NSSPRINTF(szTemp, _T(" (%i) %s"), nErrorID, szMessage);
        return WriteToLog(enLogType, szTemp);    
    }
    else
        return WriteToLog(enLogType, szMessage);    
}

int WriteIDToLog(ErrRecord er)
{
	//return CNSReportWriter::getReportWriterInstance()->writeLogEx(er);
	
	return WriteToLogEx(er);
}

/*!
 @Brief			Sends the error record for parsing
 @Various overloads to tackle various vatiations
 @Param [in]	LOGTYPE	- eg LOG_HEADER = 1, LOG_CAD = 2, LOG_GIS = 3,LOG_OTHER = 4
 @Param [in]	ErrNo	- error number eg 420
 @Return		None
 @Date			27 Jan 2009
*/
int WriteErrorRec(LOGTYPE enLogType, int ErrNo, CNSValidationDataCollector *pTempFeature, CNSProposedRelations *pTempPropRel )
{
	int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
	nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {   
	    nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er,pTempFeature,pTempPropRel));	
	  } 
	  else
	  {
	    er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;

}

int WriteErrorRec(LOGTYPE enLogType, int ErrNo,CNSAsset *pAsset, NSSTRING strMsg )
{
    int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
	    nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er,pAsset, strMsg));	
	  }
	  else
	  {
	    er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;

}

int WriteErrorRec(LOGTYPE enLogType, int ErrNo,TCHAR *pszMsg )
{
    int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
	nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
    if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == false ) //Nothing found don't change the description
	  {
	    er.strErrorDescription = pszMsg;
		nErr = WriteIDToLog(er);	
	  }
	  else
	  {
		er.nErrorNo = ErrNo;
		//nErr = WriteIDToLog(er);	
		nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er));	
	  }
	}

	return nErr;
}





int WriteErrorRec(LOGTYPE enLogType, int ErrNo,NSSTRING pszTag, NSSTRING strData )
{
	int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
    {
	  if ( retval == true )
	  {
		nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er, pszTag,  strData ));	
	  }
	  else
	  {
		er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;
}
int WriteErrorRec(LOGTYPE enLogType, int ErrNo,NSSTRING strTag,NSSTRING locLay, NSSTRING stdLay )
{
    int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
	    nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er,strTag,locLay,stdLay ));	
	  }
	  else
	  {
	    er.nErrorNo = ErrNo;
	    nErr = WriteIDToLog(er);
	  }
	}

	return nErr;
}
int WriteErrorRec(LOGTYPE enLogType, int ErrNo,NSSTRING PropName, NSSTRING Stylename, NSSTRING CurrVal, NSSTRING StdVal )
{
    int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
	    nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDesc(er,PropName, Stylename,CurrVal,StdVal ));	
	  }
	  else
	  {
	    er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;
}

int WriteErrorRecEx(LOGTYPE enLogType, int ErrNo,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2 )
{
	int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
		nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDescEx(er,Tag1, Data1,Tag2,Data2 ));	
	  }
	  else
	  {
	    er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;

}
int WriteErrorRecEx(LOGTYPE enLogType, int ErrNo, CNSValidationDataCollector *pTempFeature,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2  , NSSTRING Tag3 , NSSTRING Data3 )
{
	int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
	    nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDescEx(er,pTempFeature,Tag1, Data1,Tag2,Data2,Tag3,Data3  ));	
	  }
	  else
	  {
	    er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
	  }
	}

	return nErr;

}

int WriteErrorRecEx(LOGTYPE enLogType, int ErrNo, CNSValidationDataCollector *pTempFeature,CNSProposedRelations *pTempPropRel , NSSTRING Tag1, NSSTRING Data1)
{
    int nErr;
	ErrRecord er ; 
	bool retval = GetErrRecord(ErrNo,er);

	er.nLogType = enLogType;
    nErr = 0;
	/*First IF is for checking related error checkBox is checked on Dialog and 
	also notRequired column value = true*/
	if((CNSBaseValidator::m_nP1ErrorFlag == 1 && NSSTRCMP(_T("P1"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP2ErrorFlag == 1 && NSSTRCMP(_T("P2"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP3ErrorFlag == 1 && NSSTRCMP(_T("P3"),er.strErrorSeverity.c_str()) == 0 ||
	    CNSBaseValidator::m_nP4ErrorFlag == 1 && NSSTRCMP(_T("P4"),er.strErrorSeverity.c_str()) == 0) && er.nErrReq == 0)
	{
	  if ( retval == true )
	  {
		nErr = WriteIDToLog(CNSErrorParser::getInstance()->ParseDescEx(er,pTempFeature,pTempPropRel,Tag1,Data1));	
	  }
	  else
	  {
		er.nErrorNo = ErrNo;
		nErr = WriteIDToLog(er);
      }
	}

	return nErr;
}

int WriteErrorRec( LOGTYPE enLogType, NSSTRING strErrorString)
{
	ErrRecord er ;
	er.nLogType = enLogType;
	er.strErrorDescription = strErrorString;	 
	WriteIDToLog(er);	
	return 0;
}

//ErrRecord WriteErrorRecEr( LOGTYPE enLogType, NSSTRING strErrorString)
//{
//	ErrRecord er ;
//	er.nLogType = enLogType;
//	er.strErrorDescription = strErrorString;	 
//	//WriteIDToLog(er);	
//	////WriteToLogEx(er);
//	CNSReportWriter::getReportWriterInstance()->writeLogEx(er);
//
//	return er;
//}



