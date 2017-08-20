//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Main.cpp", cppInjectorClass);
USEFORM("About.cpp", cppAboutClass);
USEFORM("ModuleUnloader.cpp", ModuleUnloaderClass);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "cppInjector v.1.2";
		Application->CreateForm(__classid(TcppInjectorClass), &cppInjectorClass);
		Application->CreateForm(__classid(TcppAboutClass), &cppAboutClass);
		Application->CreateForm(__classid(TModuleUnloaderClass), &ModuleUnloaderClass);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
