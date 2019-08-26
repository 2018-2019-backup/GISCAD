Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports ADODB

Public Class frmAttrs
    Private Const ArrTop As Integer = 1000
    Private MyArray(ArrTop) As String

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private Sub cbRandom(ByRef lst As ListBox)

        Dim i As Integer

        For i = 0 To lst.Items.Count - 1
            MyArray(i + 1) = lst.Items(i)
        Next i
    End Sub

    Private Sub cbSort(ByRef lst As ListBox)

        Dim i As Long

        'no unnecessary sorting
        If lst.Items.Count <= 1 Then Exit Sub
        'sort array (quick sort algorithm)
        QuickSort(MyArray, LBound(MyArray), UBound(MyArray))
        'clear listbox
        lst.Items.Clear()
        'use sorted array to refill listbox
        For i = 1 To UBound(MyArray)
            If Len(Trim(MyArray(i))) > 0 Then lst.Items.Add(MyArray(i))
        Next i
    End Sub

    Private Sub QuickSort(ByVal strArray() As String, ByVal intBottom As Integer, ByVal intTop As Integer)

        Dim strPivot As String, strTemp As String
        Dim intBottomTemp As Integer, intTopTemp As Integer

        intBottomTemp = intBottom
        intTopTemp = intTop

        strPivot = strArray((intBottom + intTop) \ 2)

        While (intBottomTemp <= intTopTemp)

            While (strArray(intBottomTemp) < strPivot And intBottomTemp < intTop)
                intBottomTemp = intBottomTemp + 1
            End While

            While (strPivot < strArray(intTopTemp) And intTopTemp > intBottom)
                intTopTemp = intTopTemp - 1
            End While

            If intBottomTemp < intTopTemp Then
                strTemp = strArray(intBottomTemp)
                strArray(intBottomTemp) = strArray(intTopTemp)
                strArray(intTopTemp) = strTemp
            End If

            If intBottomTemp <= intTopTemp Then
                intBottomTemp = intBottomTemp + 1
                intTopTemp = intTopTemp - 1
            End If

        End While

        'the function calls itself until everything is in good order
        If (intBottom < intTopTemp) Then QuickSort(strArray, intBottom, intTopTemp)
        If (intBottomTemp < intTop) Then QuickSort(strArray, intBottomTemp, intTop)

    End Sub

    Private Sub cmdExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdExit.Click
        Me.Close()
    End Sub

    Private Sub cmdOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdOK.Click
        Dim blkname As String
        Dim i As Integer

        For i = 0 To lstBlocks.Items.Count - 1
            If lstBlocks.SelectedItems(i) Then
                If Len(lstBlocks.Items(i)) > 4 Then
                    blkname = lstBlocks.Items(i) 'Left(lstBlocks.List(i), Len(lstBlocks.List(i)) - 4)
                    Label1.Name = i + 1 & "/" & lstBlocks.Items.Count
                    If Not DoAttachAttrs(blkname) Then
                        lstBlocks.Items(i) = lstBlocks.Items(i) & " Not Found"
                        Debug.Print(lstBlocks.Items(i))
                    End If
                End If
            End If
        Next i
    End Sub

    Sub PlaceBlocks()
        Dim i, j As Integer
        Dim textObj As AcadMText
        Dim text As String
        Dim textPoint(0 To 2) As Double
        Dim height As Double
        Dim blockRefObj As AcadBlockReference
        Dim insertionPnt(0 To 2) As Double
        On Error GoTo End_PlaceBlocks

        insertionPnt(0) = 0.0# : insertionPnt(1) = 0.0# : insertionPnt(2) = 0.0#
        For i = 0 To lstBlocks.Items.Count - 1

            For j = 1 To 5
                'If Left(lstBlocks.List(i), 1) = "*" Then
                blockRefObj = ThisDrawing.Application.ActiveDocument.ModelSpace.InsertBlock(insertionPnt, lstBlocks.Items(i), 1.0#, 1.0#, 1.0#, 0)

                ' Define the text object
                text = lstBlocks.Items(i)
                text = Replace(text, "_", " ")
                text = Replace(text, "X50", " X 50mm DUCTS\P")
                text = Replace(text, "X125", " X 125mm DUCTS\P")
                If text.Substring(text.Length - 3) = "EXT" Then
                    text = "EXISTING\P" & text
                Else
                    text = "NEW\P" & text
                End If
                text = text.Substring(0, (text.Length - 4))
                text = Replace(text, "TRENCH", "")
                text = Replace(text, "&", "")
                text = Replace(text, "  ", " ")
                text = Replace(text, "\P ", "\P")
                text = Trim(text)

                textPoint(0) = insertionPnt(0) - 20
                textPoint(1) = (insertionPnt(1) - 2)
                textPoint(2) = 0.0#
                Debug.Print(textPoint(0) + "/" + textPoint(1) + "/" + textPoint(2))
                height = 0.5

                ' Create the text object in model space
                textObj = ThisDrawing.ModelSpace.AddMText(textPoint, 40, text)
                textObj.AttachmentPoint = AcAttachmentPoint.acAttachmentPointTopCenter
                textObj.Height = 1.8
                insertionPnt(1) = insertionPnt(1) + 40
                'End If
                i = i + 1
            Next j
            i = i - 1
            insertionPnt(0) = insertionPnt(0) + 60
            insertionPnt(1) = 0.0#
        Next i
End_PlaceBlocks:
    End Sub

    Private Sub CommandButton1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton1.Click
        GetBlocks()
    End Sub

    Private Sub CommandButton2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton2.Click
        Dim i As Integer

        For i = 0 To lstBlocks.Items.Count - 1
            lstBlocks.SelectedItems(i) = True
        Next i
    End Sub

    Private Sub CommandButton3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton3.Click
        'DoTest
        PlaceBlocks()
    End Sub

    Sub GetBlocks()
        Dim cblk As AcadBlock
        Dim enty As AcadEntity
        Dim attr As AcadAttribute
        Dim i As Integer

        lstBlocks.Items.Clear()

        For i = 0 To ThisDrawing.Blocks.Count - 1
            If (ThisDrawing.Application.ActiveDocument.Blocks.Item(i).Name.Substring(0, 2) <> "*M") Then
                If (ThisDrawing.Application.ActiveDocument.Blocks.Item(i).Name.Substring(0, 2) <> "*P") Then
                    cblk = ThisDrawing.Application.ActiveDocument.Blocks.Item(i)
                    'If Left(cblk.Name, 1) = "*" Then
                    '    'cblk.Delete
                    'Else
                    lstBlocks.Items.Add(cblk.Name)
                    'End If
                    'cblk.
                    Debug.Print(cblk.Name)
                    'For j = 0 To cblk.Count - 1
                    '    If cblk.Item(j).ObjectName = "AcDbAttributeDefinition" Then
                    '        Set attr = cblk.Item(j)
                    '        If Left(attr.TagString, 5) = "ASSET" Then
                    '            'lstBlocks.AddItem attr.TagString
                    '            attr.Layer = "0"
                    '        End If
                    '    End If
                    'Next j
                End If
            End If
        Next i

        'sort them
        cbRandom(lstBlocks)
        cbSort(lstBlocks)
    End Sub

    Sub DoTest()
        ' This example creates an attribute definition in model space.

        Dim attributeObj As AcadAttribute
        Dim height As Double
        Dim mode As Long
        Dim prompt As String
        Dim insertionPoint(0 To 2) As Double
        Dim tag As String
        Dim Value As String
        Dim cblk As AcadBlock
        Dim Y As Double
        Dim i As Integer
        Dim bFound As Boolean
        Debug.Print("----------------------------------------")

        adb.RecordSource = "select distinct block from blockattrs"
        adb.Refresh()
        With adb.Recordset
            Do While Not .EOF
                bFound = False
                For i = 0 To ThisDrawing.Blocks.Count - 1
                    cblk = ThisDrawing.Blocks.Item(i)
                    If (cblk.Name.Substring(0, Len(adb.Recordset.Fields("Block").Value)) = adb.Recordset.Fields("Block").Value) Then
                        bFound = True
                    End If
                Next
                If Not bFound Then Debug.Print(adb.Recordset.Fields("Block").Value)
                .MoveNext()
            Loop
        End With
    End Sub

    Function DoAttachAttrs(ByVal blkname As String) As Boolean
        ' This example creates an attribute definition in model space.
        Dim bFound As Boolean
        Dim attributeObj As AcadAttribute
        Dim height As Double
        Dim mode As Long
        Dim prompt As String
        Dim insertionPoint(0 To 2) As Double
        Dim tag As String
        Dim Value As String
        Dim cblk As AcadBlock
        Dim Y As Double
        Dim aVal As String
        Dim sLayerExtn As String
        Dim i, j As Integer

        On Error Resume Next
        bFound = False
        DoAttachAttrs = False
        For i = 0 To ThisDrawing.Blocks.Count - 1
            cblk = ThisDrawing.Blocks.Item(i)

            If cblk.Name = blkname Then

                'don't do anything if the database does not have attributes for this block
                bFound = False
                adb.RecordSource = "select * from blockattrs where block = '" & blkname & "' order by sequence,prompt"
                adb.Refresh()

                If Not adb.Recordset.EOF Then
                    bFound = True
                End If

                If bFound Then
                    ' first delete the existing attributes
                    For j = cblk.Count - 1 To 0 Step -1
                        If cblk.Item(j).ObjectName = "AcDbAttributeDefinition" Then
                            cblk.Item(j).Delete()
                        End If
                    Next j

                    With adb.Recordset
                        Y = 0.0#
                        Do While Not .EOF
                            insertionPoint(0) = adb.Recordset.Fields("X").Value : insertionPoint(1) = Y : insertionPoint(2) = adb.Recordset.Fields("Z").Value
                            If adb.Recordset.Fields("Y").Value <> 0 Then insertionPoint(1) = adb.Recordset.Fields("Y").Value

                            attributeObj = cblk.AddAttribute(.Fields("HEIGHT").Value, _
                                                                .Fields("MODE").Value + AcAttributeMode.acAttributeModePreset, _
                                                                .Fields("PROMPT").Value, _
                                                                insertionPoint, _
                                                                .Fields("TAG").Value, _
                                                                .Fields("VALUE").Value & "")

                            If Val(.Fields("Rotation").Value & "") <> 0 Then
                                attributeObj.Rotate(insertionPoint, Val(.Fields("Rotation").Value))
                            End If

                            If Len(Trim(.Fields("Style").Value & "")) = 0 Then
                                attributeObj.StyleName = "Standard"
                            Else
                                attributeObj.StyleName = .Fields("Style").Value & ""
                            End If

                            If IsDBNull(.Fields("Layer").Value) Then
                                attributeObj.Layer = "0"

                            ElseIf Trim(.Fields("Layer").Value) = "0" Then
                                attributeObj.Layer = .Fields("Layer").Value & ""

                            Else
                                attributeObj.Layer = .Fields("Layer").Value & ""

                                'Else
                                '    sLayerExtn = Right(cblk.Name, 4)
                                '    If sLayerExtn = "_DEL" Then sLayerExtn = "_EXT"
                                '    attributeObj.Layer = .Fields("Layer").Value & Right(cblk.Name, 4)

                            End If

                            .MoveNext()
                            Y = Y - 0.2
                            DoAttachAttrs = True
                        Loop
                    End With
                End If
            End If
        Next i
    End Function

    Private Sub CommandButton4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton4.Click
        Dim blk As AcadBlock
        Dim blks As AcadBlocks
        Dim nblk As AcadBlockReference
        Dim i As Integer
        For Each blk In ThisDrawing.Blocks
            Debug.Print(blk.Name)
            For i = 0 To blk.Count - 1
                If blk.Item(i).ObjectName = "AcDbBlockReference" Then
                    nblk = blk.Item(i)
                    Debug.Print(i + "-" + nblk.Name)
                End If
            Next i
        Next blk
    End Sub

    Private Sub CommandButton5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton5.Click
        ' This example creates an attribute definition in model space.
        Dim bFound As Boolean
        Dim attributeObj As AcadAttribute
        Dim height As Double
        Dim mode As Long
        Dim prompt As String
        Dim insertionPoint(0 To 2) As Double
        Dim tag As String
        Dim Value As String
        Dim cblk As AcadBlock
        Dim Y As Double
        Dim aVal As String
        Dim sLayerExtn As String
        Dim i, j As Integer

        On Error Resume Next
        bFound = False
        'DoAttachAttrs = False
        For i = 0 To ThisDrawing.Blocks.Count - 1
            cblk = ThisDrawing.Blocks.Item(i)

            'don't do anything if the database does not have attributes for this block
            bFound = False
            adb.RecordSource = "select * from blockattrs where block = '" & cblk.Name & "' order by sequence,prompt"
            adb.Refresh()

            If Not adb.Recordset.EOF Then
                ' first delete the existing attributes
                For j = cblk.Count - 1 To 0 Step -1
                    If cblk.Item(j).ObjectName = "AcDbAttributeDefinition" Then
                        cblk.Item(j).Delete()
                    End If
                Next j

            End If

        Next i
    End Sub

    Function DoDelAttrs(ByVal blkname As String) As Boolean

        ' This example creates an attribute definition in model space.
        Dim bFound As Boolean
        Dim attributeObj As AcadAttribute
        Dim height As Double
        Dim mode As Long
        Dim prompt As String
        Dim insertionPoint(0 To 2) As Double
        Dim tag As String
        Dim Value As String
        Dim cblk As AcadBlock
        Dim Y As Double
        Dim aVal As String
        Dim sLayerExtn As String
        Dim i, j As Integer

        On Error Resume Next
        bFound = False
        DoDelAttrs = False
        For i = 0 To ThisDrawing.Blocks.Count - 1
            cblk = ThisDrawing.Blocks.Item(i)

            If cblk.Name = blkname Then

                ' first delete the existing attributes
                For j = cblk.Count - 1 To 0 Step -1
                    If cblk.Item(j).ObjectName = "AcDbAttributeDefinition" Then
                        cblk.Item(j).Delete()
                    End If
                Next j
            End If
        Next i
    End Function

    Private Sub CommandButton6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton6.Click
        Dim bFound As Boolean
        Dim attributeObj As AcadAttribute
        Dim height As Double
        Dim mode As Long
        Dim prompt As String
        Dim insertionPoint(0 To 2) As Double
        Dim tag As String
        Dim Value As String
        Dim cblk As AcadBlock
        Dim Y As Double
        Dim aVal As String
        Dim sLayerExtn As String
        Dim i As Integer

        On Error Resume Next
        bFound = False
        For i = 0 To ThisDrawing.Blocks.Count - 1
            cblk = ThisDrawing.Blocks.Item(i)

            Debug.Print(cblk.Name + "|" + cblk.ObjectID + "|" + cblk.ObjectName)
            ' first delete the existing attributes
            'For j = cblk.Count - 1 To 0 Step -1
            '    If cblk.Item(j).ObjectName = "AcDbAttributeDefinition" Then
            '        cblk.Item(j).Delete
            '    End If
            'Next j
        Next i

    End Sub
End Class