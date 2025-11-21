//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TButton *RUN;
	TEdit *Edit1;
	TEdit *Edit2;
	TRadioButton *Encrypt;
	TRadioButton *Decrypt;
	TEdit *Edit3;
	TStaticText *StaticText1;
	TStaticText *StaticText2;
	TStaticText *StaticText3;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	void __fastcall RUNClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
