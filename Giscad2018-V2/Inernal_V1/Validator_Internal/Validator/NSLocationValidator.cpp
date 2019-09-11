#include "StdAfx.h"
#include <math.h>
#include "NSLocationValidator.h"
extern double gTolerance;
#include "NSErrorCircle.h"

extern CNSErrorCircle gErrorCircle;
CNSLocationValidator::CNSLocationValidator(void)
{
	m_pCollector = NULL;
	m_pLocalDWGMgr = NULL;
}

CNSLocationValidator::~CNSLocationValidator(void)
{
	m_mapOfCommonErrors[0].clear();
	m_mapOfCommonErrors[1].clear();
}

//void CNSLocationValidator::setObject(CNSFeatureContainer* pFeatureContainer)
//{
//	m_pFeatureContainer = pFeatureContainer;
//}
void CNSLocationValidator::setObject(CNSValidationDataCollector* pCollector)
{
	m_pCollector = pCollector;
}

void CNSLocationValidator::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
}

void CNSLocationValidator::setType(LocationType enLocType)
{
	m_enLocType = enLocType;
}

int CNSLocationValidator::getValue(NSSTRING strType, NSSTRING &strValue)
{
	STRING_MAP::iterator TempItr;
	TempItr = m_pCollector->m_mapOfRelationsAndConnectivity.find(strType);
	if(TempItr == m_pCollector->m_mapOfRelationsAndConnectivity.end())
	{
		return NS_FAIL;
	}
	strValue = (*TempItr).second;
	return NS_SUCCESS;
}

CNSPoint3D CNSLocationValidator::getPoint(NSSTRING strPt)
{
	TCHAR* szNextToken;   
	CNSPoint3D pt;
	#ifdef _INTERNAL_
		/*pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strPt.c_str(), _T(" ")));
		pt.m_dYY = NSATOF(NSSTRTOK (NULL, _T("\0")));*/
		pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strPt.c_str(), _T(" "),&szNextToken));
		pt.m_dYY = NSATOF(NSSTRTOK (NULL, _T("\0"), &szNextToken));
	#else
		pt.m_dXX = NSATOF(NSSTRTOK ((TCHAR*)strPt.c_str(), _T(" "),&szNextToken));
		pt.m_dYY = NSATOF(NSSTRTOK (NULL, _T("\0"), &szNextToken));
	#endif
	return pt;
}

NSSTRING CNSLocationValidator::getAttributeValue(STRING_MAP &mapOfAttr, NSSTRING strAttr,  bool &bAssetNumPresent)
{
    STRING_MAP::iterator TempItr;
    for(TempItr = mapOfAttr.begin(); TempItr != mapOfAttr.end(); TempItr++)
    {
        if(NSSTRCMP(strAttr.c_str(), (*TempItr).first.c_str()) == 0)
        {
            bAssetNumPresent = true;
            return (*TempItr).second.c_str();
        }
    }
    bAssetNumPresent = false;
    return _T("");
}

int CNSLocationValidator::validate()
{
	//MessageBox(NULL,_T("0"), _T("Design Validation"),MB_ICONWARNING);
	if(m_enLocType == LOC_OTHER)
		return validatePolylines();
	else
	{
		STRING_MAP::iterator TempItr;
		CNSPoint3D pt = m_pCollector->getInsertionPoint();
		TempItr = m_pCollector->m_mapOfRelationsAndConnectivity.find(_T("InsertionPoint"));
		if(TempItr == m_pCollector->m_mapOfRelationsAndConnectivity.end())
		{
			TCHAR szFormat[_MAX_PATH + 1];
			//NSSPRINTF(szFormat,_T("Missing XData for block \'%s\' found on existing layer \'%s\'"), m_pCollector->getBlockName().c_str(), m_pCollector->getLayerName().c_str());
			int nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_35,m_pCollector); //WriteToLog(LOG_GIS, szFormat);	
			gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_35, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
			//gErrorCircle.logError(Err_Invalid_Geometry, _T("Invalid geometry found"),LOG_GIS, pt.m_dXX, pt.m_dYY);
			return NS_FAIL;
		}
		NSSTRING strValue = (*TempItr).second;
		TCHAR* szNextToken;
		#ifdef _INTERNAL_
			/*double dX = NSATOF(NSSTRTOK((TCHAR*)strValue.c_str(), _T(" ")));
			double dY = NSATOF(NSSTRTOK(NULL, _T("\0")));*/
			double dX = NSATOF(NSSTRTOK((TCHAR*)strValue.c_str(), _T(" "), &szNextToken));
			double dY = NSATOF(NSSTRTOK(NULL, _T("\0"), &szNextToken));
		#else
			double dX = NSATOF(NSSTRTOK((TCHAR*)strValue.c_str(), _T(" "), &szNextToken));
			double dY = NSATOF(NSSTRTOK(NULL, _T("\0"), &szNextToken));
		#endif
		if(fabs(dX - pt.m_dXX) > gTolerance || fabs(dY - pt.m_dYY) > gTolerance)
		{	
			int nErrNumber;
			TCHAR szFormat[_MAX_PATH + 1];
			TCHAR strOriPos[_MAX_PATH + 1];
            bool bAssetNumPresent = false;
            NSSTRING strAssetNum = getAttributeValue(m_pCollector->m_mapAttributes, _T("ASSET_NUM"), bAssetNumPresent);
            if(bAssetNumPresent)
            {
                //NSSPRINTF(szFormat,_T("Asset \'%s\' having \'ASSET_NUM\' = \'%s\' on layer \'%s\' moved. Original Value = (X:%lf, Y:%lf), Current Value = (X:%lf, Y:%lf)"),  m_pCollector->getBlockName().c_str(),strAssetNum.c_str(), m_pCollector->getLayerName().c_str(), dX, dY, pt.m_dXX, pt.m_dYY);
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_14);

				NSSPRINTF(strOriPos,_T("(X:%lf, Y:%lf)"),  dX, dY);
				nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_14,m_pCollector, _T("AN$"),strAssetNum.c_str(),_T("OP$"),strOriPos);
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_14, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
            }
            else
            {
                //NSSPRINTF(szFormat,_T("Asset \'%s\' on layer \'%s\' moved. Original Value = (X:%lf, Y:%lf), Current Value = (X:%lf, Y:%lf)"), m_pCollector->getBlockName().c_str(), m_pCollector->getLayerName().c_str(), dX, dY, pt.m_dXX, pt.m_dYY);
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_30);

				NSSPRINTF(strOriPos,_T("(X:%lf, Y:%lf)"),  dX, dY);
				nErrNumber = WriteErrorRecEx(LOG_GIS,ERR_GIS_30,m_pCollector,_T("OP$"),strOriPos);
				gErrorCircle.drawCircle(nErrNumber, LOG_GIS, ERR_GIS_30, pt.m_dXX, pt.m_dYY, pt.m_dZZ);
            }

			return NS_FAIL;
		}
	}
	return NS_SUCCESS;
}

int CNSLocationValidator::validatePolylines()
{
		STRING_MAP::iterator TempItr;
		NSSTRING strTemp;
		CNSPoint3D startPt, nextPt;
		double dArea = 0; 
		bool bhasXData = true;
//MessageBox(NULL,_T("0"), _T("Design Validation"),MB_ICONWARNING);

		if(NS_SUCCESS == getValue(_T("START_VERTEX"), strTemp))
		{
			startPt = getPoint(strTemp);				
		}
		else
		{
			bhasXData = false;
			//MessageBox(NULL,_T("1"), _T("Design Validation"),MB_ICONWARNING);
		}
		if(NS_SUCCESS == getValue(_T("NEXT_VERTEX"), strTemp))
		{
			nextPt = getPoint(strTemp);			
		}
		else
		{
			bhasXData = false;
			//MessageBox(NULL,_T("2"), _T("Design Validation"),MB_ICONWARNING);
		}
		if(NS_SUCCESS == getValue(_T("NS_AREA"), strTemp))
		{
			dArea =  NSATOF(strTemp.c_str());
		}
		else
		{
			bhasXData = false;
			//MessageBox(NULL,_T("3"), _T("Design Validation"),MB_ICONWARNING);
		}
		CNSPoint3D startPtAct, nextPtAct;
		double dAreaAct = 0; 		

		m_pLocalDWGMgr->getPlineRecord(m_pCollector->getObjectId(), startPtAct, nextPtAct, dAreaAct);

		if(m_pCollector->getLayerName() == _T("Duct_SP_EXT") || m_pCollector->getLayerName() == _T("Services_SP_EXT_UG"))
		{
				return NS_SUCCESS;
		}

		if(bhasXData == false)
		{

			//MessageBox(NULL,_T("4"), _T("Design Validation"),MB_ICONWARNING);

		//	gErrorCircle.logError(Err_Invalid_Geometry, _T("Invalid geometry found"),LOG_GIS, startPtAct.m_dXX, startPtAct.m_dYY);
			TCHAR szFormat[_MAX_PATH + 1];
			NSSPRINTF(szFormat,_T("Invalid polyline/multiline found on existing layer \'%s\'"), m_pCollector->getLayerName().c_str());
		   	NSSTRING strLayer = m_pCollector->getLayerName();
			std::map<NSSTRING, int>::iterator theIt;
			theIt = m_mapOfCommonErrors[0].find(strLayer);
			int nErrorNumber = 0;
			if(theIt == m_mapOfCommonErrors[0].end())
			{
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_15);

				nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_15,m_pCollector); 				

				m_mapOfCommonErrors[0].insert(std::map<NSSTRING, int>::value_type(strLayer, nErrorNumber));
			}
			else
			{
				nErrorNumber = (*theIt).second;
			}
			
			gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_15, startPtAct.m_dXX, startPtAct.m_dYY, startPtAct.m_dZZ);
			return NS_FAIL;
		}
		if((isPointOverlapped(startPt, startPtAct) == NS_SUCCESS 
			&&isPointOverlapped(nextPt, nextPtAct) == NS_SUCCESS)
			&& fabs(dArea - dAreaAct) > gTolerance)
		{
           	TCHAR szFormat[_MAX_PATH + 1];
			NSSPRINTF(szFormat,_T("Polyline geometry has been altered on layer \'%s\'"), m_pCollector->getLayerName().c_str());
		   	NSSTRING strLayer = m_pCollector->getLayerName();
			std::map<NSSTRING, int>::iterator theIt;
			theIt = m_mapOfCommonErrors[1].find(strLayer);
			int nErrorNumber = 0;
			if(theIt == m_mapOfCommonErrors[1].end())
			{
				//WriteIDToLog(LOG_GIS, szFormat, ERR_GIS_16);

				nErrorNumber = WriteErrorRec(LOG_GIS,ERR_GIS_16,m_pCollector); 				
				
				m_mapOfCommonErrors[1].insert(std::map<NSSTRING, int>::value_type(strLayer, nErrorNumber));
			}
			else
			{
				nErrorNumber = (*theIt).second;
			}
			

			gErrorCircle.drawCircle(nErrorNumber, LOG_GIS, ERR_GIS_16, startPtAct.m_dXX, startPtAct.m_dYY, startPtAct.m_dZZ);
			//gErrorCircle.logError(Err_PLine, _T("Polyline geometry has been altered"),LOG_GIS, startPt.m_dXX, startPt.m_dYY);
	/*		TCHAR szFormat[_MAX_PATH + 1];
			NSSPRINTF(szFormat,_T("Polyline moved."));
			int nErrNumber = WriteToLog(LOG_GIS, szFormat);*/
			return NS_FAIL;
		}

	return NS_SUCCESS;
}

int CNSLocationValidator::isPointOverlapped(CNSPoint3D firstPt, CNSPoint3D secondPt)
{
	if(fabs(firstPt.m_dXX - secondPt.m_dXX) < gTolerance && fabs(firstPt.m_dYY - secondPt.m_dYY) < gTolerance)
	{
		return NS_SUCCESS;
	}
	return NS_FAIL;
}