Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Public Class ThisDrawing

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property
    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub LoadAssets()

        Dim ssblks() As AcadBlockReference = Nothing
        Dim objName As String
        'UPGRADE_ISSUE: AcadEntity object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim entry As AcadEntity
        'UPGRADE_ISSUE: AcadEntity object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim attr As AcadEntity
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim v As Object
        Dim SS As String ' Name of selection set
        Dim strAttributes As String
        Dim i As Short

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For Each entry In ThisDrawing.ModelSpace
            'UPGRADE_WARNING: Couldn't resolve default property of object entry.ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If entry.ObjectName = "AcDbBlockReference" Then
                ssblks(0) = entry
                'UPGRADE_WARNING: Couldn't resolve default property of object ssblks().Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If GetBlockSS(ssblks(0).Name, SS) Then
                    'UPGRADE_WARNING: Couldn't resolve default property of object ssblks().GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object v. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    v = ssblks(0).GetAttributes()
                    For i = LBound(v) To UBound(v)
                        'UPGRADE_WARNING: Couldn't resolve default property of object v().TextString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        'UPGRADE_WARNING: Couldn't resolve default property of object v().TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        If Trim(v(i).TagString) = "ASSET_NUM" And Trim(v(i).TextString) = "" Then
                            ' Add to Selection set
                            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            ThisDrawing.SelectionSets.Item("SS").AddItems(ssblks)
                        End If
                    Next
                End If
            End If
        Next entry
    End Sub
    Function GetBlockSS(ByVal bName As String, ByRef SelectionSet As Object) As Boolean
        Dim BlockNew As Object
        'UPGRADE_WARNING: Couldn't resolve default property of object BlockNew. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        BlockNew = False


        'UPGRADE_WARNING: Couldn't resolve default property of object BlockNew. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        BlockNew = True
    End Function
    Sub Main()
        ' Get list of data from db table - fields: group,blockname,class, etc

        CreateSS() ' Create Selection sets

        ' Scan for blocks in the drawing that match the block names in the table
        ' Only get blocks with empty asset num
        ' create a new selection set for each block group

        ' display selectionset count for user to verify

        ' loop through each selection set creating the asset in ellipse

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub CreateSS()
        Dim rs As Object
        Dim i As Object

        ' First clear existing selection sets
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = ThisDrawing.SelectionSets.Count - 1 To 0 Step -1
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ThisDrawing.SelectionSets.Item(i).Clear()
        Next i

        ' select distinct group from asset_num

        'UPGRADE_WARNING: Couldn't resolve default property of object rs.EOF. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        Do While Not rs.EOF
            'UPGRADE_WARNING: Couldn't resolve default property of object rs.group. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ThisDrawing.SelectionSets.Add(rs.group)
            'UPGRADE_WARNING: Couldn't resolve default property of object rs.movenext. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            rs.movenext()
        Loop

    End Sub
End Class