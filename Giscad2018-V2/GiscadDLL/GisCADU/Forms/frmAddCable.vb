Imports System.Windows.Forms
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports ADODB


Public Class frmAddCable

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private Sub CommandButton1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton1.Click
        Me.Close()
    End Sub

    Private Sub CommandButton2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton2.Click
        Dim layerObj As AcadLayer

        If Len(ListBox1.Text) > 0 Then
            adb.RecordSource = "select * from tblConductors where ConName = '" & ListBox1.Text & "'"
            adb.Refresh()
            With adb.Recordset
                Do While Not .EOF

                    ' Add the layer to the layers collection
                    layerObj = ThisDrawing.Layers.Add(Trim(adb.Recordset.Fields("ConLayer").Value & ""))

                    ' Make the new layer the active layer for the drawing
                    ThisDrawing.ActiveLayer = layerObj
                    ThisDrawing.SendCommand("_Pline" & vbCr)
                    .MoveNext()
                Loop
            End With
            Me.Close()
        End If
    End Sub

    Private Sub optHV_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles optHV.CheckedChanged
        LoadCableList("HV")
    End Sub

    Private Sub optLV_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles optLV.CheckedChanged
        LoadCableList("LV")
    End Sub

    Private Sub optSL_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles optSL.CheckedChanged
        LoadCableList("SL")
    End Sub

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()
        LoadCableList("")
        ' Add any initialization after the InitializeComponent() call.

    End Sub

    Sub LoadCableList(ByVal usage As String)

        ListBox1.Items.Clear()
        adb.RecordSource = "select * from tblConductors"
        If Len(usage) > 0 Then
            adb.RecordSource = adb.RecordSource & " where ConLayer like 'Mains_" & usage & "%'"
        End If

        adb.Refresh()
        With adb.Recordset
            Do While Not .EOF
                ListBox1.Items.Add(adb.Recordset.Fields("ConName").Value & "")
                .MoveNext()
            Loop
        End With

    End Sub
End Class