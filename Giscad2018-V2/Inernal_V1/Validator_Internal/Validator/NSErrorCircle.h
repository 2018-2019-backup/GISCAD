/*!
 @Class			CNSErrorCircle
 @Brief			This class draws the error circle on the position where there is an
				error and logs error in the error report
 @Author		Neilsoft Ltd
 @Date			12-09-2007
 @History 		Change history
*/
#pragma once
#include "..\..\Include\NSDWGMgr.h"
enum Err_MSG
{
    Err_PLine = 1,
	Err_Block = 2,
	Err_Asset = 3,
	Err_ExportArea = 4,
	Err_Zero = 5,
	Err_Developer = 6,
	Err_TxtHt = 7,
	ERR_FID = 8,
	Err_Invalid_Geometry = 9,
};

class CNSErrorCircle
{
public:
	CNSErrorCircle(void);
public:
	~CNSErrorCircle(void);
public:
	void logError(enum Err_MSG, TCHAR* szFormat, enum LOGTYPE, double dXX, double m_dYY, int nErrID = -1);
    void logErrorEx(enum Err_MSG, TCHAR* szFormat, enum LOGTYPE, double dXX, double m_dYY,CNSValidationDataCollector *pCollector, int nErrID = -1);
	void drawCircle(int nErrorno, enum LOGTYPE type, int ErrNo, double dX, double dY, double dZ);
	void setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr);
	void clearErrMap();
	void setGraphicIndication(int nGraphicIndication);

private:
	std::map<enum Err_MSG, int> mapofError;
	int m_Errno;
	CNSPoint3D m_Point;
	CNSDWGMgr *m_pLocalDWGMgr;
	int m_nGraphicIndication;

};
