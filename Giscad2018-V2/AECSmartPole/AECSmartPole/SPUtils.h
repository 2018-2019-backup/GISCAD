
// Generic MFC functions
void appMessage(CString csMessage, int iIcon);
void appError(CString csFileName, CString csFuncName, int iLine, CString csErrorMsg);
void displayMessage(LPTSTR fmt, ...);
int getConfirmation(CString csQuestion);
BOOL checkFileAccess(const TCHAR *szFilePath, int iMode);
BOOL setupDataSource(CString csDSN, CString csMDB);
CString suppressZero(double dValue);
BOOL CheckForDuplication(CStringArray& csaArray, CString csCheck);
BOOL CheckForDuplication(CStringArray& csaArray, CString csCheck, int& iIndex);
BOOL CheckForDuplication(CUIntArray& uiaArray, int iCheck);
int GetParameterValue(CStringArray &csaParameter, CString csParameter, CString &csValue, int iSearch);
int compare (const void * a, const void * b);

// AutoCAD SETVAR related functions
void switchOff();
void switchOn();
void saveOSMode();
void restoreOSMode();

// XData functions
void addXDataToEntity(ads_name enEnt, struct resbuf *rbpXData);
struct resbuf *getXDataFromEntity(ads_name enEnt, CString csAppName);

// XRecord functions
void addXRecordToDictionary(CString csDictName, CString csXrecName, struct resbuf *rbpXED);
struct resbuf *getXRecordFromDictionary(CString csDictName, CString csXrecName);
void deleteXRecord(CString csDictName, CString csXrecName);

// Layer functions
void setLayerColor(CString csLayerName, int iColorIndex);
void setLayer(const TCHAR *sLayerName, Adesk::Boolean iFreezeRest = Adesk::kTrue);
void createLayer(const TCHAR* sLayerName, Adesk::Boolean iFreezeRest = Adesk::kTrue, Adesk::Boolean doSetLayer = Adesk::kTrue);
BOOL purgeLayer(const TCHAR *sLayerName);
void toggleLayerDisplay(CStringArray &csaLayers);
void turnOnLayers(const TCHAR *sCurrentLayerName, CStringArray &csaLayers, CString csSubStr);
bool GetEntityLayerName(/*[IN]*/ AcDbObjectId objID, /*[OUT]*/ CString &csLayerName);

// Attribute functions
void changeAttribsLayer(AcDbObjectId objInsertId, const TCHAR *pDestLayer);
void rotateAttribs(ads_name enBlock, double dRotation);
int buildAttribsList(CString csSymName, CStringArray &csPrompts, CStringArray &csTags, CStringArray &csDefault);

// Block and entity functions
struct resbuf *Assoc(struct resbuf *rbpList, int iAssocNum);
AcDbObjectId appendEntityToDatabase(AcDbEntity *pEntity, BOOL bIsMSpace = TRUE);
BOOL ensureSymbolAvailability(CString csSymbol, AcDbObjectId& objBlock);
BOOL insertBlock(CString csBlockName, CString csLayerName, ads_point ptIns, double dScale, double dHeight, double dRotation, CString csTextStyle, AcDbObjectId& objInsert,  BOOL bIsMSpace = TRUE);
void ChangeAttributeLocation(AcDbObjectId &objInsertId, CString csAttribTag, CString csDirection = _T("NW"));
void ChangeAttributeValue(AcDbObjectId &objInsertId, CString csAttribTag, CString csAttValue);
void RotateAttribute(ads_name enBlock, double dRotation);
BOOL setLineType(CString csLType);
BOOL ChangeProperties(BOOL bIsSSet, ads_name anEnOrSS, int iNewColor,const TCHAR *pszNewLayer, const TCHAR *pszNewLType, double dNewLTScale);

// Others
void setOptionParameters();
void MakeLegendEnabled(ads_name enInsert);
bool DisplayHelp(DWORD topicID);
bool getOrCreateSortEntsDictionary(AcDbSortentsTable *&pSortTab);

// IE Toolbox specifics
int  getMaxInArray (CUIntArray &uiaInt);
void GetInsertNumbers(CString csRegappName, CUIntArray &uiaInsertNumbers);
struct resbuf *BuildFilterListForPole();
int SplitString(const CString& input, const CString& delimiter, CStringArray& results);
void selectionSort ( long arr[], long size );