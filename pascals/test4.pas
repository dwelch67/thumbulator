
program params(input, output);
var j,k: integer;

procedure addparam(var i: integer);
begin
    i:=i+1
end; { of addparam }

begin
    j:=3;
    addparam(j);
    addparam(j);
    while j<>0 do begin
        j:=j-1;
    end;
    k:=j
end.
