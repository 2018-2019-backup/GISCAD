#include "stdafx.h"
#include "..\..\Include\NSConvertorIntface.h"
#include "NSBaseConvertor.h"

IConvertor* createConvertor()
{  
	return new CNSBaseConvertor; 
}