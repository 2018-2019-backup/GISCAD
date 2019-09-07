/*!
 @Class		:	CNSHeader
 @Brief		:	This is the base class for Export and import header classes.
 @Author	:	Neilsoft Ltd
 @Date		:	06-07-2007
 @History 	:	Change history
 */
#pragma once
class CNSHeader
{
public:
	CNSHeader(void);
	virtual int readXML();
	virtual int writeDWG();
	virtual int readDWG();
	virtual int writeXML();
public:
	~CNSHeader(void);
};
