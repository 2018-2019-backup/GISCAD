#include "StdAfx.h"
#include "NSDatabaseMgr.h"
#include "..\Include\NSDirectives.h"

CNSDatabaseMgr* CNSDatabaseMgr::pDatabaseMgr = NULL;
// Constructor
CNSDatabaseMgr::CNSDatabaseMgr(void)
{
	m_pConn = NULL;
}

// Destructor
CNSDatabaseMgr::~CNSDatabaseMgr(void)
{

}

/*!
 @Brief			:	This function provides the functionality to open
					database using username and password.
 @Param [in]	:	szUserName	-	DSN UserName
 @Param [in]	:	szPwd		-	DSN Password
 @Param [in]	:	szCmdStr	-	DSN command string.
 @Return		:	int	NS_FAIL - Connection not established successfully
						NS_SUCCESS - Connection established successfully.
 @Date			:	25-06-2007
*/

int CNSDatabaseMgr::openDatabase(const TCHAR* szUserName, const TCHAR* szPwd, const TCHAR* szCmdStr)
{
	HRESULT hr = S_OK;
	try
	{
		hr = m_pConn.CreateInstance( __uuidof(Connection));
		if(hr != S_OK)
		{
			return NS_FAIL;
		}
		hr = m_pConn->Open(szCmdStr, szUserName, szPwd, -1);
		if(hr != S_OK)
		{
			return NS_FAIL;
		}
	}
	catch(...)
	{
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

/*!
 @Brief			:	This function provides the functionality to close
					database.
 @Return		:	 void
 @Date			:	25-06-2007
*/
void CNSDatabaseMgr::closeDatabase()
{
	m_pConn->Close();
}

/*!
 @Brief			:	This function provides the functionality get the record from
					database for given Feature number.
 @Param [in]	:	TABLE_TYPE	-	Table type enum value.
 @Param [in]	:	nFNO		-	Feature number
 @Return		:	Record which is fetched from the Table 
 @Date			:	25-06-2007
*/
//CNSRecord* CNSDatabaseMgr::getRecord(TABLE_TYPE tableType, int nFNO)
//{
//	CNSTable* pTable = NULL;
//	
//	TABLE_MAP::iterator it = m_mapOfTable.find(TABLE_TYPE(tableType));
//	if(it != m_mapOfTable.end())
//	{
//		pTable = (*it).second;
//	}
//	else
//	{
//		pTable = new CNSTable();
//		pTable->m_pConn = m_pConn;
//		m_mapOfTable.insert(TABLE_MAP::value_type(TABLE_TYPE(tableType), pTable));
//	}
//	return pTable->getRecord(TABLE_TYPE(tableType), nFNO);
//}

int CNSDatabaseMgr::executeQuery(const TCHAR* szQuery)
{
	try
	{
		HRESULT hr = S_OK;
		hr = m_pRecSet.CreateInstance(__uuidof(Recordset));
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_pRecSet->Open(szQuery, m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText );
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return -1;
	}
}

bool CNSDatabaseMgr::getValue(const TCHAR* szValue, variant_t & Holder)
{
	if(!m_pRecSet->adoEOF)
	{
		Holder = m_pRecSet->GetCollect(szValue);
		if(Holder.vt!=VT_NULL)
			return true;
		else
			return false;
	}
	return false;
}

void CNSDatabaseMgr::getValue(const TCHAR* szValue, std::vector<variant_t> & vValues)
{
	while(!m_pRecSet->adoEOF)
	{
		variant_t Holder = m_pRecSet->GetCollect(szValue);
		if(Holder.vt!=VT_NULL)
		{
			vValues.push_back(Holder);
		}
		m_pRecSet->MoveNext();
	}
}

CNSDatabaseMgr* CNSDatabaseMgr::getInstance()
{
    if(pDatabaseMgr == NULL)
    {
        pDatabaseMgr = new CNSDatabaseMgr();
    }
    return pDatabaseMgr;
}