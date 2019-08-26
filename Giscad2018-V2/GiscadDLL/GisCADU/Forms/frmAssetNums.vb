Imports Autodesk.AutoCAD.Interop
Imports Autodesk.AutoCAD.Interop.Common
Imports Microsoft.VisualBasic
Imports ADODB
Imports Autodesk.AutoCAD.DatabaseServices
'Imports Autodesk.AutoCAD.ApplicationServices

Public Class frmAssetNums
    Dim gsDrgNumber As String
    Dim gsCAPNo As String
    Dim gsIEProjNo As String
    Dim gsDepotCode As String
    Dim gsOrgUnit(50, 2) As String
    Dim gbInternal As Boolean
    Dim gsURL As String
    Dim sLogFile As String
    Dim gsError As String

    Private Const MResizer As String = "ResizeGrab"
    Private WithEvents m_objResizer As Label
    Private m_sngLeftResizePos As Single
    Private m_sngTopResizePos As Single
    Private m_blnResizing As Single
    Private m_depot As String
    Private m_mdb As String

    Public ReadOnly Property ThisDrawing() As AcadDocument
        Get
            Dim tAcadDoc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

            Return CType(Autodesk.AutoCAD.ApplicationServices.DocumentExtension.GetAcadDocument(tAcadDoc), AcadDocument)
            ' Return Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.AcadDocument
        End Get
    End Property

    Public Sub SetDB(ByVal sMDB As String)

        'connect to the database
        m_mdb = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" & sMDB & ";" 'ReadOnly=True;Persist Security Info=False"
    End Sub

    Private Sub m_AddResizer()
        '
        ' add resizing control to bottom right hand corner of userform
        '
        m_objResizer = New Label
        'Dim FontName As String = "Marlett"
        'Dim FontSize As Integer = 11
        'Dim FS As New Font(FontName, FontSize, FontStyle.Bold, GraphicsUnit.Display)
        'm_objResizer.Font = FS
        m_objResizer.BackColor = Color.Transparent
        m_objResizer.AutoSize = True
        m_objResizer.BorderStyle = BorderStyle.None
        m_objResizer.Text = "o"
        m_objResizer.Cursor = Cursors.SizeNWSE
        m_objResizer.ForeColor = Color.FromArgb(100, 100, 100)
        m_objResizer.BringToFront()
        m_objResizer.Top = Me.m_objResizer.ClientRectangle.Height - m_objResizer.Height
        m_objResizer.Left = Me.m_objResizer.ClientRectangle.Width - m_objResizer.Width
        Me.Controls.Add(m_objResizer)
    End Sub

    Private Sub cmbDepot_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmbDepot.Click
        DoCheckMandatory()
    End Sub

    Private Sub cmbDepot_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmbDepot.TextChanged
        DoCheckMandatory()
    End Sub

    Private Sub cmbHost_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmbHost.Click
        DoCheckMandatory()
    End Sub

    Private Sub cmbHost_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmbHost.TextChanged
        DoCheckMandatory()
    End Sub

    Private Sub m_objResizer_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles m_objResizer.MouseDown
        If e.Button = 1 Then
            m_sngLeftResizePos = e.X
            m_sngTopResizePos = e.Y
            m_blnResizing = True
        End If
    End Sub

    Private Sub m_objResizer_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles m_objResizer.MouseMove
        If e.Button = 1 Then
            With m_objResizer
                '.Left(e.X - m_sngLeftResizePos, .Top + e.Y - m_sngTopResizePos)
                Me.Width = Me.Width + e.X - m_sngLeftResizePos
                Me.Height = Me.Height + e.Y - m_sngTopResizePos
                If Me.Width < 339 Then Me.Width = 339
                If Me.Height < 176 Then Me.Height = 176

                ' Addition of code to make sure sizing handle remains fixed in the bottom right hand corner
                .Left = Me.m_objResizer.ClientRectangle.Width - .Width
                .Top = Me.m_objResizer.ClientRectangle.Height - .Height
                txtLog.Width = Me.Width - 20
                txtLog.Height = Me.Height - (txtLog.Top + 40)
                pb.Top = txtLog.Top + txtLog.Height + 2
                pb.Width = txtLog.Width
            End With
        End If
    End Sub

    Private Sub m_objResizer_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles m_objResizer.MouseUp
        If e.Button = 1 Then
            m_blnResizing = False
        End If
    End Sub

    Private Sub cmdExit_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdExit.Click
        Me.Close()
    End Sub

    Sub DoLog(ByRef msg As String)

        txtLog.Text = txtLog.Text() & msg & vbCrLf
        Me.Refresh()
        DoLogFile(msg)
        txtLog.SelectionStart = 0
    End Sub

    Sub DoLogFile(ByVal msg As String)
        FileOpen(1, sLogFile, Microsoft.VisualBasic.OpenMode.Append)
        FileClose()
    End Sub

    Sub DoLogBlk(ByVal msg1 As String, ByRef blk As AcadBlockReference)
        Dim msg As String
        msg = msg1
        msg = Replace(msg, "b$", GetBlockName(blk))
        msg = Replace(msg, "x$", blk.InsertionPoint(0))
        msg = Replace(msg, "y$", blk.InsertionPoint(1))

        txtLog.Text = txtLog.Text & msg & vbCrLf
        Me.Refresh()

        DoLogFile(msg)
    End Sub

    Function IsValidParent(ByVal blockName As String, ByVal sPFNO As String) As Boolean
        Dim sFNO As String
        Dim rs As Recordset
        Dim db As Connection
        db = New Connection
        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)

        rs = New Recordset

        IsValidParent = False
        'SELECT assetnumbers.FNO
        'FROM assetnumbers
        'WHERE (((assetnumbers.BlockName)='SUB_PADMOUNT_EXT') AND ((assetnumbers.Parent) Is Null));

        rs.Open("SELECT FNO FROM assetnumbers where blockname = '" & blockName & "' and parent Is Null", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

        If Not rs.EOF Then
            sFNO = Trim(rs.Fields("FNO").Value & "")
            If Len(sFNO) > 0 Then
                If InStr(sPFNO, sFNO) > 0 Then
                    IsValidParent = True
                End If
            End If
        End If
        rs.Close()
    End Function

    Function DoFindParent(ByVal sParentFno As String, ByVal sParent As String, ByRef pblk As AcadBlockReference) As Boolean
        Dim blk As AcadBlockReference
        Dim i As Integer
        Dim sPrnt As String

        DoFindParent = False
        For i = 0 To ThisDrawing.ModelSpace.Count - 1
            If ThisDrawing.ModelSpace.Item(i).ObjectName = "AcDbBlockReference" Then
                blk = ThisDrawing.ModelSpace.Item(i)
                sPrnt = GetAttr_Renamed(blk, "PARENT")
                If sPrnt = sParent Then
                    'is it our parent
                    If IsValidParent(GetBlockName(blk), sParentFno) Then
                        pblk = ThisDrawing.ModelSpace.Item(i)
                        DoFindParent = True
                        Exit For
                    End If
                End If
            End If
        Next i

    End Function

    Function GetOrgUnit(ByVal sDepotCode As String) As String
        Dim i As Integer

        On Error GoTo Err_GetOrgUnit
        i = 0
        Do While Len(Trim(gsOrgUnit(i, 0))) > 0
            If gsOrgUnit(i, 0) = sDepotCode Then
                GetOrgUnit = gsOrgUnit(i, 2)
            End If
            i = i + 1
        Loop
        If Len(GetOrgUnit) > 0 Then

        End If
        Return GetOrgUnit
        Exit Function

Err_GetOrgUnit:
        DoLog("*** Error: Depot Code " & sDepotCode & " is invalid")

    End Function

    Function DoGetParentEquipNo(ByVal bname As String, ByVal pclass As String, ByVal sEquipRef As String) As String
        Dim i As Integer
        Dim eqp As Object
        Dim sEqRef As String

        ' This function will retrieve the equipment no
        DoGetParentEquipNo = ""
        'First check if we already have it.
        For i = 0 To lstEquipNo.Items.Count - 1
            eqp = Split(lstEquipNo.Items(i), ",")
            If eqp(0) = bname And eqp(1) = sEquipRef Then
                DoGetParentEquipNo = eqp(2)
                Exit Function
            End If

        Next i

        ' now query ellipse
        If Len(sEquipRef) < 5 Then
            sEqRef = pclass & StrDup(5 - Len(sEquipRef), "0") + sEquipRef
        Else
            sEqRef = pclass + sEquipRef
        End If
        DoGetParentEquipNo = DoGetEquipNo(sEqRef)

    End Function

    Function DoCreateAsset(ByRef blk As AcadBlockReference, ByRef rs As Recordset, ByRef rs1 As Recordset, ByVal sDepotCode As String, ByVal uname As String) As String
        Dim bAttrs As Object
        Dim pAttrs As Object
        Dim pblk As AcadBlockReference
        Dim bname As String, x As String, y As String
        Dim pbname As String, px As String, py As String
        Dim parent As String, parentFno As String
        Dim msg As String, f$, v$
        Dim pAssetNum As String
        Dim xml As String, paramXML As String, retxml As String, errXml As String
        Dim PlantCode0 As String, address As String, acccode As String, Class_renamed As String, equipgrpid As String, class18 As String
        Dim EquipNo As String, AssetNum As Object, plantno As String
        Dim a As Integer
        Dim StreetCode As String

        DoCreateAsset = ""

        bname = GetBlockName(blk)
        x = blk.InsertionPoint(0)
        y = blk.InsertionPoint(1)

        ' First check if this block is a child
        If Len(rs.Fields("PARENT").Value & "") > 0 Then
            parent = ""
            parentFno = rs.Fields("PARENT").Value & ""
            parent = GetAttr_Renamed(blk, "PARENT")

            If Len(parent) = 0 Then ' Block does not contain the Parent attribute
                msg = "*** Error: Attribute Parent is not defined or blank on b$ at x$/y$. Block will be ignored"
                DoLogBlk(msg, blk)
                Exit Function

            Else ' locate the parent
                If Not DoFindParent(parentFno, parent, pblk) Then ' Find the first valid parent
                    msg = "*** Error: Unable to locate Parent p$ for b$ at x$/y$. Block will be ignored"
                    msg = Replace(msg, "p$", parent)
                    DoLogBlk(msg, blk)
                    Exit Function
                Else
                    pbname = GetBlockName(pblk)
                    px = pblk.InsertionPoint(0)
                    py = pblk.InsertionPoint(1)
                    pAssetNum = GetAttr_Renamed(pblk, "ASSET_NUM")
                    If Len(pAssetNum) = 0 Then
                        'check for nested blocks
                        For a = 1 To 10
                            pAssetNum = GetAttr_Renamed(pblk, "ASSET_NUM" & Trim(Str(a)))
                            If Len(pAssetNum) > 0 Then Exit For
                        Next a
                    End If
                    If Len(pAssetNum) = 0 Then
                        msg = "*** Error: The asset number on parent pb$ at px$/py$ is empty, unable to process b$ at x$/y$"
                        msg = Replace(msg, "pb$", pbname)
                        msg = Replace(msg, "px$", px)
                        msg = Replace(msg, "py$", py)
                        DoLogBlk(msg, blk)
                        Exit Function
                    End If
                End If
            End If
        End If

        Dim newEquip As CEquip
        newEquip = New CEquip

        With newEquip
            .AccountCode = GetOrgUnit(sDepotCode) & rs.Fields("AccountCode").Value & ""
            .Class_Renamed = rs.Fields("Class").Value & ""
            .GroupID = rs.Fields("EquipGrpId").Value & ""
            .EquipClassif18 = rs.Fields("EquipClassIf18").Value & ""
            .PlantCode0 = rs.Fields("Plantcode0").Value & ""
            .EquipNoD1 = GetAddress(blk) ' Get the address from the feature
            '.StreetCode = DoGetStreetCode(address)
            .StreetCode = DoGetStreetCode(GetAddress(blk))

            If Len(.StreetCode) > 0 Then
                If SetAttrValue(blk, "StreetCode", .StreetCode) Then
                End If
            End If

            If Len(pAssetNum) > 0 Then
                .PlantCode1 = pAssetNum
                .ParentEquipNo = DoGetParentEquipNo(pbname, Class_renamed, pAssetNum)
            End If

            'Now do any nameplates
            If gbInternal Then
                .nameplates.Add("PROJECT", gsIEProjNo)
            Else
                .nameplates.Add("CAP", gsCAPNo)
            End If
            .nameplates.Add("DRAWING", gsDrgNumber)

            'now process any other paramaters out of the AssetCreation Table
            Do While Not rs1.EOF
                f$ = rs1.Fields("FieldName").Value & ""
                v$ = rs1.Fields("FieldValue").Value & ""
                If Len(f$) > 0 Then
                    If v$.Substring(0, 4) = "NPTAG_" Then
                        'If Left(v$, 4) = "NPTAG_" Then
                        v$ = GetAttr_Renamed(blk, Mid(v$, 5))
                        .nameplates.Add(f$, v$)
                    ElseIf v$.Substring(0, 4) = "TAG_" Then
                        'ElseIf Left(v$, 4) = "TAG_" Then
                        v$ = GetAttr_Renamed(blk, Mid(v$, 5))
                        .Attributes.Add(f$, v$)
                    Else
                        .Attributes.Add(f$, v$)
                    End If
                End If
                rs1.MoveNext()
            Loop

        End With

        If Not EquipmentCreate(newEquip) Then
            errXml = GetError()
            msg = "*** Error: unable to generate asset number for b$ at x$/y$ - " & errXml
            DoLogBlk(msg, blk)
        Else
            DoCreateAsset = newEquip.AssetNum
            lstEquipNo.Items.Add(bname & "," & newEquip.AssetNum & "," & newEquip.EquipNo) ' Store this features equipno for later use when getting the parent

        End If

    End Function

    Private Function EquipmentCreate(ByRef oEquip As CEquip) As Boolean
        Dim xml As String
        Dim paramXML As String
        Dim retxml As String
        Dim errXml As String
        Dim msg As String
        Dim plantno As String
        Dim AssetNum() As String

        EquipmentCreate = False

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & CreateXML()

        With oEquip
            xml = xml & vbCrLf
            xml = xml & "<Parameter name=""EquipClass"">" & .Class_Renamed & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""PlantCode0"">" & .PlantCode0 & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipGrpId"">" & .GroupID & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""AccountCode"">" & .AccountCode & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""ActiveFlag"">" & .ActiveFlag & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""InputBy"">" & txtUsername.Text & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipStatus"">" & .Status & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipNoD1"">" & CharReplace(.EquipNoD1) & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipClassif6"">" & .Depot & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipClassif18"">" & .EquipClassif18 & "</Parameter>" & vbCrLf
            xml = xml & "<Parameter name=""EquipLocation"">" & .StreetCode & "</Parameter>" & vbCrLf


            ' Add in parent detail if child
            If Len(.PlantCode1) > 0 Then
                xml = xml & "<Parameter name=""PlantCode1"">" & .PlantCode1 & "</Parameter>"
                xml = xml & "<Parameter name=""ParentEquip"">" & .ParentEquipNo & "</Parameter>"
            End If

            'now process any other paramaters out of the AssetCreation Table
            Dim attr As CNamePlate
            For Each attr In .Attributes
                paramXML = "<Parameter name=""f$"">v$</Parameter>" & vbCrLf
                If Len(Trim(attr.FieldName)) > 0 Then
                    paramXML = Replace(paramXML, "f$", attr.FieldName)
                    paramXML = Replace(paramXML, "v$", CharReplace(attr.FieldValue))
                    xml = xml & paramXML
                End If
            Next attr

        End With


        'close of the request
        xml = xml & "</Request></Service>"

        xml = xml & XMLRequestEnd()
        xml = xml & XMLFooter()

        retxml = XMLExecute(xml, gsURL)

        'First check for errors
        If InStr(1, retxml, "Error") > 0 Then
            EquipmentCreate = False
            gsError = retxml
            Exit Function

        Else
            'pull out the EquipNo from the returned XML
            oEquip.EquipNo = Mid(retxml, InStr(1, retxml, "EquipNo") + 9, 12)
            plantno = Mid(retxml, InStr(1, retxml, "PlantNo") + 9, 12)
            plantno = plantno.Substring(0, InStr(plantno, "<") - 1)
            'plantno = Left(plantno, InStr(plantno, "<") - 1)
            AssetNum = GetOpId(oEquip.EquipNo, gsURL)

            If UBound(AssetNum) = 0 Then
                AssetNum(0) = Mid(plantno, 3)
                Do While AssetNum(0).Substring(0, 1) = 0
                    'Do While Left(AssetNum(0), 1) = 0
                    AssetNum(0) = Mid(AssetNum(0), 2)
                Loop

                UpdateOpId(oEquip.EquipNo, plantno)
            Else
                AssetNum(0) = AssetNum(1)
            End If

            UpdateNamePlate(oEquip.EquipNo, gsURL, oEquip.nameplates)
            UpdateColloquials(oEquip.EquipNo, gsURL, gbInternal, gsCAPNo, gsDrgNumber)
            If gbInternal Then
                If Len(gsIEProjNo) > 0 Then
                    UpdateAlternateRefs(oEquip.EquipNo, gsURL, gsIEProjNo)
                End If
            End If


        End If

        oEquip.AssetNum = AssetNum(0)
        EquipmentCreate = True

    End Function

    Private Function UpdateNamePlate(ByVal sEquipNo As String, ByVal sURL As String, ByVal nameplates As CNamePlates) As Boolean
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String
        Dim np As CNamePlate

        UpdateNamePlate = False

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""NAMEPLATE.Modify"">" & _
                 "<ReplyParameters>" & _
                     "<Parameter name=""EquipNo""/>" & _
                 "</ReplyParameters>"

        Dim i As Integer
        For Each np In nameplates
            i = i + 1
            xml = xml & "<Request id=""ID0000"" & trim(str(i)) & "">" _
                & "<Parameter name=""EquipNo"">" & sEquipNo & "</Parameter>" _
                & "<Parameter name=""AttributeName"">" & np.FieldName & "</Parameter>" _
                & "<Parameter name=""AttributeValue"">" & np.FieldValue & "</Parameter>"
        Next np

        xml = xml & "</Request></Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: Unable to update Nameplate details - " & xmlerr
            'DoLog msg
            Exit Function
        End If

        UpdateNamePlate = True
        Exit Function

Err_Routine:

    End Function

    Function UpdateColloquials(ByVal sEquipNo As String, ByVal sURL As String, ByVal gbInternal As Boolean, ByVal sProjectNo As String, ByVal gsDrgNumber As String) As Boolean
        Dim ref As String
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String

        UpdateColloquials = False

        ref = "DRG" & gsDrgNumber
        If Not gbInternal Then
            ref = ref & StrDup(24 - Len(ref), " ") & sProjectNo
        End If

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""EQUIPMENT.UpdateColloquial"">" & _
                 "<ReplyParameters>" & _
                     "<Parameter name=""EquipNo""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID00001"">" & _
                     "<Parameter name=""EquipNo"">" & sEquipNo & "</Parameter>" & _
                     "<Parameter name=""ColloqEq"">" & ref & "</Parameter>" & _
                 "</Request>" & _
                 "</Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: Unable to update Colloquials - " & xmlerr
            DoLog(msg)
            Exit Function
        End If

        UpdateColloquials = True
        Exit Function

Err_Routine:


    End Function

    Function UpdateAlternateRefs(ByVal sEquipNo As String, ByVal sURL As String, ByVal sProjectNo As String) As Boolean
        Dim ref As String
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String

        UpdateAlternateRefs = False

        ref = "PROJECT" & StrDup(26, " ") & sProjectNo
        ref = ref & StrDup(66 - Len(ref), " ")

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""EQUIPMENT.CreateAlternateref"">" & _
                 "<ReplyParameters>" & _
                     "<Parameter name=""EquipNo""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID00001"">" & _
                     "<Parameter name=""EquipNo"">" & sEquipNo & "</Parameter>" & _
                     "<Parameter name=""AlternateRef"">" & ref & "</Parameter>" & _
                 "</Request>" & _
                 "</Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            msg = "*** Error: Unable to update AlternateRef details - " & xmlerr
            DoLog(msg)
            Exit Function
        End If

        UpdateAlternateRefs = True
        Exit Function

Err_Routine:


    End Function

    Function UpdateOpId(ByVal sEquipNo As String, ByVal sPlantNo As String) As Boolean
        Dim ref As String
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String

        UpdateOpId = False

        ref = "OP_ID   Operating Number         " & sPlantNo

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""EQUIPMENT.CreateAlternateref"">" & _
                 "<ReplyParameters>" & _
                     "<Parameter name=""EquipNo""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID00001"">" & _
                     "<Parameter name=""EquipNo"">" & sEquipNo & "</Parameter>" & _
                     "<Parameter name=""AlternateRef"">" & ref & "</Parameter>" & _
                 "</Request>" & _
                 "</Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: Unable to update OpId - " & xmlerr
            DoLog(msg)
            Exit Function
        End If

        UpdateOpId = True
        Exit Function

Err_Routine:


    End Function

    Function GetOpId(ByVal EquipNo As String, ByVal sURL As String) As Object
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String
        Dim tmp() As String
        Dim i As Integer

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""EQUIPMENT.RetrieveAlternateRefs"">" & _
                 "<ReplyParameters>" & _
                 "<Parameter name=""EquipNo""/>" & _
                 "<Parameter name=""AlternateRef""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID00001"">" & _
                 "<Parameter name=""EquipNo"">" & EquipNo & "</Parameter>" & _
                "</Request></Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: unable to OP_ID for b$ at x$/y$. " & xmlerr
            'DoLog msg
            Exit Function

        Else
            'pull out the OP_ID from the returned XML

            tmp = Split(retxml, "OP_ID   Operating Number")
            'For i = LBound(tmp) To UBound(tmp)
            '    If i > 0 Then
            '        tmp(i - 1) = Trim(Mid(tmp(i), 3, 8))
            '        If Left(tmp(i - 1), 1) = "0" Then
            '            tmp(i - 1) = Mid(tmp(i - 1), 2)
            '        End If
            '    End If
            '    If i = UBound(tmp) Then
            '        tmp(i) = ""
            '    End If
            'Next i

            For i = LBound(tmp) To UBound(tmp)
                'Debug.Print tmp(i)
                tmp(i) = Trim(tmp(i))
                tmp(i) = Trim(Mid(tmp(i), 3, 8))
                Do While tmp(i).Substring(0, 1) = "0"
                    ' Do While Left(tmp(i), 1) = "0"
                    tmp(i) = Mid(tmp(i), 2)
                Loop
                Debug.Print(tmp(i))
            Next i

            GetOpId = tmp
        End If

    End Function

    Function GetError() As String
        Dim sTmp As String
        Dim sxml As String

        sxml = gsError

        If sxml.Contains("<Errors>") Then
            sTmp = Mid(sxml, InStr(1, sxml, "<Errors>"))
            sTmp = Mid(sTmp, 1, InStr(1, sTmp, "</Errors>") - 1)

            sTmp = Replace(sTmp, "</Error><Error>", vbCrLf)
            sTmp = Replace(sTmp, "</Text>", "")
            sTmp = Replace(sTmp, "</Field>", ")")
            sTmp = Replace(sTmp, "<Field>", "(")
            sTmp = Replace(sTmp, "&quot;", "")
            sTmp = Replace(sTmp, "<Errors>", "")
            sTmp = Replace(sTmp, "</Error>", "")
            sTmp = Replace(sTmp, "<Text>", "")
        Else
            sTmp = sxml.Substring(sxml.LastIndexOf("-") + 1).Trim()

        End If

        GetError = sTmp

    End Function

    Function DoGetEquipNo(ByVal sEquipRef As String) As String
        Dim ref As String
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""EQUIPMENT.Retrieve"">" & _
                 "<ReplyParameters>" & _
                 "<Parameter name=""EquipNo""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID00001"">" & _
                 "<Parameter name=""EquipRef"">" & sEquipRef & "</Parameter>" & _
                 "<Parameter name=""EquipRefSearchMethod"">E</Parameter>" & _
                "</Request></Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: unable to retrieve equip no for b$ at x$/y$. " & xmlerr
            'DoLog msg
            Exit Function

        Else
            'pull out the OP_ID from the returned XML
            '<Parameter name="EquipNo">
            DoGetEquipNo = Mid(retxml, InStr(1, retxml, "<Parameter name=""EquipNo"">") + 26, 12)
        End If

    End Function

    Function DoGetStreetCode(ByVal sAddress As String) As String
        Dim ref As String
        Dim vSC As Object
        Dim xml As String, retxml As String, xmlerr As String
        Dim msg As String

        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & "<Service name=""TABLE.Retrieve"">" & _
                 "<ReplyParameters>" & _
                 "<Parameter name=""TableCode""/>" & _
                 "</ReplyParameters>" & _
                 "<Request id=""ID10001"">" & _
                 "<Parameter name=""TableType"">EL</Parameter>" & _
                 "<Parameter name=""CodeDescription""/>" & _
                 "<Parameter name=""SearchMethod"">C</Parameter>" & _
                "</Request></Service></EllipseRequest></SOAP-ENV:Body></SOAP-ENV:Envelope>"

        retxml = XMLExecute(xml, gsURL)

        If InStr(1, retxml, "<Errors>") > 0 Then
            gsError = retxml
            xmlerr = GetError()
            msg = "*** Error: unable to streetcode for b$ at x$/y$. " & xmlerr
            'DoLog msg
            Exit Function

        Else
            'pull out the OP_ID from the returned XML
            '<Parameter name="EquipNo">
            vSC = Split(Mid(retxml, InStr(1, retxml, "<Parameter name=""TableCode"">") + 28, 10), "<")
            DoGetStreetCode = vSC(0)
        End If

    End Function

    Private Sub cmdGenerate_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdGenerate.Click
        Dim ss As AcadSelectionSet
        Dim i As Integer

        Me.Cursor = Cursors.WaitCursor
        DoLog(StrDup(50, "-"))

        gsURL = GetIniString("Ellipse", cmbHost.Items(cmbHost.SelectedIndex))

        gsDepotCode = GetDepotCode(cmbDepot.Text)

        'If Len(gsDepotCode) = 0 Then
        '    Me.MousePointer = fmMousePointerDefault
        '    DoLog "*** Error: Depotcode has not been specified. Please update Titleblock attributes"
        '    Exit Sub
        'End If

        If ThisDrawing.SelectionSets.Count = 0 Then
            DoLog("*** Error: No assets to process")
            Exit Sub
        End If

        pb.Visible = True
        'pb.Max = ThisDrawing.SelectionSets.Count

        ' Loop through each selectionset and create the equip
        For i = ThisDrawing.SelectionSets.Count To 1 Step -1
            ss = ThisDrawing.SelectionSets(i - 1)
            If ss.Count > 0 Then
                DoLog("Processing " & ss.Name)
            End If

            If UCase(ss.Name) = "SUBSWITCH" Then
                DoProcessSubSwitches(ss)

            Else
                DoProcessEquipment(ss)

            End If
            If pb.Value < pb.Maximum Then pb.Value = pb.Value + 1

        Next i

        pb.Value = 0
        pb.Visible = False

        ThisDrawing.Regen(AcRegenType.acAllViewports)

        Me.Cursor = Cursors.Default
        MsgBox("Generate numbers complete", vbOKOnly, "Asset Numbering Utility")

        DoLogFile("Generate numbers complete" & vbCrLf & Now)
        DoLog(vbCrLf & vbCrLf)
    End Sub

    Private Sub cmdScan_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdScan.Click
        Dim blk As AcadBlockReference
        Dim attr As AcadAttribute
        Dim ps As AcadPaperSpace
        Dim varAttributes() As Object
        Dim anum As String ' temporary var for asset num
        Dim ss As AcadSelectionSet
        Dim ssobjs(0) As AcadEntity
        Dim i As Integer, j As Integer
        Dim assetcount As Integer
        Dim msg As String
        Dim rs As Recordset
        Dim db As Connection
        db = New Connection
        Dim sBName As String
        Dim _blkName As String = String.Empty


        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)


        rs = New Recordset
        ' scan the drawing for information
        txtLog.Text = ""
        DoLog("Scanning drawing " & ThisDrawing.FullName)
        DoLoadOrgUnits()


        For i = ThisDrawing.SelectionSets.Count - 1 To 0 Step -1
            ThisDrawing.SelectionSets(i).Delete()
        Next i


        'Dim doc As Autodesk.AutoCAD.ApplicationServices.Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument

        'Using trans As Transaction = doc.Database.TransactionManager.StartTransaction()
        'Dim bt As BlockTable = DirectCast(trans.GetObject(doc.Database.BlockTableId, Autodesk.AutoCAD.DatabaseServices.OpenMode.ForRead), BlockTable)
        ''Dim btr As BlockTableRecord = trans.GetObject(SymbolUtilityServices.GetBlockPaperSpaceId(doc.Database), Autodesk.AutoCAD.DatabaseServices.OpenMode.ForRead)
        'Dim btr As BlockTableRecord = trans.GetObject(bt(BlockTableRecord.PaperSpace), Autodesk.AutoCAD.DatabaseServices.OpenMode.ForRead)
        'Dim idcol As ObjectIdCollection = btr.GetBlockReferenceIds(True, True)
        'MessageBox.Show(ThisDrawing.PaperSpace.Count & "-" & idcol.Count)
        'For Each id As ObjectId In btr
        '    Dim ent As Entity = DirectCast(trans.GetObject(id, Autodesk.AutoCAD.DatabaseServices.OpenMode.ForWrite), Entity)
        '    blk = TryCast(ent, AcadBlockReference)
        '    If (blk Is Nothing) Then
        '        Continue For
        '    End If


        Dim ObjPaperSpace As IEnumerable = From objId In ThisDrawing.PaperSpace.OfType(Of AcadBlockReference)()
                                       Select objId

        ''todo: break out of loop once the title block info is found?
        'For i = 0 To ThisDrawing.PaperSpace.Count - 1
        'If ThisDrawing.PaperSpace.Item(i).ObjectName = "AcDbBlockReference" Then
        For Each obj As AcadBlockReference In ObjPaperSpace
            blk = obj 'ThisDrawing.PaperSpace.Item(i)
            _blkName = GetBlockName(blk)
            If _blkName.Length >= 12 Then
                If _blkName.Substring((_blkName.Length - "_FRAME_TITLE".Length), "_FRAME_TITLE".Length) = "_FRAME_TITLE" Then
                    'If Right(GetBlockName(blk), Len("_FRAME_TITLE")) = "_FRAME_TITLE" Then
                    varAttributes = blk.GetAttributes
                    For j = LBound(varAttributes) To UBound(varAttributes)
                        'DoLog varAttributes(j).TagString & " = " & varAttributes(j).TextString & vbCrLf
                        Select Case UCase$(varAttributes(j).TagString)
                            Case "NUM"
                                DoLog("Drawing Number = " & varAttributes(j).TextString)
                                gsDrgNumber = varAttributes(j).TextString
                            Case "CAP_NO"
                                DoLog("CAP No = " & varAttributes(j).TextString)
                                gsCAPNo = varAttributes(j).TextString
                            Case "IE_NUM"
                                DoLog("Project No = " & varAttributes(j).TextString)
                                gsIEProjNo = varAttributes(j).TextString
                            Case "PROJECTTYPE"
                                gbInternal = IIf(UCase(varAttributes(j).TextString) = "EXTERNAL", False, True)
                        End Select
                    Next j
                End If
            End If
            'End If
            'Next i
        Next


        DoLog(StrDup(50, "-"))


        rs.Open("SELECT DISTINCT FeatureName FROM assetnumbers", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

        i = 0
        Do While Not rs.EOF
            ThisDrawing.SelectionSets.Add(rs.Fields("FeatureName").Value)
            'Debug.Print(ThisDrawing.SelectionSets(i).Name)
            i = i + 1
            rs.MoveNext()
        Loop
        rs.Close()
        rs = Nothing



        pb.Minimum = 0
        pb.Maximum = ThisDrawing.ModelSpace.Count
        pb.Value = 0


        ''btr = trans.GetObject(SymbolUtilityServices.GetBlockModelSpaceId(doc.Database), Autodesk.AutoCAD.DatabaseServices.OpenMode.ForRead)
        'btr = trans.GetObject(bt(BlockTableRecord.ModelSpace), Autodesk.AutoCAD.DatabaseServices.OpenMode.ForRead)
        'idcol = btr.GetBlockReferenceIds(True, True)
        'MessageBox.Show(ThisDrawing.ModelSpace.Count & "-" & idcol.Count)
        'For Each id As ObjectId In btr
        '    Dim ent As Entity = DirectCast(trans.GetObject(id, Autodesk.AutoCAD.DatabaseServices.OpenMode.ForWrite), Entity)

        '    blk = TryCast(ent, AcadBlockReference)
        '    If (blk Is Nothing) Then
        '        Continue For
        '    End If
        'blk = ent

        Dim ObjModelSpace As IEnumerable = From objId In ThisDrawing.ModelSpace.OfType(Of AcadBlockReference)()
                                               Select objId

        'Dim dbTxt As IEnumerable = From objId In ThisDrawing.ModelSpace.OfType(Of AcadBlockReference)()
        '                                        Select objId.ObjectID



        '' Now locate all blocks with an empty assetnum attribute
        'For i = 0 To ThisDrawing.ModelSpace.Count - 1
        '    '    'Debug.Print(ThisDrawing.ModelSpace(i).ObjectName)
        '    If ThisDrawing.ModelSpace(i).ObjectName = "AcDbBlockReference" Then
        For Each obj As AcadBlockReference In ObjModelSpace
            Try
                blk = obj 'ThisDrawing.ModelSpace(i)
                sBName = GetBlockName(blk)
                If sBName.Length > 4 Then
                    If sBName.Substring(Len(sBName) - Len("_NEW"), Len("_NEW")) = "_NEW" Or sBName.Equals("FUSE_SAVER", StringComparison.InvariantCultureIgnoreCase) Then
                        'If Right(sBName, Len("_NEW")) = "_NEW" Then
                        'Debug.Print(sBName)
                        varAttributes = blk.GetAttributes
                        For j = LBound(varAttributes) To UBound(varAttributes)
                            'Debug.Print(varAttributes(j).TagString)
                            If varAttributes(j).TagString.ToString.Length >= 9 Then
                                If varAttributes(j).TagString.Substring(0, Len("ASSET_NUM")) = "ASSET_NUM" Then
                                    'If Left(varAttributes(j).TagString, Len("ASSET_NUM")) = "ASSET_NUM" Then
                                    anum = varAttributes(j).TextString
                                    'Check if asset number is blank for all Xs.
                                    If ((Len(anum)) = 0) Or (anum = StrDup(Len(anum), "X")) Then

                                        ' Search for block in database and place in appropriate selection set
                                        rs = New Recordset
                                        rs.Open("select FeatureName from assetnumbers where blockname = '" & sBName & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

                                        If Not rs.EOF Then
                                            ' check for mandatory attributes
                                            If Len(GetAttr_Renamed(blk, "STREETNAME")) = 0 Or Len(GetAttr_Renamed(blk, "SUBURB")) = 0 Then
                                                msg = "*** Error: Asset number will not be generated for b$ at x$/y$. Street and/or suburb are mandatory"
                                                DoLogBlk(msg, blk)

                                            Else

                                                ss = ThisDrawing.SelectionSets.Item(rs.Fields("FeatureName").Value)
                                                ssobjs(0) = blk
                                                ss.AddItems(ssobjs)
                                            End If
                                            Exit For
                                        End If
                                    End If
                                End If
                            End If
                        Next j
                    End If
                End If
            Catch ex As Exception
                MsgBox(ex.Message)
            End Try
            'End If
            Application.DoEvents()
            If pb.Value < pb.Maximum Then
                pb.Value += 1
            End If
            'Next i
        Next

        While (pb.Value < pb.Maximum)
            pb.Value += 1
        End While


        ' Display selection sets
        assetcount = 0
        For i = ThisDrawing.SelectionSets.Count To 1 Step -1
            DoLog(ThisDrawing.SelectionSets(i - 1).Name & " = " & ThisDrawing.SelectionSets(i - 1).Count)
            'Debug.Print(ThisDrawing.SelectionSets(i).Name)
            assetcount = assetcount + ThisDrawing.SelectionSets(i - 1).Count

        Next i

        If assetcount = 0 Then
            DoLog("*** Error: There are no assets to number")
            MsgBox("Error: There are no assets to number", vbExclamation, "Asset Numbering ERROR")
        Else
            pb.Maximum = assetcount
        End If
        'End Using
    End Sub

    Sub DoCheckMandatory()
        Dim bEnabled As Boolean

        bEnabled = True

        If Len(Trim(txtUsername.Text)) = 0 Then bEnabled = False
        If Len(Trim(txtPassword.Text)) = 0 Then bEnabled = False
        'If Len(Trim(txtPosition.text)) = 0 Then bEnabled = False
        If Len(Trim(txtDistrict.Text)) = 0 Then bEnabled = False
        If Len(cmbHost.Text) = 0 Then bEnabled = False
        If Len(cmbDepot.Text) = 0 Then bEnabled = False

        cmdGenerate.Enabled = bEnabled
    End Sub

    Private Sub txtDistrict_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtDistrict.TextChanged
        DoCheckMandatory()
    End Sub

    Private Sub txtPassword_Leave(sender As Object, e As System.EventArgs) Handles txtPassword.Leave
        txtPassword.Text = UCase(txtPassword.Text)
        txtPassword.Text.ToString.Reverse()
        DoCheckMandatory()
    End Sub

    Private Sub txtPassword_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtPassword.TextChanged

    End Sub

    Private Sub txtUsername_Leave(sender As Object, e As System.EventArgs) Handles txtUsername.Leave
        txtUsername.Text = UCase(txtUsername.Text)
        txtUsername.Text.ToString.Reverse()
        DoCheckMandatory()
    End Sub

    Private Sub txtUsername_TextChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtUsername.TextChanged

    End Sub

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Dim bSettingsLoaded As Boolean
        Dim section As String, delim As String
        Dim s As Object
        Dim j As Integer, i As Integer
        Dim profile As String

        SetDB(Trim(GetIniString("Database", "LOCAL_MDB_LOCATION")))
        SaveSetting("IE_TOOLBOX", "LOGIN", "DISTRICT", "INTG")
        txtDistrict.Text = GetSetting("IE_TOOLBOX", "LOGIN", "DISTRICT")
        txtPosition.Text = GetSetting("IE_TOOLBOX", "LOGIN", "POSITION", "")
        txtUsername.Text = GetSetting("IE_TOOLBOX", "LOGIN", "USERNAME", "")
        profile = GetSetting("IE_TOOLBOX", "LOGIN", "PROFILE", "")
        m_depot = GetSetting("IE_TOOLBOX", "LOGIN", "DEPOT", "")

        section = GetIniString("Ellipse", "")
        delim = section.Substring(Len(section) - 1, 1)
        'delim = Right(section, 1)
        section = section.Substring(0, Len(section) - 1)
        'section = Left(section, Len(section) - 1)
        s = Split(section, delim)
        For j = LBound(s) To UBound(s)
            cmbHost.Items.Add(s(j))
            If s(j) = profile Then cmbHost.SelectedItem = j
        Next j

        'For i = ThisDrawing.SelectionSets.Count - 1 To 0 Step -1
        '    ThisDrawing.SelectionSets(i).Delete
        'Next i

        'DoLoadOrgUnits depot
        m_AddResizer()

        ' Open the log file
        sLogFile = ThisDrawing.FullName.Substring(0, Len(ThisDrawing.FullName) - 4) & "_Assets.log"
        'sLogFile = Left(ThisDrawing.FullName, Len(ThisDrawing.FullName) - 4) & "_Assets.log"
        DoLogFile(StrDup(50, "=") & vbCrLf & Now)
    End Sub

    Public Sub DoLoadOrgUnits()
        Dim i As Integer
        Dim rs As Recordset
        Dim db As Connection
        db = New Connection
        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)

        rs = New Recordset
        'rs.Open "select Depot,DepotName,OrgUnit from OrgUnit Order by DepotName", db, adOpenStatic, adLockReadOnly
        rs.Open("select * from OrgUnit", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)
        i = -1
        Do While Not rs.EOF
            i = i + 1
            gsOrgUnit(i, 0) = rs.Fields(0).Value & ""
            gsOrgUnit(i, 1) = rs.Fields(1).Value & ""
            gsOrgUnit(i, 2) = rs.Fields(2).Value & ""
            cmbDepot.Items.Add(gsOrgUnit(i, 0) & " (" & gsOrgUnit(i, 1) & ")")
            If gsOrgUnit(i, 1) & " (" & gsOrgUnit(i, 2) & ")" = m_depot Then
                cmbDepot.SelectedIndex = i
            End If
            rs.MoveNext()
            'ReDim gsOrgUnit(i, 1)
        Loop
        rs.Close()
    End Sub

    Function GetDepotCode(ByVal sName As String) As String
        Dim i As Integer

        GetDepotCode = ""
        For i = LBound(gsOrgUnit) To UBound(gsOrgUnit)
            'i = i + 1
            If gsOrgUnit(i, 0) & " (" & gsOrgUnit(i, 1) & ")" = sName Then
                GetDepotCode = gsOrgUnit(i, 0)
            End If
        Next i

    End Function


    Function GetAssetNum(ByRef blk As AcadBlockReference, ByVal sASSET_NUM As String) As String
        Dim anum As String
        anum = GetAttr_Renamed(blk, sASSET_NUM)
        If Len(anum) = 0 Or anum = StrDup(Len(anum), "X") Then
            anum = ""
        End If
        GetAssetNum = anum

    End Function

    Sub DoProcessEquipment(ByRef ss As AcadSelectionSet)
        Dim blk As AcadBlockReference
        Dim anum As String
        Dim attrAssetNum As String
        Dim i As Integer
        Dim bname As String
        Dim x As Double
        Dim y As Double
        Dim msg As String
        Dim AssetNum As String
        Dim rs As Recordset
        Dim rs1 As Recordset
        Dim db As Connection
        db = New Connection
        Dim sFNO As String

        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)

        For Each blk In ss
            bname = GetBlockName(blk)
            rs = New Recordset
            'rs.Open "SELECT * FROM assets where blockname = '" & bName & "' ORDER BY AssetNumbers.ASSET_NUM", db, adOpenStatic, adLockReadOnly
            rs.Open("SELECT * FROM assetnumbers where blockname = '" & bname & "' ORDER BY ASSET_NUM", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)
            rs1 = New Recordset
            sFNO = rs.Fields("FNO").Name & ""
            rs1.Open("SELECT * FROM assetcreation where fno = '" & sFNO & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

            ''If not re.eof
            While Not rs.EOF
                attrAssetNum = rs.Fields("ASSET_NUM").Name
                ' Ignore if the block already has an ASSET_NUM - This could happen when processing substation switches
                If Len(GetAssetNum(blk, attrAssetNum)) = 0 Then ' If theAsset number exists then ignore this one.
                    x = blk.InsertionPoint(0)
                    y = blk.InsertionPoint(1)

                    If Len(GetAttr_Renamed(blk, "STREETNAME")) = 0 Or Len(GetAttr_Renamed(blk, "SUBURB")) = 0 Then
                        msg = "*** Error: Unable to generate asset number for b$ at x$/y$. Street and/or suburb are mandatory"
                        DoLogBlk(msg, blk)

                    Else

                        AssetNum = DoCreateAsset(blk, rs, rs1, gsDepotCode, txtUsername.Text)
                        'AssetNum = "853306"

                        If Len(AssetNum) > 0 Then
                            If SetAttrValue(blk, attrAssetNum, AssetNum) Then
                                DoLog("Asset Number " & AssetNum & " assigned to " & bname)
                            Else
                                DoLog("*** Error: Unable to assign asset Number " & AssetNum & " to " & bname)
                            End If
                        End If
                        ''else
                        ''msg = "*** Error: Unable to generate asset number for b$ at x$/y$. Error in database configuration."
                        ''DoLogBlk msg, blk
                    End If
                End If
                On Error Resume Next
                rs.MoveNext()
            End While
        Next blk

    End Sub

    Sub DoProcessSubSwitches(ByRef ss As AcadSelectionSet)

        Dim blk As AcadBlockReference
        Dim bAttrs As Object
        Dim pAttrs As Object
        Dim pblk As AcadBlockReference
        Dim bname As String, x As String, y As String
        Dim pbname As String, px As String, py As String
        Dim parent As String, parentFno As String
        Dim msg As String, f$, v$
        Dim pAssetNum As String
        Dim xml As String, paramXML As String, retxml As String, errXml As String
        Dim PlantCode0 As String, address As String, acccode As String, Class_1 As String, equipgrpid As String, class18 As String
        Dim EquipNo As String, AssetNum As Object, plantno As String
        Dim cblk As AcadBlockReference
        Dim ssobjs(0) As AcadEntity
        Dim cs As AcadSelectionSet
        Dim i As Integer
        Dim qry As String
        Dim anum As String

        Dim rs As Recordset
        Dim db As Connection
        db = New Connection
        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)

        rs = New Recordset

        For Each blk In ss
            'qry = "SELECT * FROM assets where blockname = '" & GetBlockName(blk) & "'"
            'qry = "SELECT * FROM assetNumber where blockname = '" & GetBlockName(blk) & "'"
            qry = "SELECT * FROM assetNumbers where blockname = '" & GetBlockName(blk) & "'"

            rs.Open(qry, db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)
            If Not rs.EOF Then
                If Len(GetAssetNum(blk, rs.Fields("ASSET_NUM").Name)) = 0 Then ' ignore block already numbered


                    parent = ""
                    parentFno = rs.Fields("PARENT").Value & ""
                    parent = GetAttr_Renamed(blk, "PARENT")

                    If Len(parent) = 0 Then ' Block does not contain the Parent attribute
                        msg = "*** Error: Attribute Parent is not defined or blank on b$ at x$/y$. Block will be ignored"
                        DoLogBlk(msg, blk)
                        Exit Sub

                    Else ' locate the parent
                        If Not DoFindParent(parentFno, parent, pblk) Then
                            msg = "*** Error: Unable to locate Parent p$ for b$ at x$/y$. Block will be ignored"
                            msg = Replace(msg, "p$", parent)
                            DoLogBlk(msg, blk)
                            Exit Sub
                        Else
                            pbname = GetBlockName(pblk)
                            px = pblk.InsertionPoint(0)
                            py = pblk.InsertionPoint(1)
                            ' Get the Asset number attribute of the parent
                            pAssetNum = GetAttr_Renamed(pblk, rs.Fields("ASSET_NUM").Name)
                            If Len(pAssetNum) = 0 Then
                                pAssetNum = GetAttr_Renamed(pblk, "ASSET_NUM1")
                            End If
                            If Len(pAssetNum) = 0 Then
                                msg = "*** Error: The asset number on parent pb$ at px$/py$ is empty, unable to process b$ at x$/y$"
                                msg = Replace(msg, "pb$", pbname)
                                msg = Replace(msg, "px$", px)
                                msg = Replace(msg, "py$", py)
                                DoLogBlk(msg, blk)
                                Exit Sub

                            Else
                                'Count the children
                                cs = ThisDrawing.SelectionSets.Add("CHILDREN")
                                For Each cblk In ss
                                    If Trim(GetAttr_Renamed(cblk, "PARENT")) = parent Then 'Find all children with the same PARENT attribute
                                        ssobjs(0) = cblk
                                        cs.AddItems(ssobjs)
                                    End If
                                Next cblk
                                If (CreateSubSwg(blk, pblk, cs.Count, AssetNum)) Then
                                    i = 1
                                    For Each cblk In cs
                                        If SetAttrValue(cblk, rs.Fields("ASSET_NUM").Name, AssetNum(i)) Then
                                            DoLog("Asset Number " & AssetNum(i) & " assigned to " & GetBlockName(cblk))
                                        Else
                                            DoLog("*** Error: Unable to assign asset Number " & AssetNum(i) & " to " & GetBlockName(cblk))
                                        End If
                                        If pb.Value < pb.Maximum Then pb.Value = pb.Value + 1

                                        i = i + 1
                                    Next cblk
                                End If
                                ThisDrawing.SelectionSets.Item("CHILDREN").Delete()
                            End If
                        End If
                    End If
                End If
                rs.Close()
            End If

        Next blk


    End Sub

    Function CreateSubSwg(ByRef blk As AcadBlockReference, ByRef parentblk As AcadBlockReference, ByVal iChildren As Integer, ByRef opid As Object) As Boolean

        Dim xml As String, paramXML As String, retxml As String, f As String, v As String, errXml As String, AssetNum As Object
        Dim acccode As String, Class_1 As String, class18 As String, PlantCode0 As String, address As String, pclass As String
        Dim equipgrpid As String, pAssetNum As String, pbname As String, msg As String, EquipNo As String, plantno As String
        Dim plantcode2 As String, plantcode3 As String

        Dim rs As Recordset
        Dim db As Connection
        db = New Connection
        db.CursorLocation = CursorLocationEnum.adUseClient
        db.Open(m_mdb)

        rs = New Recordset

        CreateSubSwg = False

        ' Now build the XML
        xml = ""
        xml = xml & XMLHeader()
        xml = xml & XMLRequestStart()

        xml = xml & LoginXML(txtUsername.Text, txtPassword.Text, txtPosition.Text, txtDistrict.Text)
        xml = xml & CreateXML()

        rs.Open("SELECT * FROM assets where blockname = '" & GetBlockName(parentblk) & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)
        'rs.Open("SELECT * FROM AssetNumbers where blockname = '" & GetBlockName(parentblk) & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

        If Not rs.EOF Then
            pclass = rs.Fields("CLASS").Value & ""
            equipgrpid = rs.Fields("ChildEGI").Value & ""
            plantcode2 = rs.Fields("ChildPlantCode2").Value & ""
            plantcode3 = rs.Fields("ChildPlantCode3").Value & ""
        End If
        rs.Close()

        rs.Open("SELECT * FROM assets where blockname = '" & GetBlockName(blk) & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)
        'rs.Open("SELECT * FROM AssetNumbers where blockname = '" & GetBlockName(blk) & "'", db, CursorTypeEnum.adOpenStatic, LockTypeEnum.adLockReadOnly)

        If Not rs.EOF Then
            acccode = GetOrgUnit(gsDepotCode) & rs.Fields("AccountCode").Value & ""
            Class_1 = rs.Fields("Class").Value & ""
            class18 = rs.Fields("EquipClassIf18").Value & ""
            PlantCode0 = rs.Fields("Plantcode0").Value & ""
            address = GetAddress(blk)
        End If

        xml = xml & GenericParams(PlantCode0, address, txtUsername.Text, acccode, Class_1, equipgrpid, class18, gsDepotCode, "")

        ' Add in parent detail if child
        pAssetNum = GetAttr_Renamed(parentblk, "ASSET_NUM")
        If Len(pAssetNum) = 0 Then
            pAssetNum = GetAttr_Renamed(parentblk, "ASSET_NUM1")
        End If
        If Len(pAssetNum) > 0 Then
            xml = xml & "<Parameter name=""ParentEquip"">" & DoGetParentEquipNo(GetBlockName(parentblk), pclass, pAssetNum) & "</Parameter>"
            xml = xml & "<Parameter name=""PlantCode1"">" & pAssetNum & "</Parameter>"
            xml = xml & "<Parameter name=""PlantCode2"">" & plantcode2 & "</Parameter>"
            xml = xml & "<Parameter name=""PlantCode3"">" & plantcode3 & "</Parameter>"
            xml = xml & "<Parameter name=""OpIdInstance"">" & iChildren & "</Parameter>" '**** Important ***
        End If

        'now process any other paramaters
        Do While Not rs.EOF
            paramXML = "<Parameter name=""f$"">v$</Parameter>"
            f$ = rs.Fields("FieldName").Value & ""
            If Len(Trim(f$)) > 0 Then
                paramXML = Replace(paramXML, "f$", f$)
                v$ = rs.Fields("FieldValue").Value
                If v$.Substring(0, 4) = "TAG_" Then
                    'If Left(v$, 4) = "TAG_" Then
                    xml = xml & Replace(paramXML, "v$", GetAttr_Renamed(blk, Mid(v$, 5)))
                Else
                    xml = xml & Replace(paramXML, "v$", v$)
                End If
            End If
            rs.MoveNext()
        Loop

        'close of the request
        xml = xml & "</Request></Service>"

        xml = xml & XMLRequestEnd()
        xml = xml & XMLFooter()

        retxml = XMLExecute(xml, gsURL)

        'First check for errors
        If InStr(1, retxml, "Error") > 0 Then
            gsError = retxml
            errXml = GetError()
            msg = "*** Error: unable to generate asset number for b$ at x$/y$ - " & errXml
            DoLogBlk(msg, blk)
            Exit Function

        Else
            'pull out the EquipNo from the returned XML
            EquipNo = Mid(retxml, InStr(1, retxml, "EquipNo") + 9, 12)
            plantno = Mid(retxml, InStr(1, retxml, "PlantNo") + 9)
            plantno.Substring(InStr(plantno, ">") - 1)
            'plantno = Left(plantno, InStr(plantno, "<") - 1)
            opid = GetOpId(EquipNo, gsURL)

            'UpdateNamePlate EquipNo, gsURL, NPs
            UpdateColloquials(EquipNo, gsURL, gbInternal, gsCAPNo, gsDrgNumber)
            If gbInternal Then
                If Len(gsIEProjNo) > 0 Then
                    UpdateAlternateRefs(EquipNo, gsURL, gsIEProjNo)
                End If
            End If

            'lstEquipNo.AddItem bName & "," & assetnum & "," & equipno
            CreateSubSwg = True

        End If

        rs.Close()

    End Function


    Protected Overrides Sub Finalize()
        MyBase.Finalize()
        SaveSetting("IE_TOOLBOX", "LOGIN", "DISTRICT", txtDistrict.Text)
        SaveSetting("IE_TOOLBOX", "LOGIN", "POSITION", txtPosition.Text)
        SaveSetting("IE_TOOLBOX", "LOGIN", "USERNAME", txtUsername.Text)
        SaveSetting("IE_TOOLBOX", "LOGIN", "PROFILE", cmbHost.Text)
        SaveSetting("IE_TOOLBOX", "LOGIN", "DEPOT", cmbDepot.Text)

        Me.Controls.Remove(m_objResizer)
    End Sub
End Class