#include "StdAfx.h"
#include "../include/nsellipseconnector.h"

// Constructor
CNSEllipseConnector::CNSEllipseConnector(void)
{
	m_ptrConnector = NULL;
	m_ptrSerializer = NULL;
	m_ptrReader = NULL;

	#ifdef _DEBUG
	   debugxml = _T("");
	#endif // _DEBUG

}
// Destructor
CNSEllipseConnector::~CNSEllipseConnector(void)
{

}

/*!
@Brief			This function prepares HttpConnector object and intialzing the
SOAP connector object.
@Param [in]		strEndPointURL	End point URL.
@Return			NS_SUCCESS	-	if SOAP Connector intializing properly else return NS_FAIL
@Date			31-08-2007
*/
int CNSEllipseConnector::connect(const TCHAR* strEndPointURL)
{
	try
	{
		HRESULT hr;
		hr = m_ptrConnector.CreateInstance(__uuidof(HttpConnector30));
		if(hr != S_OK)
		{
			// Problem while creating HTTPConnector instance.
			return NS_FAIL;
		}
		m_ptrConnector->Property["EndPointURL"] = strEndPointURL;
		//Invalid end point url;
		hr = m_ptrConnector->Connect();
		if(hr != S_OK)
		{
			// Problem while intializing the SoapConnector object and prepares the connection
			//
			return NS_FAIL;
		}
		return NS_SUCCESS;
	}
	catch(...)
	{
		MessageBox(NULL, _T("Exception in connect member function"), _T("Exception"), MB_OK);
		return NS_FAIL;
	}

}

/*!
@Brief			This function prepares to start SOAP message being sent to Server.
@Param [in]		strSOAPAction	SOAP Action.
@Return			
@Date			31-08-2007
*/
int CNSEllipseConnector::beginSOAPMessage(const TCHAR* strSOAPAction)
{
	try
	{
		HRESULT hr;
		// Begin Message
		m_ptrConnector->Property["SoapAction"] = strSOAPAction;
		hr = m_ptrConnector->BeginMessage();
		if(hr != S_OK)
		{
			// Problem while start SOAP message being sent to the server.
			return NS_FAIL;
		}
		// Create SOAP Serializer
		hr = m_ptrSerializer.CreateInstance(__uuidof(SoapSerializer30));
		if(hr != S_OK)
		{
			// Problem while creating instace of SoapSerializer
			return NS_FAIL;
		}
		// Connect the serializer to the input stream of the connector
		hr = m_ptrSerializer->Init(_variant_t((IUnknown*)m_ptrConnector->InputStream));
		if(hr != S_OK)
		{
			// Problem while intializing serializer object.
			return NS_FAIL;
		}
		//Build the SOAP message
		hr = m_ptrSerializer->StartEnvelope("", "", "");
		if(hr != S_OK)
		{
			// Failed to start Envelope
			return NS_FAIL;
		}
		hr = m_ptrSerializer->StartBody("");
		if(hr != S_OK)
		{
			// Failed to start Body
			return NS_FAIL;
		}
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}

}


int CNSEllipseConnector::endSOAPMessage()
{


	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->EndBody();
		if(hr != S_OK)
		{
			return NS_FAIL;
		}
		hr = m_ptrSerializer->EndEnvelope();
		if(hr != S_OK)
		{
			return NS_FAIL;
		}

		hr = m_ptrConnector->EndMessage();
		if(hr != S_OK)
		{
			//Failed to send message to Server.
			return NS_FAIL;
		}

		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}

}

int CNSEllipseConnector::startRequest(const TCHAR* strMessageID, const TCHAR* strVersion, const TCHAR* strNS, const TCHAR* strSchemaLoc)
{
	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->StartElement("EllipseRequest", "", "NONE", "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("messageId", "", strMessageID, "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("version", "", strVersion, "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("xmlns:xsi", "", strNS, "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("xsi:noNamespaceSchemaLocation", "", strSchemaLoc, "");
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}

}

int CNSEllipseConnector::startService(const TCHAR* strService)
{
	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->StartElement("Service", "", "NONE", "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("name", "", strService, "");
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}
}

int CNSEllipseConnector::endElement()
{
	try
	{
		HRESULT hr = m_ptrSerializer->EndElement();
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}
}

int CNSEllipseConnector::buildCredentials(std::map<TCHAR*, TCHAR*> &credentialsVect)
{
	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->StartElement("Credentials", "", "NONE", "");
		if(hr != S_OK)
			return NS_FAIL;
		std::map<TCHAR*, TCHAR*>::iterator it;
		for(it = credentialsVect.begin(); it != credentialsVect.end(); it++)
		{
			hr = m_ptrSerializer->StartElement(((*it).first), "", "NONE", "");
			if(hr != S_OK)
				return NS_FAIL;
			hr = m_ptrSerializer->WriteString(((*it).second));
			if(hr != S_OK)
				return NS_FAIL;
			hr = m_ptrSerializer->EndElement();
			if(hr != S_OK)
				return NS_FAIL;
		}
		hr = m_ptrSerializer->EndElement();
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}

}

int CNSEllipseConnector::buildReply(std::vector<TCHAR*> &replyVect)
{
	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->StartElement("ReplyParameters", "", "NONE", "");
		if(hr != S_OK)
			return NS_FAIL;
		std::vector<TCHAR*>::iterator it;
		for(it = replyVect.begin(); it != replyVect.end(); it++)
		{
			hr = m_ptrSerializer->StartElement("Parameter", "", "NONE", "");
			if(hr != S_OK)
				return NS_FAIL;
			hr = m_ptrSerializer->SoapAttribute("name", "", (*it), "");
			if(hr != S_OK)
				return NS_FAIL;
			hr = m_ptrSerializer->EndElement();
			if(hr != S_OK)
				return NS_FAIL;
		}
		hr = m_ptrSerializer->EndElement();
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}
}

int CNSEllipseConnector::buildRequest(std::map<TCHAR*, TCHAR*> &mapOfRequest, const TCHAR* strRequestID, bool bMaxInst)
{
	try
	{
		HRESULT hr;
		hr = m_ptrSerializer->StartElement("Request", "", "NONE", "");
		if(hr != S_OK)
			return NS_FAIL;
		hr = m_ptrSerializer->SoapAttribute("id", "", strRequestID, "");
		if(hr != S_OK)
			return NS_FAIL;
		if(bMaxInst == true)
		{
			hr = m_ptrSerializer->SoapAttribute("MaxInst", "", "128", "");
			if(hr != S_OK)
				return NS_FAIL;
		}
		std::map<TCHAR*, TCHAR*>::iterator it;
		for(it = mapOfRequest.begin(); it != mapOfRequest.end(); it++)
		{
			hr = m_ptrSerializer->StartElement("Parameter", "", "NONE", "");
			if(hr != S_OK)
				return NS_FAIL;
			TCHAR* szAttribute = ((*it).first);
			hr = m_ptrSerializer->SoapAttribute("name", "", szAttribute, "");
			if(hr != S_OK)
				return NS_FAIL;
			TCHAR* szValue = (*it).second;
			hr = m_ptrSerializer->WriteString(szValue);
			if(hr != S_OK)
				return NS_FAIL;
			hr = m_ptrSerializer->EndElement();
			if(hr != S_OK)
				return NS_FAIL;
		}
		hr = m_ptrSerializer->EndElement();
		if(hr != S_OK)
			return NS_FAIL;
		return NS_SUCCESS;
	}
	catch(...)
	{
		return NS_FAIL;
	}
}

TCHAR* CNSEllipseConnector::getResult(std::vector<CNSDataItem*> & dataItemsVect, std::vector<TCHAR*> &strErrorTexts)
{
	BSTR namespaceuri = ::SysAllocString(L"");
	try
	{
		HRESULT hr = m_ptrReader.CreateInstance(__uuidof(SoapReader30));
		if(hr != S_OK)
		{
			return _T("");
		}

		VARIANT_BOOL bRes;

		bRes = m_ptrReader->Load(_variant_t((IUnknown*)m_ptrConnector->OutputStream), "");

		#ifdef _DEBUG
			bRes = m_ptrReader->Load(debugxml.c_str(), "");
		#endif // _DEBUG

		if(VARIANT_FALSE == bRes)
		{
			return _T("");
		}
		MSXML2::IXMLDOMElement *pElement = NULL;
		// EllipseReply

		//Open this code for testing XML load
		//MSXML2::IXMLDOMDocumentPtr pDoc= m_ptrReader->GetDOM();
		//MSXML2::IXMLDOMElementPtr pRPCStruct = pDoc->GetdocumentElement();//m_ptrReader->GetRPCStruct();
		//End of Open this code for testing XML load

		//Actual load of resultant response
		MSXML2::IXMLDOMElementPtr pRPCStruct = m_ptrReader->GetRpcStruct();
		//End of Actual load of resultant response

		MSXML2::IXMLDOMNodeListPtr pNodeList;
		MSXML2::IXMLDOMNodePtr pNode;
		if(pRPCStruct != NULL)
		{
			// EllipseReply childs i.e. ServiceResponses
			MSXML2::IXMLDOMNodePtr ptrServiceResponsesNode = pRPCStruct->selectSingleNode("ServiceResponses");
			if(ptrServiceResponsesNode != NULL)
			{
				_bstr_t baseName = ptrServiceResponsesNode->GetbaseName();				
				// ServiceResponses childs i.e Response
				MSXML2::IXMLDOMNodeListPtr ellipseResChilds = ptrServiceResponsesNode->GetchildNodes();
				if(ellipseResChilds != NULL)
				{
					for(int nCnt = 0; nCnt < ellipseResChilds->Getlength(); nCnt++)
					{
						// Response
						MSXML2::IXMLDOMNodePtr serviceResponse = ellipseResChilds->Getitem(nCnt);
						if(serviceResponse != NULL)
						{
							BSTR servResName = serviceResponse->GetbaseName();							
							// Response Childs i.e Dataitem
							MSXML2::IXMLDOMNodeListPtr serviceResponseChilds = serviceResponse->GetchildNodes();
							if(serviceResponseChilds != NULL)
							{
								for(int nCnt1 = 0; nCnt1 < serviceResponseChilds->Getlength(); nCnt1++)
								{
									
									MSXML2::IXMLDOMNodePtr response = serviceResponseChilds->Getitem(nCnt1);
									if(response != NULL)
									{
										BSTR resName = response->GetbaseName();										
										// Parameters
										MSXML2::IXMLDOMNodeListPtr dataItems = response->GetchildNodes();
										if(dataItems != NULL)
										{
											// DataItem
											CNSDataItem *pDataItem = new CNSDataItem;
											for(int nCnt2 = 0; nCnt2 < dataItems->Getlength(); nCnt2++)
											{
												MSXML2::IXMLDOMNodePtr paramter = dataItems->Getitem(nCnt2);
												if(paramter != NULL)
												{
													MSXML2::IXMLDOMNamedNodeMapPtr map = paramter->Getattributes();
													if(map != NULL)
													{
														MSXML2::IXMLDOMNodePtr nodePtr =  map->getNamedItem("name");
														if(nodePtr != NULL)
														{
															_bstr_t attValue = nodePtr->nodeValue.bstrVal;			
															BSTR paramText = paramter->Gettext();
                                                            #ifdef _INTERNAL_
                                                                TCHAR* szAttValue = (TCHAR*)_com_util::ConvertBSTRToString(attValue);															
															    TCHAR* szParamText = (TCHAR*)_com_util::ConvertBSTRToString(paramText);	
                                                                pDataItem->insertIntoMap(szAttValue, szParamText);	
                                                            #else
															    pDataItem->insertIntoMap((TCHAR*)attValue, (TCHAR*)paramText);															                                                                															    														
                                                            #endif
														}
													}
												}
											}	
											if(pDataItem->getSize() > 0)
												dataItemsVect.push_back(pDataItem);
											else
											{
												delete pDataItem;
												pDataItem = NULL;
											}
										}

									}
								}
							}
						}
					}
				}
			}
	

				// Initialize pIXMLDOMDocument (create a DOMDocument).
				// Load document.
				MSXML2::IXMLDOMNodeListPtr errorsNodePtr;
				errorsNodePtr = pRPCStruct->getElementsByTagName(
							_T("Error"));
				if(errorsNodePtr)
				{
					int nErrorListLen = errorsNodePtr->Getlength();
					for(int nCnt = 0; nCnt < nErrorListLen; nCnt++)
					{
						MSXML2::IXMLDOMNodePtr errorNodePtr = errorsNodePtr->Getitem(nCnt);
						if(errorNodePtr)
						{
							MSXML2::IXMLDOMNodePtr textNodePtr;
							textNodePtr = errorNodePtr->selectSingleNode(_T("Text"));
							TCHAR* szErrorTextValue, *szErrorFieldValue;
							if(textNodePtr)
							{
								_bstr_t attValue = textNodePtr->Gettext();
                                  #ifdef _INTERNAL_
                                     szErrorTextValue = (TCHAR*)_com_util::ConvertBSTRToString(attValue);
                                  #else
								     szErrorTextValue = (TCHAR*)attValue;
                                  #endif
								
							}
							MSXML2::IXMLDOMNodePtr FieldNodePtr;
							FieldNodePtr = errorNodePtr->selectSingleNode(_T("Field"));
							if(FieldNodePtr)
							{
								_bstr_t attValue = FieldNodePtr->Gettext();
                                   #ifdef _INTERNAL_
                                     szErrorFieldValue = (TCHAR*)_com_util::ConvertBSTRToString(attValue);
                                  #else
								     szErrorFieldValue = (TCHAR*)attValue;
                                  #endif
							}

							NSSTRING szResult = szErrorFieldValue;
							szResult += _T(":");
							szResult += szErrorTextValue;

							TCHAR *szErrMsg = new TCHAR[_MAX_PATH + 1];
							#ifdef _INTERNAL_
								//NSSTRCPY(szErrMsg, szResult.c_str());
							    NSSTRCPY(szErrMsg, _MAX_PATH + 1, szResult.c_str());
							#else
								NSSTRCPY(szErrMsg, _MAX_PATH + 1, szResult.c_str());
							#endif
							strErrorTexts.push_back(szErrMsg);
						}
					}

				}
			
				/*
				<EllipseReply messageId="a_message_id" totalErrors="1" version="1.0">
					<Errors>
						<Error>
							<Text>"INVALID USERID/PASSWORD TRY AGAIN "</Text> 
							<Field>EllipseLoginException</Field> 
						</Error>
					</Errors>
				</EllipseReply>
				*/
			/*	MSXML2::IXMLDOMNodePtr ptrErrorsNode = pRPCStruct->selectSingleNode("Errors");
				if(ptrErrorsNode!=NULL)
				{
					MSXML2::IXMLDOMNodeListPtr errorsNode = ptrErrorsNode->GetchildNodes();
					if(errorsNode != NULL)
					{
						for(int nCnt = 0; nCnt < errorsNode->Getlength(); nCnt++)
						{
							MSXML2::IXMLDOMNodePtr errorNode = errorsNode->Getitem(nCnt);
							if(errorNode != NULL)
							{
								MSXML2::IXMLDOMNodeListPtr errorNodeSubItems = errorNode->GetchildNodes();
								if(errorNodeSubItems!=NULL)
								{
									for(int nCnt1 = 0; nCnt1 < errorNodeSubItems->Getlength(); nCnt1++)
									{
										MSXML2::IXMLDOMNodePtr textNode = errorNodeSubItems->Getitem(nCnt1);
										if(textNode!=NULL)
										{
											 _bstr_t attValue = textNode->nodeValue.bstrVal;
											TCHAR* szErrorTextValue = (TCHAR*)_com_util::ConvertBSTRToString(attValue);
											MessageBox(NULL,szErrorTextValue,_T("Error"),MB_OK);
											strErrorTexts.push_back(szErrorTextValue);
										}

									}
								}
								//MSXML2::IXMLDOMNodePtr TextNode = ptrErrorsNode->Getch();
								//if(TextNode!=NULL)
								//{
								//	 _bstr_t attValue = TextNode->nodeValue.bstrVal;
								//	//_bstr_t strErrorText = TextNode->Gettext();
								//	TCHAR* szErrorTextValue = (TCHAR*)_com_util::ConvertBSTRToString(attValue);
								//	strErrorTexts.push_back(szErrorTextValue);
								//}
							}
						}
					}
				}
			}*/
		}
	}
	catch(...)
	{
		::SysFreeString(namespaceuri);		
		return _T("");
	}	
	return _T("");
}


void CNSEllipseConnector::release()
{
	try
	{
		if(m_ptrConnector != NULL)
			m_ptrConnector->Release();
		if(m_ptrSerializer != NULL)
		{
			m_ptrSerializer->Release();
		}
		if(m_ptrReader != NULL)
			m_ptrReader->Release();
	}
	catch(...)
	{
		MessageBox(NULL, _T("Exception in Release function"), _T("Exception"), MB_OK);
		return;
	}
}