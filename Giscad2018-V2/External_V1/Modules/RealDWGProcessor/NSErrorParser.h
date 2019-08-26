#pragma once
#include <map>
#include "..\Include\NSValidationDataCollector.h"
#include "..\Include\NSRelationLinker.h"

extern NSSTRING strDWG$;
extern NSSTRING strdwg$;

class CNSErrorParser
{
public:
	
	static CNSErrorParser* getInstance();
	
	ErrRecord ParseDesc(ErrRecord er); 
	//ErrRecord ParseDesc(ErrRecord er,CNSValidationDataCollector *pTempFeature, NSSTRING NoteData);
	ErrRecord ParseDesc(ErrRecord er,CNSValidationDataCollector *pTempFeature, CNSProposedRelations *pTempPropRel);
	ErrRecord ParseDesc(ErrRecord er,CNSAsset *pAsset, NSSTRING strMsg = _T("") );
	ErrRecord ParseDesc(ErrRecord er,NSSTRING pszTag, NSSTRING strData );
	ErrRecord ParseDesc(ErrRecord er,NSSTRING strTag,NSSTRING locLay, NSSTRING stdLay );
	ErrRecord ParseDesc(ErrRecord er,NSSTRING PropName, NSSTRING Stylename, NSSTRING CurrVal, NSSTRING StdVal );

	ErrRecord ParseDescEx(ErrRecord er,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2, NSSTRING Data2 );
	ErrRecord ParseDescEx(ErrRecord er,CNSValidationDataCollector *pTempFeature,NSSTRING Tag1, NSSTRING Data1, NSSTRING Tag2= _T("") , NSSTRING Data2 = _T("") , NSSTRING Tag3= _T("") , NSSTRING Data3= _T("") );
    ErrRecord ParseDescEx(ErrRecord er,CNSValidationDataCollector *pTempFeature,CNSProposedRelations *pTempPropRel , NSSTRING Tag1, NSSTRING Data1);


    NSSTRING ReplaceTag(NSSTRING strDescFromDB,NSSTRING strSmartTag,NSSTRING strTagReplacementData);
	//void SetTag(NSSTRING strSmartTag,NSSTRING strTagReplacementData);

	virtual ~CNSErrorParser(void);
private:
	CNSErrorParser(void);
	static CNSErrorParser* pErrorParser;
	std::map<NSSTRING,NSSTRING> TagMap;
	std::map<NSSTRING,NSSTRING>::iterator theMapItr;
};
