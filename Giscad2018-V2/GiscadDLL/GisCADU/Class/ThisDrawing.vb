Option Strict Off
Option Explicit On
Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports Autodesk.AutoCAD.ApplicationServices
Imports Autodesk.AutoCAD.DatabaseServices
Imports System.IO
Imports Autodesk.AutoCAD.EditorInput

Public Class ThisDrawing

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)

            'Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Sub AddCable()
        Dim frmAddCable As New Form
        'UPGRADE_WARNING: Couldn't resolve default property of object frmAddCable.Show. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        frmAddCable.Show()
    End Sub

    Sub AssetNums()
        Dim frmAssetNums As New Form
        Dim a As Object
        Dim i As Short
        Dim bSettingsLoaded As Boolean
        Dim sDb As String
        'UPGRADE_ISSUE: FileSystemObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.FullName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If Len(ThisDrawing.FullName) <> 0 Then

            'UPGRADE_WARNING: Couldn't resolve default property of object Application.Preferences. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            'UPGRADE_WARNING: Couldn't resolve default property of object a. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            a = Split(ThisDrawing.Application.Preferences.Files.SupportPath, ";")
            bSettingsLoaded = False
            For i = LBound(a) To UBound(a)
                'UPGRADE_WARNING: Couldn't resolve default property of object a(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If InStr(1, a(i), "VBA") > 0 Then
                    'UPGRADE_WARNING: Couldn't resolve default property of object a(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    SetInifile(a(i) & "\settings.ini")
                    bSettingsLoaded = True
                End If
            Next i

            If Not bSettingsLoaded Then
                MsgBox("Fatal Error: Unable to load settings", MsgBoxStyle.Critical, "Asset Numbering")
            Else
                'Get the database
                'UPGRADE_WARNING: Couldn't resolve default property of object GetIniString(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                sDb = Trim(GetIniString("Database", "db"))
                If Len(sDb) = 0 Then
                    MsgBox("Fatal Error: Unable to locate database", MsgBoxStyle.Critical, "Asset Numbering")
                Else
                    If CopyDB(sDb, "X:\GISCAD\assetnum.mdb") Then
                        Dim frAssetNums As New GISCADU.frmAssetNums
                        frAssetNums.SetDB("X:\GISCAD\assetnum.mdb")
                        'UPGRADE_WARNING: Couldn't resolve default property of object frmAssetNums.DoLoadOrgUnits. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        frAssetNums.DoLoadOrgUnits()
                        'UPGRADE_WARNING: Couldn't resolve default property of object frmAssetNums.Show. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        frAssetNums.ShowDialog()
                    Else
                        MsgBox("Unable to create local copy of database." & vbCrLf & vbCrLf & "X:\GISCAD folder may not exist", MsgBoxStyle.Critical, "GISCAD - Asset Numbering")
                    End If
                End If
            End If
        Else
            MsgBox("No current drawing to process", MsgBoxStyle.Critical, "GISCAD - Asset Numbering")

        End If
        'End

    End Sub
    Function CopyDB(ByVal sDb1 As String, ByVal sDb2 As String) As Boolean

        CopyDB = False
        On Error GoTo Err_CopyDB

        'UPGRADE_WARNING: Couldn't resolve default property of object fso.CopyFile. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        File.Copy(sDb1, sDb2, True)
        CopyDB = True

Err_CopyDB:

    End Function
    <Autodesk.AutoCAD.Runtime.CommandMethod("SetAddr")> _
    Sub SetAddr()
        'UPGRADE_ISSUE: frmAddress object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim f As frmAddress

        On Error Resume Next

        f = New frmAddress
        f.Show()

        'UPGRADE_WARNING: Couldn't resolve default property of object f.DoModal. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If (f.Modal) Then

        End If

        'UPGRADE_NOTE: Object f may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        f = Nothing

    End Sub


    <Autodesk.AutoCAD.Runtime.CommandMethod("AssetNumsOn")> _
    Sub AssetNumsON()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i As Short

        On Error Resume Next
        ' Create the selection set
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj = ThisDrawing.SelectionSets.Add("AssetNumsON")

        ' Add objects to a selection set by prompting user to select on the screen
        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj.SelectOnScreen()

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count > 0 Then

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            For i = 0 To ssetObj.Count - 1
                'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj().ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                    blk = ssetObj(i)
                    SetAttrLayer(blk, "ASSET_NUM", blk.Layer, "Features_HIDE")
                    'UPGRADE_WARNING: Untranslated statement in AssetNumsON. Please check source code.
                End If
            Next i

        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("AssetNumsON").Delete()

    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("HideInvalidPoles")> _
    Sub HideInvalidPoles()
        Dim ssetObj As SelectionSet

        Dim acDoc As Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument
        Dim usageInput As String
        Dim usage() As String
        Dim apUsage As PromptStringOptions = New PromptStringOptions("")
        apUsage.Message = vbCrLf & "Enter the Usage Value:"

        Dim hide As Integer = 1
        Dim attrValue As String
        Dim attrUsage() As String

        Dim tr As Transaction = acDoc.Database.TransactionManager.StartTransaction()

        Dim acResult As PromptResult = acDoc.Editor.GetString(apUsage)
        If acResult.Status = PromptStatus.OK Then
            usageInput = acResult.StringResult

            If String.IsNullOrEmpty(usageInput) Then
                Return
            End If

            usage = usageInput.Split(",")

            Dim typValue(1) As TypedValue
            typValue.SetValue(New TypedValue(DxfCode.Start, "INSERT"), 0)
            typValue.SetValue(New TypedValue(DxfCode.ExtendedDataRegAppName, "IE_TB_POLE"), 1)
            Dim acSelect As PromptSelectionResult = acDoc.Editor.SelectAll(New SelectionFilter(typValue))

            If acSelect.Status = PromptStatus.OK Then
                ssetObj = acSelect.Value

                If ssetObj.Count > 0 Then
                    Dim idArray() As ObjectId = ssetObj.GetObjectIds()

                    For Each blkId As ObjectId In idArray
                        Dim blkRef As BlockReference = TryCast(tr.GetObject(blkId, OpenMode.ForWrite), BlockReference)
                        Dim attCol As AttributeCollection = blkRef.AttributeCollection



                        Dim Result() As Boolean = (From attId As ObjectId In attCol
                                     Where (TryCast(tr.GetObject(attId, OpenMode.ForRead), AttributeReference).Tag.Equals("usage", StringComparison.InvariantCultureIgnoreCase))
                                     Select usage.Intersect(From str As String In TryCast(tr.GetObject(attId, OpenMode.ForRead), AttributeReference).TextString.ToString().Split(",") Select str).Count > 0).ToArray()

                        If (Result.Contains(True)) Then
                            blkRef.Visible = True
                        Else
                            blkRef.Visible = False
                        End If

                    Next

                End If
            End If
        End If

        tr.Commit()
        tr.Dispose()

        'Dim btr As BlockTableRecord = TryCast(tr.GetObject(blkRef.BlockTableRecord, OpenMode.ForRead), BlockTableRecord)
        'btr.Dispose()
        'For Each attId As ObjectId In attCol
        '    Dim attRef As AttributeReference = TryCast(tr.GetObject(attId, OpenMode.ForRead), AttributeReference)
        '    If attRef.Tag.ToString.Equals("usage", StringComparison.InvariantCultureIgnoreCase) Then
        '        attrValue = attRef.TextString
        '        If Not String.IsNullOrEmpty(attrValue) Then
        '            attrUsage = attrValue.Split(",")
        '            For Each val As String In attrUsage
        '                If usage.Contains(val) Then
        '                    hide = 0
        '                End If
        '            Next

        '            If hide = 1 Then
        '                blkRef.Visible = False
        '            Else
        '                hide = 1
        '                blkRef.Visible = True
        '            End If
        '        Else
        '            blkRef.Visible = False
        '        End If
        '    End If
        'Next

        'ThisDrawing.SelectionSets.Item("HideInvalidPoles").Delete()
    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("ShowAllPoles")> _
    Sub ShowAllPoles()
        Dim ssetObj As SelectionSet
        Dim acDoc As Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument
        Dim tr As Transaction = acDoc.Database.TransactionManager.StartTransaction()
        Dim typValue(1) As TypedValue
        typValue.SetValue(New TypedValue(DxfCode.Start, "INSERT"), 0)
        typValue.SetValue(New TypedValue(DxfCode.ExtendedDataRegAppName, "IE_TB_POLE"), 1)
        Dim acSelect As PromptSelectionResult = acDoc.Editor.SelectAll(New SelectionFilter(typValue))

        If acSelect.Status = PromptStatus.OK Then
            ssetObj = acSelect.Value

            If ssetObj.Count > 0 Then

                Dim idArray() As ObjectId = ssetObj.GetObjectIds()
                For Each blkId As ObjectId In idArray
                    Dim blkRef As BlockReference = TryCast(tr.GetObject(blkId, OpenMode.ForWrite), BlockReference)
                    'Dim btr As BlockTableRecord = TryCast(tr.GetObject(blkRef.BlockTableRecord, OpenMode.ForRead), BlockTableRecord)
                    'btr.Dispose()

                    Dim attCol As AttributeCollection = blkRef.AttributeCollection


                    Dim Result() As ObjectId = (From attId As ObjectId In attCol
                                   Where (TryCast(tr.GetObject(attId, OpenMode.ForRead), AttributeReference).Tag.Equals("usage", StringComparison.InvariantCultureIgnoreCase))
                                   Select attId).ToArray()

                    If (Result.Count > 0) Then
                        blkRef.Visible = True
                    Else
                        blkRef.Visible = False
                    End If

                    'For Each attId As ObjectId In attCol
                    '    Dim attRef As AttributeReference = TryCast(tr.GetObject(attId, OpenMode.ForRead), AttributeReference)
                    '    If attRef.Tag.ToString.Equals("usage", StringComparison.InvariantCultureIgnoreCase) Then
                    '        blkRef.Visible = True
                    '    End If
                    'Next
                Next
            End If
        End If

        tr.Commit()
        tr.Dispose()
    End Sub


    <Autodesk.AutoCAD.Runtime.CommandMethod("HidePNoAndCNo")> _
    Sub HidePNoAndCNo()
        Dim ssetObj As AcadSelectionSet
        Dim blk As AcadBlockReference
        Dim varAttributes() As Object
        Dim i As Short

        'On Error Resume Next
        ' Create the selection set
        ssetObj = ThisDrawing.SelectionSets.Add("HidePNoAndCNo")
        ssetObj.Select(AcSelect.acSelectionSetAll)


        If ssetObj.Count > 0 Then
            Try

                Dim test = From col In ssetObj
                               Where col.ObjectName = "AcDbBlockReference"
                              Select col

                For Each blk In test
                    'Changed on 23 - Nov - 2013
                    Dim vattrs() As Object
                    vattrs = blk.GetAttributes()

                    Dim selLayers = From attr In vattrs
                                  Where attr.TagString.ToString().Equals("pNumber", StringComparison.InvariantCultureIgnoreCase)
                                  Select attr

                    Dim selLayers1 = From attr In vattrs
                                 Where attr.TagString.ToString().Equals("CNUMBER", StringComparison.InvariantCultureIgnoreCase)
                                 Select attr

                    For Each attributes As Object In selLayers
                        attributes.Layer = "Features_HIDE"
                    Next

                    For Each attributes As Object In selLayers1
                        attributes.Layer = "Features_HIDE"
                    Next

                    'blk = ssetObj(i)
                    ''SetAttrLayer(blk, "pNumber", "Features_HIDE", "Features_HIDE")
                    ''SetAttrLayer(blk, "CNUMBER", "Features_HIDE", "Features_HIDE")
                Next
                '    End If
                'Next i
            Catch ex As Exception
                MessageBox.Show(ex.ToString())
            End Try
        End If

        ThisDrawing.SelectionSets.Item("HidePNoAndCNo").Delete()
    End Sub


    <Autodesk.AutoCAD.Runtime.CommandMethod("ViewPNoAndCNo")> _
    Sub ViewPNoAndCNo()
        Dim ssetObj As AcadSelectionSet
        Dim blk As AcadBlockReference
        Dim varAttributes() As Object
        Dim i As Short

        'On Error Resume Next
        ' Create the selection set
        ssetObj = ThisDrawing.SelectionSets.Add("ViewPNoAndCNo")
        ssetObj.Select(AcSelect.acSelectionSetAll)

        If ssetObj.Count > 0 Then
            Try
                Dim test = From col In ssetObj
                                 Where col.ObjectName = "AcDbBlockReference"
                                Select col
               
                For Each blk In test
                    'Changed on 23 - Nov - 2013
                    Dim vattrs() As Object
                    vattrs = blk.GetAttributes()

                    Dim selLayers = From attr In vattrs
                                  Where attr.TagString.ToString().Equals("pNumber", StringComparison.InvariantCultureIgnoreCase)
                                  Select attr

                    Dim selLayers1 = From attr In vattrs
                                 Where attr.TagString.ToString().Equals("CNUMBER", StringComparison.InvariantCultureIgnoreCase)
                                 Select attr

                    For Each attributes As Object In selLayers
                        attributes.Layer = blk.Layer
                    Next

                    For Each attributes As Object In selLayers1
                        attributes.Layer = blk.Layer
                    Next

                    'SetAttrLayer(blk, "pNumber", blk.Layer, blk.Layer)
                    'SetAttrLayer(blk, "CNUMBER", blk.Layer, blk.Layer)
                Next
              
            Catch ex As Exception
                MessageBox.Show(ex.ToString())
            End Try
        End If

        ThisDrawing.SelectionSets.Item("ViewPNoAndCNo").Delete()
    End Sub


    <Autodesk.AutoCAD.Runtime.CommandMethod("SetParent")> _
    Sub SetParent()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        Dim sParent As String
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i As Short

        On Error Resume Next
        ' Create the selection set
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj = ThisDrawing.SelectionSets.Add("SetParent")

        ' Add objects to a selection set by prompting user to select on the screen
        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj.SelectOnScreen()

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sParent = ThisDrawing.Utility.GetString(False, "Enter Parent Value: ")

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count > 0 Then

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            For i = 0 To ssetObj.Count - 1
                'Debug.Print ThisDrawing.ModelSpace(i).ObjectName
                'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj().ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                    blk = ssetObj(i)

                    'If Right(getblockname(blk), Len("_NEW")) = "_NEW" Then
                    SetAttrValue(blk, "PARENT", sParent)
                    'UPGRADE_WARNING: Untranslated statement in SetParent. Please check source code.
                    'End If
                End If
            Next i
        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("SetParent").Delete()

    End Sub

    Sub CCTLegend()
        Dim frmCCT As New Form
        Dim a() As String
        Dim i As Short
        Dim bSettingsLoaded As Boolean
        Dim sDb As String

        'Todo: move this code to it's own function as it is used elsewhere

        'UPGRADE_WARNING: Couldn't resolve default property of object Application.Preferences. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        'UPGRADE_WARNING: Couldn't resolve default property of object a. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        a = Split(ThisDrawing.Application.Preferences.Files.SupportPath, ";")
        bSettingsLoaded = False
        For i = LBound(a) To UBound(a)
            'UPGRADE_WARNING: Couldn't resolve default property of object a(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If InStr(1, a(i), "VBA") > 0 Then
                'UPGRADE_WARNING: Couldn't resolve default property of object a(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                SetInifile(a(i) & "\settings.ini")
                bSettingsLoaded = True
            End If
        Next i

        If Not bSettingsLoaded Then
            MsgBox("Fatal Error: Unable to load settings", MsgBoxStyle.Critical, "GISCAD - CCT Legend")
        Else
            'Get the database
            'UPGRADE_WARNING: Couldn't resolve default property of object GetIniString(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            sDb = Trim(GetIniString("Database", "db"))
            If Len(sDb) = 0 Then
                MsgBox("Fatal Error: Unable to locate database", MsgBoxStyle.Critical, "GISCAD - CCT Legend")
            Else
                Dim frCCT As New GISCADU.frmCCT
                'UPGRADE_WARNING: Couldn't resolve default property of object frmCCT.Show. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                frmCCT.ShowDialog()
            End If
        End If

    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("ToggleLayer")> _
    Sub ToggleLayer()

        'UPGRADE_ISSUE: AcadLayer object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim layer As AcadLayer
        Dim sLayName As String

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        Try
            sLayName = ThisDrawing.Utility.GetString(False, "Enter Layer Name to toggle (<enter> terminates input): ")
            If Len(sLayName) > 0 Then
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Layers. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                layer = ThisDrawing.Layers.Item(sLayName)
                'UPGRADE_WARNING: Couldn't resolve default property of object layer.Freeze. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                layer.Freeze = Not layer.Freeze
                'UPGRADE_WARNING: Couldn't resolve default property of object layer.LayerOn. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                'UPGRADE_WARNING: Couldn't resolve default property of object layer.Freeze. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                layer.LayerOn = Not layer.Freeze
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Regen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                ThisDrawing.Regen(AcRegenType.acAllViewports)
            End If
        Catch ex As Exception
            MessageBox.Show(ex.Message, "Toggle Layer", MessageBoxButtons.OK)
        End Try
    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("SetCCTView")> _
    Sub SetCCTView()
        'UPGRADE_ISSUE: AcadLayer object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim lyr As AcadLayer
        Dim sCCTs As String
        Dim sCCT As Object
        Dim i As Short
        Dim bState As Boolean

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sCCTs = ThisDrawing.Utility.GetString(False, "Enter CCT (<enter> terminates input): ")
        If Len(sCCTs) > 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object sCCT. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            sCCT = Split(sCCTs, ",")

            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ActiveSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If ThisDrawing.ActiveSpace = AcActiveSpace.acModelSpace Then
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Layers. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                For Each lyr In ThisDrawing.Layers
                    'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    Debug.Print(lyr.Name)
                    bState = True
                    For i = LBound(sCCT) To UBound(sCCT)
                        'UPGRADE_WARNING: Couldn't resolve default property of object sCCT(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        If InStr(1, lyr.Name, sCCT(i)) > 0 Then
                            bState = False
                        End If
                    Next i
                    'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ActiveLayer. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Freeze. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    If ThisDrawing.ActiveLayer.Name <> lyr.Name Then lyr.Freeze = bState
                    'lyr.LayerOn = bState

                Next lyr

            Else 'Paperspace View port
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Layers. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                For Each lyr In ThisDrawing.Layers
                    'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    Debug.Print(lyr.Name)
                    'ignore layer 0
                    'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    If lyr.Name <> "0" Then
                        For i = LBound(sCCT) To UBound(sCCT)
                            'UPGRADE_WARNING: Couldn't resolve default property of object sCCT(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            If InStr(1, lyr.Name, sCCT(i)) = 0 Then
                                'UPGRADE_WARNING: Couldn't resolve default property of object lyr.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                VpLayerOff((lyr.Name))
                            End If
                        Next i
                    End If
                Next lyr
                ViewPortUpdate()
            End If

            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Regen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ThisDrawing.Regen(AcRegenType.acAllViewports)
        End If

    End Sub
    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub ViewPortUpdate()
        ' Update the viewport...
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim objPViewport As AcadObject

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ActivePViewport. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport = ThisDrawing.ActivePViewport
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.MSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.MSpace = False
        'UPGRADE_WARNING: Couldn't resolve default property of object objPViewport.Display. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport.Display(False)
        'UPGRADE_WARNING: Couldn't resolve default property of object objPViewport.Display. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport.Display(True)
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.MSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.MSpace = True
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.Utility.Prompt("Done!" & vbCr)
    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("AlterAttrStyle")> _
    Sub AlterAttrStyle()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        Dim sAttrTag As String
        'UPGRADE_ISSUE: frmAttrStyle object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim f As frmAttrStyle
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i, j As Short
        Dim vattrs As Object

        On Error Resume Next


        ' Create the selection set
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ThisDrawing.PickfirstSelectionSet.Count > 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.PickfirstSelectionSet
        End If

        ' Get the attribute to modify
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sAttrTag = ThisDrawing.Utility.GetString(False, "Enter Attribute Tag: ")
        If Len(Trim(sAttrTag)) = 0 Then Exit Sub

        ''MsgBox ssetObj.Count

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("AssetNums")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If


        If ssetObj.Count > 0 Then

            f = New frmAttrStyle
            f.SetAttrTag(sAttrTag)
            If (f.DoModal) Then
                For i = 0 To ssetObj.Count - 1

                    If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                        blk = ssetObj(i)
                        vattrs = blk.GetAttributes

                        For j = LBound(vattrs) To UBound(vattrs)
                            If vattrs(j).TagString = sAttrTag Then
                                If Len(f.sTextStyle) > 0 Then vattrs(j).StyleName = f.sTextStyle
                                If f.dTextHeight > 0 Then vattrs(j).height = f.dTextHeight

                            End If
                        Next j

                    End If
                Next i
            End If
        End If

        'UPGRADE_NOTE: Object f may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        f = Nothing
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("AssetNums").Delete()

    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("DrawExtractExtents")> _
    Sub DrawExtractExtents()
        Dim coords As String
        Dim coord As Object
        'Dim pts(0) As Double
        Dim i, j As Short
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        'UPGRADE_ISSUE: AcadLWPolyline object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim pl As AcadLWPolyline

        On Error Resume Next
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PaperSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 0 To ThisDrawing.PaperSpace.Count - 1
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PaperSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If ThisDrawing.PaperSpace.Item(i).ObjectName = "AcDbBlockReference" Then
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PaperSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                blk = ThisDrawing.PaperSpace.Item(i)

                'UPGRADE_WARNING: Untranslated statement in DrawExtractExtents. Please check source code.
            End If
        Next i

    End Sub
    <Autodesk.AutoCAD.Runtime.CommandMethod("FixDwg")> _
    Sub FixDwg()
        ' This function is used to fix any known issues with a drawing that may cause validation errors.
        ' 1. End Shackles

        Dim propname, propvalue As String
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i As Short

        On Error Resume Next
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If ThisDrawing.ModelSpace.Item(i).ObjectName = "AcDbBlockReference" Then
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                blk = ThisDrawing.ModelSpace.Item(i)
                'UPGRADE_WARNING: Untranslated statement in FixDwg. Please check source code.
            End If
        Next i

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SummaryInfo. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 0 To ThisDrawing.SummaryInfo.NumCustomInfo
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SummaryInfo. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ThisDrawing.SummaryInfo.getcutombyindex(0, propname, propvalue)
            If UCase(propname) = "VERSION" And Val(propvalue) < 2 Then
                'DoFixAttrs
            End If
        Next i

    End Sub
    Sub UpdateAttributes(ByVal iUsage As Short, ByRef blk As AcadBlockReference)

        Select Case iUsage
            Case 1
                SetAttrValue(blk, "USAGE", "LV")
                SetAttrValue(blk, "CONSTRUCTED_VOLTAGE", "LV")
                SetAttrValue(blk, "OPERATING_VOLTAGE", "LV")
            Case 2
                SetAttrValue(blk, "USAGE", "SL")
                SetAttrValue(blk, "CONSTRUCTED_VOLTAGE", "LV")
                SetAttrValue(blk, "OPERATING_VOLTAGE", "LV")
            Case 3
                SetAttrValue(blk, "USAGE", "HV")
                SetAttrValue(blk, "CONSTRUCTED_VOLTAGE", "11kV")
                SetAttrValue(blk, "OPERATING_VOLTAGE", "11kV")
            Case 4
                SetAttrValue(blk, "USAGE", "TR")
                SetAttrValue(blk, "CONSTRUCTED_VOLTAGE", "LV")
                SetAttrValue(blk, "OPERATING_VOLTAGE", "LV")
        End Select

    End Sub
    <Autodesk.AutoCAD.Runtime.CommandMethod("ListErrors")> _
    Sub ListErrors()

        Dim frGISErrors As New frmGISErrors

        'UPGRADE_WARNING: Couldn't resolve default property of object frmGISErrors.Show. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        frGISErrors.Show()

    End Sub

    <Autodesk.AutoCAD.Runtime.CommandMethod("FlipAttr")> _
    Sub FlipAttr()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        Dim sAttrTag As String
        'UPGRADE_ISSUE: frmAttrStyle object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim f As frmAttrStyle
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i, j As Short
        Dim vattrs() As Object

        On Error Resume Next


        ' Create the selection set
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ThisDrawing.PickfirstSelectionSet.Count > 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.PickfirstSelectionSet
        End If

        ' Get the attribute to modify
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sAttrTag = ThisDrawing.Utility.GetString(False, "Enter Attribute Tag: ")
        If Len(Trim(sAttrTag)) = 0 Then Exit Sub

        ''MsgBox ssetObj.Count

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("AssetNums")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If


        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count > 0 Then

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            For i = 0 To ssetObj.Count - 1

                'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj(i).ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                    blk = ssetObj(i)
                    'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    vattrs = blk.GetAttributes

                    For j = LBound(vattrs) To UBound(vattrs)
                        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(j).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        If vattrs(j).TagString = sAttrTag Then
                            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(j).Backward. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Backward. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            vattrs(j).Backward = Not vattrs(j).Backward
                            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(j).UpsideDown. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().UpsideDown. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            vattrs(j).UpsideDown = Not vattrs(j).UpsideDown
                            Select Case vattrs(j).Alignment
                                Case AcAlignment.acAlignmentLeft
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentTopRight. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentTopRight
                                Case AcAlignment.acAlignmentCenter
                                    'vattrs(j).Alignment = acAlignmentTopRight
                                Case AcAlignment.acAlignmentRight
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentLeft. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentLeft
                                Case AcAlignment.acAlignmentAligned
                                    'vattrs(j).Alignment = acAlignmentTopRight
                                Case AcAlignment.acAlignmentMiddle
                                    'vattrs(j).Alignment = acAlignmentTopRight
                                Case AcAlignment.acAlignmentFit
                                    'vattrs(j).Alignment = acAlignmentTopRight
                                Case AcAlignment.acAlignmentTopLeft
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentBottomRight. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentBottomRight
                                Case AcAlignment.acAlignmentTopCenter
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentBottomCenter. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentBottomCenter
                                Case AcAlignment.acAlignmentTopRight
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentBottomLeft. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentBottomLeft
                                Case AcAlignment.acAlignmentMiddleLeft
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentMiddleRight. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentMiddleRight
                                Case AcAlignment.acAlignmentMiddleCenter
                                    'vattrs(j).Alignment = acAlignmentTopRight
                                Case AcAlignment.acAlignmentMiddleRight
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentMiddleLeft. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentMiddleLeft
                                Case AcAlignment.acAlignmentBottomLeft
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentTopRight. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentTopRight
                                Case AcAlignment.acAlignmentBottomCenter
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentTopCenter. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentTopCenter
                                Case AcAlignment.acAlignmentBottomRight
                                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentTopLeft. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                                    vattrs(j).Alignment = AcAlignment.acAlignmentTopLeft
                            End Select
                        End If
                    Next j

                End If
            Next i

        End If
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("AssetNums").Delete()

    End Sub


    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub ResetAttr()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        Dim sAttrTag As String
        'UPGRADE_ISSUE: frmAttrStyle object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim f As frmAttrStyle
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i, j As Short
        Dim vattrs As Object

        On Error Resume Next

        Dim insertionPoint(2) As Double
        Dim currInsertionPoint As Object

        ' Create the selection set
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ThisDrawing.PickfirstSelectionSet.Count > 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.PickfirstSelectionSet
        End If

        ' Get the attribute to modify
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Utility. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        sAttrTag = ThisDrawing.Utility.GetString(False, "Enter Attribute Tag: ")
        If Len(Trim(sAttrTag)) = 0 Then Exit Sub

        ''MsgBox ssetObj.Count

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("AssetNums")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If


        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count > 0 Then

            'For i = 0 To ssetObj.Count - 1
            '
            '    If ssetObj(i).ObjectName = "AcDbBlockReference" Then
            '        Set blk = ssetObj(i)
            '        vattrs = blk.GetAttributes
            '
            '        For j = LBound(vattrs) To UBound(vattrs)
            '            If vattrs(j).TagString = sAttrTag Then
            '                'vattrs(j).Alignment = acAlignmentMiddleLeft
            '                currInsertionPoint = vattrs(j).insertionPoint
            '                insertionPoint(0) = currInsertionPoint(0) - 1
            '                insertionPoint(1) = currInsertionPoint(1)
            '                insertionPoint(2) = currInsertionPoint(2)
            '                vattrs(j).insertionPoint = insertionPoint
            '                vattrs(j).Update
            '
            '            End If
            '        Next j
            '
            '    End If
            'Next i

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            For i = 0 To ssetObj.Count - 1

                'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj(i).ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                    blk = ssetObj(i)
                    'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetAttributes. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object vattrs. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    vattrs = blk.GetAttributes

                    For j = LBound(vattrs) To UBound(vattrs)
                        'UPGRADE_WARNING: Couldn't resolve default property of object vattrs(j).TagString. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                        If vattrs(j).TagString = sAttrTag Then

                            'UPGRADE_WARNING: Couldn't resolve default property of object vattrs().Alignment. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            'UPGRADE_WARNING: Couldn't resolve default property of object acAlignmentMiddleLeft. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                            vattrs(j).Alignment = AcAlignment.acAlignmentMiddleLeft
                            'vattrs(j).Update
                        End If
                    Next j

                End If
            Next i

        End If
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("AssetNums").Delete()

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub GDXData()
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        Dim sAttrTag As String
        'UPGRADE_ISSUE: frmAttrStyle object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim f As frmAttrStyle
        'UPGRADE_ISSUE: AcadBlockReference object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blk As AcadBlockReference
        Dim i, j As Short
        Dim vattrs As AcadAttribute
        Dim xdataOut(13) As Object
        Dim xtypeOut(13) As Object
        On Error Resume Next

        Dim insertionPoint(2) As Double
        Dim currInsertionPoint As Object


        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("AssetNums")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If


        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count > 0 Then


            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            For i = 0 To ssetObj.Count - 1

                'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj(i).ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ssetObj(i).ObjectName = "AcDbBlockReference" Then
                    blk = ssetObj(i)
                    'UPGRADE_WARNING: Couldn't resolve default property of object blk.GetXData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    blk.GetXData("", xtypeOut, xdataOut)
                    For j = LBound(xdataOut) To UBound(xdataOut)
                        Debug.Print(xtypeOut(j) & "-" & xdataOut(j))
                    Next j

                    'xdataOut(0) = "CID": xdataOut(1) = "1"
                    'xdataOut(2) = "CNO": xdataOut(3) = "15502"
                    'xdataOut(4) = "FID": xdataOut(5) = "GISCAD"
                    'xdataOut(6) = "FNO": xdataOut(7) = "155"
                    'xdataOut(8) = "InsertionPoint": xdataOut(9) = ""
                    'xdataOut(10) = "State": xdataOut(11) = "Inservice"
                    'xdataOut(12) = "Usage": xdataOut(12) = ""
                    'xtypeOut(0) = "1001": xtypeOut(1) = "1000"
                    'xtypeOut(2) = "1001": xtypeOut(3) = "1000"
                    'xtypeOut(4) = "1001": xtypeOut(5) = "1000"
                    'xtypeOut(6) = "1001": xtypeOut(7) = "1000"
                    'xtypeOut(8) = "1001": xtypeOut(9) = "1000"
                    'xtypeOut(10) = "1001": xtypeOut(11) = "1000"
                    'xtypeOut(12) = "1001": xtypeOut(13) = "1000"

                    'blk.SetXData xtypeOut, xdataOut
                End If
            Next i

        End If
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("AssetNums").Delete()

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub xdata()
        Dim i As Short
        Dim xdataOut(1) As Int16
        Dim xtypeOut As Int16
        ' This example creates a line and attaches extended data to that line.
        'On Error Resume Next
        ' Create the line
        'UPGRADE_ISSUE: AcadLWPolyline object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim lineObj As AcadLWPolyline

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 1 To ThisDrawing.ModelSpace.Count
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If ThisDrawing.ModelSpace.Item(i).ObjectName = "AcDbPolyline" Then
                'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ModelSpace. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                lineObj = ThisDrawing.ModelSpace.Item(i)
                'UPGRADE_WARNING: Couldn't resolve default property of object lineObj.GetXData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                lineObj.GetXData("State", xtypeOut, xdataOut)
                Debug.Print(xdataOut(0) & xdataOut(1))
                'UPGRADE_WARNING: Couldn't resolve default property of object xdataOut(1). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If xdataOut(1) = "" Then
                    'UPGRADE_WARNING: Couldn't resolve default property of object lineObj.layer. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    lineObj.Layer = "0"
                Else
                    'UPGRADE_WARNING: Couldn't resolve default property of object lineObj.layer. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object xdataOut(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    lineObj.Layer = CStr(xdataOut(1))
                End If
            End If
        Next i


    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub RemoveFeature()
        Dim sMsg As String
        Dim i As Short
        Dim sCtr As Short
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj = ThisDrawing.PickfirstSelectionSet


        sMsg = "This command will move selected features to " & vbCrLf & "the FEATURES_DEL layer to indicate that " & vbCrLf & "the data is to be removed from GIS" & vbCrLf & vbCrLf & "Select OK to continue or," & vbCrLf & "Cancel to exit the command"

        If (MsgBox(sMsg, MsgBoxStyle.Exclamation + MsgBoxStyle.OkCancel, "Remove Feature") = MsgBoxResult.Cancel) Then Exit Sub


        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("Remove")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 0 To ssetObj.Count - 1

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj().layer. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj(i).layer = "Features_DEL"
        Next i

        On Error Resume Next
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("Remove").Delete()

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub HideFeature()
        Dim sMsg As String
        Dim i As Short
        'UPGRADE_ISSUE: AcadSelectionSet object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ssetObj As AcadSelectionSet
        sMsg = "This command will move selected features to " & vbCrLf & "the FEATURES_HIDE layer" & vbCrLf & vbCrLf & "Select OK to continue or," & vbCrLf & "Cancel to exit the command"

        If (MsgBox(sMsg, MsgBoxStyle.Exclamation + MsgBoxStyle.OkCancel, "Remove Feature") = MsgBoxResult.Cancel) Then Exit Sub

        ' Create the selection set
        'If ThisDrawing.PickfirstSelectionSet.Count > 0 Then
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.PickfirstSelectionSet. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ssetObj = ThisDrawing.PickfirstSelectionSet
        'End If

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        If ssetObj.Count = 0 Then
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj = ThisDrawing.SelectionSets.Add("Remove")
            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.SelectOnScreen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj.SelectOnScreen()
        End If

        'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj.Count. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For i = 0 To ssetObj.Count - 1

            'UPGRADE_WARNING: Couldn't resolve default property of object ssetObj().layer. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            ssetObj(i).layer = "Features_HIDE"
        Next i

        On Error Resume Next
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.SelectionSets. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.SelectionSets.Item("Remove").Delete()

    End Sub

    'UPGRADE_NOTE: ThisDrawing was upgraded to ThisDrawing_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
    Sub VpLayerOff(ByRef strLayer As String)
        ' make the layer non displayable (freeze) in the current Viewport
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim objEntity As AcadObject
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim objPViewport As AcadObject
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim objPViewport2 As AcadObject
        Dim XdataType(1) As Int16
        Dim XdataValue(1) As Int16
        Dim i As Short
        Dim Counter As Short
        Dim PT1 As Object

        ' Get the active ViewPort
        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ActivePViewport. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport = ThisDrawing.ActivePViewport

        ' Get the Xdata from the Viewport
        'UPGRADE_WARNING: Couldn't resolve default property of object objPViewport.GetXData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport.GetXData("ACAD", XdataType, XdataValue)

        For i = LBound(XdataType) To UBound(XdataType)
            On Error Resume Next
            Debug.Print(XdataType(i) & "=" & XdataValue(i))
            ' Look for frozen Layers in this viewport
            'UPGRADE_WARNING: Couldn't resolve default property of object XdataType(i). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            If XdataType(i) = 1003 Then
                ' Set the counter AFTER the position of the Layer frozen layer(s)
                Counter = i + 1
                ' If the layer is already in the frozen layers xdata of this viewport the
                ' exit this sub program
                'UPGRADE_WARNING: Couldn't resolve default property of object XdataValue(i). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If XdataValue(i) = strLayer Then Exit Sub
            End If
        Next

        ' If no frozen layers exist in this viewport then
        ' find the Xdata location 1002 and place the frozen layer infront of the "}"
        ' found at Xdata location 1002
        If Counter = 0 Then
            For i = LBound(XdataType) To UBound(XdataType)
                'UPGRADE_WARNING: Couldn't resolve default property of object XdataType(i). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If XdataType(i) = 1002 Then Counter = i - 1
            Next
        End If

        ' set the Xdata for the layer that is beeing frozen
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataType(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataType(Counter) = 1003
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataValue(Counter) = strLayer

        ReDim Preserve XdataType(Counter + 1)
        ReDim Preserve XdataValue(Counter + 1)

        ' put the first "}" back into the xdata array
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataType(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataType(Counter + 1) = 1002
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataValue(Counter + 1) = "}"

        ' Keep the xdata Array and add one more to the array
        ReDim Preserve XdataType(Counter + 2)
        ReDim Preserve XdataValue(Counter + 2)

        ' put the second "}" back into the xdata array
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataType(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataType(Counter + 2) = 1002
        'UPGRADE_WARNING: Couldn't resolve default property of object XdataValue(). Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        XdataValue(Counter + 2) = "}"

        ' Reset the Xdata on to the viewport
        'UPGRADE_WARNING: Couldn't resolve default property of object objPViewport.SetXData. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        objPViewport.SetXData(XdataType, XdataValue)

        ' notice that at this point NOTHING happens in the viewport to visibly show
        ' any changes to the viewport.
        ' flipping to a different layout or turning the Mview Off and On will display the
        ' Xdata changes to the viewport.
        ' See sub ViewPortUpdate for how to update the Viewport.

    End Sub


    <Autodesk.AutoCAD.Runtime.CommandMethod("WipeoutFix")> _
    Public Sub WipeoutFix()

        'UPGRADE_ISSUE: AcadBlock object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim blockObj As AcadBlock
        'UPGRADE_ISSUE: AcadDictionary object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim eDictionary As AcadDictionary
        Dim sentityObj As Object
        'UPGRADE_ISSUE: AcadEntity object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim ents As AcadEntity
        'UPGRADE_ISSUE: AcadObject object was not upgraded. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6B85A2A7-FE9F-4FBE-AA0C-CF11AC86A305"'
        Dim arr(0) As AcadObject
        Dim col(0) As ObjectId
        Dim bname As String
        Dim ed As Editor



        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Blocks. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        For Each blockObj In ThisDrawing.Blocks
            'UPGRADE_WARNING: Couldn't resolve default property of object blockObj.Name. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            bname = blockObj.Name
            'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Blocks. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            blockObj = ThisDrawing.Blocks.Item(bname)

            'Gxet an extension dictionary and, if necessary, add a SortentsTable object**this is done "inside" the block.
            'UPGRADE_WARNING: Couldn't resolve default property of object blockObj.GetExtensionDictionary. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            eDictionary = blockObj.GetExtensionDictionary '***this was the key
            ' Prevent failed GetObject calls from throwing an exception
            On Error Resume Next
            'UPGRADE_WARNING: Couldn't resolve default property of object eDictionary.GetObject. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
            sentityObj = eDictionary.GetObject("ACAD_SORTENTS")
            If Err.Number <> 0 Then
                If sentityObj Is Nothing Then
                    ' No SortentsTable object, so add one
                    'UPGRADE_WARNING: Couldn't resolve default property of object eDictionary.AddObject. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    sentityObj = eDictionary.AddObject("ACAD_SORTENTS", "AcDbSortentsTable")
                End If
                Err.Clear() '   just a way to get error numbers if/when they occur
            End If
            On Error GoTo 0
            'On Error GoTo Err_Wipeout
            'Dim psr As PromptSelectionResult
            'psr.Value.
            For Each ents In blockObj
                'UPGRADE_WARNING: Couldn't resolve default property of object ents.ObjectName. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                If ents.ObjectName = "AcDbWipeout" Then

                    'UPGRADE_WARNING: Couldn't resolve default property of object ents.ObjectID. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.ObjectIdToObject. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'

                    arr(0) = ThisDrawing.ObjectIdToObject(ents.ObjectID)
                    Using tr As Transaction = Application.DocumentManager.MdiActiveDocument.Database.TransactionManager.StartTransaction
                        Dim db As Database = Application.DocumentManager.MdiActiveDocument.Database

                        'Dim oldHandle As String = ents.Handle
                        'Dim newObjectId As ObjectId = db.GetObjectId(False, New Handle(Convert.ToInt32(oldHandle)), 0)
                        'col(0) = newObjectId
                        Dim bt As BlockTable = tr.GetObject(db.BlockTableId, OpenMode.ForWrite)
                        Dim btr As BlockTableRecord = tr.GetObject(bt(BlockTableRecord.ModelSpace), OpenMode.ForWrite)
                        For Each id As ObjectId In btr
                            Dim entity As Entity = DirectCast(tr.GetObject(id, OpenMode.ForRead), Entity)
                            If TypeOf entity Is Wipeout Then
                                col(0) = entity.ObjectId
                                Dim dot As DrawOrderTable = tr.GetObject(btr.DrawOrderTableId, OpenMode.ForWrite)
                                dot.MoveToBottom(New ObjectIdCollection(col))
                            ElseIf TypeOf entity Is BlockReference Then
                                Dim blkRef As BlockReference = TryCast(tr.GetObject(id, OpenMode.ForWrite), BlockReference)
                                Dim btrec As BlockTableRecord = DirectCast(tr.GetObject(blkRef.BlockTableRecord, OpenMode.ForRead), BlockTableRecord)
                                For Each eId As ObjectId In btrec
                                    Dim obj As DBObject = DirectCast(tr.GetObject(eId, OpenMode.ForRead), Entity)
                                    If TypeOf obj Is Wipeout Then
                                        col(0) = obj.ObjectId
                                        Dim dot As DrawOrderTable = tr.GetObject(btrec.DrawOrderTableId, OpenMode.ForWrite)
                                        dot.MoveToBottom(New ObjectIdCollection(col))
                                    End If
                                Next
                            End If
                        Next
                        tr.Commit()
                    End Using
                    'arr(0) = ents
                    'Move the wipeout object to the bottom
                    'UPGRADE_WARNING: Couldn't resolve default property of object sentityObj.MoveToBottom. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
                    'sentityObj.MoveToBottom(col)
                    'MsgBox ents.ObjectName
                End If
            Next ents
        Next blockObj

        'Err_Wipeout:
        'Debug.Print Err.Description

        'UPGRADE_WARNING: Couldn't resolve default property of object ThisDrawing.Regen. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6A50421D-15FE-4896-8A1B-2EC21E9037B2"'
        ThisDrawing.Regen(AcRegenType.acActiveViewport)
        'clean up anything that may be left over
        'UPGRADE_NOTE: Object sentityObj may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        sentityObj = Nothing
        'UPGRADE_NOTE: Object eDictionary may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        eDictionary = Nothing
        'UPGRADE_NOTE: Object blockObj may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        blockObj = Nothing
        'UPGRADE_NOTE: Object ents may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        ents = Nothing

    End Sub
End Class
