#pragma once


// GISCADConvertorDlg dialog

class GISCADConvertorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GISCADConvertorDlg)

public:
	GISCADConvertorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GISCADConvertorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_IMPORT_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
