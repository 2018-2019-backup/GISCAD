Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Module modATTR

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            ' Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Public Sub UserCopyAttsFromBlkToAnotherBlk()
        '------------------------------------------------------------------------------
        'user selects block and then selects attributed block to match to
        '
        '------------------------------------------------------------------------------
        Dim vAtts() As Object
        Dim vAtts2() As Object
        Dim sPrompt As String
        '''''''''''''''''''''''''''''''''''''''
        On Error GoTo ErrHandler

        sPrompt = "Select Source Attributed Block: "
        'UPGRADE_WARNING: Couldn't resolve default property of object UserGetBlockWithAtts().GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vAtts. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vAtts = UserGetBlockWithAtts(sPrompt).GetAttributes

        sPrompt = "Select Destination Attributed Block: "
        'UPGRADE_WARNING: Couldn't resolve default property of object UserGetBlockWithAtts().GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object vAtts2. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        vAtts2 = UserGetBlockWithAtts(sPrompt).GetAttributes

        Call CopySameAttsFromBlkToBlk(vAtts, vAtts2)

ErrHandler:
        Select Case Err.Number
            Case 0
            Case Else
                Err.Clear()
        End Select
    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Public Function UserGetBlockWithAtts(Optional ByRef sPrompt As String = "Select Attributed Block: ") As AcadBlockReference
        Dim acObj As AcadEntity = Nothing
        Dim vPickPt As Object = Nothing
        Dim sAtt As String
        Dim retBlk As AcadBlockReference = Nothing
        '''''''''''''''''''''''''''''''''''''''
        On Error GoTo NOT_ENTITY

TRY_AGAIN:
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.Utility.GetEntity(acObj, vPickPt, sPrompt)
        'UPGRADE_WARNING: Couldn't resolve default property of object acObj.ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If acObj.ObjectName <> "AcDbBlockReference" Then
            GoTo NOT_ENTITY
        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object acObj.HasAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If acObj.HasAttributes Then
            UserGetBlockWithAtts = acObj
            retBlk = UserGetBlockWithAtts
        Else
            MsgBox("This Block has no Attributes")
        End If
        UserGetBlockWithAtts = retBlk
        Exit Function

NOT_ENTITY:
        'If you click on space or do not select an entity, this error will be generated
        If MsgBox("You have not selected a block with attributes.", MsgBoxStyle.OkCancel) = MsgBoxResult.Ok Then
            Resume TRY_AGAIN
        End If
    End Function

    Public Sub CopySameAttsFromBlkToBlk(ByVal vAtts1 As Object, ByRef vAtts2() As Object)

        Dim l As Integer
        Dim m As Integer
        '''''''''''''''''''''''''''''''''''''''
        For m = LBound(vAtts1) To UBound(vAtts1)
            For l = LBound(vAtts2) To UBound(vAtts2)
                'UPGRADE_WARNING: Couldn't resolve default property of object vAtts2(l).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                'UPGRADE_WARNING: Couldn't resolve default property of object vAtts1(m).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If vAtts1(m).TagString = vAtts2(l).TagString Then
                    'UPGRADE_WARNING: Couldn't resolve default property of object vAtts2(l).textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object vAtts1().textString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    vAtts2(l).TextString = vAtts1(m).textString
                End If
            Next l
        Next m
    End Sub
End Module