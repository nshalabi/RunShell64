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
// CRunShell64Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CRunShell64.h"
#include "CRunShell64Dlg.h"
#include "afxdialogex.h"

#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCRunShell64Dlg dialog



CCRunShell64Dlg::CCRunShell64Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CRUNSHELL64_DIALOG, pParent)
	, m_bEnableDebugging(TRUE)
	, m_strLog(_T(""))
	, m_strOffset(_T("0"))
	, m_bInsertBreakpoint(FALSE)
	, m_bCloseShellcodeFile(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCRunShell64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SHELLCODE_FILE, m_EditShellcodeFile);
	DDX_Check(pDX, IDC_CHECK_ENABLE_DEBUGGING, m_bEnableDebugging);
	DDX_Control(pDX, IDC_CHECK_ADD_FILE_HANDLES, m_CheckBoxEnableAdditionalFiles);
	DDX_Control(pDX, IDC_LIST_FILES_HANDLES, m_ListBoxAdditionalFiles);
	DDX_Control(pDX, IDC_BUTTON_ADD_FILE, m_ButtonAddFile);
	DDX_Text(pDX, IDC_EDIT_LOGS, m_strLog);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_strOffset);
	DDX_Check(pDX, IDC_CHECK_INSERT_BREAKPOINT, m_bInsertBreakpoint);
	DDX_Check(pDX, IDC_CHECK_CLOSE_SHELLCODE_FILE, m_bCloseShellcodeFile);
	DDX_Control(pDX, IDC_STATIC_WARNING, m_LabelWarning);
}

BEGIN_MESSAGE_MAP(CCRunShell64Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_ADD_FILE_HANDLES, &CCRunShell64Dlg::OnBnClickedCheckAddFileHandles)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT_I, &CCRunShell64Dlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILE, &CCRunShell64Dlg::OnBnClickedButtonAddFile)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SHELLCODE, &CCRunShell64Dlg::OnBnClickedButtonLoadShellcode)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CCRunShell64Dlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DEBUGGING, &CCRunShell64Dlg::OnBnClickedCheckEnableDebugging)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CCRunShell64Dlg::LogAction(CString strAction) {
	CTime tNow = CTime::GetCurrentTime();
	CString strTime = tNow.Format("%d/%m/%Y %H:%M:%S : ");

	m_strLog += strTime;
	m_strLog += strAction;
	m_strLog += "\r\n";

	UpdateData(FALSE);
}

void CCRunShell64Dlg::Run64Shellcode(CString strFilename, bool bDebug) {
	DWORD  dwflOldProtect, dwBytesRead;
	DWORD  nLength;
	HANDLE hFile;
	DWORD  nOffset;

	// Check offset first
	try {
		nOffset = stoi(m_strOffset.GetString(), 0, 16);
	}
	catch (...) {
		// Don't care, return back
		LogAction(CString("Invalid offset value"));
		return;
	}

	hFile = CreateFile(strFilename.GetString(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error opening file (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	nLength = GetFileSize(hFile, NULL);

	if (-1 == nLength) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error getting file size (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	m_lpBuffer = malloc(nLength);

	if (NULL == m_lpBuffer) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error allocating memory (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Change memory protection
	if (0 == VirtualProtect(m_lpBuffer, nLength, PAGE_EXECUTE_READWRITE,
		&dwflOldProtect)) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error in adjusting memory protection (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Read shellcode buffer
	if (ReadFile(hFile, m_lpBuffer, nLength, &dwBytesRead, NULL) == FALSE) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error reading shellcode file (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Prepare to jump
	m_lpFunc = (JUMPTOCODE)(((LPBYTE)m_lpBuffer) + nOffset);

	// Flag we had a first run, so we get a chance to free resources for next run
	m_bLoadedShellCode = true;

	// Alert so we can set a BP accordingly in Debugger
	if (bDebug) {
		// If we are debugging, give the user a chance to set a breakpoint
		CString promptMessage = L"";
		promptMessage.Format(L"Running Shellcode now!\nSet a breakpoint at 0x%llx\nthen select Ok, otherwise select Cancel to abort", m_lpFunc);
		int nSelected = MessageBox(promptMessage, L"Set debugger breakpoint!", MB_ICONINFORMATION | MB_OKCANCEL);

		if (nSelected == IDCANCEL) {
			LogAction(CString("Aborted Shellcode execution"));
			if (m_bCloseShellcodeFile)
			{
				CloseHandle(hFile);
			}
			return;
		}
	}

	// Execute Shellcode
	{
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Running Shellcode at 0x%llx", m_lpFunc);
		LogAction(strLogMsg);
	}

	// Close the Shellcode file before run, if this is not desired, comment the following line,
	// however, it is needed in case teh user is running the Shellcode multiple times
	if (m_bCloseShellcodeFile)
	{
		CloseHandle(hFile);
	}

	m_lpFunc();
}

void CCRunShell64Dlg::Run64ShellcodeWithBP(CString strFilename, bool bDebug) {
	DWORD  dwflOldProtect, dwBytesRead;
	DWORD  nLength;
	HANDLE hFile;
	DWORD  nOffset;
	LPVOID lpOffsetBuffer;

	// Check offset first
	try {
		nOffset = stoi(m_strOffset.GetString(), 0, 16);
	}
	catch (...) {
		// Don't care, return back
		LogAction(CString("Invalid offset value"));
		return;
	}

	hFile = CreateFile(strFilename.GetString(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error opening file (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	nLength = GetFileSize(hFile, NULL);

	if (-1 == nLength) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error getting file size (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	m_lpBuffer = malloc(nLength - nOffset + 1);
	lpOffsetBuffer = malloc(nLength);

	if (NULL == m_lpBuffer || NULL == lpOffsetBuffer) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error allocating memory (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Change memory protection
	if (0 == VirtualProtect(m_lpBuffer, nLength + 1, PAGE_EXECUTE_READWRITE,
		&dwflOldProtect)) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error in adjusting memory protection (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Read shellcode buffer
	if (ReadFile(hFile, lpOffsetBuffer, nLength, &dwBytesRead, NULL) == FALSE) {
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Error reading shellcode file (%x)", GetLastError());
		LogAction(strLogMsg);

		return;
	}

	// Shift the buffer by 1 to insert BP (0xCC)
	memset(m_lpBuffer, nLength - nOffset + 1, 0);
	memcpy((LPBYTE)m_lpBuffer + 1, (LPBYTE)lpOffsetBuffer + nOffset, nLength - nOffset);
	((LPBYTE)m_lpBuffer)[0] = 0xCC;

	// Free temporary buffer
	free(lpOffsetBuffer);

	// Cast to function pointer
	m_lpFunc = (JUMPTOCODE)m_lpBuffer;

	// Flag we had a first run, so we get a chance to free resources for next run
	m_bLoadedShellCode = true;

	// Should never happen anyhow, if the BP option is set, then debugging should be enabled
	// The MessageBox would still be a good indicator we are going to break.
	// If this is not desired, comment the if(bDebug) {...} block
	if (bDebug) {
		// If we are debugging, give the user a chance to set a breakpoint
		CString promptMessage = L"";
		promptMessage.Format(L"Running Shellcode now!\nBreakpoint already set at 0x%llx\nSelect Ok to run Shellcode, or Cancel to abort", m_lpFunc);
		int nSelected = MessageBox(promptMessage, L"Run Shellcode?", MB_ICONINFORMATION | MB_OKCANCEL);

		if (nSelected == IDCANCEL) {
			LogAction(CString("Aborted Shellcode execution"));
			if (m_bCloseShellcodeFile)
			{
				CloseHandle(hFile);
			}
			return;
		}
	}

	// Execute Shellcode
	{
		CString strLogMsg = CString("");
		strLogMsg.Format(L"Running Shellcode at 0x%llx", m_lpFunc);
		LogAction(strLogMsg);
	}

	// Close the Shellcode file before run, if this is not desired, comment the following line,
	// however, it is needed in case teh user is running the Shellcode multiple times
	if (m_bCloseShellcodeFile)
	{
		CloseHandle(hFile);
	}

	m_lpFunc();
}

void CCRunShell64Dlg::LoadAdditionalFiles()
{
	HANDLE hAdditionalFileHandle;
	CString strItem, strLogMsg;
	int nIndex;

	if (m_ListBoxAdditionalFiles.GetCount() > 0) {
		for (int i = 0; i < m_ListBoxAdditionalFiles.GetCount(); i++) {
			nIndex = m_ListBoxAdditionalFiles.GetTextLen(i);
			m_ListBoxAdditionalFiles.GetText(i, strItem.GetBuffer(nIndex));
			strItem.ReleaseBuffer();

			hAdditionalFileHandle =
				CreateFile(strItem.GetBuffer(0),
					GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);

			// Something went wrong
			if (hAdditionalFileHandle == INVALID_HANDLE_VALUE) {
				strLogMsg.Format(L"Error adding file (%s)!", strItem.GetBuffer(0));
				LogAction(strLogMsg);
			}
			else {
				strLogMsg.Format(L"Added Handle (%llx) for file (%s)", hAdditionalFileHandle, strItem.GetBuffer(0));
				LogAction(strLogMsg);
			}
		}
	}
	else
	{
		strLogMsg.Format(L"No additional files to add!");
		LogAction(strLogMsg);
	}
}

// CCRunShell64Dlg message handlers

BOOL CCRunShell64Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialization
	m_bLoadedShellCode = false;
	m_bSetShellcodeFile = false;
	m_lpBuffer = NULL;

	if (!IsDebuggerPresent()) {
		m_LabelWarning.ShowWindow(TRUE);
	}

	// Update controls
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCRunShell64Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCRunShell64Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCRunShell64Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCRunShell64Dlg::OnBnClickedCheckAddFileHandles()
{
	m_ListBoxAdditionalFiles.EnableWindow(TRUE);
	m_ButtonAddFile.EnableWindow(TRUE);
}


void CCRunShell64Dlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CCRunShell64Dlg::OnBnClickedButtonAddFile()
{
	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, L"All Files(*.*)|*.*||", this);

	fileDlg.m_ofn.lpstrTitle = L"Add additional files";

	if (fileDlg.DoModal() == IDOK)
	{
		CString strPathname = fileDlg.GetPathName();
		int nIndex = m_ListBoxAdditionalFiles.FindString(0, strPathname);
		if (nIndex == LB_ERR)
		{
			m_ListBoxAdditionalFiles.AddString(strPathname);
		}
		else
		{
			LogAction(CString("File already selected!"));
		}
	}
}


void CCRunShell64Dlg::OnBnClickedButtonLoadShellcode()
{
	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, L"Shellcode Files(*.*)|*.*||", this);

	fileDlg.m_ofn.lpstrTitle = L"Load Shellcode file";

	if (fileDlg.DoModal() == IDOK)
	{
		CString strPathname = fileDlg.GetPathName();
		m_EditShellcodeFile.SetWindowTextW(strPathname);
		m_bSetShellcodeFile = true;
	}
}


void CCRunShell64Dlg::OnBnClickedButtonRun()
{
	CString strShellcodeFile;

	// Make sure we have the updated data
	UpdateData();

	m_EditShellcodeFile.GetWindowTextW(strShellcodeFile);

	if (m_bSetShellcodeFile) {
		if (m_CheckBoxEnableAdditionalFiles.GetCheck() == BST_CHECKED) {
			// First run, load files once, if any additional files are added
			// after first run, they wont be loaded
			if (!m_bLoadedShellCode) {
				LoadAdditionalFiles();
			}
		}

		// Keep reloading the Shellcode at different memory locations
		// Clean up previous run session
		if (m_bLoadedShellCode && m_lpBuffer != NULL) {
			free(m_lpBuffer);
			m_lpBuffer = NULL;
			m_bLoadedShellCode = false;
		}

		if (m_bInsertBreakpoint) {
			Run64ShellcodeWithBP(strShellcodeFile, m_bEnableDebugging);
		}
		else {
			Run64Shellcode(strShellcodeFile, m_bEnableDebugging);
		}
	}
	else {
		LogAction(L"Load a shellcode file first!");
	}
}


void CCRunShell64Dlg::OnBnClickedCheckEnableDebugging()
{
	UpdateData(TRUE);
	CButton* pCheckBoxItem = (CButton*)GetDlgItem(IDC_CHECK_INSERT_BREAKPOINT);
	if (!m_bEnableDebugging) {
		pCheckBoxItem->EnableWindow(FALSE);
		pCheckBoxItem->SetCheck(BST_UNCHECKED);
	}
	else {
		pCheckBoxItem->EnableWindow(TRUE);
	}
	UpdateData(TRUE);
}


HBRUSH CCRunShell64Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Set the warning message to RED color
	if (nCtlColor == CTLCOLOR_STATIC) {
		if (GetDlgItem(IDC_STATIC_WARNING)->m_hWnd == pWnd->m_hWnd)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		else
		{
			return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	}
	else
	{
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}
