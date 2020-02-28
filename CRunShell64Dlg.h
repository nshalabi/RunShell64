//
// +---------+----------------------------------------------------------------+
// | AUTHOR  | NADER SHALLABI                                                 |
// +---------+----------------------------------------------------------------+
// | CONTACT | nader@nosecurecode.com                                         |
// +--------------------------------------------------------------------------+
// | This sample code is free for use, redistribution and /or                 |
// | modification without any explicit permission from the author.            |
// |                                                                          |
// | This sample code is distributed in the hope that it will be useful,      |
// | but WITHOUT ANY WARRANTY, implied or explicit.                           |
// +--------------------------------------------------------------------------+
//
// CRunShell64Dlg.h : header file
//

#pragma once

// Pointer to function type (point to Shellcode)
typedef void(*JUMPTOCODE)(void);

// CCRunShell64Dlg dialog
class CCRunShell64Dlg : public CDialogEx
{
// Construction
public:
	CCRunShell64Dlg(CWnd* pParent = nullptr);	// standard constructor

protected:
	// If true, Shellcode is loaded in memory
	bool m_bLoadedShellCode;
	// If true, Shellcode file was set
	bool m_bSetShellcodeFile;
	// Shellcode buffer
	LPVOID m_lpBuffer;
	// Pointer to Shellcode
	JUMPTOCODE m_lpFunc;

	void LogAction(CString strAction);
	void LoadAdditionalFiles();
	void Run64Shellcode(CString strFilename, bool bDebug);
	void Run64ShellcodeWithBP(CString strFilename, bool bDebug);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRUNSHELL64_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL     m_bEnableDebugging;
	BOOL     m_bInsertBreakpoint;
	BOOL     m_bCloseShellcodeFile;
	CString  m_strLog;
	CString  m_strOffset;
	CEdit    m_EditShellcodeFile;
	CButton  m_CheckBoxEnableAdditionalFiles;
	CListBox m_ListBoxAdditionalFiles;
	CButton  m_ButtonAddFile;

	afx_msg void OnBnClickedCheckAddFileHandles();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonAddFile();
	afx_msg void OnBnClickedButtonLoadShellcode();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedCheckEnableDebugging();
	CStatic m_LabelWarning;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
