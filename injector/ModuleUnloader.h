//---------------------------------------------------------------------------

#ifndef ModuleUnloaderH
#define ModuleUnloaderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "Main.h"
//---------------------------------------------------------------------------
class TModuleUnloaderClass : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *cbModules;
	TListView *ListView1;
	TButton *btnLoadModules;
	TLabel *lbLoadedModules;
	TButton *btnUnloadModule;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnLoadModulesClick(TObject *Sender);
	void __fastcall btnUnloadModuleClick(TObject *Sender);
	void __fastcall ListView1Click(TObject *Sender);
private:	// User declarations
	BOOL __fastcall ListProcessModules( DWORD dwProcessId );
public:		// User declarations
	__fastcall TModuleUnloaderClass(TComponent* Owner);
	void __fastcall UnloadModuleByName(UnicodeString ModuleName, DWORD dwProcessId);
};

//---------------------------------------------------------------------------
extern PACKAGE TModuleUnloaderClass *ModuleUnloaderClass;

BOOL __fastcall TModuleUnloaderClass::ListProcessModules( DWORD dwProcessId )
{
  HANDLE hModuleSnap;
  MODULEENTRY32 me32;
  TListItem *ListItem;

  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
	ShowMessage( "Ошибка @ Создать инструмент помогают 32 снимков (модулей)" );
    return( FALSE );
  }

  me32.dwSize = sizeof( MODULEENTRY32 );

  if( !Module32First( hModuleSnap, &me32 ) )
  {
	ShowMessage( "Ошибка @ Модуль 32 Год" );
	CloseHandle( hModuleSnap );

    return( FALSE );
  }

	if( ListView1->Items->Count > 0 )ListView1->Items->Clear();

  do
  {
	   ListItem = ListView1->Items->Add();
	   ListItem->Caption = me32.szModule; //Module Name
	   ListItem->SubItems->Add( "0x" + IntToHex( (int)me32.modBaseAddr, 1 ) ); //Module Base Addr
  }
  while( Module32Next( hModuleSnap, &me32 ) );


  lbLoadedModules->Caption = "Загруженные модули (" + IntToStr( ListView1->Items->Count ) + ")"; //+ cppInjectorClass->P
  CloseHandle( hModuleSnap );
  return( TRUE );
}

void __fastcall TModuleUnloaderClass::UnloadModuleByName(UnicodeString ModuleName, DWORD dwProcessId)
{
	HANDLE hTempModuleSnap;
	MODULEENTRY32 me32;

	me32.dwSize = sizeof( me32 );
	hTempModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId );

	if( Module32First( hTempModuleSnap, &me32 ) )
	{
		do
		{
			if( me32.szModule == ModuleName )
			{
				CreateRemoteThread( OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessId ), NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandle("Kernel32.dll"), "FreeLibrary" ), me32.hModule, 0, NULL);
			}
		}
		while( Module32Next( hTempModuleSnap, &me32 ) );
	}

   	CloseHandle( hTempModuleSnap );
}
//---------------------------------------------------------------------------
#endif
