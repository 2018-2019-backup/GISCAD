#pragma once

// CIEComboBox

class CIEComboBox : public CComboBox
{
	// DECLARE_DYNAMIC(CIEComboBox)

	public:
		CIEComboBox();
		virtual ~CIEComboBox();

		void SwitchToSimple();
		void SwitchToDropDownList();
		void SetMaxChars(int nChars);

	protected:
		BOOL RecreateComboBox(LPVOID lpParam = 0);
		// DECLARE_MESSAGE_MAP()

	private:
		int m_nDropHeight;
		int m_nMaxChars;
};


