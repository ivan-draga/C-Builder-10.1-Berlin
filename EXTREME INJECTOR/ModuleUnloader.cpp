//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ModuleUnloader.h"
#include "Main.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModuleUnloaderClass *ModuleUnloaderClass;

UnicodeString selectedModule;
//---------------------------------------------------------------------------
__fastcall TModuleUnloaderClass::TModuleUnloaderClass(TComponent* Owner)
	: TForm(Owner)
{
 int Transparency = 50;
long ExtStyle = GetWindowLong(Handle, GWL_EXSTYLE);
SetWindowLong(Handle, GWL_EXSTYLE, ExtStyle | WS_EX_LAYERED);
::SetLayeredWindowAttributes(Handle, 0 , (255 * Transparency) / 55,
LWA_ALPHA); // прозрачность
}
//---------------------------------------------------------------------------
void __fastcall TModuleUnloaderClass::FormShow(TObject *Sender)
{
    btnLoadModules->Click();
}
//---------------------------------------------------------------------------

void __fastcall TModuleUnloaderClass::btnLoadModulesClick(TObject *Sender)
{
	ListProcessModules( cppInjectorClass->dwProcId );
}
//---------------------------------------------------------------------------
void __fastcall TModuleUnloaderClass::btnUnloadModuleClick(TObject *Sender)
{
	if( selectedModule == "")MessageBox(NULL, "Нет выбранного модуля "," Предупреждение ", MB_ICONWARNING);
	int scMsgBx = MessageBox(NULL, AnsiString("Вы уверены, что хотите выгрузить " + selectedModule).c_str(), "Confirm", MB_ICONQUESTION | MB_YESNO);
	if( scMsgBx == ID_YES )UnloadModuleByName( "Выбранных модулей", cppInjectorClass->dwProcId );
}
//---------------------------------------------------------------------------
void __fastcall TModuleUnloaderClass::ListView1Click(TObject *Sender)
{
	selectedModule = ListView1->Selected->Caption;
}
//---------------------------------------------------------------------------
