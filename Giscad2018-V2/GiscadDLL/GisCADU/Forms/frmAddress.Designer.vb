<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmAddress
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
        Me.txtboxHelp = New System.Windows.Forms.TextBox()
        Me.txtSuburb = New System.Windows.Forms.TextBox()
        Me.txtStreetName = New System.Windows.Forms.TextBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.CommandButton1 = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.cmdOK = New System.Windows.Forms.Button()
        Me.cmdExit = New System.Windows.Forms.Button()
        Me.cmdHelp = New System.Windows.Forms.Button()
        Me.ListView1 = New System.Windows.Forms.ListView()
        Me.ColumnHeader1 = CType(New System.Windows.Forms.ColumnHeader(), System.Windows.Forms.ColumnHeader)
        Me.SuspendLayout()
        '
        'txtboxHelp
        '
        Me.txtboxHelp.Location = New System.Drawing.Point(332, 12)
        Me.txtboxHelp.Multiline = True
        Me.txtboxHelp.Name = "txtboxHelp"
        Me.txtboxHelp.Size = New System.Drawing.Size(243, 341)
        Me.txtboxHelp.TabIndex = 1
        '
        'txtSuburb
        '
        Me.txtSuburb.Location = New System.Drawing.Point(58, 237)
        Me.txtSuburb.Name = "txtSuburb"
        Me.txtSuburb.Size = New System.Drawing.Size(268, 20)
        Me.txtSuburb.TabIndex = 2
        '
        'txtStreetName
        '
        Me.txtStreetName.Location = New System.Drawing.Point(58, 270)
        Me.txtStreetName.Name = "txtStreetName"
        Me.txtStreetName.Size = New System.Drawing.Size(268, 20)
        Me.txtStreetName.TabIndex = 3
        '
        'Label2
        '
        Me.Label2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.Label2.Location = New System.Drawing.Point(58, 300)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(34, 23)
        Me.Label2.TabIndex = 4
        Me.Label2.Text = "0"
        Me.Label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        '
        'CommandButton1
        '
        Me.CommandButton1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CommandButton1.Location = New System.Drawing.Point(98, 300)
        Me.CommandButton1.Name = "CommandButton1"
        Me.CommandButton1.Size = New System.Drawing.Size(42, 23)
        Me.CommandButton1.TabIndex = 5
        Me.CommandButton1.Text = "..."
        Me.CommandButton1.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(5, 241)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(47, 13)
        Me.Label1.TabIndex = 6
        Me.Label1.Text = "Suburb :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(5, 272)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(41, 13)
        Me.Label3.TabIndex = 7
        Me.Label3.Text = "Street :"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(7, 304)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(37, 13)
        Me.Label4.TabIndex = 8
        Me.Label4.Text = "Total :"
        '
        'cmdOK
        '
        Me.cmdOK.Enabled = False
        Me.cmdOK.Location = New System.Drawing.Point(58, 329)
        Me.cmdOK.Name = "cmdOK"
        Me.cmdOK.Size = New System.Drawing.Size(82, 23)
        Me.cmdOK.TabIndex = 9
        Me.cmdOK.Text = "OK"
        Me.cmdOK.UseVisualStyleBackColor = True
        '
        'cmdExit
        '
        Me.cmdExit.Location = New System.Drawing.Point(151, 329)
        Me.cmdExit.Name = "cmdExit"
        Me.cmdExit.Size = New System.Drawing.Size(82, 23)
        Me.cmdExit.TabIndex = 10
        Me.cmdExit.Text = "E&xit"
        Me.cmdExit.UseVisualStyleBackColor = True
        '
        'cmdHelp
        '
        Me.cmdHelp.Location = New System.Drawing.Point(244, 329)
        Me.cmdHelp.Name = "cmdHelp"
        Me.cmdHelp.Size = New System.Drawing.Size(82, 23)
        Me.cmdHelp.TabIndex = 11
        Me.cmdHelp.Text = "Help >"
        Me.cmdHelp.UseVisualStyleBackColor = True
        '
        'ListView1
        '
        Me.ListView1.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.ColumnHeader1})
        Me.ListView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None
        Me.ListView1.Location = New System.Drawing.Point(58, 12)
        Me.ListView1.MultiSelect = False
        Me.ListView1.Name = "ListView1"
        Me.ListView1.Size = New System.Drawing.Size(268, 219)
        Me.ListView1.Sorting = System.Windows.Forms.SortOrder.Ascending
        Me.ListView1.TabIndex = 12
        Me.ListView1.UseCompatibleStateImageBehavior = False
        Me.ListView1.View = System.Windows.Forms.View.Details
        '
        'ColumnHeader1
        '
        Me.ColumnHeader1.Text = ""
        Me.ColumnHeader1.Width = 230
        '
        'frmAddress
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.ClientSize = New System.Drawing.Size(584, 361)
        Me.Controls.Add(Me.ListView1)
        Me.Controls.Add(Me.cmdHelp)
        Me.Controls.Add(Me.cmdExit)
        Me.Controls.Add(Me.cmdOK)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.CommandButton1)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.txtStreetName)
        Me.Controls.Add(Me.txtSuburb)
        Me.Controls.Add(Me.txtboxHelp)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmAddress"
        Me.Text = "Set Address v4.0"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents txtboxHelp As System.Windows.Forms.TextBox
    Friend WithEvents txtSuburb As System.Windows.Forms.TextBox
    Friend WithEvents txtStreetName As System.Windows.Forms.TextBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents CommandButton1 As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents cmdOK As System.Windows.Forms.Button
    Friend WithEvents cmdExit As System.Windows.Forms.Button
    Friend WithEvents cmdHelp As System.Windows.Forms.Button
    Friend WithEvents ListView1 As System.Windows.Forms.ListView
    Friend WithEvents ColumnHeader1 As System.Windows.Forms.ColumnHeader
End Class
