// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the REALDWGPROCESSOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// REALDWGPROCESSOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef REALDWGPROCESSOR_EXPORTS
#define REALDWGPROCESSOR_API __declspec(dllexport)
#else
#define REALDWGPROCESSOR_API __declspec(dllimport)
#endif

// This class is exported from the RealDWGProcessor.dll
class REALDWGPROCESSOR_API CRealDWGProcessor {
public:
	CRealDWGProcessor(void);
	// TODO: add your methods here.
};

extern REALDWGPROCESSOR_API int nRealDWGProcessor;

REALDWGPROCESSOR_API int fnRealDWGProcessor(void);
