
program params(input, output);
var j: integer;

procedure addparam(var i: integer);
begin
    i:=i+1
end; { of addparam }

begin
    j:=1;
    addparam (j);
    addparam (j)
end.
