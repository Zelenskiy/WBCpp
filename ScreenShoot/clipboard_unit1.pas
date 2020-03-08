unit clipboard_unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls,
  Graphics, Dialogs, StdCtrls, Clipbrd, LCLIntf,
  LCLType, ExtCtrls;

type

  { TClipboard_Form1 }

  TClipboard_Form1 = class(TForm)
    Timer1: TTimer;

    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private

  public

  end;

var
  Clipboard_Form1: TClipboard_Form1;

implementation

{$R *.lfm}

{ TClipboard_Form1 }






procedure TClipboard_Form1.Timer1Timer(Sender: TObject);
var clbBitmap:TBitmap;
  path,s,os,c:string;
  i,j: integer;
  file_name:string;
begin
  visible := False;
     os:='windows';
  {$IFDEF linux}
  os:='linux';
  {$ENDIF}
  s:= ExtractFilePath(Application.ExeName);
  if os =  'windows' then c := '\' else c := '/';
  j := length(s);
  for i:=j-1 downto 1 do begin
      if s[i] = c then begin
         s := copy(s,1,i);
         break;
      end;
  end;

     file_name := 'image.bmp';
     clbBitmap:=TBitmap.Create;
     if Clipboard.HasFormat(CF_PICTURE) or Clipboard.HasFormat(CF_BITMAP)  then begin
          clbBitmap .Assign(Clipboard) ;
          if not FileExists(s+file_name) then begin
              clbBitmap.SaveToFile(s+file_name);
              clbBitmap.Free;
              Clipboard.AsText:='Hello';

          end;
     end;

     file_name := 'is_work.txt';
     if not FileExists(s+file_name) then begin
        Application.Terminate;
     end;
     //Application.Terminate;

end;

procedure TClipboard_Form1.FormCreate(Sender: TObject);
begin
  Clipboard_Form1.Visible:=False;
end;



end.

