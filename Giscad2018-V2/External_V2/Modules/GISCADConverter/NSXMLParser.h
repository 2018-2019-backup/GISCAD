/*!
 @Class		:	CNSXMLParser
 @Brief		:	Main XML Parser class, uses MSXML DOM component.
				Provides methos to Load/Save and Open XML file
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include "NSTElement.h"

#define ERR_XML_LOADING -101
class CNSXMLParser
{
public:
	CNSXMLParser();
	~CNSXMLParser();
	
	bool loadFile(NSSTRING strFileName);
	bool validate(const NSSTRING strFileName);
	bool createFile();
	bool save(const NSSTRING strFileName);
	CNSTElement* getRootElement();
	void createRootElement(const NSSTRING strFileName, const NSSTRING strNamespaceURI);
private:
	MSXML2::IXMLDOMDocument2 *m_pXMLDoc;
};
