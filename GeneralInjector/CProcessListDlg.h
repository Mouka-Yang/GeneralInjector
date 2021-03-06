#pragma once

#include "SortListCtrl.h"

#define COL_PROC_NAME  0
#define COL_PID		   1
#define COL_IMAGE_PATH 2
#define COL_IMAGE_TYPE 3

#define IMAGE_32    _T("32 bit")
#define IMAGE_64    _T("64 bit")
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

	void EnumProcess();
	void UpdataProcessList();
	DECLARE_MESSAGE_MAP()
public:
	CSortListCtrl m_ProcessListCtrl;
	CString m_SelectedPid;
	CString m_SelectedProcessName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRefresh();
};
