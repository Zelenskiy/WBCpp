unit Unit1;

{$mode objfpc}{$H+}


interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, Buttons,
  ExtCtrls, StdCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    fullButton: TSpeedButton;
    Timer3: TTimer;
    Timer4: TTimer;
    windButton: TSpeedButton;
    Timer1: TTimer;
    Timer2: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);

    procedure SpeedButton2Click(Sender: TObject);
    procedure fullButtonClick(Sender: TObject);

    procedure Timer3Timer(Sender: TObject);
    procedure Timer4Timer(Sender: TObject);
    procedure windButtonClick(Sender: TObject);
    procedure Timer2Timer(Sender: TObject);
    procedure SetShotInWindow;
  private

  FPressed : Boolean;
    FPosX, FPosY : Integer;
    FSizeCaption: Integer;
  public
    startDrag: boolean;
    X0, Y0: integer;
    os:string;
    x:string;
    path:string;

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}


{ TForm1 }
uses LCLType, LCLIntf    ;

var apDir :string;
  var c:string;

procedure ResizeBitmap(var AImage: TBitmap; NewWidth, NewHeight: word);
var
  b1: TBitmap;
  b2: TBitmap;

begin
  try
    b1 := TBitmap.Create;
    b2 := TBitmap.Create;

    b1.SetSize(AImage.Width, AImage.Height);
    b1.Assign(AImage);

    b2.SetSize(AImage.Width, AImage.Height);
    b2.Canvas.FillRect(b2.Canvas.ClipRect);
    b2.Canvas.StretchDraw(Rect(0, 0, NewWidth, NewHeight), b1);

    AImage.Width := NewWidth;
    AImage.Height := NewHeight;

    AImage.Canvas.Clear;
    AImage.Canvas.Draw(0, 0, b2);

  finally
    FreeAndNil(b1);
    FreeAndNil(b2);
  end;
end;


procedure TForm1.SetShotInWindow;
var tmpBitmap:TBitmap;
    tmpHeight,tmpWidth:integer;
    ScreenDC: HDC;
      dirSeparator:char;
      namefile,s:string;
      w,h,i,j:integer;
      ir,ig,ib:byte;
      f:text;
      Pix: TColor;
      k:real;
      listF:TStringList;
begin
  //ShowMessage(path);
  if Form1.x='win' then
     namefile:= Form1.path+apDir+'tmp.txt'
  else
     namefile:= Form1.path+apDir+'tmp_f.txt';

  ScreenDC := GetDC(0);
  listF:=TStringList.Create;
  tmpBitmap := TBitmap.Create;
  tmpBitmap.LoadFromDevice(ScreenDC);
  w := tmpBitmap.Width;
  h:= tmpBitmap.Height;
  k:=w/h;
  w := 600;
  h := round(w/k);

  if Form1.x='win' then
     ResizeBitmap(tmpBitmap,w,h);
  w := tmpBitmap.Width;
  h:= tmpBitmap.Height;
  tmpBitmap.SaveToFile(Form1.path+apDir+'file.bmp');
  {
  listF.Add(IntToStr(w));
  listF.Add(IntToStr(h));
  s:='';
  for i:= 0 to h-1 do begin
       for j:=0 to w-1 do begin
            Pix := tmpBitmap.Canvas.Pixels[j,i];
            s:=s+  IntToStr(i) + ' ' +  IntToStr(j) + ' ' + IntToStr(Pix) +' ';
       end;
     end;
  listF.Add(s);
  listF.SaveToFile(namefile);
  }
  tmpBitmap.Free;
  listF.Free;
  Timer3.Enabled:=True;
end;

procedure TForm1.fullButtonClick(Sender: TObject);
begin
  Form1.Visible:=False;
  Timer3.Enabled:=False;
  Form1.x := 'full';

  Timer2.Enabled:=true;

end;



procedure TForm1.Timer3Timer(Sender: TObject);
var   file_name:string;
begin
  file_name := Form1.path+apDir+'flag.txt';
  if FileExists(file_name) then begin
     Form1.Visible:=True;
  end
  else begin
       //TODO
     Form1.Visible:=False
  end;


end;

procedure TForm1.Timer4Timer(Sender: TObject);
var   file_name:string;
begin
    file_name := Form1.path+apDir+'is_work.txt';
  if not FileExists(file_name) then begin
     //TODO

     Application.Terminate;
  end;
end;

procedure TForm1.windButtonClick(Sender: TObject);
begin
  Form1.Visible:=False;
  Timer3.Enabled:=False;
  Form1.x := 'win';
  Timer2.Enabled:=true;

end;

procedure TForm1.Timer2Timer(Sender: TObject);
begin
    SetShotInWindow;
    Timer2.Enabled:=false;
end;

procedure TForm1.FormCreate(Sender: TObject);
var s:string;
    i,j: integer;
begin
  os:='windows';
  {$IFDEF linux}
  os:='linux';
  {$ENDIF}
  s:= ExtractFilePath(Application.ExeName);
  if os =  'windows' then c := '\' else c := '/';
  apDir := 'cmake-build-debug' + c;
  j := length(s);
  for i:=j-1 downto 1 do begin
      if s[i] = c then begin
         s := copy(s,1,i);
         break;
      end;
  end;
  //ShowMessage(s);
  Form1.path := s;
  startDrag := False;
  form1.FormStyle:=fsSystemStayOnTop;
  form1.WindowState:= wsNormal;
  Form1.Visible:=True;
  width :=38;
  height := 120;
  form1.AlphaBlend:=true;
  form1.AlphaBlendValue:=127;{0-255}
  form1.Left := 30;
  form1.Top := 520;
  Timer3.Enabled:=True;
end;

procedure TForm1.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
 FPressed := True;
 FPosX := X;
 FPosY := Y;
end;

procedure TForm1.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
   if FPressed then
  begin
    Left := Left - FPosX + X;
    Top := Top - FPosY + Y;
  end else begin
    if (abs(x-Width)<10) or (x<10) then  begin
       Cursor:=crSizeWE;
    end
    else begin
         Cursor:=crDefault;
    end;
  end;
end;

procedure TForm1.FormMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  FPressed := False;
end;



procedure TForm1.SpeedButton2Click(Sender: TObject);
begin
  Close;
end;



end.

