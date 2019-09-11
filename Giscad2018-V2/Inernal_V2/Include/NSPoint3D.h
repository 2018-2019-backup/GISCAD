#pragma once

class _declspec(dllexport)CNSPoint3D
{
public:
	CNSPoint3D(void);
	virtual ~CNSPoint3D(void);

	CNSPoint3D(double dXX, double dYY, double dZZ = 0.0);

	double m_dXX;
	double m_dYY;
	double m_dZZ;
};
