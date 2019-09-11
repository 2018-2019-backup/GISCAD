#include "StdAfx.h"
#include "..\..\Include\NSTextHeight.h"
#include <string>

CNSTextHeight::CNSTextHeight()
{
}
CNSTextHeight::~CNSTextHeight()
{

}
double CNSTextHeight::getHeight()
{
	return m_dHeight;
}
double CNSTextHeight::getXPosition()
{
	return m_dXPosition;
}
double CNSTextHeight::getYPosition()
{
	return m_dYPosition;
}
void CNSTextHeight::setHeight(double dHeight)
{
	m_dHeight = dHeight;
}
void CNSTextHeight::setXPosition(double dXPosition)
{
	m_dXPosition = dXPosition;

}
void CNSTextHeight::setYPosition(double dYPosition)
{
	m_dYPosition = dYPosition;
}