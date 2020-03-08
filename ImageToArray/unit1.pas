unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  ExtCtrls, ExtDlgs, Buttons,  LCLType, LCLIntf , StrUtils;


type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Image1: TImage;
    Memo1: TMemo;
    OpenPictureDialog1: TOpenPictureDialog;
    SelectDirectoryDialog1: TSelectDirectoryDialog;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

const sep = '/';
var  dir:string;


function JpegToBitmap(jpeg: string):TBitmap;
var
jpg:TJpegImage;
bmp:TBitmap;
begin
   bmp := TBitmap.Create;
   jpg:=TJpegImage.Create;
   try
   jpg.LoadFromFile(jpeg);
   Bmp.Height      := jpg.Height;
   Bmp.Width       := jpg.Width;
   Bmp.PixelFormat := pf24bit;
   Bmp.Canvas.Draw(0, 0, jpg);
   bmp.Assign(jpg);
   result:=bmp;
   finally
     jpg.Free;
     bmp.Free;
   end;
end;
function PngToBitmap(png: string):TBitmap;
var
  bmp: TBitmap;
  pic: TPortableNetworkGraphic;
begin
   bmp := TBitmap.Create;
   try
   pic.LoadFromFile(png);
   bmp.Assign(pic);
   result:=bmp;
   finally
     bmp.Free;
   end;
end;

Function RPos(Const s:String;c:Char):Integer;
Var
 i:Integer;
Begin
 For i:=Length(s) Downto 1 Do
  If s[i]=c Then
   Begin
    Result:=i;
    Exit;
   End;
 Result:=0;
End;


procedure Generator(name:string);

var fileName, s:string;
  bmp:TBitmap;
  w,h,i,j:integer;
  DC:HDC;
  hWnd: THandle;
  r,g,b,ss,sss:string;
  ir,ig,ib:byte;
  Pix: TColor;
begin
  bmp:=TBitmap.Create;
     Form1.Image1.Picture.LoadFromFile(name);
     bmp.Assign(Form1.Image1.Picture.Bitmap);
     w := bmp.Width;
     h := bmp.Height;
     i := RPos(name,sep);
     j := RPos(name,'.');
     sss := copy(name, i+1,j-i-1);
     ss := 'const float '+sss+' ['+IntToStr(w)+']['+IntToStr(h)+']['+IntToStr(3)+']  = {';
     s:='';
     for i:= 1 to h do begin
       s:=s+'{';
       for j:=1 to w do begin
            Pix := bmp.Canvas.Pixels[i,j];
            ir     := Pix;
            ig     := Pix shr 8;
            ib     := Pix shr 16;
            R:=FloatToStrF(ir/256,fffixed,6,2);
            G:=FloatToStrF(ig/256,fffixed,6,2);
            B:=FloatToStrF(ib/256,fffixed,6,2);
            s:=S + '{'+R+', '+G+', '+B+'}';
            if j<>w then s:=s+',';
       end;
       s := s+'} ';
     end;
     s := s +   '};';
     s:=ReplaceText(s, ' ', '');
     s:=ReplaceText(s, '}{', '}, {');
     Form1.Memo1.Text :=Form1.Memo1.Text  + #13#13 + ss+s;
     //Form1.Memo1.Lines.SaveToFile(dir+sep+sss+'.txt');
     //Form1.Memo1.Text :=   '';
  bmp.Free;
end;

procedure TForm1.Button1Click(Sender: TObject);
var fileName:string;

begin
   if Form1.OpenPictureDialog1.Execute then begin
     fileName := Form1.OpenPictureDialog1.FileName;
     Generator(fileName);
   end;
end;

procedure GetAllFiles( Path: string; var Lb: TStringList);
var
  sRec: TSearchRec;
  isFound: boolean;
begin
  isFound := FindFirst( Path + sep+'*.png', faAnyFile, sRec ) = 0;
  while isFound do
    begin
      if ( sRec.Name <> '.' ) and ( sRec.Name <> '..' ) then
      begin
        if ( sRec.Attr and faDirectory ) = faDirectory then
           GetAllFiles( Path + sep+ sRec.Name, Lb );
        Lb.Add( Path + sep+  sRec.Name );
      end;
      Application.ProcessMessages;
      isFound := FindNext( sRec ) = 0;
  end;
  FindClose( sRec );
end;


procedure TForm1.Button2Click(Sender: TObject);
var
    Lb: TStringList;
    i:integer;
begin
    Lb:= TStringList.Create;
    if SelectDirectoryDialog1.Execute then begin
          dir := SelectDirectoryDialog1.FileName;
          GetAllFiles( dir, Lb);
          //Memo1.Lines := (Lb);
          Memo1.Text:='';
          for i:=0 to Lb.Count-1 do begin
                 Generator(Lb[i]);
          end;
          Form1.Memo1.Lines.SaveToFile(dir+sep+'all.txt');

    end;
    Lb.Free;
end;

procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  Close;
end;

end.

