Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Public Class frmTitleBlk

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)

            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Dim i As Integer
        Dim tblk As AcadBlock


        ' Find the title block
        For i = 0 To ThisDrawing.Blocks.Count - 1
            Debug.Print(ThisDrawing.Blocks(i).Name)
            If ThisDrawing.Blocks.Item(i).Name.Substring(ThisDrawing.Blocks.Item(i).Name.Length - Len("_FRAME_TITLE"), Len("_FRAME_TITLE")) = "_FRAME_TITLE" Then
                'If Right(ThisDrawing.Blocks(i).Name, Len("_FRAME_TITLE")) = "_FRAME_TITLE" Then
                tblk = ThisDrawing.Blocks(i)
            End If
        Next i

        If IsNothing(tblk) Then
            MsgBox("No title block object found")
        Else
            MsgBox(tblk.Name)
        End If
    End Sub
End Class