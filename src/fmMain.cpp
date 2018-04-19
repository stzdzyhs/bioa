//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm10 *Form10;
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm10::ToolButton6Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 5;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::FormCreate(TObject *Sender)
{
	for(int i=0;i<pcWorkArea->PageCount;i++) {
		pcWorkArea->Pages[i]->TabVisible=false;
	}
	pcWorkArea->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::ToolButton1Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::ToolButton2Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::ToolButton3Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 2;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::ToolButton4Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 3;
}
//---------------------------------------------------------------------------

void __fastcall TForm10::ToolButton5Click(TObject *Sender)
{
	pcWorkArea->ActivePageIndex = 4;
}
//---------------------------------------------------------------------------
