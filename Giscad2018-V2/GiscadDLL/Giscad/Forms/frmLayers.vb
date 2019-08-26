Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports ADODB

Public Class frmLayers
    Dim adb As New Connection
    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property
    Private Sub CommandButton1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CommandButton1.Click
        Dim layerObj As AcadLayer
        Dim i As Integer
        On Error Resume Next

        For i = ThisDrawing.Layers.Count To 1 Step -1
            ThisDrawing.Layers.Item(i).Delete()
        Next i

        adb.RecordSource = "select NewLayer,ExtLayer,DelLayer,ModLayer from blockfeatures"
        adb.Refresh()
        With adb.Recordset
            Do While Not .EOF

                ' Add the layer to the layers collection
                If !NewLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!NewLayer & ""))
                    SetColour(layerObj)
                End If
                If !ExtLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!ExtLayer & ""))
                    SetColour(layerObj)
                End If
                If !DelLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!DelLayer & ""))
                    SetColour(layerObj)
                End If
                If !ModLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!ModLayer & ""))
                    SetColour(layerObj)
                End If
                ' Make the new layer the active layer for the drawing
                'ThisDrawing.ActiveLayer = layerObj

                .MoveNext()
            Loop
        End With
        adb.RecordSource = "select ExtLayer,DelLayer from Linearfeatures"
        adb.Refresh()
        With adb.Recordset
            Do While Not .EOF


                ' Add the layer to the layers collection
                If !ExtLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!ExtLayer & ""))
                    SetColour(layerObj)
                End If
                If !DelLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!ExtLayer & ""))
                    SetColour(layerObj)
                End If
                ' Make the new layer the active layer for the drawing
                'ThisDrawing.ActiveLayer = layerObj

                .MoveNext()
            Loop
        End With
        adb.RecordSource = "select NewLayer from NewLinearfeatures"
        adb.Refresh()
        With adb.Recordset
            Do While Not .EOF


                ' Add the layer to the layers collection
                If !NewLayer & "" <> "" Then
                    layerObj = ThisDrawing.Layers.Add(Trim(!NewLayer & ""))
                    SetColour(layerObj)
                End If

                .MoveNext()
            Loop
        End With

        ' Add Misc layers
        layerObj = ThisDrawing.Layers.Add("Features_HIDE")
        layerObj = ThisDrawing.Layers.Add("AssetNumbers_EXT")
        SetColour(layerObj)
        layerObj = ThisDrawing.Layers.Add("AssetNumbers_NEW")
        SetColour(layerObj)

        MsgBox("done")
    End Sub

    Sub SetColour(ByRef layerObj As AcadLayer)
        Dim color As New AcadAcCmColor


        If InStr(layerObj.Name, "EXT") > 0 Then
            color.ColorIndex = AcColor.acWhite
            layerObj.TrueColor = color
        ElseIf InStr(layerObj.Name, "NEW") > 0 Then
            color.ColorIndex = AcColor.acYellow
            layerObj.TrueColor = color
        ElseIf InStr(layerObj.Name, "MOD") > 0 Then
            color.ColorIndex = AcColor.acGreen
            layerObj.TrueColor = color
        ElseIf InStr(layerObj.Name, "REP") > 0 Then
            color.ColorIndex = AcColor.acYellow
            layerObj.TrueColor = color
        ElseIf InStr(layerObj.Name, "Trench") > 0 Then
            color.ColorIndex = AcColor.acRed
        End If
    End Sub
End Class