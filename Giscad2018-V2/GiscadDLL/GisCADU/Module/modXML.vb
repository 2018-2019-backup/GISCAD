Option Strict Off
Option Explicit On

Module modXML
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
        xml = xml & "<Parameter name=""PlantNo""/>"
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
    Function CharReplace(ByVal txtstr As String) As String

        CharReplace = Replace(txtstr, "&", "&amp;")
        CharReplace = Replace(CharReplace, ">", "&gt;")
        CharReplace = Replace(CharReplace, "<", "&lt;")

    End Function
    'UPGRADE_NOTE: Class was upgraded to Class_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Function GenericParams(ByVal PlantCode0 As String, ByVal address As String, ByVal uname As String, ByVal acccode As String, ByVal Class_Renamed As String, ByVal equipgrpid As String, ByVal class18 As String, ByVal sDepotCode As String, ByVal StreetCode As Object) As String

        GenericParams = "" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipClass"">" & Class_Renamed & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""PlantCode0"">" & PlantCode0 & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipGrpId"">" & equipgrpid & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""AccountCode"">" & acccode & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""ActiveFlag"">Y</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""InputBy"">" & uname & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipStatus"">PR</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipNoD1"">" & CharReplace(address) & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipClassif6"">" & sDepotCode & "</Parameter>" & vbCrLf
        GenericParams = GenericParams & "<Parameter name=""EquipClassif18"">" & class18 & "</Parameter>" & vbCrLf
        'UPGRADE_WARNING: Couldn't resolve default property of object StreetCode. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        GenericParams = GenericParams & "<Parameter name=""EquipLocation"">" & StreetCode & "</Parameter>" & vbCrLf

    End Function

    Function XMLExecute(ByVal sxml As String, ByVal sURL As String) As String
        Dim strSoapAction As String

        Debug.Print("========================================================================")
        Debug.Print("XML EXECUTE")
        Debug.Print(sxml)
        strSoapAction = "process"
        XMLExecute = PostWebservice(sURL, strSoapAction, sxml)
        Debug.Print(XMLExecute)

    End Function

    Private Function PostWebservice(ByVal AsmxUrl As String, ByVal SoapActionUrl As String, ByVal XmlBody As String) As String
        Dim objDom As MSXML2.DOMDocument
        'Dim objXmlHttp As MSXML2.XMLHTTP'------Used Initially
        Dim objXmlHttp As MSXML2.ServerXMLHTTP
        Dim strRet As String
        Dim intPos1 As Short
        Dim intPos2 As Short

        Dim timeout As Object = GetIniString("SERVICE", "Timeout")

        If String.IsNullOrEmpty(timeout) Then
            timeout = 20 'Default 20 seconds
        End If

        If IsNumeric(timeout) Then
            timeout = Convert.ToInt32(timeout)
        Else
            timeout = 20 'Default 20 seconds
        End If

        If Convert.ToInt32(timeout) > 0 Then
            'Convert to milliseconds
            timeout *= 1000
        Else
            timeout = 20000 'Default 20 seconds
        End If

        On Error GoTo Err_PW

        ' Create objects to DOMDocument and XMLHTTP
        objDom = CreateObject("MSXML2.DOMDocument")
        'objDom = CreateObject("msxml2.DOMDocument.6.0")
        'objXmlHttp = CreateObject("MSXML2.XMLHTTP") '------Used Initially


        objXmlHttp = CreateObject("MSXML2.ServerXMLHTTP.6.0")

        'objXmlHttp = CreateObject("MSXML2.XMLHTTP.6.0")

        ' Load XML
        objDom.async = False
        objDom.loadXML(XmlBody)


        'objXmlHttp.setTimeouts(25000, 30000, 25000, 25000) 'Timeout values are in milliseconds

        objXmlHttp.setTimeouts(timeout, timeout, timeout, timeout) 'Timeout values are in milliseconds

        ' Open the webservice
        objXmlHttp.open("POST", AsmxUrl, False)

        ' Create headings
        objXmlHttp.setRequestHeader("Content-Type", "text/xml; charset=utf-8")
        objXmlHttp.setRequestHeader("SOAPAction", SoapActionUrl)

        ' Send XML command
        objXmlHttp.send(objDom.xml)

        'If objXmlHttp.waitForResponse(60) Then
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
        'Else
        'PostWebservice = "Error: Web Service is not available"
        'End If
        Exit Function
Err_PW:
        PostWebservice = "Error: " & Err.Number & " - " & Err.Description

    End Function
End Module