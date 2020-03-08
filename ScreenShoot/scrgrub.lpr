program scrgrub;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,  Unit1
  { you can add units after this };

//Uses Windows;          {Закоментувати для Linux}
//Var MemHnd : HWND;     {Закоментувати для Linux}

{$R *.res}


begin
  {Закоментувати для Linux
  MemHnd := CreateFileMapping(HWND($FFFFFFFF), nil, PAGE_READWRITE, 0, 10, 'temp');
  if GetLastError=ERROR_ALREADY_EXISTS then Exit;
  -----------------------------}


  Application.Initialize;
  Application.CreateForm(TForm1, Form1);


  Application.Run;
end.

