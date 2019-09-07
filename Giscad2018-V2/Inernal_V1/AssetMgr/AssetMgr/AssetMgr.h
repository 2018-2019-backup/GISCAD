// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ASSETMGR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ASSETMGR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ASSETMGR_EXPORTS
#define ASSETMGR_API __declspec(dllexport)
#else
#define ASSETMGR_API __declspec(dllimport)
#endif

// This class is exported from the AssetMgr.dll
class ASSETMGR_API CAssetMgr {
public:
	CAssetMgr(void);
	// TODO: add your methods here.
};

extern ASSETMGR_API int nAssetMgr;

ASSETMGR_API int fnAssetMgr(void);
