#include "StdAfx.h"
#include "NSTElement.h"

/*!
@Brief			Function internally used to get the current element in a list information
@Param [in]		none
@Return			none
@Date			06-06-2007
*/
void CNSTElement::get()
{ 
	if (m_nPos!=-1)
	{
		m_pElement=0;
		CComPtr<MSXML2::IXMLDOMNode> pNode;
		m_pList->get_item(m_nPos,&pNode);
		if (pNode==0) return;
		MSXML2::DOMNodeType type; pNode->get_nodeType(&type);
		if (type!=MSXML2::NODE_ELEMENT) return;
		CComQIPtr<MSXML2::IXMLDOMElement> pElement(pNode);
		m_pElement=pElement;
	}
	m_lLen=0; if (m_pElement!=0)
	{ CComPtr<MSXML2::IXMLDOMNodeList> pNodeList;
	m_pElement->get_childNodes(&pNodeList);
	pNodeList->get_length(&m_lLen);  
	}
}

/*!
@Brief			name()    - in <item>stuff</item> it returns "item"
@Param [in]		none
@Return			name of the element
@Date			06-06-2007
*/
NSSTRING CNSTElement::name() const
{ 
	if (!m_pElement) return _T("");
	CComBSTR bstrName; m_pElement->get_tagName(&bstrName);
	return NSSTRING(bstrName);
}

/*!
@Brief			attr(s)   - in <item s=L"hello">stuff</item> it returns "hello"
@Param [in]		name of the attribute to be looked for
@Return			attribute value
@Date			06-06-2007
*/
NSSTRING CNSTElement::attr(const NSSTRING strName) const
{ 
	if (!m_pElement) return _T("");
	CComBSTR bstrName(strName.c_str());
	CComVariant val(VT_EMPTY);
	val = m_pElement->getAttribute((_bstr_t)bstrName);
	if (val.vt==VT_BSTR) return val.bstrVal;
	return _T("");
}

/*!
@Brief			retrives boolean values of attribute 
@Param [in]		name of the attribute
@Return			true or false.
@Date			06-06-2007
*/
bool CNSTElement::attrBool(const NSSTRING strName,bool bDef) const
{
	NSSTRING strAttr = attr(strName);
	if (strAttr==_T("true") || strAttr==_T("TRUE")) return true;
	else if (strAttr==_T("false") || strAttr==_T("FALSE")) return false;
	else return bDef;
}

/*!
@Brief			retrives integer values of attribute 
@Param [in]		name of the attribute, default value
@Return			integer value of string
@Date			06-06-2007
*/
int CNSTElement::attrInt(const NSSTRING strName, int nDef) const
{ 
	NSSTRING strAttr = attr(strName);
	int n, nRes=NSSSCANF(strAttr.c_str(),_T("%i"),&n);
	if (nRes==1) return n; else return nDef;
}

/*!
@Brief			val()     - in <item>stuff</item> it returns "stuff"
@Param [in]		none
@Return			value of the element
@Date			06-06-2007
*/
NSSTRING CNSTElement::val() const
{ 
	if (!m_pElement) return _T("");
	CComVariant val(VT_EMPTY);
	m_pElement->get_nodeTypedValue(&val);
	if (val.vt==VT_BSTR) return val.bstrVal;
	return _T("");
}

/*!
@Brief			subnode(b)- in <item><a>hello</a><b>there</b></item> it returns the CNSTElement <b>there</b>
@Param [in]		node to be looked for
@Return			node element
@Date			12-06-2007
*/
CNSTElement CNSTElement::subnode(const NSSTRING strName) const
{ 
	if (!m_pElement) return CNSTElement();
	for (CNSTElement cElement=begin(); cElement!=end(); cElement++)
	{ if (cElement.name()==strName) return cElement;
	}
	return CNSTElement();
}

/*!
@Brief			subval(b) - in <item><a>hello</a><b>there</b></item> it returns "there"
@Param [in]		name of the sub element
@Return			value of the subelement
@Date			06-06-2007
*/
NSSTRING CNSTElement::subval(const NSSTRING strName) const
{
	if (!m_pElement) return _T("");
	CNSTElement cElement =subnode(strName);
	return cElement.val();
}

/*!
@Brief			This formats the SQL query in specila cases like SUBSTATION_TYPE etc.
@Param [in]		mapAttributes			:	map of attributes
				szQuery					:   SQL query.
@Return			formatted SQL query.
@Date			12-06-2007
*/
CNSTElement CNSTElement::begin() const
{ 
	if (!m_pElement) return CNSTElement();
	MSXML2::IXMLDOMNodeList* pNodeList;
	m_pElement->get_childNodes(&pNodeList);
	return CNSTElement(pNodeList);
}

/*!
@Brief			for (CNSTElement c=e.begin(); c!=e.end(); c++) {...} - iterators over the subnodes
@Return			iterator
@Date			06-06-2007
*/
CNSTElement CNSTElement::end() const
{ 
	return CNSTElement(m_lLen);
}

CNSTElement CNSTElement::operator++(int)
{ 
	if (m_nPos!=-1) {m_nPos++; get();}
	return *this;
}

bool CNSTElement::operator!=(const CNSTElement &Element) const
{ 
	return m_nPos!=Element.m_lLen;
}

/*!
@Brief			retrives element at position specified by index in a list
@Param [in]		index
@Return			element at position specified
@Date			06-06-2007
*/
CNSTElement CNSTElement::getAt(int nIndex) 
{
	if(m_pList)
	{
		CComPtr<MSXML2::IXMLDOMNode> pNode;
		CComPtr<MSXML2::IXMLDOMNodeList> pNodeList;
		m_pList->get_item(nIndex, &pNode);
		if(pNode == 0) return NULL;
		MSXML2::DOMNodeType type; 
		pNode->get_nodeType(&type);
		if(type!= MSXML2::NODE_ELEMENT) return NULL;
		CComQIPtr<MSXML2::IXMLDOMElement> pElem(pNode);
		return CNSTElement(pElem);
	}
	return CNSTElement();

}

/*!
@Brief			returns length of sublist
@Param [in]		none
@Return			int- length
@Date			06-06-2007
*/
int CNSTElement::getSubListLength() const
{
	long lLen = 0;
	if(m_pList)
	{
		m_pList->get_length(&lLen); 	
	}
	return lLen;
}

/*!
@Brief			NOT USED
@Param [in]		Node name
@Return			
@Date			12-06-2007
*/
CNSTElement CNSTElement::selectParentNode(const NSSTRING pszName) 
{
	if(!m_pElement) return CNSTElement();
	CComPtr<MSXML2::IXMLDOMNodeList> iNodeList;
	m_lLen = 0; 
	iNodeList = m_pElement->selectNodes(pszName.c_str());
	long lLen = 0;
	iNodeList->get_length(&lLen); 
	MSXML2::IXMLDOMNodePtr inode;
	if(lLen == 1)  
	{

		iNodeList->get_item(0,&inode);
		MSXML2::DOMNodeType type; 
		inode->get_nodeType(&type);
		if(type!= MSXML2::NODE_ELEMENT) return CNSTElement();

		CComQIPtr<MSXML2::IXMLDOMElement> pElem(inode);
		return CNSTElement(pElem);
	}
	return CNSTElement();
}

/*!
@Brief			NOT USED
@Param [in]		Node name
@Return			
@Date			12-06-2007
*/
CNSTElement CNSTElement::subList(const NSSTRING pszName) const
{
	if(!m_pElement) return CNSTElement();
	CComPtr<MSXML2::IXMLDOMNodeList>  pNodeList;
	CComPtr<MSXML2::IXMLDOMNode> pNode;
	pNodeList = m_pElement->selectNodes(pszName.c_str());
	long len = 0;
	pNodeList->get_length(&len);
	if(len > 0)
	{
		return CNSTElement(pNodeList);
	}
	else
		return CNSTElement();
}

/*!
@Brief			Write to XML file, createlement methods creates element in DOM model
@Param [in]		name of the node
@Return			DOM element.
@Date			06-06-2007
*/
CComPtr<MSXML2::IXMLDOMElement> CNSTElement::createElement(const NSSTRING strNodeName)
{
	if (!m_pElement) return NULL;
	MSXML2::IXMLDOMDocument2Ptr  pDoc = 0;
	pDoc = m_pElement->GetownerDocument();
	return CComPtr<MSXML2::IXMLDOMElement>(pDoc->createElement(strNodeName.c_str()));
}

/*!
@Brief			creates a node in the DOM Model
@Param [in]		name of the node, value and namespaceURI
@Return			created element 
@Date			06-06-2007
*/
CNSTElement CNSTElement::createNode(const NSSTRING strNodeName, const NSSTRING strVal, const NSSTRING strNameSpaceURI)
{
	if (!m_pElement) return NULL;
	MSXML2::IXMLDOMDocument2Ptr  pDoc = 0;
	pDoc = m_pElement->GetownerDocument();
	_variant_t varNodeType((short)MSXML2::NODE_ELEMENT);
	MSXML2::IXMLDOMNodePtr pNode; 
	pNode = pDoc->createNode(varNodeType,strNodeName.c_str(), strNameSpaceURI.c_str());
	pNode->nodeTypedValue = strVal.c_str();
	m_pElement->appendChild(pNode);
	CComQIPtr<MSXML2::IXMLDOMElement> pElem(pNode);
	return CNSTElement(pElem);
}

/*!
@Brief			Write to XML file, createlement methods creates element in DOM model, namespace not specified
@Param [in]		name of the node, value 
@Return			created element 
@Date			06-06-2007
*/
CNSTElement CNSTElement::createNode(const NSSTRING strNodeName, const NSSTRING strVal)
{
	CComPtr<MSXML2::IXMLDOMElement> pTempElem = NULL;
	pTempElem = createElement(strNodeName);
	if(pTempElem == NULL) return CNSTElement();
	pTempElem->nodeTypedValue = strVal.c_str();
	m_pElement->appendChild(pTempElem);
	return CNSTElement(pTempElem);
}

/*!
@Brief			set value of this node
@Param [in]		value of the node
@Return			none
@Date			12-06-2007
*/
void CNSTElement::setNodeValue(const NSSTRING strVal)
{
	m_pElement->nodeTypedValue = strVal.c_str();
}

/*!
@Brief			creates a node in the DOM Model
@Param [in]		name of the node, value and namespaceURI
@Return			created element 
@Date			06-06-2007
*/
CNSTElement CNSTElement::createAttribute(const NSSTRING strElementName, const NSSTRING strNodeName, const NSSTRING strVal)
{
	CComPtr<MSXML2::IXMLDOMElement> pTempElem = NULL;
	pTempElem = createElement(strElementName);
	if(pTempElem == NULL) return CNSTElement();
	pTempElem->setAttribute(strNodeName.c_str(), strVal.c_str());
	m_pElement->appendChild(pTempElem);
	return CNSTElement(pTempElem);
}

/*!
@Brief			set the attribute name and value
@Param [in]		name of the node, value 
@Return			none 
@Date			06-06-2007
*/
void CNSTElement::setAttribute(const NSSTRING strNodeName, const NSSTRING strVal)
{
	CComPtr<MSXML2::IXMLDOMElement> pTempElem = NULL;
	if (!m_pElement) return;
	m_pElement->setAttribute(strNodeName.c_str(), strVal.c_str());
}

int CNSTElement::getPosition()
{
	return m_nPos;
}

long CNSTElement::getLength()
{
	return m_lLen;
}
