Option Strict Off
Option Explicit On
Friend Class VbaTextFile
	
	
	Public Function TextFileExist(ByRef sFileName As String) As Boolean
		'UPGRADE_WARNING: Dir has a new behavior. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="9B7D5ADD-D8FE-4819-A36C-6DEDAF088CC7"'
		If Dir(sFileName, FileAttribute.Directory) <> "" Then
			TextFileExist = True
		Else
			TextFileExist = False
		End If
    End Function

	
	Public Sub TextFileOpen(ByRef sFileName As String, ByRef sString As String)
		FileOpen(1, sFileName, OpenMode.Output)
		PrintLine(1, sString)
		FileClose(1)
	End Sub
	
	Public Sub TextFileAppend(ByRef sFileName As String, ByRef sString As String, Optional ByRef bLastLine As Boolean = False)
		FileOpen(1, sFileName, OpenMode.Append)
		'
		If bLastLine = True Then
			Print(1, sString, TAB)
		Else
			PrintLine(1, sString)
		End If
		'
		FileClose(1)
	End Sub
	
	Public Function TextFileData(ByRef sFileName As String, Optional ByRef sKeyWord As String = "") As Object
		Dim s As String
		Dim sLine As String
		Dim sArray() As String
		ReDim sArray(0)
		Dim i As Short
		
		FileOpen(1, sFileName, OpenMode.Input)
		
		Do Until EOF(1)
			s = LineInput(1)
			If Trim(sKeyWord) = "" Then
				ReDim Preserve sArray(UBound(sArray) + 1)
				sArray(UBound(sArray)) = s
			Else
				If UCase(s) Like UCase(sKeyWord) Then
					sLine = s
					Exit Do
				End If
			End If
		Loop 
		
		FileClose(1)
		
		If sKeyWord = "" Then
			'UPGRADE_WARNING: Couldn't resolve default property of object TextFileData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			TextFileData = VB6.CopyArray(sArray)
		Else
			'UPGRADE_WARNING: Couldn't resolve default property of object TextFileData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			TextFileData = sLine
		End If
		
	End Function
	
	Public Function TextFileDataReplace(ByRef sFileName As String, ByRef sOldString As String, ByRef sNewString As String) As Boolean
		Dim s As String
		Dim i As Short
		Dim sArray() As String
		Dim sString As String
		
		TextFileDataReplace = False
		On Error GoTo myerrorHand
		
		FileOpen(1, sFileName, OpenMode.Input)
		Do Until EOF(1)
			s = LineInput(1)
			If sString = "" Then
				ReDim sArray(0)
				sString = s
				sArray(UBound(sArray)) = sString
			Else
				ReDim Preserve sArray(UBound(sArray) + 1)
				sString = s
				If UCase(sString) Like UCase(sOldString) Then
					sString = sNewString
				End If
				sArray(UBound(sArray)) = sString
			End If
		Loop 
		FileClose(1)
		'
		FileOpen(1, sFileName, OpenMode.Output)
		For i = 0 To UBound(sArray)
			PrintLine(1, sArray(i))
		Next i
		FileClose(1)
		
		TextFileDataReplace = True
myerrorHand: 
		Err.Clear()
	End Function
	
	Public Function PullStringApart(ByRef VntString As Object, ByRef sSeparator As String) As Object
		
		Dim VntInfo() As Object
		Dim sLeftOver As String
		Dim i As Short
		Dim k As Short
		Dim bDone As Boolean
		
		ReDim VntInfo(0)
		k = Len(sSeparator)
		'
		'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(0). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		VntInfo(0) = ""
		'
		'UPGRADE_WARNING: Couldn't resolve default property of object VntString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		sLeftOver = Trim(VntString)
		'
		Do Until bDone = True
			'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			If Trim(VntInfo(UBound(VntInfo))) <> "" Then
				ReDim Preserve VntInfo(UBound(VntInfo) + 1)
			End If
			
			i = InStr(1, sLeftOver, sSeparator)
			
			If i = 0 Then
				If Trim(sLeftOver) <> "" Then
					'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(UBound()). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
					VntInfo(UBound(VntInfo)) = Trim(sLeftOver)
				Else
					ReDim Preserve VntInfo(UBound(VntInfo) - 1)
				End If
				bDone = True
			Else
				'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(UBound()). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				VntInfo(UBound(VntInfo)) = Trim(CStr(Mid(sLeftOver, 1, i - k))) '- 1
				sLeftOver = Trim(CStr(Mid(sLeftOver, i + k))) ' + 1
			End If
		Loop 
		
		'UPGRADE_WARNING: Couldn't resolve default property of object PullStringApart. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		PullStringApart = VB6.CopyArray(VntInfo)
		
	End Function
	
	Public Function StringReplace(ByRef sString As String, ByRef sFind As String, ByRef sReplace As String, Optional ByRef MatchWholeWord As Boolean = False) As String
		'
		Dim vntBroken As Object
		Dim i As Short
		Dim s As String
		'
		'UPGRADE_WARNING: Couldn't resolve default property of object StringValuesSeperated(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		'UPGRADE_WARNING: Couldn't resolve default property of object vntBroken. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		vntBroken = StringValuesSeperated(sString, sFind, MatchWholeWord)
		For i = 0 To UBound(vntBroken)
			If i = 0 Then
				'UPGRADE_WARNING: Couldn't resolve default property of object vntBroken(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				s = vntBroken(i)
			Else
				'UPGRADE_WARNING: Couldn't resolve default property of object vntBroken(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				s = s & sReplace & vntBroken(i)
			End If
		Next i
		StringReplace = s
	End Function
	
	Public Function StringValuesSeperated(ByRef VntString As Object, ByRef sSeperator As String, Optional ByRef MatchWholeWord As Boolean = False) As Object
		Dim n As Short
		Dim i As Short
		Dim Vnt As Object
		Dim s() As String
		Dim nLenSep As Short
		Dim sString As String
		
		'UPGRADE_WARNING: Couldn't resolve default property of object VntString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		sString = VntString
		nLenSep = Len(sSeperator)
		
		n = InStr(1, UCase(sString), UCase(sSeperator))
		If n <> 0 Then
			'UPGRADE_WARNING: Couldn't resolve default property of object InStrPositions(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			'UPGRADE_WARNING: Couldn't resolve default property of object Vnt. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			Vnt = InStrPositions(sString, sSeperator, MatchWholeWord)
		Else
			ReDim s(0)
			s(0) = sString
			'UPGRADE_WARNING: Couldn't resolve default property of object StringValuesSeperated. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			StringValuesSeperated = VB6.CopyArray(s)
			Exit Function
		End If
		'
		'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(0). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		If LBound(Vnt) = 0 And Vnt(0) = 0 Then
			ReDim s(0)
			s(0) = sString
			'UPGRADE_WARNING: Couldn't resolve default property of object StringValuesSeperated. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			StringValuesSeperated = VB6.CopyArray(s)
			Exit Function
		End If
		'
		ReDim s(UBound(Vnt) + 1)
		
		For i = LBound(s) To UBound(s)
			If i = LBound(s) Then
				'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				s(LBound(s)) = Mid(sString, 1, Vnt(LBound(Vnt)) - 1)
			ElseIf i = UBound(s) Then 
				'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				s(UBound(s)) = Mid(sString, Vnt(UBound(Vnt)) + nLenSep)
			Else
				'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(i - 1). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(i). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				'UPGRADE_WARNING: Couldn't resolve default property of object Vnt(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
				s(i) = Mid(sString, Vnt(i - 1) + nLenSep, Vnt(i) - Vnt(i - 1) - nLenSep)
			End If
		Next i
		
		'UPGRADE_WARNING: Couldn't resolve default property of object StringValuesSeperated. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		StringValuesSeperated = VB6.CopyArray(s)
		
	End Function
	
	Private Function InStrPositions(ByRef String1 As String, ByRef String2 As String, Optional ByRef MatchWholeWord As Boolean = False) As Object
		Dim n As Short
		Dim m As Short
		Dim k As Short
		Dim i As Short
		Dim nPos() As Short
		Dim s As String
		Dim s1 As String
		Dim s2 As String
		Dim nLenString2 As Short
		ReDim nPos(0)
		n = InStr(1, UCase(String1), UCase(String2))
		If n = 0 Then Exit Function
		ReDim nPos(0)
		nPos(0) = n
		'
		If MatchWholeWord = True Then
			m = n - 1
			k = n + Len(String2)
			'
			s1 = Mid(String1, m, 1)
			s2 = Mid(String1, k, 1)
			'
			If m <> 0 And k <= Len(String1) Then
				If Mid(String1, m, 1) = " " And Mid(String1, k, 1) = " " Then
					nPos(UBound(nPos)) = n
				Else
					nPos(UBound(nPos)) = 0
				End If
			ElseIf m = 0 And k <= Len(String1) Then 
				If Mid(String1, k, 1) = " " Then
					nPos(UBound(nPos)) = n
				Else
					nPos(UBound(nPos)) = 0
				End If
			ElseIf m <> 0 And k > Len(String1) Then 
				If Mid(String1, m, 1) = " " Then
					nPos(UBound(nPos)) = n
				Else
					nPos(UBound(nPos)) = 0
				End If
			End If
		End If
		'
		nLenString2 = Len(String2)
		'
		Do Until n = 0
			'
			n = InStr(n + 1, UCase(String1), UCase(String2))
			'
			m = n - 1
			k = n + Len(String2)
			'
			If n = 0 Then Exit Do
			If MatchWholeWord = True Then
				If m <> 0 And k <= Len(String1) Then
					If Mid(String1, m, 1) = " " And Mid(String1, k, 1) = " " Then
						If nPos(0) = 0 Then
							nPos(UBound(nPos)) = n
						Else
							ReDim Preserve nPos(UBound(nPos) + 1)
							nPos(UBound(nPos)) = n
						End If
					End If
				ElseIf m = 0 And k <= Len(String1) Then 
					If Mid(String1, k, 1) = " " Then
						If nPos(0) = 0 Then
							nPos(UBound(nPos)) = n
						Else
							ReDim Preserve nPos(UBound(nPos) + 1)
							nPos(UBound(nPos)) = n
						End If
					End If
				ElseIf m <> 0 And k > Len(String1) Then 
					If Mid(String1, m, 1) = " " Then
						If nPos(0) = 0 Then
							nPos(UBound(nPos)) = n
						Else
							ReDim Preserve nPos(UBound(nPos) + 1)
							nPos(UBound(nPos)) = n
						End If
					End If
				End If
			Else
				ReDim Preserve nPos(UBound(nPos) + 1)
				nPos(UBound(nPos)) = n
			End If
		Loop 
		'
		'UPGRADE_WARNING: Couldn't resolve default property of object InStrPositions. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		InStrPositions = VB6.CopyArray(nPos)
	End Function
End Class