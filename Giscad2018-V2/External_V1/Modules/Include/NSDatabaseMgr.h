/*!
 @Class		:	CNSDatabaseMgr
 @Brief		:	This class provides the functionality for opening and closing database
				connection.
 @Author	:	Neilsoft Ltd
 @Date		:	25-06-2007
 @History 	:	Change history
 */

#pragma once
#pragma warning( push )
#pragma warning( disable: 4251)
#pragma warning( pop )
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include "map"
#include "vector"
#include "tchar.h"

enum TABLE_TYPE
	{
		TABLE_INVALID = -1,
		TABLE_FEATURE_LIST = 1,
		TABLE_COMP_LIST = 2,
		TABLE_POLE_LIST = 3,
		TABLE_SL_LIST = 4,
		TABLE_SWITCH_LIST = 5,
		TABLE_COLUMN_LIST = 6,
		TABLE_LINE_LIST = 7,
		TABLE_CHILD_LIST = 8,
		TABLE_PARENT_LIST = 9
	};
//typedef std::map<TABLE_TYPE, CNSTable*> TABLE_MAP;

class CNSDatabaseMgr//__declspec(dllexport)
{
private:
	_ConnectionPtr m_pConn; // Connection pointer
	_RecordsetPtr m_pRecSet; // Recordset pointer
	//TABLE_MAP m_mapOfTable; // map to store all table objects

public:
	CNSDatabaseMgr(void);
public:

	~CNSDatabaseMgr(void);
	static CNSDatabaseMgr* getInstance();
	int openDatabase(const TCHAR* szUserName, const TCHAR* szPwd, const TCHAR* cmdStr);
	void closeDatabase();
	/*CNSRecord* getRecord(TABLE_TYPE tableType, int nFNO);*/
	void setDatabaseFileLoc(const TCHAR *szFileLoc);
	int executeQuery(const TCHAR* szQuery);
	bool getValue(const TCHAR* szValue, variant_t & Holder);
	void getValue(const TCHAR* szValue, std::vector<variant_t> & vValues);
private:
	static CNSDatabaseMgr* pDatabaseMgr;
	//CNSDatabaseMgr(void);
};
