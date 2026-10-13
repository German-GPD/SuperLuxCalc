program SuperLuxCalc;
uses SysUtils, Math;

var
  a, b: Double;
  cmd: Integer;

begin
  Writeln('SuperLuxCalc Pascal CLI');
  Writeln('1:+  2:-  3:*  4:/  5:^  6:sqrt  7:sin  8:cos  9:tan  0:exit');

  while True do
  begin
    Write('Choose: ');
    Readln(cmd);

    if cmd = 0 then
      Break;

    if (cmd >= 1) and (cmd <= 5) then
    begin
      Write('a b: ');
      Readln(a, b);
      case cmd of
        1: Writeln('= ', a + b:0:10);
        2: Writeln('= ', a - b:0:10);
        3: Writeln('= ', a * b:0:10);
        4:
          if b = 0 then
            Writeln('Ошибка: деление на ноль')
          else
            Writeln('= ', a / b:0:10);
        5: Writeln('= ', Power(a, b):0:10);
      end;
    end
    else if cmd = 6 then
    begin
      Write('a: ');
      Readln(a);
      Writeln('= ', Sqrt(a):0:10);
    end
    else if (cmd >= 7) and (cmd <= 9) then
    begin
      Write('a (radians): ');
      Readln(a);
      case cmd of
        7: Writeln('= ', Sin(a):0:10);
        8: Writeln('= ', Cos(a):0:10);
        9: Writeln('= ', Tan(a):0:10);
      end;
    end
    else
      Writeln('Неизвестная операция');
  end;
end.
