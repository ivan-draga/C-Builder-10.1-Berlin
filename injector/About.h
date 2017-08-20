//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>

#include "Main.h"
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TcppAboutClass : public TForm
{
__published:	// IDE-managed Components
	TImage *Logo;
	TLabel *Copyright;
	TLabel *Label1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall LogoClick(TObject *Sender);
	void __fastcall CopyrightClick(TObject *Sender);
	void __fastcall Label1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TcppAboutClass(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TcppAboutClass *cppAboutClass;
//---------------------------------------------------------------------------
#endif
