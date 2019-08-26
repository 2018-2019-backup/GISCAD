Option Strict Off
Option Explicit On
Friend Class CEquip
	'local variable(s) to hold property value(s)
	Private mvarAccountCode As String 'local copy
	Private mvarClass As String 'local copy
	Private mvarGroupID As String 'local copy
	Private mvarDepot As String 'local copy
	Private mvarPlantCode0 As String 'local copy
	Private mvarEquipNoD1 As String 'local copy
	Private mvarEquipClassif18 As String 'local copy
	Private mvarStreetCode As String 'local copy
	Private mvarInputBy As String 'local copy
	Private mvarStatus As String 'local copy
	Private mvarActiveFlag As String 'local copy
	Private mvarPlantCode1 As String 'local copy
	Private mvarParentEquipNo As String 'local copy
	Private mvarEquipNoD2 As String 'local copy
	Private mvarAssetNum As String 'local copy
	Private mvarEquipNo As String 'local copy
	Private mvarNamePlates As CNamePlates 'local copy
	Private mvarAttributes As CNamePlates 'local copy

	Public Property Attributes() As CNamePlates
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.NamePlates
			Attributes = mvarAttributes
		End Get
		Set(ByVal Value As CNamePlates)
			'used when assigning an Object to the property, on the left side of a Set statement.
			'Syntax: Set x.NamePlates = Form1
			mvarAttributes = Value
		End Set
	End Property

	Public Property nameplates() As CNamePlates
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.NamePlates
			nameplates = mvarNamePlates
		End Get
		Set(ByVal Value As CNamePlates)
			'used when assigning an Object to the property, on the left side of a Set statement.
			'Syntax: Set x.NamePlates = Form1
			mvarNamePlates = Value
		End Set
	End Property

	Public Property EquipNo() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.EquipNo
			EquipNo = mvarEquipNo
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.EquipNo = 5
			mvarEquipNo = Value
		End Set
	End Property

	Public Property AssetNum() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.AssetNum
			AssetNum = mvarAssetNum
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.AssetNum = 5
			mvarAssetNum = Value
		End Set
	End Property

	Public Property EquipNoD2() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.EquipNoD2
			EquipNoD2 = mvarEquipNoD2
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.EquipNoD2 = 5
			mvarEquipNoD2 = Value
		End Set
	End Property

	Public Property ParentEquipNo() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.ParentEquipNo
			ParentEquipNo = mvarParentEquipNo
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.ParentEquipNo = 5
			mvarParentEquipNo = Value
		End Set
	End Property

	Public Property PlantCode1() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.PlantCode1
			PlantCode1 = mvarPlantCode1
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.PlantCode1 = 5
			mvarPlantCode1 = Value
		End Set
	End Property

	Public Property ActiveFlag() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.ActiveFlag
			ActiveFlag = mvarActiveFlag
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.ActiveFlag = 5
			mvarActiveFlag = Value
		End Set
	End Property

	Public Property Status() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.Status
			Status = mvarStatus
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.Status = 5
			mvarStatus = Value
		End Set
	End Property

	Public Property InputBy() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.InputBy
			InputBy = mvarInputBy
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.InputBy = 5
			mvarInputBy = Value
		End Set
	End Property

	Public Property StreetCode() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.StreetCode
			StreetCode = mvarStreetCode
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.StreetCode = 5
			mvarStreetCode = Value
		End Set
	End Property

	Public Property EquipClassif18() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.EquipClassif18
			EquipClassif18 = mvarEquipClassif18
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.EquipClassif18 = 5
			mvarEquipClassif18 = Value
		End Set
	End Property

	Public Property EquipNoD1() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.EquipNoD1
			EquipNoD1 = mvarEquipNoD1
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.EquipNoD1 = 5
			mvarEquipNoD1 = Value
		End Set
	End Property

	Public Property PlantCode0() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.PlantCode0
			PlantCode0 = mvarPlantCode0
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.PlantCode0 = 5
			mvarPlantCode0 = Value
		End Set
	End Property

	Public Property Depot() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.Depot
			Depot = mvarDepot
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.Depot = 5
			mvarDepot = Value
		End Set
	End Property

	Public Property GroupID() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.GroupID
			GroupID = mvarGroupID
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.GroupID = 5
			mvarGroupID = Value
		End Set
	End Property

	'UPGRADE_NOTE: Class was upgraded to Class_Renamed. Click for more: 'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="A9E4979A-37FA-4718-9994-97DD76ED70A7"'
	Public Property Class_Renamed() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.Class
			Class_Renamed = mvarClass
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.Class = 5
			mvarClass = Value
		End Set
	End Property

	Public Property AccountCode() As String
		Get
			'used when retrieving value of a property, on the right side of an assignment.
			'Syntax: Debug.Print X.AccountCode
			AccountCode = mvarAccountCode
		End Get
		Set(ByVal Value As String)
			'used when assigning a value to the property, on the left side of an assignment.
			'Syntax: X.AccountCode = 5
			mvarAccountCode = Value
		End Set
    End Property

    Public Sub New()
        MyBase.New()
        ActiveFlag = "Y"
        Status = "PR"
        mvarAttributes = New CNamePlates
        mvarNamePlates = New CNamePlates
    End Sub
End Class