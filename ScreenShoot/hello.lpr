program hello;
{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads ,
  {$ENDIF}{$ENDIF}
  Clipbrd ;

begin
  writeln('Hello');
end.

