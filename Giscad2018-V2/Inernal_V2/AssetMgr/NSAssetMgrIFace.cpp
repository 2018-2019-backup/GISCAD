#include "StdAfx.h"
#include "NSAssetHandler.h"

IAssetMgr* createAssetMgr()
{  
	return new CNSAssetHandler; 
}