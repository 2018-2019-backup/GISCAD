<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmCCT
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmCCT))
        Me.chkHVCCT = New System.Windows.Forms.CheckBox()
        Me.chkLVCCT = New System.Windows.Forms.CheckBox()
        Me.chkSLCCT = New System.Windows.Forms.CheckBox()
        Me.cmdOK = New System.Windows.Forms.Button()
        Me.cmdCancel = New System.Windows.Forms.Button()
        Me.lstCCT = New System.Windows.Forms.ListBox()
        Me.adb = New MSAdodcLib.Adodc()
        CType(Me.adb, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'chkHVCCT
        '
        Me.chkHVCCT.AutoSize = True
        Me.chkHVCCT.Location = New System.Drawing.Point(12, 12)
        Me.chkHVCCT.Name = "chkHVCCT"
        Me.chkHVCCT.Size = New System.Drawing.Size(98, 17)
        Me.chkHVCCT.TabIndex = 0
        Me.chkHVCCT.Text = "Final HV Circuit"
        Me.chkHVCCT.UseVisualStyleBackColor = True
        '
        'chkLVCCT
        '
        Me.chkLVCCT.AutoSize = True
        Me.chkLVCCT.Location = New System.Drawing.Point(12, 44)
        Me.chkLVCCT.Name = "chkLVCCT"
        Me.chkLVCCT.Size = New System.Drawing.Size(96, 17)
        Me.chkLVCCT.TabIndex = 1
        Me.chkLVCCT.Text = "Final LV Circuit"
        Me.chkLVCCT.UseVisualStyleBackColor = True
        '
        'chkSLCCT
        '
        Me.chkSLCCT.AutoSize = True
        Me.chkSLCCT.Location = New System.Drawing.Point(12, 80)
        Me.chkSLCCT.Name = "chkSLCCT"
        Me.chkSLCCT.Size = New System.Drawing.Size(96, 17)
        Me.chkSLCCT.TabIndex = 2
        Me.chkSLCCT.Text = "Final SL Circuit"
        Me.chkSLCCT.UseVisualStyleBackColor = True
        '
        'cmdOK
        '
        Me.cmdOK.Location = New System.Drawing.Point(182, 8)
        Me.cmdOK.Name = "cmdOK"
        Me.cmdOK.Size = New System.Drawing.Size(98, 23)
        Me.cmdOK.TabIndex = 3
        Me.cmdOK.Text = "OK"
        Me.cmdOK.UseVisualStyleBackColor = True
        '
        'cmdCancel
        '
        Me.cmdCancel.Location = New System.Drawing.Point(182, 44)
        Me.cmdCancel.Name = "cmdCancel"
        Me.cmdCancel.Size = New System.Drawing.Size(98, 23)
        Me.cmdCancel.TabIndex = 4
        Me.cmdCancel.Text = "Cancel"
        Me.cmdCancel.UseVisualStyleBackColor = True
        '
        'lstCCT
        '
        Me.lstCCT.FormattingEnabled = True
        Me.lstCCT.Location = New System.Drawing.Point(12, 112)
        Me.lstCCT.Name = "lstCCT"
        Me.lstCCT.Size = New System.Drawing.Size(268, 95)
        Me.lstCCT.TabIndex = 5
        '
        'adb
        '
        Me.adb.Location = New System.Drawing.Point(149, 80)
        Me.adb.Name = "adb"
        Me.adb.OcxState = CType(resources.GetObject("adb.OcxState"), System.Windows.Forms.AxHost.State)
        Me.adb.Size = New System.Drawing.Size(131, 23)
        Me.adb.TabIndex = 6
        Me.adb.Visible = False
        '
        'frmCCT
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(292, 222)
        Me.Controls.Add(Me.adb)
        Me.Controls.Add(Me.lstCCT)
        Me.Controls.Add(Me.cmdCancel)
        Me.Controls.Add(Me.cmdOK)
        Me.Controls.Add(Me.chkSLCCT)
        Me.Controls.Add(Me.chkLVCCT)
        Me.Controls.Add(Me.chkHVCCT)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmCCT"
        Me.Text = "Generate CCT Legend 3.1"
        CType(Me.adb, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents chkHVCCT As System.Windows.Forms.CheckBox
    Friend WithEvents chkLVCCT As System.Windows.Forms.CheckBox
    Friend WithEvents chkSLCCT As System.Windows.Forms.CheckBox
    Friend WithEvents cmdOK As System.Windows.Forms.Button
    Friend WithEvents cmdCancel As System.Windows.Forms.Button
    Friend WithEvents lstCCT As System.Windows.Forms.ListBox
    Friend WithEvents adb As MSAdodcLib.Adodc
End Class
