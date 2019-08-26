/*!
@Class		CNSDWGMgr
@Brief		Provides functionality to open and to save a drawing
and to add new layer, new text style and text element.	
@Author	Neilsoft Ltd
@Date		14 June 2007
@History 	
*/
//test share
//--------------------------------------------------------
//#include "stdafx.h"
#include "afxwin.h"
#include "shlwapi.h"
#include "urlmon.h"
#include "Wininet.h"
#include "adesk.h"
#include "dbsymtb.h"
#include "dbents.h"
#include "dbelipse.h"
#include "dbspline.h"
#include "dblead.h"
#include "dbray.h"
#include "dbxline.h"
#include "dbmline.h"
#include "dbbody.h"
#include "dbregion.h"
#include "dbsol3d.h"
#include "acgi.h"
//#include "myacgi.h"
#include "acestext.h"
//#include "maps.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "rxregsvc.h"

//#include "dir.h"
#include "acdbxref.h"
#include "dbacis.h"

//**********************************

#include "windows.h"

#include "dbents.h"
#include "dbhatch.h"
#include "dbimage.h"
#include "imgdef.h"
#include "imgent.h"
#include "dbole.h"
#include "dbfcf.h"
#include "dbproxy.h"


#include "dbapserv.h"
#include "dbmstyle.h"
#include "gelnsg3d.h"
#include "tchar.h"

//--------------------------------------------------------
#include <math.h>
#include "tchar.h"
//#include "dbsymtb.h"
#include "dbsymutl.h"
#include "dbid.h" 
#include "dbapserv.h"
#include "dbmain.h"
#include "acdbabb.h"
#include "dblayout.h"
#include "AcDbLMgr.h"
#include "dbents.h"
#include "dbdict.h"
#include "dbMPolygon.h"
#include "dbidmap.h"
#include "summinfo.h"
#include "dbxrecrd.h"
#include "sstream"
#include <atlstr.h>
#include "dbdynblk.h"

#include "dbregion.h"
#include "dblead.h"
#include "dbmleader.h"
#include "dbmleaderstyle.h"
#include "acgiviewport.h"
#include "dbtable.h"
#include "dbsol3d.h"

#include "dbmstyle.h"
#include "acedads.h"

#include "acdb.h"

#define DEFAULT_TEXT_HEIGHT 1.8

// Multiline change request start
#include "dbmline.h"

#include <dbobjptr.h>

#ifndef _INTERNAL_
#include "dbDictUtil.h"
#endif
// Multiline change request end

#ifdef _INTERNAL_
#include "dbpl.h"
#endif

#include "rxregsvc.h"
#include "geplin3d.h"
#include "AcString.h"
#include "..\Include\NSDirectives.h"
#include "..\Include\NSDWGMgr.h"
#include "..\Include\NSLayer.h"
#include "..\Include\NSTextStyle.h"

#include "dbxutil.h"
#include <fstream>

typedef std::map<NSSTRING, NSSTRING> STRING_MAP;

/*!
@Brief			Default Constructor
@Date			14 June 2007
*/
CNSDWGMgr::CNSDWGMgr(void)
{
	m_pCurrentDatabase = NULL;
	m_dTextHeight = 2.5;
}

/*!
@Brief			Default Destructor
@Date			14 June 2007
*/
CNSDWGMgr::~CNSDWGMgr(void)
{
#ifndef _INTERNAL_
	if(m_pCurrentDatabase!=NULL)
	{
		delete m_pCurrentDatabase;
		m_pCurrentDatabase = NULL;
	}
#endif
}



class CreatentHostApp : public AcDbHostApplicationServices
{
	Acad::ErrorStatus findFile(TCHAR* p_szFullPathOut, int nBufferLength,
		const TCHAR* p_szFilename, AcDbDatabase* pDb = NULL,
		AcDbHostApplicationServices::FindFileHint = kDefault);

	// These two functions return the full path to the root folder where roamable/local 
	// customizable files were installed. Note that the user may have reconfigured 
	// the location of some the customizable files using the Options Dialog 
	// therefore these functions should not be used to locate customizable files. 
	// To locate customizable files either use the findFile function or the 
	// appropriate system variable for the given file type. 
	//
	Acad::ErrorStatus getRoamableRootFolder(const TCHAR*& folder);
	Acad::ErrorStatus getLocalRootFolder(const TCHAR*& folder);
	// make sure you implement getAlternateFontName. In case your findFile implementation
	// fails to find a font you should return a font name here that is guaranteed to exist.
	virtual TCHAR * getAlternateFontName() const
	{
		return _T("txt.shx"); //findFile will be called again with this name
	}
};



Acad::ErrorStatus CreatentHostApp::getRoamableRootFolder(const TCHAR*& folder)
{
	Acad::ErrorStatus ret = Acad::eOk;
	static TCHAR buf[_MAX_PATH] = _T("\0"); //MDI SAFE
	if (buf[0]==0)
		if (GetModuleFileName(NULL, buf, _MAX_PATH) != 0)
			ret = Acad::eRegistryAccessError;
	folder = buf;
	return ret;
}



Acad::ErrorStatus CreatentHostApp::getLocalRootFolder(const TCHAR*& folder)
{
	Acad::ErrorStatus ret = Acad::eOk;
	static TCHAR buf[MAX_PATH] = _T("\0"); //MDI SAFE
	if (buf[0]==0)
		if (GetModuleFileName(NULL, buf, MAX_PATH) != 0)
			ret = Acad::eRegistryAccessError;
	folder = buf;
	return ret;
}


Acad::ErrorStatus CreatentHostApp::findFile(TCHAR* p_szFullPathOut,
											int nBufferLength,
											const TCHAR* p_szFilename,
											AcDbDatabase* pDb,
											AcDbHostApplicationServices::FindFileHint hint)
{
	TCHAR pExtension[5];
	switch (hint)
	{
	case kCompiledShapeFile:
		NSSTRCPY(pExtension, _T(".shx"));
		break;
	case kTrueTypeFontFile:
		NSSTRCPY(pExtension, _T(".ttf"));
		break;
	case kPatternFile:
		NSSTRCPY(pExtension, _T(".pat"));
		break;
	case kARXApplication:
		NSSTRCPY(pExtension, _T(".dbx"));
		break;
	case kFontMapFile:
		NSSTRCPY(pExtension, _T(".fmp"));
		break;
	case kXRefDrawing:
		NSSTRCPY(pExtension, _T(".dwg"));
		break;
	case kFontFile:                // Fall through.  These could have
	case kEmbeddedImageFile:       // various extensions
	default:
		pExtension[0] = _T('\0');
		break;
	}
	TCHAR* p_szFilePart;
	DWORD result;
	result = SearchPath(NULL, p_szFilename, pExtension, nBufferLength, 
		p_szFullPathOut, &p_szFilePart);
	if (result && result < (DWORD)nBufferLength)
		return Acad::eOk;
	else
		return Acad::eFileNotFound;
}

void getupperString(TCHAR *pInput)
{
	NSSTRING strTemp = pInput;
#ifndef _INTERNAL_
	NSSTRUPR(pInput, strTemp.size()+1);
#else
	NSSTRUPR(pInput, strTemp.size()+1);
	/*NSSTRUPR(pInput);*/
#endif
}

bool checkStringSuffix(NSSTRING strOrginal, NSSTRING strSuffix)
{
	NSSTRING strTemp;
	if(strOrginal.size() >= strSuffix.size())
	{
		strTemp = strOrginal.substr((strOrginal.size() - strSuffix.size()), strOrginal.size());
	}

	if(NSSTRCMP(strTemp.c_str(), strSuffix.c_str()) != 0)
	{
		return false;
	}
	return true;
}

int findAndInsert(STRING_MAP &mapOfData, NSSTRING sKey, NSSTRING sValue)
{
	// Add position of first vertex and Area of a polyline to map of XDATA
	STRING_MAP::iterator theMapItr;
	theMapItr = mapOfData.find(sKey);
	if(theMapItr == mapOfData.end())
	{
		mapOfData.insert(std::map<NSSTRING, NSSTRING>::value_type(sKey, sValue));
	}
	else
	{
		(*theMapItr).second = sValue;
	}
	return NS_SUCCESS;
}

/*!
@Brief			Adds a new layer to layer table of current database
@Param [in]	TCHAR*	- Layer name		
@Return		int:	0 - NS_SUCCESS, 1 - NS_FAIL
@Date			14 June 2007
*/
int CNSDWGMgr::addNewLayer(TCHAR* pszLayerName, AcDbObjectId &layerId, bool bNonPlotting)
{
	AcDbLayerTable *pLayerTbl = NULL;
	if(m_pCurrentDatabase == NULL)
		return NS_FAIL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pLayerTbl, AcDb::kForWrite))
		return NS_FAIL;

	if (!pLayerTbl->has(pszLayerName))
	{
		AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
		layerId = pLayerTblRcd->id();

		if(Acad::eOk != pLayerTblRcd->setName(pszLayerName)) // set layer name
		{
			pLayerTbl->close();
			return NS_FAIL;
		}

		if(Acad::eOk != pLayerTblRcd->setIsFrozen(0))// layer to THAWED
		{
			pLayerTbl->close();
			return NS_FAIL;
		}

		pLayerTblRcd->setIsOff(0);   // layer to ON
		pLayerTblRcd->setVPDFLT(0);  // viewport default
		pLayerTblRcd->setIsLocked(0);// un-locked
		pLayerTblRcd->setIsPlottable(bNonPlotting);

		AcCmColor color;
		color.setColorIndex(1); // set color to red
		pLayerTblRcd->setColor(color);	

		// For linetype, we need to provide the object ID of
		// the linetype record for the linetype we want to
		// use. First, we need to get the object ID.
		AcDbLinetypeTable *pLinetypeTbl = NULL;
		AcDbObjectId ltId;
		if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pLinetypeTbl, AcDb::kForRead))
		{
			pLayerTbl->close();
			return NS_FAIL;
		}

		if((pLinetypeTbl->getAt(_T("DASHED"), ltId)) != Acad::eOk)
		{
			pLinetypeTbl->getAt(_T("CONTINUOUS"), ltId);
		}
		pLinetypeTbl->close();

		pLayerTblRcd->setLinetypeObjectId(ltId);		// set line type
		if(Acad::eOk != pLayerTbl->add(pLayerTblRcd))
		{
			pLayerTblRcd->close();
			pLayerTbl->close();
			return NS_FAIL;
		}

		pLayerTblRcd->close();
		pLayerTbl->close();

		if(Acad::eOk != AcDbSymbolUtilities::getLayerId(layerId, pszLayerName, m_pCurrentDatabase))
			return NS_FAIL;
	}
	else // layer of the same name already exists
	{
		pLayerTbl->close();
		return NS_FAIL;
	}
	return NS_SUCCESS;   
}

CreatentHostApp gCreatentHostApp;

/*!
@Brief			Opens existing drawing or creates a new drawing
@Param [in]		TCHAR*	- File name
@Param [in]		bool	- true: opens existing drawing, false: creates new drawing						
@Return			int	NS_SUCCESS/NS_FAIL
@Date			14 June 2007
*/
int CNSDWGMgr::openDWG(TCHAR* p_szFileName, bool bOpenExisting)
{
	Acad::ErrorStatus es = Acad::eOk;

#ifndef _INTERNAL_
	if(Acad::eOk != acdbSetHostApplicationServices(&gCreatentHostApp))
		return NS_FAIL;
	long lcid = 0x00000409; // English
	//if(Acad::eOk != acdbValidateSetup(lcid))
	//	return NS_FAIL;

#endif

	if(bOpenExisting)// Here false means, the database object is completely empty.
		m_pCurrentDatabase = new AcDbDatabase(Adesk::kFalse);
	else// true means, the database object contains the minimum necessary for a complete database.
		m_pCurrentDatabase = new AcDbDatabase();
	if(m_pCurrentDatabase != NULL)
	{
		if(bOpenExisting)  //Open existing database
		{
			es = m_pCurrentDatabase->readDwgFile(p_szFileName);
			//es = m_pCurrentDatabase->readDwgFile(p_szFileName, _SH_DENYNO);
			//es = m_pCurrentDatabase->closeInput(false);
		}
	}
	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}

void CNSDWGMgr::getSourceDatabase(AcDbDatabase* db)
{
	db=m_pCurrentDatabase;
}


/*
@Brief			Saves a drawing depending upon a file format
@Param [in]		TCHAR*	File name
@Param [in]		bool	drawing format true: bAs2004, false: bAs2007
@Return			int		NS_SUCCESS/NS_FAIL
@Date			14 June 2007
*/
int CNSDWGMgr::saveCurrentDWG(TCHAR* p_szFileName, bool bAs2004, NSSTRING saveFormat)
{
	Acad::ErrorStatus st = Acad::eOk;
#ifndef _INTERNAL_
	if(m_pCurrentDatabase != NULL)
	{
		if(bAs2004 || NSSTRCMP(saveFormat.c_str(), _T("AutoCAD 2006")) == 0)
		{
			//st = acdbSaveAs2004(m_pCurrentDatabase, p_szFileName);
			st = m_pCurrentDatabase->save();
		}
		else
		{
			//st = m_pCurrentDatabase->save();
			st = m_pCurrentDatabase->saveAs(p_szFileName,FALSE, AcDb::kDHL_CURRENT);
		}
	}
	if(st == Acad::eFileSharingViolation) return NS_SHARING_VIOLATION;
	return ((st == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
#else
	if(m_pCurrentDatabase != NULL)
	{
		st = m_pCurrentDatabase->save();
	}
	if(st == Acad::eFileSharingViolation) return NS_SHARING_VIOLATION;
	return ((st == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
#endif
}

/*!
@Brief			Adds a new Text Style to Text style table
@Param [in]		TCHAR* - Style name
@Param [in]		TCHAR* - Font name
@Return			int 0 - success, 1 - fail
@Date			14 June 2007
*/
int CNSDWGMgr::addNewTextStyle(TCHAR* p_szStyleName, TCHAR* p_szFontName)
{
	AcDbObjectId idTextStyle = NULL;
	AcDbTextStyleTable *pTable = NULL; 
	AcDbTextStyleTableRecord *pTableRecord = NULL; 

	if(Acad::eOk != m_pCurrentDatabase->getTextStyleTable(pTable, AcDb::kForWrite))
		return NS_FAIL;

	if(Acad::eOk != pTable->getAt(ACDB_MODEL_SPACE,pTableRecord, AcDb::kForWrite))
	{
		pTable->close();
		return NS_FAIL;
	}

	pTableRecord = new AcDbTextStyleTableRecord;
	if(Acad::eOk != pTableRecord->setName(p_szStyleName))	//Set font style name
	{
		pTableRecord->close();
		pTable->close();
		return NS_FAIL;
	}

	if(Acad::eOk != pTableRecord->setFont(p_szFontName,FALSE,FALSE,200,100)) // Set font name
	{
		pTableRecord->close();
		pTable->close();
		return NS_FAIL;
	}

	if(Acad::eOk != pTableRecord->setTextSize(3.0))			// Set text size
	{
		pTableRecord->close();
		pTable->close();
		return NS_FAIL;
	}

	if(Acad::eOk != pTable->add(idTextStyle,pTableRecord))	// add tabkerecord to table
	{
		pTableRecord->close();
		pTable->close();
		return NS_FAIL;
	}

	pTableRecord->close(); 
	pTable->close();
	return NS_SUCCESS;
}



/*!
@Brief			Adds a Text Element
@Param [in]		TCHAR* Text to be written
@Param [in]		x coordinate
@Param [in]		y coordinate
@Param [in]		z coordinate
@Return			int: 0 - NS_SUCCESS, 1 - NS_FAIL
@Date			14 June 2007
*/

int CNSDWGMgr::addTextElement(TCHAR* p_szText, double dXX, double dYY, double dZZ, NSSTRING strLayerName) {
	AcGePoint3d point(dXX,dYY,dZZ);
	//AcDbText* objText = new AcDbText(point,_T("Hrishi"));

	AcDbMText* objText = new AcDbMText();
	objText->setLocation(point);
	objText->setContents(p_szText);
	AcCmColor col;
	col.setColorIndex(1);
	objText->setColor(col);
	objText->setHeight(0.4);
	objText->setLayer(strLayerName.c_str());	

	if(objText != NULL && m_pCurrentDatabase != NULL)
	{
		AcDbBlockTable *pBlockTable = NULL;
		AcDbBlockTableRecord *pBlockTableRecord = NULL;

		//Get Symbol table from current database
		if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
		{
			objText->close();		
			return NS_FAIL;
		}

		//Get block table record from Model Space
		if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord, AcDb::kForWrite))
		{
			objText->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		//Add Text entitiy to this table record
		if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(objText))
		{
			objText->close();
			pBlockTableRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}
		objText->close();
		pBlockTableRecord->close();
		pBlockTable->close();
	}
	return NS_SUCCESS;
}

int CNSDWGMgr::ReleaseHostDwg()
{
	acdbReleaseHostDwg(m_pCurrentDatabase);	
	return NS_SUCCESS;
}

/*!
@Brief			Close and cleanup the opened database
@Param [in]	None
@Return		int: 0 - NS_SUCCESS, 1 - NS_FAIL
@Date			14 June 2007
*/
int CNSDWGMgr::closeDatabase()
{

	//**//SUBIR  - 10.02.2010 
	//Clean the database
	Acad::ErrorStatus st = Acad::eOk;
#ifndef _INTERNAL_ // in external validation	
	try
	{	  
		/*#ifndef _DEBUG
		st = acdbCleanUp();
		#endif*/	

		//if(m_pCurrentDatabase != NULL)
		//delete m_pCurrentDatabase;

		m_pCurrentDatabase = NULL;
	}
	catch(...)
	{

	}
#else // in internal validation	

	if(m_pCurrentDatabase != NULL)
		delete m_pCurrentDatabase;

	m_pCurrentDatabase = NULL;

#endif
	//**//SUBIR  - 10.02.2010 


#ifndef _INTERNAL_
	return ((st == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
#else
	return NS_SUCCESS;
#endif
}




/*
@Brief			Retrieves Object Id of the given blockname
@Param(out)		AcDbObjectId	NULL is returned if no Object of the given name is found
@Param(in)		const TCHAR*	Block Name
@Return			int				NS_SUCCESS - if block found , NS_FAIL - if no block is found of given name
*/
int CNSDWGMgr::getBlock(const TCHAR* p_szBlkname, AcDbObjectId& blockId)
{
	blockId = AcDbObjectId::kNull;

	AcDbBlockTable *pBlockTable = NULL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead)) //get block table
		return NS_FAIL;
	AcDbBlockTableRecord* pBlkTableRecord = NULL;
	if(Acad::eOk != pBlockTable->getAt(p_szBlkname, pBlkTableRecord, AcDb::kForRead)) // get block table record of the given blockname
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	blockId = pBlkTableRecord->objectId();    // get object Id of the retrieved block table record. It will be returned.

	pBlkTableRecord->close();
	pBlockTable->close();

	return ((blockId != AcDbObjectId::kNull) ? NS_SUCCESS : NS_FAIL);
}



/*
@Brief			Inserts a block with given attributes
@Param(in)		CNSBlock*	Block which contains block name, insertion point, rotation and attribute list
@Param(in)		TCHAR*		Layer name
@Param(in)		std::map<NSSTRING, CNSAttributeRef*>	Attributes
@Param(in)		std::map<NSSTRING, NSSTRING>			XDATA
@Return			int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::insertBlockWithAttributes(CNSBlock* blkBlock, TCHAR* pszLayerName, ATT_MAP &mapOfBlkAtt, STRING_MAP &mapOfRelationships)
{
	int nError = 0;
	bool bNewLayerAdded = false;

	// Get Object ID of the Block Definition to be insserted
	AcDbObjectId blockDefId = NULL;
	if(NS_SUCCESS != getBlock(blkBlock->getBlockName().c_str(), blockDefId))
	{
		// Block definition of this name is not found so insert Ghost block
		//insertGhostBlock(blkBlock, blkAttMap);
		if(NSSTRCMP(blkBlock->getBlockName().c_str(),_T("ERR_CIRCLE")) == 0)
			return NS_ERR_CIRCLE_NOT_FOUND;

		if(NS_SUCCESS != getBlock(_T("Ghost"), blockDefId))
		{
			return NS_GHOST_BLOCK_NOT_FOUND;
		}
		nError = NS_ERR_GHOST_BLOCK;
		pszLayerName = _T("UNKNOWN_FEATURES");
	}

	// Get layer ID of the layer on which the block is to be inserted
	AcDbObjectId LayerId = NULL;
	Acad::ErrorStatus es = AcDbSymbolUtilities::getLayerId(LayerId, pszLayerName, m_pCurrentDatabase);
	//if(es != Acad::eOk || nError == NS_ERR_GHOST_BLOCK)
	if(es != Acad::eOk)
	{
		// If layer is not present create new layer
		int nResult = NS_FAIL;
		if(NSSTRCMP(pszLayerName,_T("GIS_ERRORS")) == 0)
			nResult = addNewLayer(pszLayerName, LayerId, true);
		else
			nResult = addNewLayer(pszLayerName, LayerId);
		if(nResult == NS_FAIL)
			return nResult;
		else
			bNewLayerAdded = true;
	}	

	// Set up the block reference to block definition retrieved through getBlock()
	AcDbBlockReference *pBlkRef = new AcDbBlockReference;
	if(Acad::eOk != pBlkRef->setBlockTableRecord(blockDefId))
	{
		return NS_FAIL;
	}

	// Set Block Position, rotation angle and layer
	CNSPoint3D pt;
	blkBlock->getInsertionpoint(pt);
	AcGePoint3d ptBase = AcGePoint3d(pt.m_dXX, pt.m_dYY, 0.00);		
	if(Acad::eOk != pBlkRef->setPosition(ptBase)
		|| (Acad::eOk != pBlkRef->setRotation(blkBlock->getRotation()))
		|| (Acad::eOk != pBlkRef->setLayer(LayerId)))
	{
		pBlkRef->close();
		return NS_FAIL;
	}

	// Open the current database's model space block table record.
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))
	{
		pBlkRef->close();
		return NS_FAIL;
	}

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite))
	{
		pBlkRef->close();
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();

	// Append the block reference to the model space block table record.
	AcDbObjectId newEntId;
	if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(newEntId, pBlkRef))
		return NS_FAIL;

	pBlockTableRecord->close();

	// Now set attributes to the inserted block
	// Open the block definition for read.
	AcDbBlockTableRecord *pBlockDef = NULL;
	acdbOpenObject(pBlockDef, blockDefId, AcDb::kForRead);

	// Set up a block table record iterator to iterate over the attribute definitions.
	AcDbBlockTableRecordIterator *pAttDefIterator;
	pBlockDef->newIterator(pAttDefIterator);
	AcDbEntity *pEnt;
	AcDbAttributeDefinition *pAttdef;
	std::map<NSSTRING, NSSTRING>::iterator itrStringMap;
	ATT_MAP::iterator itrMap;

	CNSAttributeRefMap *pAttrRefMap = NULL;

	for (pAttDefIterator->start(); !pAttDefIterator->done(); pAttDefIterator->step())
	{
		CNSAttributeRef *pAttrRef = NULL;
		NSSTRING strValue = _T("");
		NSSTRING strAttrValue;

		// Get the next entity.
		pAttDefIterator->getEntity(pEnt, AcDb::kForRead);

		// Make sure the entity is an attribute definition and not a constant.
		pAttdef = AcDbAttributeDefinition::cast(pEnt);

		if (pAttdef != NULL && !pAttdef->isConstant())
		{
			// We have a non-constant attribute definition, so build an attribute entity.
			AcDbAttribute *pAtt = new AcDbAttribute();
			AcGePoint3d basePoint = pAttdef->position();
			basePoint += pBlkRef->position().asVector();
			TCHAR* pszTag = pAttdef->tag();
			NSSTRING strTag(pszTag);
			getupperString(pszTag);
			itrMap = mapOfBlkAtt.find(strTag);
			CNSCompositeGeometry *pCompGeo = NULL;
			//  Read Z attribute value from Block and convert string to double
			if(NSSTRCMP(pszTag, _T("Z")) == 0)
			{
				char szDouble[128];
				wchar_t sz[128];
				::sprintf(szDouble,"%lf",pt.m_dZZ);
				MultiByteToWideChar(CP_ACP,	0,	szDouble ,-1,sz,sizeof(sz));
				strValue = sz;
			}
			//------------------------------------------

			if(itrMap != mapOfBlkAtt.end())
			{ 
				pAttrRefMap = (*itrMap).second;
				if(pAttrRefMap)
				{
					//----Add FB_REFERENCE attribute in comma separated format if multiple---
					if(NSSTRCMP(pszTag, _T("FB_REFERENCE")) == 0 &&
						pAttrRefMap->getSize() > 1)
					{
						int RefPos = 1;
						while(pAttrRefMap->getAttrRef(RefPos) != NULL)
						{
							pAttrRef =  pAttrRefMap->getAttrRef(RefPos);
							if(pAttrRef)
							{
								if(strValue != _T(""))
									strValue.append(_T(", "));
								strValue = strValue.append(pAttrRef->getAttributeValue().c_str());
								pCompGeo = pAttrRef->getCompGeometry();
							}
							RefPos++;
						}
					}
					//-----------------------------------------------
					else
					{
						pAttrRef =  pAttrRefMap->getAttrRef(1);
						if(pAttrRef)
						{
							strValue = pAttrRef->getAttributeValue().c_str();
							pCompGeo = pAttrRef->getCompGeometry();
						}
					}
				}
			}

			/*itrMap = mapOfBlkAtt.find(strTag);
			CNSCompositeGeometry *pCompGeo = NULL;
			if(itrMap != mapOfBlkAtt.end())
			{ 
			pAttrRef = (*itrMap).second;
			strValue = pAttrRef->getAttributeValue().c_str();
			pCompGeo = pAttrRef->getCompGeometry();
			}*/

			double dRotation = 0;
			if(pCompGeo != NULL)
			{
				dRotation = ((CNSBlock*)pCompGeo->getAt(0))->getRotation();
			}
			else
			{
				dRotation = pAttdef->rotation();
			}

			// retrieve the value of the block attribute from map of attributes
			if(nError != NS_ERR_GHOST_BLOCK)
			{
				//	itrMap = mapOfBlkAtt.find(strTag);
				//	if(itrMap != mapOfBlkAtt.end())
				if(pAttrRef && itrMap == mapOfBlkAtt.end())
				{ 
					strValue = pAttrRef->getAttributeValue();
				}
			}
			else // It is a Ghost block. The attribute values are stored in mapOfRelationships
			{
				//??????
				itrStringMap = mapOfRelationships.find(strTag);
				if(itrStringMap != mapOfRelationships.end())
				{ 
					strValue = (*itrStringMap).second;
				}
			}
			if(pAttdef->isPreset() && NSSTRCMP(strValue.c_str(), _T("")) == 0)
			{
				strValue = pAttdef->textString();
			}

			// Set Attribute properties and values
			if(Acad::eOk != pAtt->setPropertiesFrom(pAttdef)
				|| Acad::eOk != pAtt->setPosition(basePoint)
				|| Acad::eOk != pAtt->setTextStyle(pAttdef->textStyle())
				|| Acad::eOk != pAtt->setHeight(pAttdef->height())
				|| Acad::eOk != pAtt->setRotation(dRotation)
				|| Acad::eOk != pAtt->setTag(pszTag)
				|| Acad::eOk != pAtt->setFieldLength(pAttdef->fieldLength())
				|| Acad::eOk != pAtt->setTextString(strValue.c_str())
				|| Acad::eOk != pAtt->setInvisible(pAttdef->isInvisible()))
			{
				pAtt->close();
				delete pAttDefIterator;
				pBlockDef->close();
				pBlkRef->close();
				return NS_FAIL;
			}

			if(pCompGeo != NULL)
			{
				setAlignment(pAtt, ((CNSBlock*)pCompGeo->getAt(0))->getAlignment());
				pAtt->adjustAlignment(m_pCurrentDatabase);
			}

			pAtt->setAlignmentPoint(basePoint);

			// Append Attribute
			AcDbObjectId attId;
			if(Acad::eOk != pBlkRef->appendAttribute(attId, pAtt))
			{
				pAtt->close();
				delete pAttDefIterator;
				pBlockDef->close();
				pBlkRef->close();
				return NS_FAIL;
			}

			AcDbObjectIdArray ObjIDs;
			ObjIDs.append(pAtt->objectId());
			pAtt->close();

#ifndef _INTERNAL_
			acdbForceTextAdjust(ObjIDs);
#endif

		}

		if(pEnt != NULL)
			pEnt->close(); // use pEnt... pAttdef might be NULL
	}

	double dRotation = 0;
	if(blkBlock != NULL)
	{
		pBlkRef->setRotation(blkBlock->getRotation());
	}

	delete pAttDefIterator;
	pBlockDef->close();
	pBlkRef->close();

	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%lf %lf "), (ptBase.x), (ptBase.y));
	//mapOfRelationships.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("InsertionPoint"), szFormat));
	findAndInsert(mapOfRelationships, _T("InsertionPoint"), szFormat);

	// Add Xdata if it is not a ghost block
	//if(nError != NS_ERR_GHOST_BLOCK)
	{
		STRING_MAP::iterator XDataItr;
		for(XDataItr = mapOfRelationships.begin(); XDataItr != mapOfRelationships.end(); XDataItr++)
		{
			addXDATA(newEntId, (*XDataItr).first, (*XDataItr).second); 
		}
	}

	if(nError == NS_ERR_GHOST_BLOCK)
	{
		return nError;
	}
	if(bNewLayerAdded == true)
		return NS_NEW_LAYER_ADDED;
	return NS_SUCCESS;
}



/*
@Brief			Converts a string to vector of 3Dpoints of which z coordinate is 0.0
@Param(in)		NSSTRING	conatais 2D polyline vertices e.g. "0.0 0.0 4.0 2.0" Here (0.0, 0.0) is one point and (4.0, 2.0) is an another point
@Param(in)		std::vector<CNSPoint3D>& stores polyline vertices as CNSPoint3Ds
@Return			int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::convertString2Vertices(NSSTRING strPolyLineVertices, std::vector<CNSPoint3D>& vctPolyLineVertices)
{
	TCHAR* pszTemp = NULL;
	TCHAR* pszSep = _T(" ");
	CNSPoint3D ptTemp;
	int nCordinateCounter = 1;

	// Establish string and get the first token (coordinate):
	TCHAR* pszNextToken;   //dummy 
	//pszTemp = wcstok_s ((TCHAR*)strPolyLineVertices.c_str(), pszSep, &pszNextToken);
#ifdef _INTERNAL_
	//pszTemp = NSSTRTOK ((TCHAR*)strPolyLineVertices.c_str(), pszSep);
	pszTemp = NSSTRTOK ((TCHAR*)strPolyLineVertices.c_str(), pszSep, &pszNextToken);
#else
	pszTemp = NSSTRTOK ((TCHAR*)strPolyLineVertices.c_str(), pszSep, &pszNextToken);
#endif

	// While there are tokens in "string"
	while( pszTemp != NULL )
	{
		int reminderVal = nCordinateCounter%3;

		switch(reminderVal)
		{
		case 1 : 
			ptTemp.m_dXX = NSATOF(pszTemp);
			nCordinateCounter++;
			break;
		case 2 : 
			ptTemp.m_dYY = NSATOF(pszTemp);
			nCordinateCounter++;
			break;
		case 0 : 
			ptTemp.m_dZZ = NSATOF(pszTemp);
			nCordinateCounter++;
			vctPolyLineVertices.push_back(ptTemp);
			break;
		}      

		// Get next token: (coordinate)
#ifdef _INTERNAL_
		//pszTemp = NSSTRTOK( NULL, pszSep);
		pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
#else
		pszTemp = NSSTRTOK( NULL, pszSep, &pszNextToken);
#endif
	}

	// If total number coordinates is not even
	if(nCordinateCounter%2 != 0)
		return NS_FAIL;

	return NS_SUCCESS;
}


int CNSDWGMgr::getBlockXData(CNSCompositeGeometry *pCompositeGeo, STRING_MAP &mapOfXData)
{
	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCNO());
	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CNO"), szFormat));

	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCID());
	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CID"), szFormat));

	return NS_SUCCESS;
}

int CNSDWGMgr::addAttributeText(ATT_MAP& mapOfBlkAtt, NSSTRING strLayerName)
{
	ATT_MAP::iterator AttrIter;
	ATTREF_MAP::iterator AttrRefItr;
	CNSAttributeRefMap* pRefMap = NULL;
	CNSAttributeRef* pAttrRef = NULL;
	int nSize = 0;
	for(AttrIter = mapOfBlkAtt.begin(); AttrIter != mapOfBlkAtt.end(); AttrIter++)
	{
		pRefMap = (*AttrIter).second;
		if(pRefMap == NULL) continue;
		ATTREF_MAP mapOfAttrRef;
		pRefMap->getAttrReferences(mapOfAttrRef);
		for(AttrRefItr = mapOfAttrRef.begin(); AttrRefItr != mapOfAttrRef.end(); AttrRefItr++)
		{
			pAttrRef = (*AttrRefItr).second;
			if(pAttrRef == NULL) continue;
			CNSCompositeGeometry* pCompGeometry =  pAttrRef->getCompGeometry();
			if(pCompGeometry != NULL)
			{
				CNSBlock *pBlock = (CNSBlock*)pCompGeometry->getAt(0);
				if(pBlock)
				{
					CNSPoint3D pt;
					pBlock->getInsertionpoint(pt);
					NSSTRING strText = pAttrRef->getAttributeValue();
					insertText(pBlock, strText, strLayerName);
					//addTextElement((TCHAR*)pRef->getAttributeValue().c_str(), pt.m_dXX, pt.m_dYY, pt.m_dZZ);
				}
			}
		}
	}
	//ATT_MAP::iterator AttrIter;
	//CNSAttributeRefMap* pRefMap = NULL;
	//CNSAttributeRef* pAttrRef = NULL;
	//int nSize = 0;
	//for(AttrIter = mapOfBlkAtt.begin(); AttrIter != mapOfBlkAtt.end(); AttrIter++)
	//{
	//	pRefMap = (*AttrIter).second;
	//	if(pRefMap == NULL) continue;
	//	nSize = pRefMap->getSize();
	//	for(int nRefCntr = 0; nRefCntr < nSize; nRefCntr++)
	//	{
	//		pAttrRef = pRefMap->getAt(nRefCntr);
	//		if(pAttrRef == NULL) continue;
	//		CNSCompositeGeometry* pCompGeometry =  pAttrRef->getCompGeometry();
	//		if(pCompGeometry != NULL)
	//		{
	//			CNSBlock *pBlock = (CNSBlock*)pCompGeometry->getAt(0);
	//			if(pBlock)
	//			{
	//				CNSPoint3D pt;
	//				pBlock->getInsertionpoint(pt);
	//				NSSTRING strText = pAttrRef->getAttributeValue();
	//				insertText(pBlock, strText, strLayerName);
	//				//addTextElement((TCHAR*)pRef->getAttributeValue().c_str(), pt.m_dXX, pt.m_dYY, pt.m_dZZ);
	//			}
	//		}
	//	
	//	}
	//CNSCompositeGeometry* pCompGeometry =  pRef->getCompGeometry();
	//if(pCompGeometry != NULL)
	//{
	//	CNSBlock *pBlock = (CNSBlock*)pCompGeometry->getAt(0);
	//	if(pBlock)
	//	{
	//		CNSPoint3D pt;
	//		pBlock->getInsertionpoint(pt);
	//		NSSTRING strText = pRef->getAttributeValue();
	//		insertText(pBlock, strText, strLayerName);
	//		//addTextElement((TCHAR*)pRef->getAttributeValue().c_str(), pt.m_dXX, pt.m_dYY, pt.m_dZZ);
	//	}
	//}
	//}
	return NS_SUCCESS;
}

int CNSDWGMgr::getPrimaryCompIndex(CNSCompositeContainer *pContainer)
{
	if(pContainer == NULL) return 0;
	CNSCompositeGeometry *pCompositeGeo = NULL;
	int nCompSize = pContainer->getSize();
	for(int nCompCntr = 0; nCompCntr < nCompSize; nCompCntr++)
	{
		pCompositeGeo = pContainer->getAt(nCompCntr);
		if(pCompositeGeo == NULL) continue;
		COMPONENT_TYPE enCompType = pCompositeGeo->getCompType();
		if(enCompType == COMP_POINT)
		{
			if(pCompositeGeo->getCompRepresentation() == 1)
				return nCompCntr;
		}
	}
	return 0;
}

int CNSDWGMgr::createCompositeGeometry(CNSFeatureContainer* pFeatureContainer)
{
	if(pFeatureContainer == NULL) return NS_FAIL;
	CNSCompositeContainer *container = NULL;
	container = pFeatureContainer->getComponentContainer();
	STRING_MAP *mapXData;//, mapAttributes;
	NSSTRING strLayerName = pFeatureContainer->getLayerName();
	NSSTRING strTypeName = pFeatureContainer->getTypeName();
	mapXData = pFeatureContainer->getXData();
	//pFeatureContainer->getBlockAttributes(mapAttributes);

	CNSCompositeGeometry *pCompositeGeo = NULL;
	int nCompSize = container->getSize();
	bool bTextAdded = false; 
	bool bByLayerSet = false;
	COMPONENT_TYPE enCompType = COMP_INVALID;
	//int nIndex = getPrimaryCompIndex(container);
	for(int nCompCntr = 0; nCompCntr < nCompSize; nCompCntr++)
	{
		pCompositeGeo = container->getAt(nCompCntr);
		if(pCompositeGeo == NULL) continue;
		int nSize = pCompositeGeo->getPoslistSize();

		enCompType = pCompositeGeo->getCompType();

		if(enCompType != COMP_POINT /*&& (pCompositeGeo->getCompType() != COMP_TEXTPOINT)*/ )
		{
			if(enCompType != COMP_TEXTPOINT)
			{
				/*	if(enCompType == COMP_BOUNDARY)
				{
				drawMPolygon(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), *mapXData);
				}
				else*/
				{
					int nRet;
					// Multiline change request start
					if(enCompType == COMP_COMPOSITE_MLINE)
						nRet = drawMultiLine(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), false, *mapXData);
					else
						// Multiline change request end
						nRet = drawPolyLine(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), false, *mapXData);
					if(NS_FAIL == nRet)
					{
						return NS_FAIL;
					}
					else if(nRet == NS_BYLAYER_SET)
					{
						bByLayerSet = true;
					}
				}
			}
			if(bTextAdded == false)
			{
				addAttributeText(*(pFeatureContainer->m_mapOfBlkAtt), strLayerName);
				bTextAdded = true;
			}		
		}
		else	
		{
			pCompositeGeo = container->getAt(nCompCntr);
			if(pCompositeGeo == NULL) continue;
			int nPosSize = pCompositeGeo->getPoslistSize();
			if(nPosSize > 0)
			{
				CNSBlock* pBlk = (CNSBlock*)pCompositeGeo->getAt(0);
				if(pBlk == NULL) continue;
				if(NSSTRCMP((TCHAR*)strTypeName.c_str() , _T("")) == 0) continue;
				getBlockXData(pCompositeGeo, *mapXData);
				pBlk->setBlockName(strTypeName.c_str());
				return insertBlockWithAttributes(pBlk, (TCHAR*)strLayerName.c_str(), *(pFeatureContainer->m_mapOfBlkAtt), *mapXData);
			}
		}

		//NSSTRING strPos;
		//std::vector<CNSPoint3D> vPoint3d;
		//bool bPointfound = false;
		//for(int nGeoCntr = 0; nGeoCntr < nSize; nGeoCntr++)
		//{
		//	pCompositeGeo->getAt(nGeoCntr, strPos, enElement);
		//	if(enElement == ELE_BLK_POINT || enElement == ELE_TXT_POINT) 
		//	{
		//		bPointfound = true; continue;
		//	}
		//	//convertString2Vertices(strPos, vPoint3d);
		//}
		//if(bPointfound == false)
		//	//drawPolyLine(vPoint3d, szLineType, szLayerName, false, mapOfXData);
		//	drawPolyLine(pCompositeGeo, szLineType, szLayerName, false, mapOfXData);
	}
	if(bByLayerSet == true) return NS_BYLAYER_SET;
	return NS_SUCCESS;
}

int CNSDWGMgr::createCompositeGeometryEx(CNSFeatureContainer* pFeatureContainer)
{
	if(pFeatureContainer == NULL) return NS_FAIL;
	CNSCompositeContainer *container = NULL;
	container = pFeatureContainer->getComponentContainer();
	STRING_MAP *mapXData;//, mapAttributes;
	NSSTRING strLayerName = pFeatureContainer->getLayerName();
	NSSTRING strTypeName = pFeatureContainer->getTypeName();
	mapXData = pFeatureContainer->getXData();
	//pFeatureContainer->getBlockAttributes(mapAttributes);

	CNSCompositeGeometry *pCompositeGeo = NULL;
	int nCompSize = container->getSize();
	bool bTextAdded = false; 
	bool bByLayerSet = false;
	COMPONENT_TYPE enCompType = COMP_INVALID;
	//int nIndex = getPrimaryCompIndex(container);
	for(int nCompCntr = 0; nCompCntr < nCompSize; nCompCntr++)
	{
		pCompositeGeo = container->getAt(nCompCntr);
		if(pCompositeGeo == NULL) continue;
		int nSize = pCompositeGeo->getPoslistSize();

		enCompType = pCompositeGeo->getCompType();

		if(enCompType != COMP_POINT /*&& (pCompositeGeo->getCompType() != COMP_TEXTPOINT)*/ )
		{
			if(enCompType != COMP_TEXTPOINT)
			{
				/*	if(enCompType == COMP_BOUNDARY)
				{
				drawMPolygon(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), *mapXData);
				}
				else*/
				{
					int nRet;
					// Multiline change request start
					if(enCompType == COMP_COMPOSITE_MLINE)
						nRet = drawMultiLine(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), false, *mapXData);
					else
						// Multiline change request end
						nRet = drawPolyLine(pCompositeGeo, (TCHAR*)strTypeName.c_str(), (TCHAR*)strLayerName.c_str(), false, *mapXData);
					if(NS_FAIL == nRet)
					{
						return NS_FAIL;
					}
					else if(nRet == NS_BYLAYER_SET)
					{
						bByLayerSet = true;
					}

					//mIndex++;
				}
			}
			if(bTextAdded == false)
			{
				addAttributeText(*(pFeatureContainer->m_mapOfBlkAtt), strLayerName);
				bTextAdded = true;				
			}	

		}
		else	
		{
			pCompositeGeo = container->getAt(nCompCntr);
			if(pCompositeGeo == NULL) continue;
			int nPosSize = pCompositeGeo->getPoslistSize();
			if(nPosSize > 0)
			{
				CNSBlock* pBlk = (CNSBlock*)pCompositeGeo->getAt(0);
				if(pBlk == NULL) continue;
				if(NSSTRCMP((TCHAR*)strTypeName.c_str() , _T("")) == 0) continue;
				getBlockXData(pCompositeGeo, *mapXData);
				pBlk->setBlockName(strTypeName.c_str());
				insertBlockWithAttributes(pBlk, (TCHAR*)strLayerName.c_str(), *(pFeatureContainer->m_mapOfBlkAtt), *mapXData);
			}
		}


	}
	if(bByLayerSet == true) return NS_BYLAYER_SET;
	return NS_SUCCESS;
}



/*
@Brief			Creates a polyline with the input vector of CNSPoint3Ds
@Param(in)		CNSCompositeGeometry*
@Param(in)		TCHAR*		LineType
@Param(in)		TCHAR*		LayerName
@Param(in)		bool		true: creates closed polygon	false: does not create a closed polygon implicitly
@Param(in)		STRING_MAP	xdata	
@Return		int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::drawPolyLine(CNSCompositeGeometry *pCompositeGeo, TCHAR* szLineType, TCHAR* szLayerName, bool bClosed, STRING_MAP &mapOfXData)
{
	CNSCompositeData* pCompositeData = NULL;
	AcDbPolyline *pPolyLine = new AcDbPolyline();
	std::vector<CNSPoint3D> tempVectPoints;
	CNSPoint3D tempVertex;
	double tempZ;
	int nGeoSize = pCompositeGeo->getPoslistSize();
	for(int nGeocntr = 0; nGeocntr<nGeoSize; nGeocntr++) 
	{
		pCompositeData = pCompositeGeo->getAt(nGeocntr);
		NSSTRING strVertexPoints;
		GEO_ELEMENT GL;
		pCompositeData->getData(strVertexPoints, GL);
		//Fetching Z value only for writing to Xdata
		convertString2Vertices(strVertexPoints, tempVectPoints);
		tempVertex = tempVectPoints.at(1);
		tempZ = tempVertex.m_dZZ;
		//--------------------------------------------------
		if(GL == ELE_LINEAR)
		{
			addPolyLineLinearSegment(pPolyLine, strVertexPoints);
		}
		if(GL == ELE_CUVRVE || GL == ELE_CURVE_MEMEBR)
		{
			addPolyLineArcSegment(pPolyLine, strVertexPoints);
		}
	}

	AcDbObjectId dashdotId;
	AcDbObjectId layerID;
	if(NSSTRCMP(szLineType, _T("")) == 0)
		szLineType = _T("ByLayer");
	if(NSSTRCMP(szLayerName, _T("")) == 0)
		szLayerName = _T("UNKNOWN_FEATURES");	// if szLayerName is empty put it on layer "UNKNOWN_FEATURES"

	getLinetypeIdFromString(szLineType, dashdotId);
	getLayerNameIdFromString(szLayerName, layerID);
	if(layerID == NULL)
	{
		int nResult = 0;
		if(NSSTRCMP(szLayerName,_T("GIS_ERRORS")) == 0)
			nResult = addNewLayer(szLayerName, layerID, true);
		else
			nResult = addNewLayer(szLayerName, layerID);

		// If layer is not present create it
		if(NS_FAIL == nResult)
			return NS_FAIL;
	}
	if(dashdotId == NULL)
	{
		getLinetypeIdFromString(_T("ByLayer"), dashdotId);
	}
	if(Acad::eOk != pPolyLine->setLinetype(dashdotId)
		||	Acad::eOk != pPolyLine->setLayer(layerID))
	{
		delete pPolyLine;
		return NS_FAIL;
	}

	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))
	{
		delete pPolyLine;
		return NS_FAIL;
	}

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite))
	{	
		delete pPolyLine;
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();

	// Append the pline object to the database and
	// obtain its object ID.
	AcDbObjectId plineObjId;
	if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(plineObjId, pPolyLine))
	{	
		delete pPolyLine;
		pBlockTableRecord->close();
		return NS_FAIL;
	}
	pBlockTableRecord->close();

	// Get string corresponding to first vertex of the polyline
	AcGePoint3d startPoint;
	pPolyLine->getStartPoint(startPoint);
	TCHAR szTempString[50] = _T("");
	NSSPRINTF(szTempString, _T("%f %f %f "), (startPoint.x), (startPoint.y), tempZ);
	NSSTRING strStartVertex(szTempString);

	// Get string corresponding to second last vertex of the polyline
	AcGePoint3d vertexPoint;
	int nVertices = pPolyLine->numVerts();
	pPolyLine->getPointAt(1, vertexPoint);
	TCHAR szTempString1[50] = _T("");
	NSSPRINTF(szTempString1, _T("%f %f %f"), (vertexPoint.x), (vertexPoint.y), tempZ);
	NSSTRING strPointVertex(szTempString1);

	// Get string corresponding to area of polygon
	double dArea;
	pPolyLine->getArea(dArea);
	NSSPRINTF(szTempString, _T("%f"), dArea);
	NSSTRING strArea(szTempString);

	if((pCompositeGeo->getCompType() == COMP_BOUNDARY)
		|| (pCompositeGeo->getCompType() == COMP_POLYGON)
		|| (pCompositeGeo->getCompType() == COMP_COMPOSITE_POLYGON))
	{
		pPolyLine->setClosed(Adesk::kTrue);
	}


	pPolyLine->close();

	// Add position of first vertex and Area of a polyline to map of XDATA
	findAndInsert(mapOfXData, _T("START_VERTEX"), strStartVertex);
	findAndInsert(mapOfXData, _T("NEXT_VERTEX"), strPointVertex);
	findAndInsert(mapOfXData, _T("NS_AREA"), strArea);

	/*  mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("START_VERTEX"), strStartVertex));
	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("NEXT_VERTEX"), strPointVertex));
	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("NS_AREA"), strArea.c_str()));*/

	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCNO());
	//	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CNO"), szFormat));
	findAndInsert(mapOfXData, _T("CNO"), szFormat);


	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCID());
	//	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CID"), szFormat));
	findAndInsert(mapOfXData, _T("CID"), szFormat);


	// Add map entries to the polyline XDATA
	STRING_MAP::iterator XDataItr;
	for(XDataItr = mapOfXData.begin(); XDataItr != mapOfXData.end(); XDataItr++)
	{
		addXDATA(plineObjId, (*XDataItr).first, (*XDataItr).second); 
	}
	if(NSSTRCMP(szLineType, _T("ByLayer")) == 0)
	{
		return NS_BYLAYER_SET;
	}
	return NS_SUCCESS;
}

/*
@Brief			Creates a MultiLine with the input vector of CNSPoint3Ds
@Param(in)		CNSCompositeGeometry*
@Param(in)		TCHAR*		LineType
@Param(in)		TCHAR*		LayerName
@Param(in)		bool		true: creates closed polygon	false: does not create a closed polygon implicitly
@Param(in)		STRING_MAP	xdata	
@Return		int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::drawMultiLine(CNSCompositeGeometry *pCompositeGeo, TCHAR* szLineType, TCHAR* szLayerName, bool bClosed, STRING_MAP &mapOfXData)
{
	// ACAD based app used ObjectARX2006 library which does not know AcDbUtil used from getMultiLineStyleIdFromString() below.
	// This function is anyway not going to be called from ACAD application
#ifndef _INTERNAL_

	CNSCompositeData* pCompositeData = NULL;
	AcDbMline *pMLine = new AcDbMline();

	AcDbObjectId dashdotId;
	AcDbObjectId layerID;
	/*if(NSSTRCMP(szLineType, _T("")) == 0)
	szLineType = _T("ByLayer");*/
	if(NSSTRCMP(szLayerName, _T("")) == 0)
		szLayerName = _T("UNKNOWN_FEATURES");	// if szLayerName is empty put it on layer "UNKNOWN_FEATURES"

	getMultiLineStyleIdFromString(szLineType, dashdotId);
	getLayerNameIdFromString(szLayerName, layerID);
	if(layerID == NULL)
	{
		int nResult = 0;
		if(NSSTRCMP(szLayerName,_T("GIS_ERRORS")) == 0)
			nResult = addNewLayer(szLayerName, layerID, true);
		else
			nResult = addNewLayer(szLayerName, layerID);

		// If layer is not present create it
		if(NS_FAIL == nResult)
			return NS_FAIL;
	}
	if(dashdotId == NULL)
	{
		//getLinetypeIdFromString(_T("ByLayer"), dashdotId);
		dashdotId = m_pCurrentDatabase->cmlstyleID();
	}
	if(Acad::eOk != pMLine->setStyle(dashdotId)
		||	Acad::eOk != pMLine->setLayer(layerID))
	{
		delete pMLine;
		return NS_FAIL;
	}
	if(Acad::eOk!=pMLine->setNormal(AcGeVector3d::kZAxis)||
		Acad::eOk!=pMLine->setScale(m_pCurrentDatabase->cmlscale()))
	{
		delete pMLine;
		return NS_FAIL;
	}

	// Now add the 3D vertices to the polyline object
	int nGeoSize = pCompositeGeo->getPoslistSize();
	std::vector<CNSPoint3D> tempVectPoints;
	CNSPoint3D tempVertex;
	double tempZ;
	for(int nGeocntr = 0; nGeocntr<nGeoSize; nGeocntr++) 
	{
		pCompositeData = pCompositeGeo->getAt(nGeocntr);
		NSSTRING strVertexPoints;
		GEO_ELEMENT GL;
		pCompositeData->getData(strVertexPoints, GL);
		//Fetching Z value only for writing to Xdata
		convertString2Vertices(strVertexPoints, tempVectPoints);
		tempVertex = tempVectPoints.at(1);
		tempZ = tempVertex.m_dZZ;
		//--------------------------------------------------

		/*if(GL == ELE_LINEAR)*/
		{
			addMLineSegment(pMLine, strVertexPoints);
		}
		/*if(GL == ELE_CUVRVE || GL == ELE_CURVE_MEMEBR)
		{
		addPolyLineArcSegment(pPolyLine, strVertexPoints);
		}*/
	}

	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))
	{
		delete pMLine;
		return NS_FAIL;
	}

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite))
	{	
		delete pMLine;
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();

	// Append the pline object to the database and
	// obtain its object ID.
	AcDbObjectId plineObjId;
	if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(plineObjId, pMLine))
	{	
		delete pMLine;
		pBlockTableRecord->close();
		return NS_FAIL;
	}
	pBlockTableRecord->close();

	// Get string corresponding to first vertex of the polyline
	AcGePoint3d startPoint;
	startPoint = pMLine->vertexAt(0);
	TCHAR szTempString[50] = _T("");
	NSSPRINTF(szTempString, _T("%f %f %f"), (startPoint.x), (startPoint.y), tempZ);
	NSSTRING strStartVertex(szTempString);

	// Get string corresponding to second last vertex of the polyline
	AcGePoint3d vertexPoint;
	int nVertices = pMLine->numVertices();
	vertexPoint = pMLine->vertexAt(1);
	TCHAR szTempString1[50] = _T("");
	NSSPRINTF(szTempString1, _T("%f %f %f"), (vertexPoint.x), (vertexPoint.y), tempZ);
	NSSTRING strPointVertex(szTempString1);

	// Get string corresponding to area of polygon
	double dArea =0.0;
	//pPolyLine->getArea(dArea);
	NSSPRINTF(szTempString, _T("%f"), dArea);
	NSSTRING strArea(szTempString);

	//if((pCompositeGeo->getCompType() == COMP_BOUNDARY)
	//	|| (pCompositeGeo->getCompType() == COMP_POLYGON)
	//	|| (pCompositeGeo->getCompType() == COMP_COMPOSITE_POLYGON))
	//{
	//	pPolyLine->setClosed(Adesk::kTrue);
	//}


	pMLine->close();

	// Add position of first vertex and Area of a polyline to map of XDATA
	findAndInsert(mapOfXData, _T("START_VERTEX"), strStartVertex);
	findAndInsert(mapOfXData, _T("NEXT_VERTEX"), strPointVertex);
	findAndInsert(mapOfXData, _T("NS_AREA"), strArea);

	//mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("START_VERTEX"), strStartVertex));
	//mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("NEXT_VERTEX"), strPointVertex));
	//mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("NS_AREA"), strArea.c_str()));

	TCHAR szFormat[_MAX_PATH + 1];
	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCNO());
	findAndInsert(mapOfXData, _T("CNO"), szFormat);
	//	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CNO"), szFormat));

	NSSPRINTF(szFormat, _T("%i"), pCompositeGeo->getCID());
	findAndInsert(mapOfXData, _T("CID"), szFormat);
	//	mapOfXData.insert(std::map<NSSTRING, NSSTRING>::value_type(_T("CID"), szFormat));

	// Add map entries to the polyline XDATA
	STRING_MAP::iterator XDataItr;
	for(XDataItr = mapOfXData.begin(); XDataItr != mapOfXData.end(); XDataItr++)
	{
		addXDATA(plineObjId, (*XDataItr).first, (*XDataItr).second); 
	}
	/*if(NSSTRCMP(szLineType, _T("ByLayer")) == 0)
	{
	return NS_BYLAYER_SET;
	}*/
	return NS_SUCCESS;
#endif
	return NS_SUCCESS;
}


int CNSDWGMgr::setBlockAttributes(TCHAR* blockName, std::map<NSSTRING,NSSTRING> mAttributes)
{
	AcDbBlockTable *pBlockTable;
	Acad::ErrorStatus es;
	const TCHAR *szFileName;
	m_pCurrentDatabase->getFilename(szFileName);
	es = m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite);

	// Get a pointer to the MODEL_SPACE BlockTableRecord.
	//
	AcDbBlockTableRecord *pBlockTableRecord;
	es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlockIterator;
	pBlockTableRecord->newIterator(pBlockIterator);

	for (pBlockIterator->start(); !pBlockIterator->done();pBlockIterator->step())
	{
		AcDbEntity *pEnt;
		es = pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		AcDbBlockReference *pAcDbBlockReference = (AcDbBlockReference *)pEnt;

		AcDbObjectId blockId = pAcDbBlockReference->blockTableRecord();

		AcDbBlockTableRecord *pRecord = 0;
		acdbOpenObject(pRecord, blockId, AcDb::kForWrite);
		if (pRecord)
		{
			TCHAR *pName = NULL;
			if (pRecord->getName(pName) == Acad::eOk)
			{
				int nResult = NSSTRCMP(blockName, pName);
				if(nResult != 0)
				{
					acdbFree(pName);
					pRecord->close();
					pEnt->close();
					pAcDbBlockReference->close();
					continue;
				}
			}
			pRecord->close();
		}
		AcDbObjectIterator* pIter = pAcDbBlockReference->attributeIterator();
		while (!pIter->done())
		{
			AcDbObjectId objId = pIter->objectId();
			AcDbAttribute *pAtt;
			if (pAcDbBlockReference->openAttribute(pAtt, objId, AcDb::kForWrite)== Acad::eOk)
			{
				// code for checking if attribute is present in the map and then setting value should come here            
				const TCHAR* szAttName = pAtt->tag();
				std::map<NSSTRING,NSSTRING>::iterator it;
				for(it = mAttributes.begin(); it != mAttributes.end(); it++)
				{
					if(NSSTRCMP(szAttName, ((*it).first).c_str()) == 0)
					{
						pAtt->setTextString(((*it).second).c_str());
					}
				}
				pAtt->close();
			}
			pIter->step();
		}
		pEnt->close();
		pAcDbBlockReference->close();
		delete pIter;
	}
	delete pBlockIterator;
	pBlockTableRecord->close();
	return NS_SUCCESS;
}

std::vector<TCHAR*> CNSDWGMgr::getTitleBlockNames()
{
	std::vector<TCHAR*> aTitleBlocks;
	AcDbBlockTable *pBlockTable;
	m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite);
	// Get a pointer to the MODEL_SPACE BlockTableRecord.
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlockIterator;
	pBlockTableRecord->newIterator(pBlockIterator);
	for (; !pBlockIterator->done();pBlockIterator->step())
	{
		AcDbEntity *pEnt;
		pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
		if(pEnt == NULL)
			continue;
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}
		AcDbBlockReference *pAcDbBlockReference = (AcDbBlockReference *)pEnt;
		AcDbObjectId blockId = pAcDbBlockReference->blockTableRecord();

		AcDbBlockTableRecord *pRecord = 0;
		acdbOpenObject(pRecord, blockId, AcDb::kForWrite);
		if (pRecord)
		{
			TCHAR *pName;
			if (pRecord->getName(pName) == Acad::eOk)
			{
				NSSTRING strBlkName(pName);
				//size_t value = strBlkName.find(_T("_TITLE"));
				size_t value = findString(strBlkName, _T("_TITLE"));
				if (value != NSSTRING::npos)
				{
					aTitleBlocks.push_back(pName);
				}
				// Here check if block name has suffix _TITLE and if it has then only add it to array

				//acdbFree(pName);
			}
			pRecord->close();
		}
		pEnt->close();
		pAcDbBlockReference->close();
	}

	delete pBlockIterator;
	pBlockTableRecord->close();
	return aTitleBlocks;
}

AcDbObjectIdArray a_ids;
int CNSDWGMgr::setTitleBlockAttributes(std::map<NSSTRING,NSSTRING> mAttributes)
{
	AcDbBlockTable* pBTable = NULL;
	m_pCurrentDatabase->getBlockTable(pBTable, AcDb::kForRead);
	AcDbBlockTableRecord* pTRecord = NULL;	
	int Loop=1;

	for(Loop=1;Loop<5;Loop++)
	{		
		//AcDbObjectId id;
		NSSTRING strFrame;
		if(Loop==1)
		{
			strFrame=_T("A1_FRAME_TITLE");
		}
		else if(Loop ==2)
		{
			strFrame=_T("A2_FRAME_TITLE");
		}
		else if(Loop ==3)
		{
			strFrame=_T("A3_FRAME_TITLE");
		}
		else if(Loop ==4)
		{
			strFrame=_T("A4_FRAME_TITLE");
		}
		else
			continue;

		if(Acad::eOk != pBTable->getAt(strFrame.c_str(),pTRecord, AcDb::kForWrite))
		{
			continue;
		}
		if(Acad::eOk != pTRecord->getBlockReferenceIds(a_ids))
			continue;

		pTRecord->close();


		for(int i=0;i<a_ids.length();i++)
		{
			AcDbObject *pObj;
			AcDbObjectId id = a_ids[i];				
			//if (pBlockReference.openStatus() == Acad::eOk)
			if(Acad::eOk==acdbOpenObject(pObj,id,AcDb::kForRead))
			{
				AcDbBlockReference *pBlockReference =AcDbBlockReference::cast(pObj);
				if(pBlockReference == NULL)
					continue;

				AcDbObjectIterator *pIter = NULL;
				pIter = pBlockReference->attributeIterator();
				for(pIter->start();!pIter->done();pIter->step())
				{
					AcDbObjectId objId = pIter->objectId();
					AcDbAttribute *pAtt;
					if (pBlockReference->openAttribute(pAtt, objId, AcDb::kForWrite)== Acad::eOk)
					{
						// code for checking if attribute is present in the map and then setting value should come here            
						const TCHAR* szAttName = pAtt->tag();
						std::map<NSSTRING,NSSTRING>::iterator it;
						for(it = mAttributes.begin(); it != mAttributes.end(); it++)
						{
							if(NSSTRCMP(szAttName, ((*it).first).c_str()) == 0)
							{
								pAtt->setTextString(((*it).second).c_str());
							}
						}
						pAtt->close();
					}				
				}
			}
		}
	}
	return NS_SUCCESS;

//*************************************Original************************************************
	//AcDbBlockTable *pBlockTable;
	//Acad::ErrorStatus es;
	//es = m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite);
	//pBlockTable->close();
	//AcDbBlockTableRecord *pBlockTableRecord;
	//AcDbDictionary* pDict;
	//es = m_pCurrentDatabase->getLayoutDictionary(pDict,AcDb::kForRead);
	//AcDbDictionaryIterator* dicIter = pDict->newIterator();
	//for(;!dicIter->done();dicIter->next())
	//{
	//	AcDbLayout* layout;
	//	const TCHAR* name;
	//	dicIter->getObject((AcDbObject*&)layout,AcDb::kForWrite);
	//	layout->getLayoutName(name);
	//	AcDbObjectId layoutId = layout->getBlockTableRecordId();
	//	layout->close();
	//	es = acdbOpenObject(pBlockTableRecord,layoutId,AcDb::kForWrite);
	//	AcDbBlockTableRecordIterator *pBlockIterator;
	//	pBlockTableRecord->newIterator(pBlockIterator);
	//	for (; !pBlockIterator->done();pBlockIterator->step())
	//	{
	//		AcDbEntity *pEnt;
	//		pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
	//		if(!pEnt)
	//			continue;
	//		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
	//		{
	//			pEnt->close();
	//			continue;
	//		}
	//		AcDbBlockReference *pAcDbBlockReference = (AcDbBlockReference *)pEnt;
	//		AcDbObjectId blockId = pAcDbBlockReference->blockTableRecord();
	//		AcDbBlockTableRecord *pRecord = 0;
	//		acdbOpenObject(pRecord, blockId, AcDb::kForWrite);
	//		if (pRecord)
	//		{
	//			TCHAR *pName;
	//			if (pRecord->getName(pName) == Acad::eOk)
	//			{
	//				NSSTRING strBlkName(pName);
	//				//size_t value = strBlkName.find(_T("_TITLE"));
	//				size_t value = findString(strBlkName, _T("_TITLE"));
	//				if (value == NSSTRING::npos)
	//				{
	//					pRecord->close();
	//					pEnt->close();
	//					pAcDbBlockReference->close();
	//					continue;
	//				}
	//			}

	//			pRecord->close();
	//		}
	//		AcDbObjectIterator* pIter = pAcDbBlockReference->attributeIterator();
	//		for(pIter->start();!pIter->done();pIter->step())
	//		{
	//			AcDbObjectId objId = pIter->objectId();
	//			AcDbAttribute *pAtt;
	//			if (pAcDbBlockReference->openAttribute(pAtt, objId, AcDb::kForWrite)== Acad::eOk)
	//			{
	//				// code for checking if attribute is present in the map and then setting value should come here            
	//				const TCHAR* szAttName = pAtt->tag();
	//				std::map<NSSTRING,NSSTRING>::iterator it;
	//				for(it = mAttributes.begin(); it != mAttributes.end(); it++)
	//				{
	//					if(NSSTRCMP(szAttName, ((*it).first).c_str()) == 0)
	//					{
	//						pAtt->setTextString(((*it).second).c_str());
	//					}
	//				}
	//				pAtt->close();
	//			}
	//			//pIter->step();
	//		}
	//		pEnt->close();
	//		pAcDbBlockReference->close();
	//		delete pIter;
	//	} 
	//	pBlockTableRecord->close();
	//	delete pBlockIterator;
	//}
	//delete dicIter;
	//pDict->close();
	//return NS_SUCCESS;
}

void CNSDWGMgr::getLinetypeIdFromString(const TCHAR* str, AcDbObjectId& id)
{
	Acad::ErrorStatus err;
	// Get the table of currently loaded linetypes.
	//
	AcDbLinetypeTable *pLinetypeTable;
	err = m_pCurrentDatabase->getSymbolTable(pLinetypeTable, AcDb::kForRead);
	/*if (err != Acad::eOk)
	return err;*/
	// Get the ID of the linetype with the name that
	// str contains.
	//
	err = pLinetypeTable->getAt(str, id, Adesk::kTrue);
	pLinetypeTable->close();
	/*return err;*/
}

void CNSDWGMgr::getMultiLineStyleIdFromString(const TCHAR* str, AcDbObjectId& id)
{
#ifndef _INTERNAL_
	Acad::ErrorStatus err;

	err = AcDbDictUtil::getMLStyleId(id,str,m_pCurrentDatabase);
	if (err != Acad::eOk)
		id.setNull();
#endif
}

void CNSDWGMgr::getLayerNameIdFromString(const TCHAR* str, AcDbObjectId& id)
{
	Acad::ErrorStatus err;
	// Get the table of currently loaded linetypes.
	//
	AcDbLayerTable *pLayerTable;
	err = m_pCurrentDatabase->getSymbolTable(pLayerTable, AcDb::kForRead);
	/*if (err != Acad::eOk)
	return err;*/
	// Get the ID of the linetype with the name that
	// str contains.
	//
	err = pLayerTable->getAt(str, id, Adesk::kTrue);
	pLayerTable->close();
	/*return err;*/
}


///*
// @Brief				This function retieves the requested title block attributes
// @Param(out)		std::map<NSSTRING, NSSTRING>	map of title block attributes
// @Return			int								NS_SUCCESS/NS_FAIL
//*/
//int CNSDWGMgr::listTitleBlockHeaderAttributes(std::map<NSSTRING, NSSTRING> &mapTableAttributes)
//{
//	// Open the current database's LayoutDictionary.
//	AcDbDictionary* pLytDict = NULL;
//	if(Acad::eOk != m_pCurrentDatabase->getLayoutDictionary(pLytDict, AcDb::kForRead))
//		return NS_FAIL;
//
//	//Iterate layout dictionary
//	bool bTitleBlockFound = false;
//	
//	AcDbDictionaryIterator* pDicIter = pLytDict->newIterator();
//	for(pDicIter; !pDicIter->done(); pDicIter->next())
//    {
//		if(bTitleBlockFound == true)
//			break;
//			
//		AcDbObject* pObjLyt = NULL;
//		if(Acad::eOk != pDicIter->getObject(pObjLyt, AcDb::kForRead))
//		{
//			pLytDict->close();
//			return NS_FAIL;
//		}
//
//		AcDbObjectId layoutBTRId = ((AcDbLayout*)pObjLyt)->getBlockTableRecordId();
//		pObjLyt->close();
//
//		AcDbBlockTableRecord* pBlockTableRecord = NULL; // BTR corresponding to the layout in which Title block is found
//		if(Acad::eOk != acdbOpenObject(pBlockTableRecord,layoutBTRId, AcDb::kForRead))
//		{
//			pLytDict->close();
//			return NS_FAIL;
//		}
//		
//		// Iterate BTR to get at the reference of TITLE Block
//		AcDbBlockTableRecordIterator *pBlockIterator = NULL;
//		if(Acad::eOk != pBlockTableRecord->newIterator(pBlockIterator))
//		{
//			pLytDict->close();
//			pBlockTableRecord->close();
//			return NS_FAIL;
//		}
//
//		for(pBlockIterator->start(); !pBlockIterator->done(); pBlockIterator->step())
//		{
//			AcDbEntity* pEnt = NULL;
//			if(Acad::eOk != pBlockIterator->getEntity(pEnt, AcDb::kForRead))
//				continue;
//				
//			//TCHAR *t = (ACHAR *)AcDbBlockReference::desc()->appName();
//			if(!pEnt->isKindOf(AcDbBlockReference::desc()))
//			{
//				//pLytDict->close();
//				//pBlockTableRecord->close();
//				pEnt->close();
//				continue;
//			}
//
//			AcDbBlockReference* pTitleBlockReference = (AcDbBlockReference*)pEnt;
//			AcDbObjectId blkTblID = pTitleBlockReference->blockTableRecord();
//			AcDbBlockTableRecord* blkTblRecord = NULL;					// BTR corresponding to the TITLE Block
//			if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
//			{
//				pTitleBlockReference->close();
//				pBlockTableRecord->close();
//				pLytDict->close();
//				return NS_FAIL;
//			}
//			
//			// Get block name
//			TCHAR* pszInsertedBlockName = NULL;
//			blkTblRecord->getName(pszInsertedBlockName);
//			blkTblRecord->close();
//			
//			NSSTRING strInsertedBlockName = NSSTRING(pszInsertedBlockName);
//			if(strInsertedBlockName.size() > 6)
//			{
//				NSSTRING szBlockNameSuffix = strInsertedBlockName.substr((strInsertedBlockName.size()-6), 6);
//				if(NSSTRCMP(szBlockNameSuffix.c_str(), _T("_TITLE")) != 0 )
//				{
//					pTitleBlockReference->close();
//					continue;
//				}
//			}		
//			
//			//if( NSSTRCMP(pszInsertedBlockName, _T("A1_FRAME_TITLE")) == 0)
//			//{
//				AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
//
//				for(pIter->start(); !pIter->done(); pIter->step())
//				{
//					AcDbObjectId AttrId = pIter->objectId();
//					AcDbAttribute *pAtt;
//					if(Acad::eOk != pTitleBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
//					{
//						pLytDict->close();
//						pTitleBlockReference->close();
//						return NS_FAIL;
//					}
//
//					// code for checking if attribute is present in the map and then setting value should come here            
//					const TCHAR* pszAttName = pAtt->tag();
//					NSSTRING szAttName = pszAttName;
//					NSSTRING szAttValue = pAtt->textString();
//
//					std::map<NSSTRING, NSSTRING>::iterator itrMap;
//					itrMap = mapTableAttributes.find(szAttName);
//					// Store value in the map to be returned
//					if(itrMap != mapTableAttributes.end())
//					{ 
//						(*itrMap).second = szAttValue;
//					}					
//					pAtt->close();
//				}
//				delete pIter;
//			//}
//			
//			pEnt->close();
//			pTitleBlockReference->close();
//			bTitleBlockFound = TRUE;
//		}
//		delete pBlockIterator;
//		pBlockTableRecord->close();
//	}
//	pLytDict->close();
//	delete pDicIter;
//	return NS_SUCCESS;
//}


/*
@Brief				This function retieves the requested title block attributes
@Param(out)		std::map<NSSTRING, NSSTRING>	map of title block attributes
@Return			int								NS_SUCCESS/NS_FAIL
*/



AcDbObjectIdArray ids;
int CNSDWGMgr::listTitleBlockHeaderAttributes(std::map<NSSTRING, NSSTRING> &mapTableAttributes, std::map<NSSTRING, NSSTRING> &mapofPrompValues)
{
	AcDbBlockTable* pBTable = NULL;
	m_pCurrentDatabase->getBlockTable(pBTable, AcDb::kForRead);
	

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBTable->getAt(ACDB_PAPER_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBTable->close();
		return NS_FAIL;
	}	
	AcDbBlockTableRecordIterator *p_Iter;
	pBlockTableRecord->newIterator(p_Iter);

	for(;!p_Iter->done();p_Iter->step())
	{
		AcDbObjectId objId;

		p_Iter->getEntityId(objId);

		// open the entity for read
		AcDbObjectPointer<AcDbBlockReference>pTitleBlockReference (objId, AcDb::kForRead);
		// if ok
		if (pTitleBlockReference.openStatus() == Acad::eOk)
		{

			AcDbObjectId blkTblID = pTitleBlockReference->blockTableRecord();

			AcDbBlockTableRecord* blkTblRecord = NULL;
			if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
			{						
				continue;
			}

			// Get block name
			TCHAR* pszInsertedBlockName = NULL;
			blkTblRecord->getName(pszInsertedBlockName);

			NSSTRING strInsertedBlockName = NSSTRING(pszInsertedBlockName);

			if(strInsertedBlockName.size()<=6)
				continue;

			NSSTRING szBlockNameSuffix = strInsertedBlockName.substr((strInsertedBlockName.size()-6), 6);
			if(NSSTRCMP(szBlockNameSuffix.c_str(), _T("_TITLE")) != 0 )//A1_FRAME_TITLE, A2_FRAME_TITLE, A3_FRAME_TITLE, A4_FRAME_TITLE
			{	
				continue;
			}	

			NSSTRING_MAP tempMapAttPrompt;
			AcDbBlockTableRecordIterator *pBTRItr;
			blkTblRecord->newIterator(pBTRItr);
			for(pBTRItr->start(); !pBTRItr->done(); pBTRItr->step())
			{
				AcDbEntity* pAttDefEnt = NULL;
				if (Acad::eOk ==pBTRItr->getEntity(pAttDefEnt, AcDb::kForRead))
				{
					if(pAttDefEnt == NULL)
					{
						continue;
					}
					if(pAttDefEnt->isKindOf(AcDbAttributeDefinition::desc()))
					{
						AcDbAttributeDefinition *pTempAttdef = (AcDbAttributeDefinition*)(pAttDefEnt);
						TCHAR* pszAttDefTag = pTempAttdef->tag();
						TCHAR* pszAttDefPrompt = pTempAttdef->prompt();
						NSSTRING strAttDefPrompt = (NSSTRING)pszAttDefPrompt;
						int nDot = strAttDefPrompt.find_last_of(_T("("));
						strAttDefPrompt = strAttDefPrompt.substr(nDot+1, (strAttDefPrompt.length()-2));
						if(NSSTRCMP(strAttDefPrompt.c_str(),_T("GIS)")) == 0)
						{
							getupperString(pszAttDefTag);
							tempMapAttPrompt.insert(NSSTRING_MAP::value_type(pszAttDefTag, pszAttDefPrompt));
						}
					}
					pAttDefEnt->close();
				}
			}
			delete pBTRItr;
			blkTblRecord->close();
			//if( NSSTRCMP(pszInsertedBlockName, _T("A1_FRAME_TITLE")) == 0)

			//{
			AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
			for(pIter->start(); !pIter->done(); pIter->step())
			{
				AcDbObjectId AttrId = pIter->objectId();
				AcDbAttribute *pAtt;
				if (Acad::eOk != pTitleBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
				{
					pTitleBlockReference->close();					
					pBlockTableRecord->close();
					return NS_FAIL;
				}

				// code for checking if attribute is present in the map and then setting value should come here            
				const TCHAR* pszAttName = pAtt->tag();
				NSSTRING szAttName = pszAttName;
				NSSTRING szAttValue = pAtt->textString();
				std::map<NSSTRING, NSSTRING>::iterator itrMap;
			
				for(itrMap = mapTableAttributes.begin(); itrMap != mapTableAttributes.end(); itrMap++)
				{
					if(NSSTRCMP(szAttName.c_str(), ((*itrMap).first).c_str()) == 0)
					{
						(*itrMap).second = szAttValue;
					}
				}

				pAtt->close();

				//To store prompt values whose values are NULL
				itrMap = tempMapAttPrompt.find(szAttName);
				if(itrMap != tempMapAttPrompt.end())
				{ 
					NSSTRING_MAP::iterator tempPromptMapItr;
#ifndef _INTERNAL_
					NSSTRUPR((TCHAR*)szAttName.c_str(), szAttName.size()+1);
#else
					NSSTRUPR((TCHAR*)szAttName.c_str(), szAttName.size()+1);
					//NSSTRUPR((TCHAR*)szAttName.c_str());
#endif
					tempPromptMapItr = tempMapAttPrompt.find(szAttName.c_str());
					if(tempPromptMapItr != tempMapAttPrompt.end())
					{
						(*itrMap).second = (*tempPromptMapItr).second;
						if(NSSTRCMP(szAttValue.c_str(), _T("")) == 0)
							mapofPrompValues.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, (*tempPromptMapItr).second));
					}
				}
			}

			return NS_SUCCESS;
		}		
	}

return NS_FAIL;

//***************************************last fine working Code 02 Jan 2014************************************ //Due to error in Convert to XML, this code was changed

//	AcDbBlockTableRecord* pTRecord = NULL;
//	bool bTitleBlockFound = false;
//	int Loop=1;
//	try
//	{
//		for(Loop=1;Loop<5;Loop++)
//		{
//			if(bTitleBlockFound == true)
//			{
//				continue;
//			}
//			//AcDbObjectId id;
//			NSSTRING strFrame;
//			if(Loop==1)
//			{
//				strFrame=_T("A1_FRAME_TITLE");
//			}
//			else if(Loop ==2)
//			{
//				strFrame=_T("A2_FRAME_TITLE");
//			}
//			else if(Loop ==3)
//			{
//				strFrame=_T("A3_FRAME_TITLE");
//			}
//			else if(Loop ==4)
//			{
//				strFrame=_T("A4_FRAME_TITLE");
//			}
//			else
//				continue;	
//
//			if(Acad::eOk != pBTable->getAt(strFrame.c_str(),pTRecord, AcDb::kForRead))
//			{
//				continue;
//			}
//	
//			pBTable->close();
//			NSSTRING_MAP tempMapAttPrompt;
//			AcDbBlockTableRecordIterator *pBTRItr = NULL;
//
//			pTRecord->newIterator(pBTRItr);
//
//			for(pBTRItr->start(); !pBTRItr->done(); pBTRItr->step())
//			{
//				AcDbEntity* pAttDefEnt = NULL;
//				if (Acad::eOk ==pBTRItr->getEntity(pAttDefEnt, AcDb::kForRead))
//				{
//					if(pAttDefEnt == NULL)
//					{
//						continue;
//					}
//					if(pAttDefEnt->isKindOf(AcDbAttributeDefinition::desc()))
//					{
//						AcDbAttributeDefinition *pTempAttdef = (AcDbAttributeDefinition*)(pAttDefEnt);
//						TCHAR* pszAttDefTag = pTempAttdef->tag();
//						TCHAR* pszAttDefPrompt = pTempAttdef->prompt();
//						NSSTRING strAttDefPrompt = (NSSTRING)pszAttDefPrompt;
//						int nDot = strAttDefPrompt.find_last_of(_T("("));
//						strAttDefPrompt = strAttDefPrompt.substr(nDot+1, (strAttDefPrompt.length()-2));
//						if(NSSTRCMP(strAttDefPrompt.c_str(),_T("GIS)")) == 0)
//						{
//							getupperString(pszAttDefTag);
//							tempMapAttPrompt.insert(NSSTRING_MAP::value_type(pszAttDefTag, pszAttDefPrompt));
//						}
//					}
//					pAttDefEnt->close();
//				}
//			}
//			delete pBTRItr;
//			//pTRecord->close();
//
//
//
//			if(Acad::eOk != pTRecord->getBlockReferenceIds(ids))
//				continue;
//
//			pTRecord->close();
//		
//
//			for(int i=0;i<ids.length();i++)
//			{
//				AcDbObject *pObj;
//				AcDbObjectId id = ids[i];				
//				//if (pBlockReference.openStatus() == Acad::eOk)
//				if(Acad::eOk==acdbOpenObject(pObj,id,AcDb::kForRead))
//				{
//					AcDbBlockReference *pBlockReference =AcDbBlockReference::cast(pObj);
//					if(pBlockReference == NULL)
//						continue;
//
//
//					AcDbObjectIterator *pIter = NULL;
//					pIter = pBlockReference->attributeIterator();
//
//
//					//AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
//					for(pIter->start(); !pIter->done(); pIter->step())
//					{
//						AcDbObjectId AttrId = pIter->objectId();
//						AcDbAttribute *pAtt;
//						if (Acad::eOk != pBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
//						{		
//							//return NS_FAIL;
//							continue;
//						}
//
//						// code for checking if attribute is present in the map and then setting value should come here            
//						const TCHAR* pszAttName = pAtt->tag();
//						NSSTRING szAttName = pszAttName;
//						NSSTRING szAttValue = pAtt->textString();
//						std::map<NSSTRING, NSSTRING>::iterator itrMap;
//						for(itrMap = mapTableAttributes.begin(); itrMap != mapTableAttributes.end(); itrMap++)
//						{
//							if(NSSTRCMP(szAttName.c_str(), ((*itrMap).first).c_str()) == 0)
//							{
//								(*itrMap).second = szAttValue;
//							}
//						}
//
//						pAtt->close();
//
//						//To store prompt values whose values are NULL
//						itrMap = tempMapAttPrompt.find(szAttName);
//						if(itrMap != tempMapAttPrompt.end())
//						{ 
//							NSSTRING_MAP::iterator tempPromptMapItr;
//#ifndef _INTERNAL_
//							NSSTRUPR((TCHAR*)szAttName.c_str(), szAttName.size()+1);
//#endif
//							tempPromptMapItr = tempMapAttPrompt.find(szAttName.c_str());
//							if(tempPromptMapItr != tempMapAttPrompt.end())
//							{
//								(*itrMap).second = (*tempPromptMapItr).second;
//								if(NSSTRCMP(szAttValue.c_str(), _T("")) == 0)
//									mapofPrompValues.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, (*tempPromptMapItr).second));
//							}
//						}
//					}
//					pBlockReference->close();
//					delete pIter;					
//					break;
//					//bTitleBlockFound = true;
//				}
//			}
//
//			//pTRecord->close();		
//
//		}	
//
//	}
//	catch(...){}
	//return NS_SUCCESS;

//*****************************************************************************************

//**********************************Original************************************************

//	 Open the current database's LayoutDictionary.
//	AcDbDictionary* pLytDict = NULL;
//	if(Acad::eOk != m_pCurrentDatabase->getLayoutDictionary(pLytDict, AcDb::kForRead))
//		return NS_FAIL;
//
//	AcDbObject* pObjLyt = NULL;
//
//
//	//Iterate layout dictionary
//	bool bTitleBlockFound = false;
//	AcDbDictionaryIterator* pDicIter = pLytDict->newIterator();
//	for(; !pDicIter->done(); pDicIter->next())
//	{
//		if(bTitleBlockFound == true)
//			break;
//		if(Acad::eOk != pDicIter->getObject(pObjLyt, AcDb::kForWrite))
//		{
//			pLytDict->close();
//			delete pDicIter;
//			return NS_FAIL;
//		}
//
//		const ACHAR* layoutName;
//		((AcDbLayout*)pObjLyt)->getLayoutName(layoutName);
//
//		if(NSSTRCMP(layoutName,_T("Model")) == 0)
//		{
//			pObjLyt->close();
//			pLytDict->close();
//			continue;
//		}
//
//		AcDbObjectId layoutBTRId = ((AcDbLayout*)pObjLyt)->getBlockTableRecordId();
//		pObjLyt->close();
//
//		AcDbBlockTableRecord* pBlockTableRecord = NULL;
//		if(Acad::eOk != acdbOpenObject(pBlockTableRecord,layoutBTRId,AcDb::kForRead))
//		{
//			pLytDict->close();
//			delete pDicIter;
//			return NS_FAIL;
//		}
//
//		AcDbBlockTableRecordIterator *pBlockIterator = NULL;
//
//		if(Acad::eOk != pBlockTableRecord->newIterator(pBlockIterator))
//		{
//			pLytDict->close();
//			delete pDicIter;
//			pBlockTableRecord->close();
//			return NS_FAIL;
//		}
//		int Count=0;
//
//		for (pBlockIterator->start(); !pBlockIterator->done(); pBlockIterator->step())
//		{
//			AcDbEntity* pEnt = NULL;
//
//			Acad::ErrorStatus es=	pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
//
//			if(es != Acad::eOk)
//				continue;
//
//			if(pEnt == NULL)
//				continue;
//
//			if(pEnt->isKindOf(AcDbLeader::desc()) || pEnt->isKindOf(AcDbMLeader::desc()))
//			{
//				//pLytDict->close();
//				pEnt->close();
//				continue;
//			}
//			if(pEnt->isKindOf(AcDbSurface::desc()) || pEnt->isKindOf(AcDbRotatedDimension::desc()))
//			{
//				//pLytDict->close();
//				pEnt->close();
//				continue;
//			}
//			//if(pEnt->isKindOf(AcDbHatch::desc()) || pEnt->isKindOf(AcDbViewport::desc()) || pEnt->isKindOf(AcDbAlignedDimension::desc()) || pEnt->isKindOf(AcDbSolid::desc()) || pEnt->isKindOf(AcDbAttributeDefinition::desc()) || pEnt->isKindOf(AcDbText::desc())|| pEnt->isKindOf(AcDbArc::desc()) || pEnt->isKindOf(AcDbMText::desc()) || pEnt->isKindOf(AcDbLine::desc()) || pEnt->isKindOf(AcDbCircle::desc()))
//			if(pEnt->isKindOf(AcDbHatch::desc()) || pEnt->isKindOf(AcDbViewport::desc()) || pEnt->isKindOf(AcDbAttributeDefinition::desc()) || pEnt->isKindOf(AcDbText::desc())|| pEnt->isKindOf(AcDbArc::desc()) || pEnt->isKindOf(AcDbMText::desc()) || pEnt->isKindOf(AcDbLine::desc()) || pEnt->isKindOf(AcDbCircle::desc()))
//			{
//				//pLytDict->close();
//				pEnt->close();
//				continue;
//			}
//			try
//			{
//			if(!pEnt->isKindOf(AcDbBlockReference::desc()))
//			{
//				//pLytDict->close();
//				pEnt->close();
//				continue;
//			}
//			}
//			catch(...)
//	{
//		continue;
//	}
//
//		
//			AcDbBlockReference* pTitleBlockReference = (AcDbBlockReference*)pEnt;
//			AcDbObjectId blkTblID = pTitleBlockReference->blockTableRecord();
//
//			AcDbBlockTableRecord* blkTblRecord = NULL;
//			if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
//			{
//				pLytDict->close();
//				delete pDicIter;
//				pEnt->close();
//				pBlockTableRecord->close();
//				delete pBlockIterator;
//				return NS_FAIL;
//			}
//
//			// Get block name
//			TCHAR* pszInsertedBlockName = NULL;
//			blkTblRecord->getName(pszInsertedBlockName);
//
//			NSSTRING strInsertedBlockName = NSSTRING(pszInsertedBlockName);
//			if(strInsertedBlockName.size() > 6)
//			{
//				NSSTRING szBlockNameSuffix = strInsertedBlockName.substr((strInsertedBlockName.size()-6), 6);
//				if(NSSTRCMP(szBlockNameSuffix.c_str(), _T("_TITLE")) != 0 )//A1_FRAME_TITLE, A2_FRAME_TITLE, A3_FRAME_TITLE, A4_FRAME_TITLE
//				{
//					blkTblRecord->close();
//					pEnt->close();
//					continue;
//				}
//			}
//			else
//			{
//				blkTblRecord->close();
//				pEnt->close();
//				continue;
//			}
//
//			NSSTRING_MAP tempMapAttPrompt;
//			AcDbBlockTableRecordIterator *pBTRItr;
//			blkTblRecord->newIterator(pBTRItr);
//			for(pBTRItr->start(); !pBTRItr->done(); pBTRItr->step())
//			{
//				AcDbEntity* pAttDefEnt = NULL;
//				if (Acad::eOk ==pBTRItr->getEntity(pAttDefEnt, AcDb::kForRead))
//				{
//					if(pAttDefEnt == NULL)
//					{
//						continue;
//					}
//					if(pAttDefEnt->isKindOf(AcDbAttributeDefinition::desc()))
//					{
//						AcDbAttributeDefinition *pTempAttdef = (AcDbAttributeDefinition*)(pAttDefEnt);
//						TCHAR* pszAttDefTag = pTempAttdef->tag();
//						TCHAR* pszAttDefPrompt = pTempAttdef->prompt();
//						NSSTRING strAttDefPrompt = (NSSTRING)pszAttDefPrompt;
//						int nDot = strAttDefPrompt.find_last_of(_T("("));
//						strAttDefPrompt = strAttDefPrompt.substr(nDot+1, (strAttDefPrompt.length()-2));
//						if(NSSTRCMP(strAttDefPrompt.c_str(),_T("GIS)")) == 0)
//						{
//							getupperString(pszAttDefTag);
//							tempMapAttPrompt.insert(NSSTRING_MAP::value_type(pszAttDefTag, pszAttDefPrompt));
//						}
//					}
//					pAttDefEnt->close();
//				}
//			}
//			delete pBTRItr;
//			blkTblRecord->close();
//			//if( NSSTRCMP(pszInsertedBlockName, _T("A1_FRAME_TITLE")) == 0)
//
//			//{
//			AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
//			for(pIter->start(); !pIter->done(); pIter->step())
//			{
//				AcDbObjectId AttrId = pIter->objectId();
//				AcDbAttribute *pAtt;
//				if (Acad::eOk != pTitleBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
//				{
//					pLytDict->close();
//					delete pDicIter;
//					pTitleBlockReference->close();
//					delete pBlockIterator;
//					pBlockTableRecord->close();
//					return NS_FAIL;
//				}
//
//				// code for checking if attribute is present in the map and then setting value should come here            
//				const TCHAR* pszAttName = pAtt->tag();
//				NSSTRING szAttName = pszAttName;
//				NSSTRING szAttValue = pAtt->textString();
//				std::map<NSSTRING, NSSTRING>::iterator itrMap;
//				//itrMap = mapTableAttributes.find(szAttName);
//
//				//// Store value in the map to be returned
//				//if(itrMap != mapTableAttributes.end())
//				//{ 
//				//	(*itrMap).second = szAttValue;
//
//				//	/*NSSTRING_MAP::iterator tempPromptMapItr;
//				//	tempPromptMapItr = tempMapAttPrompt.find(szAttName.c_str());
//				//	if(tempPromptMapItr != tempMapAttPrompt.end())
//				//	{
//				//	mapofPrompValues.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, (*tempPromptMapItr).second));
//				//	}*/
//				//}   
//
//
//				for(itrMap = mapTableAttributes.begin(); itrMap != mapTableAttributes.end(); itrMap++)
//				{
//					if(NSSTRCMP(szAttName.c_str(), ((*itrMap).first).c_str()) == 0)
//					{
//						(*itrMap).second = szAttValue;
//					}
//				}
//
//				pAtt->close();
//
//				//To store prompt values whose values are NULL
//				itrMap = tempMapAttPrompt.find(szAttName);
//				if(itrMap != tempMapAttPrompt.end())
//				{ 
//					NSSTRING_MAP::iterator tempPromptMapItr;
//#ifndef _INTERNAL_
//					NSSTRUPR((TCHAR*)szAttName.c_str(), szAttName.size()+1);
//#else
//					NSSTRUPR((TCHAR*)szAttName.c_str(), szAttName.size()+1);
//					//NSSTRUPR((TCHAR*)szAttName.c_str());
//#endif
//					tempPromptMapItr = tempMapAttPrompt.find(szAttName.c_str());
//					if(tempPromptMapItr != tempMapAttPrompt.end())
//					{
//						(*itrMap).second = (*tempPromptMapItr).second;
//						if(NSSTRCMP(szAttValue.c_str(), _T("")) == 0)
//							mapofPrompValues.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, (*tempPromptMapItr).second));
//					}
//				}
//			}
//			delete pIter;
//			//}
//			pEnt->close();
//			pTitleBlockReference->close();
//			bTitleBlockFound = TRUE;
//		}    
//		delete pBlockIterator;
//		pBlockTableRecord->close();
//	}
//	pLytDict->close();
//	delete pDicIter;
//
//
//
//	return NS_SUCCESS;
//*********************************************************************************************************
}
/*
@Brief			Adds XDATA to the given object
@Param(in)		AcDbObjectId	Object to which XDATA is to be attached
@Param(in)		TCHAR*			XDATA: Application name
@Param(in)		TCHAR*			XDATA: Value
@Return		int				NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::addXDATA(AcDbObjectId ObjectId, NSSTRING p_szAppName, NSSTRING p_szAppValue)
{	Acad::ErrorStatus es = Acad::eOk;

	//Open object for write
	AcDbObject *pObj;
	if(Acad::eOk != acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForWrite))	
		return NS_FAIL;

	struct  resbuf  *pRb, *pTemp;    
	pRb = pObj->xData(p_szAppName.c_str());
	if (pRb != NULL) 
	{
		for (pTemp = pRb; pTemp->rbnext != NULL; pTemp = pTemp->rbnext);
	}
	else
	{
		//register application name (XDATA-Key)
		registerApplication(m_pCurrentDatabase, p_szAppName.c_str());

		pRb = acutNewRb(AcDb::kDxfRegAppName);
		pTemp = pRb;

		pTemp->resval.rstring =  (ACHAR*) new ACHAR[(NSSTRLEN(p_szAppName.c_str())+1)];
#ifdef _INTERNAL_
		//NSSTRCPY(pTemp->resval.rstring,(ACHAR*)p_szAppName.c_str());
		NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(p_szAppName.c_str())+1, (ACHAR*)p_szAppName.c_str());
#else
		NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(p_szAppName.c_str())+1, (ACHAR*)p_szAppName.c_str());
#endif
	}

	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;

	pTemp->resval.rstring =  (ACHAR*) new ACHAR[(NSSTRLEN(p_szAppValue.c_str())+1)];

#ifdef _INTERNAL_
	//NSSTRCPY(pTemp->resval.rstring,(ACHAR*)p_szAppValue.c_str());
	NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(p_szAppValue.c_str())+1, (ACHAR*)p_szAppValue.c_str());
#else
	NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(p_szAppValue.c_str())+1, (ACHAR*)p_szAppValue.c_str());
#endif

	es = pObj->setXData(pRb);

	pObj->close();
	//acutRelRb(pTemp);
#ifndef _DEBUG
	acutRelRb(pRb);
#endif

	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}


/*
@Brief		This function is used to add Relationship/Connectivity related XDATA
*/
/*
int CNSDWGMgr::addXDATA(long lObjectId, std::map<NSSTRING, NSSTRING> &mapXData)
{
Acad::ErrorStatus es = Acad::eOk;
//AcDbHandle objectHandle = (AcDbHandle)lObjectHandle;

AcDbObjectId ObjectId;
//m_pCurrentDatabase->getAcDbObjectId(ObjectId, false, objectHandle, 0);
ObjectId.setFromOldId(lObjectId);

//Open object for write
AcDbObject *pObj;
if(Acad::eOk != (es = acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForWrite)))
return NS_FAIL;

// For New features delete all XDATA
resbuf *xdata = NULL;
while ((xdata = pObj->xData(NULL)) != NULL)
{
resbuf *xdata_next = xdata->rbnext;
xdata->rbnext = NULL;
pObj->setXData(xdata);
xdata->rbnext = xdata_next;
#ifndef _DEBUG
acutRelRb(xdata);
#endif
}

std::map<NSSTRING, NSSTRING>::iterator mapItr;
for(mapItr = mapXData.begin(); mapItr != mapXData.end(); mapItr++)
{
NSSTRING strAppName, strAppValue;
strAppName = (*mapItr).first;
strAppValue = (*mapItr).second;

struct  resbuf  *pRb, *pTemp;    
pRb = pObj->xData(strAppName.c_str());
if (pRb != NULL )
{
// If XDATA already present then delete it 
pRb->rbnext = NULL;
pObj->setXData(pRb);
//for (pTemp = pRb; pTemp->rbnext != NULL; pTemp = pTemp->rbnext);
// if strAppValue is null delete these 2 links
}
if (NSSTRCMP(strAppValue.c_str(), _T("")) != 0)
{
// if strAppValue is NOT null add a new link

//register application name (XDATA-Key)
registerApplication(m_pCurrentDatabase, strAppName.c_str());

pRb = acutNewRb(AcDb::kDxfRegAppName);
pTemp = pRb;

pTemp->resval.rstring =  (ACHAR*) new ACHAR[(NSSTRLEN(strAppName.c_str())+1)];
#ifdef _INTERNAL_
NSSTRCPY(pTemp->resval.rstring,(ACHAR*)strAppName.c_str());
#else
NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(strAppName.c_str())+1, (ACHAR*)strAppName.c_str());
#endif

pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
pTemp = pTemp->rbnext;

pTemp->resval.rstring = (ACHAR*) new ACHAR[(NSSTRLEN(strAppValue.c_str())+1)];
#ifdef _INTERNAL_
NSSTRCPY(pTemp->resval.rstring,(ACHAR*)strAppValue.c_str());
#else
NSSTRCPY(pTemp->resval.rstring, NSSTRLEN(strAppValue.c_str())+1, (ACHAR*)strAppValue.c_str());
#endif
es = pObj->setXData(pRb);
}

#ifndef _DEBUG
acutRelRb(pRb);
#endif
}

pObj->close();

return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}
*/





//********************************************************* Commented on 31-Jan-2014 for 64-bit built****************************************************
//int CNSDWGMgr::addXDATA(long lObjectId, std::map<NSSTRING, NSSTRING> &mapXData)
//{
//	Acad::ErrorStatus es = Acad::eOk;
//	AcDbObjectId ObjectId;
//	ObjectId.setFromOldId(lObjectId);
//
//	//Open object for write
//	AcDbObject *pObj;
//	if(Acad::eOk != (es = acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForWrite)))
//		return NS_FAIL;
//
//	std::map<NSSTRING, NSSTRING>::iterator mapItr;
//	for(mapItr = mapXData.begin(); mapItr != mapXData.end(); mapItr++)
//	{
//		NSSTRING strAppName, strAppValue;
//		strAppName = (*mapItr).first;
//		//strAppName.append(_T("__NS"));
//		strAppValue = (*mapItr).second;
//
//		struct  resbuf  *pRb, *pTemp;    
//		pRb = pObj->xData(strAppName.c_str());
//		// If XDATA is already present replace the xdata
//		if (pRb != NULL && pRb->rbnext != NULL)
//		{
//			if (NSSTRCMP(strAppValue.c_str(), _T("")) != 0)
//			{
//				pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
//				pRb->rbnext = pTemp;
//
//				pTemp->resval.rstring = (ACHAR*) new ACHAR[(NSSTRLEN(strAppValue.c_str())+1)];
//#ifdef _INTERNAL_
//				//NSSTRCPY(pTemp->resval.rstring,(ACHAR*)strAppValue.c_str());
//				NSSTRCPY(pTemp->resval.rstring, NSSTRLEN(strAppValue.c_str())+1, (ACHAR*)strAppValue.c_str());
//#else
//				NSSTRCPY(pTemp->resval.rstring, NSSTRLEN(strAppValue.c_str())+1, (ACHAR*)strAppValue.c_str());
//#endif
//			}
//			else
//			{
//				pRb->rbnext = NULL;
//			}
//			es = pObj->setXData(pRb);
//		}
//		// else a new xadata element
//		else if (NSSTRCMP(strAppValue.c_str(), _T("")) != 0)
//		{	
//			//register application name (XDATA-Key)
//			registerApplication(m_pCurrentDatabase, strAppName.c_str());
//
//			pRb = acutNewRb(AcDb::kDxfRegAppName);
//			pTemp = pRb;
//
//			pTemp->resval.rstring =  (ACHAR*) new ACHAR[(NSSTRLEN(strAppName.c_str())+1)];
//#ifdef _INTERNAL_
//			//NSSTRCPY(pTemp->resval.rstring,(ACHAR*)strAppName.c_str());
//			NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(strAppName.c_str())+1, (ACHAR*)strAppName.c_str());
//#else
//			NSSTRCPY(pTemp->resval.rstring,NSSTRLEN(strAppName.c_str())+1, (ACHAR*)strAppName.c_str());
//#endif
//
//			pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
//			pTemp = pTemp->rbnext;
//
//			pTemp->resval.rstring = (ACHAR*) new ACHAR[(NSSTRLEN(strAppValue.c_str())+1)];
//#ifdef _INTERNAL_
//			//NSSTRCPY(pTemp->resval.rstring,(ACHAR*)strAppValue.c_str());
//			NSSTRCPY(pTemp->resval.rstring, NSSTRLEN(strAppValue.c_str())+1, (ACHAR*)strAppValue.c_str());
//#else
//			NSSTRCPY(pTemp->resval.rstring, NSSTRLEN(strAppValue.c_str())+1, (ACHAR*)strAppValue.c_str());
//#endif
//			es = pObj->setXData(pRb);
//		}
//
//#ifndef _DEBUG
//		acutRelRb(pRb);
//#endif
//	}
//
//	pObj->close();
//
//	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
//}

/*
@Brief			Registers the application name with the database
@Param(in)		AcDbDatabase*	Pointer to database
@Param(in)		NSSTRING			Application name
*/
void CNSDWGMgr::registerApplication(AcDbDatabase* pDatabase, NSSTRING strAppName)
{
	AcDbRegAppTable *pRegAppTable; 
	AcDbObjectId blockId;
	if (pDatabase->getRegAppTable(pRegAppTable, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbRegAppTableRecord *pRecord = new AcDbRegAppTableRecord; 
		if (pRecord)
		{
			pRecord->setName(strAppName.c_str());
			if (pRegAppTable->add(blockId, pRecord) == Acad::eOk)
				pRecord->close();
			else 
				delete pRecord;
		}         
		pRegAppTable->close();
		//pRecord->close();
	}
}



/*
@Brief			Returns vector of requested blocks
@Param(out)		vector<CNSFeatureContainer*>&
@param(in)		TCHAR*	Layer name 
@param(in)		TCHAR*	Block name
@Return			int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::selectBlocks(std::vector<CNSFeatureContainer*>& vectBlocks, TCHAR* pszLayerName, TCHAR* pszBlockName)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Rotation, Attributes and XDATA) of model space pBlockRef entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt = NULL;

	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// If the entity is a pBlockRef get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}
	
		NSSTRING strLayerName = NSSTRING(pszLayerName);
		NSSTRING strLayer;
		// Select blocks on given layer name otherwise select on all layers
		if(NSSTRCMP(pszLayerName, _T("all")) !=0 )
		{
			NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
			strLayer = strEntityLayer;
			if(strEntityLayer.size() >= strLayerName.size())
			{
				strEntityLayer = strEntityLayer.substr((strEntityLayer.size()-strLayerName.size()), strEntityLayer.size());
			}
			if(NSSTRCMP(pszLayerName, strEntityLayer.c_str()) != 0)
			{
				pEnt->close();
				continue;
			}
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		// we can get the name from the BlockTableRecord type
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		NSSTRING strBlockName = NSSTRING(pszBlockName);
		// Select blocks of given name otherwise select all blocks
		NSSTRING strInsertedBlockName;
		if(NSSTRCMP(pszBlockName, _T("all")) !=0 )
		{
			strInsertedBlockName = NSSTRING(pszInsertedBlockName1);

			getEffectiveName( blkTblRecord,  pBlockRef, strInsertedBlockName/*,  pszInsertedBlockName*/);

			NSSTRING strtemp = strInsertedBlockName;
			if(strtemp.size() > strBlockName.size())
				strtemp = strtemp.substr((strtemp.size()-strBlockName.size()), strtemp.size());

			if(NSSTRCMP(pszBlockName, strtemp.c_str()) != 0)
			{
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				continue;
			}	
		}
		CNSFeatureContainer *pFeatContainer = new CNSFeatureContainer;
		CNSBlock* pTempNSBlock = new CNSBlock();
		pTempNSBlock->setBlockName(strInsertedBlockName);
		pTempNSBlock->setRotation(pBlockRef->rotation());
		pTempNSBlock->setAlignment(0); //For block the PointInAlignment value is 0.
		double tempdZ = 0;
		//pTempNSBlock->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);
		pTempNSBlock->setType(ELE_BLK_POINT);

		// Get attributes of the pBlockRef entity
		AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
		AcDbAttribute* pTempAttribute = NULL;
		CNSAttributeRef *pRef = NULL;
		pFeatContainer->m_mapOfBlkAtt = new ATT_MAP;
		//--------- Reading Z attribute value -------------------
		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				delete pTempNSBlock;
				pTempAttribute->close();
				delete attributeIterator;
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				pRecord->close();
				pBlockTable->close();				
				return NS_FAIL;
			}
			TCHAR* pszKey= pTempAttribute->tag();					
			TCHAR* pszValue = pTempAttribute->textString();
			
			if(NSSTRCMP(pszKey, _T("Z")) == 0 && pszValue[0] == _T('\0'))
				tempdZ = 0;
			else if(NSSTRCMP(pszKey, _T("Z")) == 0)
				tempdZ = NSATOF(NSSTRTOK(NULL, _T("\0"), &pszValue)); // Z value.
			if(pTempAttribute != NULL)
				pTempAttribute->close();
		}
		//----------------------------------------------

		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				delete pTempNSBlock;
				pTempAttribute->close();
				delete attributeIterator;
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				pRecord->close();
				pBlockTable->close();				
				return NS_FAIL;
			}

			TCHAR* pszValue = NULL;
			pRef = new CNSAttributeRef;
			TCHAR* pszKey= pTempAttribute->tag();
			//--Add drawing number to FB_REFERENCE attribute for New Feature--
			if(NSSTRCMP(pszKey,_T("FB_REFERENCE")) == 0 &&
				NSSTRCMP(pszLayerName,_T("NEW")) == 0)
				pRef->setAttributeValue(m_strDrawingNum);
			else if(NSSTRCMP(pszKey,_T("FB_REFERENCE")) == 0 &&  //For modified append FB Ref
				NSSTRCMP(pszLayerName,_T("MOD")) == 0)
			{
				NSSTRING tempStr(pTempAttribute->textString());
				if(NSSTRCMP(tempStr.c_str(),_T("")) != 0 && NSSTRCMP(m_strDrawingNum.c_str(),_T("")) != 0) 
					tempStr.append(_T(","));
				tempStr.append(m_strDrawingNum);
				pRef->setAttributeValue(tempStr);
			}
			else
				pRef->setAttributeValue(pTempAttribute->textString());

			//-------------------------------------------

			//pRef->setAttributeValue(pszValue);
			CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;
			CNSBlock *pBlock = new CNSBlock;
			pGeometry->add(pBlock);
			AcGePoint3d InsertionPoint = pTempAttribute->position();
			pBlock->setInsertionPoint(InsertionPoint.x, InsertionPoint.y, tempdZ);
			pBlock->setRotation(pTempAttribute->rotation());
			pBlock->setAlignment(getAlignment(pTempAttribute));
			pGeometry->setCompType(COMP_TEXTPOINT);
			pRef->setCompGeometry(pGeometry);
			pFeatContainer->addAttributeRef(pszKey, 1, pRef);
			// Adds attribute to the Block to be returned
			//pFeatContainer->m_mapOfBlkAtt->insert(ATT_MAP::value_type(pszKey, pRef));	
			if(pTempAttribute != NULL)
				pTempAttribute->close();
		}

		//set insertion point of block 
		pTempNSBlock->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y, tempdZ);

		delete attributeIterator;				

		// Retrieve XDATA of the pBlockRef entity and add it to pTempNSBlock
		//std::map<NSSTRING, NSSTRING> mapXDataRead;
		//readXDATA(pEnt->id(), mapXDataRead);
		//pTempNSBlock->addXDATA(mapXDataRead);	
		pFeatContainer->m_mapOfXData = new STRING_MAP;
		readXDATA(pEnt->id(), *pFeatContainer->m_mapOfXData);		

		int SubFeatureCount = createSubFeatures(pBlockRef, pFeatContainer); 		

		if(pBlockRef != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();

		// Add the CNSBlock to the vector to be returned
		NSSTRING strList;
		GEO_ELEMENT enType = ELE_INVALID;

		CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;

		std::map<NSSTRING, NSSTRING>::iterator XdataIter;
		NSSTRING strValue;
		STRING_MAP::iterator TempItr;
		//readXDATA(pEnt->id(), pFeatContainer->mapOfXData);
		XdataIter = pFeatContainer->m_mapOfXData->find(_T("CNO"));
		if(XdataIter != pFeatContainer->m_mapOfXData->end())
			pGeometry->setCNO(NSATOI(((*XdataIter).second).c_str()));
		XdataIter = pFeatContainer->m_mapOfXData->find(_T("CID"));
		if(XdataIter != pFeatContainer->m_mapOfXData->end())
			pGeometry->setCID(NSATOI(((*XdataIter).second).c_str()));
		else
			pGeometry->setCID(1);
		pGeometry->setName(strInsertedBlockName);
		pGeometry->setLayername(strLayer);
		pGeometry->setCompType(COMP_POINT);
		//pTempNSBlock->getData(strList, enType);
		//pGeometry->addPoslist(enType, strList);
		pGeometry->add(pTempNSBlock);
		CNSCompositeContainer *pContainer = new CNSCompositeContainer;
		pContainer->add(pGeometry);
		pFeatContainer->m_pContainer = pContainer;
		vectBlocks.push_back(pFeatContainer);

	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	if(pBlockTable != NULL)
		pBlockTable->close();

	return NS_SUCCESS;
}



/*
@Brief			Reads XDATA of the given 
@Param(in)		ObjectId of the object of which XDATA is to be read
@Param(out)	std::map<NSSTRING, NSSTRING>	XDATA.
@Return		NS_SUCCESS/NS_FAIL
*/
/*
int CNSDWGMgr::readXDATA(AcDbObjectId ObjectId, std::map<NSSTRING, NSSTRING> &mapXData)
{
//Open object for write
AcDbObject *pObj;
if(Acad::eOk != acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForRead))	
return NS_FAIL;

struct  resbuf  *pRb, *pTemp; 
pRb = pObj->xData(NULL);
if (pRb != NULL) 
{
// If xdata is present, then retrieve all key-value pairs
for (pTemp = pRb; pTemp != NULL;)
{


NSSTRING strAppName = pTemp->resval.rstring;

// take next application name if it is not added by this application
if(strAppName.find(_T("__NS")) != -1)
{
// remove psuffix __NS from the application name
NSSTRING strFinalAppName = strAppName.substr(0, strAppName.size()-4);
NSSTRING strAppValue = pTemp->rbnext->resval.rstring;
mapXData.insert(std::map<NSSTRING, NSSTRING>::value_type(strFinalAppName, strAppValue));
pTemp = pTemp->rbnext->rbnext;
}
else // get next application name
{
while(pTemp != NULL)
{
if(pTemp->restype == AcDb::kDxfRegAppName)
{
break;
}
else
pTemp = pTemp->rbnext;
}
}

// read xdata values
//
//NSSTRING strAppValue = _T("");
//int nValueCount = 0;
//while(pTemp != NULL || pTemp->restype != AcDb::kDxfRegAppName)
//{
//	TCHAR *pszAppvalue = pTemp->rbnext->resval.rstring;
//	if(nValueCount = 0)
//	{
//		strAppValue.append(pszAppvalue);
//	}
//	else
//	{
//		strAppValue.append(_T(", "));
//		strAppValue.append(pszAppvalue);
//		nValueCount++;
//	}
//	pTemp = pTemp->rbnext;
//}

//mapXData.insert(std::map<NSSTRING, NSSTRING>::value_type(strAppName, strAppValue));
}
}
pObj->close();
return NS_SUCCESS;
}
*/
int CNSDWGMgr::readXDATA(AcDbObjectId ObjectId, std::map<NSSTRING, NSSTRING> &mapXData)
{
	//Open object for write
	AcDbObject *pObj;
	if(Acad::eOk != acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForRead))	
		return NS_FAIL;

	struct  resbuf  *pRb, *pTemp; 
	pRb = pObj->xData(NULL);


	// If xdata is present, then retrieve all key-value pairs
	for (pTemp = pRb; pTemp != NULL;)
	{
		if(pTemp->restype == AcDb::kDxfRegAppName)
		{
			NSSTRING strAppName = pTemp->resval.rstring;
			pTemp = pTemp->rbnext;

			////////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY
			if(pTemp->restype == AcDb::kDxfXdControlString)
			{
				pTemp = pTemp->rbnext;
			}
			////////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY

			if(pTemp!= NULL && pTemp->restype == AcDb::kDxfXdAsciiString)
			{
				NSSTRING strAppValue = pTemp->resval.rstring;
				mapXData.insert(std::map<NSSTRING, NSSTRING>::value_type(strAppName, strAppValue));
				pTemp = pTemp->rbnext;
			}
			//else
			//{
			//	pTemp = pTemp->rbnext;
			//}
		}
		else
		{
			pTemp = pTemp->rbnext;
		}
	}
	pObj->close();
	return NS_SUCCESS;
}



/*
@Brief			Extended to read XData attached to sub-feature manually using Express tool
@Brief			Reads XDATA of the given 
@Param(in)		ObjectId of the object of which XDATA is to be read
@Param(out)	std::map<NSSTRING, NSSTRING>	XDATA.
@Return		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::readXDATAEx(AcDbObjectId ObjectId, std::map<NSSTRING, NSSTRING> &mapXData)
{
	//Open object for write
	AcDbObject *pObj;
	if(Acad::eOk != acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForRead))	
		return NS_FAIL;

	struct  resbuf  *pRb, *pTemp; 
	pRb = pObj->xData(NULL);


	// If xdata is present, then retrieve all key-value pairs
	for (pTemp = pRb; pTemp != NULL;)
	{
		if(pTemp->restype == AcDb::kDxfRegAppName)
		{
			NSSTRING strAppName = pTemp->resval.rstring;
			pTemp = pTemp->rbnext;

			//////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY
			if(pTemp->restype == AcDb::kDxfXdControlString)
			{
				pTemp = pTemp->rbnext;
			}
			//////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY

			if(pTemp!= NULL && pTemp->restype == AcDb::kDxfXdAsciiString)
			{
				NSSTRING strAppValue = pTemp->resval.rstring;
				mapXData.insert(std::map<NSSTRING, NSSTRING>::value_type(strAppName, strAppValue));
				pTemp = pTemp->rbnext;
			}
			//else
			//{
			//	pTemp = pTemp->rbnext;
			//}
		}
		else
		{
			pTemp = pTemp->rbnext;
		}
	}
	pObj->close();
	return NS_SUCCESS;
}





/*
@Brief			Selects blocks on deleted layer of the model_space
@Param(in)		std::vector<CNSDelBlock*>
@Return		NS_SUCCESS/NS_FAIL		
*/
int CNSDWGMgr::selectDeletedBlocks(std::vector<CNSDelBlock*> &vectDelBlocks)
{
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Rotation, Attributes and XDATA) of model space pBlockRef entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt;

	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// If the entity is a pBlockRef get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		// Select blocks on layers representing deleted features
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
		NSSTRING strLayerSuffix = _T("");
		if(strEntityLayer.size() > 4)
			strLayerSuffix = strEntityLayer.substr((strEntityLayer.size()-4), 4);
		if(NSSTRCMP(strLayerSuffix.c_str(), _T("_DEL")) != 0 )
		{
			pEnt->close();
			continue;
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;

		// we can get the name from the BlockTableRecord type
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		CNSDelBlock* pTempNSBlock = new CNSDelBlock();
		pTempNSBlock->szBlockName = pszInsertedBlockName;
		//pTempNSBlock->setRotation(pBlockRef->rotation());
		//CNSPoint3D blkPosition = CNSPoint3D(pBlockRef->position().x, pBlockRef->position().y);
		//pTempNSBlock->setInsertionPoint(blkPosition);

		//read the XDAata
		std::map<NSSTRING, NSSTRING> mapXDataRead;
		readXDATA(pEnt->id(), mapXDataRead);

		std::map<NSSTRING, NSSTRING>::iterator itrmapXDataRead;
		itrmapXDataRead = mapXDataRead.find(_T("Usage"));
		if(itrmapXDataRead != mapXDataRead.end())
		{ 
			pTempNSBlock->szUsage = (*itrmapXDataRead).second;
		}

		itrmapXDataRead = mapXDataRead.find(_T("Orientation"));
		if(itrmapXDataRead != mapXDataRead.end())
		{ 
			pTempNSBlock->szOrientation = (*itrmapXDataRead).second;
		}

		itrmapXDataRead = mapXDataRead.find(_T("FID"));
		if(itrmapXDataRead != mapXDataRead.end())
		{ 
			pTempNSBlock->FID = (*itrmapXDataRead).second;
		}

		itrmapXDataRead = mapXDataRead.find(_T("FNO"));
		if(itrmapXDataRead != mapXDataRead.end())
		{ 
			pTempNSBlock->FNO = (*itrmapXDataRead).second;
		}

		pBlockRef->close();
		pEnt->close();

		// Add the CNSBlock to the vector to be returned
		vectDelBlocks.push_back(pTempNSBlock);

	} //end model space iteration

	delete pBlkTblRcdItr;
	pRecord->close();
	pBlockTable->close();

	return NS_SUCCESS;
}



/*
@Brief			This Function finds whether a particular Attribute definition is attached to the Blockdefinition or not.
@Param(in)		NSSTRING		Attribute Definition Name
@Param(in)		AcDbObjectId	Block Id within which Attribute definition is to be searced
@return		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::blockDefHasAttributeDef(NSSTRING strAttributeDefinitionName, AcDbObjectId blockDefId)
{
	AcDbBlockTableRecord *pBlockDef = NULL;
	acdbOpenObject(pBlockDef, blockDefId, AcDb::kForRead);

	// Set up a block table record iterator to iterate over the attribute definitions.
	AcDbBlockTableRecordIterator *pAttDefIterator;
	pBlockDef->newIterator(pAttDefIterator);
	AcDbEntity *pEnt = NULL;
	AcDbAttributeDefinition *pTempAttrDef = NULL;
	std::map<NSSTRING, NSSTRING>::iterator itrMap;
	NSSTRING strValue;
	NSSTRING strAttrValue;

	for (pAttDefIterator->start(); !pAttDefIterator->done(); pAttDefIterator->step())
	{
		// Get the next entity.
		pAttDefIterator->getEntity(pEnt, AcDb::kForRead);

		// Make sure the entity is an attribute definition and is not a constant.
		pTempAttrDef = AcDbAttributeDefinition::cast(pEnt);
		if (pTempAttrDef != NULL && !pTempAttrDef->isConstant())
		{
			// We have a non-constant attribute definition

			TCHAR* pszTag = pTempAttrDef->tag();
			//NSSTRING strTag(pszTag);
			if(!(NSSTRCMP(strAttributeDefinitionName.c_str(), pszTag)))
			{
				// Return the AcDbAttributeDefinition
				//pAttdef = pTempAttrDef;

				pTempAttrDef->close();
				pBlockDef->close();
				pEnt->close();

				return NS_SUCCESS;
			}	
		}
		if(pTempAttrDef != NULL)
			pTempAttrDef->close();
		if(pEnt != NULL)
			pEnt->close();
	}
	pBlockDef->close();

	return NS_FAIL;
}

int CNSDWGMgr::setDatabase(AcDbDatabase* pDatabase)
{
	if(pDatabase == NULL) return NS_FAIL;
	m_pCurrentDatabase = pDatabase;
	return NS_SUCCESS;
}

/*
@Brief			Inserts a Ghost Block
@param(in)		CNSBlock*
@Param(in)		std::map<NSSTRING, NSSTRING>	Attributes to be added to the block to be inserted in Model space
@Return		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::insertGhostBlock(CNSBlock* blkBlock,  std::map<NSSTRING, CNSAttributeRef*> &mapOfBlkAtt)
{
	Acad::ErrorStatus es = Acad::eOk;

	// Get Object ID of the Block Definition to be insserted
	AcDbObjectId blockDefId = NULL;
	//if(NS_SUCCESS != getBlock((blkBlock->getBlockName()), blockDefId))
	if(NS_SUCCESS != getBlock(_T("Ghost"), blockDefId))
		return NS_FAIL;

	// Get layer ID of the layer on which the block is to be inserted
	TCHAR *pszLayerName = _T("UNKNOWN_FEATURES");
	AcDbObjectId LayerId = NULL;
	if(Acad::eOk != AcDbSymbolUtilities::getLayerId(LayerId, pszLayerName, m_pCurrentDatabase))
	{
		if(LayerId == NULL)
		{
			if(NS_FAIL == addNewLayer(_T("UNKNOWN_FEATURES"), LayerId))
				return NS_FAIL;
		}
	}

	// Set up the block reference to block definition retrieved through getBlock()
	AcDbBlockReference *pBlkRef = new AcDbBlockReference;
	if(Acad::eOk != pBlkRef->setBlockTableRecord(blockDefId))
	{
		return NS_FAIL;
	}

	// Set Block Position, rotation angle and layer
	CNSPoint3D pt;
	blkBlock->getInsertionpoint(pt);
	AcGePoint3d ptBase = AcGePoint3d(pt.m_dXX, pt.m_dYY, 0.00);		
	if(Acad::eOk != pBlkRef->setPosition(ptBase)
		|| (Acad::eOk != pBlkRef->setRotation(blkBlock->getRotation()))
		|| (Acad::eOk != pBlkRef->setLayer(LayerId)))
	{
		pBlkRef->close();
		return NS_FAIL;
	}

	// Open the current database's model space block table record.
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))
	{
		pBlkRef->close();
		return NS_FAIL;
	}

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite))
	{
		pBlkRef->close();
		pBlockTable->close();
		return NS_FAIL;
	}
	es = pBlockTable->close();

	// Append the block reference to the model space block table record.
	AcDbObjectId insertedBlockId;
	if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(insertedBlockId, pBlkRef))
		return NS_FAIL;

	es = pBlockTableRecord->close();

	// Now set attributes to the inserted block
	// Open the block definition for read.
	AcDbBlockTableRecord *pBlockDef;
	acdbOpenObject(pBlockDef, blockDefId, AcDb::kForRead);

	// Set up a block table record iterator to iterate over the attribute definitions.
	AcDbBlockTableRecordIterator *pAttDefIterator;
	pBlockDef->newIterator(pAttDefIterator);
	AcDbEntity *pEnt;
	AcDbAttributeDefinition *pAttdef;
	std::map<NSSTRING, CNSAttributeRef*>::iterator itrMap;
	NSSTRING pszValue;
	NSSTRING pszAttrValue;

	CNSAttributeRef *pRef =NULL;
	std::map<NSSTRING, CNSAttributeRef*>::iterator itrblkAttMap;	
	for(itrblkAttMap = mapOfBlkAtt.begin(); itrblkAttMap != mapOfBlkAtt.end(); itrblkAttMap++)
	{
		pBlockDef->close();
		pRef = (*itrblkAttMap).second;
		// Check if the ghost block has that attribute
		if((blockDefHasAttributeDef((itrblkAttMap->first), blockDefId))) // returns 0 if attribute definition is already present otherwise returns 1
		{	
			// Ghost block does not contain the mentioed attribute
			// Create a new Attribute definition for Ghost Block Definition
			addAttributeDefinition(_T("Ghost") ,(TCHAR*)(itrblkAttMap->first).c_str(), (TCHAR*)pRef->getAttributeValue().c_str());
		}
		if(Acad::eOk != acdbOpenObject(pBlockDef, blockDefId, AcDb::kForWrite))
			return NS_FAIL;
	}

	for (pAttDefIterator->start(); !pAttDefIterator->done(); pAttDefIterator->step())
	{
		// Get the next entity.
		pAttDefIterator->getEntity(pEnt, AcDb::kForRead);

		// Make sure the entity is an attribute definition and is not a constant.
		pAttdef = AcDbAttributeDefinition::cast(pEnt);
		if (pAttdef != NULL && !pAttdef->isConstant())
		{
			// We have a non-constant attribute definition, so build an attribute entity.
			AcDbAttribute *pAtt = new AcDbAttribute();

			AcGePoint3d basePoint = pAttdef->position();
			basePoint += pBlkRef->position().asVector();

			TCHAR* pszTag = pAttdef->tag();
			NSSTRING strTag(pszTag);
			//itrMap = blkAttMap.find(strTag);
			//if(itrMap != blkAttMap.end())
			//{ 
			//	pszValue = (*itrMap).second;
			//}
			//else // Atrribute not found in Map
			//{
			//	continue;
			//	//pszValue = _T("REDUNDANT_ATTRIBUTE");
			//}
			CNSAttributeRef *pAttrRef = NULL;
			getupperString(pszTag);
			if(NSSTRCMP(pszTag, _T("Z")) == 0)
			{
				char szDouble[128];
				wchar_t sz[128];
				::sprintf(szDouble,"%lf",pt.m_dZZ);
				MultiByteToWideChar(CP_ACP,	0,	szDouble ,-1,sz,sizeof(sz));
				pszValue = sz;
			}
			itrMap = mapOfBlkAtt.find(strTag);
			if(itrMap != mapOfBlkAtt.end())
			{ 
				pAttrRef = (*itrMap).second;
				pszValue = pAttrRef->getAttributeValue();
			}
			else
			{
				continue;
			}

			// Set Attribute properties and values
			if(Acad::eOk != pAtt->setPropertiesFrom(pAttdef)
				|| Acad::eOk != pAtt->setPosition(basePoint)
				|| Acad::eOk != pAtt->setHeight(pAttdef->height())
				|| Acad::eOk != pAtt->setRotation(pAttdef->rotation())
				|| Acad::eOk != pAtt->setTag(pszTag)
				|| Acad::eOk != pAtt->setFieldLength(pAttdef->fieldLength())
				|| Acad::eOk != pAtt->setTextString(pszValue.c_str()))
			{
				pAtt->close();
				delete pAttDefIterator;
				es = pBlockDef->close();
				es = pBlkRef->close();
				return NS_FAIL;
			}

			// Append Attribute
			AcDbObjectId attId;
			if(Acad::eOk != pBlkRef->appendAttribute(attId, pAtt))
			{
				pAtt->close();
				delete pAttDefIterator;
				es = pBlockDef->close();
				es = pBlkRef->close();
				return NS_FAIL;
			}
			es = pAtt->close();
		}
		if(pEnt)
			es = pEnt->close(); // use pEnt... pAttdef might be NULL
	}

	delete pAttDefIterator;
	es = pBlockDef->close();
	es = pBlkRef->close();

	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}



/*
@Brief			Adds a new attribute definition to an already existing block definition
@Param(in)		TCHAR*	Block name
@Param(in)		TCHAR*	Attribute Tag
@Param(in)		TCHAR*	Attribute Value
@Return		int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::addAttributeDefinition(TCHAR* p_szBlkname, TCHAR* p_szAttributeTag, TCHAR* p_szAttributeValue)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbObjectId blockId;
	if(NS_SUCCESS != getBlock(p_szBlkname, blockId)) // get block ID to which the entity is to be added
		return NS_FAIL;

	// open blocktable record of that block
	AcDbBlockTableRecord *pBlockDef;
	if(Acad::eOk != acdbOpenObject(pBlockDef, blockId, AcDb::kForWrite))
		return NS_FAIL;
	//create new attribute definition
	AcDbAttributeDefinition *pAttdef = new AcDbAttributeDefinition; 

	// Set the attribute definition values.
	if((Acad::eOk != pAttdef->setPosition(pBlockDef->origin())
		|| (Acad::eOk != pAttdef->setHeight(0.2))
		|| (Acad::eOk != pAttdef->setRotation(0.0)))
		|| pAttdef->setHorizontalMode(AcDb::kTextLeft)
		|| pAttdef->setVerticalMode(AcDb::kTextBase)
		|| pAttdef->setInvisible(Adesk::kTrue)
		|| pAttdef->setConstant(Adesk::kTrue)
		|| pAttdef->setTextString(p_szAttributeValue)
		|| pAttdef->setTag(p_szAttributeTag)
		|| pAttdef->setInvisible(Adesk::kTrue)
		|| pAttdef->setPreset(Adesk::kTrue))
	{
		pAttdef->close();
		pBlockDef->close();
		return NS_FAIL;
	}

	// Append the attribute definition to the block.
	AcDbObjectId entityId;
	es = pBlockDef->appendAcDbEntity(entityId, pAttdef);
	es = pAttdef->close();
	es = pBlockDef->close();

	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}

int CNSDWGMgr::clearTextLabels(std::map<NSSTRING, TEXTINFO_VECTOR*> &mapTextLabels)
{
	std::map<NSSTRING, TEXTINFO_VECTOR*>::iterator TextInfoItr;
	TEXTINFO_VECTOR* pVector = NULL;
	TEXTINFO_VECTOR::iterator theVectITr;
	for(TextInfoItr = mapTextLabels.begin(); TextInfoItr != mapTextLabels.end(); TextInfoItr++)
	{
		pVector = (*TextInfoItr).second;
		for(theVectITr = pVector->begin(); theVectITr != pVector->end(); theVectITr++)
		{
			delete (*theVectITr);
		}
		delete pVector;
		pVector = NULL;
	}
	mapTextLabels.clear();
	return NS_SUCCESS;
}

/*
@Brief			Selects all polylines in the model space
@Param(out)	std::vector<CNSFeatureContainer*>& 	vector of polylines with xdata	
@Param(in)		NSSTRING	Layer on which polylines are to be searched, default value "all"
@Param(in)		NSSTRING	Line type of which polylines are to be selected, default value "all"			
@return		int			NS_SUCCESS/NS_FAIL
*/
//int CNSDWGMgr::selectPolyLines(std::vector<CNSFeatureContainer*>& vectFeatures, NSSTRING strLayerName, NSSTRING strLineType, bool bWithCableCode)
int CNSDWGMgr::selectPolyLines(std::vector<CNSFeatureContainer*>& vectFeatures, std::vector<NSSTRING>&vectCadestrialLayers, NSSTRING strLayerName, NSSTRING strLineType, bool bWithCableCode, short nExcludeFlag)
{
	AcDbBlockTable* pBlockTable =NULL;
	AcDbBlockTableRecord *pRecord = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Get BTR Iterator
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	std::map<NSSTRING, TEXTINFO_VECTOR*> mapTextLabels;
	if(nExcludeFlag == 2 && vectCadestrialLayers.size() > 0)
	{
		selectTextLabelsOnDP(mapTextLabels, vectCadestrialLayers);
	}

	// Iterate the model-space BTR to get polylines and lines(i.e. cables)
	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			clearTextLabels(mapTextLabels);
			return NS_FAIL;
		}

		// If this function is used for selecting cables only AcDbLines are selected
		if(bWithCableCode)
		{
			if(
				!(pEnt->isKindOf(AcDbLine::desc()) 
				|| pEnt->isKindOf(AcDbArc::desc()) 
				|| pEnt->isKindOf(AcDbPolyline::desc())
				|| pEnt->isKindOf(AcDbMline::desc()) // changes for busbar suport-80 hrs change request
				)
				)
			{
				pEnt->close();
				pRecord->close();
				pBlockTable->close();
				continue;
			}
		}

		// If the entity is a polyline get its details otherwise get next enitity
		if(!(pEnt->isKindOf(AcDbPolyline::desc())
			|| pEnt->isKindOf(AcDb2dPolyline::desc())
			|| pEnt->isKindOf(AcDbLine::desc())
			|| pEnt->isKindOf(AcDbArc::desc())
			|| pEnt->isKindOf(AcDbMline::desc())) // changes for busbar suport-80 hrs change request
			)
		{
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			continue;
		}

		// Select polyLines on given layer name otherwise select polyLines on all layers
		if(NSSTRCMP(strLayerName.c_str(), _T("all")) !=0 )
		{
			NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
			if(!bWithCableCode)
			{
				int size = (int)strEntityLayer.size() - (int)strLayerName.size();
				if(size < 0)
				{
					pEnt->close();
					pRecord->close();
					pBlockTable->close();
					continue;
				}
				strEntityLayer = strEntityLayer.substr((strEntityLayer.size()-strLayerName.size()), strEntityLayer.size());
				if(NSSTRCMP(strLayerName.c_str(), strEntityLayer.c_str()) != 0)
				{		
					pEnt->close();
					pRecord->close();
					pBlockTable->close();
					continue;
				}
			}
			else
			{
				//int nIndex = (int)strEntityLayer.find_last_of(_T("_NEW"));
				int nIndex = rFindString(strEntityLayer, _T("_NEW"));
				if(nIndex == -1)
				{
					pEnt->close();
					pRecord->close();
					pBlockTable->close();;
					continue;
				}
			}
		}

		if(NSSTRCMP(strLineType.c_str(), _T("all")) !=0 )
		{
			NSSTRING strEntityLineType = NSSTRING(pEnt->linetype());
			if(strEntityLineType.size() > strLineType.size())
				strEntityLineType = strEntityLineType.substr((strEntityLineType.size()-strLineType.size()), strEntityLineType.size());

			if(NSSTRCMP(strLineType.c_str(), strEntityLineType.c_str()) != 0)
			{
				pEnt->close();
				pRecord->close();
				pBlockTable->close();
				continue;
			}
		}

		if(nExcludeFlag != 0)
		{
			NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
			std::vector<NSSTRING>::iterator theDPItr;
			if(nExcludeFlag == 1)
			{
				bool bFound = false;
				for(theDPItr = vectCadestrialLayers.begin(); theDPItr != vectCadestrialLayers.end(); theDPItr++)
				{
					if(NSSTRCMP((*theDPItr).c_str(), strEntityLayer.c_str()) ==0 )
					{
						pEnt->close();
						pRecord->close();
						pBlockTable->close();
						bFound = true;
						break;
					}
				}
				if(bFound == true)
					continue;
			}
			else
			{
				for(theDPItr = vectCadestrialLayers.begin(); theDPItr != vectCadestrialLayers.end(); theDPItr++)
				{
					if(NSSTRCMP((*theDPItr).c_str(), strEntityLayer.c_str()) ==0 )
					{
						break;
					}
				}
				if(theDPItr == vectCadestrialLayers.end())
				{
					pEnt->close();
					pRecord->close();
					pBlockTable->close();
					continue;
				}
			}
		}
		CNSFeatureContainer *pFeatContainer = new CNSFeatureContainer;
		CNSCompositeContainer *pContainer = new CNSCompositeContainer;
		CNSCompositeGeometry *pTempPolyLine = new CNSCompositeGeometry;  // stores vector of CNSCompositeData
		pContainer->add(pTempPolyLine);
		pFeatContainer->m_pContainer = pContainer;
		std::map<NSSTRING, NSSTRING>::iterator XdataIter;
		NSSTRING strValue;
		pFeatContainer->m_mapOfXData = new STRING_MAP;
		readXDATA(pEnt->id(), *pFeatContainer->m_mapOfXData);

		int nRes = NS_FAIL;
		if(pEnt->isKindOf(AcDbPolyline::desc()))
			//if(!bWithCableCode && (pEnt->isKindOf(AcDb2dPolyline::desc())))
		{	
			nRes = getPolyLineSegments (pEnt, pTempPolyLine, pFeatContainer);
		}
		if(bWithCableCode && (pEnt->isKindOf(AcDbLine::desc())))
		{
			nRes = getLineSegments(pEnt, pTempPolyLine,  pFeatContainer);
		}
		if(bWithCableCode && (pEnt->isKindOf(AcDbArc::desc())))
		{
			nRes = getArcSegments(pEnt, pTempPolyLine,  pFeatContainer);
		}
		// changes for busbar suport-80 hrs change request start
		if(bWithCableCode && (pEnt->isKindOf(AcDbMline::desc())))
		{
			nRes = getMultiLineSegments(pEnt, pTempPolyLine,  pFeatContainer);
		}
		// changes for busbar suport-80 hrs change request end
		if(nRes != NS_SUCCESS)
		{
			//	delete pTempPolyLine;
			delete pContainer;
			delete pFeatContainer;
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			clearTextLabels(mapTextLabels);
			return NS_FAIL;
		}

		// Set line type and layer name
		pTempPolyLine->setLayername((TCHAR*)pEnt->layer());
		pTempPolyLine->setName((TCHAR*)pEnt->linetype());
		pFeatContainer->setLayerName((TCHAR*)pEnt->layer());
		//if(!(pEnt->isKindOf(AcDbMline::desc()))
		pFeatContainer->setTypeName((TCHAR*)pEnt->linetype());
		//else
		//pFeatContainer->setTypeName((TCHAR*)pEnt->mlst);

		//read the XDAata
		//	std::map<NSSTRING, NSSTRING> mapXDataRead;
		XdataIter = pFeatContainer->m_mapOfXData->find(_T("CNO"));
		if(XdataIter != pFeatContainer->m_mapOfXData->end())
			pTempPolyLine->setCNO(NSATOI(((*XdataIter).second).c_str()));
		XdataIter = pFeatContainer->m_mapOfXData->find(_T("CID"));
		if(XdataIter != pFeatContainer->m_mapOfXData->end())
			pTempPolyLine->setCID(NSATOI(((*XdataIter).second).c_str()));
		else
			pTempPolyLine->setCID(1);

		pFeatContainer->m_mapOfBlkAtt = new ATT_MAP;

		if(nExcludeFlag == 2)
		{
			if(pEnt->isKindOf(AcDbPolyline::desc()))   
			{
				AcDbPolyline *pPolyline = (AcDbPolyline*) pEnt ;
				if(pPolyline && pPolyline->isClosed())
				{
					AcGePoint3d vertexPoint;
					int nVertices = pPolyline->numVerts();
					double *pXList = new double[nVertices];
					double *pYList = new double[nVertices];
					int nVertCntr = 0;
					for(; nVertCntr < nVertices; nVertCntr++)
					{
						pPolyline->getPointAt(nVertCntr, vertexPoint);
						pXList[nVertCntr] = vertexPoint.x;
						pYList[nVertCntr] = vertexPoint.y;
					}
					NSSTRING szEntityLayer = (TCHAR*)pEnt->layer();
#ifndef _INTERNAL_
					NSSTRUPR((TCHAR*)szEntityLayer.c_str(), szEntityLayer.size()+1); 
#else
					NSSTRUPR((TCHAR*)szEntityLayer.c_str(), szEntityLayer.size()+1); 
					// NSSTRUPR((TCHAR*)szEntityLayer.c_str());
#endif
					std::map<NSSTRING, TEXTINFO_VECTOR*>::iterator TextInfoItr;
					TextInfoItr = mapTextLabels.find(szEntityLayer);
					if(TextInfoItr != mapTextLabels.end())
					{
						TEXTINFO_VECTOR* pVector = (*TextInfoItr).second;
						TEXTINFO_VECTOR::iterator VectItr;
						CNSTextInfo *pInfo = NULL;
						for(VectItr = pVector->begin(); VectItr != pVector->end(); VectItr++)
						{
							pInfo = (*VectItr);
							if(1 == isPointInsidePolyLine(nVertices, pXList, pYList,pInfo->m_InsertionPt.m_dXX,pInfo->m_InsertionPt.m_dYY))
							{
								// pFeatContainer->m_mapOfXData->insert(STRING_MAP::value_type(_T("TEXT_LABEL"), pInfo->m_strText));
								CNSAttributeRef *pRef = new CNSAttributeRef;
								pRef->setAttributeValue(pInfo->m_strText);
								CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;
								CNSBlock *pBlock = new CNSBlock;
								pGeometry->add(pBlock);
								//   AcGePoint3d InsertionPoint = pInfo->->position();
								pBlock->setInsertionPoint(pInfo->m_InsertionPt.m_dXX, pInfo->m_InsertionPt.m_dYY, 0.0);
								pBlock->setRotation(pInfo->m_dRotation);
								pBlock->setAlignment(pInfo->m_nAlignment);
								pGeometry->setCompType(COMP_TEXTPOINT);
								pRef->setCompGeometry(pGeometry);
								pFeatContainer->addAttributeRef(_T("LOT_NUM"), 1, pRef);
								break;
							}
						}
					}
				}
			}
		}   

		pEnt->close();

		// Add the CNSBlock to the vector to be returned		
		vectFeatures.push_back(pFeatContainer);

	} //end model space iteration

	delete pBlkTblRcdItr;
	pRecord->close();
	pBlockTable->close();
	clearTextLabels(mapTextLabels);
	return NS_SUCCESS;
}
/*
@Brief			selects Text labels on layer "DEVELOPER_PLAN"
@Param(out)	std::vector<CNSBlock*>
@return		int		NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::selectTextLabelsOnDP(std::map<NSSTRING, TEXTINFO_VECTOR*> &mapTextLabels, std::vector<NSSTRING> szCadastreLayers)
{
	AcDbBlockTable* pBlockTable =NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space block table record
	// Copy the contents(Text String, Insertion point, Rotation) of model space Text entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		AcDbEntity *pEnt;
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// If the entity is a Text get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbText::desc()))
		{
			pEnt->close();
			continue;		
		}

		NSSTRING szEntityLayer = NSSTRING(pEnt->layer());
		// Select blocks on layer DEVELOPER_PLAN
		std::vector<NSSTRING>::iterator szLayerItr;
		bool bLayerMatched = false;
		for(szLayerItr = szCadastreLayers.begin();szLayerItr !=szCadastreLayers.end();szLayerItr++)
		{
			NSSTRING strLayerName = *(szLayerItr);
			if(NSSTRCMP(szEntityLayer.c_str(), strLayerName.c_str()) == 0)
			{
				bLayerMatched = true;
				break;		
			}
		}
		if(!bLayerMatched)
		{
			pEnt->close();
			continue;
		}

		CNSTextInfo *pInfo = new CNSTextInfo;
		AcDbText* pText = (AcDbText*)pEnt;
		pInfo->m_strText = pText->textString();
		pInfo->m_dRotation =  pText->rotation();
		pInfo->m_nAlignment = getAlignment(pText);
		pInfo->m_InsertionPt = CNSPoint3D(pText->position().x, pText->position().y);

		std::map<NSSTRING, TEXTINFO_VECTOR*>::iterator theTextItr;
		TEXTINFO_VECTOR *pVectorInfo  = NULL;
#ifndef _INTERNAL_
		NSSTRUPR((TCHAR*)szEntityLayer.c_str(), szEntityLayer.size()+1); 
#else
		NSSTRUPR((TCHAR*)szEntityLayer.c_str(), szEntityLayer.size()+1); 
		//NSSTRUPR((TCHAR*)szEntityLayer.c_str());
#endif

		theTextItr = mapTextLabels.find(szEntityLayer);
		if(theTextItr != mapTextLabels.end())
		{
			pVectorInfo = (*theTextItr).second;
			pVectorInfo->push_back(pInfo);
		}
		else
		{
			pVectorInfo = new TEXTINFO_VECTOR;
			pVectorInfo->push_back(pInfo);
			mapTextLabels.insert(std::map<NSSTRING, TEXTINFO_VECTOR*>::value_type(szEntityLayer.c_str(),pVectorInfo));
		}

		if(pEnt != NULL)
			pEnt->close();		
		if(pText != NULL)		
			pText->close();

	} //end model space iteration

	delete pBlkTblRcdItr;
	pRecord->close();
	pBlockTable->close();	

	return NS_SUCCESS;
}

/*
@Brief			Gets a polyline coordinates in a string
consecutive linear segments of a polyline form a single CNSCompositeData entity
Common point between two consecutive linear segments is added to the string only once.
Each arc segment forms a different CNSCompositeData entity
@Param(in)		AcDbEntity
@Param(out)	CNSCompositeGeometry - CNSCompositeData entities form one CNSCompositeGeometry which represents a complete polyline
@Return		int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getPolyLineSegments(AcDbEntity *pPolyLineEntity, CNSCompositeGeometry *pTempPolyLine,CNSFeatureContainer *pFeatContainer)
{
	GEO_ELEMENT prevSegmentType = ELE_INVALID;
	CNSCompositeData *pTempEnt = NULL;
	NSSTRING strLineSegment;
	double tempdZ = 0.0;
	// Iterate polyline segments
	AcDbPolyline *pPolyLine = (AcDbPolyline*)pPolyLineEntity;
	int nPolyLineVertices = pPolyLine->numVerts();
	for(int iCnt=0; iCnt != (nPolyLineVertices-1); iCnt++)
	{		
		double fBulge = 0.0;
		if(Acad::eOk != pPolyLine->getBulgeAt(iCnt, fBulge))
		{	
			delete pTempEnt;
			return NS_FAIL;
		}

		if(fBulge == 0.0)	// this is a line
		{
			AcGeLineSeg2d line2D;
			TCHAR szVertex[MAX_PATH+1];
			if(Acad::eOk != pPolyLine->getLineSegAt(iCnt, line2D))
			{	
				delete pTempEnt;
				return NS_FAIL;
			}

			// If this is first segment or previous segment was an arc create a new CNSCompositeData object
			// All consecutive line segments form one CNSCompositeData object
			if(prevSegmentType == ELE_INVALID || prevSegmentType == ELE_CUVRVE) 
			{
				//Create a new line segment
				pTempEnt = new CNSCompositeData;				
				tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));
				NSSPRINTF(szVertex, _T("%lf %lf %lf "), line2D.startPoint().x, line2D.startPoint().y, tempdZ);
				strLineSegment.append(szVertex);
			}
			tempdZ = getZvalue(pFeatContainer, _T("NEXT_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), line2D.endPoint().x, line2D.endPoint().y, tempdZ);
			strLineSegment.append(szVertex);

			// If Next Segment is an arc or if this line segment is last segment of the polyline add it to compositedata
			double fNextBulge = 0.0;
			if(Acad::eOk != pPolyLine->getBulgeAt((iCnt+1), fNextBulge))
			{
				delete pTempEnt;
				return NS_FAIL;
			}
			if(fNextBulge != 0 || (iCnt + 1) == (nPolyLineVertices - 1))
			{
				pTempEnt->setData(strLineSegment, ELE_LINEAR);
				pTempPolyLine->add(pTempEnt);
				strLineSegment = _T("");
			}

			prevSegmentType = ELE_LINEAR;
		}
		else	// this is a arc
		{
			AcGeCircArc2d* arc2D = new AcGeCircArc2d();
			if(Acad::eOk != pPolyLine->getArcSegAt(iCnt, *arc2D))
			{
				return NS_FAIL;
			}
			AcGePoint2d startPoint, intermediatePoint, endPoint;
			startPoint = arc2D->startPoint();
			endPoint = arc2D->endPoint();

			//Calculating point on curve
			AcGePoint2dArray unnamed;
			unnamed.setLogicalLength(3);
			arc2D->getSamplePoints(3, unnamed);
			intermediatePoint = unnamed.at(1);			
			delete arc2D;

			NSSTRING strArcSegment;
			TCHAR szVertex[MAX_PATH+1];
			tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), startPoint.x, startPoint.y, tempdZ);
			strArcSegment.append(szVertex);
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), intermediatePoint.x, intermediatePoint.y, tempdZ);
			strArcSegment.append(szVertex);	
			tempdZ = getZvalue(pFeatContainer, _T("NEXT_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), endPoint.x, endPoint.y, tempdZ);
			strArcSegment.append(szVertex);			

			// Add this segment to pTempPolyLine
			CNSCompositeData *pTempEnt = new CNSCompositeData;
			pTempEnt->setData(strArcSegment, ELE_CUVRVE);
			pTempPolyLine->add(pTempEnt);

			strLineSegment = _T("");
			prevSegmentType = ELE_CUVRVE;
		}
	}
	return NS_SUCCESS;
}

int CNSDWGMgr::getPolyLineSegmentsEx(AcDbEntity *pPolyLineEntity, CNSCompositeGeometry *pTempPolyLine,  double sx, double sy, double sz,double ParentRotRad, double ParPosX, double ParPosY, CNSFeatureContainer *pSubFeatContainer)
{
	GEO_ELEMENT prevSegmentType = ELE_INVALID;
	CNSCompositeData *pTempEnt = NULL;
	NSSTRING strLineSegment;
	double tempdZ = 0.0;
	// Iterate polyline segments
	AcDbPolyline *pPolyLine = (AcDbPolyline*)pPolyLineEntity;
	int nPolyLineVertices = pPolyLine->numVerts();
	for(int iCnt=0; iCnt != (nPolyLineVertices-1); iCnt++)
	{		
		double fBulge = 0.0;
		if(Acad::eOk != pPolyLine->getBulgeAt(iCnt, fBulge))
		{	
			delete pTempEnt;
			return NS_FAIL;
		}

		if(fBulge == 0.0)	// this is a line
		{
			AcGeLineSeg2d line2D;
			TCHAR szVertex[MAX_PATH+1];
			if(Acad::eOk != pPolyLine->getLineSegAt(iCnt, line2D))
			{	
				delete pTempEnt;
				return NS_FAIL;
			}

			// If this is first segment or previous segment was an arc create a new CNSCompositeData object
			// All consecutive line segments form one CNSCompositeData object
			if(prevSegmentType == ELE_INVALID || prevSegmentType == ELE_CUVRVE) 
			{
				//Create a new line segment
				pTempEnt = new CNSCompositeData;				

				double Sx = line2D.startPoint().x;	double Sy = line2D.startPoint().y;
				normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
				tempdZ = getZvalue(pSubFeatContainer, _T("START_VERTEX"));
				NSSPRINTF(szVertex, _T("%lf %lf %lf "),Sx ,Sy, tempdZ);
				strLineSegment.append(szVertex);
			}

			double Ex = line2D.endPoint().x;	double Ey = line2D.endPoint().y;
			normalizePoint(Ex,Ey,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
			tempdZ = getZvalue(pSubFeatContainer, _T("NEXT_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), Ex, Ey, tempdZ);
			strLineSegment.append(szVertex);

			// If Next Segment is an arc or if this line segment is last segment of the polyline add it to compositedata
			double fNextBulge = 0.0;
			if(Acad::eOk != pPolyLine->getBulgeAt((iCnt+1), fNextBulge))
			{
				delete pTempEnt;
				return NS_FAIL;
			}
			if(fNextBulge != 0 || (iCnt + 1) == (nPolyLineVertices - 1))
			{
				pTempEnt->setData(strLineSegment, ELE_LINEAR);
				pTempPolyLine->add(pTempEnt);
				strLineSegment = _T("");
			}

			prevSegmentType = ELE_LINEAR;
		}
		else	// this is a arc
		{
			AcGeCircArc2d* arc2D = new AcGeCircArc2d();
			if(Acad::eOk != pPolyLine->getArcSegAt(iCnt, *arc2D))
			{
				return NS_FAIL;
			}
			AcGePoint2d startPoint, intermediatePoint, endPoint;
			startPoint = arc2D->startPoint();
			endPoint = arc2D->endPoint();

			//Calculating point on curve
			AcGePoint2dArray unnamed;
			unnamed.setLogicalLength(3);
			arc2D->getSamplePoints(3, unnamed);
			intermediatePoint = unnamed.at(1);			
			delete arc2D;

			NSSTRING strArcSegment;
			TCHAR szVertex[MAX_PATH+1];

			double Sx = startPoint.x;	double Sy = startPoint.y;
			normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 

			double Ix = intermediatePoint.x;	double Iy = intermediatePoint.y;
			normalizePoint(Ix,Iy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 

			double Ex = endPoint.x;	double Ey =  endPoint.y;
			normalizePoint(Ex,Ey,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
			//tempdZ = getZvalue(pSubFeatContainer, _T("START_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), Sx, Sy, tempdZ);
			strArcSegment.append(szVertex);
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), Ix, Iy, tempdZ);
			strArcSegment.append(szVertex);	
			//tempdZ = getZvalue(pSubFeatContainer, _T("NEXT_VERTEX"));
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), Ex, Ey, tempdZ);
			strArcSegment.append(szVertex);			

			// Add this segment to pTempPolyLine
			CNSCompositeData *pTempEnt = new CNSCompositeData;
			pTempEnt->setData(strArcSegment, ELE_CUVRVE);
			pTempPolyLine->add(pTempEnt);

			strLineSegment = _T("");
			prevSegmentType = ELE_CUVRVE;
		}
	}
	return NS_SUCCESS;
}


/*

@Brief			Gets a MultiLine coordinates in a string
@Param(in)		AcDbEntity
@Param(out)	CNSCompositeGeometry 
@Return		int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getMultiLineSegments(AcDbEntity *pMLineEntity, CNSCompositeGeometry *pTempMLine, CNSFeatureContainer *pFeatContainer)
{
	if(!(pMLineEntity->isKindOf(AcDbMline::desc())))
	{
		return NS_FAIL;
	}
	AcDbMline *pMLine = (AcDbMline*) pMLineEntity;

	CNSCompositeData *pTempEnt = NULL; 

	int nNumVert = pMLine->numVertices();
	NSSTRING strLineSegment(_T(""));
	double tempdZ = 0.0;

	for(int iNum=0;iNum<nNumVert-1;iNum++) 
	{   
		TCHAR szVertex[MAX_PATH+1];	
		if(pTempEnt == NULL)
		{
			tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));
			pTempEnt = new CNSCompositeData;
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), pMLine->vertexAt(iNum).x, pMLine->vertexAt(iNum).y, tempdZ);	
			strLineSegment.append(szVertex);
		}
		tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));
		NSSPRINTF(szVertex, _T("%lf %lf %lf "), pMLine->vertexAt(iNum+1).x, pMLine->vertexAt(iNum+1).y, tempdZ);	
		strLineSegment.append(szVertex);
		if(iNum+1 ==nNumVert-1)   
		{
			pTempEnt->setData(strLineSegment, ELE_LINEAR);
			pTempMLine->add(pTempEnt);
			strLineSegment = _T("");
		}
	}

	return NS_SUCCESS;
}

int CNSDWGMgr::getMultiLineSegmentsEx(AcDbEntity *pMLineEntity, CNSCompositeGeometry *pTempMLine,  double sx, double sy, double sz,double ParentRotRad, double ParPosX, double ParPosY, CNSFeatureContainer *pSubFeatContainer)
{
	if(!(pMLineEntity->isKindOf(AcDbMline::desc())))
	{
		return NS_FAIL;
	}
	AcDbMline *pMLine = (AcDbMline*) pMLineEntity;

	CNSCompositeData *pTempEnt = NULL; 

	int nNumVert = pMLine->numVertices();
	NSSTRING strLineSegment(_T(""));
	double tempdZ = 0.0;
	for(int iNum=0;iNum<nNumVert-1;iNum++) 
	{   
		TCHAR szVertex[MAX_PATH+1];	
		if(pTempEnt == NULL)
		{
			double Sx = pMLine->vertexAt(iNum).x;	double Sy = pMLine->vertexAt(iNum).y;
			normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
			tempdZ = getZvalue(pSubFeatContainer, _T("START_VERTEX"));
			pTempEnt = new CNSCompositeData;
			NSSPRINTF(szVertex, _T("%lf %lf %lf "), Sx, Sy, tempdZ);	
			strLineSegment.append(szVertex);
		}

		double Sx = pMLine->vertexAt(iNum+1).x;	double Sy = pMLine->vertexAt(iNum+1).y;
		normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
		tempdZ = getZvalue(pSubFeatContainer, _T("NEXT_VERTEX"));
		NSSPRINTF(szVertex, _T("%lf %lf %lf "),Sx , Sy, tempdZ);	
		strLineSegment.append(szVertex);
		if(iNum+1 ==nNumVert-1)   
		{
			pTempEnt->setData(strLineSegment, ELE_LINEAR);
			pTempMLine->add(pTempEnt);
			strLineSegment = _T("");
		}
	}

	return NS_SUCCESS;
}


/*
@Brief			Gets a Line coordinates in a string
@Param(in)		AcDbEntity
@Param(out)	CNSCompositeGeometry 
@Return		int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getLineSegments(AcDbEntity *pLineEntity, CNSCompositeGeometry *pTempPolyLine, CNSFeatureContainer *pFeatContainer)
{
	if(!(pLineEntity->isKindOf(AcDbLine::desc())))
	{
		return NS_FAIL;
	}
	AcDbLine *pLine = (AcDbLine*) pLineEntity;
	double tempdZ = 0;
	NSSTRING strLineSegment(_T(""));
	TCHAR szVertex[MAX_PATH+1];	
	tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));
	NSSPRINTF(szVertex, _T("%lf %lf %lf "), pLine->startPoint().x, pLine->startPoint().y, tempdZ);	
	strLineSegment.append(szVertex);
	tempdZ = getZvalue(pFeatContainer, _T("NEXT_VERTEX"));
	NSSPRINTF(szVertex, _T("%lf %lf %lf "), pLine->endPoint().x, pLine->endPoint().y, tempdZ);	
	strLineSegment.append(szVertex);

	CNSCompositeData *pTempEnt = new CNSCompositeData;
	pTempEnt->setData(strLineSegment, ELE_LINEAR);
	pTempPolyLine->add(pTempEnt);

	return NS_SUCCESS;
}


int CNSDWGMgr::getLineSegmentsEx(AcDbEntity *pLineEntity, CNSCompositeGeometry *pTempPolyLine,  double sx, double sy, double sz,double ParentRotRad, double ParPosX, double ParPosY,  CNSFeatureContainer *pSubFeatContainer)
{
	if(!(pLineEntity->isKindOf(AcDbLine::desc())))
	{
		return NS_FAIL;
	}
	AcDbLine *pLine = (AcDbLine*) pLineEntity;

	NSSTRING strLineSegment(_T(""));
	TCHAR szVertex[MAX_PATH+1];	
	double tempdZ = 0.0;
	double Sx = pLine->startPoint().x;	double Sy = pLine->startPoint().y;
	normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 

	double Ex = pLine->endPoint().x;	double Ey = pLine->endPoint().y;
	normalizePoint(Ex,Ey,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 
	tempdZ = getZvalue(pSubFeatContainer, _T("START_VERTEX"));
	NSSPRINTF(szVertex, _T("%lf %lf %lf "), Sx, Sy, tempdZ);	
	strLineSegment.append(szVertex);
	tempdZ = getZvalue(pSubFeatContainer, _T("NEXT_VERTEX"));
	NSSPRINTF(szVertex, _T("%lf %lf %lf "), Ex, Ey, tempdZ);	
	strLineSegment.append(szVertex);

	CNSCompositeData *pTempEnt = new CNSCompositeData;
	pTempEnt->setData(strLineSegment, ELE_LINEAR);
	pTempPolyLine->add(pTempEnt);

	return NS_SUCCESS;
}


/*
@Brief			Gets a Arc coordinates in a string
@Param(in)		AcDbEntity
@Param(out)	CNSCompositeGeometry 
@Return		int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getArcSegments(AcDbEntity *pArcEntity, CNSCompositeGeometry *pTempPolyLine, CNSFeatureContainer *pFeatContainer)
{
	AcDbArc *pArc = (AcDbArc*) pArcEntity;
	AcGePoint3d startPt, centerPt, endPt;
	pArc->getStartPoint(startPt);
	centerPt = pArc->center();
	pArc->getEndPoint(endPt);


	///Arc midpoint on the curve
	AcGeCircArc3d* arc3D = new AcGeCircArc3d;
	arc3D->setCenter(centerPt);
	arc3D->setRadius(pArc->radius());
	arc3D->setAngles(pArc->startAngle(), pArc->endAngle());

	AcGePoint3d intermediatePoint;
	////Calculating point on curve
	AcGePoint3dArray unnamed;
	unnamed.setLogicalLength(3);
	arc3D->getSamplePoints(3, unnamed);
	intermediatePoint = unnamed.at(1);			
	delete arc3D;

	double tempdZ = getZvalue(pFeatContainer, _T("START_VERTEX"));  //call to get Z value from Xdata
	NSSTRING strArcSegment(_T(""));
	TCHAR szVertex[MAX_PATH+1];	
	//	NSSPRINTF(szVertex, _T("%lf %lf %lf %lf %lf %lf"), startPt.x, startPt.y, centerPt.x, centerPt.y, endPt.x, endPt.y);	
	NSSPRINTF(szVertex, _T("%lf %lf %lf %lf %lf %lf %lf %lf %lf "), startPt.x, startPt.y, tempdZ, intermediatePoint.x, intermediatePoint.y, tempdZ, endPt.x, endPt.y, tempdZ);	
	strArcSegment.append(szVertex);

	CNSCompositeData *pTempEnt = new CNSCompositeData;
	pTempEnt->setData(strArcSegment, ELE_CURVE_MEMEBR);
	pTempPolyLine->add(pTempEnt);

	return NS_SUCCESS;
}


int CNSDWGMgr::getArcSegmentsEx(AcDbEntity *pArcEntity, CNSCompositeGeometry *pTempPolyLine,  double sx, double sy, double sz,double ParentRotRad, double ParPosX, double ParPosY, CNSFeatureContainer *pSubFeatContainer)
{
	AcDbArc *pArc = (AcDbArc*) pArcEntity;
	AcGePoint3d startPt, centerPt, endPt;
	pArc->getStartPoint(startPt);
	centerPt = pArc->center();
	pArc->getEndPoint(endPt);


	///Arc midpoint on the curve
	AcGeCircArc3d* arc3D = new AcGeCircArc3d;
	arc3D->setCenter(centerPt);
	arc3D->setRadius(pArc->radius());
	arc3D->setAngles(pArc->startAngle(), pArc->endAngle());

	AcGePoint3d intermediatePoint;
	////Calculating point on curve
	AcGePoint3dArray unnamed;
	unnamed.setLogicalLength(3);
	arc3D->getSamplePoints(3, unnamed);
	intermediatePoint = unnamed.at(1);			
	delete arc3D;



	double Sx = startPt.x;	double Sy =  startPt.y;
	normalizePoint(Sx,Sy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 

	double Ix = intermediatePoint.x;	double Iy = intermediatePoint.y;
	normalizePoint(Ix,Iy,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 

	double Ex = endPt.x;	double Ey =  endPt.y;
	normalizePoint(Ex,Ey,sx,sy,sz,ParentRotRad, ParPosX, ParPosY); 


	NSSTRING strArcSegment(_T(""));
	TCHAR szVertex[MAX_PATH+1];	
	double tempdZ = 0.0;
	tempdZ = getZvalue(pSubFeatContainer, _T("START_VERTEX"));  //call to get Z value from Xdata
	//	NSSPRINTF(szVertex, _T("%lf %lf %lf %lf %lf %lf"), startPt.x, startPt.y, centerPt.x, centerPt.y, endPt.x, endPt.y);	
	NSSPRINTF(szVertex, _T("%lf %lf %lf %lf %lf %lf %lf %lf %lf "), Sx, Sy, tempdZ, Ix, Iy, tempdZ, Ex, Ey, tempdZ);	
	strArcSegment.append(szVertex);

	CNSCompositeData *pTempEnt = new CNSCompositeData;
	pTempEnt->setData(strArcSegment, ELE_CURVE_MEMEBR);
	pTempPolyLine->add(pTempEnt);

	return NS_SUCCESS;
}



/*
@Brief			Adds a linear segment to the polyline
@Param(out)	AcDbPolyline
@Param(in)		NSSTRING		String representing linear segment's vertices
@return		int		NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::addPolyLineArcSegment(AcDbPolyline *pPolyLine, NSSTRING strArcPoints)
{
	std::vector<CNSPoint3D> vectPoints;
	convertString2Vertices(strArcPoints, vectPoints);
	//If the the string does not contain exactly three points
	if(vectPoints.size() != 3)
		return NS_FAIL;

	// Store the three CNSPoint3D in a AcGePoint3dArray
	AcGePoint2dArray ptArr;
	ptArr.setLogicalLength(3);

	CNSPoint3D arcPoint;
	std::vector<CNSPoint3D>::iterator PointItr;
	int nPointCntr = 0;
	for(PointItr = vectPoints.begin(); nPointCntr <= 2; PointItr++)
	{
		arcPoint = *PointItr;
		ptArr[nPointCntr++].set(arcPoint.m_dXX, arcPoint.m_dYY);
	}

	AcGeCircArc2d* arc2D = new AcGeCircArc2d();
	AcGeError error;
	arc2D->set(ptArr.at(0),ptArr.at(1),ptArr.at(2),error);

	double fStAngle=0.0;
	double fStartAngle  = arc2D->startAng();
	double fEndAngle = arc2D->endAng();
	int nClockWise = arc2D->isClockWise(); // 
	double fBulge=0.0;
	if(fStartAngle > fEndAngle)
	{
		fStAngle = fStartAngle - (8 * atan(1.0f)) ;
	}
	else
	{
		fStAngle=fStartAngle;
	}
	fBulge = tan((fEndAngle - fStAngle) / 4);

	if(Acad::eOk != pPolyLine->addVertexAt(pPolyLine->numVerts(), ptArr.at(0), (nClockWise ? -fBulge : fBulge)))
	{
#ifndef _INTERNAL_
		ptArr.removeAll();
#endif
		return NS_FAIL;
	}
	pPolyLine->addVertexAt(pPolyLine->numVerts(), ptArr.at(2));
	{
#ifndef _INTERNAL_
		ptArr.removeAll();
#endif
		return NS_FAIL;
	}

#ifndef _INTERNAL_
	ptArr.removeAll();
#endif

	/*if(bClosed)
	pPolyLine->setClosed(bClosed);*/

	return NS_SUCCESS;
}



/*
@Brief			Adds a linear segment to the polyline
@Param(out)	AcDbPolyline
@Param(in)		NSSTRING		String representing linear segment's vertices
@return		int				NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::addPolyLineLinearSegment(AcDbPolyline *pPolyLine, NSSTRING strVertexPoints)
{
	std::vector<CNSPoint3D> vectPoints;
	if(NS_SUCCESS != convertString2Vertices(strVertexPoints, vectPoints))
		NS_FAIL;

	//Get vertex points from vector in an AcGePoint2dArray
	AcGePoint2dArray ptArr;
	ptArr.setLogicalLength((int)vectPoints.size());

	CNSPoint3D vertex;
	int nPointCntr = 0;
	std::vector<CNSPoint3D>::iterator PointItr;
	for(PointItr = vectPoints.begin(); PointItr != vectPoints.end(); PointItr++)
	{
		vertex = *PointItr;
		ptArr[nPointCntr++].set(vertex.m_dXX, vertex.m_dYY);
	}

	//Add vertices to Linear segment of a poly line	
	for(int nVertexCnt = 0; nVertexCnt < ptArr.length(); nVertexCnt++)
	{
		if(Acad::eOk != pPolyLine->addVertexAt(pPolyLine->numVerts(), ptArr.at(nVertexCnt)))
		{
#ifndef _INTERNAL_
			ptArr.removeAll();
#endif
			return NS_FAIL;
		}
	}

	/*if(bClosed)
	pPolyLine->setClosed(bClosed);*/

#ifndef _INTERNAL_
	ptArr.removeAll();
#endif
	return NS_SUCCESS;
}

/*
@Brief			Adds a new point to the multiline
@Param(out)	AcDbPolyline
@Param(in)		NSSTRING		String representing linear segment's vertices
@return		int				NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::addMLineSegment(AcDbMline *pMLine, NSSTRING strVertexPoints)
{
	std::vector<CNSPoint3D> vectPoints;
	if(NS_SUCCESS != convertString2Vertices(strVertexPoints, vectPoints))
		NS_FAIL;

	//Get vertex points from vector in an AcGePoint2dArray
	AcGePoint3dArray ptArr;
	ptArr.setLogicalLength((int)vectPoints.size());

	CNSPoint3D vertex;
	int nPointCntr = 0;
	std::vector<CNSPoint3D>::iterator PointItr;
	for(PointItr = vectPoints.begin(); PointItr != vectPoints.end(); PointItr++)
	{
		vertex = *PointItr;
		ptArr[nPointCntr++].set(vertex.m_dXX, vertex.m_dYY,0.0);
	}

	//Add vertices to Linear segment of a poly line	
	for(int nVertexCnt = 0; nVertexCnt < ptArr.length(); nVertexCnt++)
	{
		if(Acad::eOk != pMLine->appendSeg(ptArr.at(nVertexCnt)))
		{
#ifndef _INTERNAL_
			ptArr.removeAll();
#endif
			return NS_FAIL;
		}
	}

	/*if(bClosed)
	pPolyLine->setClosed(bClosed);*/

#ifndef _INTERNAL_
	ptArr.removeAll();
#endif
	return NS_SUCCESS;
}


/*
@Brief			Sets the precision of the decimal numbers to be handled in the database
@Param(in)		int		decimal numbers upto which precision is required
@return		int		NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::setDatabasePrecision(int nPrecision)
{
	if(Acad::eOk != m_pCurrentDatabase->setLuprec(nPrecision))
	{
		return NS_FAIL;
	}
	return NS_SUCCESS;
}

//************************************************* Commented on 02-Feb-2014 *********************************
/*
@Brief			Inserts an MPolygon entity
@Param(in)		CNSCompositeGeometry*		contains MPolygon geometry
Each pCompositeData in CNSCompositeGeometry represents an MPolygon loop element
@Param(in)		NSSTRING					LayerName
@Param(in)		STRING_MAP					Map of XDATA
*/
////int CNSDWGMgr::drawMPolygon(CNSCompositeGeometry *pCompositeGeo, NSSTRING strLineType, NSSTRING strLayerName, STRING_MAP &mapOfXData)
////{
////	if (!acrxLoadModule(_T("AcMPolygonObj17.dbx"), false))
////		return NS_FAIL;
////
////	CNSCompositeData* pCompositeData = NULL;
////	AcDbMPolygon* pMPolygon = NULL;
////
////	int nGeoSize = pCompositeGeo->getPoslistSize();
////	for(int nGeocntr = 0; nGeocntr<nGeoSize; nGeocntr++) 
////	{
////		pCompositeData = pCompositeGeo->getAt(nGeocntr);
////		NSSTRING strVertexPoints;
////		GEO_ELEMENT GL;
////		pCompositeData->getData(strVertexPoints, GL);
////
////		if(GL == ELE_LINEAR)
////		{
////			if(nGeocntr == 0)
////			{
////				pMPolygon = new AcDbMPolygon();
////			}
////			AcDbPolyline *pPolyLine = new AcDbPolyline();
////			if(NS_SUCCESS != addPolyLineLinearSegment(pPolyLine, strVertexPoints))
////			{
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////
////			// If polyline is not closed it gives error while adding it to pMPolygon
////			pPolyLine->setClosed(Adesk::kTrue);
////
////			// Add  polyline(loop element) to the database
////			AcDbObjectId idLoopElement;
////			if(NS_SUCCESS != postToDb(pPolyLine, idLoopElement))
////			{
////				pPolyLine->close();
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////
////			//Set line type and layer name of the loop element
////			AcDbObjectId loopEleLineTypeId;
////			AcDbObjectId loopEleLayerID;
////			getLinetypeIdFromString(strLineType.c_str(), loopEleLineTypeId);
////			if(loopEleLineTypeId == NULL)
////			{
////				getLinetypeIdFromString(_T("ByLayer"), loopEleLineTypeId);
////			}
////			if(NSSTRCMP(strLayerName.c_str(), _T("")) == 0)
////				strLayerName = _T("UNKNOWN_FEATURES");
////			getLayerNameIdFromString(strLayerName.c_str(), loopEleLayerID);
////			if(loopEleLayerID == NULL)
////			{
////				// If layer is not present add a new layer
////				if(NS_FAIL == addNewLayer((TCHAR*)strLayerName.c_str(), loopEleLayerID))
////					return NS_FAIL;
////			}
////			if( (Acad::eOk != pPolyLine->setLinetype(loopEleLineTypeId))
////				|| (Acad::eOk != pPolyLine->setLayer(loopEleLayerID)))
////			{
////				pPolyLine->close();
////				return NS_FAIL;
////			}
////			//
////
////			pPolyLine->close();
////
////			// Add the polyline (loop element) to pMPolygon
////			//
////			AcDbEntity *pLoopEnt;
////			if(Acad::eOk != acdbOpenObject(pLoopEnt, idLoopElement, AcDb::kForRead))
////			{
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////			if(Acad::eOk != pMPolygon->appendLoopFromBoundary((AcDbPolyline*)pLoopEnt))
////			{
////				pPolyLine->close();
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////			pLoopEnt->close();
////		}
////
////		if(GL == ELE_CUVRVE || GL == ELE_CURVE_MEMEBR)
////		{
////			if(nGeocntr == 0)
////			{
////				pMPolygon = new AcDbMPolygon();
////			}
////			AcDbPolyline *pPolyLine = new AcDbPolyline();
////			if(NS_SUCCESS != addPolyLineArcSegment(pPolyLine, strVertexPoints))
////			{
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////			// Add  polyline to the database
////			AcDbObjectId idLoopElement;
////			if(NS_SUCCESS != postToDb(pPolyLine, idLoopElement))
////			{
////				pPolyLine->close();
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////
////			//Set line type and layer name of the loop element
////			AcDbObjectId loopEleLineTypeId;
////			AcDbObjectId loopEleLayerID;
////			getLinetypeIdFromString(strLineType.c_str(), loopEleLineTypeId);
////			if(loopEleLineTypeId == NULL)
////			{
////				getLinetypeIdFromString(_T("ByLayer"), loopEleLineTypeId);
////			}
////			if(NSSTRCMP(strLayerName.c_str(), _T("")) == 0)
////				strLayerName = _T("UNKNOWN_FEATURES");
////			getLayerNameIdFromString(strLayerName.c_str(), loopEleLayerID);
////			if(loopEleLayerID == NULL)
////			{
////				// If layer is not present add a new layer
////				if(NS_FAIL == addNewLayer((TCHAR*)strLayerName.c_str(), loopEleLayerID))
////					return NS_FAIL;
////			}
////			if( (Acad::eOk != pPolyLine->setLinetype(loopEleLineTypeId))
////				|| (Acad::eOk != pPolyLine->setLayer(loopEleLayerID)))
////			{
////				pPolyLine->close();
////				return NS_FAIL;
////			}
////			//
////
////			// Add the polyline (loop element) to pMPolygon 
////			pMPolygon->appendLoopFromBoundary(pPolyLine);
////			{
////				pPolyLine->close();
////				if(pMPolygon != NULL)
////					pMPolygon->close();
////				return NS_FAIL;
////			}
////			pPolyLine->close();
////		}
////	}
////
////	AcDbObjectId idOuterBoundary;
////	if(NS_SUCCESS != postToDb(pMPolygon,idOuterBoundary))
////	{
////		if(pMPolygon != NULL)
////			pMPolygon->close();
////		return NS_FAIL;
////	}
////
////	AcDbObjectId lineTypeId;
////	AcDbObjectId layerID;
////
////	getLinetypeIdFromString(strLineType.c_str(), lineTypeId);
////	if(lineTypeId == NULL)
////	{
////		getLinetypeIdFromString(_T("ByLayer"), lineTypeId);
////	}
////
////	if(NSSTRCMP(strLayerName.c_str(), _T("")) == 0)
////		strLayerName = _T("UNKNOWN_FEATURES");
////	getLayerNameIdFromString(strLayerName.c_str(), layerID);
////	if(layerID == NULL)
////	{
////		// If layer is not present add a new layer
////		if(NS_FAIL == addNewLayer((TCHAR*)strLayerName.c_str(), layerID))
////			return NS_FAIL;
////	}
////
////	if( (Acad::eOk != pMPolygon->setLinetype(lineTypeId))
////		|| (Acad::eOk != pMPolygon->setLayer(layerID)))
////	{
////		pMPolygon->close();
////		return NS_FAIL;
////	}
////
////	pMPolygon->close();
////
////	// Add map entries to the MPolygon XDATA
////	STRING_MAP::iterator XDataItr;
////	for(XDataItr = mapOfXData.begin(); XDataItr != mapOfXData.end(); XDataItr++)
////	{
////		if(NS_SUCCESS != addXDATA(idOuterBoundary, (*XDataItr).first, (*XDataItr).second))
////		{
////			return NS_FAIL;
////		}
////	}
////
////	return NS_SUCCESS;
////}



/*
@Brief			Adds the entity to model space
@Param(in)		AcDbEntity		Entity to be added to the model space
@Param(out)	AcDbObjectId	Id of the newly added entity
@Return		int				NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::postToDb(AcDbEntity* ent, AcDbObjectId& objId)
{
	AcDbBlockTable*        pBlockTable = NULL;
	AcDbBlockTableRecord*  pMSpaceRecord = NULL;
	Acad::ErrorStatus es;

	{
		if ((m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))!= Acad::eOk) 
			return NS_FAIL;
		if ((pBlockTable->getAt(ACDB_MODEL_SPACE, pMSpaceRecord, AcDb::kForWrite)) != Acad::eOk) 
		{
			pBlockTable->close();
			return NS_FAIL;
		}
		pBlockTable->close();

		if((es = pMSpaceRecord->appendAcDbEntity(objId, ent)) != Acad::eOk) 
		{
			pMSpaceRecord->close();
			return NS_FAIL;
		}
		pMSpaceRecord->close();
	}
	//no need to close the entity here
	return NS_SUCCESS;
}



/*
@Brief			Serts the Text or Attribute alignment
@Param(in)		AcDbText
@Param(in)		int			number(0, 1, 2, 4, 5, 6, 8, 9, 10) representing Aignment
@Return			int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::setAlignment(AcDbText *pText, int nAignment)
{
	switch(nAignment)
	{
	case 0:	// center center
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextCenter))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextVertMid))
			return NS_FAIL;
		break;

	case 1:	// center Left
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextLeft))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextVertMid))
			return NS_FAIL;
		break;


	case 2: // center right
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextRight))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextVertMid))
			return NS_FAIL;
		break;


	case 4: // top center
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextCenter))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextTop))
			return NS_FAIL;
		break;

	case 5: // top left
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextLeft))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextTop))
			return NS_FAIL;
		break;


	case 6: // top right
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextRight))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextTop))
			return NS_FAIL;
		break;


	case 8: // lower center
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextCenter))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextBottom))
			return NS_FAIL;
		break;


	case 9: // lower left
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextLeft))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextBottom))
			return NS_FAIL;
		break;


	case 10: // lower right
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextRight))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextBottom))
			return NS_FAIL;
		break;


	case 11: // center center
		if(Acad::eOk != pText->setHorizontalMode(AcDb::kTextCenter))
			return NS_FAIL;
		if(Acad::eOk != pText->setVerticalMode(AcDb::kTextVertMid))
			return NS_FAIL;
		break;


	}

	if(Acad::eOk != pText->adjustAlignment(m_pCurrentDatabase))
		return NS_FAIL;

	return NS_SUCCESS;
}



/*
@Brief			Gets the alignment of the Text or Attribute alignment
@Param(in)		AcDbText	
@Return			int			number(0, 1, 2, 4, 5, 6, 8, 9, 10) representing Aignment
*/
int CNSDWGMgr::getAlignment(AcDbText *pText)
{
	int nAignment;
	//if(pText->verticalMode() == AcDb::kTextVertMid) nAignment = 0;
	//if(pText->verticalMode() == AcDb::kTextTop) nAignment = 4;
	//if(pText->verticalMode() == AcDb::kTextBottom) nAignment = 8;

	//if(pText->horizontalMode() == AcDb::kTextMid) nAignment+=1;
	//if(pText->horizontalMode() == AcDb::kTextLeft) nAignment+=1;
	//if(pText->horizontalMode() == AcDb::kTextRight) nAignment+=1;


	if(pText->verticalMode() == AcDb::kTextVertMid)	// either 0, 1 or 2
	{
		if(pText->horizontalMode() == AcDb::kTextMid) 
		{
			nAignment = 0;
		}
		else
		{
			if(pText->horizontalMode() == AcDb::kTextLeft)
			{
				nAignment = 1;
			}
			else
			{
				nAignment = 2;
			}
		}
	}
	else 
	{
		if(pText->verticalMode() == AcDb::kTextTop) // either 4, 5 or 6
		{
			if(pText->horizontalMode() == AcDb::kTextMid) 
			{
				nAignment = 4;
			}
			else
			{
				if(pText->horizontalMode() == AcDb::kTextLeft)
				{
					nAignment = 5;
				}
				else
				{
					nAignment = 6;
				}
			}
		}
		else // either 8, 9 or 10
		{
			if(pText->verticalMode() == AcDb::kTextBottom) 
			{
				if(pText->horizontalMode() == AcDb::kTextMid) 
				{
					nAignment = 8;
				}
				else
				{
					if(pText->horizontalMode() == AcDb::kTextLeft)
					{
						nAignment = 9;
					}
					else
					{
						nAignment = 10;
					}
				}
			}
			else // default
			{
				nAignment = 9;
			}
		}
	}

	return nAignment;
}



/*
@Brief			selects Text labels on layer "DEVELOPER_PLAN"
@Param(out)	std::vector<CNSBlock*>
@return		int		NS_FAIL/NS_SUCCESS
*/
int CNSDWGMgr::selectTextLabels(std::vector<CNSFeatureContainer*> &vectTextLabels, std::vector<NSSTRING> szCadastreLayers)
{
	AcDbBlockTable* pBlockTable =NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space block table record
	// Copy the contents(Text String, Insertion point, Rotation) of model space Text entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		AcDbEntity *pEnt;
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		NSSTRING szEntityLayer = NSSTRING(pEnt->layer());
		// Select blocks on layer DEVELOPER_PLAN
		std::vector<NSSTRING>::iterator szLayerItr;
		bool bLayerMatched = false;
		for(szLayerItr = szCadastreLayers.begin();szLayerItr !=szCadastreLayers.end();szLayerItr++)
		{
			NSSTRING strLayerName = *(szLayerItr);
			if(NSSTRCMP(szEntityLayer.c_str(), strLayerName.c_str()) == 0)
			{
				bLayerMatched = true;
				break;		
			}
		}
		if(!bLayerMatched)
		{
			pEnt->close();
			continue;
		}

		// If the entity is a Text get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbText::desc()))
		{
			pEnt->close();
			continue;		
		}

		//AcDbBlockReference* block = (AcDbBlockReference*)pEnt;
		CNSFeatureContainer *pFeatContainer = new CNSFeatureContainer;
		CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;
		pGeometry->setCompType(COMP_TEXTPOINT);
		CNSBlock * pTempText = new CNSBlock();
		AcDbText* pText = (AcDbText*)pEnt;
		CNSAttributeRef* pRef = new CNSAttributeRef;
		pRef->setAttributeValue(pText->textString());
		pRef->setCompGeometry(pGeometry);
		pFeatContainer->m_mapOfBlkAtt = new ATT_MAP;
		pFeatContainer->m_mapOfXData = new STRING_MAP;
		pFeatContainer->addAttributeRef(_T("Label"), 1, pRef);
		// Adds attribute to the Block to be returned
		//pTempNSBlock->addAttribute(pszKey, pszValue, );
		//pFeatContainer->m_mapBlkAtt.insert(std::map<NSSTRING, NSSTRING>::value_type(pszKey, pszValue));
		//	pFeatContainer->m_mapOfBlkAtt->insert(std::map<NSSTRING, CNSAttributeRef*>::value_type(_T("Label"), pRef));



		//	pTempText->setBlockName(pText->textString());
		pTempText->setRotation(pText->rotation());
		CNSPoint3D textPosition = CNSPoint3D(pText->position().x, pText->position().y);
		pTempText->setInsertionPoint(textPosition.m_dXX,textPosition.m_dYY);
		pTempText->setAlignment(0); //For block the PointInAlignment value is 0.
		//pTempText->alignmentPoint();
		pGeometry->add(pTempText);
		CNSCompositeContainer *pContainer = new CNSCompositeContainer;
		pContainer->add(pGeometry);
		pFeatContainer->m_pContainer = pContainer;

		//AcDbMText* pText = (AcDbMText*)pEnt;
		//pTempText->setBlockName(pText->contents());
		//pTempText->setRotation(pText->rotation());
		//CNSPoint3D textPosition = CNSPoint3D(pText->location().x, pText->location().y);
		//pTempText->setInsertionPoint(textPosition);
		//AcDbMText::AttachmentPoint alignment = pText->attachment();

		if(pEnt != NULL)
			pEnt->close();		
		if(pText != NULL)		
			pText->close();

		// Add the CNSBlock to the vector to be returned
		vectTextLabels.push_back(pFeatContainer);

	} //end model space iteration

	delete pBlkTblRcdItr;
	pRecord->close();
	pBlockTable->close();	

	return NS_SUCCESS;
}



/*
@Brief			Inserts a AcDbText on given layer
@Param(in)		CNSBlock	represents TextString, TestPosition, TextRotation
@Param(in)		NSSTRING	LayerName
@Return		int			NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::insertText(CNSBlock* blkBlock, NSSTRING strText, NSSTRING pszLayerName)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbText *pText = new AcDbText();
	CNSPoint3D nsBasePoint;
	blkBlock->getInsertionpoint(nsBasePoint);
	AcGePoint3d basePoint(nsBasePoint.m_dXX, nsBasePoint.m_dYY, 0.0);

	//es = pText->setTextString(blkBlock->getBlockName());
	//es = pText->setRotation(blkBlock->getRotation());
	//es = pText->setPosition(basePoint);

	if(Acad::eOk != pText->setTextString(strText.c_str())
		|| Acad::eOk != pText->setRotation(blkBlock->getRotation())
		|| Acad::eOk != pText->setPosition(basePoint)
		|| Acad::eOk != pText->setHeight(m_dTextHeight))
	{
		pText->close();
		return NS_FAIL;
	}


	// Open the current database's model space block table record.
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))
	{
		pText->close();
		return NS_FAIL;
	}

	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite))
	{
		pText->close();
		pBlockTable->close();
		return NS_FAIL;
	}
	es = pBlockTable->close();

	// Append the Text entity the model space block table record.
	AcDbObjectId newEntId;
	if(Acad::eOk != pBlockTableRecord->appendAcDbEntity(newEntId, pText))
		return NS_FAIL;

	es = pText->setLayer(pszLayerName.c_str());

	pText->close();
	es = pBlockTableRecord->close();

	return ((es == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}
/*
@Brief           Gets list of all layer properties
@Param(out)      map<NSSTRING,CNSLayer*>		vector of layer properties
@Return          int							NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getLayerProperties(std::map<NSSTRING, CNSLayer*> &layerProperties)
{	
	TCHAR* pszLayerName;    
	TCHAR* pszLineTypeName;
	TCHAR* pszPlotStyleName;

	AcDbLayerTable* pLayer;
	if(m_pCurrentDatabase->getSymbolTable(pLayer, AcDb::kForRead) != Acad::eOk)
	{
		return NS_FAIL;
	}

	AcDbLayerTableIterator* pLayerIter;
	pLayer->newIterator(pLayerIter);
	AcDbLayerTableRecord* pRecord;
	for(pLayerIter->start(); !pLayerIter->done(); pLayerIter->step())
	{
		if(pLayerIter->getRecord(pRecord, AcDb::kForRead) != Acad::eOk)
		{
			pLayer->close();
			return NS_FAIL;
		}
		if(pRecord->getName(pszLayerName) != Acad::eOk)
		{
			pRecord->close();
			pLayer->close();
			return NS_FAIL;
		}

		AcCmColor color = pRecord->color();
		int nColor = color.colorIndex();
		bool bIsFrozen =   pRecord->isFrozen();
		bool bIsLocked = pRecord->isLocked();
		bool bIsOff = pRecord->isOff();
		bool bPlotState = pRecord->isPlottable();
		int nLineWt = (int)pRecord->lineWeight();
		pszPlotStyleName = pRecord->plotStyleName();


		AcDbObjectId ltId = pRecord->linetypeObjectId();          //Getting the Linetype id
		AcDbObject* pLayerLineObj;
		if(acdbOpenAcDbObject(pLayerLineObj, ltId, AcDb::kForRead) == Acad::eOk)
		{		
			AcDbLinetypeTableRecord* pLineTableRecord =(AcDbLinetypeTableRecord*) pLayerLineObj;
			if(pLineTableRecord->getName(pszLineTypeName) != Acad::eOk)   //Getting Linetype name
			{
				pLineTableRecord->close();
				pLayer->close();
				return NS_FAIL;
			}              
			pLineTableRecord->close();
		}
		CNSLayer *pLayerObj =  new CNSLayer;
		pLayerObj->setLayername(pszLayerName);
		pLayerObj->setColor(nColor);
		pLayerObj->setIsFrozen(bIsFrozen);
		pLayerObj->setIsLocked(bIsLocked);
		pLayerObj->setIsOff(bIsOff);
		pLayerObj->setPlotState(bPlotState);
		pLayerObj->setLineWt(nLineWt);
		pLayerObj->setPlotStyleName(pszPlotStyleName);
		pLayerObj->setLineTypeName(pszLineTypeName);

		//Add the objects in the map
		layerProperties.insert(std::map<NSSTRING, CNSLayer*>::value_type(pszLayerName, pLayerObj));
		pRecord->close();	
	}
	delete pLayerIter;
	pLayer->close();
	return NS_SUCCESS;
}

/*
@Brief           Gets list of all textstyle properties
@Param(out)      vector<CNSTextStyle*>    vector of textstyle properties
@Return          int                     NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getTextStyleProperties(std::map<NSSTRING,CNSTextStyle*> &TextStyleProperties)
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbTextStyleTable *pTextStyleTbl = NULL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pTextStyleTbl, AcDb::kForRead)) //get line table
	{
		return NS_FAIL;
	}

	AcDbTextStyleTableIterator* pTextIterator;
	pTextStyleTbl->newIterator(pTextIterator);
	AcDbTextStyleTableRecord *pTextStyleRecord;
	TCHAR* pszTextName;
	for(pTextIterator->start(); !pTextIterator->done(); pTextIterator->step())
	{
		if(Acad::eOk != pTextIterator->getRecord(pTextStyleRecord, AcDb::kForRead))
		{
			pTextStyleTbl->close();
			return NS_FAIL;
		}

		if(Acad::eOk != pTextStyleRecord->getName(pszTextName))
		{
			pTextStyleRecord->close();
			pTextStyleTbl->close();
			return NS_FAIL;
		}

		TCHAR* pFontName;
		Adesk::Boolean bBold = false;
		Adesk::Boolean bItalic = false;
		int nCharSet;
		int nPicthFamily = 0;	
		if(Acad::eOk != pTextStyleRecord->font(pFontName, bBold, bItalic, nCharSet, nPicthFamily))
		{
			pTextStyleRecord->close();
			pTextStyleTbl->close();
			return NS_FAIL;
		}
		if(NSSTRCMP(pFontName, _T("")) == 0)
		{
			if(Acad::eOk != pTextStyleRecord->fileName(pFontName))
			{
				pTextStyleRecord->close();
				pTextStyleTbl->close();
				return NS_FAIL;
			}
		}


		//Create object of Textstyle and store the properties in it
		CNSTextStyle *TextStyleObj = new CNSTextStyle;
		if(pszTextName != NULL)
		{
			TextStyleObj->setTextStyleName(pszTextName);
		}
		if(pFontName != NULL)
		{
			TextStyleObj->setFontName(pFontName);
		}
		if(pszTextName != NULL)
		{
			TextStyleProperties.insert(std::map<NSSTRING, CNSTextStyle*>::value_type(pszTextName, TextStyleObj));
		}
		pTextStyleRecord->close();
	}
	delete pTextIterator;
	pTextStyleTbl->close();
	return NS_SUCCESS;
}

/*
@Brief			Gets list of all line type names
@Param(out)	map<NSSTRING, CNSLineType*>	map of line type names
@Return		int							NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::getLineTypeList(std::map<NSSTRING, CNSLineType*> &LineTypeProperties)
{
	AcDbLinetypeTable *pLinetypeTbl = NULL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pLinetypeTbl, AcDb::kForRead)) //get line table
		return NS_FAIL;

	// Create a new iterator that starts at table
	// beginning and skips deleted.
	AcDbLinetypeTableIterator *pLtIterator;
	pLinetypeTbl->newIterator(pLtIterator);

	// Walk the table, getting every table record and
	// storing the linetype name in a map
	AcDbLinetypeTableRecord *pLtTableRcd;
	TCHAR* pszLtName;
	TCHAR* pszAsciiDescription;
	for (pLtIterator->start(); !pLtIterator->done(); pLtIterator->step())
	{
		if(Acad::eOk != pLtIterator->getRecord(pLtTableRcd, AcDb::kForRead))
		{
			pLinetypeTbl->close();
			return NS_FAIL;
		}
		if(Acad::eOk != pLtTableRcd->getName(pszLtName))
		{
			pLtTableRcd->close();
			pLinetypeTbl->close();
			return NS_FAIL;
		}


		//if(Acad::eOk != pLtTableRcd->asciiDescription(pszAsciiDescription))
		if(Acad::eOk != pLtTableRcd->comments(pszAsciiDescription))
		{	
			pLtTableRcd->close();
			pLinetypeTbl->close();
			return NS_FAIL;
		}


		CNSLineType *pLineType = new CNSLineType();

		if(pszLtName != NULL)
			pLineType->setLineTypeName(pszLtName);

		if(pszAsciiDescription != NULL)
			pLineType->setAsciiDescription(pszAsciiDescription);
		if(pszLtName != NULL && pszAsciiDescription != NULL)
		{
			LineTypeProperties.insert(std::map<NSSTRING, CNSLineType*>::value_type(pszLtName, pLineType));
		}
		pLtTableRcd->close();		
	}
	delete pLtIterator;
	pLinetypeTbl->close();
	return NS_SUCCESS;
}

/*
@Brief			Gets list of all dimension type names
@Param(out)	map<NSSTRING, CNSDimStyle*>	vector of dimension type names
@Return		int							0-NS_SUCCESS, 1-NS_FAIL
*/
int CNSDWGMgr::getDimStyleList(std::map<NSSTRING, CNSDimStyle*> &DimStyleProperties)
{
	AcDbDimStyleTable* pDimStyleTable = NULL;
	Acad::ErrorStatus es = Acad::eOk;

	//Purge unused _EXT layers
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pDimStyleTable, AcDb::kForRead)) 
	{
		return NS_FAIL;
	}
	AcDbDimStyleTableIterator* pDimStyleIter;
	TCHAR* pszDimStyleName;
	TCHAR* pszFontName;

	pDimStyleTable->newIterator(pDimStyleIter);
	AcDbDimStyleTableRecord* pDimStyleTableRecord;
	for(pDimStyleIter->start(); !pDimStyleIter->done(); pDimStyleIter->step())
	{
		if(Acad::eOk == pDimStyleIter->getRecord(pDimStyleTableRecord, AcDb::kForRead))
		{
			if (Acad::eOk != pDimStyleTableRecord->getName(pszDimStyleName))
			{
				pDimStyleTableRecord->close();
				pDimStyleTable->close();
				return NS_FAIL;
			}

			AcDbObjectId nTxtStyleId = pDimStyleTableRecord->dimtxsty();
			if(!nTxtStyleId.isNull())
			{	
				AcDbObject* pObjTextStyle;
				if(Acad::eOk == acdbOpenObject(pObjTextStyle,nTxtStyleId,AcDb::kForRead))
				{
					AcDbTextStyleTableRecord* pTextStyle = (AcDbTextStyleTableRecord*)pObjTextStyle;
					Adesk::Boolean bbold = false;
					Adesk::Boolean bItalic = false;
					int pCharSet;
					int pPicthFamily = 0;	
					if(Acad::eOk != pTextStyle->font(pszFontName, bbold, bItalic,pCharSet,pPicthFamily))
					{
						pTextStyle->close();
						pDimStyleTableRecord->close();
						pDimStyleTable->close();
						return NS_FAIL;
					}
					if(NSSTRCMP(pszFontName, _T("")) == 0)
					{
						if(Acad::eOk != pTextStyle->fileName(pszFontName))
						{
							pTextStyle->close();
							pDimStyleTableRecord->close();
							pDimStyleTable->close();
							return NS_FAIL;
						}
					}
					pTextStyle->close();
				}
			}

			TCHAR* pszLineTypeName;
			AcDbObject* pLineObj;
			AcDbObjectId lineTypeId;
			//If there is no line type then by default it is taking "BY_LAYER"
			//as line type name
#ifndef _INTERNAL_
			lineTypeId = pDimStyleTableRecord->dimltype();
#else
			lineTypeId = pDimStyleTableRecord->id();
#endif

			if(Acad::eOk == acdbOpenAcDbObject(pLineObj,lineTypeId, AcDb::kForRead))
			{
				AcDbLinetypeTableRecord* pLineTableRecord =(AcDbLinetypeTableRecord*) pLineObj;
				if( Acad::eOk != pLineTableRecord->getName(pszLineTypeName))
				{
					pLineTableRecord->close();
					pDimStyleTableRecord->close();
					pDimStyleTable->close();
					return NS_FAIL;
				}
			}
			else
				pszLineTypeName = _T("BY_BLOCK");

			double dTxtHeight = pDimStyleTableRecord->dimtxt();

			CNSDimStyle* pDimObj = new CNSDimStyle();
			pDimObj->setDimStyleName(pszDimStyleName);
			pDimObj->setFont(pszFontName);
			pDimObj->setLineTypeName(pszLineTypeName);
			pDimObj->setTxtHt(dTxtHeight);

			DimStyleProperties.insert(std::map<NSSTRING, CNSDimStyle*>::value_type(pszDimStyleName, pDimObj));

			pDimStyleTableRecord->close();
			if(pLineObj != NULL)
				pLineObj->close();
		}
	}
	pDimStyleTable->close();
	return NS_SUCCESS;
}
/*
@brief		Checks the visibility of the block attributes and stores it in the map
@Param(out)	Map containing key as the block name and a vector that stores the CNSAttribute object.
@Return		int				0-NS_SUCCESS, 1-NS_FAIL
*/
int CNSDWGMgr::checkBlockAttributeVisibility(std::map<NSSTRING, std::map<NSSTRING,CNSAttributeRef*>*> &mapBlockAttrVisibility)
{
	AcDbBlockTable* pBlockTable = NULL;
	TCHAR* pszBlockName;

	m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableIterator* pBlockTableIter;
	pBlockTable->newIterator(pBlockTableIter);
	AcDbBlockTableRecord* pBlockDef = NULL;
	CString strtocmp = _T("_USER");
	CString tempStrLayerName = "";
	//AcDbObjectIdArray refIds;
	// new AcDbObjectIdArray();

	for(pBlockTableIter->start(); !pBlockTableIter->done(); pBlockTableIter->step())
	{
		if(Acad::eOk != pBlockTableIter->getRecord(pBlockDef, AcDb::kForRead))
			continue;
		if(!pBlockDef->hasAttributeDefinitions())
		{
			pBlockDef->close();
			continue;
		}

		if(Acad::eOk != pBlockDef->getName(pszBlockName))
		{
			pBlockDef->close();
			delete pBlockTableIter;
			pBlockTable->close();
		}

		NSSTRING strTempBlkName(pszBlockName);

		// strInsertedBlockName = NSSTRING(pszBlockName);
		//getEffectiveName( pBlockDef,  pBlockRef, strInsertedBlockName,  pszInsertedBlockName);

		if (NSSTRING(strTempBlkName).substr(0,1) == _T ("*")) // TO RESTRICT THE ANNONIMUS BLOCKS TO BE REPORTED
		{
			pBlockDef->close();
			continue;
		}

		AcDbAttributeDefinition* pAttDef = NULL;
		AcDbBlockTableRecordIterator* pBlockTblRecordIter;
		pBlockDef->newIterator(pBlockTblRecordIter);

		TCHAR* LayerName = NULL;

		int Process =0;
		AcDbBlockReferenceIdIterator *pIterator;
		pBlockDef->newBlockReferenceIdIterator(pIterator);
		for (pIterator->start(); !pIterator->done(); pIterator->step()) 
		{
			AcDbBlockReference *pBlockRef;
			pIterator->getBlockReference(pBlockRef, AcDb::kForRead);
			NSSTRING Layer = pBlockRef->layer();

			CString tempStrLayerName = Layer.substr(0,5).c_str();
			if(!tempStrLayerName.CompareNoCase(strtocmp))
			{
				Process=1;
				continue;
			}
			Process =0;
			break;
		}

		if(Process == 1)
		{
			pBlockDef->close();
			continue;
		}


		std::map<NSSTRING, CNSAttributeRef*> *mapofAttrList = new std::map<NSSTRING, CNSAttributeRef*>;
		for(pBlockTblRecordIter->start(); !pBlockTblRecordIter->done(); pBlockTblRecordIter->step())
		{
			AcDbEntity* pEnt;
			pBlockTblRecordIter->getEntity(pEnt, AcDb::kForRead);

			if(!pEnt->isKindOf(AcDbAttributeDefinition::desc()))
			{
				pEnt->close();
				continue;
			}

			AcDbAttributeDefinition* pAttDef = (AcDbAttributeDefinition*)pEnt;
			TCHAR* pszKey = pAttDef->tag();

			int nInVisible = pAttDef->isInvisible();
			bool bValue;
			(nInVisible == 0 ? (bValue = true) : (bValue = false));

			CNSAttributeRef *attRefObj = new CNSAttributeRef();
			attRefObj->setTagName(pszKey);
			attRefObj->setIsVisible(bValue);
			getupperString(pszKey);
			mapofAttrList->insert(std::map<NSSTRING,CNSAttributeRef*>::value_type(pszKey,attRefObj));
			pEnt->close();
		}

		pBlockDef->close();
		NSSTRING strBlkName = pszBlockName;
#ifndef _INTERNAL_
		NSSTRUPR(pszBlockName, strBlkName.size()+1); 
#else
		NSSTRUPR(pszBlockName, strBlkName.size()+1); 
		//NSSTRUPR((TCHAR*)pszBlockName);
#endif
		mapBlockAttrVisibility.insert(std::map<NSSTRING, std::map<NSSTRING, CNSAttributeRef*>*>::value_type(pszBlockName, mapofAttrList));
	}
	//refIds.setPhysicalLength(0);

	delete pBlockTableIter;
	pBlockTable->close();
	return NS_SUCCESS;
}



/*
@Brief			Gets all Blocks and Liner features to be validated
@param(out)	std::map<long, CNSValidationDataCollector*>		map of Blocks To Be Validated
@param(out)	std::map<long, CNSValidationDataCollector*>		map of Linear Features To Be Validated
@Param(in)		bool											if true - gets attribute and XDATA information
@Param(in)		int												nRelationshipFlag
@Param(in)		TCHAR*											pszLayerNameSuffix (_NEW/_MOD/_EXT)
*/
int CNSDWGMgr::getFeaturesToBeValidated(std::map<long, CNSValidationDataCollector*> &mapBlocksToBeValidated, std::map<long, CNSValidationDataCollector*> &mapLinearFeaturesToBeValidated, bool bGetXInfo, bool getLinerFeatures, TCHAR* pszLayerNameSuffix)
{
	// Get Model-space Block table record
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;
	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Attributes and XDATA) of model space pBlockRef entity into a CNSValidationDataCollector
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt = NULL;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		NSSTRING strLayerNameSuffix = NSSTRING(pszLayerNameSuffix); // function parameter
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());			// Layer name of the Entity

		if(	getLinerFeatures // Busbar implementation -80 hrs change request start
			&& (pEnt->isKindOf(AcDbPolyline::desc()) || pEnt->isKindOf(AcDbMline::desc()))
			&& (NSSTRCMP(strLayerNameSuffix.c_str(), _T("_EXT")) == 0))
		{
			NSSTRING strEntityLayerSuffix;
			if(strEntityLayer.size() > 4)
				strEntityLayerSuffix = strEntityLayer.substr((strEntityLayer.size() - 4), 4);

			if(NSSTRCMP(strEntityLayerSuffix.c_str(), _T("_EXT")) != 0)
			{
				pEnt->close();
				continue;	// The line is not an existing cable
			}
			else			// it is a cable
			{
				CNSValidationDataCollector *pCNSLinearFeature = new CNSValidationDataCollector;

				if(pEnt->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pTempLine  = (AcDbPolyline*)pEnt;

					pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
					pCNSLinearFeature->setFeatureType(FEATURE_LINEAR);
					pCNSLinearFeature->setFeatureStatus(STATUS_EXT);
					AcGePoint3d startPoint, endPoint;
					pTempLine->getStartPoint(startPoint);
					pTempLine->getEndPoint(endPoint);
					pCNSLinearFeature->setInsertionPoint(startPoint.x, startPoint.y);
					pCNSLinearFeature->setEndPoint(endPoint.x, endPoint.y);
					pCNSLinearFeature->setLayerName(pTempLine->layer());
					pCNSLinearFeature->setLineType(pTempLine->linetype());
					pCNSLinearFeature->setScaleFactor(1.000, 1.000);
				}
				// Busbar implementation -80 hrs change request start
				else if (pEnt->isKindOf(AcDbMline::desc()))
				{
					AcDbMline *pTempLine  = (AcDbMline*)pEnt;

					pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
					pCNSLinearFeature->setFeatureType(FEATURE_LINEAR);
					pCNSLinearFeature->setFeatureStatus(STATUS_EXT);
					AcGePoint3d startPoint, endPoint;
					int nVert = pTempLine->numVertices();
					if(nVert>0)
					{
						startPoint = pTempLine->vertexAt(0);
						endPoint = pTempLine->vertexAt(nVert-1);
					}
					pCNSLinearFeature->setInsertionPoint(startPoint.x, startPoint.y);
					pCNSLinearFeature->setEndPoint(endPoint.x, endPoint.y);
					pCNSLinearFeature->setLayerName(pTempLine->layer());
					pCNSLinearFeature->setLineType(pTempLine->linetype());
					pCNSLinearFeature->setScaleFactor(1.000, 1.000);
				}
				// Busbar implementation -80 hrs change request end

				if(bGetXInfo)
					readXDATA(pEnt->id(), pCNSLinearFeature->m_mapOfRelationsAndConnectivity);

				mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSLinearFeature));
			}
			pEnt->close();
			continue;
		}


		if(getLinerFeatures 
			&& ((pEnt->isKindOf(AcDbLine::desc())) || 
			(pEnt->isKindOf(AcDbPolyline::desc())) || 
			(pEnt->isKindOf(AcDbArc::desc())) ||
			(pEnt->isKindOf(AcDbMline::desc())))
			) 
		{
			NSSTRING strCableLayerNameSuffix = NSSTRING(pszLayerNameSuffix);
			//int nIndex = (int)strEntityLayer.rfind(strCableLayerNameSuffix);
			int nIndex = rFindString(strEntityLayer, strCableLayerNameSuffix);
			if(nIndex == -1)
			{	
				pEnt->close();
				continue;	// The line is not a cable
			}
			else			// it is a cable
			{						
				CNSValidationDataCollector *pCNSLinearFeature = new CNSValidationDataCollector;
				pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
				pCNSLinearFeature->setFeatureType(FEATURE_LINEAR);
				pCNSLinearFeature->setFeatureStatus(STATUS_NEW);
				pCNSLinearFeature->setLayerName(pEnt->layer());
				pCNSLinearFeature->setLineType(pEnt->linetype());
				pCNSLinearFeature->setScaleFactor(1.000, 1.000);

				if(pEnt->isKindOf(AcDbLine::desc()))
				{
					AcDbLine *pTempLine  = (AcDbLine*)pEnt;
					pCNSLinearFeature->setInsertionPoint(pTempLine->startPoint().x, pTempLine->startPoint().y);
					pCNSLinearFeature->setEndPoint(pTempLine->endPoint().x, pTempLine->endPoint().y);
				}
				else if(pEnt->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline *pTempPLine  = (AcDbPolyline*)pEnt;
					AcGePoint3d startPoint, endPoint;
					pTempPLine->getStartPoint(startPoint);
					pTempPLine->getEndPoint(endPoint);
					pCNSLinearFeature->setInsertionPoint(startPoint.x, startPoint.y);
					pCNSLinearFeature->setEndPoint(endPoint.x, endPoint.y);

					pCNSLinearFeature->bPolyLine = TRUE;
					double dPolyineVertices = pTempPLine->numVerts();
					for(int vertCounter = 0; vertCounter < dPolyineVertices; vertCounter++)
					{
						AcGePoint3d pTempPoint;
						pTempPLine->getPointAt(vertCounter, pTempPoint);
						pCNSLinearFeature->addToVectOfPolylineVertices(pTempPoint.x, pTempPoint.y);
					}

				}
				else if(pEnt->isKindOf(AcDbArc::desc()))// if it is an arc
				{
					AcDbArc *pTempArc  = (AcDbArc*)pEnt;

					AcGePoint3d startPt, endPt, centrePt;
					pTempArc->getStartPoint(startPt);
					pTempArc->getEndPoint(endPt);
					centrePt = pTempArc->center();
					CNSCircle  *pCircle = new CNSCircle;
					pCircle->m_dRadius = pTempArc->radius();
					pCircle->setCenterPt(centrePt.x, centrePt.y, centrePt.z);
					pCNSLinearFeature->setCircle(pCircle);
					pCNSLinearFeature->setFeatureType(FEATURE_ARC);
					pCNSLinearFeature->setInsertionPoint(startPt.x, startPt.y);
					pCNSLinearFeature->setEndPoint(endPt.x, endPt.y);
				}

				else if(pEnt->isKindOf(AcDbMline::desc()))// if it is an MLine
				{
					AcDbMline *pTempMline  = (AcDbMline*)pEnt;

					AcGePoint3d startPt, endPt;
					int nNumVerts = pTempMline->numVertices();
					if(nNumVerts>0)
					{
						startPt = pTempMline->vertexAt(0);
						endPt = pTempMline->vertexAt(nNumVerts-1);
					}

					pCNSLinearFeature->setInsertionPoint(startPt.x, startPt.y);
					pCNSLinearFeature->setEndPoint(endPt.x, endPt.y);
				}

				mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSLinearFeature));
			}
			pEnt->close();
			continue;	// XDATA and Attributes are not taken for linear features
		}

		// Following code is for getting Block feature data
		// if the entity is a pblockref get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		// NSSTRING strEntityLayerSuffix = strEntityLayer.substr((strEntityLayer.size()-4), 4);
		// Select Blocks on Proper layer
		if(strEntityLayer.size() >= strLayerNameSuffix.size())
		{
			strEntityLayer = strEntityLayer.substr((strEntityLayer.size()-strLayerNameSuffix.size()), strEntityLayer.size());
		}

		if(NSSTRCMP(strEntityLayer.c_str(), strLayerNameSuffix.c_str()) != 0) 
		{
			pEnt->close();
			continue;
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Select Blocks of proper BlockName
		// Decide Block status
		NSSTRING strTempBlkName(pszInsertedBlockName1);

		getEffectiveName( blkTblRecord,  pBlockRef, strTempBlkName/*,  pszInsertedBlockName*/);



		FeatureStatus FStatus = STATUS_INVALID;
		//if((strLayerNameSuffix.find(_T("_NEW")) != -1) && (strTempBlkName.find(_T("_NEW")) != -1))
		if((findString(strLayerNameSuffix, _T("_NEW")) != -1) && (findString(strTempBlkName, _T("_NEW")) != -1))
		{	
			FStatus = STATUS_NEW;
		}
		else
		{
			//if((strLayerNameSuffix.find(_T("_MOD")) != -1) && (strTempBlkName.find(_T("_EXT")) != -1))
			if((findString(strLayerNameSuffix, _T("_MOD")) != -1) && (findString(strTempBlkName, _T("_EXT")) != -1))
			{			
				FStatus = STATUS_MOD;
			}
			else
			{
				//if((strLayerNameSuffix.find(_T("_EXT")) != -1) && (strTempBlkName.find(_T("_EXT")) != -1))
				if((findString(strLayerNameSuffix, _T("_EXT")) != -1) && (findString(strTempBlkName, _T("_EXT")) != -1))
					FStatus = STATUS_EXT;
			}
		}
		if(FStatus == STATUS_INVALID)
		{
			blkTblRecord->close();
			pEnt->close();
			continue;
		}

		CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
		pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
		pCNSValidationDataCollector->setFeatureType(FEATURE_BLOCK);
		pCNSValidationDataCollector->setBlockName(strTempBlkName);
		pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
		pCNSValidationDataCollector->setLineType(pBlockRef->linetype());
		pCNSValidationDataCollector->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);
		pCNSValidationDataCollector->setScaleFactor(pBlockRef->scaleFactors().sx, pBlockRef->scaleFactors().sy);

		pCNSValidationDataCollector->setFeatureStatus(FStatus);


		if(bGetXInfo == true) // Get XDATA and attribute of the feature if bGetXInfo is true
		{		
			// Get attributes of the pBlockRef entity
			AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
			AcDbAttribute* pTempAttribute = NULL;
			for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
			{
				AcDbObjectId attObjId = attributeIterator->objectId();
				if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
				{
					//pTempAttribute->close();
					delete attributeIterator;
					blkTblRecord->close();
					pBlockRef->close();
					pEnt->close();
					pRecord->close();
					pBlockTable->close();				
					return NS_FAIL;
				}			
				TCHAR* pszKey= pTempAttribute->tag();
				TCHAR* pszValue = pTempAttribute->textString();
				getupperString(pszKey);
				//	getupperString(pszValue);
				pCNSValidationDataCollector->m_mapAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(NSSTRING(pszKey), NSSTRING(pszValue)));
				if(pTempAttribute != NULL)
					pTempAttribute->close();
			}

			//if(pTempAttribute != NULL)
			//pTempAttribute->close();
			delete attributeIterator;				

			// Retrieve XDATA of the pBlockRef entity and add it to pCNSValidationDataCollector
			std::map<NSSTRING, NSSTRING> mapXDataRead;
			readXDATA(pEnt->id(), mapXDataRead);
			pCNSValidationDataCollector->addXDATA(mapXDataRead);
		}

		//		// Add the CNSValidationDataCollector to the map to be returned
		mapBlocksToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(pEnt->id().asOldId(), pCNSValidationDataCollector));

		//AcDbHandle entHandle;
		//pEnt->getAcDbHandle(entHandle);//acdbHandEnt
		//mapBlocksToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type((long)(entHandle), pCNSValidationDataCollector));

		if(pEnt != NULL)
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
		}
	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	if(pBlockTable != NULL)
		pBlockTable->close();

	return NS_SUCCESS;
}

int CNSDWGMgr::getFeaturesOnLayer(std::map<long, CNSValidationDataCollector*> &mapBlocksToBeValidated, std::map<long, CNSValidationDataCollector*> &mapLinearFeaturesToBeValidated, int nRelationshipFlag, TCHAR* pszLayerName)
{
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Rotation, Attributes and XDATA) of model space pBlockRef entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			continue;
		}

		// if the entity is a pblockref get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()) 
			&& !pEnt->isKindOf(AcDbPolyline::desc())
			&& !pEnt->isKindOf(AcDbLine::desc()) 
			&& !pEnt->isKindOf(AcDbArc::desc())
			&& !pEnt->isKindOf(AcDbMline::desc()) // Change for BUSBAR implementation
			)
		{
			pEnt->close();
			continue;
		}

		// Select blocks on layers representing New features
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
		//if(NSSTRCMP(pszLayerName,_T("_NEW")) == 0 && pEnt->isKindOf(AcDbPolyline::desc()))
		//{
		//	pEnt->close();
		//	continue;	
		//}

		if(pEnt->isKindOf(AcDbLine::desc())) // If it is a cable the layer name is suffixed with cable-code after _NEW_
		{
			if(NSSTRCMP(pszLayerName,_T("_NEW")) != 0) // dont search for other layer
			{
				pEnt->close();
				continue;	// The line is not a cable
			}
			//int nIndex = (int)strEntityLayer.find_last_of(_T("_NEW_"));
			int nIndex = rFindString(strEntityLayer, _T("_NEW_"));
			if(nIndex == -1)
			{	
				pEnt->close();
				continue;	// The line is not a cable
			}
			else			// it is a cable
			{
				AcDbLine *pTempLine  = (AcDbLine*)pEnt;

				CNSValidationDataCollector *pCNSLinearFeature = new CNSValidationDataCollector;
				pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
				pCNSLinearFeature->setInsertionPoint(pTempLine->startPoint().x, pTempLine->startPoint().y);
				pCNSLinearFeature->setEndPoint(pTempLine->endPoint().x, pTempLine->endPoint().y);
				pCNSLinearFeature->setLayerName(pTempLine->layer());
				pCNSLinearFeature->setLineType(pTempLine->linetype());

				mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSLinearFeature));
				pEnt->close();
				continue;	
			}
		}

		else if(pEnt->isKindOf(AcDbArc::desc())) // If it is a cable the layer name is suffixed with cable-code after _NEW_
		{
			if(NSSTRCMP(pszLayerName,_T("_NEW")) != 0) // dont search for other layer
			{
				pEnt->close();
				continue;	//only new arcs will be considered
			}
			int nIndex = rFindString(strEntityLayer, _T("_NEW_"));
			if(nIndex == -1)
			{	
				pEnt->close();
				continue;	// The line is not a cable
			}
			else			// it is a cable
			{
				AcDbArc *pTempArc  = (AcDbArc*)pEnt;

				CNSValidationDataCollector *pCNSLinearFeature = new CNSValidationDataCollector;
				pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
				AcGePoint3d tempPt;
				pTempArc->getStartPoint(tempPt);
				pCNSLinearFeature->setInsertionPoint(tempPt.x, tempPt.y);
				pTempArc->getEndPoint(tempPt);
				pCNSLinearFeature->setEndPoint(tempPt.x, tempPt.y);
				pCNSLinearFeature->setLayerName(pTempArc->layer());
				pCNSLinearFeature->setLineType(pTempArc->linetype());

				mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSLinearFeature));
				pEnt->close();
				continue;	
			}

		}
		// BUSBAR implementation -- 80 hrs change request start
		else if(pEnt->isKindOf(AcDbMline::desc())) 
		{
			int nIndex = rFindString(strEntityLayer, pszLayerName);
			if(nIndex == -1)
			{
				pEnt->close();
				continue;
			}
			AcDbMline *pTempMline  = (AcDbMline*)pEnt;
			CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;

			CNSValidationDataCollector *pCNSLinearFeature = new CNSValidationDataCollector;
			pCNSLinearFeature->setObjectId((long) pEnt->id().asOldId());
			AcGePoint3d startPoint,endPoint;
			int nVert = pTempMline->numVertices();
			if(nVert>0)
			{
				startPoint = pTempMline->vertexAt(0);
				endPoint = pTempMline->vertexAt(nVert-1);
			}
			pCNSValidationDataCollector->setInsertionPoint(startPoint.x, startPoint.y);
			pCNSValidationDataCollector->setEndPoint(endPoint.x, endPoint.y);
			pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
			pCNSValidationDataCollector->setLayerName(pTempMline->layer());
			pCNSValidationDataCollector->setLineType(pTempMline->linetype());
			readXDATA(pEnt->id(), pCNSValidationDataCollector->m_mapOfRelationsAndConnectivity);
			mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSValidationDataCollector));
			pEnt->close();
			continue;	

		}
		// BUSBAR implementation -- 80 hrs change request end
		if(strEntityLayer.size() < 4)
		{
			pEnt->close();
			continue;
		}
		NSSTRING strLayerSuffix = strEntityLayer.substr((strEntityLayer.size()-4), 4);
		if(pEnt->isKindOf(AcDbPolyline::desc()))
		{
			int nIndex = rFindString(strEntityLayer, pszLayerName);
			if(nIndex == -1)
			{
				pEnt->close();
				continue;
			}

			/*		if(NSSTRCMP(strLayerSuffix.c_str(), pszLayerName) != 0) 
			{
			pEnt->close();
			continue;
			}*/
			CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
			AcDbPolyline *pTempLine  = (AcDbPolyline*)pEnt;
			AcGePoint3d startPoint, endPoint;
			pTempLine->getStartPoint(startPoint);
			pTempLine->getEndPoint(endPoint);
			pCNSValidationDataCollector->setInsertionPoint(startPoint.x, startPoint.y);
			pCNSValidationDataCollector->setEndPoint(endPoint.x, endPoint.y);
			pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
			pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
			pCNSValidationDataCollector->setLineType(pTempLine->linetype());
			readXDATA(pEnt->id(), pCNSValidationDataCollector->m_mapOfRelationsAndConnectivity);
			mapLinearFeaturesToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSValidationDataCollector));

			pEnt->close();
			continue;
		}

		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			if(NSSTRCMP(strLayerSuffix.c_str(), pszLayerName) != 0) 
			{
				pEnt->close();
				continue;
			}
		}


		//NSSTRING strLayerName = NSSTRING(pszLayerName);
		//NSSTRING strLayer;
		//// Select blocks on given layer name otherwise select on all layers
		//if(NSSTRCMP(pszLayerName, _T("all")) != 0 )
		//{
		//	NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
		//	strLayer = strEntityLayer;
		//	if(strEntityLayer.size() >= strLayerName.size())
		//	{
		//		strEntityLayer = strEntityLayer.substr((strEntityLayer.size()-strLayerName.size()), strEntityLayer.size());
		//	}
		//	if(NSSTRCMP(pszLayerName, strEntityLayer.c_str()) != 0)
		//		continue;
		//}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			continue;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
		}

		NSSTRING strTempBlkName(pszInsertedBlockName1);

		getEffectiveName( blkTblRecord,  pBlockRef, strTempBlkName/*,  pszInsertedBlockName*/);

		CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
		pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
		pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
		pCNSValidationDataCollector->setBlockName(strTempBlkName);//setBlockName(pszInsertedBlockName);
		pCNSValidationDataCollector->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);	

		// Get attributes of the pBlockRef entity
		AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
		AcDbAttribute* pTempAttribute = NULL;
		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				pTempAttribute->close();
				delete attributeIterator;
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				continue;
			}			
			TCHAR* pszKey= pTempAttribute->tag();
			TCHAR* pszValue = pTempAttribute->textString();
			getupperString(pszKey);
			//		getupperString(pszValue);
			//		if(NSSTRCMP(pszLayerName, _T("_NEW")) == 0)
			pCNSValidationDataCollector->m_mapAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(NSSTRING(pszKey), NSSTRING(pszValue)));
			if(pTempAttribute != NULL)
				pTempAttribute->close();
		}
		//	if(NSSTRCMP(pszLayerName, _T("_EXT")) == 0)
		readXDATA(pEnt->id(), pCNSValidationDataCollector->m_mapOfRelationsAndConnectivity);

		//if(pTempAttribute != NULL)
		//pTempAttribute->close();
		if(blkTblRecord != NULL)
			blkTblRecord->close();
		delete attributeIterator;				

		if(pBlockRef != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();



		//		// Add the CNSValidationDataCollector to the map to be returned
		mapBlocksToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSValidationDataCollector));

	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	if(pBlockTable != NULL)
		pBlockTable->close();

	return NS_SUCCESS;
}
/*
@brief		This function gets the textHeight of the text in a vector
@Param(in)	NULL
@Param(out)	std::vector of class CNSTextHeight that consist the height value and
the position of text
*/
int CNSDWGMgr::getTextHeight(std::vector<CNSTextHeight*> &vectTxtHt)
{
	Acad::ErrorStatus es = Acad::eOk;
	TCHAR* pszTextString = NULL;
	AcDbBlockTable* pBlockTable;

	if (Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord;

	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();

	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			continue;
		}
		if(!pEnt->isKindOf(AcDbText::desc()))
		{
			pEnt->close();
			continue;
		}
		AcDbText* pText = (AcDbText*)pEnt;

		//Get the height and position
		double dHeight = pText->height();
		AcGePoint3d position;
		position = pText->position();
		double dX = position.x;
		double dY = position.y;

		CNSTextHeight* pTxtHtObj = new CNSTextHeight();
		//pTxtHtObj->setTxtString(pszTextString);
		pTxtHtObj->setHeight(dHeight);
		pTxtHtObj->setXPosition(dX);
		pTxtHtObj->setYPosition(dY);

		vectTxtHt.push_back(pTxtHtObj);
		pEnt->close();
	}
	delete pBlkTblRcdItr;
	pBlockTableRecord->close();
	return NS_SUCCESS;
}
/*
@brief		Checks if there is any Polyline on the given layers  and is in open state
and stores its position in vector
@Param(in)	std::vector List of layers from where to get the lines
@Param(out)	std::vector storing the positions of the geometry
*/
int CNSDWGMgr::getOpenPLinesOnDeveloperPlan(std::vector<CNSPoint3D> &vectPtsOnDevPlan, std::vector<NSSTRING> szVectLayersToCheck)
{
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord;

	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();

	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt = NULL;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt, AcDb::kForRead))
		{
			continue;
		}
		if(pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pObjPLine = (AcDbPolyline*)pEnt;
			TCHAR* pszLayerName = pObjPLine->layer();
			//char* strLayerName = (char*)pszLayerName;
			std::vector<NSSTRING>::iterator vLayerIter;
			for(vLayerIter = szVectLayersToCheck.begin(); vLayerIter != szVectLayersToCheck.end(); vLayerIter++)
			{
				NSSTRING strTempLayer = (NSSTRING)(*(vLayerIter));
				if(NSSTRCMP(pszLayerName, strTempLayer.c_str()) == 0)
				{
					if(!pObjPLine->isClosed())
					{
						AcGePoint3d startPoint; 
						pObjPLine->getStartPoint(startPoint);
						double dX = startPoint.x;
						double dY = startPoint.y;

						CNSPoint3D point;
						point.m_dXX = dX;
						point.m_dYY = dY;
						// Add this point into vector and send this vector back
						// It will be used to display error block in DWG
						vectPtsOnDevPlan.push_back(point);
						break;
					}
					pObjPLine->close();
				}
			}
		}
		if(pEnt != NULL)
			pEnt->close();
	}
	delete pBlkTblRcdItr;
	pBlockTableRecord->close();
	return NS_SUCCESS;
}
/*
@brief		Checks if there is any geometry on zero layer and stores its position in vector
@Param(in)	NULL
@Param(out)	std::vector storing the positions of the geometry
*/
int CNSDWGMgr::getGeometryOnZeroLayer(std::vector<CNSPoint3D> &vectZeroLayer)
{
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt;
	for(pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{
		
		pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead);
		if(pEnt == NULL)
			continue;
		TCHAR* pszLayerName = pEnt->layer();

		if(NSSTRCMP(pszLayerName, _T("0")) == 0)
		{
			bool bEntityChecked = false;
			AcGePoint3d point;

			if(pEnt->isKindOf(AcDbPolyline::desc()) || pEnt->isKindOf(AcDbLine::desc()) )
			{
				AcDbPolyline* pObjPLine = (AcDbPolyline*)pEnt;
				pObjPLine->getStartPoint(point);
				bEntityChecked = true;
			}
			else if(pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pObjLine = (AcDbLine*)pEnt;
				pObjLine->getStartPoint(point);
				bEntityChecked = true;
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle* pObjCircle = (AcDbCircle*)pEnt;
				point = pObjCircle->center();
				bEntityChecked = true;
			}
			else if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc* pObjArc = (AcDbArc*)pEnt;
				point = pObjArc->center();
				bEntityChecked = true;
			}
			else if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pBlkRef = (AcDbBlockReference*)pEnt;
				point = pBlkRef->position();
				bEntityChecked = true;
			}
			if(bEntityChecked == true)
			{
				CNSPoint3D temp;
				temp.m_dXX = point.x;
				temp.m_dYY = point.y;
				vectZeroLayer.push_back(temp);
			}
		}
		pEnt->close();
	}
	delete pBlkTblRcdItr; 
	pBlockTableRecord->close();
	return NS_SUCCESS;
}
/*
@brief		Gets the area, start position, end postion of the particular ObjectID
if it is of AcDbPolyline type
@Param(in)	long ObjectId
@Param(out)	CNSPoint3D pointStart, CNSPoint3D pointVertex, double dArea
*/
int CNSDWGMgr::getPlineRecord(long ObjectId, CNSPoint3D &startPoint, CNSPoint3D &nextPoint, double &dArea)
{
	AcDbObject *pEnt = NULL;
	AcDbObjectId temp;
	temp.setFromOldId((LONG_PTR)&ObjectId);

	if(Acad::eOk != acdbOpenObject(pEnt, temp, AcDb::kForRead))
	{
		return NS_FAIL;
	}

	if(pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline* pObjPLine = (AcDbPolyline*)pEnt;

		if(Acad::eOk != pObjPLine->getArea(dArea))
		{
			return NS_FAIL;
		}

		AcGePoint3d firstPoint;
		pObjPLine->getStartPoint(firstPoint);
		startPoint.m_dXX = firstPoint.x;
		startPoint.m_dYY = firstPoint.y;

		AcGePoint3d vertexPoint;
		int nVertices = pObjPLine->numVerts();
		pObjPLine->getPointAt(1, vertexPoint);
		nextPoint.m_dXX = vertexPoint.x;
		nextPoint.m_dYY = vertexPoint.y;
	}

	if(pEnt->isKindOf(AcDbMline::desc()))
	{
		AcDbMline* pObjPLine = (AcDbMline*)pEnt;

		/*if(Acad::eOk != pObjPLine->getArea(dArea))
		{
		return NS_FAIL;
		}*/

		dArea = 0.0;
		AcGePoint3d firstPoint;
		firstPoint = pObjPLine->vertexAt(0);
		startPoint.m_dXX = firstPoint.x;
		startPoint.m_dYY = firstPoint.y;

		AcGePoint3d vertexPoint;
		int nVertices = pObjPLine->numVertices();
		vertexPoint = pObjPLine->vertexAt(1);
		nextPoint.m_dXX = vertexPoint.x;
		nextPoint.m_dYY = vertexPoint.y;
	}
	pEnt->close();

	return NS_SUCCESS;
}

int CNSDWGMgr::selectNonStdBlocks(std::map<long, CNSValidationDataCollector*> &mapBlocksToBeValidated)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Rotation, Attributes and XDATA) of model space pBlockRef entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// If the entity is a pBlockRef get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		if(NS_SUCCESS == isTypeStd(pszInsertedBlockName))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			continue;
		}

		CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
		pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
		pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
		pCNSValidationDataCollector->setBlockName(pszInsertedBlockName);
		pCNSValidationDataCollector->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);	

		// Get attributes of the pBlockRef entity
		AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
		AcDbAttribute* pTempAttribute = NULL;
		CNSAttributeRef *pRef = NULL;
		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				pTempAttribute->close();
				delete attributeIterator;
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				pRecord->close();
				pBlockTable->close();				
				return NS_FAIL;
			}
			TCHAR* pszKey= pTempAttribute->tag();
			TCHAR* pszValue = pTempAttribute->textString();
			getupperString(pszKey);
			//	getupperString(pszValue);
			pCNSValidationDataCollector->m_mapAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(NSSTRING(pszKey), NSSTRING(pszValue)));
			pTempAttribute->close();
		}
		readXDATA(pEnt->id(), pCNSValidationDataCollector->m_mapOfRelationsAndConnectivity);

		delete attributeIterator;				

		if(pEnt != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();
		blkTblRecord->close();

		//		// Add the CNSValidationDataCollector to the map to be returned
		mapBlocksToBeValidated.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSValidationDataCollector));

	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	if(pBlockTable != NULL)
		pBlockTable->close();

	return NS_SUCCESS;

}

int CNSDWGMgr::isTypeStd(NSSTRING strExtName)
{
	if(strExtName.length() < 4) return NS_FAIL;
	NSSTRING strTemp  = strExtName.substr(strExtName.size()- 4, strExtName.size());
	if(NSSTRCMP(strTemp.c_str(), _T("_EXT")) == 0)
	{
		return NS_SUCCESS;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_NEW")) == 0)
	{
		return NS_SUCCESS;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_DEL")) == 0)
	{
		return NS_SUCCESS;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_MOD")) == 0)
	{
		return NS_SUCCESS;
	}
	else if(NSSTRCMP(strTemp.c_str(), _T("_REP")) == 0)
	{
		return NS_SUCCESS;
	}
	return NS_FAIL;
}

//#ifndef _INTERNAL_
void CNSDWGMgr::withinarea(std::vector<CNSPoint3D> vectPoints, std::vector<CNSPoint3D> &vectPositions)
{
	std::map<AcDbObjectId, std::vector<AcGePoint3d> > mapoflinesData;
	std::map<AcDbObjectId, AcGePoint3d> mapofblocksData;

	// At the start itself get and save data for all lines and blocks in the drawing (only once)
	GetLineObjects(mapoflinesData);
	GetBlockObjectsWithInsertionPoint(mapofblocksData);

	std::map<AcDbObjectId,AcGePoint3d>::iterator blockDataItr;
	AcGePoint3d tmpPoint;

	for(blockDataItr = mapofblocksData.begin(); blockDataItr != mapofblocksData.end(); blockDataItr++)
	{
		if(!isPointInsideBoundary((*blockDataItr).second,vectPoints))
		{
			AcDbEntity* ent;
			if(Acad::eOk == acdbOpenObject(ent,(*blockDataItr).first,AcDb::kForRead))
			{
				tmpPoint = (*blockDataItr).second;
				CNSPoint3D posObj;
				posObj.m_dXX = tmpPoint.x;
				posObj.m_dYY = tmpPoint.y;
				vectPositions.push_back(posObj);
				ent->close();
			}
		}
	}
	std::map<AcDbObjectId,std::vector<AcGePoint3d> >::iterator lineDataItr;
	std::vector<AcGePoint3d> PtsVect;
	for(lineDataItr = mapoflinesData.begin(); lineDataItr != mapoflinesData.end(); lineDataItr++)
	{
		BOOL result = CheckIfLineWithinBlockBoundary(vectPoints,(*lineDataItr).second);
		if(!result)
		{
			AcDbEntity* ent;
			if(Acad::eOk == acdbOpenObject(ent,(*lineDataItr).first,AcDb::kForRead))
			{
				PtsVect = (*lineDataItr).second;
				CNSPoint3D posObj;
				posObj.m_dXX = PtsVect.at(0).x;
				posObj.m_dYY = PtsVect.at(0).y;
				vectPositions.push_back(posObj);
				ent->close();
			}
		}
	}
}
//#endif

int CNSDWGMgr::GetInsertionPoint(long nObjectID, CNSPoint3D &pt)
{
	Acad::ErrorStatus es;
	AcDbObject *pEntity;
	AcDbObjectId pBlockObjID;
	pBlockObjID.setFromOldId(nObjectID);
	es = acdbOpenObject(pEntity, pBlockObjID, AcDb::kForRead);
	if(es != Acad::eOk)
		return NS_FAIL;
	if(!pEntity->isKindOf(AcDbBlockReference::desc()))
	{
		pEntity->close();
		return NS_FAIL;
	}
	AcDbBlockReference *pRef = (AcDbBlockReference*)pEntity;
	AcGePoint3d p3dPosition  = pRef->position();
	pt.m_dXX = p3dPosition.x;
	pt.m_dYY = p3dPosition.y;
	pt.m_dZZ = p3dPosition.z;

	pRef->close();
	pEntity->close();
	return NS_SUCCESS;
}

int CNSDWGMgr::GetBlockObjectsWithInsertionPoint(std::map<AcDbObjectId,AcGePoint3d> &blockData)
{
	AcDbBlockTable *pBlockTable = NULL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord = NULL;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt;
	AcArray<AcGePolyline3d*> pLine3DArr;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{
		pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead);
		if(pEnt == NULL)
			continue;
		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcGePoint3dArray arrPoints;
			AcGePoint3d pTopLeft,pBottomLeft,pBottomRight,pTopRight;
			AcDbBlockReference* pBlock = (AcDbBlockReference*)pEnt;
			AcGePoint3d p3dPosition   = pBlock->position();
			AcDbObjectId id = pBlock->objectId();
			blockData.insert(std::map<AcDbObjectId,AcGePoint3d>::value_type(id,p3dPosition));
		}
		pEnt->close();
	}
	pBlockTableRecord->close();
	delete pBlkTblRcdItr;
	return NS_SUCCESS;
}

//#ifndef _INTERNAL_
int CNSDWGMgr::GetLineObjects(std::map<AcDbObjectId,std::vector<AcGePoint3d> > &lineData)
{

//	AcDbLinetypeTable *pLineTable =NULL;
//	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pLineTable, AcDb::kForRead))
//	{
//			return NS_FAIL;
//	}
//
//	AcDbLinetypeTableRecord *pLineTableRecord = NULL;
//
//	AcDbLinetypeTableIterator *pLineItr = NULL;
//pLineTable->newIterator(pLineItr);
//	AcDbEntity *pEnt;
//	AcArray<AcGePolyline3d*> pLine3DArr;
//for(pLineItr->start(); !pLineItr->done();pLineItr->step())
//{
//	pLineItr->getRecord(pLineTableRecord,AcDb::kForRead);
//	
//}

	AcDbBlockTable *pBlockTable = NULL;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord = NULL;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead,false))
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	pBlockTable->close();

	AcDbBlockTableRecordIterator *pBlkTblRcdItr = NULL;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt;
	AcArray<AcGePolyline3d*> pLine3DArr;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{		
		try
		{
		Acad::ErrorStatus et =	pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead);
		if(pEnt == NULL)
			continue;
		if(pEnt->isKindOf(AcDbLine::desc()))
		{
			std::vector<AcGePoint3d> PtsVect;
			AcDbLine* objLine = (AcDbLine*)pEnt;
			AcDbObjectId id = objLine->objectId();
			AcGePoint3d startPt = objLine->startPoint();
			AcGePoint3d endPt = objLine->endPoint();
			PtsVect.push_back(startPt);
			PtsVect.push_back(endPt);
			lineData.insert(std::map<AcDbObjectId,std::vector<AcGePoint3d> >::value_type(id,PtsVect));
		}
		pEnt->close();
		}
		catch(...){}
	}
	pBlockTableRecord->close();
	delete pBlkTblRcdItr;
	return NS_SUCCESS;
}
//#endif

bool CNSDWGMgr::isPointInsideBoundary(AcGePoint3d point,std::vector<CNSPoint3D>vectPoints)
{
	bool result = TRUE;
	double dMinX = 0.0, dMaxX = 0.0, dMinY = 0.0, dMaxY = 0.0;
	if(vectPoints[0].m_dXX < vectPoints[2].m_dXX)
	{
		dMinX = vectPoints[0].m_dXX;
		dMaxX = vectPoints[2].m_dXX;
	}
	else
	{
		dMaxX = vectPoints[0].m_dXX;
		dMinX = vectPoints[2].m_dXX;
	}
	if(vectPoints[0].m_dYY < vectPoints[2].m_dYY)
	{
		dMinY = vectPoints[0].m_dYY;
		dMaxY = vectPoints[2].m_dYY;
	}
	else
	{
		dMaxY = vectPoints[0].m_dYY;
		dMinY = vectPoints[2].m_dYY;
	}
	if((point.x < dMinX)||(point.x > dMaxX)||(point.y < dMinY)||(point.y > dMaxY))
		result = false;

	return result;
}

bool CNSDWGMgr::CheckIfLineWithinBlockBoundary(std::vector<CNSPoint3D>vectLinePoints, std::vector<AcGePoint3d> points)
{
	// We have a square area as boundary and need to check if given point
	// is within that square or not. No need to have any fancy algorithm. Just check
	// based on X,Y cordinates of the test point and boundary points.

	if(points.empty()) // || points.length()!=2)
		return false;

	bool result = true;

	result = isPointInsideBoundary(points[0],vectLinePoints);

	if(!result)
	{
		result = isPointInsideBoundary(points[1],vectLinePoints);
	}
	return result;
}

void CNSDWGMgr::purge()
{
	AcDbLayerTable* pLayerTable = NULL;
	AcDbBlockTable* pBlockTable = NULL;
	AcDbObjectIdArray aLayerIds,aBlockIds;
	Acad::ErrorStatus es;

	//Purge unused layers
	if ((es = m_pCurrentDatabase->getSymbolTable(pLayerTable, AcDb::kForWrite))== Acad::eOk) 
	{
		AcDbLayerTableIterator* pLayerIter;

		pLayerTable->newIterator(pLayerIter);
		AcDbLayerTableRecord* pRecord;
		for(pLayerIter->start(); !pLayerIter->done(); pLayerIter->step())
		{
			AcString strLayerName;
			if(pLayerIter->getRecord(pRecord, AcDb::kForRead) == Acad::eOk)
			{
				pRecord->getName(strLayerName);
				NSSTRING strLayer = strLayerName;
				getupperString((TCHAR*)strLayer.c_str());
				int nLength = strLayer.length();
				int n_NewFound = rFindString(strLayer, _T("_NEW"));
				int n_New_Found = strLayer.find(_T("_NEW_"));
				if(((n_NewFound!=-1) && (n_NewFound == nLength-4))||n_New_Found !=-1)
				{
					AcDbObjectId iLayerId = pRecord->id();
					aLayerIds.append(iLayerId);
				}
				pRecord->close();
			}
		}
		pLayerTable->close();
	}

	if(aLayerIds.length()>0)
	{
		es = m_pCurrentDatabase->purge(aLayerIds);
		if(es == Acad::eOk)
		{
			for(int i=0;i<aLayerIds.length();i++)
			{	
				AcDbObject* pObjLayer;
				es = acdbOpenObject(pObjLayer,aLayerIds.at(i),AcDb::kForWrite);
				if(es == Acad::eOk)
				{
					if(pObjLayer!=NULL && pObjLayer->isKindOf(AcDbLayerTableRecord::desc()))
					{
						AcDbLayerTableRecord* pLayerTableRecord = (AcDbLayerTableRecord*)pObjLayer;
						/*ACHAR*strLayerName = new ACHAR(25);
						pLayerTableRecord->getName(strLayerName);*/
						pLayerTableRecord->erase();
					}
					pObjLayer->close();
				}
			}
		}
	}

	//Purge unused _EXT blocks
	if ((es = m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForWrite))== Acad::eOk) 
	{
		AcDbBlockTableIterator* pBlockIter;


		pBlockTable->newIterator(pBlockIter);
		AcDbBlockTableRecord* pRecord;
		for(pBlockIter->start(); !pBlockIter->done(); pBlockIter->step())
		{
			// ACHAR* strBlockName = new ACHAR[_MAX_PATH+1];
			AcString strBlockName;
			if(pBlockIter->getRecord(pRecord, AcDb::kForRead) == Acad::eOk)
			{
				/*pRecord->getName(strBlockName);
				NSSTRING strBlock = strBlockName;

				int nLength = strBlock.length();
				//int nExtFound = strBlock.rfind(_T("_EXT"));
				int nExtFound = rFindString(strBlock, _T("_EXT"));
				if(nExtFound == -1)
				{
				//nExtFound = strBlock.rfind(_T("_ext"));
				nExtFound = rFindString(strBlock, _T("_ext"));
				}
				if((nExtFound!=-1) && (nExtFound == nLength-4))*/
				{
					AcDbObjectId iLayerId = pRecord->id();
					aBlockIds.append(iLayerId);
				}
				pRecord->close();
			}
		}
		pBlockTable->close();
	}

	if(aBlockIds.length()>0)
	{
		es = m_pCurrentDatabase->purge(aBlockIds);
		if(es == Acad::eOk)
		{
			for(int i=0;i<aBlockIds.length();i++)
			{	
				AcDbObject* pObjBlock;
				es = acdbOpenObject(pObjBlock,aBlockIds.at(i),AcDb::kForWrite);
				if(es == Acad::eOk)
				{
					if(pObjBlock!=NULL && pObjBlock->isKindOf(AcDbBlockTableRecord::desc()))
					{
						AcDbBlockTableRecord* pBlockTableRecord = (AcDbBlockTableRecord*)pObjBlock;
						/*ACHAR*strBlockName = new ACHAR(25);
						pBlockTableRecord->getName(strBlockName);*/
						pBlockTableRecord->erase();
					}
					pObjBlock->close();
				}
			}
		}
	}
}
int CNSDWGMgr::eraseGISErrorLayer()
{
	AcDbBlockTable *pBlockTable;
	if(Acad::eOk != m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))
	{
		return NS_FAIL;
	}
	AcDbBlockTableRecord *pBlockTableRecord;
	if(Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForRead))
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	pBlockTable->close();
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pBlockTableRecord->newIterator(pBlkTblRcdItr);

	//AcDbEntity *pEnt;
	//AcDbObjectId pObjId;

	
	for(pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{
		AcDbEntity* pEnt = NULL;
		AcDbObject* ppObj = NULL;
		Acad::ErrorStatus es ; 

		/*es = pBlkTblRcdItr->getEntityId(pObjId);*/

		es = pBlkTblRcdItr->getEntity(pEnt,AcDb::kForWrite);
		//Acad::ErrorStatus es = pBlkTblRcdItr->getEntity(pEnt,AcDb::kForWrite);
		if(pEnt == NULL)
			continue;

		TCHAR* pszLayerName = pEnt->layer();
	
		if(NSSTRCMP(pszLayerName, _T("GIS_ERRORS")) == 0)	
		{
			pEnt->erase(true);
		}

		pEnt->close();
	}
	//}
	pBlockTableRecord->close();
	delete pBlkTblRcdItr;


	return NS_SUCCESS;
}


/*
@Brief			Sets the currently active database to m_pCurrentDatabase
@Return			int		NS_SUCCESS
@Date			14 Sept 2007
*/
int CNSDWGMgr::setCurrentDatabaseActive()
{
	m_pCurrentDatabase = acdbHostApplicationServices()->workingDatabase();
	return NS_SUCCESS;
}

/*
@Brief			Creates LOG layout and writes validation report to it.
@Param [in]		TCHAR*	log string to write
@Return			int		NS_SUCCESS/NS_FAIL
@Date			14 Sept 2007
*/

#ifdef _INTERNAL_
int CNSDWGMgr::WriteToLoglayout(NSSTRING strLog)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbObjectId lytId;
	if(Acad::eOk != (AcDbSymbolUtilities::getLayerId(lytId,_T("VALIDATION_LOG"), m_pCurrentDatabase)))
	{
		addNewLayer(_T("VALIDATION_LOG"), lytId);
	}

	if(lytId != NULL)
	{
		struct resbuf rbFilter;
		ads_name SelectionSet;
		TCHAR sBuf[15];
		rbFilter.restype = 8; // Layer name 
		NSSTRCPY(sBuf, _T("VALIDATION_LOG"));
		rbFilter.resval.rstring = sBuf; 
		rbFilter.rbnext = NULL;
		acedSSGet(_T("X"), NULL, NULL, &rbFilter, SelectionSet);
		//delete all elements in the selection set

		long  SelectionSetLength = 0;
		NSSTRING strMsg;
		acedSSLength(SelectionSet, &SelectionSetLength);
		for (int i = 0; i < SelectionSetLength; i++)
		{
			ads_name entToDelete;
			acedSSName(SelectionSet, i, entToDelete);
			AcDbObjectId objIdToBeDel;
			if(Acad::eOk != acdbGetObjectId(objIdToBeDel,entToDelete))
				continue;
			AcDbObject* objToBeDel = NULL;
			if(Acad::eOk != acdbOpenObject(objToBeDel,objIdToBeDel, AcDb::kForWrite))
				continue;
			if(Acad::eOk != objToBeDel->erase())
				continue;
			objToBeDel->close();
		}

		struct resbuf *cmdlist;
		cmdlist = acutBuildList(5005, _T("regen"), 0); // 5005 represents RTSTR
		if (cmdlist == NULL)
		{
			return NS_FAIL;
		}
		acedCmd(cmdlist);
		acutRelRb(cmdlist);

		acedSSFree(SelectionSet);


		// get the insertion point of the text report from export area stores in Title block attributes
		std::vector<CNSPoint3D>vectPointsofInterest;
		std::map<NSSTRING, NSSTRING>mapTitleAttributes;
		std::map<NSSTRING, NSSTRING> mapofPrompValues;
		mapTitleAttributes.insert(std::map<NSSTRING,NSSTRING>::value_type(_T("ExportAreaOfInterest"),_T("")));
		listTitleBlockHeaderAttributes(mapTitleAttributes,mapofPrompValues);
		NSSTRING strAttValues = mapTitleAttributes[_T("ExportAreaOfInterest")];
		convertString2Vertices(strAttValues,vectPointsofInterest);

		double dInsXX, dInsYY;
		dInsXX = dInsYY = 0.0;
		if(vectPointsofInterest.size() > 3)
		{
			dInsXX = vectPointsofInterest[3].m_dXX;
			dInsYY = vectPointsofInterest[3].m_dYY;
		}
		addTextElement((TCHAR*)strLog.c_str(),dInsXX, dInsYY, 0.0,_T("VALIDATION_LOG"));
		//switch off the layer
		AcDbObject* pLayerObj;
		es = acdbOpenObject(pLayerObj,lytId,AcDb::kForWrite);
		if(es == Acad::eOk)
		{
			AcDbLayerTableRecord* pLayerTblRec = (AcDbLayerTableRecord*)pLayerObj;
			if(pLayerTblRec!=NULL)
				pLayerTblRec->setIsOff(true);
			pLayerObj->close();
		}
	}
	return NS_SUCCESS;
}
#endif

/*
@Brief			Saves a drawing depending upon a file format
@Param [in]		TCHAR*	File name
@Param [in]		bool	drawing format true: bAs2004, false: bAs2007
@Return			int		NS_SUCCESS/NS_FAIL
@Date			14 June 2007
*/
int CNSDWGMgr::saveActiveDWGAs2004()
{
	Acad::ErrorStatus st = Acad::eOk;
	if(m_pCurrentDatabase != NULL)
	{
		st = m_pCurrentDatabase->save();
	}
	if(st == Acad::eFileSharingViolation) 
		return NS_SHARING_VIOLATION;
	return ((st == Acad::eOk) ? NS_SUCCESS : NS_FAIL);
}


int CNSDWGMgr::isPaperSpaceValid()
{
	AcDbBlockTable*        pBlockTable = NULL;
	AcDbBlockTableRecord*  pPspaceRecord = NULL;
	Acad::ErrorStatus es;

	if ((es = m_pCurrentDatabase->getSymbolTable(pBlockTable, AcDb::kForRead))!= Acad::eOk) 
		return NS_FAIL;
	if ((es = pBlockTable->getAt(ACDB_PAPER_SPACE, pPspaceRecord, AcDb::kForRead)) != Acad::eOk) 
	{
		pBlockTable->close();
		return NS_FAIL;
	}
	if ((es = pBlockTable->close()) != Acad::eOk) 
	{
		pPspaceRecord->close();
		return NS_FAIL;
	}

	int nRetValue = NS_SUCCESS;

	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	pPspaceRecord->newIterator(pBlkTblRcdItr);

	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done();pBlkTblRcdItr->step())
	{
		pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead);
		if(pEnt == NULL)
			continue;

		NSSTRING strLayer = pEnt->layer();
		if((NSSTRCMP(_T("LEGENDS"), strLayer.c_str()) == 0) || (NSSTRCMP(_T("NOTES"),strLayer.c_str()) == 0))
		{
			pEnt->close();
			continue;
		}

		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
			AcDbObjectId blockId = pBlockRef->blockTableRecord();
			AcDbBlockTableRecord *pRecord = 0;
			acdbOpenObject(pRecord, blockId, AcDb::kForRead);
			if (pRecord)
			{
				TCHAR *pName;
				if (pRecord->getName(pName) == Acad::eOk)
				{
					NSSTRING strBlkName(pName);
					//size_t value = strBlkName.find(_T("_TITLE"));
					size_t value = findString(strBlkName, _T("_TITLE"));
					if (value == NSSTRING::npos)
					{
						pEnt->close();
						nRetValue = NS_ERR_PAPERSPACE;
						pRecord->close();
						break;
					}
				}
			}
			pRecord->close();
			pEnt->close();
		}
		else
		{
			if(pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pLine = (AcDbLine*)pEnt;
				//if(strLayer.find(_T("_NEW_"))) // check if its on any cable layer
				if(findString(strLayer, _T("_NEW_")) != -1) // check if its on any cable layer
				{
					pEnt->close();
					nRetValue = NS_ERR_PAPERSPACE;
					break;
				}
			}
			pEnt->close();
		}
	}
	pPspaceRecord->close();
	delete pBlkTblRcdItr;
	return nRetValue;
}


/*
@brief			Compares the values of different title blocks and stores it in the map.
@Param(in)		MAP containing title block attributes and their values.
@Param(out)		MAP containing the name of attributes whose values are different.
*/
int CNSDWGMgr::validateTitleBlockAttributes(/*std::map<NSSTRING, NSSTRING> mapTitleAttributes,*/ std::map<NSSTRING, NSSTRING> &mapTitleAttributesNames)
{
	std::map<NSSTRING, NSSTRING> mapTitleAttributes;
	std::map<NSSTRING, NSSTRING> mapTitleAttributesTemp;
	STRING_MAP mapOfPromptAttributes;
	int nTitleBlockCount = 0;

	AcDbBlockTable* pBTable = NULL;
	m_pCurrentDatabase->getBlockTable(pBTable, AcDb::kForRead);
	AcDbBlockTableRecord* pTRecord = NULL;
	bool bTitleBlockFound = false;
	int Loop=1;
	try
	{
		for(Loop=1;Loop<5;Loop++)
		{
			if(bTitleBlockFound == true)
			{
				continue;
			}
			//AcDbObjectId id;
			NSSTRING strFrame;
			if(Loop==1)
			{
				strFrame=_T("A1_FRAME_TITLE");
			}
			else if(Loop ==2)
			{
				strFrame=_T("A2_FRAME_TITLE");
			}
			else if(Loop ==3)
			{
				strFrame=_T("A3_FRAME_TITLE");
			}
			else if(Loop ==4)
			{
				strFrame=_T("A4_FRAME_TITLE");
			}
			else
				continue;

			AcDbEntity* pEnt = NULL;
			AcDbObjectId recId;
			if(Acad::eOk != pBTable->getAt(strFrame.c_str(),pTRecord, AcDb::kForRead))
			{
				continue;
			}

			STRING_MAP::iterator theTempItr;
			NSSTRING strCompareValue, strTempValue;
			if(nTitleBlockCount == 0)
			{				
				//getPromptValues(recId.asOldId(), mapOfPromptAttributes);				
				if(Acad::eOk != pTRecord->getBlockReferenceIds(ids))
					continue;

				//pTRecord->close();

				for(int i=0;i<ids.length();i++)
				{
					if(i>0)
						continue;

					AcDbObject *pObj;
					AcDbObjectId id = ids[i];				
					//if (pBlockReference.openStatus() == Acad::eOk)
					if(Acad::eOk==acdbOpenObject(pObj,id,AcDb::kForRead))
					{
						AcDbBlockReference *pBlkRef =AcDbBlockReference::cast(pObj);
						if(pBlkRef == NULL)
							continue;

						// Get attributes with empty values
						AcDbObjectIterator* itrAttr = pBlkRef->attributeIterator();
						STRING_MAP mapOfEmptyAttributes;
						for(itrAttr->start(); !itrAttr->done(); itrAttr->step())
						{
							AcDbAttribute* pAttr = NULL;
							AcDbObjectId AttrId = itrAttr->objectId();
							if(Acad::eOk != pBlkRef->openAttribute(pAttr, AttrId, AcDb::kForRead))
							{
								continue;
							}
							TCHAR* pszAttrValue = pAttr->textString();
							if(NSSTRCMP(pszAttrValue, _T("")) == 0)
							{
								mapOfEmptyAttributes.insert(STRING_MAP::value_type(pAttr->tag(),_T("")));
							}
							pAttr->close();
						}
						delete itrAttr;

						//Get all Attributes
						AcDbBlockTableRecordIterator *pBTRItr;
						pTRecord->newIterator(pBTRItr);

						for(pBTRItr->start(); !pBTRItr->done(); pBTRItr->step())
						{
							AcDbEntity* pAttDefEnt = NULL;
							if (Acad::eOk ==pBTRItr->getEntity(pAttDefEnt, AcDb::kForRead))
							{
								if(pAttDefEnt == NULL)
								{
									continue;
								}
								if(pAttDefEnt->isKindOf(AcDbAttributeDefinition::desc()))
								{
									AcDbAttributeDefinition *pTempAttdef = (AcDbAttributeDefinition*)(pAttDefEnt);
									TCHAR* pszAttDefTag = pTempAttdef->tag();
									STRING_MAP::iterator itrMapofEmptyAttr = mapOfEmptyAttributes.find(pszAttDefTag);
									if(itrMapofEmptyAttr == mapOfEmptyAttributes.end())
									{
										pAttDefEnt->close();
										continue;
									}
									TCHAR* pszAttDefPrompt = pTempAttdef->prompt();
									NSSTRING strAttDefPrompt = (NSSTRING)pszAttDefPrompt;
									int nDot = strAttDefPrompt.find_last_of(_T("("));
									strAttDefPrompt = strAttDefPrompt.substr(nDot+1, (strAttDefPrompt.length()-2));
									if(NSSTRCMP(strAttDefPrompt.c_str(),_T("GIS)")) == 0)
									{									
										mapOfPromptAttributes.insert(NSSTRING_MAP::value_type(pszAttDefTag, pszAttDefPrompt));
									}
								}
								pAttDefEnt->close();
							}
						}
						delete pBTRItr;
					}
				}		


			}					

			if(Acad::eOk != pTRecord->getBlockReferenceIds(ids))
				continue;			

			for(int i=0;i<ids.length();i++)
			{
				if(i>0)
					continue;

				AcDbObject *pObj;
				AcDbObjectId id = ids[i];				
				//if (pBlockReference.openStatus() == Acad::eOk)
				if(Acad::eOk==acdbOpenObject(pObj,id,AcDb::kForRead))
				{
					AcDbBlockReference *pTitleBlockReference =AcDbBlockReference::cast(pObj);
					if(pTitleBlockReference == NULL)
						continue;

					AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
					for(pIter->start(); !pIter->done(); pIter->step())
					{
						AcDbObjectId AttrId = pIter->objectId();
						AcDbAttribute *pAtt;
						if (Acad::eOk != pTitleBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
						{
							pTitleBlockReference->close();
							return NS_FAIL;
						}
						// code for checking if attribute is present in the map and then setting value should come here            
						const TCHAR* pszAttName = pAtt->tag();
						NSSTRING szAttName = pszAttName;
						NSSTRING szAttValue = pAtt->textString();
						std::map<NSSTRING, NSSTRING>::iterator itrMap;
						if(nTitleBlockCount == 0)
						{
							mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, szAttValue));
						}
						else
						{
							for(itrMap = mapTitleAttributes.begin(); itrMap != mapTitleAttributes.end(); itrMap++)
							{
								bool bGISPrompt = false;
								for(theTempItr = mapOfPromptAttributes.begin(); theTempItr != mapOfPromptAttributes.end(); theTempItr++)
								{
									if(NSSTRCMP(szAttName.c_str(), ((*theTempItr).first).c_str()) == 0)
									{
										bGISPrompt = true;
										break;
									}
								}
								if(bGISPrompt == true)
								{
									if(NSSTRCMP(szAttValue.c_str(),((*itrMap).second).c_str()) != 0)
									{
										mapTitleAttributesNames.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, _T("")));
									}
								}
							}			
						}
						pAtt->close();
					}
					delete pIter;
				}	
			}
		}
	}
	catch(...){}
	return NS_SUCCESS;



//************************************************************Original***********************************************************

//	std::map<NSSTRING, NSSTRING> mapTitleAttributes;
//	std::map<NSSTRING, NSSTRING> mapTitleAttributesTemp;
//	STRING_MAP mapOfPromptAttributes;
//	// Open the current database's LayoutDictionary.
//	AcDbDictionary* pLytDict = NULL;
//	if(Acad::eOk != m_pCurrentDatabase->getLayoutDictionary(pLytDict, AcDb::kForRead))
//		return NS_FAIL;
//
//	AcDbObject* pObjLyt = NULL;
//
//	//Iterate layout dictionary
//	int nTitleBlockCount = 0;
//	AcDbDictionaryIterator* pDicIter = pLytDict->newIterator();
//	for(; !pDicIter->done(); pDicIter->next())
//	{
//		if(Acad::eOk != pDicIter->getObject(pObjLyt, AcDb::kForWrite))
//		{
//			pLytDict->close();
//			//	return NS_FAIL;
//			continue;
//		}
//		 const ACHAR* layoutName;
//		((AcDbLayout*)pObjLyt)->getLayoutName(layoutName);
//
//if(NSSTRCMP(layoutName,_T("Model")) == 0)
//{
//pObjLyt->close();
//pLytDict->close();
//continue;
//}
//
//		AcDbObjectId layoutBTRId = ((AcDbLayout*)pObjLyt)->getBlockTableRecordId();
//		pObjLyt->close();
//
//		AcDbBlockTableRecord* pBlockTableRecord = NULL;
//		if(Acad::eOk != acdbOpenObject(pBlockTableRecord,layoutBTRId,AcDb::kForRead))
//		{
//			pObjLyt->close();
//			pLytDict->close();
//			continue;
//			//	return NS_FAIL;
//		}
//
//		AcDbBlockTableRecordIterator *pBlockIterator = NULL;
//		if(Acad::eOk != pBlockTableRecord->newIterator(pBlockIterator))
//		{
//			pLytDict->close();
//			pBlockTableRecord->close();
//			continue;
//		}
//
//		for (pBlockIterator->start(); !pBlockIterator->done(); pBlockIterator->step())
//		{
//			AcDbEntity* pEnt = NULL;
//			Acad::ErrorStatus es=	pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
//			if(pEnt == NULL)
//				continue;
//			TCHAR *t = (ACHAR *)AcDbBlockReference::desc()->appName();			
//
//			if(!pEnt->isKindOf(AcDbBlockReference::desc()))
//			{
//				pLytDict->close();
//				pEnt->close();
//				continue;
//			}
//
//			AcDbBlockReference* pTitleBlockReference = (AcDbBlockReference*)pEnt;
//			AcDbObjectId blkTblID = pTitleBlockReference->blockTableRecord();
//			AcDbBlockTableRecord* blkTblRecord = NULL;
//			if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
//			{
//				pEnt->close();
//				//pLytDict->close();
//				continue;//return NS_FAIL;
//			}
//
//			// Get block name
//			TCHAR* pszInsertedBlockName = NULL;
//			blkTblRecord->getName(pszInsertedBlockName);
//			blkTblRecord->close();
//
//			NSSTRING strInsertedBlockName = NSSTRING(pszInsertedBlockName);
//			if(strInsertedBlockName.size() > 6)
//			{
//				NSSTRING szBlockNameSuffix = strInsertedBlockName.substr((strInsertedBlockName.size()-6), 6);
//				if(NSSTRCMP(szBlockNameSuffix.c_str(), _T("_TITLE")) != 0 )
//				{
//					pEnt->close();
//					continue;
//				}
//			}	
//			else
//			{
//				blkTblRecord->close();
//				pEnt->close();
//				continue;
//			}
//
//			/////////////////////////
//
//			STRING_MAP::iterator theTempItr;
//			NSSTRING strCompareValue, strTempValue;
//			if(nTitleBlockCount == 0)
//			{
//				pEnt->close();
//				getPromptValues(pEnt->id().asOldId(), mapOfPromptAttributes);
//				pBlockIterator->getEntity(pEnt, AcDb::kForWrite);
//				if(pEnt == NULL)
//					continue;
//			}
//
//
//			////////////////////////
//			AcDbObjectIterator* pIter = pTitleBlockReference->attributeIterator();
//
//			for(pIter->start(); !pIter->done(); pIter->step())
//			{
//				AcDbObjectId AttrId = pIter->objectId();
//				AcDbAttribute *pAtt;
//				if (Acad::eOk != pTitleBlockReference->openAttribute(pAtt, AttrId, AcDb::kForRead))
//				{
//					pLytDict->close();
//					pTitleBlockReference->close();
//					return NS_FAIL;
//				}
//
//				// code for checking if attribute is present in the map and then setting value should come here            
//				const TCHAR* pszAttName = pAtt->tag();
//				NSSTRING szAttName = pszAttName;
//				NSSTRING szAttValue = pAtt->textString();
//
//				std::map<NSSTRING, NSSTRING>::iterator itrMap;
//				if(nTitleBlockCount == 0)
//				{
//					mapTitleAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, szAttValue));
//				}
//				else
//				{
//					for(itrMap = mapTitleAttributes.begin(); itrMap != mapTitleAttributes.end(); itrMap++)
//					{
//						bool bGISPrompt = false;
//						for(theTempItr = mapOfPromptAttributes.begin(); theTempItr != mapOfPromptAttributes.end(); theTempItr++)
//						{
//							if(NSSTRCMP(szAttName.c_str(), ((*theTempItr).first).c_str()) == 0)
//							{
//								bGISPrompt = true;
//								break;
//							}
//						}
//						if(bGISPrompt == true)
//						{
//							if(NSSTRCMP(szAttValue.c_str(),((*itrMap).second).c_str()) != 0)
//							{
//								mapTitleAttributesNames.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, _T("")));
//							}
//						}
//
//					}
//					//	if(itrMap == mapTitleAttributes.end())
//					//	{ 
//					//		mapTitleAttributesNames.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, szAttValue));
//					//	}
//				}
//				pAtt->close();
//			}
//			delete pIter;
//
//			pEnt->close();
//			pTitleBlockReference->close();
//			nTitleBlockCount = nTitleBlockCount + 1;
//		}	
//		delete pBlockIterator;
//		pBlockTableRecord->close();
//	}
//	pLytDict->close();
//	delete pDicIter;
//	return NS_SUCCESS;

//********************************************************************************************************************************
}

/*
@brief			Stores the prompt name and the tag of attribute of the given object id in map.
@Param(in)		Object ID
@Param(out)		MAP containing the name of attribute and its prompt name.
*/
int CNSDWGMgr::getPromptValues(long lobjID, std::map<NSSTRING, NSSTRING> &mapofPrompValues)
{
	Acad::ErrorStatus es;
	AcDbObjectId objID;
	objID.setFromOldId(lobjID);

	AcDbEntity *pEnt = NULL;
	if(Acad::eOk != (es = acdbOpenObject(pEnt, objID, AcDb::kForRead)))
	{
		return NS_FAIL;
	}

	if(!pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		pEnt->close();
		return NS_FAIL;;
	}

	// Get attributes with empty values
	AcDbBlockReference* pBlkRef = (AcDbBlockReference*)pEnt;
	AcDbObjectIterator* itrAttr = pBlkRef->attributeIterator();
	STRING_MAP mapOfEmptyAttributes;
	for(itrAttr->start(); !itrAttr->done(); itrAttr->step())
	{
		AcDbAttribute* pAttr = NULL;
		AcDbObjectId AttrId = itrAttr->objectId();
		if(Acad::eOk != pBlkRef->openAttribute(pAttr, AttrId, AcDb::kForRead))
		{
			continue;
		}

		TCHAR* pszAttrValue = pAttr->textString();
		if(NSSTRCMP(pszAttrValue, _T("")) == 0)
		{
			mapOfEmptyAttributes.insert(STRING_MAP::value_type(pAttr->tag(),_T("")));
		}
		pAttr->close();
	}
	delete itrAttr;


	TCHAR *t = (ACHAR *)AcDbBlockReference::desc()->appName();

	AcDbBlockReference* pTitleBlockReference = (AcDbBlockReference*)pEnt;
	AcDbObjectId blkTblID = pTitleBlockReference->blockTableRecord();
	AcDbBlockTableRecord* blkTblRecord = NULL;
	if(Acad::eOk != (es = acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead)))
	{
		pEnt->close();
		return NS_FAIL;
	}  

	NSSTRING_MAP tempMapAttPrompt;
	AcDbBlockTableRecordIterator *pBTRItr;
	blkTblRecord->newIterator(pBTRItr);
	for(pBTRItr->start(); !pBTRItr->done(); pBTRItr->step())
	{
		AcDbEntity* pAttDefEnt = NULL;
		Acad::ErrorStatus es =  pBTRItr->getEntity(pAttDefEnt, AcDb::kForRead);
		if(es != Acad::eOk)
			continue;
		if(pAttDefEnt->isKindOf(AcDbAttributeDefinition::desc()))
		{
			AcDbAttributeDefinition *pTempAttdef = (AcDbAttributeDefinition*)(pAttDefEnt);
			TCHAR* pszAttDefTag = pTempAttdef->tag();

			// If the attribute value is not empty do not consider the atribute tag and prompt
			STRING_MAP::iterator itrMapofEmptyAttr = mapOfEmptyAttributes.find(pszAttDefTag);
			if(itrMapofEmptyAttr == mapOfEmptyAttributes.end())
			{
				pAttDefEnt->close();
				continue;
			}

			//TCHAR* pszAttDefValue = pTempAttdef->textString();

			//if(NSSTRCMP(pszAttDefValue, _T("")) == 0)
			{
				TCHAR* pszAttDefPrompt = pTempAttdef->prompt();
				NSSTRING strAttDefPrompt = (NSSTRING)pszAttDefPrompt;
				int nDot = strAttDefPrompt.find_last_of(_T("("));
				if((int)strAttDefPrompt.size() > 2)
				{
					strAttDefPrompt = strAttDefPrompt.substr(nDot+1, (strAttDefPrompt.length()-2));
					if(NSSTRCMP(strAttDefPrompt.c_str(),_T("GIS)")) == 0)
						mapofPrompValues.insert(NSSTRING_MAP::value_type(pszAttDefTag, pszAttDefPrompt));
				}
			}
		}
		pAttDefEnt->close();
	}
	delete pBTRItr;
	pEnt->close();
	blkTblRecord->close();
	return NS_SUCCESS;
}

int CNSDWGMgr::rFindString(NSSTRING strBaseString, NSSTRING strStringToBeFind)
{
#ifndef _INTERNAL_
	NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );
#else
	NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );
	/*NSSTRUPR((TCHAR*)strBaseString.c_str());
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str());*/
#endif
	return (int)strBaseString.rfind(strStringToBeFind);
}

int CNSDWGMgr::findString(NSSTRING strBaseString, NSSTRING strStringToBeFind)
{
#ifndef _INTERNAL_
	NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );	
#else
	NSSTRUPR((TCHAR*)strBaseString.c_str(), strBaseString.size()+1);
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str(), strStringToBeFind.size()+1 );	
	/*NSSTRUPR((TCHAR*)strBaseString.c_str());
	NSSTRUPR((TCHAR*)strStringToBeFind.c_str());	*/
#endif
	return (int)strBaseString.find(strStringToBeFind);
}
/*
@brief			Displays standard drawing version number in Validation Report
@Param(in)		pStdDatabase :- Standard drawing.
@Param(out)		Version
*/
NSSTRING CNSDWGMgr::getVersionInfo(CNSDWGMgr &pStdDatabase)
{
	double dval;
	NSSTRING strVersion=L"";
	AcDbDictionary *pNamedobj;
	AcDbBlockTable *pBlockTable;
	Acad::ErrorStatus er;
	AcDbDatabase* srcDb = pStdDatabase.m_pCurrentDatabase;

	er = srcDb->getSymbolTable(pBlockTable, AcDb::kForWrite);
	pBlockTable->close();
	er = srcDb->getNamedObjectsDictionary(pNamedobj, AcDb::kForRead);
	AcDbDictionary *pDict;
	er = pNamedobj->getAt(_T("EEVER_DICT"), (AcDbObject*&)pDict,
		AcDb::kForRead);
	pNamedobj->close();
	if(er == Acad::eOk)
	{
		AcDbXrecord *pXrec;
		er = pDict->getAt(_T("Version_No"), (AcDbObject*&) pXrec,
			AcDb::kForRead);
		pDict->close();
		if(er == Acad::eOk)
		{
			struct resbuf *pRbList;
			pXrec->rbChain(&pRbList);
			//acharVersion = pRbList->resval.rstring;
			dval = pRbList->resval.rreal;
			std::wstringstream os;
			os << dval;  
			strVersion = os.str(); 
			size_t nResult = strVersion.find_last_of(_T("."));
			if (nResult == std::string::npos)// no decimal found
			{ 
				strVersion.append(_T(".0"));
			}
			pXrec->close();
			acutRelRb(pRbList);
		}
		return strVersion;
	}
	return strVersion;
}

int CNSDWGMgr::scanAssets(ASSET_VECTOR &m_vectofAssets)
{
	// Get Model-space Block table record
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;
	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	pBlockTable->close();

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Attributes and XDATA) of model space pBlockRef entity into a CNSAsset
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		//pBlockTable->close();
		return NS_FAIL;
	}

	NSSTRING strLayerNameSuffix = NSSTRING(_T("_NEW")); // function parameter
	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			//pRecord->close();
			//pBlockTable->close();
			//delete pBlkTblRcdItr;
			continue;
		}
		// if the entity is a pblockref get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		// Select Liner features on given layer
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());
		strLayerNameSuffix = _T("_NEW");

		if(pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			if(!checkStringSuffix(strEntityLayer,strLayerNameSuffix))
			{
				pEnt->close();
				continue;
			}
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			continue;
			//pRecord->close();
			//pBlockTable->close();
			//return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
			//pRecord->close();
			//pBlockTable->close();
			//return NS_FAIL;
		}
		TCHAR szTemp[_MAX_PATH + 1];

		NSSTRCPY(szTemp, pszInsertedBlockName1);

		NSSTRING strEntBlkName(szTemp);		

		getEffectiveName( blkTblRecord,  pBlockRef, strEntBlkName/*,  pszInsertedBlockName*/);

		if(!checkStringSuffix(strEntBlkName,_T("_NEW")))
		{
			pEnt->close();
			continue;
		}

		CNSAsset *pCNSAsset = new CNSAsset;
		/*pCNSAsset->setObjectID((long) pEnt->id().asOldId());
		pCNSAsset->setBlockName(pszInsertedBlockName);
		pCNSAsset->setLayerName(NSSTRING(pEnt->layer()));*/

		// Get attributes of the pBlockRef entity
		AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
		AcDbAttribute* pTempAttribute = NULL;
		NSSTRING strUsage;
		NSSTRING strOrientation;
		NSSTRING strAssetNumber;
		bool bHasAssetNum = false;
		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				//pTempAttribute->close();
				//delete attributeIterator;
				//blkTblRecord->close();
				//pBlockRef->close();
				//pEnt->close();
				//pRecord->close();
				//pBlockTable->close();				
				//return NS_FAIL;
				continue;
			}			
			TCHAR* pszKey= pTempAttribute->tag();
			if(NSSTRCMP(pszKey, _T("ASSET_NUM")) == 0)
			{
				strAssetNumber = pTempAttribute->textString();	
				pCNSAsset->setObjectID((long) pEnt->id().asOldId());
				pCNSAsset->setBlockName(strEntBlkName);
				pCNSAsset->setLayerName(NSSTRING(pEnt->layer()));
				pCNSAsset->setAssetNumber(strAssetNumber);
				bHasAssetNum = true;
			}
			//else if(NSSTRCMP(pszKey, _T("USAGE")) == 0)
			//{
			//	strUsage = pTempAttribute->textString();
			//}
			//else if(NSSTRCMP(pszKey, _T("ORIENTATION")) == 0)
			//{
			//	strOrientation = pTempAttribute->textString();
			//}
			TCHAR* strVal = pTempAttribute->textString();
			getupperString(pszKey);
			pCNSAsset->m_mapOfAttributes.insert(std::map<NSSTRING,NSSTRING>::value_type(pszKey,strVal));

			if(pTempAttribute != NULL)
				pTempAttribute->close();

		}
		delete attributeIterator;
		if(bHasAssetNum == true)
		{
			m_vectofAssets.push_back(pCNSAsset);
		}
		else
		{
			delete pCNSAsset;
			pCNSAsset = NULL;
		}
		/*if(NSSTRCMP(strAssetNumber.c_str(), _T("")) == 0)
		{
		//::CoInitialize(NULL);
		if(CNSDatabaseMgr::getInstance()->openDatabase(_T(""), _T(""), _T("GISCAD")) != NS_FAIL) 
		{
		TCHAR szQuery[_MAX_PATH + 1];
		sprintf(szQuery, _T("Select FeatureName, fno from BlockFeatures where NewBlock = \'%s\' and Usage = \'%s\' and Orientation = \'%s\'"),
		pszInsertedBlockName, strUsage.c_str(), strOrientation.c_str());
		int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(szQuery);
		_variant_t vFeatureName, vFNO;
		if(nDBRet != -1)
		{
		bool bResult = CNSDatabaseMgr::getInstance()->getValue("FeatureName", vFeatureName);
		if(bResult)
		{
		bResult = CNSDatabaseMgr::getInstance()->getValue("fno", vFNO);
		int nFNO;
		if(bResult)
		{
		nFNO = vFNO.intVal;
		}
		NSSTRING strKey = (NSSTRING)((_bstr_t)(vFeatureName.bstrVal));
		std::map<NSSTRING, ASSET_VECTOR*>::iterator it = m_mapofAssets.find(strKey);
		ASSET_VECTOR* pVector;
		if(it == m_mapofAssets.end())
		{
		pVector = new ASSET_VECTOR;
		pCNSAsset->setFNO(nFNO);
		pVector->push_back(pCNSAsset);
		m_mapofAssets.insert(std::map<NSSTRING, ASSET_VECTOR*>::value_type(strKey, pVector));
		}
		else
		{
		pVector = (*it).second;
		pVector->push_back(pCNSAsset);
		}
		}
		}
		CNSDatabaseMgr::getInstance()->closeDatabase();
		}
		//::CoUninitialize();
		}*/
		if(pEnt != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();
		blkTblRecord->close();

		// Add the CNSAsset to the map to be returned
	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	//if(pBlockTable != NULL)
	//pBlockTable->close();
	return NS_SUCCESS;
}

//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************
//int CNSDWGMgr::setAssetNumber(long blkObjID, NSSTRING strAssetNo)
//{
//	AcDbEntity *pEntity = NULL;
//	AcDbObjectId pBlockObjID;
//	pBlockObjID.setFromOldId(blkObjID);
//	Acad::ErrorStatus es =  acdbOpenObject(pEntity, pBlockObjID, AcDb::kForWrite);
//	if(es != Acad::eOk) return NS_FAIL;
//	AcDbBlockReference *pRef = (AcDbBlockReference*)pEntity;
//	AcDbObjectIterator *pObjItr = pRef->attributeIterator();
//	for(pObjItr->start(); !pObjItr->done(); pObjItr->step())
//	{
//		AcDbObjectId attID = pObjItr->objectId();
//		AcDbAttribute *pAttribute;
//		es = pRef->openAttribute(pAttribute, attID, AcDb::kForWrite);
//		if(es == Acad::eOk)
//		{
//			const TCHAR* szAttName = pAttribute->tag();
//			if(NSSTRCMP(szAttName, _T("ASSET_NUM")) == 0)
//			{
//				pAttribute->setTextString(strAssetNo.c_str());
//			}	
//		}
//		if(pAttribute != NULL)
//			pAttribute->close();
//	}
//	pRef->close();
//	pEntity->close();
//	delete pObjItr;
//	return NS_SUCCESS;
//}
//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************

//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************
//int CNSDWGMgr::getBlockName(long nBlockObjID, NSSTRING &strBlockName)
//{
//	Acad::ErrorStatus es;
//	AcDbObject *pEntity;
//	AcDbObjectId pBlockObjID;
//	pBlockObjID.setFromOldId(nBlockObjID);
//	es = acdbOpenObject(pEntity, pBlockObjID, AcDb::kForRead);
//	if(es != Acad::eOk)
//		return NS_FAIL;
//	if(!pEntity->isKindOf(AcDbBlockReference::desc()))
//	{
//		pEntity->close();
//		return NS_FAIL;
//	}
//	AcDbBlockReference *pRef = (AcDbBlockReference*)pEntity;
//	// we can get the name from the BlockTableRecord type
//	AcDbObjectId blkTblID = pRef->blockTableRecord();
//	AcDbBlockTableRecord* blkTblRecord = NULL;
//	if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
//	{
//		pRef->close();
//		pEntity->close();
//		return NS_FAIL;
//	}
//
//	// Set pBlockRef name
//	TCHAR* pszBlockName = NULL;
//	if(Acad::eOk != blkTblRecord->getName(pszBlockName))
//	{
//		pRef->close();
//		pEntity->close();
//		blkTblRecord->close();
//		return NS_FAIL;
//	}
//
//	strBlockName = NSSTRING(pszBlockName);
//	pRef->close();
//	pEntity->close();
//	blkTblRecord->close();
//	return NS_SUCCESS;
//}
//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************

//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************
//int CNSDWGMgr::readBlockAttributes(long nBlockObjID, std::map<NSSTRING, NSSTRING> & mapOfAttributes)
//{
//	Acad::ErrorStatus es;
//	AcDbObject *pEntity;
//	AcDbObjectId pBlockObjID;
//	pBlockObjID.setFromOldId(nBlockObjID);
//	es = acdbOpenObject(pEntity, pBlockObjID, AcDb::kForRead);
//	if(es != Acad::eOk)
//		return NS_FAIL;
//	if(!pEntity->isKindOf(AcDbBlockReference::desc()))
//	{
//		pEntity->close();
//		return NS_FAIL;
//	}
//	AcDbBlockReference *pRef = (AcDbBlockReference*)pEntity;
//	AcDbObjectIterator *pObjItr = pRef->attributeIterator();
//	for(pObjItr->start(); !pObjItr->done(); pObjItr->step())
//	{
//		AcDbObjectId attID = pObjItr->objectId();
//		AcDbAttribute *pAttribute;
//		Acad::ErrorStatus es = pRef->openAttribute(pAttribute, attID, AcDb::kForRead);
//		if(es == Acad::eOk)
//		{
//			TCHAR* szAttName = pAttribute->tag();
//			getupperString(szAttName);
//			const TCHAR* szValue = pAttribute->textString();	
//			mapOfAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(szAttName, szValue));
//		}
//		if(pAttribute != NULL)
//			pAttribute->close();
//	}
//	pRef->close();
//	pEntity->close();
//	delete pObjItr;
//	return NS_SUCCESS;
//}
//**************************************************** Commented on 31-Jan-2014 for 64-bit conversion ******************************************************


int CNSDWGMgr::getAssetNumbers(std::vector<CNSAsset*> & assetVector)
{
	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;
	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	pBlockTable->close();

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Attributes and XDATA) of model space pBlockRef entity into a CNSAsset
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			continue;
		}
		// if the entity is a pblockref get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		// Select Liner features on given layer
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());

		////Asset selection issue
		bool bIsNew = checkStringSuffix(strEntityLayer, _T("_NEW"));
		if(bIsNew == false)
		{
			pEnt->close();
			continue;
		}

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			continue;
		}

		// Set pBlockRef name
		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
		}
		TCHAR szTemp[_MAX_PATH + 1];
		NSSTRCPY(szTemp, pszInsertedBlockName1);
		NSSTRUPR(szTemp);
		NSSTRING strEntBlkName(szTemp);

		getEffectiveName( blkTblRecord,  pBlockRef, strEntBlkName/*,  pszInsertedBlockName*/);

		//Asset selection issue
		//Asset invalid if _NEW block placed on layer other than new
		if(false == checkStringSuffix(strEntBlkName, _T("_NEW")))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
		}


		//////
		/*CNSAsset *pCNSAsset = new CNSAsset;
		pCNSAsset->setObjectID((long) pEnt->id().asOldId());
		pCNSAsset->setBlockName(pszInsertedBlockName);
		pCNSAsset->setLayerName(NSSTRING(pEnt->layer()));*/

		// Get attributes of the pBlockRef entity
		AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
		AcDbAttribute* pTempAttribute = NULL;

		for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
		{
			AcDbObjectId attObjId = attributeIterator->objectId();
			if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
			{
				continue;
			}			
			TCHAR* pszKey= pTempAttribute->tag();

			NSSTRING AttTag (pszKey);

			if (AttTag.length() >= 9)
			{
				//NSSTRING PreFixAttTag = AttTag.substr(0,9);

				if(AttTag.substr(0,9) == _T("ASSET_NUM"))
				{
					//if(NSSTRCMP(pszKey, _T("ASSET_NUM")) == 0)// for normal blocks
					//{
					CNSAsset *pCNSAsset = new CNSAsset;
					pCNSAsset->setObjectID((long) pEnt->id().asOldId());
					pCNSAsset->setBlockName(strEntBlkName);
					pCNSAsset->setLayerName(NSSTRING(pEnt->layer()));

					//assetVector.push_back(pTempAttribute->textString());
					pCNSAsset->setAssetNumber(pTempAttribute->textString());

					assetVector.push_back(pCNSAsset);

					//}
					//else // for sub-elements of nested blocks
					//{
					//	CNSAsset *pCNSAsset = new CNSAsset;
					//	pCNSAsset->setObjectID((long) pEnt->id().asOldId());
					//	pCNSAsset->setBlockName(pszInsertedBlockName);
					//	pCNSAsset->setLayerName(NSSTRING(pEnt->layer()));

					//	//assetVector.push_back(pTempAttribute->textString());
					//	pCNSAsset->setAssetNumber(pTempAttribute->textString());

					//	assetVector.push_back(pCNSAsset);
					//}

				}
			}

			if(pTempAttribute != NULL)
				pTempAttribute->close();

		}
		//		assetVector.push_back(pCNSAsset);
		delete attributeIterator;
		if(pBlockRef != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();
		blkTblRecord->close();

		// Add the CNSAsset to the map to be returned
	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();

	return NS_SUCCESS;
}

void CNSDWGMgr::setTextHeight(double dTextHeight)
{
	m_dTextHeight = dTextHeight;
}

/*
@brief			Copies and replaces the specified block from other drawing into the current drawing
@Param(in)		Source drawing Object, Block name to copy from source drawing 
@Param(out)		MAP containing the name of attribute and its prompt name.
*/
int CNSDWGMgr::copyBlockFromDrawing(CNSDWGMgr &pStdDatabase,NSSTRING szBlockToCopy)
{
	Acad::ErrorStatus es;
	AcDbDatabase* srcDb = pStdDatabase.m_pCurrentDatabase;

	if(srcDb == NULL)
		return NS_FAIL;

	//Read block table from std database
	AcDbBlockTable*pBlockTable;
	AcDbObjectId blockId = AcDbObjectId::kNull;
	es = srcDb->getSymbolTable(pBlockTable, AcDb::kForRead);
	if(Acad::eOk != es) //get block table
		return NS_FAIL;

	AcDbBlockTableRecord* pBlkTableRecord = NULL;
	if(Acad::eOk != pBlockTable->getAt((TCHAR*)(szBlockToCopy.c_str()), pBlkTableRecord, AcDb::kForRead)) // get block table record of the given blockname
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	if(pBlkTableRecord == NULL)
		return NS_FAIL;

	blockId = pBlkTableRecord->objectId();    // get object Id of the retrieved block table record.

	pBlkTableRecord->close();
	pBlockTable->close();

	if(blockId != NULL)
	{
		AcDbIdMapping idMap;
		AcDbObjectIdArray inputObjIdArr;
		inputObjIdArr.append(blockId);
		AcDbObjectId currDbModelSpcId = m_pCurrentDatabase->currentSpaceId();
		es = srcDb->wblockCloneObjects(inputObjIdArr,currDbModelSpcId,idMap,AcDb::kDrcReplace);
		if(es == Acad::eOk)
			return NS_SUCCESS;
	}
	return NS_FAIL;
}

/*
@Brief			Returns vector of invalid blocks
@Param(out)		vector<CNSFeatureContainer*>&
@param(in)		TCHAR*	Layer name 
@param(in)		TCHAR*	Block name
@Return			int		NS_SUCCESS/NS_FAIL
*/
int CNSDWGMgr::selectInvalidBlocks(std::vector<CNSValidationDataCollector *> &vectBlocks)
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	// Iterate the model space pBlockRef table record
	// Copy the contents(Block name, Insertion point, Rotation, Attributes and XDATA) of model space pBlockRef entity into a CNSBlock
	//
	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();
		pBlockTable->close();
		return NS_FAIL;
	}

	AcDbEntity *pEnt = NULL;

	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// If the entity is a pBlockRef get its details otherwise get next enitity
		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		NSSTRING pszLayerName = NSSTRING(pEnt->layer());

		NSSTRING strLayerName = NSSTRING(pszLayerName);
		if(NS_SUCCESS == isTypeStd(strLayerName))
		{
			pEnt->close();
			continue;
		}


		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		// we can get the name from the BlockTableRecord type
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		// Set pBlockRef name
		TCHAR* pszBlockName = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszBlockName))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			pRecord->close();
			pBlockTable->close();
			return NS_FAIL;
		}

		NSSTRING strBlockName = NSSTRING(pszBlockName);
		if(NS_SUCCESS != isTypeStd(strBlockName))
		{
			pEnt->close();
			continue;
		}

		CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
		pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
		pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
		pCNSValidationDataCollector->setBlockName(strBlockName);
		pCNSValidationDataCollector->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);	


		if(pBlockRef != NULL)
			pBlockRef->close();
		if(pEnt != NULL)
			pEnt->close();


		vectBlocks.push_back(pCNSValidationDataCollector);

	} //end model space iteration

	delete pBlkTblRcdItr;
	if(pRecord != NULL)
		pRecord->close();
	if(pBlockTable != NULL)
		pBlockTable->close();

	return NS_SUCCESS;
}


int CNSDWGMgr::isPointInsidePolyLine(int nvert, double *vertx, double *verty, double testx, double testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
			c = !c;
	}
	return c;
}


void CNSDWGMgr::getEffectiveName( AcDbBlockTableRecord *blkTblRecord, AcDbBlockReference *pBlockRef,   NSSTRING &strTempBlkName/*, TCHAR* pszInsertedBlockName*/)
{

	TCHAR *pName = NULL;
	if(blkTblRecord->isAnonymous())
	{
		AcDbDynBlockReference* pDynObj = new AcDbDynBlockReference(pBlockRef);
		if(pDynObj)
		{
			AcDbObjectId objDynBlkId = pDynObj->dynamicBlockTableRecord();
			if(!objDynBlkId.isNull())
			{
				AcDbObject* pObj;
				Acad::ErrorStatus er = acdbOpenAcDbObject(pObj,objDynBlkId,AcDb::kForRead);
				if(pObj)
				{
					AcDbBlockTableRecord* pBTRecord = (AcDbBlockTableRecord*)pObj;
					if(pBTRecord)
					{							
						pBTRecord->getName(pName);
						strTempBlkName = pName;

						acdbFree(pName);
					}
					pBTRecord->close();
				}
				pObj->close();
			}
		}			
		delete pDynObj;
	}

}




int CNSDWGMgr::createSubFeatures(AcDbBlockReference *pNestedBlockRef, CNSFeatureContainer *pNestedFeatContainer)
{
	int cnt = 0; 


	AcDbObjectId blkTblID = pNestedBlockRef->blockTableRecord();
	AcDbBlockTableRecord* blkTblRecord1 = NULL;
	if(Acad::eOk != acdbOpenObject(blkTblRecord1, blkTblID, AcDb::kForRead))
	{
		pNestedBlockRef->close();
		//blkTblRecord->close();
		return cnt;
	}
	else
	{
		AcDbBlockTableRecordIterator *pBlockIterator;
		blkTblRecord1->newIterator(pBlockIterator);


		for (; !pBlockIterator->done(); pBlockIterator->step())
		{

			AcDbEntity *pEntity;
			pBlockIterator->getEntity(pEntity, AcDb::kForRead);

			AcDbHandle objHandle;
			pEntity->getAcDbHandle(objHandle);


			///// BLOCK starts
			if(pEntity->isKindOf(AcDbBlockReference::desc()))
			{	

				AcDbBlockReference* pSubBlockRef = (AcDbBlockReference*)pEntity;						
				AcDbObjectId blkTblID = pSubBlockRef->blockTableRecord();
				AcDbBlockTableRecord* blkTblRecord2 = NULL;
				if(Acad::eOk != acdbOpenObject(blkTblRecord2, blkTblID, AcDb::kForRead))
				{
					pSubBlockRef->close();
					pEntity->close();
					//pRecord->close();
					//pBlockTable->close();
					return NS_FAIL;
				}

				// Set pBlockRef name
				TCHAR* pszInsertedBlockName = NULL;
				if(Acad::eOk != blkTblRecord2->getName(pszInsertedBlockName))
				{
					blkTblRecord2->close();
					pSubBlockRef->close();
					pEntity->close();
					//pRecord->close();
					//pBlockTable->close();
					return NS_FAIL;
				}

				NSSTRING strInsertedBlockName = NSSTRING(pszInsertedBlockName);

				getEffectiveName( blkTblRecord2,  pSubBlockRef, strInsertedBlockName/*,  pszInsertedBlockName*/);


				//////////////// Normalizing Position with reference to parent Block	

				AcGeScale3d ParScl3D = pNestedBlockRef->scaleFactors();						
				double ParentRotRad = pNestedBlockRef->rotation();
				double ParPosX = pNestedBlockRef->position().x;
				double ParPosY = pNestedBlockRef->position().y;

				double x = pSubBlockRef->position().x;
				double y = pSubBlockRef->position().y;

				normalizePoint(x,y,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY);



				ads_point SamplePoint;						 
				SamplePoint[0] = cos(pSubBlockRef->rotation())* ParScl3D.sx;
				SamplePoint[1] = sin(pSubBlockRef->rotation())* ParScl3D.sy;

				ads_point pt1;pt1[0]=0;	pt1[1]=0;
				double ActualChildRotRad = ParentRotRad + acutAngle(pt1, SamplePoint);

				double PI = 4 * atan(1.0); double ActualChildRotDeg = ActualChildRotRad*(180.0/PI);

				//////////////// Normalizing Position with reference to parent Block


				CNSFeatureContainer *pSubFeatContainer = new CNSFeatureContainer;
				CNSBlock* pTempNSBlock = new CNSBlock();
				pTempNSBlock->setBlockName(strInsertedBlockName);
				pTempNSBlock->setRotation(ActualChildRotRad);
				pTempNSBlock->setAlignment(0); //For block the PointInAlignment value is 0.
				pTempNSBlock->setInsertionPoint(x, y);
				pTempNSBlock->setType(ELE_BLK_POINT);

				////////////////////////////////////////////////////

				ATT_MAP* pParentAttributes = pNestedFeatContainer->getAttributeMap();		

				pSubFeatContainer->m_mapOfBlkAtt = new ATT_MAP;

				ATT_MAP::iterator it;
				ATTREF_MAP::iterator refItr;
				CNSAttributeRefMap *pRefMap = NULL;
				CNSAttributeRef *pAttRef = NULL;
				for(it = pParentAttributes->begin(); it != pParentAttributes->end(); it++)
				{
					/*		NSSTRING strAttTag =  (*it).first;
					size_t value = findString(strAttTag, _T("ASSET_NUM"));
					if (value != NSSTRING::npos)
					{
					continue;
					}
					*/
					pRefMap = (*it).second;
					if(pRefMap)
					{
						ATTREF_MAP mapOfAttrRef;
						pRefMap->getAttrReferences(mapOfAttrRef);
						for(refItr = mapOfAttrRef.begin(); refItr != mapOfAttrRef.end(); refItr++)
						{
							pAttRef = (*refItr).second;
							if(pAttRef)
							{
								CNSAttributeRef *pRef = new CNSAttributeRef(*pAttRef);
								pSubFeatContainer->addAttributeRef((*it).first, 1, pRef);
							}
						}

					}
					pSubFeatContainer->m_mapOfParentAttributes.insert(STRING_MAP::value_type(((*it).first).c_str(),((*it).second)->getAttrRef(1)->getAttributeValue()));


				}

				//pSubFeatContainer->setAttributeMap(pNestedFeatContainer->getAttributeMap());

				// Get attributes of the pBlockRef entity
				AcDbObjectIterator *attributeIterator = pSubBlockRef->attributeIterator();
				AcDbAttribute* pTempAttribute = NULL;
				CNSAttributeRef *pRef = NULL;
				//pSubFeatContainer->m_mapOfBlkAtt = new ATT_MAP;
				for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
				{
					AcDbObjectId attObjId = attributeIterator->objectId();
					if(Acad::eOk != pSubBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
					{
						delete pTempNSBlock;
						pTempAttribute->close();
						delete attributeIterator;
						blkTblRecord2->close();
						pSubBlockRef->close();
						pEntity->close();
						//pRecord->close();
						//pBlockTable->close();				
						return NS_FAIL;
					}

					TCHAR* pszKey= pTempAttribute->tag();					
					TCHAR* pszValue = pTempAttribute->textString();
					pRef = new CNSAttributeRef;
					pRef->setAttributeValue(pszValue);
					CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;
					CNSBlock *pBlock = new CNSBlock;
					pGeometry->add(pBlock);
					AcGePoint3d InsertionPoint = pTempAttribute->position();

					// Date 2010_06_10
					AcGeScale3d ParScl3D = pNestedBlockRef->scaleFactors();						
					double ParentRotRad = pNestedBlockRef->rotation();
					double ParPosX = pNestedBlockRef->position().x;
					double ParPosY = pNestedBlockRef->position().y;

					double x = InsertionPoint.x;
					double y = InsertionPoint.y;

					normalizePoint(x,y,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY);

					pBlock->setInsertionPoint(x, y, 0.0);
					// Date 2010_06_10

					pBlock->setRotation(pTempAttribute->rotation());
					pBlock->setAlignment(getAlignment(pTempAttribute));
					pGeometry->setCompType(COMP_TEXTPOINT);
					pRef->setCompGeometry(pGeometry);
					//pSubFeatContainer->addAttributeRef(pszKey, 1, pRef);		
					pSubFeatContainer->updateAttributeRef(pszKey, 1, pRef);		


					// Adds attribute to the Block to be returned
					//pFeatContainer->m_mapOfBlkAtt->insert(ATT_MAP::value_type(pszKey, pRef));	
					if(pTempAttribute != NULL)
						pTempAttribute->close();
				}


				delete attributeIterator;			
				////////////////////////////////////////////////////						


				pSubFeatContainer->SetAssetNumberFromParent();


				pSubFeatContainer->m_mapOfXData = new STRING_MAP;
				readXDATAEx(pEntity->id(), *pSubFeatContainer->m_mapOfXData);

				setRelationInfo(pEntity->id(),pSubFeatContainer, pNestedFeatContainer  );					


				CNSCompositeGeometry *pGeometry = new CNSCompositeGeometry;
				pGeometry->setName(strInsertedBlockName);
				pGeometry->setLayername(pSubBlockRef->layer());
				pGeometry->setCompType(COMP_POINT);
				//pTempNSBlock->getData(strList, enType);
				//pGeometry->addPoslist(enType, strList);
				pGeometry->add(pTempNSBlock);
				CNSCompositeContainer *pContainer = new CNSCompositeContainer;
				pContainer->add(pGeometry);
				pSubFeatContainer->m_pContainer = pContainer;


				++cnt ;

				pNestedFeatContainer->createAnewSubFeature(pSubFeatContainer);
				//pFeatContainer->
				blkTblRecord2->close();				
			}
			///// BLOCK ends


			///// LINEAR FEATURES STARTS

			if(
				pEntity->isKindOf(AcDbPolyline::desc())||
				pEntity->isKindOf(AcDb2dPolyline::desc())||
				pEntity->isKindOf(AcDbLine::desc())||
				pEntity->isKindOf(AcDbArc::desc())||
				pEntity->isKindOf(AcDbMline::desc()) 
				)
			{
				AcGeScale3d ParScl3D = pNestedBlockRef->scaleFactors();						
				double ParentRotRad = pNestedBlockRef->rotation();
				double ParPosX = pNestedBlockRef->position().x;
				double ParPosY = pNestedBlockRef->position().y;

				CNSFeatureContainer *pSubFeatContainer = new CNSFeatureContainer;
				CNSCompositeContainer *pContainer = new CNSCompositeContainer;
				CNSCompositeGeometry *pTempPolyLine = new CNSCompositeGeometry;  // stores vector of CNSCompositeData
				pContainer->add(pTempPolyLine);
				pSubFeatContainer->m_pContainer = pContainer;

				std::map<NSSTRING, NSSTRING>::iterator XdataIter;
				NSSTRING strValue;
				pSubFeatContainer->m_mapOfXData = new STRING_MAP;
				readXDATA(pEntity->id(), *pSubFeatContainer->m_mapOfXData);

				int nRes = NS_FAIL;
				if(pEntity->isKindOf(AcDbPolyline::desc()))
					//if(!bWithCableCode && (pEnt->isKindOf(AcDb2dPolyline::desc())))
				{	
					nRes = getPolyLineSegmentsEx (pEntity, pTempPolyLine,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY, pSubFeatContainer);
				}
				if( (pEntity->isKindOf(AcDbLine::desc())))
				{
					nRes = getLineSegmentsEx(pEntity, pTempPolyLine,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY, pSubFeatContainer);
				}
				if( (pEntity->isKindOf(AcDbArc::desc())))
				{
					nRes = getArcSegmentsEx(pEntity, pTempPolyLine,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY, pSubFeatContainer);
				}
				// changes for busbar suport-80 hrs change request start
				if( (pEntity->isKindOf(AcDbMline::desc())))
				{
					nRes = getMultiLineSegmentsEx(pEntity, pTempPolyLine,ParScl3D.sx,ParScl3D.sy,ParScl3D.sz,ParentRotRad, ParPosX, ParPosY, pSubFeatContainer);
				}
				// changes for busbar suport-80 hrs change request end
				if(nRes != NS_SUCCESS)
				{
					//	delete pTempPolyLine;
					delete pContainer;
					delete pSubFeatContainer;
					pEntity->close();
					//pRecord->close();
					//pBlockTable->close();
					//clearTextLabels(mapTextLabels);
					return NS_FAIL;
				}

				// Set line type and layer name
				pTempPolyLine->setLayername((TCHAR*)pEntity->layer());
				pTempPolyLine->setName((TCHAR*)pEntity->layer()); 
				pSubFeatContainer->setLayerName((TCHAR*)pEntity->layer());
				//if(!(pEnt->isKindOf(AcDbMline::desc()))
				pSubFeatContainer->setTypeName((TCHAR*)pEntity->linetype());
				//else
				//pFeatContainer->setTypeName((TCHAR*)pEnt->mlst);

				//read the XDAata
				//	std::map<NSSTRING, NSSTRING> mapXDataRead;

				XdataIter = pSubFeatContainer->m_mapOfXData->find(_T("CNO"));
				if(XdataIter != pSubFeatContainer->m_mapOfXData->end())
					pTempPolyLine->setCNO(NSATOI(((*XdataIter).second).c_str()));
				XdataIter = pSubFeatContainer->m_mapOfXData->find(_T("CID"));
				if(XdataIter != pSubFeatContainer->m_mapOfXData->end())
					pTempPolyLine->setCID(NSATOI(((*XdataIter).second).c_str()));
				else
					pTempPolyLine->setCID(1);

				pSubFeatContainer->m_mapOfBlkAtt = new ATT_MAP;


				pSubFeatContainer->m_mapOfXData = new STRING_MAP;
				readXDATAEx(pEntity->id(), *pSubFeatContainer->m_mapOfXData);


				if((*pSubFeatContainer->m_mapOfXData).size()>0)
				{
					setRelationInfo(pEntity->id(),pSubFeatContainer,pNestedFeatContainer );

					pNestedFeatContainer->createAnewSubFeature(pSubFeatContainer);

					++cnt ;
				}
			}
			///// LINEAR FEATURES ends


			pEntity->close();
		}
		delete pBlockIterator;

	}

	blkTblRecord1->close();


	return cnt;
}




int CNSDWGMgr::setRelationInfo(AcDbObjectId ObjectId, CNSFeatureContainer *pSubFeatContainer, CNSFeatureContainer *pNestedFeatContainer)
{
	//Open object for write
	AcDbObject *pObj;
	if(Acad::eOk != acdbOpenAcDbObject(pObj, ObjectId, AcDb::kForRead))	
		return NS_FAIL;

	struct  resbuf  *pRb, *pTemp; 
	pRb = pObj->xData(NULL);


	// If xdata is present, then retrieve all key-value pairs
	for (pTemp = pRb; pTemp != NULL;)
	{
		if(pTemp->restype == AcDb::kDxfRegAppName)
		{
			NSSTRING strAppName = pTemp->resval.rstring;
			pTemp = pTemp->rbnext;

			//////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY
			if(pTemp->restype == AcDb::kDxfXdControlString)
			{
				pTemp = pTemp->rbnext;
			}
			//////////////// NEW CODE TO INCORPORATE MANUAL XDATA ENTRY

			if(pTemp!= NULL && pTemp->restype == AcDb::kDxfXdAsciiString)
			{
				NSSTRING strAppValue = pTemp->resval.rstring;
				//mapXData.insert(std::map<NSSTRING, NSSTRING>::value_type(strAppName, strAppValue));

				pSubFeatContainer->setRelation(strAppName,strAppValue,pNestedFeatContainer);

				pTemp = pTemp->rbnext;
			}
			//else
			//{
			//	pTemp = pTemp->rbnext;
			//}
		}
		else
		{
			pTemp = pTemp->rbnext;
		}
	}
	pObj->close();


	//int iTemp = pSubFeatContainer->isParentReferenced();

	//if(iTemp)
	//		pSubFeatContainer->replaceParentReferenc(REL_NODE1, _T("testing..."));

	return NS_SUCCESS;
}


void CNSDWGMgr::normalizePoint(double &ptX, double &ptY,  double sx, double sy, double sz,double ParentRotRad, double ParPosX, double ParPosY)
{
	double PI = 4 * atan(1.0);

	AcGeMatrix3d Mat3D ;
	Mat3D.entry[0][0]=sx;
	Mat3D.entry[1][1]=sy; 
	Mat3D.entry[2][2]=sz;


	//double ParentRotDeg = ParentRotRad*(180.0/PI);

	ads_point pt1;
	ads_point pt2;

	pt1[0]=0;	pt1[1]=0;						

	pt2[0]= ptX * sx;		
	pt2[1]= ptY * sy;		

	double ChildRotRad = acutAngle(pt1, pt2); 
	//double ChildRotDeg = ChildRotRad*(180.0/PI);

	double EffChildRotRad = ChildRotRad + ParentRotRad;
	//double EffChildRotDeg = ParentRotDeg + ChildRotDeg ;					


	AcGePoint3d ChildPos ; 
	ChildPos.x = ptX;
	ChildPos.y = ptY;


	AcGePoint3d NewPos = ChildPos.transformBy(Mat3D);						

	double L = pow((NewPos.x * NewPos.x + NewPos.y * NewPos.y),0.5) ;
	double Lx = L * cos(EffChildRotRad);
	double Ly = L * sin(EffChildRotRad);


	ptX = Lx  + ParPosX;
	ptY = Ly +  ParPosY;

	return ;
}


// This funtion fill the input map with all those blocks whose 
//layer name ends with given pszNameSuff but their name does not ends with pszNameSuff
int CNSDWGMgr::getBlocksOnNonMatchingLayer(std::map<long, CNSValidationDataCollector*> &mapBlocksOnWrongLayer,   TCHAR* pszNameSuff)
{

	AcDbBlockTable* pBlockTable = NULL;
	if (Acad::eOk != m_pCurrentDatabase->getBlockTable(pBlockTable, AcDb::kForRead))
		return NS_FAIL;

	AcDbBlockTableRecord *pRecord;
	if (Acad::eOk != pBlockTable->getAt(ACDB_MODEL_SPACE, pRecord, AcDb::kForRead) )
	{
		pBlockTable->close();
		return NS_FAIL;
	}

	if(pBlockTable != NULL)
		pBlockTable->close();

	AcDbBlockTableRecordIterator *pBlkTblRcdItr;
	if(Acad::eOk != pRecord->newIterator(pBlkTblRcdItr))
	{
		pRecord->close();		
		return NS_FAIL;
	}

	if(pRecord != NULL)
		pRecord->close();

	AcDbEntity *pEnt = NULL;
	for (pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step())
	{
		if(Acad::eOk != pBlkTblRcdItr->getEntity(pEnt,AcDb::kForRead))
		{
			continue;
		}

		if(!pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			pEnt->close();
			continue;
		}

		// Select blocks on layers representing New features
		NSSTRING strEntityLayer = NSSTRING(pEnt->layer());		

		AcDbBlockReference* pBlockRef = (AcDbBlockReference*)pEnt;
		AcDbObjectId blkTblID = pBlockRef->blockTableRecord();
		AcDbBlockTableRecord* blkTblRecord = NULL;
		if(Acad::eOk != acdbOpenObject(blkTblRecord, blkTblID, AcDb::kForRead))
		{
			pBlockRef->close();
			pEnt->close();
			continue;
		}



		TCHAR* pszInsertedBlockName1 = NULL;
		if(Acad::eOk != blkTblRecord->getName(pszInsertedBlockName1))
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
		}

		NSSTRING strTempBlkName(pszInsertedBlockName1);

		getEffectiveName( blkTblRecord,  pBlockRef, strTempBlkName);

		NSSTRING strBlockNameSuffix = _T("");
		if(strTempBlkName.size() < 4)
		{
			blkTblRecord->close();
			pBlockRef->close();
			pEnt->close();
			continue;
		}
		strBlockNameSuffix = strTempBlkName.substr((strTempBlkName.size()-4), 4);


		if (NSSTRCMP(strBlockNameSuffix.c_str(),pszNameSuff) == 0) // dont search for other layer
		{
			NSSTRING strLayerNameSuffix = _T("");
			if(strEntityLayer.size() < 4)
			{
				blkTblRecord->close();
				pBlockRef->close();
				pEnt->close();
				continue;
			}
			strLayerNameSuffix = strEntityLayer.substr((strEntityLayer.size()-4), 4);

			if (NSSTRCMP(strLayerNameSuffix.c_str(),pszNameSuff) != 0) 
			{
				CNSValidationDataCollector *pCNSValidationDataCollector = new CNSValidationDataCollector;
				pCNSValidationDataCollector->setObjectId((long) pEnt->id().asOldId());
				pCNSValidationDataCollector->setLayerName(NSSTRING(pEnt->layer()));
				pCNSValidationDataCollector->setBlockName(strTempBlkName);//setBlockName(pszInsertedBlockName);
				pCNSValidationDataCollector->setInsertionPoint(pBlockRef->position().x, pBlockRef->position().y);	

				AcDbObjectIterator *attributeIterator = pBlockRef->attributeIterator();
				AcDbAttribute* pTempAttribute = NULL;
				for(attributeIterator->start(); !attributeIterator->done(); attributeIterator->step())
				{
					AcDbObjectId attObjId = attributeIterator->objectId();
					if(Acad::eOk != pBlockRef->openAttribute(pTempAttribute, attObjId, AcDb::kForRead))
					{
						pTempAttribute->close();
						delete attributeIterator;
						blkTblRecord->close();
						pBlockRef->close();
						pEnt->close();
						continue;
					}			
					TCHAR* pszKey= pTempAttribute->tag();
					TCHAR* pszValue = pTempAttribute->textString();

					if(pTempAttribute != NULL)
						pTempAttribute->close();

					getupperString(pszKey);

					pCNSValidationDataCollector->m_mapAttributes.insert(std::map<NSSTRING, NSSTRING>::value_type(NSSTRING(pszKey), NSSTRING(pszValue)));

				}

				readXDATA(pEnt->id(), pCNSValidationDataCollector->m_mapOfRelationsAndConnectivity);						

				delete attributeIterator;

				mapBlocksOnWrongLayer.insert(std::map<long, CNSValidationDataCollector*>::value_type(long(pEnt->id().asOldId()), pCNSValidationDataCollector));

			}

		}

		if(pBlockRef != NULL)
			pBlockRef->close();
		if(blkTblRecord != NULL)
			blkTblRecord->close();
	} 
	delete pBlkTblRcdItr;
	if(pEnt != NULL)
		pEnt->close();




	return NS_SUCCESS;
}
/*
@brief			Get Feature container and XData key to serch value in map of xdata 
and return Z  value specified to key
@Param(in)		strXdatakey:- key field in Xdata.
@Param(out)		
*/
double CNSDWGMgr::getZvalue(CNSFeatureContainer *pFeatContainer, NSSTRING strXDataKey)
{
	STRING_MAP::iterator TempItr;
	TempItr = pFeatContainer->m_mapOfXData->find(strXDataKey);
	TCHAR* szNextToken;
	double dZ = 0.0;
	if(TempItr != pFeatContainer->m_mapOfXData->end())
	{
		NSSTRING tempStrValue = (*TempItr).second;
		double dX = NSATOF(NSSTRTOK((TCHAR*)tempStrValue.c_str(), _T(" "), &szNextToken));
		double dY = NSATOF(NSSTRTOK(szNextToken, _T(" "), &szNextToken));
		if(NSSTRCMP(szNextToken, _T("")) == 0)
			dZ = 0.0;
		else
			dZ = NSATOF(NSSTRTOK(NULL, _T("\0"), &szNextToken));
	}
	return dZ;
}

/*
@brief			Set Drawing number.
@Param(in)		strDrawingNum:- Current drawing number.
@Param(out)		
*/

void CNSDWGMgr::setDrawingNumber(NSSTRING strDrawingNum)
{
	m_strDrawingNum = strDrawingNum;
}

/*
@brief			Displays AutoCAD File Format Information
@Param(in)		m_strDWGFilename :- Current drawing.
@Param(out)		File Format
*/
NSSTRING CNSDWGMgr::getDWGVersionInfo(const wchar_t *m_strDWGFilename)
{	
	NSSTRING dwgverstr=L"";
	int size=6;
	wchar_t *buffer=new wchar_t[size];

	NSIFSTREAM inputFile(m_strDWGFilename,std::ios::in | std::ios::binary);
	inputFile.seekg(0,std::ios::beg);
	inputFile.read(buffer,6);
	inputFile.close();
	NSSTRING txtStr(buffer,6);
	delete[] buffer;

	if(txtStr==L"AC1024")
		dwgverstr=L"AutoCAD 2010";
	else if(txtStr==L"AC1021")
		dwgverstr=L"AutoCAD 2008";
	else if(txtStr==L"AC1018")
		dwgverstr=L"AutoCAD 2006";
	else if(txtStr==L"AC1015")
		dwgverstr=L"AutoCAD 2002";
	else if(txtStr==L"AC1014")
		dwgverstr=L"AutoCAD Release 14";
	else if(txtStr==L"AC1012")
		dwgverstr=L"AutoCAD Release 13";
	return dwgverstr;
}