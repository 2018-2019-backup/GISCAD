/*!
 @Class		ConfigFile
 @Brief		Provides functionality to read and write configuration file
 @Author	Neilsoft Ltd
 @Date		14 June 2007
 @History 	
 */

#include "StdAfx.h"
#include "..\..\Include\nsconfigfilemgr.h"
#include<AtlBase.h>
#include<AtlConv.h>

/*!
 @Brief			Constructor - Construct a ConfigFile without a file; empty
 @Date			13 June 2007
*/
ConfigFile::ConfigFile(void)
	: m_szDelimiter( NSSTRING(1,_T('=')) ), m_szComment( NSSTRING(1,_T('#')) )
{
	// 
}


/*!
 @Brief			Constructor
 @Param [in]	NSSTRING	- Filename
 @Param [in]	NSSTRING	- Delimiter
 @Param [in]	NSSTRING	- Comment
 @Param [in]	NSSTRING	- Sentry
 @Date			13 June 2007
*/
ConfigFile::ConfigFile( TCHAR* strFilename, NSSTRING strDelimiter,
                        NSSTRING strComment, NSSTRING strSentry )
	: m_szDelimiter(strDelimiter), m_szComment(strComment), m_szSentry(strSentry),m_strFilename(strFilename)
{
	// Construct a ConfigFile, getting keys and values from given file
	
	NSIFSTREAM in( strFilename);
	if( !in ) throw FileNotFound( strFilename ); 
	    AddToStream(in);
}

/*!
 @Brief			Destructor 
 @Date			13 June 2007
*/
ConfigFile::~ConfigFile(void)
{
}

/*!
 @Brief			Remove Key and its Value
 @Param [in]	NSSTRING&	- Key
 @Date			13 June 2007
*/
void ConfigFile::remove( const NSSTRING& strKey )
{
	m_mapContents.erase( m_mapContents.find( strKey ) );
	return;
}


/* static */
/*!
 @Brief			Indicate whether Key is found
 @Param [in]	NSSTRING&	- Key
 @Return		bool		- returns true if key is found
 @Date			13 June 2007
*/
bool ConfigFile::keyExists( const NSSTRING& strKey ) const
{
	m_MapConstIterator ContentItr = m_mapContents.find( strKey );
	return ( ContentItr != m_mapContents.end() );
}


/* static */
/*!
 @Brief			Remove leading and trailing whitespace
 @Param [in]	NSSTRING& 
 @Date			13 June 2007
*/
void ConfigFile::trim( NSSTRING& s )
{
	static const TCHAR szWhitespace[] = _T(" \n\t\v\r\f");
	s.erase( 0, s.find_first_not_of(szWhitespace) );
	s.erase( s.find_last_not_of(szWhitespace) + 1U );
}


/*!
 @Brief			Save a ConfigFile to os
 @Param [in]	NSOSTREAM&
 @Return		NSOSTREAM&
 @Date			13 June 2007
*/
NSOSTREAM& ConfigFile::GetFromStream( NSOSTREAM& os)
{
	for(m_MapConstIterator ContentItr = m_mapContents.begin();
	     ContentItr != m_mapContents.end();
		 ++ContentItr )
	{
		os << ContentItr->first << " " << m_szDelimiter << " ";
		os << ContentItr->second << std::endl;
	}
	return os;
}


/*!
 @Brief			Reads config file and populates map
 @Param [in]	NSISTREAM&
 @Return		NSISTREAM&
 @Date			13 June 2007
*/
NSISTREAM& ConfigFile::AddToStream( NSISTREAM& is)
{
	// Load a ConfigFile from is
	// Read in keys and values, keeping internal whitespace
	typedef NSSTRING::size_type pos;
	const NSSTRING& strDelim  = m_szDelimiter;  // separator
	const NSSTRING& strComm   = m_szComment;    // Comment
	const NSSTRING& strSentry = m_szSentry;     // end of file Sentry
	const pos skip = strDelim.length();        // length of separator
	
#if _UNICODE
	NSSTRING strNextLine = _T("");  // might need to readValue ahead to see where Value ends
#else
	NSSTRING strNextLine = "";  // might need to readValue ahead to see where Value ends
#endif
	
	while( is || strNextLine.length() > 0 )
	{
		// Read an entire Line at a time
		NSSTRING strLine;
		if( strNextLine.length() > 0 )
		{
			strLine = strNextLine;  // we readValue ahead; use it now
			strNextLine = NSDQUOATE;
		}
		else
		{
			std::getline( is, strLine);
		}
		
		// Ignore comments
		strLine = strLine.substr( 0, strLine.find(strComm) );
		
		// Check for end of file Sentry
		if( strSentry != NSDQUOATE && strLine.find(strSentry) != NSSTRING::npos ) return is;
		
		// Parse the Line if it contains a Delimiter
		pos delimPos = strLine.find( strDelim );
		if( delimPos < NSSTRING::npos )
		{
			// Extract the Key
			NSSTRING strKey = strLine.substr( 0, delimPos );
			strLine.replace( 0, delimPos+skip, NSDQUOATE );
			
			// See if Value continues on the next Line
			// Stop at blank Line, next Line with a Key, end of stream,
			// or end of file Sentry
			bool terminate = false;
			while( !terminate && is )
			{
				std::getline(is, strNextLine );
				terminate = true;
				
				NSSTRING strNlcopy = strNextLine;
				ConfigFile::trim(strNlcopy);
				if( strNlcopy == NSDQUOATE ) continue;
				
				strNextLine = strNextLine.substr( 0, strNextLine.find(strComm) );
				if( strNextLine.find(strDelim) != NSSTRING::npos )
					continue;
				if( strSentry != NSDQUOATE && strNextLine.find(strSentry) != NSSTRING::npos )
					continue;
				
				strNlcopy = strNextLine;
				ConfigFile::trim(strNlcopy);
				if( strNlcopy != NSDQUOATE )
				{
					#ifdef _UNICODE
						strLine += _T("\n");
					#else
						strLine += "\n";
					#endif
				}
				strLine += strNextLine;
				terminate = false;
			}
			
			// Store Key and Value
			ConfigFile::trim(strKey);
			ConfigFile::trim(strLine);
			m_mapContents[strKey] = strLine;  // overwrites if Key is repeated
		}
	}
	return is;
}

/*!
 @Brief			Get the Value corresponding to Key and store in var
				Return true if Key is found
				Otherwise leave var untouched
 @Param [in]	TCHAR*	- key
 @Param [out]	TCHAR[]	- value
 @Return		bool	- Return true if Key is found	
 @Date			13 June 2007
*/
bool ConfigFile::readString( TCHAR szValue[],  TCHAR* pszKey )
{
	NSSTRING strTest;
	m_MapConstIterator ContentItr = m_mapContents.find(pszKey);
	bool found = ( ContentItr != m_mapContents.end() );
	if( found )
	{
		strTest = ( ContentItr->second );
		#ifdef _INTERNAL_
			//NSSTRCPY(szValue, (TCHAR*)strTest.c_str());
			NSSTRCPY(szValue, strTest.size()+1, (TCHAR*)strTest.c_str());
		#else
			NSSTRCPY(szValue, strTest.size()+1, (TCHAR*)strTest.c_str());
		#endif
	}
	return found;
}


/* static */
/*!
 @Brief			Convert from a T to a NSSTRING
				Type T must support << operator
 @Param [in]	T& 
 @Return		NSSTRING	
 @Date			13 June 2007
*/
template<class T>
NSSTRING ConfigFile::T_as_string( const T& t )
{

	// Type T must support << operator
	NSOSTRINGSTREAM ost;
	ost << t;
	return ost.str();
}


/* static */
/*!
 @Brief			Convert from a NSSTRING to a T
				Type T must support >> operator
 @Param [in]	NSSTRING& 
 @Return		T	
 @Date			13 June 2007
*/
template<class T>
T ConfigFile::string_as_T( const NSSTRING& s )
{
	//Type T must support >> operator
	T t;
	NSISTRINGSTREAM ist(s);
	ist >> t;
	return t;
}


/* static */
/*!
 @Brief			Convert from a NSSTRING to a NSSTRING
 @Param [in]	NSSTRING& 
 @Return		NSSTRING	
 @Date			13 June 2007
*/

#ifndef _INTERNAL_

	template<>
	NSSTRING ConfigFile::string_as_T<NSSTRING>( const NSSTRING& s )
	{
		return s;
	}
#endif

/* static */
/*!
 @Brief			Convert from a NSSTRING to a bool
				Interpret "false", "F", "no", "n", "0" as false
				Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
 @Param [in]	NSSTRING&
 @Return		bool
 @Date			13 June 2007
*/

#ifndef _INTERNAL_

	template<>
	bool ConfigFile::string_as_T<bool>( const NSSTRING& s )
	{

		bool bTerminate = true;
		NSSTRING strSup = s;
		for( NSSTRING::iterator p = strSup.begin(); p != strSup.end(); ++p )
			*p = toupper(*p);  // make NSSTRING all caps
	#if _UNICODE
		if( strSup==NSSTRING(_T("FALSE")) || strSup==NSSTRING(_T("F")) ||
			strSup==NSSTRING(_T("NO")) || strSup==NSSTRING(_T("N")) ||
			strSup==NSSTRING(_T("0")) || strSup==NSSTRING(_T("NONE")) )
	#else
		if( strSup==NSSTRING("FALSE") || strSup==NSSTRING("F") ||
			strSup==NSSTRING("NO") || strSup==NSSTRING("N") ||
			strSup==NSSTRING("0") || strSup==NSSTRING("NONE") )
	#endif
			bTerminate = false;
		return bTerminate;
	}
#endif


/*!
 @Brief			reads a value from the map
 @Param [in]	NSSTRING& 	key
 @Return		T			value
 @Date			13 June 2007
*/
template<class T>
T ConfigFile::readValue( const NSSTRING& strKey ) const
{
	// Read the Value corresponding to Key
	m_MapConstIterator ContentItr = m_mapContents.find(strKey);
	if( ContentItr == m_mapContents.end() ) throw KeyNotFound(strKey);
	return string_as_T<T>( ContentItr->second );
}



/*!
 @Brief			reads a value from the map
 @Param [in]	NSSTRING& 	key
 @Param [in]	T&			value
 @Return		T			value
 @Date			13 June 2007
*/
template<class T>
T ConfigFile::readValue( const NSSTRING& strKey, const T& Value ) const
{
	// Return the Value corresponding to Key or given default szValue
	// if Key is not found
	m_MapConstIterator ContentItr = m_mapContents.find(strKey);
	if( ContentItr == m_mapContents.end() ) return Value;
	return string_as_T<T>( ContentItr->second );
}


/*!
 @Brief			reads a value from the map
 @Param [in]	NSSTRING&	key
 @Param [out]	T&			value
 @Return		bool		true if key is found
 @Date			13 June 2007
*/
template<class T>
bool ConfigFile::readInto( T& var, const NSSTRING& strKey ) const
{
	// Get the Value corresponding to Key and store in var
	// Return true if Key is found
	// Otherwise leave var untouched
	m_MapConstIterator ContentItr = m_mapContents.find(strKey);
	bool found = ( ContentItr != m_mapContents.end() );
	if( found ) var = string_as_T<T>( ContentItr->second );
	return found;
}


/*!
 @Brief			reads a value from the map
 @Param [in]	NSSTRING&	key
 @Param [in]	T&			value
 @Param [out]	T&			searched or default value
 @Return		bool		true if key is found
 @Date			13 June 2007
*/
template<class T>
bool ConfigFile::readInto( T& var, const NSSTRING& strKey, const T& Value ) const
{
	// Get the Value corresponding to Key and store in var
	// Return true if Key is found
	// Otherwise set var to given default
	m_MapConstIterator ContentItr = m_mapContents.find(strKey);
	bool found = ( ContentItr != m_mapContents.end() );
	if( found )
		var = string_as_T<T>( ContentItr->second );
	else
		var = Value;
	return found;
}

/*!
 @Brief			adds a key-value pair to the map
 @Param [in]	NSSTRING key
 @Param [in]	T& value
 @Date			13 June 2007
*/
template<class T>
void ConfigFile::add( NSSTRING strKey, const T& Value )
{
	// Add a Key with given Value
	NSSTRING szVal = T_as_string( Value );
	trim(strKey);
	trim(Value);
	m_mapContents[strKey] = Value;
	return;
}

void ConfigFile::add( NSSTRING strKey, NSSTRING strValue)
{
	// Add a Key with given Value
	trim(strKey);
	trim(strValue);
	m_mapContents[strKey] = strValue;
	return;
}

void ConfigFile::updateURLMap(std::map<NSSTRING,NSSTRING> & m_mapofURL, TCHAR* szSearchString)
{
	std::map<NSSTRING,NSSTRING>::iterator itr;
	for(itr = m_mapContents.begin(); itr != m_mapContents.end(); itr++)
	{
		NSSTRING strKey = (*itr).first;
		NSSTRING strValue = (*itr).second;
		int nIndex = (int)strKey.find(szSearchString);
		if(nIndex != -1 && nIndex == 0)
		{
			m_mapofURL.insert(std::map<NSSTRING,NSSTRING>::value_type(strKey, strValue));
		}
	}
}

/*!
 @Brief			Updates validation rule error messages to .ini file.
 @Param [in]	NSSTRING dlgErrorMsgs - status of error messages from GISCADLoader Dialog
                NSSTRING fileErrorMsgs - statue of error messages from .ini file
 @Param [in]	
 @Date			
*/

/*bool ConfigFile::UpdateErrorMsgToFile(const NSSTRING dlgErrorMsgs, const NSSTRING fileErrorMsgs)
{
	//NSIFSTREAM inputFile(m_strFilename,std::ios::in | std::ios::binary);
	//std::wofstream outFile(m_strFilename + "_New",std::ios::out | std::ios::binary);
		
	NSIFSTREAM inputFile(m_strFilename,std::ios::in | std::ios::binary);
	inputFile.seekg(0,std::ios::end);
	long size = inputFile.tellg();
	wchar_t *buffer=new wchar_t[size];
	inputFile.seekg(0,std::ios::beg);
	inputFile.read(buffer,size);
	inputFile.close();
	NSSTRING txtStr(buffer,size);
	delete[] buffer;
	size_t off=0;
	TCHAR* pszNextDlgToken; 
	TCHAR* pszNextFileToken;
	TCHAR* strDlgError = NSSTRTOK((TCHAR*)dlgErrorMsgs.c_str(), _T(","), &pszNextDlgToken);
	TCHAR* strFileError = NSSTRTOK((TCHAR*)fileErrorMsgs.c_str(), _T(","), &pszNextFileToken);
	while(strDlgError != NULL)
	{
		off=0;
		if(NSSTRCMP(strFileError,strDlgError) == 0)
		{
			strDlgError = NSSTRTOK( NULL,_T(","), &pszNextDlgToken);
			strFileError = NSSTRTOK( NULL,_T(","), &pszNextFileToken);
			continue;
		}
		while ((off=txtStr.find(strFileError,off))!= std::string::npos)
		{
			txtStr.replace(off,NSSTRLEN(strFileError),strDlgError);
				
		}
		strDlgError = NSSTRTOK( NULL,_T(","), &pszNextDlgToken);
		strFileError = NSSTRTOK( NULL,_T(","), &pszNextFileToken);
	}
	std::wofstream outFile(m_strFilename,std::ios::out | std::ios::binary);
	outFile.write(txtStr.c_str(),size);
	txtStr.clear();
	outFile.close();
	return 0;
	

}*/

/*
Method - To update the Settings.ini if Error Flags are Updated from 
		 Dialog checkbox.
Purpose - New Error Flags values are updated in Settings.ini
*/

bool ConfigFile::UpdateErrorMsgToFile(const NSSTRING dlgErrorMsgs, const NSSTRING fileErrorMsgs)
{
	
	const char *errFlg = new char[12];
	errFlg = "[ERRORFLAG]";

	std::wstring m_strFilenameNew = m_strFilename;
	int sizeNewFile = m_strFilenameNew.length();
	m_strFilenameNew = m_strFilenameNew.substr(0,(sizeNewFile -4));
	m_strFilenameNew = m_strFilenameNew.append(_T("_NEW.ini"));
	
	std::ifstream inputFile(m_strFilename,std::ios::in );
	std::ofstream outFile(m_strFilenameNew.c_str(),std::ios::out | std::ios::binary);

	inputFile.seekg(0,std::ios::end);
	long size = inputFile.tellg();
	TCHAR* pszNextDlgToken;
	int sizeLineBuffer;
	std::string linebuffer;
	inputFile.seekg(0,std::ios::beg);
	
	while(!inputFile.eof())       // Read the file 
	{

		getline(inputFile,linebuffer);
				
		if(strcmp(linebuffer.c_str(),errFlg) == 0)
		{
			outFile << linebuffer << "\r\n"; 

			getline(inputFile,linebuffer);
			TCHAR* strDlgError = NSSTRTOK((TCHAR*)dlgErrorMsgs.c_str(), _T(","), &pszNextDlgToken);
			
			
			for(int i=0;i<=3;i++)
			{
			wchar_t *strErrorFlags=strDlgError;
			std::wstring strwError(strErrorFlags);
			std::string strError(CW2A(strwError.c_str()));
			outFile << strError << "\r\n";
			getline(inputFile,linebuffer);
			strDlgError = NSSTRTOK( NULL,_T(","), &pszNextDlgToken);
			}
		}
		else 
		{
			outFile << linebuffer << "\r\n"; 
		}
	
	}

	inputFile.close();
	outFile.close();


	DeleteFile(m_strFilename);
	MoveFile(m_strFilenameNew.c_str(),m_strFilename);
	
	return 0;
	

}