Option Strict Off
Option Explicit On
Friend Class CNamePlates
	Implements System.Collections.IEnumerable
	'local variable to hold collection
	Private mCol As Collection
	
	Public Function Add(ByRef FieldName As String, ByRef FieldValue As String, Optional ByRef sKey As String = "") As CNamePlate
		'create a new object
		Dim objNewMember As CNamePlate
        objNewMember = New CNamePlate
		
		'set the properties passed into the method
		objNewMember.FieldName = FieldName
		objNewMember.FieldValue = FieldValue
		If Len(sKey) = 0 Then
			mCol.Add(objNewMember)
		Else
			mCol.Add(objNewMember, sKey)
        End If
		
		'return the object created
		Add = objNewMember
		'UPGRADE_NOTE: Object objNewMember may not be destroyed until it is garbage collected. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="6E35BFF6-CD74-4B09-9689-3E1A43DF8969"'
        objNewMember = Nothing
	End Function
	
	Default Public ReadOnly Property Item(ByVal vntIndexKey As Object) As CNamePlate
		Get
			'used when referencing an element in the collection
			'vntIndexKey contains either the Index or Key to the collection,
			'this is why it is declared as a Variant
			'Syntax: Set foo = x.Item(xyz) or Set foo = x.Item(5)
			Item = mCol.Item(vntIndexKey)
		End Get
	End Property

	Public ReadOnly Property Count() As Integer
		Get
			'used when retrieving the number of elements in the
			'collection. Syntax: Debug.Print x.Count
			Count = mCol.Count()
		End Get
	End Property
	
	
	'UPGRADE_NOTE: NewEnum property was commented out. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="B3FC1610-34F3-43F5-86B7-16C984F0E88E"'
	'Public ReadOnly Property NewEnum() As stdole.IUnknown
		'Get
			'this property allows you to enumerate
			'this collection with the For...Each syntax
			'NewEnum = mCol._NewEnum
		'End Get
	'End Property
	
	Public Function GetEnumerator() As System.Collections.IEnumerator Implements System.Collections.IEnumerable.GetEnumerator
		'UPGRADE_TODO: Uncomment and change the following line to return the collection enumerator. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="95F9AAD0-1319-4921-95F0-B9D3C4FF7F1C"'
        GetEnumerator = mCol.GetEnumerator
	End Function
	
	
	Public Sub Remove(ByRef vntIndexKey As Object)
		'used when removing an element from the collection
		'vntIndexKey contains either the Index or Key, which is why
		'it is declared as a Variant
		'Syntax: x.Remove(xyz)
		
		
		mCol.Remove(vntIndexKey)
    End Sub
	
	Public Sub New()
		MyBase.New()
        mCol = New Collection
    End Sub
	
	Protected Overrides Sub Finalize()
		mCol = Nothing
		MyBase.Finalize()
	End Sub
End Class