<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmAddCable
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmAddCable))
        Me.ListBox1 = New System.Windows.Forms.ListBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.optService = New System.Windows.Forms.RadioButton()
        Me.optSL = New System.Windows.Forms.RadioButton()
        Me.optLV = New System.Windows.Forms.RadioButton()
        Me.optHV = New System.Windows.Forms.RadioButton()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.OptionButton2 = New System.Windows.Forms.RadioButton()
        Me.OptionButton1 = New System.Windows.Forms.RadioButton()
        Me.CommandButton2 = New System.Windows.Forms.Button()
        Me.CommandButton1 = New System.Windows.Forms.Button()
        Me.adb = New MSAdodcLib.Adodc()
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        CType(Me.adb, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'ListBox1
        '
        Me.ListBox1.FormattingEnabled = True
        Me.ListBox1.Location = New System.Drawing.Point(7, 28)
        Me.ListBox1.Name = "ListBox1"
        Me.ListBox1.Size = New System.Drawing.Size(383, 173)
        Me.ListBox1.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(4, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(97, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Select Cable Type:"
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.optService)
        Me.GroupBox1.Controls.Add(Me.optSL)
        Me.GroupBox1.Controls.Add(Me.optLV)
        Me.GroupBox1.Controls.Add(Me.optHV)
        Me.GroupBox1.Location = New System.Drawing.Point(7, 211)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(383, 50)
        Me.GroupBox1.TabIndex = 2
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Usage"
        '
        'optService
        '
        Me.optService.AutoSize = True
        Me.optService.Location = New System.Drawing.Point(300, 19)
        Me.optService.Name = "optService"
        Me.optService.Size = New System.Drawing.Size(61, 17)
        Me.optService.TabIndex = 6
        Me.optService.TabStop = True
        Me.optService.Text = "Service"
        Me.optService.UseVisualStyleBackColor = True
        '
        'optSL
        '
        Me.optSL.AutoSize = True
        Me.optSL.Location = New System.Drawing.Point(221, 19)
        Me.optSL.Name = "optSL"
        Me.optSL.Size = New System.Drawing.Size(38, 17)
        Me.optSL.TabIndex = 5
        Me.optSL.TabStop = True
        Me.optSL.Text = "SL"
        Me.optSL.UseVisualStyleBackColor = True
        '
        'optLV
        '
        Me.optLV.AutoSize = True
        Me.optLV.Location = New System.Drawing.Point(142, 19)
        Me.optLV.Name = "optLV"
        Me.optLV.Size = New System.Drawing.Size(38, 17)
        Me.optLV.TabIndex = 4
        Me.optLV.TabStop = True
        Me.optLV.Text = "LV"
        Me.optLV.UseVisualStyleBackColor = True
        '
        'optHV
        '
        Me.optHV.AutoSize = True
        Me.optHV.Location = New System.Drawing.Point(61, 19)
        Me.optHV.Name = "optHV"
        Me.optHV.Size = New System.Drawing.Size(40, 17)
        Me.optHV.TabIndex = 3
        Me.optHV.TabStop = True
        Me.optHV.Text = "HV"
        Me.optHV.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.OptionButton2)
        Me.GroupBox2.Controls.Add(Me.OptionButton1)
        Me.GroupBox2.Location = New System.Drawing.Point(7, 268)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(383, 47)
        Me.GroupBox2.TabIndex = 3
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Orientation"
        '
        'OptionButton2
        '
        Me.OptionButton2.AutoSize = True
        Me.OptionButton2.Location = New System.Drawing.Point(137, 19)
        Me.OptionButton2.Name = "OptionButton2"
        Me.OptionButton2.Size = New System.Drawing.Size(41, 17)
        Me.OptionButton2.TabIndex = 5
        Me.OptionButton2.TabStop = True
        Me.OptionButton2.Text = "UG"
        Me.OptionButton2.UseVisualStyleBackColor = True
        '
        'OptionButton1
        '
        Me.OptionButton1.AutoSize = True
        Me.OptionButton1.Location = New System.Drawing.Point(56, 19)
        Me.OptionButton1.Name = "OptionButton1"
        Me.OptionButton1.Size = New System.Drawing.Size(41, 17)
        Me.OptionButton1.TabIndex = 4
        Me.OptionButton1.TabStop = True
        Me.OptionButton1.Text = "OH"
        Me.OptionButton1.UseVisualStyleBackColor = True
        '
        'CommandButton2
        '
        Me.CommandButton2.Location = New System.Drawing.Point(228, 325)
        Me.CommandButton2.Name = "CommandButton2"
        Me.CommandButton2.Size = New System.Drawing.Size(75, 23)
        Me.CommandButton2.TabIndex = 4
        Me.CommandButton2.Text = "OK"
        Me.CommandButton2.UseVisualStyleBackColor = True
        '
        'CommandButton1
        '
        Me.CommandButton1.Location = New System.Drawing.Point(315, 325)
        Me.CommandButton1.Name = "CommandButton1"
        Me.CommandButton1.Size = New System.Drawing.Size(75, 23)
        Me.CommandButton1.TabIndex = 5
        Me.CommandButton1.Text = "Cancel"
        Me.CommandButton1.UseVisualStyleBackColor = True
        '
        'adb
        '
        Me.adb.Location = New System.Drawing.Point(12, 325)
        Me.adb.Name = "adb"
        Me.adb.OcxState = CType(resources.GetObject("adb.OcxState"), System.Windows.Forms.AxHost.State)
        Me.adb.Size = New System.Drawing.Size(142, 23)
        Me.adb.TabIndex = 8
        Me.adb.Visible = False
        '
        'frmAddCable
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(399, 355)
        Me.Controls.Add(Me.adb)
        Me.Controls.Add(Me.CommandButton1)
        Me.Controls.Add(Me.CommandButton2)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ListBox1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmAddCable"
        Me.Text = "Add Mains 3.1"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        CType(Me.adb, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents ListBox1 As System.Windows.Forms.ListBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents optService As System.Windows.Forms.RadioButton
    Friend WithEvents optSL As System.Windows.Forms.RadioButton
    Friend WithEvents optLV As System.Windows.Forms.RadioButton
    Friend WithEvents optHV As System.Windows.Forms.RadioButton
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents OptionButton2 As System.Windows.Forms.RadioButton
    Friend WithEvents OptionButton1 As System.Windows.Forms.RadioButton
    Friend WithEvents CommandButton2 As System.Windows.Forms.Button
    Friend WithEvents CommandButton1 As System.Windows.Forms.Button
    Friend WithEvents adb As MSAdodcLib.Adodc
End Class
