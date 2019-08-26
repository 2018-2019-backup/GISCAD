<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmAssetNums
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
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.txtDistrict = New System.Windows.Forms.TextBox()
        Me.txtPosition = New System.Windows.Forms.TextBox()
        Me.txtPassword = New System.Windows.Forms.TextBox()
        Me.txtUsername = New System.Windows.Forms.TextBox()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.cmbHost = New System.Windows.Forms.ComboBox()
        Me.cmbDepot = New System.Windows.Forms.ComboBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.cmdScan = New System.Windows.Forms.Button()
        Me.cmdGenerate = New System.Windows.Forms.Button()
        Me.cmdExit = New System.Windows.Forms.Button()
        Me.txtLog = New System.Windows.Forms.TextBox()
        Me.pb = New System.Windows.Forms.ProgressBar()
        Me.lstEquipNo = New System.Windows.Forms.ListBox()
        Me.GroupBox1.SuspendLayout()
        Me.SuspendLayout()
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.txtDistrict)
        Me.GroupBox1.Controls.Add(Me.txtPosition)
        Me.GroupBox1.Controls.Add(Me.txtPassword)
        Me.GroupBox1.Controls.Add(Me.txtUsername)
        Me.GroupBox1.Controls.Add(Me.Label5)
        Me.GroupBox1.Controls.Add(Me.Label4)
        Me.GroupBox1.Controls.Add(Me.Label3)
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.cmbHost)
        Me.GroupBox1.Location = New System.Drawing.Point(12, 12)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(307, 159)
        Me.GroupBox1.TabIndex = 0
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Ellipse Login"
        '
        'txtDistrict
        '
        Me.txtDistrict.Location = New System.Drawing.Point(119, 125)
        Me.txtDistrict.Name = "txtDistrict"
        Me.txtDistrict.ReadOnly = True
        Me.txtDistrict.Size = New System.Drawing.Size(176, 20)
        Me.txtDistrict.TabIndex = 9
        '
        'txtPosition
        '
        Me.txtPosition.Location = New System.Drawing.Point(119, 99)
        Me.txtPosition.Name = "txtPosition"
        Me.txtPosition.Size = New System.Drawing.Size(176, 20)
        Me.txtPosition.TabIndex = 8
        '
        'txtPassword
        '
        Me.txtPassword.Location = New System.Drawing.Point(119, 73)
        Me.txtPassword.Name = "txtPassword"
        Me.txtPassword.PasswordChar = Global.Microsoft.VisualBasic.ChrW(42)
        Me.txtPassword.Size = New System.Drawing.Size(176, 20)
        Me.txtPassword.TabIndex = 7
        '
        'txtUsername
        '
        Me.txtUsername.Location = New System.Drawing.Point(119, 47)
        Me.txtUsername.Name = "txtUsername"
        Me.txtUsername.Size = New System.Drawing.Size(176, 20)
        Me.txtUsername.TabIndex = 6
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(18, 129)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(55, 13)
        Me.Label5.TabIndex = 5
        Me.Label5.Text = "District :"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(18, 104)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(60, 13)
        Me.Label4.TabIndex = 4
        Me.Label4.Text = "Position :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(18, 76)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(69, 13)
        Me.Label3.TabIndex = 3
        Me.Label3.Text = "Password :"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(18, 51)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(71, 13)
        Me.Label2.TabIndex = 2
        Me.Label2.Text = "Username :"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(18, 24)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(41, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Host :"
        '
        'cmbHost
        '
        Me.cmbHost.FormattingEnabled = True
        Me.cmbHost.Location = New System.Drawing.Point(119, 19)
        Me.cmbHost.Name = "cmbHost"
        Me.cmbHost.Size = New System.Drawing.Size(176, 21)
        Me.cmbHost.TabIndex = 0
        '
        'cmbDepot
        '
        Me.cmbDepot.FormattingEnabled = True
        Me.cmbDepot.Location = New System.Drawing.Point(131, 183)
        Me.cmbDepot.Name = "cmbDepot"
        Me.cmbDepot.Size = New System.Drawing.Size(188, 21)
        Me.cmbDepot.TabIndex = 1
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.Location = New System.Drawing.Point(30, 186)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(49, 13)
        Me.Label6.TabIndex = 10
        Me.Label6.Text = "Depot :"
        '
        'cmdScan
        '
        Me.cmdScan.Location = New System.Drawing.Point(332, 19)
        Me.cmdScan.Name = "cmdScan"
        Me.cmdScan.Size = New System.Drawing.Size(107, 23)
        Me.cmdScan.TabIndex = 11
        Me.cmdScan.Text = "&Scan"
        Me.cmdScan.UseVisualStyleBackColor = True
        '
        'cmdGenerate
        '
        Me.cmdGenerate.Location = New System.Drawing.Point(332, 53)
        Me.cmdGenerate.Name = "cmdGenerate"
        Me.cmdGenerate.Size = New System.Drawing.Size(107, 23)
        Me.cmdGenerate.TabIndex = 12
        Me.cmdGenerate.Text = "&Generate Numbers"
        Me.cmdGenerate.UseVisualStyleBackColor = True
        '
        'cmdExit
        '
        Me.cmdExit.Location = New System.Drawing.Point(332, 85)
        Me.cmdExit.Name = "cmdExit"
        Me.cmdExit.Size = New System.Drawing.Size(107, 23)
        Me.cmdExit.TabIndex = 14
        Me.cmdExit.Text = "E&xit"
        Me.cmdExit.UseVisualStyleBackColor = True
        '
        'txtLog
        '
        Me.txtLog.Location = New System.Drawing.Point(12, 212)
        Me.txtLog.Multiline = True
        Me.txtLog.Name = "txtLog"
        Me.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.txtLog.Size = New System.Drawing.Size(427, 171)
        Me.txtLog.TabIndex = 15
        '
        'pb
        '
        Me.pb.Location = New System.Drawing.Point(12, 392)
        Me.pb.Name = "pb"
        Me.pb.Size = New System.Drawing.Size(427, 23)
        Me.pb.TabIndex = 16
        '
        'lstEquipNo
        '
        Me.lstEquipNo.FormattingEnabled = True
        Me.lstEquipNo.Location = New System.Drawing.Point(468, 13)
        Me.lstEquipNo.Name = "lstEquipNo"
        Me.lstEquipNo.Size = New System.Drawing.Size(318, 368)
        Me.lstEquipNo.TabIndex = 17
        '
        'frmAssetNums
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(450, 426)
        Me.Controls.Add(Me.lstEquipNo)
        Me.Controls.Add(Me.pb)
        Me.Controls.Add(Me.txtLog)
        Me.Controls.Add(Me.cmdExit)
        Me.Controls.Add(Me.cmdGenerate)
        Me.Controls.Add(Me.cmdScan)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.cmbDepot)
        Me.Controls.Add(Me.GroupBox1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmAssetNums"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Asset Numbering Utility V3.5"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents txtPosition As System.Windows.Forms.TextBox
    Friend WithEvents txtPassword As System.Windows.Forms.TextBox
    Friend WithEvents txtUsername As System.Windows.Forms.TextBox
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents cmbHost As System.Windows.Forms.ComboBox
    Friend WithEvents txtDistrict As System.Windows.Forms.TextBox
    Friend WithEvents cmbDepot As System.Windows.Forms.ComboBox
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents cmdScan As System.Windows.Forms.Button
    Friend WithEvents cmdGenerate As System.Windows.Forms.Button
    Friend WithEvents cmdExit As System.Windows.Forms.Button
    Friend WithEvents txtLog As System.Windows.Forms.TextBox
    Friend WithEvents pb As System.Windows.Forms.ProgressBar
    Friend WithEvents lstEquipNo As System.Windows.Forms.ListBox
End Class
