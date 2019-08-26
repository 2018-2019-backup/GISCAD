/*!
 @Class		:	CNSTElement
 @Brief		:	This template class is responsible for holdin the information Read from XML and Write to XML
				from DOMDocument component.
 @Author	:	Neilsoft Ltd
 @Date		:	12-07-2007
 @History 	:	Change history
 */
#pragma once
#include <windows.h>
//#include <msxml.h>
#import <msxml6.dll> named_guids  
using namespace MSXML2;
#include <objsafe.h>
#include <objbase.h>
#include <atlbase.h>
#pragma warning( push )
#pragma warning( disable: 4018 4786)
#include <string>
#pragma warning( pop )
using namespace std;
#include "..\..\Include\NSDirectives.h"

/*
wchar_t *src=L""
  L"<?xml version=\"1.0\" encoding=\"utf-16\"?>\r\n"
  L"<root desc=\"Simple Prog\">\r\n"
  L"  <text>Hello World</text>\r\n"
  L"    <layouts>\r\n"
  L"    <lay pos=\"15\" bold=\"true\"/>\r\n"
  L"    <layoff pos=\"12\"/>\r\n"
  L"    <layin pos=\"17\"/>\r\n"
  L"  </layouts>\r\n"
  L"</root>\r\n";
*/

// CNSTElement -- a simple class to wrap up IXMLDomElement and iterate its children.
//   name()    - in <item>stuff</item> it returns "item"
//   val()     - in <item>stuff</item> it returns "stuff"
//   attr(s)   - in <item s=L"hello">stuff</item> it returns "hello"
//   subnode(b)- in <item><a>hello</a><b>there</b></item> it returns the CNSTElement <b>there</b>
//   subval(b) - in <item><a>hello</a><b>there</b></item> it returns "there"
//   for (CNSTElement c=e.begin(); c!=e.end(); c++) {...} - iterators over the subnodes
 

class CNSTElement
{
public:
	~CNSTElement(){;};
	//
	CNSTElement() : m_pElement(0), m_pList(0), m_nPos(-1), m_lLen(0) {}
	CNSTElement(int _clen) : m_pElement(0), m_pList(0), m_nPos(-1), m_lLen(_clen) {}
	CNSTElement(CComPtr<MSXML2::IXMLDOMElement> pElement) : m_pElement(pElement), m_pList(0), m_nPos(-1), m_lLen(0) {get();}
	CNSTElement(CComPtr<MSXML2::IXMLDOMNodeList> pList) : m_pElement(0), m_pList(pList), m_nPos(0), m_lLen(0) {get();}
	
	NSSTRING name() const;
	NSSTRING attr(const NSSTRING strName) const;
	bool attrBool(const NSSTRING strName,bool bDef) const;
	int attrInt(const NSSTRING strName, int nDef) const;
	NSSTRING val() const;
	CNSTElement subnode(const NSSTRING strName) const;
	CNSTElement subList(const NSSTRING strName) const;
	int getSubListLength() const;

	NSSTRING subval(const NSSTRING strName) const;
	CNSTElement createNode(const NSSTRING strNodeName, const NSSTRING strVal, const NSSTRING strNameSpaceURI);
	CNSTElement createNode(const NSSTRING strNodeName, const NSSTRING strVal);
	CNSTElement createAttribute(const NSSTRING strElementName, const NSSTRING strNodeName, const NSSTRING strVal);
	CComPtr<MSXML2::IXMLDOMElement> createElement(const NSSTRING strNodeName);
	void setAttribute(const NSSTRING strNodeName, const NSSTRING strVal);
	bool addSubnode(const CNSTElement &e) const;
	CNSTElement begin() const;
	CNSTElement end() const;
	CNSTElement operator++(int);
	bool operator!=(const CNSTElement &Element) const;
	CNSTElement getAt(int nIndex) ;
	CNSTElement selectParentNode(const NSSTRING pszName) ;
	int getPosition();
	long getLength();
	void setNodeValue(const NSSTRING strVal);
private:
	//Methods
	void get();

	//Member variables
	CComPtr<MSXML2::IXMLDOMElement> m_pElement;
	CComPtr<MSXML2::IXMLDOMNodeList> m_pList; 
	int m_nPos; 
	long m_lLen;
};