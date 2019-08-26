<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmAttrs
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmAttrs))
        Me.lstBlocks = New System.Windows.Forms.ListBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.CommandButton1 = New System.Windows.Forms.Button()
        Me.CommandButton2 = New System.Windows.Forms.Button()
        Me.cmdOK = New System.Windows.Forms.Button()
        Me.CommandButton3 = New System.Windows.Forms.Button()
        Me.CommandButton4 = New System.Windows.Forms.Button()
        Me.CommandButton5 = New System.Windows.Forms.Button()
        Me.cmdExit = New System.Windows.Forms.Button()
        Me.CommandButton6 = New System.Windows.Forms.Button()
        Me.adb = New MSAdodcLib.Adodc()
        CType(Me.adb, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'lstBlocks
        '
        Me.lstBlocks.FormattingEnabled = True
        Me.lstBlocks.Location = New System.Drawing.Point(12, 12)
        Me.lstBlocks.Name = "lstBlocks"
        Me.lstBlocks.Size = New System.Drawing.Size(484, 238)
        Me.lstBlocks.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(12, 266)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(39, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Label1"
        '
        'CommandButton1
        '
        Me.CommandButton1.Location = New System.Drawing.Point(502, 12)
        Me.CommandButton1.Name = "CommandButton1"
        Me.CommandButton1.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton1.TabIndex = 2
        Me.CommandButton1.Text = "Load Blocks"
        Me.CommandButton1.UseVisualStyleBackColor = True
        '
        'CommandButton2
        '
        Me.CommandButton2.Location = New System.Drawing.Point(502, 45)
        Me.CommandButton2.Name = "CommandButton2"
        Me.CommandButton2.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton2.TabIndex = 3
        Me.CommandButton2.Text = "Select All"
        Me.CommandButton2.UseVisualStyleBackColor = True
        '
        'cmdOK
        '
        Me.cmdOK.Location = New System.Drawing.Point(502, 78)
        Me.cmdOK.Name = "cmdOK"
        Me.cmdOK.Size = New System.Drawing.Size(105, 23)
        Me.cmdOK.TabIndex = 4
        Me.cmdOK.Text = "Add Attrs"
        Me.cmdOK.UseVisualStyleBackColor = True
        '
        'CommandButton3
        '
        Me.CommandButton3.Location = New System.Drawing.Point(502, 111)
        Me.CommandButton3.Name = "CommandButton3"
        Me.CommandButton3.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton3.TabIndex = 5
        Me.CommandButton3.Text = "Place Blocks"
        Me.CommandButton3.UseVisualStyleBackColor = True
        '
        'CommandButton4
        '
        Me.CommandButton4.Location = New System.Drawing.Point(502, 144)
        Me.CommandButton4.Name = "CommandButton4"
        Me.CommandButton4.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton4.TabIndex = 6
        Me.CommandButton4.Text = "Refresh Blocks"
        Me.CommandButton4.UseVisualStyleBackColor = True
        '
        'CommandButton5
        '
        Me.CommandButton5.Location = New System.Drawing.Point(502, 177)
        Me.CommandButton5.Name = "CommandButton5"
        Me.CommandButton5.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton5.TabIndex = 7
        Me.CommandButton5.Text = "Del Attrs"
        Me.CommandButton5.UseVisualStyleBackColor = True
        '
        'cmdExit
        '
        Me.cmdExit.Location = New System.Drawing.Point(502, 227)
        Me.cmdExit.Name = "cmdExit"
        Me.cmdExit.Size = New System.Drawing.Size(105, 23)
        Me.cmdExit.TabIndex = 8
        Me.cmdExit.Text = "Exit"
        Me.cmdExit.UseVisualStyleBackColor = True
        '
        'CommandButton6
        '
        Me.CommandButton6.Location = New System.Drawing.Point(391, 261)
        Me.CommandButton6.Name = "CommandButton6"
        Me.CommandButton6.Size = New System.Drawing.Size(105, 23)
        Me.CommandButton6.TabIndex = 9
        Me.CommandButton6.Text = "CommandButton6"
        Me.CommandButton6.UseVisualStyleBackColor = True
        '
        'adb
        '
        Me.adb.Location = New System.Drawing.Point(237, 261)
        Me.adb.Name = "adb"
        Me.adb.OcxState = CType(resources.GetObject("adb.OcxState"), System.Windows.Forms.AxHost.State)
        Me.adb.Size = New System.Drawing.Size(133, 23)
        Me.adb.TabIndex = 10
        Me.adb.Visible = False
        '
        'frmAttrs
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(618, 303)
        Me.Controls.Add(Me.adb)
        Me.Controls.Add(Me.CommandButton6)
        Me.Controls.Add(Me.cmdExit)
        Me.Controls.Add(Me.CommandButton5)
        Me.Controls.Add(Me.CommandButton4)
        Me.Controls.Add(Me.CommandButton3)
        Me.Controls.Add(Me.cmdOK)
        Me.Controls.Add(Me.CommandButton2)
        Me.Controls.Add(Me.CommandButton1)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.lstBlocks)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmAttrs"
        Me.Text = "Attach Block Attributes 3.1"
        CType(Me.adb, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents lstBlocks As System.Windows.Forms.ListBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents CommandButton1 As System.Windows.Forms.Button
    Friend WithEvents CommandButton2 As System.Windows.Forms.Button
    Friend WithEvents cmdOK As System.Windows.Forms.Button
    Friend WithEvents CommandButton3 As System.Windows.Forms.Button
    Friend WithEvents CommandButton4 As System.Windows.Forms.Button
    Friend WithEvents CommandButton5 As System.Windows.Forms.Button
    Friend WithEvents cmdExit As System.Windows.Forms.Button
    Friend WithEvents CommandButton6 As System.Windows.Forms.Button
    Friend WithEvents adb As MSAdodcLib.Adodc
End Class
