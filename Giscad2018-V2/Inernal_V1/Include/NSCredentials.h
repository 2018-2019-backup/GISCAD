#pragma once

class CNSTitleBlkInfo
{
public:
	TCHAR m_szCAP_NO[_MAX_PATH + 1];
	TCHAR m_szDrawingNumber[_MAX_PATH + 1];
	TCHAR m_szProjectNumber[_MAX_PATH + 1];
	TCHAR m_szProjectType[_MAX_PATH + 1];
	TCHAR m_szDepotCode[_MAX_PATH + 1];
	TCHAR m_szExportID[_MAX_PATH + 1];
};

class CNSUserCredentials
{
public:
	TCHAR m_szUserName[_MAX_PATH + 1];
	TCHAR m_szPassword[_MAX_PATH + 1];
	TCHAR m_szDistrict[_MAX_PATH + 1];
	TCHAR m_szPosition[_MAX_PATH + 1];
	TCHAR m_szHostID[_MAX_PATH + 1];
};

enum ASSET_TYPE
{
	ASSET_INVALID = -1,
	ASSET_POLE = 0,
	ASSET_COLUMN = 1,
	ASSET_PILLAR = 2,
	ASSET_SL = 3,
	ASSET_SLCP = 4,
	ASSET_CUBICLE = 5,
	ASSET_LVSERVICEPILLAR = 6,
	ASSET_SWITCH = 7,
	ASSET_SUBSTATION = 8,
	ASSET_SUBSWITCH = 9
};