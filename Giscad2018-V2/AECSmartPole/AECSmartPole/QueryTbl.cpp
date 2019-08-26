////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : QueryTbl.cpp
// Description      : Facilitates database access
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "QueryTbl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utility function to reduce the number of redundant spaces in the SQL statement
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString ResetSpaces(CString csSQL)
{
  // Make a copy of the string
  CString csSQLCopy = csSQL;

  // Repeatedly replace all double spaces with a single space
  csSQLCopy.Replace(_T("  "), _T(" "));
  csSQLCopy.Replace(_T("  "), _T(" "));
  csSQLCopy.Replace(_T("  "), _T(" "));
  csSQLCopy.Replace(_T("  "), _T(" "));
  csSQLCopy.Replace(_T("  "), _T(" "));
  csSQLCopy.Replace(_T("  "), _T(" "));

  // Return the copied and un-double-spaced string
  return csSQLCopy;
}

CQueryTbl::CQueryTbl()
{
  m_iColums = -1;
  m_pcsaRow = NULL;
  m_iCurLine = 0;
  m_csCurFile = _T("");
  m_csDatabasePassword = _T("");
}

CQueryTbl::~CQueryTbl()
{
  Close();
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : ClearPointersMemory
|  Created     : 3-9-2002   8:37
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : To Clear memory alocated by the pointers stored in this array
|-------------------------------------------------------------------------------------------------------*/
void CQueryTbl::Close()
{
  for (int iCount = GetUpperBound(); iCount > -1; iCount--)
  {
    m_pcsaRow = (CStringArray *)GetAt(iCount);
    m_pcsaRow->RemoveAll();
    RemoveAt(iCount);
    delete m_pcsaRow;
  }
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : SqlExecute
|  Created     : 3-9-2002   8:43
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : Executes an Sql without returning anything
|-------------------------------------------------------------------------------------------------------*/
BOOL CQueryTbl::SqlExecute(CString csDsn, CString csSql, int iLineNumb, CString csFileName, CString csFunctionName)
{
  // Force a closure of the database
  Close();

  // Reset the number of columns
  m_iColums = -1;    
    
  // Open the database
  CDatabase cdDSN;
  try { cdDSN.Open(csDsn, FALSE, FALSE, _T("ODBC;UID=;PWD=" + m_csDatabasePassword)); } 
  catch (CDBException *e) 
  { 
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError + _T("\n\n") + ResetSpaces(csSql)); 
    return FALSE;
  }
  
  // Execute the SQL statement
  try { cdDSN.ExecuteSQL(csSql); } 
  catch (CDBException *e) 
  { 
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError + _T("\n\n") + ResetSpaces(csSql)); 
    return FALSE;
  }

  // Close that connection
  cdDSN.Close();

  // Success
  return TRUE;
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : SqlRead
|  Created     : 3-9-2002   8:43
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : Executes and returns records read by it
|-------------------------------------------------------------------------------------------------------*/
BOOL CQueryTbl::SqlRead(CString csDsn, CString csSql, int iLineNumb, CString csFileName, CString csFunctionName)
{
  // Force a closure of the database
  Close();

  // Reset the number of columns
  m_iColums = -1;    

  // Open the database
  CDatabase cdDSN;
  try { cdDSN.Open(csDsn, FALSE, FALSE, _T("ODBC;UID=;PWD=" + m_csDatabasePassword), FALSE); } 
  catch (CDBException *e) 
  { 
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError + _T("\n\n") + ResetSpaces(csSql)); 
    return FALSE;
  }

	// Open the table with the given SQL statement
  CRecordset crRecords(&cdDSN);
  try { crRecords.Open(AFX_DB_USE_DEFAULT_TYPE, csSql); } 
  catch (CDBException *e) 
  {
    cdDSN.Close();
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError + _T("\n\n") + ResetSpaces(csSql)); 
    return FALSE;
  }

  // If no records resulted, close the table and return now
  if (crRecords.IsBOF())
  {
    crRecords.Close();
    cdDSN.Close();
    return TRUE;
  }

  // Get the field count
  m_iColums = crRecords.GetODBCFieldCount();
  
  // Retrieve the field values into the row arrays
  for (crRecords.MoveFirst(); !crRecords.IsEOF(); crRecords.MoveNext())
  {
    m_pcsaRow = new CStringArray;
    for (short iC = 0; iC < m_iColums; iC++)
    {
      crRecords.GetFieldValue(iC, m_csTemp);
      m_pcsaRow->Add(m_csTemp);
    }
    Add(m_pcsaRow);
  }

  // Store the file name and the line number
  m_csCurFile = csFileName;
  m_iCurLine  = iLineNumb;

  // Close the recordset
  crRecords.Close();

  // Close the database
  cdDSN.Close();

  // Return success
  return TRUE;
}


//--------------------------------------------------------------------------------------------------------
//-Utility functions


/*-------------------------------------------------------------------------------------------------------
|  Function    : GetRows
|  Created     : 3-9-2002   9:40
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : Caller
|  Returns     : Nothing
|  Description : To get the number of rows fetched
|-------------------------------------------------------------------------------------------------------*/
int CQueryTbl::GetRows()
{
  return GetSize();
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : GetColumns
|  Created     : 3-9-2002   9:41
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : Caller
|  Returns     : Nothing
|  Description : To get the number of columns in the result
|-------------------------------------------------------------------------------------------------------*/
int CQueryTbl::GetColumns()
{
  return m_iColums;
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : GetRowAt
|  Created     : 3-9-2002   9:43
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : To get the data in the column and index
|-------------------------------------------------------------------------------------------------------*/
CStringArray* CQueryTbl::GetRowAt(int iRow)
{
  if ((iRow >= GetSize()) || (iRow < 0))
  {
    CString csLine;
    csLine.Format(_T("%d"), iRow);
    appError(m_csCurFile, _T("GetRowAt"), m_iCurLine, _T("Invalid row index [") + csLine + _T("] provided for function"));
    return NULL;
  }

  return (CStringArray*)GetAt(iRow);
}


/*-------------------------------------------------------------------------------------------------------
|  Function    : GetColumnAt
|  Created     : 3-9-2002   9:50
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : To get the entire column in a string array
|-------------------------------------------------------------------------------------------------------*/
void CQueryTbl::GetColumnAt(int iColumn, CStringArray &csaColumn)
{
  if ((iColumn >= m_iColums) || (iColumn < 0))
  {
    appError(_T(__FILE__), _T("GetColumnAt"), __LINE__, _T("Invalid column index provided for function"));
    return;
  }
  
  // First Clear all the data
  csaColumn.RemoveAll();

  for (int iCount = 0; iCount < GetSize(); iCount++)
  {
    m_pcsaRow = (CStringArray *) GetAt(iCount);
    csaColumn.Add(m_pcsaRow->GetAt(iColumn));
  }
  
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : Open
|  Created     : 13-9-2002   11:55
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : Opens a database in the member database object
|-------------------------------------------------------------------------------------------------------*/
BOOL CQueryTbl::Open(CString csDsn, int iLineNumb, CString csFileName, CString csFunctionName)
{ 
  if (m_cdDsn.IsOpen())
  {
    appError(csFileName, csFunctionName, iLineNumb, _T("Data base already opened")); 
    return FALSE;
  }

  // Open the database
  try { m_cdDsn.Open(csDsn, FALSE, FALSE, _T("ODBC;UID=;PWD=" + m_csDatabasePassword)); } 
  catch (CDBException *e) 
  { 
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError); 
    return FALSE;
  }
  return TRUE;
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : ExecuteSql
|  Created     : 13-9-2002   11:55
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : Excecute the sql in member database object
|-------------------------------------------------------------------------------------------------------*/
BOOL CQueryTbl::ExecuteSql(CString csSql, int iLineNumb, CString csFileName, CString csFunctionName)
{
  if (!m_cdDsn.IsOpen())
  {
    appError(csFileName, csFunctionName, iLineNumb, _T("Database not opened")); 
    return FALSE;
  }

  // Execute the given SQL on the already _topen database
  try { m_cdDsn.ExecuteSQL(csSql); } 
  catch (CDBException *e) 
  {
    appError(csFileName, csFunctionName, iLineNumb, e->m_strError + _T("\n\n") + ResetSpaces(csSql)); 
    return FALSE;
  }

  return TRUE;
}

/*-------------------------------------------------------------------------------------------------------
|  Function    : CloseDatabase
|  Created     : 13-9-2002   11:54
|  Filename    : \\DCSSERVER\PROJECTS\SPACE2K\Input\QueryTbl.cpp
|  Author      : Rakesh S. S
|  Arguments   : Nill
|  Called from : 
|  Returns     : Nothing
|  Description : Close the member database object
|-------------------------------------------------------------------------------------------------------*/
BOOL CQueryTbl::CloseDatabase()
{
  if (!m_cdDsn.IsOpen())
  {
    return FALSE;
  }
  return TRUE;
}

//--------------------------------------------------------------------------------------------------------
//public functions
void CQueryTbl::RemoveAt(int nIndex, int nCount)
{
  CPtrArray::RemoveAt(nIndex, nCount);
}

void* CQueryTbl::GetAt(int nIndex) const
{
  return CPtrArray::GetAt(nIndex);
}

int CQueryTbl::Add(void* newElement)
{
  return CPtrArray::Add(newElement);
}
