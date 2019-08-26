Option Strict Off
Option Explicit On
Imports System.Runtime.InteropServices
Imports System.Text
Module Module3
	
	Declare Function GetPrivateProfileInt Lib "kernel32"  Alias "GetPrivateProfileIntA"(ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal nDefault As Integer, ByVal lpFileName As String) As Integer
	'UPGRADE_ISSUE: Declaring a parameter 'As Any' is not supported. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
    'Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As StringBuilder, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
	'UPGRADE_ISSUE: Declaring a parameter 'As Any' is not supported. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
	'UPGRADE_ISSUE: Declaring a parameter 'As Any' is not supported. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"'
    Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpString As String, ByVal lpFileName As String) As Integer
    Private Declare Auto Function GetPrivateProfileString Lib "kernel32" (ByVal lpAppName As String, _
               ByVal lpKeyName As String, _
               ByVal lpDefault As String, _
               ByVal lpReturnedString As StringBuilder, _
               ByVal nSize As Integer, _
               ByVal lpFileName As String) As Integer
	
	Public gInifile As String
	
	
	'UPGRADE_NOTE: default was upgraded to default_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Function GetIniFileInt(ByVal gInifile As String, ByVal section As String, ByVal variable As String, ByVal default_Renamed As Short) As Integer
		' This procedure is used to get a numeric variable from a file
		
		Dim Value As Integer
		
		Value = GetPrivateProfileInt(section, variable, default_Renamed, gInifile)
		'Call WriteDebugFile("GetIniFileInt: File = " & gInifile & " [" & section & "] " & variable & " = " & Value)
		GetIniFileInt = Value
		
	End Function
	
	Function GetIniFileString(ByVal gInifile As String, ByVal section As String, ByVal variable As String) As String
		
		' get a string parameter from the ini file
        Dim zBuff As StringBuilder
		Dim nChars As Integer
		Dim nBuffLen As Integer
		Dim j As Short
		
		'Call WriteDebugFile("GetIniFileString(): Start", True)
		'Call WriteDebugFile("Inifile: " & gInifile)
		'Call WriteDebugFile("Section: " & section)
		'Call WriteDebugFile("parameter: " & variable)
		
		'keep making the buffer bigger until it is big enough
		Do 
            nBuffLen = nBuffLen + 256
            'zBuff = Space(nBuffLen)
            zBuff = New StringBuilder(nBuffLen + 500)
			If variable <> "" Then
				nChars = GetPrivateProfileString(section, variable, "", zBuff, nBuffLen, gInifile)
			Else
				nChars = GetPrivateProfileString(section, 0, "", zBuff, nBuffLen, gInifile)
			End If
			
			'nChars holds nBuffLen - 2 if buffer too small
		Loop Until nChars <> nBuffLen - 2
		
		If nChars > 0 Then
			'GetIniFileString = Left(zBuff, nChars)
            GetIniFileString = Mid(zBuff.ToString, 1, nChars)
			'Call WriteDebugFile("GetIniString[" & section & "] " & variable & " = " & Left(zBuff, nChars))
		Else
			GetIniFileString = ""
			'Call WriteDebugFile("GetIniString[" & section & "] " & variable & " is not set")
		End If
		
		'Call WriteDebugFile("GetIniFileString(): Start", True)
		
	End Function
	
	Function GetLit_Delim(ByVal idx As Short, ByVal literals As String, ByVal delim As String) As String
		
		' Find a substring within another string delimited by a common delimiter
		
		Dim j As Short
		Dim start_pos As Short
		Dim delim_pos As Short
		Dim tvalue As String
		
		start_pos = 0
		delim_pos = 0
		
		' if the delimiter doesn't exist then exit
		If InStr(literals, delim) = 0 Then
			GetLit_Delim = ""
			Exit Function
		End If
		
		' delimiter must exist so let's find the substring
		'Call WriteDebugFile("GetLit_Delim: string = " & literals$)
		'Call WriteDebugFile("Index = " & Str(idx%))
		
		For j = 1 To idx
			start_pos = delim_pos + 1
			delim_pos = InStr(start_pos, literals, delim)
			If delim_pos = 0 Then
				delim_pos = Len(literals) + 1
			End If
		Next j
		
		If start_pos > Len(literals) Then
			GetLit_Delim = ""
		Else
			'Call WriteDebugFile("Start = " & Str(start_pos%))
			'Call WriteDebugFile("delim = " & Str(delim_pos%))
			tvalue = Mid(literals, start_pos, delim_pos - start_pos)
			'Call WriteDebugFile("lit = " & tvalue)
			GetLit_Delim = tvalue
		End If
		
	End Function
	
	'UPGRADE_NOTE: default was upgraded to default_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Function GetIniInt(ByVal section As String, ByVal variable As String, ByVal default_Renamed As Short) As Short
		
		' This procedure is used to get a numeric variable from the .ini file
		''GED On Error goto GetIniIntErr
		
		Dim Value As Short
		
		Value = GetIniFileInt(gInifile, section, variable, default_Renamed)
		GetIniInt = Value
		
	End Function
	
	Function GetIniString(ByRef section As String, ByRef variable As String) As Object
		
		' get a string parameter from the ini file
		'Call WriteDebugFile("GetIniString(): Start", True)
		
		GetIniString = GetIniFileString(gInifile, section, variable)
		'Call WriteDebugFile("GetIniString(): End", True)
		
	End Function
	
	Function PutIniString(ByVal section As String, ByVal entry As String, ByVal Value As String) As Short
		
		PutIniString = False
		PutIniString = WritePrivateProfileString(section, entry, Value, gInifile)
		'**'GED 'debug.print "PutIniString[" & section & "] " & entry$ & " = " & Value$
		
	End Function
	
	Function SetInifile(ByVal inifile As String) As Short
		
		SetInifile = False
		gInifile = inifile
		'UPGRADE_WARNING: Dir has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
        If Dir(gInifile) = "" Then Exit Function
		SetInifile = True
		
	End Function
End Module