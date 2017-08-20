//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include "Main.h"
#include "ModuleUnloader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TcppAboutClass *cppAboutClass;
//---------------------------------------------------------------------------
__fastcall TcppAboutClass::TcppAboutClass(TComponent* Owner)
	: TForm(Owner)
{
int Transparency = 50;
long ExtStyle = GetWindowLong(Handle, GWL_EXSTYLE);
SetWindowLong(Handle, GWL_EXSTYLE, ExtStyle | WS_EX_LAYERED);
::SetLayeredWindowAttributes(Handle, 0 , (255 * Transparency) / 55,
LWA_ALPHA); // прозрачность
}
//---------------------------------------------------------------------------
void __fastcall TcppAboutClass::FormShow(TObject *Sender)
{
   	cppAboutClass->Caption = "      Изгатовитель   Skype: Ivan-Draga2";
}
//---------------------------------------------------------------------------
void __fastcall TcppAboutClass::LogoClick(TObject *Sender)
{
	MessageBox(NULL, "Ivan-draga2", "Skype", MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TcppAboutClass::CopyrightClick(TObject *Sender)
{
 AnsiString doc = "mailto:" + Copyright->Caption + "?Subject=Моя_тема";
ShellExecute(NULL,"open",doc.c_str(),NULL,NULL,SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall TcppAboutClass::Label1Click(TObject *Sender)
{
 AnsiString doc = "http://as42959-91-193-253-153.mol.net.ua/";
ShellExecute(NULL,"open",doc.c_str(),NULL,NULL,SW_RESTORE);
}
//---------------------------------------------------------------------------

