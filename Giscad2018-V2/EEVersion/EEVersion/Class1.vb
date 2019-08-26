Imports Autodesk.AutoCAD.ApplicationServices
Imports Autodesk.AutoCAD.DatabaseServices

Public Class EEversionCommand

    <Autodesk.AutoCAD.Runtime.CommandMethod("EEver")> _
    Public Sub EEver()
        Try
            Dim doc As Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument
            Dim hs As HostApplicationServices = HostApplicationServices.Current
            Dim path As String = hs.FindFile(doc.Name, doc.Database, FindFileHint.Default)
            hs.Dispose()
            If path.EndsWith(".dwt", StringComparison.InvariantCultureIgnoreCase) Then
                Dim frm As New EEversion
                frm.ShowDialog()
            Else
                MsgBox("Drawing is not Template(.dwt) File", MsgBoxStyle.OkOnly, "Error Dialog")
            End If

        Catch ex As Exception
            MsgBox("Drawing is not Template(.dwt) File", MsgBoxStyle.OkOnly, "Error Dialog")
        End Try


    End Sub

End Class
