Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Public Class frmAttrStyle
    Private bAction As Boolean
    Public sTextStyle As String
    Public dTextHeight As Double

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Public Sub SetAttrTag(ByVal sAttrTag As String)
        ' Update Form Caption
        TextBox2.Text = sAttrTag
    End Sub

    Public Sub New()
        Dim i As Integer
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        ' Load Text Styles
        For i = 0 To ThisDrawing.TextStyles.Count - 1
            Debug.Print(ThisDrawing.TextStyles(i).Name)
            If Len(ThisDrawing.TextStyles(i).Name) > 0 Then
                cmbStyle.Items.Add(ThisDrawing.TextStyles(i).Name)
            End If
        Next i
    End Sub

    Private Sub cmdOK_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdOK.Click
        sTextStyle = cmbStyle.Text
        dTextHeight = Val(txtHeight.Text)
        bAction = True
        Me.Hide()
    End Sub

    Private Sub cmdExit_ClientSizeChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdExit.ClientSizeChanged
        Me.Hide()
        bAction = False
    End Sub

    Public Function DoModal() As Boolean
        Me.ShowDialog()
    End Function

    Private Sub cmdExit_Click(sender As System.Object, e As System.EventArgs) Handles cmdExit.Click
        Me.Close()
    End Sub
End Class