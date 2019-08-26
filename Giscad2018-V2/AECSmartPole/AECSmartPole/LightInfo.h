#pragma once

class CLightInfo
{
public:
	CLightInfo(void);
	~CLightInfo(void);

	int     m_iLightNumber;
	CString m_csLightNumber; 
	CString m_csIsStandard;
	CString m_csLanternSchedule;
	CString m_csLanternStatus;
	CString m_csLanternType;
	CString m_csLanternStockCode;
	CString m_csLanternLamp;
	CString m_csLampStockCode;
	CString m_csRateCode;
	CString m_csKingbolt;
	CString m_csLanternMast;
	CString m_csMastCode;
	CString m_csLanternOutreach;
	CString m_csOutreachCode;
	CString m_csLanternBracket;
	CString m_csBracketCode;
	CString m_csLanternLocation;
	CString m_csLanternCategory;
	CString m_csAssetColumn;
	CString m_csAssetLantern;
	CString m_csLanternColumn;
	CString m_csChargeTo;

	CString m_csRagBolt;
	CString m_csFoundationType;
	CString m_csMASTRATE;
	CString m_csOutreachRate;
	CString m_csBracketRate;	
	CString m_csMountingHeight;

	CString m_csTariffClass;
	CString m_csLuminaireRate;
	CString m_csPoleColumnRate; 
	

	CString m_csReplaceIsStandard;
	CString m_csReplaceLanternSchedule;
	CString m_csReplaceLanternType;
	CString m_csReplaceLanternLamp;
	CString m_csReplaceRateCode;
	CString m_csReplaceLanternCategory;
	CString m_csReplaceAssetColumn;
	CString m_csReplaceAssetLantern;
	CString m_csReplaceChargeTo;

	int m_iQty;
	int m_iColumnQty;
};
