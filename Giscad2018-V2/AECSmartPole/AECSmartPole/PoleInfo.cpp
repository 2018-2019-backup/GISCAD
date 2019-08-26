#include "stdafx.h"
#include "poleinfo.h"
#include <vector>

CPoleInfo::CPoleInfo(void)
{
	m_csPoleNumber = _T("");
	m_csPoleStatus  = _T("");
	m_csFieldNumberExist  = _T("");
	m_csFieldNumberNew  = _T("");
	m_csPoleSize  = _T("");
	m_csPoleStay  = _T("");
	m_csPoleFooting  = _T("");
	m_csPoleDia  = _T("");
	m_csHoleDepth  = _T("");
	m_csHoleDia  = _T("");
	m_csPoleHV  = _T("");
	m_csPoleLV  = _T("");
	m_csLanternStatus  = _T("");
	m_csLanternBracket  = _T("");
	m_csSpanLength  = _T("");
	m_csLineDevDeg  = _T("");
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : CPoleInfo::~CPoleInfo
// Description  : Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPoleInfo::~CPoleInfo(void)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function name: sortPoleInfo
// Description  : Sorts the vector array of pole information instances in ascending order of pole numbers.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void sortPoleInfo (std:: vector <CPoleInfo> &poleInfo)
{
	CPoleInfo tmpPoleInfo;
	for (int i = 0; i < poleInfo.size() - 1; i++)
	{
		for (int j = i + 1; j < poleInfo.size(); j++)
		{
			if (_ttoi(poleInfo[i].m_csPoleNumber) > _ttoi(poleInfo[j].m_csPoleNumber))
			{
				tmpPoleInfo = poleInfo[i]; // Swapping entire class
				poleInfo[i] = poleInfo[j];
				poleInfo[j] = tmpPoleInfo;
			}
		}
	}

	return;
}
