/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : AttachCableDlg.cpp
// Created          : 7th May 2007
// Description      : Attach cable dialog implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Resource.h"
#include "AttachCableDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic implementation for Pole Information dialog
IMPLEMENT_DYNAMIC(CAttachCableDlg, CDialog)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::CAttachCableDlg
// Description  : Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAttachCableDlg::CAttachCableDlg(CWnd* pParent /*=NULL*/)	: CDialog(CAttachCableDlg::IDD, pParent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::~CAttachCableDlg
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAttachCableDlg::~CAttachCableDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::DoDataExchange
// Description  : Control to variable and vice versa exchange
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAttachCableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONDUCTOR_TYPE, m_ectrlConductor);
	DDX_CBString(pDX, IDC_CONDUCTOR_TYPE, m_csConductor);
	DDX_Control(pDX, IDC_CONDUCTOR_LTYPE, m_cbLineType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : BEGIN_MESSAGE_MAP
// Description  : Windows messages mapped to class methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAttachCableDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_CONDUCTOR_TYPE, OnSelectConductorType)
	ON_MESSAGE(ENAC_UPDATE, OnUpdateConductorType)
	ON_BN_CLICKED(IDHELP, OnHelp)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::OnInitDialog
// Description  : Called by the MFC framework before the dialog is displayed on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAttachCableDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
  // Add all the values to the cable type combo
	//////////////////////////////////////////////////////////////////////////
	m_ectrlConductor.Init();
	m_ectrlConductor.SetMode();
		
	for (int iCtr = 0; iCtr < g_csaConductorNames.GetSize(); iCtr++) 
	{
		m_ectrlConductor.AddString((LPCTSTR)g_csaConductorNames.GetAt(iCtr));
		m_ectrlConductor.AddSearchString((LPCTSTR)g_csaConductorNames.GetAt(iCtr));
	}

  // If we are coming here from the loop
  if (!m_csConductor.IsEmpty()) 
  {
    // Select the previous conductor and disable the "Start" check box
    m_ectrlConductor.SelectString(-1, m_csConductor);
  }
    	
	// Sets the line type based on the  
	OnSelectConductorType();

	//////////////////////////////////////////////////////////////////////////
	// Add all the values to the line type combo
	//////////////////////////////////////////////////////////////////////////
	m_cbLineType.SubclassDlgItem(IDC_CONDUCTOR_LTYPE, this);
	
  // Force line type to "ByLayer" by default (otherwise CRASH!)
	if (m_csLType.IsEmpty() || m_cbLineType.SelectString(-1, m_csLType) == CB_ERR) m_cbLineType.SetCurSel(0);

  // Everything is OK
  return TRUE;
}

LRESULT CAttachCableDlg::OnUpdateConductorType(WPARAM lParam, LPARAM wParam)
{
	UpdateData(true);
	OnSelectConductorType();
	return 0;
}

void CAttachCableDlg::OnSelectConductorType()
{
  // Get the selected conductor
	int iSel;
	CString csConductor; 
	
	iSel = m_ectrlConductor.GetCurSel();
	if (iSel != CB_ERR) 
		m_ectrlConductor.GetLBText(iSel, csConductor);
	else 
		GetDlgItemText(IDC_CONDUCTOR_TYPE, csConductor);
	
  // Get the index of this string in the global array
  int iIndex = 0; 
	if (CheckForDuplication(g_csaConductorNames, csConductor, iIndex))
	{
		// Select the corresponding line type
		m_cbLineType.SelectString(-1, g_csaConductorLTypes.GetAt(iIndex));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::OnHelp()
// Description  : Displays the help topic for this dialog
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAttachCableDlg::OnHelp() 
{
	CTime tmStart(CTime::GetCurrentTime());
	while (T)
	{
		if (DisplayHelp((DWORD) _T("Attaching_cables.htm"))) break;

		CTime tmCur(CTime::GetCurrentTime());
		CTimeSpan tmSpan; tmSpan = tmCur - tmStart;
		if (tmSpan.GetTotalSeconds() > 5) break;
	}
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CAttachCableDlg::OnOK
// Description  : Called when the user clicks on the "Attach" button
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAttachCableDlg::OnOK()
{
	UpdateData();
	
  // Get the selected conductor
  int iSel = m_ectrlConductor.SelectString(-1, m_csConductor);
	// int iSel = m_ectrlConductor.GetCurSel(); For ComboBox
  if (iSel == CB_ERR) { ShowBalloon(_T("Conductor type must be selected."), this, IDC_CONDUCTOR_TYPE); return; }

	// Line type
	int iSelLType = m_cbLineType.GetCurSel();
	m_cbLineType.GetLBText(iSelLType, m_csLType);
		
  // Store the name, layer and layer color
  m_csConductor  = g_csaConductorNames.GetAt(iSel);
  m_csLayer      = g_csaConductorLayers.GetAt(iSel);
  m_csLayerColor = g_csaConductorLayerColors.GetAt(iSel);
  
  // Close the dialog
  CDialog::OnOK();

	// Ensure that the pole layer is created and current
	createLayer(m_csLayer, Adesk::kFalse);
}

//////////////////////////////////////////////////////////////////////////
// Function    : UnifyCablesAndDeleteSegments
// Description : Redraws the polyline cable along the vertices's given. Also
//               deletes the individual segments if the unified cable done.
// Arguments   : 1. [IN] Array of vertex points for the new cable
//               2. [IN] Array of object id's of individual segments. These
//											 will be deleted after the new polyline is drawn.
//               3. [IN] Line type to be set for the new polyline
//               4. [IN] Layer to be set for the new polyline.
//////////////////////////////////////////////////////////////////////////
bool UnifyCablesAndDeleteSegments(AcGePoint3dArray &aryVertices, AcDbObjectIdArray &aryObjIDs, CString csLType, CString csLayer, CString csLColor)
{
	// Check if the there are any objIDs in the array
	if (aryVertices.length() <= 1) return true;

	// Create a polyline entity of cable segments
	AcDbPolyline *pCable = new AcDbPolyline();
	for (int iVrt = 0; iVrt < aryVertices.logicalLength(); iVrt++) 
	{
		AcGePoint2d geVertex(aryVertices[iVrt][X], aryVertices[iVrt][Y]); 
		pCable->addVertexAt(iVrt, geVertex); 
	}
	
	if (setLineType(csLType)) { Acad::ErrorStatus es= pCable->setLinetype(csLType); if (es != Acad::eOk) { delete pCable; acutPrintf(_T("\nError: %s"), acadErrorStatusText(es)); return false; }}

	Acad::ErrorStatus es= pCable->setColorIndex(256);
	if (es != Acad::eOk) { delete pCable; acutPrintf(_T("\nError: %s"), acadErrorStatusText(es)); return false; }

	// Set layer color 
	setLayerColor(csLayer, _tstoi(csLColor));
	
	// Set the layer
	pCable->setLayer(csLayer);
	
	AcDbObjectId objId = appendEntityToDatabase(pCable, TRUE);
	pCable->close();
	if (!objId.isValid()) return false;
	
	// Delete all the segments if unification is done
	AcDbBlockTable *pBlkTbl = NULL;
	es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);
	if (es != Acad::eOk) { acutPrintf(_T("\nError: %s"), acadErrorStatusText(es)); return false; }

	AcDbObject *pObj = NULL;
	for (int iCtr = 0; iCtr < aryObjIDs.length(); iCtr++)
	{
		es = acdbOpenObject(pObj, aryObjIDs[iCtr],	AcDb::kForWrite);
		if (es != Acad::eOk) { pBlkTbl->close(); (_T("\nError: %s"), acadErrorStatusText(es)); return false; }

		pObj->erase();
		pObj->close();
	}

	pBlkTbl->close();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : Command_AttachCable
// Description  : Called when the user enters the "AEC_AttachCable" command in AutoCAD
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Command_AttachCable()
{
	// Get the current layer name. This is required to set the layer back after the command exits.
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	// Load the line types into this drawing
	if (checkFileAccess(g_csBaseFolder + _T("\\EE_ToolBox.lin"), 00) == FALSE) return;
	for (int iCtr = 0; iCtr < g_csaConductorLTypes.GetSize(); iCtr++) setLineType(g_csaConductorLTypes.GetAt(iCtr));

	// Display the "Attach cable" dialog
	CAttachCableDlg dlgAC;
	CAcModuleResourceOverride useMe;
	if (dlgAC.DoModal() == IDCANCEL) { acedSetVar(_T("CLAYER"), &rbCLayer); return; }

	int iRet = 0, iCable = 0;
	ads_name enPole;
	ads_point ptPrev = { -9999.0, -9999.0, -9999.0} , ptNext;
	AcDbObjectId objPrevPole;
	
	AcDbObjectIdArray aryObjIDs;
	AcGePoint3dArray aryVertices;

	// Loop until user cancels
	bool bSelectAsset = true;
	while (TRUE)
	{
		if (bSelectAsset)
		{
			iRet = acedEntSel(_T("\rSelect asset or ENTER to pick a point: "), enPole, ptNext);
		}
		else
		{
			if ((ptPrev[X] == -9999.0) && (ptPrev[Y] == -9999.0) && (ptPrev[Z] == -9999.0))
				iRet = acedGetPoint(NULL, _T("\rPick a point or ENTER to select asset: "), ptNext);
			else
				iRet = acedGetPoint(ptPrev, _T("\rPick a point or ENTER to select asset: "), ptNext);
		}

		if ((iRet == -5001) || (iRet == 5000))
		{
			bSelectAsset = !bSelectAsset;
			continue;
		}
		else if (iRet == RTCAN) 
		{
			// Draw the polyline
			if (!UnifyCablesAndDeleteSegments(aryVertices, aryObjIDs, dlgAC.m_csLType, dlgAC.m_csLayer, dlgAC.m_csLayerColor)) acutPrintf(_T("\nUnable to join the cable segments into one."));
			
			// If the user cancels, return now
			acedSetVar(_T("CLAYER"), &rbCLayer); 
			return; 
		}
		// Otherwise, if user has selected an entity
		else if (iRet == RTNORM)
		{
			if (bSelectAsset)
			{
				//////////////////////////////////////////////////////////////////////////
				// User selects an asset to specify the point to draw cable. So get the 
				// insertion point of the asset to draw the cable.
				//////////////////////////////////////////////////////////////////////////
				// Get the insertion point and scale factor of the pole
				AcDbObjectId objPole; acdbGetObjectId(objPole, enPole);
				AcDbBlockReference *pPole; if (acdbOpenObject(pPole, objPole, AcDb::kForRead) != Acad::eOk) continue;
				ptNext[X] = pPole->position()[X]; ptNext[Y] = pPole->position()[Y]; ptNext[Z] = 0.0;
				AcGeScale3d geScale = pPole->scaleFactors();
				pPole->close();
			}
			else
			{
				//////////////////////////////////////////////////////////////////////////
				// User selects a point to draw the cable.
				//////////////////////////////////////////////////////////////////////////
				// Nothing to do.
			}

			// If this is not the first pole
			if (iCable > 0) 
			{
				// Create a line from the previous point to this one
				AcDbLine *pCable = new AcDbLine;
				pCable->setStartPoint(AcGePoint3d(ptPrev[X], ptPrev[Y], ptPrev[Z]));
				pCable->setEndPoint(AcGePoint3d(ptNext[X], ptNext[Y], 0.0));

				if (setLineType(dlgAC.m_csLType))
				{
					Acad::ErrorStatus es= pCable->setLinetype(dlgAC.m_csLType);
					if (es != Acad::eOk) { acutPrintf(_T("\nError: %s"), acadErrorStatusText(es)); }
				}

				// Add this line to the drawing
				AcDbObjectId objId = appendEntityToDatabase(pCable); 
				if (objId.isValid()) aryObjIDs.append(objId);

				// Store the end point to redraw the polyline
				if (aryVertices.isEmpty()) aryVertices.append(AcGePoint3d(ptPrev[X], ptPrev[Y], 0.0));
				aryVertices.append(AcGePoint3d(ptNext[X], ptNext[Y], 0.0));
        
				// Close the cable
				pCable->close();

				// Store point this as previous point
				ptPrev[X] = ptNext[X]; ptPrev[Y] = ptNext[Y]; ptPrev[Z] = 0.0;

				// Increment the cable counter
				iCable++;
		  }
			else 
			{
				//////////////////////////////////////////////////////////////////////////
				// Otherwise, the start point is specified or the first pole is selected.
				//////////////////////////////////////////////////////////////////////////
				// Store the insertion point in the previous point
				ptPrev[X] = ptNext[X]; ptPrev[Y] = ptNext[Y]; ptPrev[Z] = 0.0;
				
				// Set the cable counter
				iCable = 2;
			}
		}
	}

	// Reset the layer that prevailed at the beginning of the command.
	acedSetVar(_T("CLAYER"), &rbCLayer);
}


