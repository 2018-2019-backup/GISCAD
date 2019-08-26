Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports GISCADU


Public Class ThisDrawing

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    <Autodesk.AutoCAD.Runtime.CommandMethod("addCable")> _
    Sub AddCable()
        Dim frAddCable As New frmAddCable
        frAddCable.Show()
    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("assetNums")> _
    Sub AssetNums()
        Try
            Dim sPath As String
            Dim nIndex As Integer
            sPath = Trim(System.Reflection.Assembly.GetExecutingAssembly().Location)
            nIndex = sPath.LastIndexOf("\")
            sPath = sPath.Substring(0, nIndex)
            If System.IO.File.Exists(sPath & "\settings.ini") Then
                SetInifile(sPath & "\settings.ini")
                'MsgBox(sPath & "\settings.ini")
                GISCADU.modINI.SetInifile(sPath & "\settings.ini")
                Dim frAssetNums As New GISCADU.frmAssetNums
                frAssetNums.ShowDialog()
            Else
                MsgBox("Drawing should be saved first ," & vbCrLf & " before Asset Numbering can be executed", _
                       MsgBoxStyle.Critical, "Asset Numbering")
            End If
        Catch ex As Exception
            MsgBox("Drawing should be saved first, " & vbCrLf & " before Asset Numbering can be executed", _
                       MsgBoxStyle.Critical, "Asset Numbering")
            'MsgBox("Fatal Error: Unable to load settings", MsgBoxStyle.Critical, "Asset Numbering")
        End Try
    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("Legendtest")> _
    Sub legendtest()
        Dim i As Integer
        Dim pline As AcadPolyline


        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            'Debug.Print ThisDrawing.ModelSpace(i).ObjectName

            If ThisDrawing.ModelSpace(i).ObjectName = "AcDbPolyline" Then
                Debug.Print(ThisDrawing.ModelSpace(i).Layer)
                'UPGRADE_WARNING: Untranslated statement in legendtest. Please check source code.
                'Set pline = ThisDrawing.ModelSpace(i).
                'If InStr(1, pline.Layer, "_NEW_") > 1 Then
                '    Debug.Print pline.Layer; " "; pline.Linetype
                '
                'End If
            End If
        Next i
    End Sub
End Class