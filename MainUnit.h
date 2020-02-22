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

#ifndef MainUnitH
#define MainUnitH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>

// ---------------------------------------------------------------------------
typedef void(*JUMPTOCODE)(void);
// ---------------------------------------------------------------------------
class TMainForm : public TForm {
__published: // IDE-managed Components
	TMemo *MemoLogs;
	TEdit *EditShellcodeFile;
	TButton *ButtonLoadShellcodeFile;
	TLabel *LabelFile;
	TOpenDialog *OpenDialogShellcode;
	TGroupBox *GroupBoxOptions;
	TCheckBox *CheckBoxDebug;
	TCheckBox *CheckBoxEnableAdditionalFiles;
	TListBox *ListBoxAdditionalFiles;
	TButton *ButtonAddFile;
	TLabel *LabelLog;
	TPanel *PanelBottom;
	TButton *ButtonRun;
	TLabel *LabelOffset;
	TEdit *EditOffset;
	TCheckBox *CheckBoxReload;
	TLabel *LabelWarning;
	TButton *ButtonAbout;

	void __fastcall ButtonRunClick(TObject *Sender);
	void __fastcall ButtonLoadShellcodeFileClick(TObject *Sender);
	void __fastcall ButtonAddFileClick(TObject *Sender);
	void __fastcall CheckBoxEnableAdditionalFilesClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonAboutClick(TObject *Sender);

private: // User declarations
	JUMPTOCODE lpFunc;
    LPVOID lpBuffer;
	bool bLoadedShellCode;
    void __fastcall AppException(TObject *Sender, Exception *E);
	void __fastcall Run64Shellcode(String sFilename, bool bDebug);
    void __fastcall Run64ShellcodeNoPrep(bool bDebug);
	void __fastcall LogAction(String sAction);
	void __fastcall LoadAdditionalFiles();

public: // User declarations
	__fastcall TMainForm(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
