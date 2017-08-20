//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <winsock2.h>
#include<DateUtils.hpp>

#include "Main.h"
#include "ModuleUnloader.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TcppInjectorClass *cppInjectorClass;

UnicodeString Dll;
HANDLE hWatcherThread;  //clBtnFace
int nReason;
//---------------------------------------------------------------------------

__fastcall TcppInjectorClass::TcppInjectorClass(TComponent* Owner)
	: TForm(Owner)
{
int Transparency = 50;
long ExtStyle = GetWindowLong(Handle, GWL_EXSTYLE);
SetWindowLong(Handle, GWL_EXSTYLE, ExtStyle | WS_EX_LAYERED);
::SetLayeredWindowAttributes(Handle, 0 , (255 * Transparency) / 55,
LWA_ALPHA); // прозрачность
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::FormShow(TObject *Sender)
{
	EnablePriv("SeDebugPrivilege");
	btnRefresh->Click();
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::btnInjectClick(TObject *Sender)
{
	if( Dll == "" )
	{
		MessageBox(NULL, "Нет Модуль Selected "," Предупреждение", MB_ICONWARNING);
		return;
	}

	if( dwProcId == 0 )
	{
		MessageBox(NULL, "Нет Процесс Выбранный "," Предупреждение", MB_ICONWARNING);
		return;
	}

   	if( ModuleExists( dwProcId, AnsiString(eModule->Text).c_str() ) == TRUE )
	{
	   // ShowMessage( AnsiString(eModule->Text).c_str() );
		ModuleUnloaderClass->UnloadModuleByName( AnsiString(eModule->Text).c_str(), dwProcId );
    }

    InjectModule( dwProcId, AnsiString(Dll).c_str() );
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::btnSelectModuleClick(TObject *Sender)
{
	if( OpenDialog1->Execute() )
	__try
	{

		Dll = OpenDialog1->FileName;
	}
	__finally
	{
		eModule->Text = ExtractFileName(Dll);
		eModule->Hint = Dll;
    }
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::pListClick(TObject *Sender)
{
	try
	{
		dwProcId = pList->Selected->SubItems->CommaText.ToInt();
		PrintMemoryInfo( dwProcId );
	}
	catch(EAccessViolation &AccessViolation)
	{
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::btnRefreshClick(TObject *Sender)
{
	PROCESSENTRY32 lppe;
	TListItem *ListItem;

	lppe.dwSize = sizeof(lppe);
	HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hSnap == INVALID_HANDLE_VALUE )
	{
		MessageBox( Application->Handle, "Ошибка @ Создание снимка", "Ошибка", MB_ICONERROR );
		return;
    }

	if( pList->Items->Count > 0 )pList->Items->Clear();

	if( Process32First( hSnap, &lppe ) )
	{
		do
		{
			if( lppe.th32ProcessID != 0 && lppe.th32ProcessID != 4 ) // Hide system critical processes
			{
				ListItem = pList->Items->Add();
				ListItem->Caption = lppe.szExeFile;
				ListItem->SubItems->Add( IntToStr( (int)lppe.th32ProcessID ) );
            }
		}
		while(Process32Next(hSnap, &lppe));
	}

	if( hSnap != INVALID_HANDLE_VALUE)CloseHandle( hSnap );

	pCount->Caption = "Запущенные процессы: " + IntToStr( pList->Items->Count );
}
//---------------------------------------------------------------------------

DWORD WINAPI hWatcher_Thread(Pointer Ptr)
{
	PROCESSENTRY32 lppe;
	HANDLE hSnapShot;
	DWORD dwPid = 0;

	lppe.dwSize = sizeof( lppe );

	do
	{
		hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		Process32First( hSnapShot, &lppe );
		do
		{
			if( wcscmp( UpperCase(lppe.szExeFile).c_str() , UpperCase(cppInjectorClass->eProcess->Text).c_str() ) == 0 )
			{
				dwPid = lppe.th32ProcessID;
				break;
			}
		}
		while( Process32Next( hSnapShot, &lppe ) );

		CloseHandle(hSnapShot);

		Sleep(100);
	}
	while(dwPid == 0);

	if(hSnapShot != INVALID_HANDLE_VALUE)CloseHandle(hSnapShot);
	cppInjectorClass->InjectModule(dwPid, AnsiString(Dll).c_str());
    nReason = 0;
	cppInjectorClass->cbWatcher->Checked = false;

	return 0;
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::cbWatcherClick(TObject *Sender)
{
switch(cbWatcher->Checked)
{
	case true:
	if(eProcess->Text == "")
	{
		MessageBox(NULL, "Нет Процесс включал в себя "," Предупреждение", MB_ICONWARNING);
		cbWatcher->Checked = false;
	}
	else if(eProcess->Text != "")
	{
		if(eModule->Text == "")
		{
			MessageBox(NULL, "Нет Модуль Selected "," Предупреждение", MB_ICONWARNING);
			cbWatcher->Checked = false;
		}
		else if(eModule->Text != "")
		{
		    nReason = 1;
			hWatcherThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hWatcher_Thread, NULL, 0, NULL);
		}
	}
	break;
	case false:
	switch(nReason)
	{
		case 1:
		if(hWatcherThread != INVALID_HANDLE_VALUE)
		{
			TerminateThread(hWatcherThread, 0);
			CloseHandle(hWatcherThread);
		}
		break;
		case 0:break;
	}
	break;
}
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::InjectTo1Click(TObject *Sender)
{
    btnInject->Click();
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::PopupMenu1Popup(TObject *Sender)
{
	try
	{
	    //Simulate A Left Mouse Click (To Store The Name Of The Selected Process In P)
		SendMessage( pList->Handle, WM_LBUTTONDOWN, 0, 0 );
		SendMessage( pList->Handle, WM_LBUTTONUP, 0, 0 );

		//Rename The Caption
		InjectTo1->Caption = "Вводите в [" + pList->Selected->Caption + "]";
		EndProcess1->Caption = "Завершить процесс [" + pList->Selected->Caption + "]";
	}
	catch(EAccessViolation &AccessViolation)
	{
		return;
    }
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::EndProcess1Click(TObject *Sender)
{
	TerminateProcess( OpenProcess( PROCESS_TERMINATE, False, dwProcId ), 0 );
}
//---------------------------------------------------------------------------


void __fastcall TcppInjectorClass::BitBtn1Click(TObject *Sender)
{
 ModuleUnloaderClass->Show();
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::BitBtn2Click(TObject *Sender)
{
 cppAboutClass->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::FormActivate(TObject *Sender)
{
 DWORD dwTimerHi, dwTimerLo;
  double dRes;
  asm
   {
      DW 0x310F
      mov dwTimerLo, EAX
      mov dwTimerHi, EDX
    }

  Sleep (500);
  asm
   {
      DW 0x310F
      sub EAX, dwTimerLo
      sub EAX, dwTimerHi
      mov dwTimerLo, EAX
      mov dwTimerHi, EDX
   }
  dRes = dwTimerLo/(1000.0*500);

  AnsiString tickcpu="Тактовая частота: ";
  tickcpu+=dRes;
  tickcpu+=" Mhz";


  Kros->Caption = tickcpu;
  char name[MAX_PATH];
DWORD size = MAX_PATH;
GetUserName(name, &size);
Proces->Caption = AnsiString(name);
}
//---------------------------------------------------------------------------

void __fastcall TcppInjectorClass::Timer1Timer(TObject *Sender)
{

WORD wVersionRequested;
 WSADATA WSAData;
 wVersionRequested = MAKEWORD(1,1);
 WSAStartup(wVersionRequested,&WSAData);
 hostent *P;
 char s[128];
 in_addr in;
 char *P2;
 gethostname(s, 128);
 P = gethostbyname(s);
 in.S_un.S_un_b.s_b1 = P->h_addr_list[0][0];
 in.S_un.S_un_b.s_b2 = P->h_addr_list[0][1];
 in.S_un.S_un_b.s_b3 = P->h_addr_list[0][2];
 in.S_un.S_un_b.s_b4 = P->h_addr_list[0][3];
 P2 = inet_ntoa(in);
 WSACleanup();
 Klient -> Caption = P->h_name;
 Admin -> Caption = P2;

 int x=DayOf(Now());
Soket->Caption=TimeToStr(Now());
Datayi->Caption=DateToStr(Now());

}
//---------------------------------------------------------------------------

