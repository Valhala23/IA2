//---------------------------------------------------------------------------

#ifndef URnaH
#define URnaH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------

class TFmRna : public TForm
{
	__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TChart *Chart1;
	TFastLineSeries *Series1;
	TButton *Button1;
	TButton *Button2;
	TTabSheet *TabSheet2;
	TListBox *ListBox1;
	TChart *Chart2;
	TFastLineSeries *Series2;
	TMemo *Memo1;
	TShape *Shape1;
	TShape *Shape2;
	TShape *Shape3;
	TShape *Shape4;
	TShape *Shape5;
	TShape *Shape6;
	TShape *Shape7;
	TShape *Shape8;
	TShape *Shape9;
	TShape *Shape10;
	TShape *Shape11;
	TShape *Shape12;
	TShape *Shape13;
	TShape *Shape14;
	TShape *Shape15;
	TShape *Shape16;
	TShape *Shape17;
	TShape *Shape18;
	TShape *Shape19;
	TShape *Shape20;
	TLabel *Label1;
	TLabel *Label2;
		void __fastcall Button1Click(TObject *Sender);
		void __fastcall Button2Click(TObject *Sender);
        void __fastcall AtualizaGrafico();
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);


	private:	// User declarations
	public:		// User declarations
		__fastcall TFmRna(TComponent* Owner);
};

//---------------------------------------------------------------------------

class Thread : public TThread
{
	private:
	protected:
		void __fastcall Execute();

	public:
		__fastcall Thread(bool CreateSuspended);
};

//---------------------------------------------------------------------------
extern PACKAGE TFmRna *FmRna;
//---------------------------------------------------------------------------
#endif
