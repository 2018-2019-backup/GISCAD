// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VALIDATOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VALIDATOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef VALIDATOR_EXPORTS
#define VALIDATOR_API __declspec(dllexport)
#else
#define VALIDATOR_API __declspec(dllimport)
#endif

// This class is exported from the Validator.dll
class VALIDATOR_API CValidator {
public:
	CValidator(void);
	// TODO: add your methods here.
};

extern VALIDATOR_API int nValidator;

VALIDATOR_API int fnValidator(void);
