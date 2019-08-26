Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Module modACAD

    Function GetBlockName(ByRef blk As AcadBlockReference) As String

        On Error GoTo Err_GetBlockName

        'UPGRADE_WARNING: Couldn't resolve default property of object blk.IsDynamicBlock. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If blk.IsDynamicBlock Then
            'UPGRADE_WARNING: Couldn't resolve default property of object blk.EffectiveName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            GetBlockName = blk.EffectiveName
        Else
            'UPGRADE_WARNING: Couldn't resolve default property of object blk.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            GetBlockName = blk.Name
        End If
        Exit Function
Err_GetBlockName:
        GetBlockName = "UNKNOWN"
    End Function
	
	'UPGRADE_NOTE: GetAttr was upgraded to GetAttr_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Function GetAttr_Renamed(ByRef blk As AcadBlockReference, ByRef tag As String) As String
		Dim i As Short
        Dim vattrs() As Object
        Dim retAtr As String = ""
		
		'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
		vattrs = blk.GetAttributes
		
		For i = LBound(vattrs) To UBound(vattrs)
			'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
			If vattrs(i).TagString = tag Then
				'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                GetAttr_Renamed = Trim(vattrs(i).TextString)
                retAtr = GetAttr_Renamed
			End If
		Next i
        GetAttr_Renamed = retAtr
	End Function

    Public Function SetAttrValue(ByRef blk As AcadBlockReference, ByVal tag As String, ByVal sAttrVal As String) As Boolean
        Dim i As Short
        Dim vattrs() As Object
        SetAttrValue = False
        'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If vattrs(i).TagString = tag Then
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                vattrs(i).textString = sAttrVal
                SetAttrValue = True
            End If
        Next i

    End Function

    Function SetAttrStyle(ByRef blk As AcadBlockReference, ByVal sTag As String, ByVal sStyle As String) As Boolean
        Dim i As Short
        Dim vattrs() As Object
        SetAttrStyle = False
        'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If vattrs(i).TagString = sTag Then
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Style. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                vattrs(i).Style = sStyle
                SetAttrStyle = True
            End If
        Next i

    End Function

    Function SetAttrHeight(ByRef blk As AcadBlockReference, ByVal sTag As String, ByVal dHeight As String) As Boolean
        Dim i As Short
        Dim vattrs() As Object
        SetAttrHeight = False
        'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If vattrs(i).TagString = sTag Then
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().height. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                vattrs(i).height = dHeight
                SetAttrHeight = True
            End If
        Next i
    End Function

    Function SetAttrLayer(ByRef blk As AcadBlockReference, ByVal tag As String, ByVal sLayer1 As String, ByVal sLayer2 As String) As Boolean
        Dim i As Short
        Dim vattrs() As Object
        SetAttrLayer = False
        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            If vattrs(i).TagString.ToString().Equals(tag, StringComparison.InvariantCultureIgnoreCase) Then
                vattrs(i).Layer = IIf(vattrs(i).Layer = sLayer2, sLayer1, sLayer2)
                SetAttrLayer = True
            End If
        Next i
    End Function


    Function GetAddress(ByRef blk As AcadBlockReference) As String
        Dim i As Short
        Dim vattrs() As Object
        Dim Street As String = ""
        Dim Suburb As String = ""

        'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If vattrs(i).TagString = "STREETNAME" Then
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                Street = Trim(vattrs(i).textString)
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(i).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ElseIf vattrs(i).TagString = "SUBURB" Then
                'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                Suburb = Trim(vattrs(i).textString)
            End If
        Next i
        ''GetAddress = Left(Replace(Street & " - " & Suburb, "&", "and"), 40)
        GetAddress = Left(Suburb & " - " & Street, 40)
    End Function
End Module