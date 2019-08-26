Imports Autodesk.AutoCAD.ApplicationServices
Imports Autodesk.AutoCAD.DatabaseServices
Public Class EEver

    Dim doc As Document = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument
    Private Sub EEver_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        txtVersion.Text = getVersionInfo(doc.Database)
    End Sub

    Private Function getVersionInfo(ByVal srcDb As Database) As String

        Dim strVersion As String = ""
        Try
            Using tr As Transaction = srcDb.TransactionManager.StartTransaction()
                Dim pNamedobj As DBDictionary
                pNamedobj = TryCast((tr.GetObject(srcDb.NamedObjectsDictionaryId, OpenMode.ForRead)), DBDictionary)
                If Not IsDBNull(pNamedobj) Then
                    Dim pDict As DBDictionary = TryCast(tr.GetObject(pNamedobj.GetAt("EEVER_DICT"), OpenMode.ForRead), DBDictionary)
                    If Not IsDBNull(pDict) Then
                        Dim pXrec As Xrecord = TryCast(tr.GetObject(pDict.GetAt("Version_No"), OpenMode.ForRead), Xrecord)
                        If Not IsDBNull(pXrec) Then
                            Dim pRbList As TypedValue() = pXrec.Data.AsArray()
                            If TypeOf pRbList.GetValue(0) Is TypedValue Then
                                strVersion = DirectCast(pRbList.GetValue(0), TypedValue).Value.ToString()
                            Else
                                strVersion = pRbList.GetValue(0).ToString()
                            End If

                        End If
                    End If

                End If

            End Using
        Catch ex As Exception

        End Try


        Return strVersion

    End Function

    Private Function UpdateVersionInfo(ByVal srcDb As Database, ByVal strVersion As String) As Boolean
        Dim Result As Boolean = False
        Try
            Using tr As Transaction = srcDb.TransactionManager.StartTransaction()
                Dim pNamedobj As DBDictionary
                pNamedobj = TryCast((tr.GetObject(srcDb.NamedObjectsDictionaryId, OpenMode.ForWrite)), DBDictionary)
                If Not IsDBNull(pNamedobj) Then
                    Dim pDict As DBDictionary = TryCast(tr.GetObject(pNamedobj.GetAt("EEVER_DICT"), OpenMode.ForWrite), DBDictionary)
                    If Not IsDBNull(pDict) Then
                        Dim pXrec As Xrecord = TryCast(tr.GetObject(pDict.GetAt("Version_No"), OpenMode.ForWrite), Xrecord)
                        If Not IsDBNull(pXrec) Then
                            Dim pRbList As TypedValue() = pXrec.Data.AsArray()
                            If TypeOf pRbList.GetValue(0) Is TypedValue Then
                                Dim typValue As New TypedValue(DirectCast(pRbList.GetValue(0), TypedValue).TypeCode, strVersion)
                                pXrec.Data = New ResultBuffer(typValue)
                                Result = True
                            Else
                                Dim typValue As New TypedValue(DirectCast(pRbList.GetValue(0), TypedValue).TypeCode, strVersion)
                                pXrec.Data = New ResultBuffer(typValue)
                                Result = True
                            End If

                        End If
                    End If

                End If

            End Using
        Catch ex As Exception

        End Try
        Return Result
    End Function


    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click
        If txtVersion.Text <> String.Empty Then
            If UpdateVersionInfo(doc.Database, txtVersion.Text) Then
                Me.Close()
            Else
                MessageBox.Show("Version Number not Updated")
            End If
        Else
            MessageBox.Show("Version Number is Mandatory")
        End If

    End Sub

    Private Sub Button2_Click(sender As System.Object, e As System.EventArgs) Handles Button2.Click
        Me.Close()
    End Sub


End Class