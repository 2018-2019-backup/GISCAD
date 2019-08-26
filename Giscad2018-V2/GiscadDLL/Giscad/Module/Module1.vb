Option Strict Off
Option Explicit On

Module Module1
	Function XMLHeader() As String
		
		XMLHeader = "<?xml version=""1.0"" encoding=""UTF-8""?>"
		XMLHeader = XMLHeader & "<SOAP-ENV:Envelope SOAP-ENV:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/""" & vbCrLf
		XMLHeader = XMLHeader & " xmlns:SOAP-ENC=""http://schemas.xmlsoap.org/soap/encoding/""" & vbCrLf
		XMLHeader = XMLHeader & " xmlns:xsi=""http://www.w3.org/1999/XMLSchema-instance""" & vbCrLf
		XMLHeader = XMLHeader & " xmlns:SOAP-ENV=""http://schemas.xmlsoap.org/soap/envelope/""" & vbCrLf
		XMLHeader = XMLHeader & " xmlns:xsd=""http://www.w3.org/1999/XMLSchema"">" & vbCrLf
        XMLHeader = XMLHeader & "<SOAP-ENV:Body>" & vbCrLf
		
    End Function

	Function XMLFooter() As String
		XMLFooter = vbCrLf & "</SOAP-ENV:Body></SOAP-ENV:Envelope>" & vbCrLf
    End Function

	Function XMLRequestStart() As String
		
		XMLRequestStart = "<EllipseRequest messageId=""a_message_id"" version=""1.0"""
		XMLRequestStart = XMLRequestStart & " xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"""
		XMLRequestStart = XMLRequestStart & " xsi:noNamespaceSchemaLocation=""H:\Scada-ELL\XSD\EllipseRequest.xsd"">"
		
    End Function

	Function XMLRequestEnd() As String
		XMLRequestEnd = "</EllipseRequest>"
    End Function

	Function CreateXML() As String
		Dim xml As String
		
		xml = ""
		xml = xml & "<Service name=""EQUIPMENT.Create"">"
		xml = xml & "<ReplyParameters>"
		xml = xml & "<Parameter name=""EquipNo""/>"
		xml = xml & "</ReplyParameters>"
		xml = xml & "<Request id=""ID00001"">"
		
		CreateXML = xml
		
    End Function

	Function LoginXML(ByVal uname As String, ByVal pwd As String, ByVal pos As String, ByVal sDistrict As String) As String
		
		LoginXML = ""
		LoginXML = LoginXML & "<Credentials>"
		LoginXML = LoginXML & "<Username>" & uname & "</Username>"
		LoginXML = LoginXML & "<Password>" & pwd & "</Password>"
		LoginXML = LoginXML & "<Position>" & pos & "</Position>"
		LoginXML = LoginXML & "<District>" & sDistrict & "</District>"
		LoginXML = LoginXML & "</Credentials>" & vbCrLf
		
	End Function
	
	'UPGRADE_NOTE: class was upgraded to class_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Function GenericParams(ByVal plantcode0 As String, ByVal address As String, ByVal uname As String, ByVal acccode As String, ByVal class_Renamed As String, ByVal equipgrpid As String, ByVal class18 As String, ByVal sDepotCode As String) As String
		
		GenericParams = ""
		GenericParams = GenericParams & "<Parameter name=""EquipClass"">" & class_Renamed & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""PlantCode0"">" & plantcode0 & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""EquipGrpId"">" & equipgrpid & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""AccountCode"">" & acccode & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""ActiveFlag"">Y</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""InputBy"">" & uname & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""EquipStatus"">PR</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""EquipNoD1"">" & address & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""EquipClassif6"">" & sDepotCode & "</Parameter>"
		GenericParams = GenericParams & "<Parameter name=""EquipClassif18"">" & class18 & "</Parameter>"
		
    End Function

	Function XMLExecute(ByVal sXML As String, ByVal sURL As String) As String
        Dim strSoapAction As String
		
		Debug.Print(sXML)

        strSoapAction = "process"
		XMLExecute = PostWebservice(sURL, strSoapAction, sXML)
        Debug.Print(XMLExecute)
    End Function

	Private Function PostWebservice(ByVal AsmxUrl As String, ByVal SoapActionUrl As String, ByVal XmlBody As String) As String
        Dim objDom As MSXML2.DOMDocument
        Dim objXmlHttp As MSXML2.XMLHTTP
		Dim strRet As String
		Dim intPos1 As Short
		Dim intPos2 As Short
		
		On Error GoTo Err_PW
		
		' Create objects to DOMDocument and XMLHTTP
        objDom = CreateObject("MSXML2.DOMDocument")
		objXmlHttp = CreateObject("MSXML2.XMLHTTP")
		
        ' Load XML
        objDom.async = False
		objDom.loadXML(XmlBody)
		
        ' Open the webservice
        objXmlHttp.open("POST", AsmxUrl, False)
		
        ' Create headings
        objXmlHttp.setRequestHeader("Content-Type", "text/xml; charset=utf-8")
		objXmlHttp.setRequestHeader("SOAPAction", SoapActionUrl)
		
        ' Send XML command		
		objXmlHttp.send(objDom.xml)
		
        ' Get all response text from webservice
		strRet = objXmlHttp.responseText
		
        ' Close object
		objXmlHttp = Nothing
		
		' Extract result
		intPos1 = InStr(strRet, "Result>") + 7
		intPos2 = InStr(strRet, "</")
		If intPos1 > 7 And intPos2 > 0 Then
			strRet = Mid(strRet, intPos1, intPos2 - intPos1)
		End If
		
		' Return result
		PostWebservice = strRet
		
		Exit Function
Err_PW: 
		PostWebservice = "Error: " & Err.Number & " - " & Err.Description
		
	End Function
End Module