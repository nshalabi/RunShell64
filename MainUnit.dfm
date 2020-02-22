object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'RunShell64 - 1.0'
  ClientHeight = 590
  ClientWidth = 748
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LabelFile: TLabel
    Left = 8
    Top = 16
    Width = 62
    Height = 13
    Caption = 'Shellcode file'
  end
  object LabelLog: TLabel
    AlignWithMargins = True
    Left = 3
    Top = 317
    Width = 742
    Height = 13
    Align = alBottom
    Caption = '  Log'
    ExplicitWidth = 23
  end
  object LabelOffset: TLabel
    Left = 575
    Top = 16
    Width = 46
    Height = 13
    Caption = 'Offset 0x'
  end
  object MemoLogs: TMemo
    AlignWithMargins = True
    Left = 3
    Top = 336
    Width = 742
    Height = 204
    Align = alBottom
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object EditShellcodeFile: TEdit
    Left = 76
    Top = 12
    Width = 445
    Height = 21
    Enabled = False
    ReadOnly = True
    TabOrder = 0
  end
  object ButtonLoadShellcodeFile: TButton
    Left = 527
    Top = 12
    Width = 42
    Height = 21
    Caption = '...'
    TabOrder = 1
    OnClick = ButtonLoadShellcodeFileClick
  end
  object GroupBoxOptions: TGroupBox
    Left = 0
    Top = 39
    Width = 748
    Height = 275
    Align = alBottom
    Caption = 'Options'
    TabOrder = 2
    object CheckBoxDebug: TCheckBox
      Left = 10
      Top = 24
      Width = 135
      Height = 17
      Caption = 'Enable debugging'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CheckBoxEnableAdditionalFiles: TCheckBox
      Left = 10
      Top = 47
      Width = 151
      Height = 17
      Caption = 'Add additional file handles'
      TabOrder = 1
      OnClick = CheckBoxEnableAdditionalFilesClick
    end
    object ListBoxAdditionalFiles: TListBox
      Left = 10
      Top = 70
      Width = 730
      Height = 169
      Enabled = False
      ItemHeight = 13
      TabOrder = 2
    end
    object ButtonAddFile: TButton
      Left = 10
      Top = 245
      Width = 95
      Height = 25
      Caption = 'Add file handles'
      Enabled = False
      TabOrder = 3
      OnClick = ButtonAddFileClick
    end
    object CheckBoxReload: TCheckBox
      Left = 184
      Top = 24
      Width = 185
      Height = 17
      Caption = 'Reload shellcode with every run'
      TabOrder = 4
    end
  end
  object PanelBottom: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 546
    Width = 742
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
    object LabelWarning: TLabel
      Left = 86
      Top = 16
      Width = 131
      Height = 13
      Caption = 'No debugger detected !'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      Visible = False
    end
    object ButtonRun: TButton
      Left = 5
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Run'
      TabOrder = 0
      OnClick = ButtonRunClick
    end
    object ButtonAbout: TButton
      Left = 710
      Top = 8
      Width = 27
      Height = 25
      Caption = '!'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = ButtonAboutClick
    end
  end
  object EditOffset: TEdit
    Left = 627
    Top = 12
    Width = 113
    Height = 21
    TabOrder = 5
    Text = '0'
  end
  object OpenDialogShellcode: TOpenDialog
    DefaultExt = '*.*'
    Filter = 'All Files (*.*)|*.*'
    Options = [ofReadOnly, ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open Shellcode File'
    Left = 224
    Top = 120
  end
end
