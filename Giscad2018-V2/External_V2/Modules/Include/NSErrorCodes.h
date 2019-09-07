#pragma once
//#include "stdafx.h" 
#include "..\Include\NSReportWriter.h"
#include "..\Include\NSDirectives.h"

class CNSValidationDataCollector;
class CNSRelationLinker;
class CNSProposedRelations;
class CNSAsset;


/*Issue#187
Each error needs to be given an error number.
All CAD related Errors are to ne numbered between 100 and 199.
GIS errrors related to ASSET numbering are to be numbered from 300-399
GIS errors related to Connectivity are to be numbered 400-420.
GIS errors related to Parent/Child are to be numbered 421-440
GIS errors related to One2Many are to be numbered 441-460
All remaining GIS related errors are to be numbered 200-299.
eg: GIS_ERR_1, (425) No parenet found for block "SUB_NEW" on layer "SUBS_HV_LV_NEW".*/


//Remaning GIS errors from 200 to 299
#define ERR_GIS_2 202
#define ERR_GIS_3 203
#define ERR_GIS_4 204
#define ERR_GIS_5 205
#define ERR_GIS_6 206
#define ERR_GIS_7 207
#define ERR_GIS_8 208
#define ERR_GIS_9 209
#define ERR_GIS_10 210
#define ERR_GIS_11 211
#define ERR_GIS_12 212
#define ERR_GIS_13 213
#define ERR_GIS_14 214
#define ERR_GIS_15 215
#define ERR_GIS_16 216
#define ERR_GIS_37 217
#define ERR_GIS_20 220
#define ERR_GIS_21 221
#define ERR_GIS_22 222
#define ERR_GIS_23 223
#define ERR_GIS_24 224
#define ERR_GIS_25 225
#define ERR_GIS_26 226
#define ERR_GIS_27 227
#define ERR_GIS_28 228
#define ERR_GIS_29 229
#define ERR_GIS_30 230
#define ERR_GIS_31 231

#define ERR_GIS_35 235 // Missing XData
#define ERR_GIS_36 236 // Asset Number AN$ for B$ at P$ already exists in GIS

#define ERR_GIS_38 238 //Template Version number check
#define ERR_GIS_39 239 //Incorrect base drawing used. Extract designated area from GIS


//connectivity and parent child relationship errors
#define ERR_GIS_17 421  //Parent not found -Range FOR BLOCK
#define ERR_GIS_32 422  //Parent not found -Range FOR LINEAR FEATURE
#define ERR_GIS_33 423  //MANYOWNERSHIP Parent not found -Range FOR BLOCK
#define ERR_GIS_34 424  //MANYOWNERSHIP Parent not found -Range FOR LINEAR FEATURE

#define ERR_GIS_18 400  //Inadaquate electrical connectivity - Range
#define ERR_GIS_19 401  //Inadaquate electrical connectivity for block

//Asset validation errors ranging from 300-399
#define ERR_GIS_0 300 //Asset Number value is empty for Block
#define ERR_GIS_1 302 //Asset Number value \'%s\' returned by Ellipse not found in the drawing
#define ERR_GIS_301 301  //Asset Number value of \'%s\' for Block \'%s\' on layer \'%s\' not found in Ellipse
#define ERR_GIS_303 303
#define ERR_GIS_304 304
#define ERR_GIS_305 305
#define ERR_GIS_306 306
#define ERR_GIS_307 307
#define ERR_GIS_308 308
#define ERR_GIS_309 309
#define ERR_GIS_310 310
#define ERR_GIS_311 311
#define ERR_GIS_312 312
#define ERR_GIS_313 313

//All cad error ranging from 100 to 199
#define ERR_CAD_0 100
#define ERR_CAD_1 101
#define ERR_CAD_2 102
#define ERR_CAD_3 103
#define ERR_CAD_4 104
#define ERR_CAD_5 105
#define ERR_CAD_6 106
#define ERR_CAD_7 107
#define ERR_CAD_8 108
#define ERR_CAD_9 109
#define ERR_CAD_10 110
#define ERR_CAD_11 111
#define ERR_CAD_12 112
#define ERR_CAD_13 113
#define ERR_CAD_14 114
#define ERR_CAD_15 115
#define ERR_CAD_16 116
#define ERR_CAD_17 117
#define ERR_CAD_18 118 //There are no new assets to validate
#define ERR_CAD_19 119 //Drawing is missing header information required for GIS Import
#define ERR_CAD_20 120 //Xref is present in this drawing

bool GetErrRecord(int ErrNo, ErrRecord &ErrRec);

int WriteErrorRec(LOGTYPE enLogType,int ErrNo,CNSValidationDataCollector *pTempFeature, CNSProposedRelations *pTempPropRel = NULL);
int WriteErrorRec(LOGTYPE enLogType,int ErrNo,CNSAsset *pAsset,NSSTRING pszMsg = _T("")  );

int WriteErrorRec(LOGTYPE enLogType,int ErrNo,TCHAR *pszMsg = _T(""));
int WriteErrorRec(LOGTYPE enLogType,int ErrNo,NSSTRING strTag,NSSTRING strData );
int WriteErrorRec(LOGTYPE enLogType,int ErrNo,NSSTRING strTag,NSSTRING locLay, NSSTRING stdLay );
int WriteErrorRec(LOGTYPE enLogType,int ErrNo,NSSTRING PropName, NSSTRING Stylename, NSSTRING CurrVal, NSSTRING StdVal );
int WriteErrorRec(LOGTYPE enLogType, NSSTRING strErrorString);

int WriteErrorRecEx(LOGTYPE enLogType,int ErrNo, NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2 );
int WriteErrorRecEx(LOGTYPE enLogType,int ErrNo, CNSValidationDataCollector *pTempFeature,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2= _T("") , NSSTRING Data2 = _T("") , NSSTRING Tag3= _T("") , NSSTRING Data3= _T("") );
int WriteErrorRecEx(LOGTYPE enLogType,int ErrNo, CNSValidationDataCollector *pTempFeature,CNSProposedRelations *pTempPropRel = NULL, NSSTRING Tag1= _T(""), NSSTRING Data1 = _T(""));





int WriteIDToLog(LOGTYPE enLogType, TCHAR* szMessage, int nErrorID = -1);
int WriteIDToLog(ErrRecord er);


//ErrRecord WriteErrorRecEr( LOGTYPE enLogType, NSSTRING strErrorString);