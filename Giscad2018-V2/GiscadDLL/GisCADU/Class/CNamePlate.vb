Option Strict Off
Option Explicit On
Friend Class CNamePlate
	Private mvarFieldName As String 'local copy
	Private mvarFieldValue As String 'local copy

	Public Property FieldValue() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.FieldValue
			FieldValue = mvarFieldValue
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.FieldValue = 5
			mvarFieldValue = Value
		End Set
	End Property

	Public Property FieldName() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.FieldName
			FieldName = mvarFieldName
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.FieldName = 5
			mvarFieldName = Value
		End Set
	End Property
End Class