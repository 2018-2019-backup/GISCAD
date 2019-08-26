Public Class frmAddrHelp

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        TextBox1.Text = "This command is used to update the street name and suburb on multiple blocks." _
        & vbCrLf & vbCrLf _
        & "Select from the list, or" _
        & vbCrLf _
        & "Enter the street name and suburb in the fields provided." _
        & vbCrLf & vbCrLf _
        & "Blank values will result in the data being deleted from the selected blocks." _
        & vbCrLf & vbCrLf _
        & "Only NEW blocks will be updated."
    End Sub

    Private Sub cmdOK_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdOK.Click
        Me.Close()
    End Sub
End Class