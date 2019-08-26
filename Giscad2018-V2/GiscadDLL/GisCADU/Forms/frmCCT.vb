Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports ADODB

Public Class frmCCT
    Dim rs As Recordset

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Private Sub cmdCancel_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Close()
    End Sub

    Private Sub cmdOK_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdOK.Click
        Dim l() As String
        Dim cbl() As String
        Dim pcbl() As String
        Dim pline As AcadLWPolyline
        Dim i As Integer
        Dim ccode As Integer

        lstCCT.Items.Clear()

        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            'Debug.Print ThisDrawing.ModelSpace(i).ObjectName
            If ThisDrawing.ModelSpace(i).ObjectName = "AcDbPolyline" Then
                pline = ThisDrawing.ModelSpace(i)
                With pline

                    Debug.Print(.Layer)

                    If (chkHVCCT.Checked) Then
                        If (.Layer.Substring(0, Len("Mains_HV_NEW")) = "Mains_HV_NEW") Then
                            'If Left$(.Layer, Len("Mains_HV_NEW")) = "Mains_HV_NEW" Then
                            lstAdd(.Layer, .Linetype, .Length)
                        End If
                    End If
                    If (chkLVCCT.Checked) Then
                        If (.Layer.Substring(0, Len("Mains_LV_NEW")) = "Mains_LV_NEW") Then
                            'If Left$(.Layer, Len("Mains_LV_NEW")) = "Mains_LV_NEW" Then
                            lstAdd(.Layer, .Linetype, .Length)
                        End If
                    End If
                    If (chkSLCCT.Checked) Then
                        If (.Layer.Substring(0, Len("Mains_SL_NEW")) = "Mains_SL_NEW") Then
                            'If Left$(.Layer, Len("Mains_SL_NEW")) = "Mains_SL_NEW" Then
                            lstAdd(.Layer, .Linetype, .Length)
                        End If
                    End If

                End With
            End If
        Next i

        Dim lineObj As AcadLine
        Dim textObj As AcadMText
        Dim startPoint(0 To 2) As Double
        Dim endPoint(0 To 2) As Double
        Dim textPoint(0 To 2) As Double
        Dim cableType As String = ""
        Dim offset As Integer

        offset = 0
        For i = 0 To lstCCT.Items.Count - 1
            ' Create the line
            startPoint(0) = 0.0# : startPoint(1) = 0.0# + offset : startPoint(2) = 0.0#
            endPoint(0) = 15 : endPoint(1) = 0.0# + offset : endPoint(2) = 0.0#
            lineObj = ThisDrawing.PaperSpace.AddLine(startPoint, endPoint)
            cbl = Split(lstCCT.Items.Item(i), ",")
            lineObj.Linetype = cbl(1)
            lineObj.Layer = "Notes"

            rs.Open("SELECT conname FROM tblconductors where conlayer = '" & cbl(0) & "'")
            adb.Refresh()

            Do While Not rs.EOF
                cableType = rs.Fields("conname").Name
                rs.MoveNext()
            Loop


            textPoint(0) = endPoint(0) + 5 : textPoint(1) = endPoint(1) : textPoint(2) = 0.0#
            Text = "Lay " & cableType & " RL = " & Int(cbl(2)) & " CL = "
            textObj = ThisDrawing.PaperSpace.AddMText(textPoint, 40, Text)
            textObj.AttachmentPoint = AcAttachmentPoint.acAttachmentPointMiddleLeft
            textObj.Height = 2.5
            textObj.Layer = "Notes"
            offset = offset - 10

        Next i
        Me.Close()
    End Sub

    Sub lstAdd(ByVal sLayer As String, ByVal sLinetype As String, ByVal dLength As Double)

        Dim i As Integer
        Dim cbl() As String
        Dim bFound As Boolean

        bFound = False
        For i = 0 To lstCCT.Items.Count - 1
            cbl = Split(lstCCT.Items.Item(i), ",")
            If cbl(0) = sLayer And cbl(1) = sLinetype Then
                cbl(2) = Trim(Str(Val(cbl(2)) + dLength))
                lstCCT.Items.RemoveAt(i)
                lstCCT.Items.Add(Join(cbl, ","))
                bFound = True
            End If
        Next i
        If Not bFound Then
            lstCCT.Items.Add(sLayer & "," & sLinetype & "," & Trim(Str(dLength)))
        End If
    End Sub
End Class
