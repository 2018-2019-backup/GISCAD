#include "stdafx.h"
#include "..\..\Include\NSValidatorIntface.h"
#include "NSBaseValidator.h"

IValidator* createValidator()
{  
	return new CNSBaseValidator; 
}