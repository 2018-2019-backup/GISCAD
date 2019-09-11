#include "StdAfx.h"
#include "NSTextHeightTbl.h"
#include "math.h"
#include "NSErrorCircle.h"
#include "..\..\Include\NSDatabaseMgr.h"
extern CNSErrorCircle gErrorCircle;
extern double gTolerance;
#define PRECISION 0.0001
CNSTextHeightTbl::CNSTextHeightTbl(void)
{
	m_pStdDWGMgr = NULL;
	m_pLocalDWGMgr = NULL;
}

CNSTextHeightTbl::~CNSTextHeightTbl(void)
{
}

void CNSTextHeightTbl::setStdDWGObject(CNSDWGMgr* pStdDWGMgr)
{
	m_pStdDWGMgr = pStdDWGMgr;
}

void CNSTextHeightTbl::setLocalDWGObject(CNSDWGMgr* pLocalDWGMgr)
{
	m_pLocalDWGMgr = pLocalDWGMgr;
	gErrorCircle.setLocalDWGObject(pLocalDWGMgr);
}

int CNSTextHeightTbl::validate()
{
	//getting all the properties of the local dwg file
	std::vector<CNSTextHeight*> vectTxtHt;
	m_pLocalDWGMgr->getTextHeight(vectTxtHt);
	std::vector<CNSTextHeight*>::iterator vectTxtHtItr;
	std::vector<double> vectHt;

	CNSTextHeight* pObj = NULL;			
	for(vectTxtHtItr = vectTxtHt.begin(); vectTxtHtItr != vectTxtHt.end(); vectTxtHtItr++)
	{
		pObj = *vectTxtHtItr;	
		double dTxtHt = pObj->getHeight();		   
		double dXPosition = pObj->getXPosition();  
		double dYPosition = pObj->getYPosition(); 

		TCHAR szQuery[MAX_PATH+1];
		int nDBRet = CNSDatabaseMgr::getInstance()->executeQuery(_T("Select AllowedTextHeights from TextHeights"));
		std::vector<variant_t> vectVarientHeights;
		std::vector<variant_t>::iterator itrTxt;
		if(nDBRet != -1)
		{
			CNSDatabaseMgr::getInstance()->getValue(_T("AllowedTextHeights"), vectVarientHeights);
		}
		bool bInValid = false;
		for(itrTxt = vectVarientHeights.begin(); itrTxt != vectVarientHeights.end(); itrTxt++)
		{
			variant_t varValue = (*itrTxt);
			double value = (double)varValue;
			double dCompareValue = fabs(dTxtHt - value);
			if(dCompareValue > PRECISION)
			{
				bInValid = true;
			}
			else
			{
				bInValid = false;
				break;
			}
		}
		if(bInValid)
        {
			gErrorCircle.logError(Err_TxtHt, _T("Text height is non-standard"), LOG_CAD, dXPosition, dYPosition, ERR_CAD_16);			
        }
		/*double dCompareValue1 = fabs(dTxtHt - 1.8);
		double dCompareValue2 = fabs(dTxtHt - 3.5);
		double dCompareValue3 = fabs(dTxtHt - 5.0);
		double dCompareValue4 = fabs(dTxtHt - 7.0); 

		if(dCompareValue1 > gTolerance && dCompareValue2 > gTolerance && dCompareValue3 > gTolerance && dCompareValue4 > gTolerance)
		{
			gErrorCircle.logError(Err_TxtHt, _T("Text height is non-standard"), LOG_CAD, dXPosition, dYPosition);
		}*/
	}
	//Delete the objects of the class TextHeight from the vector.
	for(vectTxtHtItr = vectTxtHt.begin(); vectTxtHtItr != vectTxtHt.end(); vectTxtHtItr++)
	{
		delete *vectTxtHtItr;
	}
	vectTxtHt.clear();
	return NS_SUCCESS;
}