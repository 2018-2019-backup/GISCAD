#pragma once


// GISCADConverterDialog dialog

class GISCADConverterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(GISCADConverterDialog)

public:
	GISCADConverterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GISCADConverterDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_IMPORT_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
