<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmGISErrors
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.lstErrors = New System.Windows.Forms.ListBox()
        Me.cmdExit = New System.Windows.Forms.Button()
        Me.cmdZoomTo = New System.Windows.Forms.Button()
        Me.ChkP1 = New System.Windows.Forms.CheckBox()
        Me.ChkP4 = New System.Windows.Forms.CheckBox()
        Me.ChkP3 = New System.Windows.Forms.CheckBox()
        Me.ChkP2 = New System.Windows.Forms.CheckBox()
        Me.SuspendLayout()
        '
        'lstErrors
        '
        Me.lstErrors.FormattingEnabled = True
        Me.lstErrors.Location = New System.Drawing.Point(7, 7)
        Me.lstErrors.Name = "lstErrors"
        Me.lstErrors.Size = New System.Drawing.Size(357, 511)
        Me.lstErrors.TabIndex = 0
        '
        'cmdExit
        '
        Me.cmdExit.Location = New System.Drawing.Point(289, 543)
        Me.cmdExit.Name = "cmdExit"
        Me.cmdExit.Size = New System.Drawing.Size(75, 23)
        Me.cmdExit.TabIndex = 1
        Me.cmdExit.Text = "Exit"
        Me.cmdExit.UseVisualStyleBackColor = True
        '
        'cmdZoomTo
        '
        Me.cmdZoomTo.Location = New System.Drawing.Point(208, 543)
        Me.cmdZoomTo.Name = "cmdZoomTo"
        Me.cmdZoomTo.Size = New System.Drawing.Size(75, 23)
        Me.cmdZoomTo.TabIndex = 2
        Me.cmdZoomTo.Text = "Zoom To"
        Me.cmdZoomTo.UseVisualStyleBackColor = True
        '
        'ChkP1
        '
        Me.ChkP1.AutoSize = True
        Me.ChkP1.Location = New System.Drawing.Point(7, 543)
        Me.ChkP1.Name = "ChkP1"
        Me.ChkP1.Size = New System.Drawing.Size(39, 17)
        Me.ChkP1.TabIndex = 3
        Me.ChkP1.Text = "P1"
        Me.ChkP1.UseVisualStyleBackColor = True
        '
        'ChkP4
        '
        Me.ChkP4.AutoSize = True
        Me.ChkP4.Location = New System.Drawing.Point(142, 543)
        Me.ChkP4.Name = "ChkP4"
        Me.ChkP4.Size = New System.Drawing.Size(39, 17)
        Me.ChkP4.TabIndex = 4
        Me.ChkP4.Text = "P4"
        Me.ChkP4.UseVisualStyleBackColor = True
        '
        'ChkP3
        '
        Me.ChkP3.AutoSize = True
        Me.ChkP3.Location = New System.Drawing.Point(97, 543)
        Me.ChkP3.Name = "ChkP3"
        Me.ChkP3.Size = New System.Drawing.Size(39, 17)
        Me.ChkP3.TabIndex = 5
        Me.ChkP3.Text = "P3"
        Me.ChkP3.UseVisualStyleBackColor = True
        '
        'ChkP2
        '
        Me.ChkP2.AutoSize = True
        Me.ChkP2.Location = New System.Drawing.Point(52, 543)
        Me.ChkP2.Name = "ChkP2"
        Me.ChkP2.Size = New System.Drawing.Size(39, 17)
        Me.ChkP2.TabIndex = 6
        Me.ChkP2.Text = "P2"
        Me.ChkP2.UseVisualStyleBackColor = True
        '
        'frmGISErrors
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(372, 584)
        Me.Controls.Add(Me.ChkP2)
        Me.Controls.Add(Me.ChkP3)
        Me.Controls.Add(Me.ChkP4)
        Me.Controls.Add(Me.ChkP1)
        Me.Controls.Add(Me.cmdZoomTo)
        Me.Controls.Add(Me.cmdExit)
        Me.Controls.Add(Me.lstErrors)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmGISErrors"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "GIS Errors V4.0"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents lstErrors As System.Windows.Forms.ListBox
    Friend WithEvents cmdExit As System.Windows.Forms.Button
    Friend WithEvents cmdZoomTo As System.Windows.Forms.Button
    Friend WithEvents ChkP1 As System.Windows.Forms.CheckBox
    Friend WithEvents ChkP4 As System.Windows.Forms.CheckBox
    Friend WithEvents ChkP3 As System.Windows.Forms.CheckBox
    Friend WithEvents ChkP2 As System.Windows.Forms.CheckBox
End Class
