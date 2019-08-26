Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Module ModStart

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private MyTxt As New VbaTextFile

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Public Sub CopyAttributes()
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blockref As AcadBlockReference
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim returnObj As AcadObject
        Dim basePnt As Object
        Dim sname As String

        On Error GoTo MYFix

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        Call ThisDrawing.Utility.GetEntity(returnObj, basePnt, "Select A Block")

        'UPGRADE_WARNING: Couldn't resolve default property of object returnObj.ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If UCase(returnObj.ObjectName) = UCase("AcDbBlockReference") Then
            blockref = returnObj
            'UPGRADE_WARNING: Couldn't resolve default property of object blockref.HasAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If blockref.HasAttributes = True Then
                Call GetAttribs(blockref)
            Else
                Call MsgBox("No Attributes Found", MsgBoxStyle.Exclamation, "Error")
            End If
        Else
            Call MsgBox("Next Time Pick a Block", MsgBoxStyle.Exclamation, "Error")
        End If

        Exit Sub

MYFix:
        Err.Clear()

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Public Sub PasteAttributes()
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blockref As AcadBlockReference
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim returnObj As AcadObject
        Dim basePnt As Object
        Dim sname As String

        On Error GoTo MYFix

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        Call ThisDrawing.Utility.GetEntity(returnObj, basePnt, "Select A Block")

        'UPGRADE_WARNING: Couldn't resolve default property of object returnObj.ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If UCase(returnObj.ObjectName) = UCase("AcDbBlockReference") Then
            blockref = returnObj
            'UPGRADE_WARNING: Couldn't resolve default property of object blockref.HasAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If blockref.HasAttributes = True Then
                Call PasteAttribs(blockref)
            Else
                Call MsgBox("No Attributes Found", MsgBoxStyle.Exclamation, "Error")
            End If
        Else
            Call MsgBox("Next Time Pick a Block", MsgBoxStyle.Exclamation, "Error")
        End If

        Exit Sub

MYFix:
        Err.Clear()
    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Private Sub GetAttribs(ByRef blockref As AcadBlockReference)
        Dim ThisDrawing_Renamed As Object
        Dim attribs() As Object
        Dim i As Short
        Dim sTag As String
        Dim sValue As String
        Dim sTxtFile As String
        Dim VntInfo() As String
        On Error GoTo MYFix
        '========================================================
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Application. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sTxtFile = ThisDrawing.Application.Path & "\CopyPastAtt.txt"
        'UPGRADE_WARNING: Couldn't resolve default property of object blockref.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object attribs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        attribs = blockref.GetAttributes

        ReDim VntInfo(UBound(attribs))
        For i = LBound(attribs) To UBound(attribs)
            'UPGRADE_WARNING: Couldn't resolve default property of object attribs().TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            sTag = Trim(attribs(i).TagString)
            'UPGRADE_WARNING: Couldn't resolve default property of object attribs().TextString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            sValue = attribs(i).TextString
            'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            VntInfo(i) = sTag & "|" & sValue
        Next i

        'UPGRADE_WARNING: Couldn't resolve default property of object blockref.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        Call MyTxt.TextFileOpen(sTxtFile, "Block Name = " & blockref.Name)
        For i = LBound(VntInfo) To UBound(VntInfo)
            'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            Call MyTxt.TextFileAppend(sTxtFile, CStr(VntInfo(i)))
        Next i

        Exit Sub

MYFix:
        Err.Clear()
    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Private Sub PasteAttribs(ByRef blockref As AcadBlockReference)
        Dim attribs As Object
        Dim i As Short
        Dim k As Short
        Dim sTag As String
        Dim sValue As String
        Dim VntValue As Object
        Dim sTxtFile As String
        Dim VntInfo As Object

        On Error GoTo MYFix
        '========================================================
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Application. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sTxtFile = ThisDrawing.Application.Path & "\CopyPastAtt.txt"
        'UPGRADE_WARNING: Couldn't resolve default property of object blockref.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object attribs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        attribs = blockref.GetAttributes

        'UPGRADE_WARNING: Couldn't resolve default property of object MyTxt.TextFileData(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object VntInfo. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        VntInfo = MyTxt.TextFileData(sTxtFile)
        If UBound(VntInfo) > 1 Then
            For i = 2 To UBound(VntInfo)
                'UPGRADE_WARNING: Couldn't resolve default property of object MyTxt.StringValuesSeperated(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                'UPGRADE_WARNING: Couldn't resolve default property of object VntValue. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                VntValue = MyTxt.StringValuesSeperated(VntInfo(i), "|")
                If UBound(VntValue) = 1 Then
                    'UPGRADE_WARNING: Couldn't resolve default property of object VntValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    sTag = VntValue(0)
                    'UPGRADE_WARNING: Couldn't resolve default property of object VntValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    sValue = VntValue(1)

                    For k = LBound(attribs) To UBound(attribs)
                        'UPGRADE_WARNING: Couldn't resolve default property of object attribs().TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        If Trim(attribs(k).TagString) = sTag Then
                            'UPGRADE_WARNING: Couldn't resolve default property of object attribs().TextString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            attribs(k).TextString = sValue
                            Exit For
                        End If
                    Next k
                End If
            Next i
        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object blockref.Update. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        blockref.Update()

        Exit Sub

MYFix:
        Err.Clear()
    End Sub
End Module