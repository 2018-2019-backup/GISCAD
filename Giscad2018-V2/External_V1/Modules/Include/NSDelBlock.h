#pragma once

#include "string"
#include "..\Include\NSDirectives.h"

class CNSDelBlock
{
public:
	NSSTRING szBlockName;
	NSSTRING szUsage;
	NSSTRING szOrientation;
	NSSTRING FID;
	NSSTRING FNO;

public:
	CNSDelBlock(void){};
	~CNSDelBlock(void){};
};
