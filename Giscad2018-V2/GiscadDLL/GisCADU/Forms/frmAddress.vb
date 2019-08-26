Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common

Public Class frmAddress
    Private bAction As Boolean
    Public Suburb As String
    Public Street As String
    Dim ssetObj As AcadSelectionSet
    Dim oListBox As ListBox

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            ' Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private Sub cmdOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdOK.Click
        Dim blk As AcadBlockReference
        Dim i As Integer

        'On Error Resume Next

        bAction = True
        Suburb = Trim(txtSuburb.Text)
        Street = Trim(txtStreetName.Text)

        'update the attributes on those blocks with theses attributes
        For i = 0 To ssetObj.Count - 1
            If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                blk = ssetObj(i)
                If GetBlockName(blk).Substring(GetBlockName(blk).Length - Len("_NEW")) = "_NEW" Then
                    SetAttrValue(blk, "SUBURB", Suburb)
                    SetAttrValue(blk, "STREETNAME", Street)
                End If
            End If
        Next i

        ThisDrawing.SelectionSets.Item("SetAddr").Delete()

        Me.Hide()
    End Sub

    Private Sub cmdHelp_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdHelp.Click
        'If Me.Width = 336 Then
        '    Me.Width = 590
        '    cmdHelp.Text = "< Help"
        'Else
        '    Me.Width = 336
        '    cmdHelp.Text = "Help >"
        'End If
        If cmdHelp.Text.EndsWith(">", StringComparison.InvariantCultureIgnoreCase) Then
            cmdHelp.Text = "< &Help"
            txtboxHelp.Visible = True
        Else
            cmdHelp.Text = "&Help >"
            txtboxHelp.Visible = False
        End If
    End Sub

    Private Sub cmdExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdExit.Click
        On Error Resume Next
        Me.Hide()
        ThisDrawing.SelectionSets.Item("SetAddr").Delete()

        bAction = False
    End Sub

    Private Sub CommandButton1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton1.Click
        On Error Resume Next
        Me.Hide()
        ThisDrawing.SelectionSets.Item("SetAddr").Delete()
        ssetObj = ThisDrawing.SelectionSets.Add("SetAddr")
        ssetObj.SelectOnScreen()
        Label2.Text = ssetObj.Count
        CheckMandatory()
        Me.Show()
    End Sub

    Sub CheckMandatory()
        Dim bEnable As Boolean
        bEnable = True
        'On Error Resume Next
        If Len(txtSuburb.Text) = 0 Then bEnable = False
        If Len(txtStreetName.Text) = 0 Then bEnable = False
        If Val(Label2.Text) = 0 Then bEnable = False

        cmdOK.Enabled = bEnable

    End Sub

    Private Sub txtStreetName_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtStreetName.TextChanged
        CheckMandatory()
    End Sub

    Private Sub txtSuburb_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtSuburb.TextChanged
        CheckMandatory()
    End Sub

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Dim i As Integer, j As Integer
        Dim xdataOut As Object
        Dim xtypeOut As Object
        Dim sAddress As String
        Dim vTmp As Object
        Dim bFound As Boolean
        Dim pl As AcadLWPolyline

        Me.Width = 336

        On Error Resume Next
        'Scan the drawing for any suburb and street information
        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            'Debug.Print ThisDrawing.ModelSpace.Item(i).ObjectName; ","; ThisDrawing.ModelSpace.Item(i).layer
            If ThisDrawing.ModelSpace.Item(i).ObjectName = "AcDbPolyline" Then
                pl = ThisDrawing.ModelSpace.Item(i)
                If pl.Layer = "Cadastre Roads_SP_EXT" Then
                    ' Return the xdata for the line
                    pl.GetXData("", xtypeOut, xdataOut)
                    sAddress = ""
                    For j = LBound(xtypeOut) To UBound(xtypeOut) - 1
                        Debug.Print("***" + xdataOut(j + 1))
                        vTmp = Split(xdataOut(j + 1), ",")
                        Select Case Trim(xdataOut(j) & "")
                            Case "SUBURB"
                                sAddress = vTmp(0) & sAddress
                            Case "LPI_NAME"
                                sAddress = " - " & vTmp(0)
                        End Select
                    Next j
                    sAddress = Trim(Replace(sAddress, "  ", " "))
                    If Len(sAddress) > 0 Then
                        bFound = False
                        For j = 0 To ListView1.Items.Count - 1
                            If sAddress = ListView1.Items.Item(j).Text Then
                                bFound = True
                            End If
                        Next j
                        If Not bFound Then ListView1.Items.Add(sAddress.TrimEnd())
                    End If
                End If
            End If
        Next i

        'Now sort the list box.
        'SortListBox(ListView1, 0, 1, 1)

        txtboxHelp.Text = "This command is used to update the street name and suburb on multiple blocks." _
            & vbCrLf & vbCrLf _
            & "Select from the list, or" _
            & vbCrLf _
            & "Enter the street name and suburb in the fields provided." _
            & vbCrLf & vbCrLf _
            & "Blank values will result in the data being deleted from the selected blocks." _
            & vbCrLf & vbCrLf _
            & "Only NEW blocks will be updated."

        ' Create the selection set
        If ThisDrawing.PickfirstSelectionSet.Count > 0 Then
            ssetObj = ThisDrawing.PickfirstSelectionSet
            Label2.Text = ssetObj.Count
        End If

        txtboxHelp.Visible = False

    End Sub

    'Sub SortListBox(ByVal oLb As ListView, ByVal sCol As Integer, ByVal sType As Integer, ByVal sDir As Integer)
    '    Dim vaItems As Object
    '    Dim i As Long, j As Long
    '    Dim c As Integer
    '    Dim vTemp As Object

    '    'Put the items in a variant array
    '    vaItems = oLb.Items


    '    'Sort the Array Alphabetically(1)
    '    If sType = 1 Then
    '        For i = LBound(vaItems, 1) To UBound(vaItems, 1) - 1
    '            For j = i + 1 To UBound(vaItems, 1)
    '                'Sort Ascending (1)
    '                If sDir = 1 Then
    '                    If vaItems(i, sCol) > vaItems(j, sCol) Then
    '                        For c = 0 To oLb.Columns.Count - 1 'Allows sorting of multi-column ListBoxes
    '                            vTemp = vaItems(i, c)
    '                            vaItems(i, c) = vaItems(j, c)
    '                            vaItems(j, c) = vTemp
    '                        Next c
    '                    End If

    '                    'Sort Descending (2)
    '                ElseIf sDir = 2 Then
    '                    If vaItems(i, sCol) < vaItems(j, sCol) Then
    '                        For c = 0 To oLb.Columns.Count - 1 'Allows sorting of multi-column ListBoxes
    '                            vTemp = vaItems(i, c)
    '                            vaItems(i, c) = vaItems(j, c)
    '                            vaItems(j, c) = vTemp
    '                        Next c
    '                    End If
    '                End If

    '            Next j
    '        Next i
    '        'Sort the Array Numerically(2)
    '        '(Substitute CInt with another conversion type (CLng, CDec, etc.) depending on type of numbers in the column)
    '    ElseIf sType = 2 Then
    '        For i = LBound(vaItems, 1) To UBound(vaItems, 1) - 1
    '            For j = i + 1 To UBound(vaItems, 1)
    '                'Sort Ascending (1)
    '                If sDir = 1 Then
    '                    If CInt(vaItems(i, sCol)) > CInt(vaItems(j, sCol)) Then
    '                        For c = 0 To oLb.Columns.Count - 1 'Allows sorting of multi-column ListBoxes
    '                            vTemp = vaItems(i, c)
    '                            vaItems(i, c) = vaItems(j, c)
    '                            vaItems(j, c) = vTemp
    '                        Next c
    '                    End If

    '                    'Sort Descending (2)
    '                ElseIf sDir = 2 Then
    '                    If CInt(vaItems(i, sCol)) < CInt(vaItems(j, sCol)) Then
    '                        For c = 0 To oLb.Columns.Count - 1 'Allows sorting of multi-column ListBoxes
    '                            vTemp = vaItems(i, c)
    '                            vaItems(i, c) = vaItems(j, c)
    '                            vaItems(j, c) = vTemp
    '                        Next c
    '                    End If
    '                End If

    '            Next j
    '        Next i
    '    End If

    '    'Set the list to the array
    '    oLb.Items.AddRange(vaItems)
    'End Sub

    Private Sub ListView1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ListView1.Click
        Dim addr() As String
        addr = Split(ListView1.SelectedItems(0).Text, "-")

        txtSuburb.Text = Trim(addr(0))

        'txtStreetName.Text = Trim(addr(1)) 'Initially Used
        'Following is changed to hide type from street text box
        Dim street As String = Trim(addr(1))
        If (street.Contains(" ")) Then
            Dim typeIndex As String = street.LastIndexOf(" ")
            txtStreetName.Text = Trim(addr(1).Remove(typeIndex + 1))
        Else
            txtStreetName.Text = Trim(addr(1))
        End If

    End Sub

End Class