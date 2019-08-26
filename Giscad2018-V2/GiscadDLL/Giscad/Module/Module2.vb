Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Module Module2
    'UPGRADE_NOTE: GetAttr was upgraded to GetAttr_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Function GetAttr_Renamed(ByRef blk As AcadBlockReference, ByRef tag As String) As String
        Dim i As Integer
        Dim retAttr As String = ""
        Dim vattrs() As Object

        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            If vattrs(i).TagString = tag Then
                retAttr = Trim(vattrs(i).TextString)
            End If
        Next i
        GetAttr_Renamed = retAttr
    End Function

    Function GetAddress(ByRef blk As AcadBlockReference) As String
        Dim i As Integer

        Dim vattrs() As Object
        Dim street As String = ""
        Dim suburb As String = ""

        vattrs = blk.GetAttributes

        For i = LBound(vattrs) To UBound(vattrs)
            If vattrs(i).TagString = "STREETNAME" Then
                street = Trim(vattrs(i).TextString)
            ElseIf vattrs(i).TagString = "SUBURB" Then
                suburb = Trim(vattrs(i).TextString)
            End If
        Next i
        GetAddress = street & " - " & suburb
    End Function
End Module