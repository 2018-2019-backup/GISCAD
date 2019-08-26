Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports System.Collections.Generic

Public Class frmGISErrors

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            ' Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private Sub cmdExit_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdExit.Click
        Me.Close()
    End Sub

    Private Sub cmdZoomTo_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdZoomTo.Click
        If Len(lstErrors.Text) <> 0 Then
            ZoomTo()
        Else
            MsgBox("No selection made", vbInformation + vbOKOnly, "GISCAD")
        End If
    End Sub

    Sub ZoomTo()
        Dim sel As String
        Dim coords As Object
        Dim coordsy As Object
        Dim xy As Object
        Dim point1(0 To 2) As Double
        Dim point2(0 To 2) As Double

        xy = Split(lstErrors.Text, "#")
        coords = Split(xy(1), ",")
        coordsy = Split(coords(1), "(")

        point1(0) = coords(0) - 5 : point1(1) = coordsy(0) - 5 : point1(2) = 0
        point2(0) = coords(0) + 5 : point2(1) = coordsy(0) + 5 : point2(2) = 0
        ThisDrawing.Application.ZoomWindow(point1, point2)
    End Sub

    Private Sub lstErrors_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles lstErrors.DoubleClick
        If Len(lstErrors.Text) <> 0 Then
            ZoomTo()
        Else
            MsgBox("No selection made", vbInformation + vbOKOnly, "GISCAD")
        End If
    End Sub
    ''' <summary>
    ''' Populate Error symbol data in the dialog box if the Error block tag(Err_type) value = 1 
    ''' Err_Type value has been configured as ErrorFlag in settings.ini file as P1, P2, P3, P4
    ''' </summary>
    ''' <remarks></remarks>
    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Dim blk As AcadBlockReference
        Dim varAttributes() As Object
        Dim i As Integer, j As Integer, k As Integer
        Dim _blkName As String = String.Empty
        Dim dtab, dtab1 As New System.Data.DataTable
        dtab.Columns.Add("dummy")
        lstErrors.Items.Clear()
        ChkP1.Checked = False
        ChkP2.Checked = False
        ChkP3.Checked = False
        ChkP4.Checked = False
        ChkP1.Enabled = True
        ChkP2.Enabled = True
        ChkP3.Enabled = True
        ChkP4.Enabled = True

        ''Read Inifile Errorflag keys
        Dim sPath As String
        Dim nIndex As Integer
        sPath = Trim(System.Reflection.Assembly.GetExecutingAssembly().Location)
        nIndex = sPath.LastIndexOf("\")
        sPath = sPath.Substring(0, nIndex)
        If System.IO.File.Exists(sPath & "\settings.ini") Then
            SetInifile(sPath & "\settings.ini")
        End If
        Dim objIni As New Ini.IniFile(gInifile)
        Dim tempcol As System.Collections.Generic.List(Of String)
        tempcol = objIni.IniGetKeys("ERRORFLAG")
        Dim temp As New Dictionary(Of String, String)
        Dim str As String = String.Empty
        temp.Clear()
        For Each ts As String In tempcol
            str = objIni.IniReadValue("ERRORFLAG", ts)
            temp.Add(ts, str)
        Next
        Dim list As New List(Of String)(temp.Keys)
        Cursor.Current = Cursors.WaitCursor

        ''Populate Error symbol data in the dialog box if the Error block tag(Err_type) value = 1 
        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            'Debug.Print(ThisDrawing.ModelSpace(i).ObjectName)
            If ThisDrawing.ModelSpace(i).ObjectName = "AcDbBlockReference" Then
                blk = ThisDrawing.ModelSpace(i)
                'Debug.Print(blk.Name)
                _blkName = GetBlockName(blk)
                If Len(_blkName) >= 10 Then
                    If (UCase$(_blkName).Substring(0, Len("Err_Circle")) = "ERR_CIRCLE") Then
                        varAttributes = blk.GetAttributes
                        For k = LBound(varAttributes) To UBound(varAttributes)
                            If varAttributes(k).TagString = "ERR_TYPE" Then
                                For Each dicstr As String In list
                                    If temp.Item(dicstr).Trim.ToString = 1 Then
                                        If dicstr = "P1" Then
                                            ChkP1.Checked = True
                                        End If
                                        If dicstr = "P2" Then
                                            ChkP2.Checked = True
                                        End If
                                        If dicstr = "P3" Then
                                            ChkP3.Checked = True
                                        End If
                                        If dicstr = "P4" Then
                                            ChkP4.Checked = True
                                        End If
                                        If (varAttributes(k).TextString = dicstr) Then
                                            For j = LBound(varAttributes) To UBound(varAttributes)
                                                'Debug.Print(varAttributes(j).TagString)
                                                If varAttributes(j).TagString = "ERR_NUMBER" Then
                                                    dtab.Rows.Add(varAttributes(j).TextString & "#" & blk.InsertionPoint(0) & "," & blk.InsertionPoint(1) & " (" & dicstr & ")")
                                                    Exit For
                                                    'lstErrors.Items.Add(varAttributes(j).TextString & "-" & blk.InsertionPoint(0) & "," & blk.InsertionPoint(1))
                                                End If
                                            Next j
                                        End If
                                    End If
                                Next
                            End If
                        Next
                    End If
                End If
            End If
        Next i
        Cursor.Current = Cursors.WaitCursor
        If ChkP1.Checked = False Then
            ChkP1.Enabled = False
        End If
        If ChkP2.Checked = False Then
            ChkP2.Enabled = False
        End If
        If ChkP3.Checked = False Then
            ChkP3.Enabled = False
        End If
        If ChkP4.Checked = False Then
            ChkP4.Enabled = False
        End If

        'Listing the values in listbox
        dtab1 = dtab.DefaultView.ToTable(True)
        For l As Integer = 0 To dtab1.Rows.Count - 1
            lstErrors.Items.Add(dtab1.Rows(l)(0).ToString)
        Next
    End Sub

    Private Sub ChkP1_CheckStateChanged(sender As Object, e As System.EventArgs) Handles ChkP1.CheckStateChanged
        Dim mychk As CheckBox = DirectCast(sender, CheckBox)
        If mychk.Checked = False Then
            ChkP1.Checked = True
        End If
    End Sub

    Private Sub ChkP2_CheckStateChanged(sender As Object, e As System.EventArgs) Handles ChkP2.CheckStateChanged
        Dim mychk As CheckBox = DirectCast(sender, CheckBox)
        If mychk.Checked = False Then
            ChkP2.Checked = True
        End If
    End Sub

    Private Sub ChkP3_CheckStateChanged(sender As Object, e As System.EventArgs) Handles ChkP3.CheckStateChanged
        Dim mychk As CheckBox = DirectCast(sender, CheckBox)
        If mychk.Checked = False Then
            ChkP3.Checked = True
        End If
    End Sub

    Private Sub ChkP4_CheckStateChanged(sender As Object, e As System.EventArgs) Handles ChkP4.CheckStateChanged
        Dim mychk As CheckBox = DirectCast(sender, CheckBox)
        If mychk.Checked = False Then
            ChkP4.Checked = True
        End If
    End Sub
End Class