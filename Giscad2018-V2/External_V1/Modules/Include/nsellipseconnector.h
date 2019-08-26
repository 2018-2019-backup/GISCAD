#pragma once
#pragma warning(disable: 4251)
#include "..\Include\NSDirectives.h"
#include "..\Include\nsdataitem.h"
#include "vector"
#include "map"
#import "msxml4.dll"
#import "C:\Program Files (x86)\Common Files\MSSoap\Binaries\MSSOAP30.dll" \
	exclude("IStream", "ISequentialStream", "_LARGE_INTEGER", \
	"_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")
using namespace MSSOAPLib30;
using namespace std;


class __declspec(dllexport)CNSEllipseConnector
{
public:
	CNSEllipseConnector(void);
	~CNSEllipseConnector(void);
public:
	int connect(const TCHAR* strEndPointURL);
	int beginSOAPMessage(const TCHAR* strSOAPAction);
	int endSOAPMessage();
	int buildCredentials(std::map<TCHAR*, TCHAR*> &credentialsVect);
	int startService(const TCHAR* strService);
	int startRequest(const TCHAR* strMessageID, const TCHAR* strVersion, const TCHAR* strNS, const TCHAR* strSchemaLoc);
	int endElement();
	int buildReply(std::vector<TCHAR*> &replyVect);
	int buildRequest(std::map<TCHAR*, TCHAR*> &mapOfRequest, const TCHAR* strRequestID, bool bMaxInst = false);
	//TCHAR* getResult(std::vector<CNSDataItem*> & dataItemsVect, std::vector<TCHAR*> &strErrorTexts);
	TCHAR* getResult(std::vector<CNSDataItem*> & dataItemsVect, std::vector<TCHAR*> &strErrorTexts);
	void release();

	#ifdef _DEBUG
	    NSSTRING debugxml;
	#endif // _DEBUG
	
private:
	ISoapConnectorPtr m_ptrConnector;
	ISoapSerializerPtr m_ptrSerializer;
	ISoapReaderPtr m_ptrReader;
	

};
