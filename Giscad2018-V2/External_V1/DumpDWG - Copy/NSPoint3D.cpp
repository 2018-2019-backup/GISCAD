//#include "StdAfx.h"
#include "..\Include\NSPoint3D.h"

CNSPoint3D::CNSPoint3D(void)
{
	m_dXX = 0;
	m_dYY = 0;
	m_dZZ = 0;
}

CNSPoint3D::~CNSPoint3D()
{
}	

CNSPoint3D::CNSPoint3D(double dXX, double dYY, double dZZ)
{
	m_dXX = dXX;
	m_dYY = dYY;
	m_dZZ = dZZ;
}
