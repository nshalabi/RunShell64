//
// +---------+----------------------------------------------------------------+
// | AUTHOR  | NADER SHALLABI                                                 |
// +---------+----------------------------------------------------------------+
// | CONTACT | nader@nosecurecode.com                                         |
// +--------------------------------------------------------------------------+
// | This sample code is free for use, redistributionand /or                  |
// | modification without any explicit permission from the author.            |
// |                                                                          |
// | This sample code is distributed in the hope that it will be useful,      |
// | but WITHOUT ANY WARRANTY, implied or explicit.                           |
// +--------------------------------------------------------------------------+
//

#include <vcl.h>
#include <TlHelp32.h>
#pragma hdrstop

#include "MainUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	bLoadedShellCode = false;
	Application->OnException = AppException;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::AppException(TObject *Sender, Exception *E) {
	String exceptionMessage = "Exception : ";
	exceptionMessage += E->Message;
	LogAction(exceptionMessage);
}

// ---------------------------------------------------------------------------
void TMainForm::LogAction(String sAction) {
	String Current = DateTimeToStr(Now());
	MemoLogs->Lines->Add(Current + " : " + sAction);
}

// ---------------------------------------------------------------------------
void TMainForm::Run64Shellcode(String sFilename, bool bDebug) {
	DWORD dwflOldProtect, dwBytesRead;
	DWORD nLength;
	HANDLE hFile;
	DWORD nOffset;

	// Check offset first
	try {
		nOffset = StrToInt(UnicodeString("$") + EditOffset->Text);
	}
	catch (...) {
		// Don't care, return back
		LogAction("Invalid offset value");
		return;
	}

	hFile = CreateFile(sFilename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		LogAction(Format("Error opening file (%d)",
			ARRAYOFCONST((GetLastError()))));

		return;
	}

	nLength = GetFileSize(hFile, NULL);

	if (-1 == nLength) {
		LogAction(Format("Error getting file size (%d)",
			ARRAYOFCONST((GetLastError()))));

		return;
	}

	lpBuffer = malloc(nLength);

	if (NULL == lpBuffer) {
		LogAction(Format("Error allocating memory (%d)",
			ARRAYOFCONST((GetLastError()))));

		return;
	}

	// Change memory protection
	if (0 == VirtualProtect(lpBuffer, nLength, PAGE_EXECUTE_READWRITE,
		&dwflOldProtect)) {
		LogAction(Format("Error in adjusting memory protection (%d)",
			ARRAYOFCONST((GetLastError()))));

		return;
	}

	// Read shellcode buffer
	if (ReadFile(hFile, lpBuffer, nLength, &dwBytesRead, NULL) == FALSE) {
		LogAction(Format("Error reading shellcode file (%d)",
			ARRAYOFCONST((GetLastError()))));

		return;
	}

	// Prepare to jump
	lpFunc = (JUMPTOCODE)(((LPBYTE)lpBuffer) + nOffset);

	bLoadedShellCode = true;

	if (bDebug) {
		// If we are debugging, give the user a chance to set a breakpoint
		String promptMessage = "Running shellcode now!\nSet a breakpoint at 0x";
		promptMessage += IntToHex((__int64)lpFunc);
		promptMessage += "\nthen select Ok, otherwise select Cancel to abort";
		int nSelected = MessageDlg(promptMessage, mtInformation,
			TMsgDlgButtons() << mbOK << mbCancel, 0);

		if (nSelected == mrCancel) {
			LogAction("Aborted shellcode execution");
			return;
		}
	}

	// Execute Shellcode
	LogAction("Running shellcode at 0x" + IntToHex((__int64)lpFunc));

	// Close the file
	CloseHandle(hFile);

	__asm {
		call lpFunc
	}
}

// ---------------------------------------------------------------------------
void TMainForm::Run64ShellcodeNoPrep(bool bDebug) {

	if (bDebug) {
		// If we are debugging, give the user a chance to set a breakpoint
		String promptMessage = "Running shellcode now!\nSet a breakpoint at 0x";
		promptMessage += IntToHex((__int64)lpFunc);
		promptMessage += "\nthen select Ok, otherwise select Cancel to abort";

		int nSelected = MessageDlg(promptMessage, mtInformation,
			TMsgDlgButtons() << mbOK << mbCancel, 0);

		if (nSelected == mrCancel) {
			LogAction("Aborted shellcode execution");
			return;
		}
	}

	// Execute Shellcode
	LogAction("Running shellcode at 0x" + IntToHex((__int64)lpFunc));

	__asm {
		call lpFunc
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::LoadAdditionalFiles() {
	HANDLE hAdditionalFileHandle;

	if (ListBoxAdditionalFiles->Items->Count > 0) {
		for (int i = 0; i < ListBoxAdditionalFiles->Items->Count; i++) {
			hAdditionalFileHandle =
				CreateFile(ListBoxAdditionalFiles->Items->Strings[i].c_str(),
				GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);

			// Something went wrong
			if (hAdditionalFileHandle == INVALID_HANDLE_VALUE) {
				LogAction("Error adding file (" +
					ListBoxAdditionalFiles->Items->Strings[i] + ")");
			}
			else {
				LogAction("Added file (" +
					ListBoxAdditionalFiles->Items->Strings[i] + ")" +
					" File handle (0x" +
					IntToHex((__int64)hAdditionalFileHandle) + ")");
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonRunClick(TObject *Sender) {
	if (1 == EditShellcodeFile->Tag) {
		if (CheckBoxEnableAdditionalFiles->Checked) {
			LoadAdditionalFiles();
		}

		// Keep reloading the shellcode at different memory locations
		if (CheckBoxReload->Checked) {
			// Clean up previous run session
			if (bLoadedShellCode) {
				free(lpBuffer);
				lpBuffer = NULL;
				bLoadedShellCode = false;
			}

			Run64Shellcode(EditShellcodeFile->Text, CheckBoxDebug->Checked);
		}
		else {
			// If we had a previous run, just execute the loaded shellcode again
			if (bLoadedShellCode) {
				Run64ShellcodeNoPrep(CheckBoxDebug->Checked);
			}
			else {
				// First time
				Run64Shellcode(EditShellcodeFile->Text, CheckBoxDebug->Checked);
			}
		}
	}
	else {
		LogAction("Load a shellcode file first");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonLoadShellcodeFileClick(TObject *Sender) {
	if (OpenDialogShellcode->Execute()) {
		EditShellcodeFile->Text = OpenDialogShellcode->FileName;
		EditShellcodeFile->Tag = 1; // Modification flag
		LogAction("Loaded shellcode file (" + EditShellcodeFile->Text + ")");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddFileClick(TObject *Sender) {
	if (OpenDialogShellcode->Execute()) {
		ListBoxAdditionalFiles->Items->Add(OpenDialogShellcode->FileName);
		LogAction("Added additonal file (" +
			OpenDialogShellcode->FileName + ")");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBoxEnableAdditionalFilesClick(TObject *Sender) {
	ListBoxAdditionalFiles->Enabled = CheckBoxEnableAdditionalFiles->Checked;
	ButtonAddFile->Enabled = CheckBoxEnableAdditionalFiles->Checked;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender) {
	if (!IsDebuggerPresent()) {
		LabelWarning->Visible = true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonAboutClick(TObject *Sender) {
	ShowMessage("  Nader Shalabi - (C) 2020\n  nader@nosecurecode.com");
}
// ---------------------------------------------------------------------------
