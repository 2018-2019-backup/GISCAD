////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project name     : IE Toolbox
// Client name      : AEC Systems
// File name        : SPUtils.cpp
// Created          : 25th April 2007
// Description      : Project specific utilities.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Hherror.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name : appMessage
// Arguments     : Message, as string
// Returns       : Nothing
// Called from   : Anywhere
// Description   : Displays the message by calling the MessageBox function with the information icon.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void appMessage(CString csMessage, int iIcon)
{
  if (!iIcon) iIcon = MB_ICONINFORMATION;
  ::MessageBox(::GetActiveWindow(), csMessage, _T("EE ToolBox message"), MB_OK | iIcon);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name : appError
// Arguments     : 1. File name, as string
//                 2. Function name, as string
//                 3. Line number, as int
//                 4. Error message, as string
//
// Returns       : Nothing
// Called from   : Anywhere
// Description   : Displays an error message by calling the MessageBox function with the appropriate icon.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void appError(CString csFileName, CString csFuncName, int iLine, CString csErrorMsg)
{
  // Format the message to be displayed
  CString csMessage;
  csMessage.Format(_T("%s (Line %d)\n\n%s\n\n%s"), csFileName.Mid(csFileName.ReverseFind(_T('\\')) + 1), iLine, csFuncName, csErrorMsg);

  // Display it
  ::MessageBox(::GetActiveWindow(), csMessage, _T("EE ToolBox Error"), MB_OK | MB_ICONSTOP);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name : displayMessage
// Arguments     : Message, as string
// Returns       : Nothing
// Called from   : Anywhere
// Description   : Displays the message by calling the MessageBox function with the appropriate icon. This 
//                 new version accepts variable arguments, exactly like a normal printf function
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayMessage(LPTSTR fmt, ...)
{
  CString csDispl;

  // format and write the data we were given
  va_list args;
  va_start(args, fmt);
  csDispl.FormatV(fmt, args);
  appMessage(csDispl, MB_ICONINFORMATION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name : getConfirmation
// Arguments     : Question to display, as string
// Returns       : IDYES if user answered yes, IDNO if not
// Called from   : Anywhere
// Description   : Displays a message box with the given question and two buttons, YES and NO. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getConfirmation(CString csQuestion)
{
  return (::MessageBox(::GetActiveWindow(), csQuestion, _T("Please confirm!"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : checkFileAccess
// Arguments        : 1. File name to check for, as const char *
//                    2. Access mode, as integer
// Called from      : Anywhere
// Returns          : TRUE if file exists, FALSE if not
// Description      : Checks if the given file exists, displays an error message if not.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL checkFileAccess(const TCHAR *szFilePath, int iMode)
{
  if (_taccess(szFilePath, iMode))
  {
    CString csError;
    switch (iMode)
    {
      case 00   : csError.Format(_T("File does not exist\n%s"),               szFilePath); break;
      case 02   : csError.Format(_T("No write permission for file\n%s"),      szFilePath); break;
      case 04   : csError.Format(_T("No read permission for file\n%s"),       szFilePath); break;
      case 06   : csError.Format(_T("No read/write permission for file\n%s"), szFilePath); break;
      default   : csError.Format(_T("Unknown mode %d for file\n%s"),   iMode, szFilePath); break;
    }
    appMessage(csError, MB_ICONSTOP);
    return FALSE;
  }
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : setupDataSource
// Arguments        : 1. DSN, as CString
//                    2. MS-Access database name, as CString (assumed to be in "Database" folder)
//
// Called from      : DefineDSNs
// Returns          : TRUE if successful, FALSE if not
// Description      : Sets up a data source name.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL setupDataSource(CString csDSN, CString csMDB)
{
  TCHAR sDSNStr[512];
  TCHAR sError[_MAX_PATH]; 

  _stprintf(sDSNStr, _T("DSN=%s;UID=;PWD=;DBQ=%s;"), csDSN, csMDB);
	#ifdef _WIN64
  if (SQLConfigDataSource(NULL, ODBC_ADD_DSN, _T("Microsoft Access Driver (*.mdb, *.accdb)"), sDSNStr) == FALSE)
	#else
	if (SQLConfigDataSource(NULL, ODBC_ADD_DSN, _T("Microsoft Access Driver (*.mdb)"), sDSNStr) == FALSE)
	#endif
  { 
    CString csError;

    csError.Format(_T("Unable to setup DSN \"%s\"\nto %s"), csDSN, csMDB);
    _tcscpy(sError, csError);
    appError(_T(__FILE__), _T("SetupDataSource"), __LINE__, sError); 
    return FALSE; 
  }
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : suppressZeroes
// Arguments        : Decimal value, as double
// Returns          : String equivalent of decimal, as string
// Called from      : Anywhere
// Description      : Converts the decimal value to string and removes all trailing zeros.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString suppressZero(double dValue)
{
  CString csReturn;
  csReturn.Format(_T("%.2f"), dValue);

  if (dValue == 0.0) csReturn = _T("0");
  else
  {
    int iCount = (csReturn.GetLength() - 1);

    if (csReturn.Find(_T('.')) > 0)
    {
      //.. First remove the zeros at last
      while (true)
      {
        if (csReturn.GetAt(iCount) != _T('0')) break;
        if (iCount <= 0 ) break;
        csReturn.SetAt(iCount, _T(' '));
        iCount--;
      }

      //.. If the last one is . then also remove it
      if (iCount >= 0) 
      {
        if (csReturn.GetAt(iCount) == _T('.')) csReturn.SetAt(iCount, _T(' '));
      }
    }
  }

  csReturn.TrimLeft();
  csReturn.TrimRight();

  return csReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : CheckForDuplication
// Arguments        : 1. Array to check values in, as reference to CStringArray
//                    2. Value to check for in array, as CString
//
// Returns          : TRUE if found, FALSE if not
// Called from      : Anywhere
// Description      : Checks for presence of string in the array
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckForDuplication(CStringArray& csaArray, CString csCheck, int &iIndex)
{
  for (int iCtr = 0; iCtr < csaArray.GetSize(); iCtr++)
  {
    if (!csaArray.GetAt(iCtr).CompareNoCase(csCheck)) { iIndex = iCtr; return TRUE; }
  }

  return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : CheckForDuplication
// Arguments        : 1. Array to check values in, as reference to CStringArray
//                    2. Value to check for in array, as CString
//
// Returns          : TRUE if found, FALSE if not
// Called from      : Anywhere
// Description      : Checks for presence of string in the array
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckForDuplication(CUIntArray& uiaArray, int iCheck)
{
	for (int iCtr = 0; iCtr < uiaArray.GetSize(); iCtr++)
	{
		if (uiaArray.GetAt(iCtr) == iCheck) return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : CheckForDuplication
// Arguments        : 1. Array to check values in, as reference to CStringArray
//                    2. Value to check for in array, as CString
//
// Returns          : TRUE if found, FALSE if not
// Called from      : Anywhere
// Description      : Checks for presence of string in the array
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckForDuplication(CStringArray& csaArray, CString csCheck)
{
  int iUnused = 0;
  return CheckForDuplication(csaArray, csCheck, iUnused);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : switchOff
// Arguments        : None
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Switches off a few AutoCAD variables
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void switchOff()
{
  struct resbuf rbSetvar;

  rbSetvar.restype = RTSHORT;
  rbSetvar.resval.rint = 0;

  acedSetVar(_T("CMDECHO"),   &rbSetvar);
  acedSetVar(_T("BLIPMODE"),  &rbSetvar);
  // acedSetVar(_T("UCSICON"),   &rbSetvar);
  // acedSetVar(_T("OSMODE"),    &rbSetvar);

  rbSetvar.resval.rint = 5;
  acedSetVar(_T("EXPERT"),    &rbSetvar);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : switchOn
// Arguments        : None
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Switches on a few AutoCAD variables
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void switchOn()
{
  struct resbuf rbSetvar;

  rbSetvar.restype = RTSHORT;
  rbSetvar.resval.rint = 1;

  acedSetVar(_T("CMDECHO"),   &rbSetvar);
  acedSetVar(_T("HIGHLIGHT"), &rbSetvar);
  acedSetVar(_T("BLIPMODE"),  &rbSetvar);
  acedSetVar(_T("UCSICON"),   &rbSetvar);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : Assoc
// Arguments        : 1. The ENTGET list to be parsed
//                    2. The association number
// Returns          : The struct resbuf of the association no. asked for.
// Called from      : Anywhere
// Description      : This is an equivalent of the ASSOC function of AutoLISP.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct resbuf *Assoc(struct resbuf *rbpList, int iAssocNum)
{
  struct resbuf *rbpTemp;

  for (rbpTemp = rbpList; rbpTemp; rbpTemp = rbpTemp->rbnext)
    if (rbpTemp->restype == iAssocNum) return rbpTemp;
  return (struct resbuf *)NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : addXDataToEntity
// Arguments        : 1. Entity name to which xdata has to be attached, as ads_name
//                    2. XData, as struct resbuf
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Adds the given xdata to the entity.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void addXDataToEntity(ads_name enEnt, struct resbuf *rbpXData)
{
  // Register the application
  acdbRegApp(rbpXData->resval.rstring);

  // Open the object and set the xdata
  AcDbObjectId objId;
  AcDbObject *pObject;
  acdbGetObjectId(objId, enEnt);
  if (acdbOpenObject(pObject, objId, AcDb::kForWrite) != Acad::eOk) return;
  pObject->setXData(rbpXData);
  pObject->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : getXDataFromEntity
// Arguments        : 1. Entity name from which xdata has to be extracted, as ads_name
//                    2. Application name as string 
// Returns          : Pointer to XData as struct resbuf *
// Called from      : Anywhere
// Description      : Retrieves the xdata from an entity
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct resbuf *getXDataFromEntity(ads_name enEnt, CString csAppName)
{
  // Open the object and set the xdata
  AcDbObjectId objId;
  AcDbObject *pObject;
  acdbGetObjectId(objId, enEnt);
  if (acdbOpenObject(pObject, objId, AcDb::kForRead) != Acad::eOk) return (struct resbuf *)NULL;
  struct resbuf *rbpXData = pObject->xData(csAppName);
  pObject->close();

  return rbpXData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : addXRecordToDictionary
// Arguments        : 1. Dictionary name, as string
//                    2. Xrecord name, as string
//                    3. XED to be attached, as struct resbuf *
// Returns          : None
// Called from      : Anywhere
// Description      : Creates a dictionary not attached to any entity.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void addXRecordToDictionary(CString csDictName, CString csXrecName, struct resbuf *rbpXED)
{
  if (!rbpXED) return;

  // Get the named objects dictionary for the current drawing (no error message if it fails, since this is just a safety check)
  AcDbDictionary *pNamedObj, *pDict;
  if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNamedObj, AcDb::kForWrite) != Acad::eOk) return;

  // Check if the specified dictionary name is already present in this named object
  BOOL bNewDict = FALSE;
  if (pNamedObj->getAt(csDictName, (AcDbObject *&) pDict, AcDb::kForWrite) == Acad::eKeyNotFound)
  {
    // Create a new dictionary in the named objects
    pDict = ::new AcDbDictionary;
    AcDbObjectId DictId;
    pNamedObj->setAt(csDictName, pDict, DictId);

    // Set the flag
    bNewDict = TRUE;
  }

  AcDbXrecord *pXrec;
  AcDbObjectId objXRec, xrecObjId;

  // If this is a new dictionary OR the given xrecord is not present in the dictionary
  if ((bNewDict == TRUE) || (pDict->getAt(csXrecName, objXRec) == Acad::eKeyNotFound))
  {
    // Add a new XRecord to the dictionary
    pXrec = ::new AcDbXrecord;
    pDict->setAt(csXrecName, pXrec, xrecObjId);

    // Set the data it should contain
    pXrec->setFromRbChain(*rbpXED);

    // Close the xrecord
    pXrec->close();
  }
  // Otherwise, change the xrecord
  else
  {
    // Open the xrecord for writing
    Acad::ErrorStatus es = acdbOpenObject(pXrec, objXRec, AcDb::kForWrite);
    if (es != Acad::eOk)
    {
      pDict->close();
      pNamedObj->close();
      acutPrintf(_T("\nError %d opening XRecord '%s' in dictionary '%s' for writing.\n"), es, csXrecName, csDictName);
      return;
    }

    // Reset the data it contains
    pXrec->setFromRbChain(*rbpXED);

    // Close the xrecord
    pXrec->close();
  }

  // Close the xrecord dictionary
  pDict->close();

  // Close the named objects dictionary
  pNamedObj->close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : getXRecordFromDictionary
// Arguments        : 1. Dictionary name, as string
//                    2. Xrecord name, as string
// Returns          : Xrecord attached to the dictionary, as struct resbuf *
// Called from      : Anywhere
// Description      : Retrieves the Xrecord of given name in the dictionary of the specified name.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct resbuf *getXRecordFromDictionary(CString csDictName, CString csXrecName)
{
  Acad::ErrorStatus eStatus;

  // Get the named objects dictionary
  AcDbDictionary *pNamedObj;
  acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNamedObj, AcDb::kForRead);

  // see if the dictionary name specified is present
  AcDbDictionary *pDict;
  eStatus = pNamedObj->getAt(csDictName, (AcDbObject *&)pDict, AcDb::kForRead);
  pNamedObj->close();
  if (eStatus == Acad::eKeyNotFound) return (struct resbuf *) NULL;

  // See if the xrecord name specified is present
  AcDbXrecord *pXrec;
  eStatus = pDict->getAt(csXrecName, (AcDbObject *&)pXrec, AcDb::kForRead);
  pDict->close();
  if (eStatus == Acad::eKeyNotFound) return (struct resbuf *) NULL;

  // Get the xrecord value and return it
  struct resbuf *rbpXED;
  pXrec->rbChain(&rbpXED);
  pXrec->close();

  return rbpXED;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : deleteXRecord
// Arguments        : Dictionary name and XRecord as char
// Called from      : DeleteXRecord()
// Returns          : Nothing
// Description      : Deletes the XRecord passed as argument in the dictionary name passed on
// Created on       : 5/12/99
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void deleteXRecord(CString csDictName, CString csXrecName)
{
  Acad::ErrorStatus eStatus;

  // Get the named objects dictionary
  AcDbDictionary *pNamedObj;
  acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNamedObj, AcDb::kForRead);

  // see if the dictionary name specified is present
  AcDbDictionary *pDict;
  eStatus = pNamedObj->getAt(csDictName, (AcDbObject *&)pDict, AcDb::kForRead);
  pNamedObj->close();
  if (eStatus == Acad::eKeyNotFound)
  {
    //ads_printf("\nCould not delete the XRecord!");
    return;
  }

  // See if the xrecord name specified is present
  AcDbXrecord *pXrec;
  eStatus = pDict->getAt(csXrecName, (AcDbObject *&)pXrec, AcDb::kForWrite);
  pDict->close();
  if (eStatus == Acad::eKeyNotFound) 
  {
    //ads_printf("\nCould not delete the XRecord!");
    return;
  }

  // Delete the XRecord
  pXrec->erase();
  pXrec->close();
}

void setLayerColor(CString csLayerName, int iColorIndex)
{
	AcDbLayerTable *pLayerTbl;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);
	if (es != Acad::eOk) return;

	// If the layer doesn't exist return quietly
	if (!(pLayerTbl->has(csLayerName))) { pLayerTbl->close(); return; }

	// Get the object id and the layer table record
	AcDbLayerTableRecord *pLayerTblRecord = NULL;
	es = pLayerTbl->getAt(csLayerName, pLayerTblRecord, AcDb::kForWrite);
	if (es != Acad::eOk) return;
	pLayerTbl->close();

	AcCmColor color; color.setColorIndex(iColorIndex); 
	pLayerTblRecord->setColor(color);
	pLayerTblRecord->close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : setLayer
// Arguments   : const char * for layer name
// Return      : None
// Called from : Anywhere
// Comments    : Sets the layer name specified as current and freezes the rest
/////////////////////////////////////////////////////////////////////////////////////////////////////
void setLayer(const TCHAR *sLayerName, Adesk::Boolean iFreezeRest)
{
  AcDbObjectId objLayer;
  AcDbLayerTable *pLayerTbl;
  AcDbLayerTableIterator *pLayerIterator;
  AcDbLayerTableRecord *pLayerTblRecord = ::new AcDbLayerTableRecord;

  // Set the current space to MODEL
  // acdbHostApplicationServices()->workingDatabase()->setTilemode(1);

  // Get the layer table pointer
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);

  // If the layer doesn't exist return quitely
  if (!(pLayerTbl->has(sLayerName))) { pLayerTbl->close(); return; }

  // Get the object id and the layer table record
  pLayerTbl->getAt(sLayerName, objLayer);
  pLayerTbl->getAt(sLayerName, pLayerTblRecord, AcDb::kForWrite);
  pLayerTblRecord->setIsFrozen(0);  // Thaw the layer specified
  pLayerTblRecord->close();

  // A close to the pointer to the layer table is forced to successfully execute the statement following it
  pLayerTbl->close();

  // Set the layer specified as current
  acdbHostApplicationServices()->workingDatabase()->setClayer(objLayer);

  // If the programmer has opted not to freeze the other layers, return
  if (iFreezeRest == Adesk::kFalse) return;

  // Freeze the rest of the layers
  const TCHAR *pLayerName;
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);
  pLayerTbl->newIterator(pLayerIterator);
  for (; !pLayerIterator->done(); pLayerIterator->step())
  {  
    pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForWrite);
    pLayerTblRecord->getName(pLayerName);
    if (_tcsicmp(pLayerName, sLayerName)) pLayerTblRecord->setIsFrozen(1);
    pLayerTblRecord->close();
    //free(pLayerName);
  }

  // Delete the layer iterator and close the Layer Table pointer
  delete pLayerIterator;
  pLayerTbl->close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : createLayer
// Arguments   : char * giving the name of the new layer
//               Freeze other layers flag (defaults to true)
//
// Return      : None
// Called from : Anywhere
// Comments    : Creates a new layer given its name. Sets this layer as current and freezes the rest
/////////////////////////////////////////////////////////////////////////////////////////////////////
void createLayer(const TCHAR* sLayerName, Adesk::Boolean iFreezeRest, Adesk::Boolean doSetLayer)
{
  AcDbLayerTable *pLayerTbl;
  AcDbObjectId objLayer;

  // Get this drawing's layer table pointer
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);

  // Create a new layer table record
  AcDbLayerTableRecord *pLayerTblRecord = ::new AcDbLayerTableRecord;
  if (!(pLayerTbl->has(sLayerName)))
  {
    // Fill in the appropriate information. We also have to specify the color 
    // and linetype for this layer. AutoCAD assumes the rest.
    pLayerTblRecord->setName(sLayerName);
    pLayerTblRecord->setIsFrozen(0);		

    AcCmColor color; color.setColorIndex(7); pLayerTblRecord->setColor(color);

    AcDbLinetypeTable *pLinetypeTbl;
    AcDbObjectId ltId;
    acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
    pLinetypeTbl->getAt(_T("CONTINUOUS"), ltId);
    pLinetypeTbl->close();
    pLayerTblRecord->setLinetypeObjectId(ltId);

    // Add this record to the layer table
    pLayerTbl->add(pLayerTblRecord); 
    pLayerTbl->getAt(sLayerName, objLayer);

    // Close the record
    pLayerTblRecord->close();
  }

  // Close the pointer of the layer table
  pLayerTbl->close();

  // Set the layer as the current layer and modify the status of other layers accordingly
	if (doSetLayer == Adesk::kFalse) return;
  setLayer(sLayerName, iFreezeRest);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : toggleVPortLayers
// Arguments   : CStringArray &, Array of layer names to be switched off
// Return      : None
// Called from : switchOfLayers
// Comments    : Turns off/on the given layers in the current view port.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleVPortLayers(CStringArray &csaLayers)
{
	// Get the current CVPORT value
	struct resbuf rbCVPort; acedGetVar(_T("CVPORT"), &rbCVPort);
	
	// Get the current layer name
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	// Create a object ID array of layers to be frozen and thawed
	AcDbObjectIdArray objLayerIDForFreeze;
	AcDbObjectIdArray objLayerIDForThaw;
	AcDbObjectIdArray objLayersID;

	const TCHAR *pLayerName;
	const TCHAR *sLayerName[254];
	CString csLayer;
	AcDbObjectId objLayerId;
	AcDbLayerTable *pLayerTbl = NULL;
	
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); return; }

  /*
	for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
	{
		es = pLayerTbl->getAt(csaLayers.GetAt(iCtr), objLayerId, AcDb::kForRead);
		if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pLayerTbl->close(); return; }
		objLayersID.append(objLayerId);
	}
	pLayerTbl->close();

	// Get the current view port pointer
	AcDbLayoutManager *pLayoutMngr = acdbHostApplicationServices()->layoutManager();
	if (pLayoutMngr == NULL) return;

	//.. Get the view ports placed in the current layout
	AcDbLayout *pLayout = pLayoutMngr->findLayoutNamed(pLayoutMngr->findActiveLayout(Adesk::kFalse), TRUE);
	AcDbObjectIdArray objIdArray = pLayout->getViewportArray();
	pLayout->close();

	for (int iIndex = 0; iIndex < objIdArray.length(); iIndex++)
	{
		AcDbViewport *pVPort = NULL;
		if ((es = acdbOpenObject(pVPort, objIdArray[iIndex], AcDb::kForWrite)) == Acad::eOk)
		{
			// Not the current viewport
			if (pVPort->number() != rbCVPort.resval.rint) { pVPort->close(); continue; }
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
			
			for (int iCtr = 0; iCtr < objLayersID.length(); iCtr++)
			{
				if (pVPort->isLayerFrozenInViewport(objLayersID[iCtr]))
					objLayerIDForThaw.append(objLayersID[iCtr]);
				else 
					objLayerIDForFreeze.append(objLayersID[iCtr]);
			}
			
			if (objLayerIDForFreeze.length() > 0)
			{
				es = pVPort->freezeLayersInViewport(objLayerIDForFreeze);
				if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
			}

			if (objLayerIDForThaw.length() > 0)
			{
				pVPort->thawLayersInViewport(objLayerIDForThaw);
				if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
			}

			pVPort->close();
			break;
		}
		else { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
	}

	acedCommandS(RTSTR, _T(".REGEN"), NULL); 
	return;
  */

  	//////////////////////////////////////////////////////////////////////////
	// The code beneath will switch on layers in the array and switch on/off
	// layers that are not a part of the array.
	//////////////////////////////////////////////////////////////////////////
  AcDbLayerTableIterator *pLayerIterator = NULL; 
  AcDbLayerTableRecord   *pLayerTblRecord = NULL;
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
  pLayerTbl->newIterator(pLayerIterator);
	bool bMatched;
  bool bIsFrozen;
  for (; !pLayerIterator->done(); pLayerIterator->step())
  {  
		bMatched = FALSE;

		pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForRead);
		pLayerTblRecord->getName(pLayerName);
    bIsFrozen = pLayerTblRecord->isFrozen();
		pLayerTblRecord->close();
		
		for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
		{
			csLayer.Format(_T("%s"), pLayerName);
			if (!csLayer.CompareNoCase(csaLayers.GetAt(iCtr))) { bMatched = TRUE; break; }
		}

		// Add the layer object id to the appropriate array
		pLayerTbl->getAt(pLayerName, objLayerId, AcDb::kForRead); pLayerTbl->close();
		if (bMatched || bIsFrozen) 
      objLayerIDForThaw.append(objLayerId);
    else
      objLayerIDForFreeze.append(objLayerId);
	}
  delete pLayerIterator;
  pLayerTbl->close();
	
	// Get the current view port pointer
	AcDbLayoutManager *pLayoutMngr = acdbHostApplicationServices()->layoutManager();
	if (pLayoutMngr == NULL) return;

	//.. Get the view ports placed in the current layout
	AcDbLayout *pLayout = NULL;//pLayoutMngr->findLayoutNamed(pLayoutMngr->findActiveLayout(Adesk::kFalse), TRUE);
	pLayoutMngr->getLayoutNamed(pLayoutMngr->findActiveLayout(Adesk::kFalse), pLayout, AcDb::OpenMode::kForRead);
	if (pLayout == NULL) return;
	AcDbObjectIdArray objIdArray = pLayout->getViewportArray();
	pLayout->close();
	
	for (int iIndex = 0; iIndex < objIdArray.length(); iIndex++)
	{
		AcDbViewport *pVPort = NULL;
		if ((es = acdbOpenObject(pVPort, objIdArray[iIndex], AcDb::kForWrite)) == Acad::eOk)
		{
			// Not the current viewport
			if (pVPort->number() != rbCVPort.resval.rint) { pVPort->close(); continue; }
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			es = pVPort->freezeLayersInViewport(objLayerIDForFreeze);
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			pVPort->thawLayersInViewport(objLayerIDForThaw);
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			pVPort->close();
			break;
		}
		else { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : turnOnVPortLayers
// Arguments   : CStringArray &, Array of layer names to be switched on
// Return      : None
// Called from : turnOnLayers
// Comments    : Turns on the given layers in the current view port and if a layer name has the
//               substring that is also turned on. Rest of the layers are turned off.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void turnOnVPortLayers(CStringArray &csaLayers, CString csSubStr)
{
	// Get the current CVPORT value. If the current viewport number is _T("1") simply return the function
	struct resbuf rbCVPort; acedGetVar(_T("CVPORT"), &rbCVPort);
	
	// Get the current layer name
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	// Create a object ID array of layers to be frozen and thawed
	AcDbObjectIdArray objLayerIDForFreeze;
	AcDbObjectIdArray objLayerIDForThaw;
	AcDbObjectIdArray objLayersID;

	const TCHAR *pLayerName;
	const TCHAR *sLayerName[254];
	CString csLayer;
	AcDbObjectId objLayerId;
	AcDbLayerTable *pLayerTbl = NULL;

	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); return; }
	
	////////////////////////////////////////////////////////////////////////////////////////////
	// Iterate through the layer table array and separately collect layers that are to be 
	// switched on and off.
	////////////////////////////////////////////////////////////////////////////////////////////
	// Get the layer table iterator
	AcDbLayerTableIterator *pLayerIterator = NULL; 
	AcDbLayerTableRecord   *pLayerTblRecord = NULL;
	pLayerTbl->newIterator(pLayerIterator);
	pLayerTbl->close();

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	// Get the current layer name. If it has to be turned off, we can set layer "0" as current.
	AcDbObjectId objLayer;
	bool bSetLayerZeroCurrent = false;
	csLayer.Format(_T("%s"), rbCLayer.resval.rstring);	csLayer = csLayer.MakeUpper();
	if ((csLayer.Find(csSubStr + _T("_"), 0) != -1) || (csLayer.Find(_T("_") + csSubStr + _T("_"), 0) != -1) || !csLayer.CompareNoCase(csSubStr))
	{
		// Nothing to be done
	}
	else
	{
		// Set the layer specified as current
		es = pLayerTbl->getAt(_T("0"), pLayerTblRecord, AcDb::kForWrite);
		if (es != Acad::eOk) 
		{
			acutPrintf(_T("\nERROR %d: %s"), __LINE__, acadErrorStatusText(es));
			pLayerTbl->close();
			return;
		}
		else
		{
			// Thaw the layer
			pLayerTblRecord->setIsFrozen(Adesk::kFalse);
			bSetLayerZeroCurrent = true;
			objLayer = pLayerTblRecord->objectId();
			pLayerTblRecord->close();
		}
	}

	// Close the layer table
	pLayerTbl->close();

	// Turn on the layer "0" is required
	if (bSetLayerZeroCurrent) {	acdbHostApplicationServices()->workingDatabase()->setClayer(objLayer); }
	//////////////////////////////////////////////////////////////////////////
			
	bool bMatched;
	for (; !pLayerIterator->done(); pLayerIterator->step())
	{ 
		pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForRead);
		pLayerTblRecord->getName(pLayerName); csLayer.Format(_T("%s"), pLayerName); csLayer = csLayer.MakeUpper();
		objLayerId = pLayerTblRecord->objectId();
		pLayerTblRecord->close();

		// If the layer name has the given substring it must be turned on
		if ((csLayer.Find(csSubStr + _T("_"), 0) != -1) || (csLayer.Find(_T("_") + csSubStr + _T("_"), 0) != -1) || !csLayer.CompareNoCase(csSubStr))
		{
			objLayerIDForThaw.append(objLayerId);
			continue; 
		}

		// If the layer name is in the layers array it is turned on as well
		bMatched = FALSE;
		for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
		{
			if (!csLayer.CompareNoCase(csaLayers.GetAt(iCtr))) { bMatched = TRUE; break; }
		}

		// Add the layer object id to the appropriate array
		if (bMatched) objLayerIDForThaw.append(objLayerId); else objLayerIDForFreeze.append(objLayerId);
	}

	delete pLayerIterator;

	// Get the current view port pointer
	AcDbLayoutManager *pLayoutMngr = acdbHostApplicationServices()->layoutManager();
	if (pLayoutMngr == NULL) return;

	//.. Get the view ports placed in the current layout
	AcDbLayout *pLayout = NULL;// pLayoutMngr->findLayoutNamed(pLayoutMngr->findActiveLayout(Adesk::kFalse), TRUE);
	pLayoutMngr->getLayoutNamed(pLayoutMngr->findActiveLayout(Adesk::kFalse), pLayout, AcDb::OpenMode::kForRead);
	if (pLayout == NULL) return;
	AcDbObjectIdArray objIdArray = pLayout->getViewportArray();
	pLayout->close();

	for (int iIndex = 0; iIndex < objIdArray.length(); iIndex++)
	{
		AcDbViewport *pVPort = NULL;
		if ((es = acdbOpenObject(pVPort, objIdArray[iIndex], AcDb::kForWrite)) == Acad::eOk)
		{
			// Not the current viewport
			if (pVPort->number() != rbCVPort.resval.rint) { pVPort->close(); continue; }
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			es = pVPort->freezeLayersInViewport(objLayerIDForFreeze);
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			pVPort->thawLayersInViewport(objLayerIDForThaw);
			if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }

			pVPort->close();
			break;
		}
		else { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); pVPort->close(); return; }
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : toggleLayerDisplay
// Arguments   : CStringArray &, Array of layer names to be switched off/on
// Return      : None
// Called from : Anywhere
// Comments    : Turns on the layers in the given array and turns on the layers that has the input
//               string in its name. Rest of the layers are switched off.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void turnOnLayers(const TCHAR *pCurrentLayerName, CStringArray &csaLayers, CString csSubStr)
{
	// Set the current space to MODEL
	if (!acdbHostApplicationServices()->workingDatabase()->tilemode()) { turnOnVPortLayers(csaLayers, csSubStr); return; }

	// When in model space
	AcDbObjectId objLayer;
	AcDbLayerTable *pLayerTbl = NULL;
	AcDbLayerTableIterator *pLayerIterator = NULL;
	AcDbLayerTableRecord *pLayerTblRecord = NULL;

	// For each layer in the layer table take appropriate action
	const TCHAR *pLayerName;
	
	CString csLayer;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); return; }

	es = pLayerTbl->newIterator(pLayerIterator);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); return; }

	// Get the current layer name. If it has to be turned off, we can set layer "0" as current.
	bool bSetLayerZeroCurrent = false;
	csLayer.Format(_T("%s"), pCurrentLayerName);	csLayer = csLayer.MakeUpper();
	if ((csLayer.Find(csSubStr + _T("_"), 0) != -1) || (csLayer.Find(_T("_") + csSubStr + _T("_"), 0) != -1) || !csLayer.CompareNoCase(csSubStr))
	{
		// Nothing to be done
	}
	else
	{
		// Set the layer specified as current
		es = pLayerTbl->getAt(_T("0"), pLayerTblRecord, AcDb::kForWrite);
		if (es != Acad::eOk) 
		{
			acutPrintf(_T("\nERROR %d: %s"), __LINE__, acadErrorStatusText(es));
			pLayerTbl->close();
			return;
		}
		else
		{
			// Thaw the layer
			pLayerTblRecord->setIsFrozen(Adesk::kFalse);
			bSetLayerZeroCurrent = true;
			objLayer = pLayerTblRecord->objectId();
			pLayerTblRecord->close();
		}
	}

	// Close the layer table
	pLayerTbl->close();

	// Turn on the layer "0" is required
	if (bSetLayerZeroCurrent)
	{
		pCurrentLayerName = _T("0");
		acdbHostApplicationServices()->workingDatabase()->setClayer(objLayer);
	}

	for (; !pLayerIterator->done(); pLayerIterator->step())
	{  
		es = pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForWrite); 
		if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); delete pLayerTblRecord;	return; }
	
		es = pLayerTblRecord->getName(pLayerName);
		if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); delete pLayerTblRecord;	return; }

		csLayer.Format(_T("%s"), pLayerName);	csLayer = csLayer.MakeUpper();

		// If layer 0 is set as current thaw it
		
		// By default switch off the layer if it is not the current layer. AS we know current layers cannot be switched to frozen.
		if (_tcsicmp(pLayerName, pCurrentLayerName)) { pLayerTblRecord->setIsFrozen(true); 
		} 
				
		// If the layer name has the given substring it must be turned on
		if ((csLayer.Find(csSubStr + _T("_"), 0) != -1) || (csLayer.Find(_T("_") + csSubStr + _T("_"), 0) != -1) || !csLayer.CompareNoCase(csSubStr))
		{
			// For "HV_*" or "*_HV_*" or "HV"
			pLayerTblRecord->setIsFrozen(false); pLayerTblRecord->close(); continue; 
		}
		
		// If the layer name is present in the layer array. All layers in the array has to be switched on too.
		for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
		{
			if (!csLayer.CompareNoCase(csaLayers.GetAt(iCtr))) 
			{
				pLayerTblRecord->setIsFrozen(false);	
				break; 
			}
		}
		
		pLayerTblRecord->close();
	}
	
	// Delete the layer iterator and close the Layer Table pointer
	delete pLayerIterator;
} // csSubStr

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : toggleLayerDisplay
// Arguments   : CStringArray &, Array of layer names to be switched off/on
// Return      : None
// Called from : Anywhere
// Comments    : Turns off/on the layers in the given array.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleLayerDisplay(CStringArray &csaLayers)
{
	// Set the current space to MODEL
	if (!acdbHostApplicationServices()->workingDatabase()->tilemode()) { toggleVPortLayers(csaLayers); return; }

	// Get the current layer name
	struct resbuf rbCLayer; acedGetVar(_T("CLAYER"), &rbCLayer);

	AcDbLayerTable *pLayerTbl = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
	if (es != Acad::eOk) { acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); return; }

  CString csLayer;
  const TCHAR *pLayerName;
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);
  AcDbLayerTableIterator *pLayerIterator; pLayerTbl->newIterator(pLayerIterator);
  AcDbLayerTableRecord *pLayerTblRecord = NULL;
  bool bMatched;
  for (; !pLayerIterator->done(); pLayerIterator->step())
  {  
    pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForWrite);
    pLayerTblRecord->getName(pLayerName);
    
    bMatched = FALSE;
    for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
    {
      csLayer.Format(_T("%s"), pLayerName);
      if (!csLayer.CompareNoCase(csaLayers.GetAt(iCtr))) { bMatched = TRUE; break; }
    }

    // Always THAW the layer in the array and for other layers toggle the visibility.
    if (bMatched)
      pLayerTblRecord->setIsFrozen(FALSE); 
    else
      pLayerTblRecord->setIsFrozen(!pLayerTblRecord->isFrozen()); 

    pLayerTblRecord->close();
  }

  delete pLayerIterator;
  pLayerTbl->close();
  

  //////////////////////////////////////////////////////////////////////////
  // Below code thaws/freezes all the layers specified in the array
  //////////////////////////////////////////////////////////////////////////
  /*
	AcDbLayerTableRecord *pLayerTblRecord = NULL;
	for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
	{
		es = pLayerTbl->getAt(csaLayers.GetAt(iCtr), pLayerTblRecord, AcDb::kForWrite);
		if (es != Acad::eOk)
		{
			// acutPrintf(_T("\nError at %d : %s"), __LINE__, acadErrorStatusText(es)); 
			pLayerTbl->close(); return; 
		}

		if (!pLayerTblRecord->isFrozen() && !csaLayers.GetAt(iCtr).CompareNoCase(rbCLayer.resval.rstring))
		{
			acutPrintf(_T("\nLayer %s is the current layer and cannot be switched off."), csaLayers.GetAt(iCtr));
		}

		pLayerTblRecord->setIsFrozen(!pLayerTblRecord->isFrozen());
		pLayerTblRecord->close();
	}

	pLayerTbl->close();
  */


	acedCommandS(RTSTR, _T(".REGEN"), NULL);
	return;

	//////////////////////////////////////////////////////////////////////////
	// The code beneath will switch off layers in the array and switch on 
	// layers that are not a part of the array.
	//////////////////////////////////////////////////////////////////////////

	/*
	AcDbObjectId objLayer;
	AcDbLayerTable *pLayerTbl = NULL;
	AcDbLayerTableIterator *pLayerIterator = NULL;
	AcDbLayerTableRecord *pLayerTblRecord = NULL;

	// For each layer in the layer table take appropriate act
	const TCHAR *pLayerName;
	const TCHAR *sLayerName[254];
	CString csLayer;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForWrite);
	pLayerTbl->newIterator(pLayerIterator);
	pLayerTbl->close();

	for (; !pLayerIterator->done(); pLayerIterator->step())
	{  
		pLayerIterator->getRecord(pLayerTblRecord, AcDb::kForWrite);
		pLayerTblRecord->getName(pLayerName);
		pLayerTblRecord->setIsFrozen(false);

		for (int iCtr = 0; iCtr < csaLayers.GetSize(); iCtr++)
		{
			csLayer.Format(_T("%s"), pLayerName);
			if (!csLayer.CompareNoCase(csaLayers.GetAt(iCtr))) 
			{
				if (!_tcsicmp(pLayerName, sLayerName))
				{
					acutPrintf(_T("\nLayer %s is the current layer and cannot be switched off."));
				}
				pLayerTblRecord->
				pLayerTblRecord->setIsFrozen(true);	
				break; 
			}
		}
		
		pLayerTblRecord->close();
	}

	// Delete the layer iterator and close the Layer Table pointer
	delete pLayerIterator;
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function    : purgeLayer
// Arguments   : const TCHAR * giving the name of the layer to be purged
// Return      : TRUE if successful, FALSE if not
// Called from : Anywhere
// Comments    : Checks if this layer has any hard references and purges it, if not.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL purgeLayer(const TCHAR *sLayerName)
{
  CString csError;
  AcDbObjectId objLayer;
  AcDbObjectIdArray objArray;
  AcDbLayerTable *pLayerTbl;

  // Get the layer table pointer for reading
  acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

  // If the layer doesn't exist return with a message not to repeat such stupidities in future
  if (!(pLayerTbl->has(sLayerName))) 
  { 
    pLayerTbl->close(); 
    csError.Format(_T("Layer %s does not exist."), sLayerName);
    appMessage(csError, MB_ICONSTOP);
    return FALSE; 
  }

  // Get the object id of this layer and add it to the object id array
  pLayerTbl->getAt(sLayerName, objLayer);
  pLayerTbl->close();
  objArray.append(objLayer);

  // Now, call the purge() function on the current database with this array
  acdbHostApplicationServices()->workingDatabase()->purge(objArray);

  // If the array does not contains the object id, there are some hard references to it
  if (!objArray.contains(objLayer))
  { 
    csError.Format(_T("Layer %s is hard referenced or it is the current layer."), sLayerName);
    //appMessage(csError, MB_ICONSTOP);
    return FALSE;
  }

  // Open the object and call its erase() function
  AcDbLayerTableRecord *pLayerTblRecord;
  if (acdbOpenObject(pLayerTblRecord, objLayer, AcDb::kForWrite) != Acad::eOk) 
  { appMessage(_T("Unable to get layer table record pointer."), MB_ICONSTOP); return FALSE; } 
  pLayerTblRecord->erase();
  pLayerTblRecord->close();

  // All's well that ends well
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : changeAttribsLayer
// Arguments        : 1. The parent entity, as object id
//                    2. The destination layer, as string
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Loops through the parent entity and for each attribute found, changes its layer to
//                    the destination layer. This function is necessitated because if a user creates a block
//                    on a layer other that 0, the attrib entities tend to remain on that layer even if the 
//                    block is inserted on a different layer. Strange bug, but it is there.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void changeAttribsLayer(AcDbObjectId objInsertId, const TCHAR *pDestLayer)
{
  // Get the attributes attached to this insert
  AcDbBlockReference *pInsert;
  acdbOpenObject(pInsert, objInsertId, AcDb::kForRead);
  AcDbObjectIterator *pIter = pInsert->attributeIterator();
  pInsert->close();

  // For all attributes in this block, change the layer to the destination layer
  AcDbAttribute *pAtt;
  AcDbObjectId objAttId;
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    objAttId = pIter->objectId();
    acdbOpenObject(pAtt, objAttId, AcDb::kForWrite);
    pAtt->setLayer(pDestLayer);
    pAtt->close();
  }

  delete pIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : rotateAttribs
// Arguments        : 1. The parent entity, as object id
//                    2. Rotation angle.
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Rotates the attributes within the given block to the given angle.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rotateAttribs(ads_name enBlock, double dRotation)
{
  AcDbObjectId objInsertId;
  if (acdbGetObjectId(objInsertId, enBlock) != Acad::eOk) return;

  // Get the attributes attached to this insert
  AcDbBlockReference *pInsert;
  acdbOpenObject(pInsert, objInsertId, AcDb::kForRead);
  AcDbObjectIterator *pIter = pInsert->attributeIterator();
  pInsert->close();

  // Rotate all attributes of this block back to zero.
  AcDbAttribute *pAtt;
  AcDbObjectId objAttId;

  for (pIter->start(); !pIter->done(); pIter->step())
  {
    objAttId = pIter->objectId();
    acdbOpenObject(pAtt, objAttId, AcDb::kForWrite);
    pAtt->setRotation(0.0);
    AcGePoint3d ptIns = pAtt->alignmentPoint();
    if (pAtt->horizontalMode() != AcDb::kTextLeft) pAtt->setPosition(ptIns);
    pAtt->close();
  }

  delete pIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : buildAttribsList
// Arguments        : 1. Symbol name, as string
//                    2. Resultant array of prompts, as reference to CStringArray
//                    3. Resultant array of tags, as reference to CStringArray
//                    4. Resultant array of defaults, as reference to CStringArray
//
// Called from      : Anywhere
// Returns          : CStringArray which contains the attribute prompts.
// Description      : Reads the specified drawing and fills the array with attribute prompts.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int buildAttribsList(CString csSymName, CStringArray &csPrompts, CStringArray &csTags, CStringArray &csDefault)
{
  TCHAR sSymName[256];
  _tcscpy(sSymName, LPCTSTR(csSymName));

  // Check for the existence of the given file
  if (_taccess(sSymName, 00)) return 0;

  // Open the drawing for reading, without opening it in the AutoCAD Drawing Editor
  AcDbDatabase *pDb = new AcDbDatabase(Adesk::kFalse);
  if (pDb->readDwgFile(sSymName) != Acad::eOk) { delete pDb; return 0; }

  // Open the model space block table record
  AcDbBlockTable *pBlkTbl;
  AcDbBlockTableRecord *pBlkTblRcd;
  if (pDb->getBlockTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) { delete pDb; return 0; }
  if (pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead) != Acad::eOk) { delete pDb; return 0; }
  pBlkTbl->close(); 

  // Initialise a new block table record iterator
  AcDbBlockTableRecordIterator *pBlkTblRcdItr;
  pBlkTblRcd->newIterator(pBlkTblRcdItr);

  // For each entity in the database, check if it is an ATTDEF object
  int iNumAttribs = 0;
  AcDbEntity *pEnt;
  AcDbAttributeDefinition *pAttdef;
  for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
  {
    // Get the next entity in the drawing database. The check is in case the database is corrupted
    if (pBlkTblRcdItr->getEntity(pEnt, AcDb::kForRead) != Acad::eOk) { delete pBlkTblRcdItr; delete pDb; return 0; }

    // Cast the entity pointer as an AcDbAttributeDefinition pointer. 
    // If it returns NULL, then this entity is not an ATTDEF object
    if (pAttdef = AcDbAttributeDefinition::cast(pEnt))

    {
      // Add this attribute to the array
      csPrompts.Add(pAttdef->prompt());
      csTags.Add(pAttdef->tag());
      csDefault.Add(pAttdef->textString());
      iNumAttribs++;
    }

    // Close the entity
    pEnt->close();
  }

  // Close the block table record and delete the pointers
  pBlkTblRcd->close();  
  delete pBlkTblRcdItr; 
  delete pDb;           

  return iNumAttribs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : appendEntityToDatabase
// Description  : Adds an entity to the current drawing's database
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AcDbObjectId appendEntityToDatabase(AcDbEntity *pEntity, BOOL bIsMSpace)
{
  // Get the current drawing's block table
  Acad::ErrorStatus es;
  AcDbObjectId objId;
  AcDbBlockTable *pBlockTable;
  es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
  if (es != Acad::eOk) { acutPrintf(_T("\n%s opening block table for reading."), acadErrorStatusText(es)); return objId; }

  // Get the model space block table record from the block table
  AcDbBlockTableRecord *pSpaceRecord;
	es = pBlockTable->getAt((bIsMSpace ? ACDB_MODEL_SPACE : ACDB_PAPER_SPACE), pSpaceRecord, AcDb::kForWrite);
  if (es != Acad::eOk) { acutPrintf(_T("\n%s retrieving model space for writing."), acadErrorStatusText(es)); pBlockTable->close(); return objId; }

  // Close the block table
  pBlockTable->close();

  // Append the equipment to the model space block table record
  es = pSpaceRecord->appendAcDbEntity(objId, pEntity);
  if (es != Acad::eOk) { acutPrintf(_T("\n%s appending equipment to model space."), acadErrorStatusText(es)); pSpaceRecord->close(); return objId; }  

  // Close the model space block table record
  pSpaceRecord->close();

  // Success
  return objId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : ensureSymbolAvailability
// Description  : Checks if the block is available and if not, clones it from the standard drawing into the current drawing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ensureSymbolAvailability(CString csSymbol, AcDbObjectId& objBlock)
{
  Acad::ErrorStatus es;

  // Get a pointer to the current drawing
  AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();

  // If this symbol is not already present
  if (acdbTblSearch(_T("BLOCK"), csSymbol, 0) == NULL)
  {
    // Set the "PROXYNOTICE" system variable to 0
    CString csProxyNotice = _T("PROXYNOTICE");
    struct resbuf rbProxyNotice;
    acedGetVar(csProxyNotice, &rbProxyNotice);
    int iOldProxyNotice = rbProxyNotice.resval.rint;
    rbProxyNotice.resval.rint = 0;
    acedSetVar(csProxyNotice, &rbProxyNotice);

    // Open the standards drawing for reading
    AcDbDatabase *pStdDb = new AcDbDatabase;
    if ((es = pStdDb->readDwgFile(g_csDWGLocation, AcDbDatabase::kForReadAndAllShare)) != Acad::eOk) { delete pStdDb ; acutPrintf(_T("\nError %s reading standards drawing.\n"), acadErrorStatusText(es)); return FALSE; }

    // Get the Block Table from standards drawing
    AcDbBlockTable *pStdBT;
    if ((es = pStdDb->getSymbolTable(pStdBT, AcDb::kForRead)) != Acad::eOk) { delete pStdDb ; acutPrintf(_T("\nError %s getting block table from standards drawing.\n"), acadErrorStatusText(es)); return FALSE; }

    // Get the Block Table from the current drawing
    AcDbBlockTable *pCurBT;
    if ((es = pCurDb->getSymbolTable(pCurBT, AcDb::kForRead)) != Acad::eOk) { delete pStdDb ; acutPrintf(_T("\nError %s getting block table from current drawing.\n"), acadErrorStatusText(es)); return FALSE; }

    // Get the object ID of the current block table
    AcDbObjectId curBTObjectId = pCurBT->objectId();
    pCurBT->close();

    // Get the required block from the standards drawing
    AcDbObjectId objStdBlock;
    if ((es = pStdBT->getAt(csSymbol, objStdBlock)) != Acad::eOk) { pStdBT->close(); delete pStdDb; acutPrintf(_T("\nBlock '%s' not found in standards drawing.\n"), csSymbol); return FALSE; }
    pStdBT->close();

    // Create an object ID array
    AcDbObjectIdArray objectIds;
    objectIds.append(objStdBlock);

    // Clone the block from the standard drawing into the current drawing
    AcDbIdMapping idMap;
    if ((es = pCurDb->wblockCloneObjects(objectIds, curBTObjectId, idMap, AcDb::kDrcIgnore)) != Acad::eOk) { delete pStdDb ; acutPrintf(_T("\nError %s inserting block into current drawing.\n"), acadErrorStatusText(es)); return FALSE; }

    // Delete the standards drawing pointer
    delete pStdDb;

    // Reset the "PROXYNOTICE" system variable
    rbProxyNotice.resval.rint = iOldProxyNotice;
    acedSetVar(csProxyNotice, &rbProxyNotice);
  }

  // Get the Block Table from the current drawing
  AcDbBlockTable *pCurBT;
  if ((es = pCurDb->getSymbolTable(pCurBT, AcDb::kForRead)) != Acad::eOk) { acutPrintf(_T("\nError %s getting block table from current drawing.\n"), acadErrorStatusText(es)); return FALSE; }

  // Get the object id of the required block
  if ((es = pCurBT->getAt(csSymbol, objBlock)) != Acad::eOk) { pCurBT->close(); acutPrintf(_T("\nError %s getting block from current drawing.\n"), acadErrorStatusText(es)); return FALSE; }
  pCurBT->close();

  // Success
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function name: saveOSMode()
// Description  :
//////////////////////////////////////////////////////////////////////////
void saveOSMode()
{
	// Get the OSMODE and add it to XREC	
	struct resbuf rbOsmode; acedGetVar(L"OSMODE", &rbOsmode);
	struct resbuf *rbpOSMode = acutBuildList(AcDb::kDxfInt16, rbOsmode.resval.rint, NULL);
	addXRecordToDictionary(DICT_IETB, _T("IE_TB_OSMODE"), rbpOSMode);
	acutRelRb(rbpOSMode);

	// Switch off OSMODE
	struct resbuf rbSetvar; rbSetvar.restype = RTSHORT; rbSetvar.resval.rint = 0; acedSetVar(_T("OSMODE"), &rbSetvar);
}

void restoreOSMode()
{
	// Set the OSMODE saved in the XREC
	struct resbuf *rbpOSMode = getXRecordFromDictionary(DICT_IETB, _T("IE_TB_OSMODE"));
	if (rbpOSMode) 
	{
		struct resbuf rbSetvar; rbSetvar.restype = RTSHORT; 
		rbSetvar.resval.rint = rbpOSMode->resval.rint; acedSetVar(_T("OSMODE"), &rbSetvar);

		acutRelRb(rbpOSMode);
	}
}

//////////////////////////////////////////////////////////////////////////
// Function			: setOptionParameters
// Description  : Retrieves the default values from the current drawing or
//////////////////////////////////////////////////////////////////////////
void setOptionParameters()
{
	g_csPoleTextStyle  = _T("Standard");
	g_csPoleTextHeight = _T("2.5");
	g_csPoleDiameter	 = _T("2.0");
	g_csSchTextStyle	 = _T("Standard");
	g_csSchTextHeight	 = _T("2.0");

	// Retrieve the current values for the drawings xrecord
	struct resbuf *rbpOptions = getXRecordFromDictionary(DICT_IETB, XREC_IETB);
	if (rbpOptions != NULL)
	{
		g_csPoleTextStyle  = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csPoleTextHeight = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csPoleDiameter   = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csSchTextStyle   = rbpOptions->resval.rstring; rbpOptions = rbpOptions->rbnext;
		g_csSchTextHeight  = rbpOptions->resval.rstring;
	}
	else
	{
		// If the Xrecord is not there, take it from the MDB's
		CQueryTbl tblData;
		CString csSQL;
		csSQL.Format(_T("SELECT [PNTextStyle], [PNTextHeight], [PoleDia], [SchTextStyle], [SchTextHeight] FROM [tblConfig]"));
		if (!tblData.SqlRead(DSN_IETB, csSQL, __LINE__, __FILE__, _T("setOptionParameters()"))) return;
		g_csPoleTextStyle  = tblData.GetRowAt(0)->GetAt(0);
		g_csPoleTextHeight = tblData.GetRowAt(0)->GetAt(1);
		g_csPoleDiameter   = tblData.GetRowAt(0)->GetAt(2);
		g_csSchTextStyle   = tblData.GetRowAt(0)->GetAt(3);
		g_csSchTextHeight  = tblData.GetRowAt(0)->GetAt(4);

		// Save the specified information to the drawings xrecord
		rbpOptions = acutBuildList(AcDb::kDxfText, g_csPoleTextStyle, 
														   AcDb::kDxfText, g_csPoleTextHeight,
															 AcDb::kDxfText, g_csPoleDiameter,
															 AcDb::kDxfText, g_csSchTextStyle,
														   AcDb::kDxfText, g_csSchTextHeight, NULL
														  );
		addXRecordToDictionary(DICT_IETB, XREC_IETB, rbpOptions);
		acutRelRb(rbpOptions);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : setLineType
// Description  : Sets the line type specified as current line type. If not found in the current drawing,
//                the same is loaded from the IE_ToolBox standard drawing.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL setLineType(CString csLType)
{
	// Get the symbol table pointer
	Acad::ErrorStatus es;
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();
	AcDbLinetypeTable *pLType = NULL;
	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLType, AcDb::kForRead) != Acad::eOk) return FALSE;
	pLType->close();

	// Check if the line type is present in the current drawing.
	if (!pLType->has(csLType))
	{
		// CString csPath; csPath.Format("%s\\IE_ToolBox.lin", g_csStdDrawing.Mid(0, g_csStdDrawing.Find("\\IE_CAD_Standards.dwg")));
		Acad::ErrorStatus es = acdbLoadLineTypeFile(csLType, g_csBaseFolder + _T("\\EE_ToolBox.lin"), NULL);
		if (es != Acad::eOk) 
		{
			/*CString csMsg; 
			csMsg.Format(_T("\n**ERROR: %s**. Unable to load [%s] line type.\nPlease verify if this line type is defined in EE_ToolBox.lin file."), acadErrorStatusText(es), csLType);
			acutPrintf(csMsg);*/
			return FALSE;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : insertBlock
// Description  : Inserts the block and appends all attributes with null values
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL insertBlock(CString csBlockName, CString csLayerName, ads_point ptIns, double dScale, double dHeight, double dRotation, CString csTextStyle, AcDbObjectId& objInsert, BOOL bIsMSpace)
{
  // Ensure that the symbol is available in the current drawing
  AcDbObjectId objBlock;
	if (ensureSymbolAvailability(csBlockName, objBlock) == FALSE) { return FALSE; }
	
  // Convert the insertion base point
  AcGePoint3d geBasePoint(ptIns[X], ptIns[Y], ptIns[Z]);

  // Create a new block reference object
  AcDbBlockReference *pInsert = new AcDbBlockReference;
  pInsert->setBlockTableRecord(objBlock);
  pInsert->setPosition(geBasePoint);
  pInsert->setNormal(AcGeVector3d(0, 0, 1));
  pInsert->setRotation(dRotation);
  pInsert->setScaleFactors(AcGeScale3d(dScale, dScale, dScale));
  pInsert->setLayer(csLayerName);

  // Add this entity to the current drawing
  // NOTE: This will create the block reference without any attributes in it
  objInsert = appendEntityToDatabase(pInsert, bIsMSpace);
	
  // Open the block definition for read
  AcDbBlockTableRecord *pBlockDef; acdbOpenObject(pBlockDef, objBlock, AcDb::kForRead);

  // Create a new record iterator from this block definition
  AcDbBlockTableRecordIterator *pIterator; pBlockDef->newIterator(pIterator);

  AcDbEntity *pEnt;
  AcDbAttributeDefinition *pAttdef;

  // For each entity in the block
  for (pIterator->start(); !pIterator->done(); pIterator->step()) 
  {
    // Get the next entity
    pIterator->getEntity(pEnt, AcDb::kForRead);

    // Make sure the entity is an attribute definition and not a constant
    pAttdef = AcDbAttributeDefinition::cast(pEnt);
    if (pAttdef != NULL && !pAttdef->isConstant()) 
    {
      // We have a non-constant attribute definition, so create an attribute entity
      AcDbAttribute *pAtt = new AcDbAttribute;
      pAtt->setPropertiesFrom(pAttdef);
      pAtt->setInvisible(pAttdef->isInvisible());
			pAtt->setVisibility(pAttdef->visibility());

			// Translate attribute by block reference
			geBasePoint = pAttdef->position();
			geBasePoint += pInsert->position().asVector();
			pAtt->setPosition(geBasePoint);

			if (dHeight) pAtt->setHeight(dHeight); else pAtt->setHeight(pAttdef->height());
			
      pAtt->setRotation(pAttdef->rotation());
      pAtt->setTag(pAttdef->tag());
      pAtt->setFieldLength(pAttdef->fieldLength());
      pAtt->setHorizontalMode(pAttdef->horizontalMode());
      pAtt->setVerticalMode(pAttdef->verticalMode());
			pAtt->setAlignmentPoint(pAttdef->alignmentPoint() + pInsert->position().asVector());
			      
			// Text style
			if (!csTextStyle.IsEmpty())
			{
				AcDbObjectId objStyle;
				AcDbTextStyleTable *pTxtStyleTbl;
				acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTxtStyleTbl, AcDb::kForRead);
				if (pTxtStyleTbl->getAt(csTextStyle, objStyle) == Acad::eOk) pAtt->setTextStyle(objStyle);
				pTxtStyleTbl->close();
			}

      // By default, the attribute value will be empty
      // pAtt->setTextString(_T(""));
			pAtt->setTextString(pAttdef->textString());

			// Comm. on 29.01.09. 
			// Mail on 28.01.09
			// The issue is if the block is on an SL layer and the user moves the block to an LV layer, when the user uses the view control 
			// command to set to the LV view only the graphics are displayed and not the asset number as it's still on the SL layer. 
			// I also have this block on the tool palette. If the user inserts the block from there then the attributes are placed on layer 0 
			// (due to the block definition) and the user can change the layer and the asset number displays as expected.
			// pAtt->setLayer(csLayerName); 
			pAtt->setLayer(pAttdef->layer()); 
						
      // Append the attribute to the block reference
      pInsert->appendAttribute(pAtt);

      // Close the attribute 
      pAtt->close();
    }

    // Close the entity
    pEnt->close();
  }

  // Delete the iterator
  delete pIterator;

  // Close the block definition
  pBlockDef->close();

  // Close the block reference
  pInsert->close();

  // Success
  return TRUE;
}

//..//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//.. Function Name :  GetParameterValue
//.. Arugument     :  1. csaParameter as CStringArray
//..                  2. csParameter as CString
//..                  3. csValue as CString
//.. Called From   :  Anywhere 
//.. Return Type   :  int - Pos Of the parameter in the array
//.. Description   :  This function will give value for the parameter passed
//..//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetParameterValue(CStringArray &csaParameter, CString csParameter, CString &csValue, int iSearch)
{
	//.. Initialize the value to null
	csValue.Empty();

	//.. Add '#' at the end of the string.
	csParameter = csParameter + _T("#");

	//.. Check for the value in each single element of the array.
	for (int iCounter = ((iSearch < 0) ? 0 : iSearch); iCounter < csaParameter.GetSize(); iCounter++)
	{
		//MessageBox(NULL, csaParameter.GetAt(iCounter), _T("Test"), MB_OK); 
		if (csaParameter.GetAt(iCounter).Find(csParameter) == 0)
		{
			csValue = csaParameter.GetAt(iCounter).Mid(csParameter.GetLength())  ;
			//MessageBox(NULL, csValue, _T("Test"), MB_OK); 

			return iCounter;
		}
	}
	//.. Everything is OK
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : ChangeAttributeLocation
// Arguments        : 1. Parent entity name.
//                    2. Attribute tag for which position has to be changed.
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Relocates the attribute value for the given tag.
////////////////////////////////////////////////////////////////////////////////////////////////
void ChangeAttributeLocation(AcDbObjectId &objInsertId, CString csAttribTag, CString csDirection)
{
	// Get the attributes attached to this insert
	AcDbBlockReference *pInsert;	
	acdbOpenObject(pInsert, objInsertId, AcDb::kForRead);
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	AcGeScale3d acgeScale = pInsert->scaleFactors();
	ads_point ptInsert = { pInsert->position().x, pInsert->position().y, 0.0 };

	// Get the geometry extents
	AcDbExtents exInsert; 
	if (pInsert->geomExtentsBestFit(exInsert) != Acad::eOk) { pInsert->close(); return; }
	AcGePoint3d geMin; geMin = exInsert.minPoint();
	AcGePoint3d geMax; geMax = exInsert.maxPoint();
	pInsert->close();

	// Change the location of the specified attribute.
	AcDbAttribute *pAtt;
	AcDbObjectId objAttId;
	AcGePoint3d ptIns, ptPos;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		objAttId = pIter->objectId();
		acdbOpenObject(pAtt, objAttId, AcDb::kForWrite);
		
		CString csTag; csTag.Format(_T("%s"), pAtt->tag());
		if (!csTag.CompareNoCase(csAttribTag))
		{
			if (csDirection == _T("NW"))
			{
				ptPos.x = geMin.x; ptPos.y = geMax.y; ptPos.z = 0.0;
				pAtt->setPosition(ptPos);

				ptIns.x = geMin.x; ptIns.y = geMax.y; ptIns.z = 0.0;
				pAtt->setAlignmentPoint(ptIns);
			}
			else if (csDirection == _T("E"))
			{
				ptPos.x = ((geMin.x + geMax.x) / 2) + acgeScale.sx + 2.0; 
				ptPos.y = ((geMin.y + geMax.y) / 2) - (acgeScale.sx / 2); ptPos.z = 0.0;
				
				pAtt->setPosition(ptPos);

				ptIns.x = ((geMin.x + geMax.x) / 2) + acgeScale.sx; 
				ptIns.y = ((geMin.y + geMax.y) / 2) - (acgeScale.sx / 2); ptIns.z = 0.0;

				pAtt->setAlignmentPoint(ptIns);
			}
		}
		
		pAtt->close();
	}

	delete pIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : ChangeAttributeLocation
// Arguments        : 1. Parent entity name.
//                    2. Attribute tag for which position has to be changed.
// Returns          : Nothing
// Called from      : Anywhere
// Description      : Relocates the attribute value for the given tag.
////////////////////////////////////////////////////////////////////////////////////////////////
void ChangeAttributeValue(AcDbObjectId &objInsertId, CString csAttribTag, CString csAttValue)
{
	// Get the attributes attached to this insert
	AcDbBlockReference *pInsert;	
	acdbOpenObject(pInsert, objInsertId, AcDb::kForRead);
	AcDbObjectIterator *pIter = pInsert->attributeIterator();
	pInsert->close();

	// Change the location of the specified attribute.
	AcDbAttribute *pAtt;
	AcDbObjectId objAttId;
	AcGePoint3d ptIns, ptPos;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		objAttId = pIter->objectId();
		acdbOpenObject(pAtt, objAttId, AcDb::kForWrite);

		CString csTag; csTag.Format(_T("%s"), pAtt->tag());
		if (!csTag.CompareNoCase(csAttribTag))
		{
			pAtt->setTextString(csAttValue);
		}

		pAtt->close();
	}

	delete pIter;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : compare
// Description      : Used in qsort() to sort integers.
////////////////////////////////////////////////////////////////////////////////////////////////
int compare (const void * a, const void * b) { return ( *(int*)a - *(int*)b ); }

//////////////////////////////////////////////////////////////////////////
// Function name	: MakeLegendEnabled
// Description    : Attaches XDATA called "LEGEND" which will make this block
//                  recognized for LEGEND table generation.
//////////////////////////////////////////////////////////////////////////
void MakeLegendEnabled(ads_name enInsert)
{
	struct resbuf *rbpLegend = acutBuildList(AcDb::kDxfRegAppName, _T("LEGEND"), AcDb::kDxfXdInteger16, 1, NULL);
	addXDataToEntity(enInsert, rbpLegend); 
	acutRelRb(rbpLegend);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : ChangeEntityProperties
// Arguments        : 1. Object ID of the entity whose properties are to be modified, as AcDbObjectId
//                    2. New color, as ACI integer     (defaults to 0,    i.e., no color change)
//                    3. New layer, as string          (defaults to NULL, i.e., no layer change)
//                    4. New linetype, as string       (defaults to NULL, i.e., no linetype change)
//                    5. New linetype scale, as double (defaults to 1.0,  i.e., no linetype scale change)
//
// Called from      : Anywhere
// Returns          : TRUE if all specified operations are successful, FALSE if not
// Description      : Changes the specified properties of the entity.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ChangeEntityProperties(AcDbObjectId objId, int iNewColor, const TCHAR *pszNewLayer, const TCHAR *pszNewLType, double dNewLTScale)
{
	// Get a pointer to the current database
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();

	// Open the entity for writing
	Acad::ErrorStatus es;
	AcDbEntity *pEnt; 
	if (acdbOpenObject(pEnt, objId, AcDb::kForWrite) != Acad::eOk) return FALSE; 
	
	// If the color specified is not 0, change the entity's color
	if (iNewColor > 0) pEnt->setColorIndex(iNewColor);

	// If the layer specified is not NULL, change the entity's layer
	if (pszNewLayer) 
	{
		// Open the layer table for reading
		AcDbLayerTable *pLayerTbl;
		if (pCurDb->getLayerTable(pLayerTbl, AcDb::kForRead) != Acad::eOk) { pEnt->close(); return FALSE; }

		// Check if the specified layer exists
		if (!pLayerTbl->has(pszNewLayer)) { pLayerTbl->close(); pEnt->close(); return FALSE; }

		// Get the object id of the specified layer from the layer table
		AcDbObjectId objLayer; if (pLayerTbl->getAt(pszNewLayer, objLayer) != Acad::eOk) { pLayerTbl->close(); pEnt->close(); return FALSE; }

		// Close the layer table
		pLayerTbl->close();

		// Change the entity's layer
		pEnt->setLayer(objLayer);
	}

	// If the linetype specified is not NULL, change the entity's linetype
	if (pszNewLType)
	{
		// Open the linetype table for reading
		AcDbLinetypeTable *pLTypeTbl;
		if (pCurDb->getLinetypeTable(pLTypeTbl, AcDb::kForRead) != Acad::eOk) { pEnt->close(); return FALSE; }

		// Check if the specified linetype exists
		if (!pLTypeTbl->has(pszNewLType)) 
		{ 
			// Close the linetype table, otherwise we cannot use the "loadLineTypeFile" function
			pLTypeTbl->close();

			// Try to load the linetype from the "ACAD.LIN" standard file
			if (pCurDb->loadLineTypeFile(pszNewLType, _T("ACAD.LIN")) != Acad::eOk) { pEnt->close(); return FALSE; }

			// Re-open the linetype table
			if (pCurDb->getLinetypeTable(pLTypeTbl, AcDb::kForRead) != Acad::eOk) { pEnt->close(); return FALSE; }
		}

		// Get the object id of the specified linetype from the linetype table
		AcDbObjectId objLType; if (pLTypeTbl->getAt(pszNewLType, objLType) != Acad::eOk) { pLTypeTbl->close(); pEnt->close(); return FALSE; }

		// Close the linetype table
		pLTypeTbl->close();

		// Change the entity's linetype
		pEnt->setLinetype(objLType);
	}

	// If the linetype scale specified is not 1.0, change the entity's linetype scale
	if (dNewLTScale != 1.0) pEnt->setLinetypeScale(dNewLTScale);

	// Close the entity pointer
	pEnt->close();

	// Return a success flag
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name    : ChangeProperties
// Arguments        : 1. Flag signifying whether a selection set or an entity is specified, as BOOL
//                    2. Entity name of the selection set or entity, as ads_name
//                    3. New color, as ACI integer     (defaults to 0,    i.e., no color change)
//                    4. New layer, as string          (defaults to NULL, i.e., no layer change)
//                    5. New linetype, as string       (defaults to NULL, i.e., no linetype change)
//                    6. New linetype scale, as double (defaults to 1.0,  i.e., no linetype scale change)
//
// Called from      : Anywhere
// Returns          : TRUE if all specified operations are successful, FALSE if not
// Description      : Depending on whether this is a selection set or a single entity, this will call the
//                    base function once or as many times as there are entities in the selection set.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ChangeProperties(BOOL bIsSSet, ads_name anEnOrSS, int iNewColor,const TCHAR *pszNewLayer, const TCHAR *pszNewLType, double dNewLTScale)
{
	// If this is a single entity...
	if (bIsSSet == FALSE)
	{
		// Get the object ID of the specified entity and call the base function on it
		AcDbObjectId objId; if (acdbGetObjectId(objId, anEnOrSS) != Acad::eOk) return FALSE;
		return ChangeEntityProperties(objId, iNewColor, pszNewLayer, pszNewLType, dNewLTScale);
	}
	// If this is a selection set
	else
	{
		// Get the length of the selection set
		Adesk::Int32 lLength; if (acedSSLength(anEnOrSS, &lLength) != RTNORM) { return FALSE; }

		// Get the name and object id of each entity in the selection set and call the base function on it
		ads_name enSSEnt;
		AcDbObjectId objId;
		for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
		{
			if (acedSSName(anEnOrSS, lCtr, enSSEnt) != RTNORM) { return FALSE; }
			if (acdbGetObjectId(objId, enSSEnt) != Acad::eOk)  { return FALSE; }
			if (!ChangeEntityProperties(objId, iNewColor, pszNewLayer, pszNewLType, dNewLTScale)) { return FALSE; }
		}
	}

	// Return a success flag
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function name: GetEntityLayerName()
// Description  : Gets the given entities layer name.
//////////////////////////////////////////////////////////////////////////
bool GetEntityLayerName(/*[IN]*/ AcDbObjectId objID, /*[OUT]*/ CString &csLayerName)
{
	// Get a pointer to the current database
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();

	// Open the entity for writing
	AcDbEntity *pEnt; if (acdbOpenObject(pEnt, objID, AcDb::kForRead) != Acad::eOk) return false;

	ACHAR *pszLayerName = pEnt->layer(); pEnt->close();
	csLayerName.Format(_T("%s"), pszLayerName); acutDelString(pszLayerName);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Function name: DisplayHelp()
// Description  : Displays the selected topic from the applications CHM file
//////////////////////////////////////////////////////////////////////////
bool DisplayHelp(DWORD topicID)
{
	// Show specific context
	HWND res = 0;
	LPCTSTR pszFile = g_csBaseFolder + _T("\\EE_Toolbox.chm");

	//////////////////////////////////////////////////////////////////////////
	// Show specific topic
	//////////////////////////////////////////////////////////////////////////
	//res = ::HtmlHelp(adsw_acadMainWnd(), pszFile, HH_DISPLAY_TOPIC, topicID);
	CTime tmStart(CTime::GetCurrentTime());
	if  (res == 0) 
	{
		//res = ::HtmlHelp(adsw_acadMainWnd(), pszFile, HH_CLOSE_ALL, topicID);
		return false;
	}
	return true;

	/*
	if (res == 0)
	{
		res = ::HtmlHelp(adsw_acadMainWnd(), pszFile, HH_DISPLAY_TOPIC, topicID);
		if (res == 0)
		{
			// Close the html help window
			// res = ::HtmlHelp(adsw_acadMainWnd(), pszFile, HH_CLOSE_ALL, topicID);

			// Grab that error
			HH_LAST_ERROR hherr;
			hherr.cbStruct = sizeof(HH_LAST_ERROR);
			HtmlHelp(adsw_acadMainWnd(), pszFile, HH_GET_LAST_ERROR, (DWORD) &hherr);
			if (FAILED(hherr.hr) && hherr.description != NULL)
			{
				// Convert the BSTR description to CString
				CString csDescription;
				LPSTR p = csDescription.GetBuffer(SysStringLen(hherr.description) + 1); 

				::WideCharToMultiByte(CP_ACP, 0, hherr.description,	-1, p, SysStringLen(hherr.description)+1, NULL,	NULL);
				
				// Display
				MessageBox(adsw_acadMainWnd(), csDescription, "Help Error", MB_OK);
				csDescription.ReleaseBuffer();

				// free the string after use
				::SysFreeString(hherr.description);
			}
		}
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
// Function name: getOrCreateSortEntsDictionary()
// Description  : Gets the AcDbSortEntsTable of the model space of current
//                drawing. If its not present, create a new one.
//////////////////////////////////////////////////////////////////////////
bool getOrCreateSortEntsDictionary(AcDbSortentsTable *&pSortTab)
{
	// Get the current working database
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();

	if (!pDb) return false;

	
	AcDbBlockTable *pBT	= NULL;
	AcDbBlockTableRecord *pBTR	= NULL;
	Acad::ErrorStatus es;

	// Get block Table
	es = pDb->getBlockTable(pBT,AcDb::kForRead);
	if (Acad::eOk != es) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); return false; }

	// Get model space block table record
	acDocManager->lockDocument(acDocManager->document(pDb));
	if (Acad::eOk != es) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); return false; }

	es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	pBT->close();
	if (Acad::eOk != es) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); return false; }
	
	// Get the AcDbSortEntsTable
	es = pBTR->getSortentsTable(pSortTab, AcDb::kForWrite, true);
	pBTR->close();
	if (Acad::eOk != es) { acutPrintf(_T("\nERROR @%d: %s"), __LINE__, acadErrorStatusText(es)); return false; }

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Get the maximum number in the array
//////////////////////////////////////////////////////////////////////////
int getMaxInArray (CUIntArray &uiaInt) 
{ 
	int iMaxInt = 0;
	for (int iCtr = 0; iCtr < uiaInt.GetSize(); iCtr++) { iMaxInt = (uiaInt[iCtr] > iMaxInt) ? uiaInt[iCtr] : iMaxInt; }
	return iMaxInt;
}

//////////////////////////////////////////////////////////////////////////
// Function name: GetInsertNumbers
// Description  : 
//////////////////////////////////////////////////////////////////////////
void GetInsertNumbers(CString csRegappName, CUIntArray &uiaInsertNumbers)
{
	uiaInsertNumbers.RemoveAll();

	// Collect all poles
	ads_name ssInserts;
	struct resbuf *rbpFilt = acutBuildList(67, 0, AcDb::kDxfRegAppName, csRegappName, NULL);
	if (acedSSGet(_T("X"), NULL, NULL, rbpFilt, ssInserts) != RTNORM) { acedSSFree(ssInserts); acutRelRb(rbpFilt); return; }
	acutRelRb(rbpFilt);

	// Get the XDATA for each and retrieve its insert number
	ads_name enInsert;
	struct resbuf *rbpXData = NULL;
	Adesk::Int32 lLength = 0L; acedSSLength(ssInserts, &lLength);
	for (Adesk::Int32 lCtr = 0L; lCtr < lLength; lCtr++)
	{
		acedSSName(ssInserts, lCtr, enInsert);
		if (!(rbpXData = getXDataFromEntity(enInsert, csRegappName))) continue;

		/**/ if (!csRegappName.CompareNoCase(XDATA_COLUMN))	
		{
			uiaInsertNumbers.Add(rbpXData->rbnext->resval.rint); 
		}
		else if (!csRegappName.CompareNoCase(XDATA_POLE)) 	{	uiaInsertNumbers.Add(rbpXData->rbnext->resval.rint); }
		else if (!csRegappName.CompareNoCase(XDATA_LIGHT))
		{
			uiaInsertNumbers.Add(rbpXData->rbnext->rbnext->rbnext->resval.rint); 

			//struct resbuf *rbpGet = acdbEntGet(enInsert); rbpGet = Assoc(rbpGet, 10);
			//acutPrintf("\n[L%d][%.2f,%.2f]", rbpXData->rbnext->rbnext->rbnext->resval.rint, rbpGet->resval.rpoint[X], rbpGet->resval.rpoint[Y]);
		}

		acutRelRb(rbpXData);
	}

	// acutPrintf("\nNumbers...."); for (int i = 0; i < uiaInsertNumbers.GetSize(); i++) acutPrintf("[%d]", uiaInsertNumbers[i]);

	acedSSFree(ssInserts);
}

//////////////////////////////////////////////////////////////////////////
// Function name: BuildFilterListForPole()
// Description  : Builds the filter list for selecting valid pole inserts.
//////////////////////////////////////////////////////////////////////////
struct resbuf *BuildFilterListForPole()
{
	// Make a selection set filter of all valid pole insert names
	struct resbuf *rbpFilter = NULL;

	if (g_csaPoleBlocks.GetSize() <= 0) return rbpFilter;
	if (g_csaPoleBlocks.GetSize() == 1)
	{
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), 2, g_csaPoleBlocks.GetAt(0), NULL);
	}
	else
	{
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), -4, _T("<OR"), NULL);

		struct resbuf *rbpTemp = NULL;
		for (int iCtr = 0; iCtr < g_csaPoleBlocks.GetSize(); iCtr++)
		{
			for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
			rbpTemp->rbnext = acutBuildList(2, g_csaPoleBlocks.GetAt(iCtr), NULL);
		}

		for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);
	}

	return rbpFilter;
}

//////////////////////////////////////////////////////////////////////////
// Function name: BuildFilterListForLight()
// Description  : Builds the filter list for selecting valid light inserts.
//////////////////////////////////////////////////////////////////////////
struct resbuf *BuildFilterListForLight()
{
	// Make a selection set filter of all valid pole insert names
	struct resbuf *rbpFilter = NULL;

	if (g_csaLanternBlocks.GetSize() <= 0) return rbpFilter;
	if (g_csaLanternBlocks.GetSize() == 1)
	{
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), 2, g_csaLanternBlocks.GetAt(0), NULL);
		//acutPrintf(L"\n1.[%s]", g_csaLanternBlocks.GetAt(0));
	}
	else
	{
		rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), -4, _T("<OR"), NULL);

		struct resbuf *rbpTemp = NULL;
		for (int iCtr = 0; iCtr < g_csaLanternBlocks.GetSize(); iCtr++)
		{
			//acutPrintf(L"\n2.[%s]", g_csaLanternBlocks.GetAt(iCtr));

			for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
			rbpTemp->rbnext = acutBuildList(2, g_csaLanternBlocks.GetAt(iCtr), NULL);
		}

		for (rbpTemp = rbpFilter; rbpTemp->rbnext; rbpTemp = rbpTemp->rbnext);
		rbpTemp->rbnext = acutBuildList(-4, _T("OR>"), NULL);


		//rbpFilter = acutBuildList(67, 0, RTDXF0, _T("INSERT"), -4, _T("<OR"),2,_T("SL_LANTERN_NEW") , -4, _T("OR>"),NULL);
	}

	return rbpFilter;
}

//////////////////////////////////////////////////////////////////////////
// Function		 : SplitString
// Description : Retrieves the substring between the specified delimiters.
// Author      : STL Split String By Paul J. Weiss. 
//////////////////////////////////////////////////////////////////////////
int SplitString(const CString& input, const CString& delimiter, CStringArray& results)
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength();
	int isize = input.GetLength();

	CArray<INT, int> positions;
	newPos = input.Find (delimiter, 0);
	if (newPos < 0) { return 0; }

	int numFound = 0;
	while (newPos > iPos)
	{
		numFound++;
		positions.Add(newPos);
		iPos = newPos;
		newPos = input.Find (delimiter, iPos+sizeS2+1);
	}

	for (int i = 0; i <= positions.GetSize(); i++)
	{
		CString s;
		if (i == 0)	s = input.Mid(i, positions[i]);
		else
		{
			int offset = positions[i-1] + sizeS2;
			if (offset < isize)
			{
				if (i == positions.GetSize())
					s = input.Mid(offset);
				else if ( i > 0 )
					s = input.Mid( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2);
			}
		}

		if (s.GetLength() > 0) results.Add(s);
	}

	return numFound;
}

//////////////////////////////////////////////////////////////////////////
// Selection sort used to sort an array of integers into ascending order
//////////////////////////////////////////////////////////////////////////
void swap ( long& x, long& y ) { long temp; temp = x; x = y;  y = temp; }
void selectionSort ( long arr[], long size )
{
	long indexOfMin;
	long pass;
	long j;

	for ( pass = 0; pass < size - 1; pass++ )
	{
		indexOfMin = pass;

		for ( j = pass + 1; j < size; j++ )
			if ( arr[j] < arr[pass] )
				indexOfMin = j;

		swap ( arr[pass], arr[indexOfMin] );
	}
}