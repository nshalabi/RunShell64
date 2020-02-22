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
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainUnit.cpp", MainForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "x64 Shellcode Runner";
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		ShowMessage("Test1");
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			ShowMessage("Test2");
			throw Exception("");
		}
		catch (Exception &exception)
		{
			ShowMessage("Test3");
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
