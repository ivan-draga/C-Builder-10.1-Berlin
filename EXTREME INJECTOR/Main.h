//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>
#include <Menus.hpp>
#include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Buttons.hpp>
#pragma comment(lib, "Psapi.lib")

#include "About.h"
#include "ModuleUnloader.h"
//---------------------------------------------------------------------------
class TcppInjectorClass : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog1;
	TPanel *MainPanel;
	TGroupBox *gbProcessWatcher;
	TLabel *Label1;
	TCheckBox *cbWatcher;
	TEdit *eProcess;
	TGroupBox *gbProcesses;
	TLabel *pCount;
	TListView *pList;
	TButton *btnRefresh;
	TGroupBox *gbLoadDll;
	TButton *btnInject;
	TButton *btnSelectModule;
	TEdit *eModule;
	TGroupBox *gbExInfo;
	TListView *ExInfo;
	TPopupMenu *PopupMenu1;
	TMenuItem *InjectTo1;
	TMenuItem *EndProcess1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Proces;
	TLabel *Kros;
	TLabel *Klient;
	TLabel *Admin;
	TTimer *Timer1;
	TLabel *Datayi;
	TLabel *Soket;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnInjectClick(TObject *Sender);
	void __fastcall btnSelectModuleClick(TObject *Sender);
	void __fastcall pListClick(TObject *Sender);
	void __fastcall btnRefreshClick(TObject *Sender);
	void __fastcall cbWatcherClick(TObject *Sender);
	void __fastcall InjectTo1Click(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall EndProcess1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	BOOL __fastcall EnablePriv(LPCSTR lpszPriv); // by Napalm
	BOOL __fastcall ModuleExists( DWORD dwProcessId, LPCSTR lpszModule );
	void __fastcall PrintMemoryInfo( DWORD processID );
public:		// User declarations
	__fastcall TcppInjectorClass(TComponent* Owner);
	void __fastcall InjectModule(DWORD dwProcessId, LPCSTR lpszModule);
    DWORD dwProcId;
};
//---------------------------------------------------------------------------
extern PACKAGE TcppInjectorClass *cppInjectorClass;

void __fastcall TcppInjectorClass::InjectModule( DWORD dwProcessId, LPCSTR lpszModule )
{
	SIZE_T BytesWritten;
	HANDLE hRemoteThread = NULL;

	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessId );
	if( hProcess != INVALID_HANDLE_VALUE )
	{
		LPVOID MemRegion = VirtualAllocEx( hProcess, NULL, strlen(lpszModule), MEM_COMMIT, PAGE_READWRITE );
		if(MemRegion != NULL)
		__try
		{
			WriteProcessMemory( hProcess, MemRegion, (LPCVOID)lpszModule, strlen( lpszModule ), &BytesWritten );
			hRemoteThread = CreateRemoteThread( hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress( GetModuleHandle("kernel32.dll"), "LoadLibraryA" ), MemRegion, 0, NULL );
			WaitForSingleObject( hRemoteThread, INFINITE );
		}
		__finally
		{
			CloseHandle( hRemoteThread );
			VirtualFreeEx( hProcess, MemRegion, 0, MEM_RELEASE );
			CloseHandle( hProcess );
		}
		else
		{
			MessageBox( NULL, AnsiString("Ошибка @ Виртуальный AllocEx " + IntToStr( (int)GetLastError() )).c_str(), "Error", MB_ICONERROR );
			return;
        }
	}
	else
	{
	   MessageBox( NULL, AnsiString("Ошибка @ открытого процесса " + IntToStr( (int)GetLastError() )).c_str(), "Error", MB_ICONERROR );
	   return;
	}
}


BOOL __fastcall TcppInjectorClass::EnablePriv( LPCSTR lpszPriv ) // by Napalm
{
    HANDLE hToken;
    LUID luid;
	TOKEN_PRIVILEGES tkprivs;

    ZeroMemory( &tkprivs, sizeof(tkprivs) );

	if( !OpenProcessToken( GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken ) )
        return FALSE;

	if( !LookupPrivilegeValueA( NULL, lpszPriv, &luid ) )
	{
		CloseHandle(hToken);
		return FALSE;
    }

    tkprivs.PrivilegeCount = 1;
    tkprivs.Privileges[0].Luid = luid;
    tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
	CloseHandle( hToken );
    return bRet;
}

void __fastcall TcppInjectorClass::PrintMemoryInfo( DWORD processID )
{
	HANDLE hProcess;
	TListItem *ExInfoItem;
	PROCESS_MEMORY_COUNTERS pmc;

	if(ExInfo->Items->Count > 0)ExInfo->Items->Clear();

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		ExInfoItem = ExInfo->Items->Add();
		ExInfoItem->Caption = IntToStr( (int)pmc.WorkingSetSize / 1024 ) + " K";
		ExInfoItem->SubItems->Add( IntToStr( (int)pmc.PageFaultCount ) + " K" );
	}

    CloseHandle( hProcess );
}

BOOL __fastcall TcppInjectorClass::ModuleExists( DWORD dwProcessId, LPCSTR lpszModule )
{
	MODULEENTRY32 lpme;
	HANDLE hSnapShot;
	BOOL bRET;

	lpme.dwSize = sizeof( lpme );

	hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwProcessId );

	if( Module32First( hSnapShot, &lpme ) )
	{
		do
		{
			if( UpperCase(lpme.szModule) == UpperCase(lpszModule) )
			{
				bRET = TRUE;
			}
		}
		while( Module32Next( hSnapShot, &lpme ) );
	}

	CloseHandle( hSnapShot );

	return bRET;
}
//---------------------------------------------------------------------------
#endif
