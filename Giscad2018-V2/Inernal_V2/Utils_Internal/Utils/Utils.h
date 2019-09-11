// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTILS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTILS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif

// This class is exported from the Utils.dll
class UTILS_API CUtils {
public:
	CUtils(void);
	// TODO: add your methods here.
};

extern UTILS_API int nUtils;

UTILS_API int fnUtils(void);
