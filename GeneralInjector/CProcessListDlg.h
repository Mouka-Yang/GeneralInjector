#pragma once

#define COL_PROC_NAME  0
#define COL_PID		   1
#define COL_IMAGE_PATH 2

// CProcessListDlg dialog

class CProcessListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessListDlg)

public:
	CProcessListDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CProcessListDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	void AddData(int row, int col, LPTSTR str);
	void EnumProcess();
	void UpdataProcessList();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ProcessListCtrl;
	afx_msg void OnBnClickedButton1();
};
