
{ I (dwelch@dwelch.com) found this code here:
    http://www.cs.hs-rm.de/~weber/comp/pascals3.html
    Commented out the interpret line.  I am interested in the the
    pcode output
    Commented out the hardcoded filename, using filename from command line
}

{ Pascal-S from N Wirth's paper 'Pascal-S: a subset and its implementation'
  which is most easily found in the book 'Pascal: the language and its
  implementation' edited by Barron. You might also like to look at
  'Principles of concurrent programming' by Ben-Ari (the first edition)
  which contains a munged version of Pascal-S that supports some
  concurrency.}

{ This version of Pascal-S was originally fetched from csvax.cs.caltech.edu
  where it lived in directory /jan. I believe that it was set up by Jan van
  de Snepscheut. I don't know anything else about its provenance. I modified
  the program to suit Turbo Pascal version 5.5 as detailed in the next
  comment. Jan's fixes to the published program are detailed in the comment
  after that.

                             Adrian Johnstone, 22 March 1995
                             adrian@dcs.rhbnc.ac.uk
}

{For Turbo Pascal:
   changed string to sstring
   changed halt to hhalt
   changed getch to read from infile instead of stdin and added file assign
   statements to mainline routine.
   removed label 99:
   changed 'goto 99' to halt;
   added chr(10) and chr(13) to list of throw-aways in getsym
}

{ line 295 (counting from 1 starting at program PascalS) is
        gen1(mulc, ttab[t].size); gen0(add)
  whereas the version printed in the book accidentally reads
        gen1(mulc, ttab[t].size)
  the present version also implements boolean negation

  the procedure funcdeclaration in the version printed in the book is
  erroneous. The first line on page 376 in the book should read
        if lev>1 then dx:=-1
  the last line of the procedure should read
        gen1(exit,itab[f].resultadr-dx); lev:=lev-1; dx:=odx
}

program PascalS(infile, output);

const cxmax  = 2000;     { size of code array }
      amax   = 131071;    { maximum address }

type  opcode = (add, neg, mul, divd, remd, div2, rem2, eqli, neqi, lssi,
                leqi, gtri, geqi, dupl, swap, andb, orb,
                load, stor, hhalt, wri, wrc, wrl, rdi, rdc, rdl, eol,
                ldc, ldla, ldl, ldg, stl, stg, move, copy, addc, mulc,
                jump, jumpz, call, adjs, sets, exit);
      instr  = record case op: opcode of
                 add, neg, mul, divd, remd, div2, rem2, eqli, neqi, lssi,
                 leqi, gtri, geqi, dupl, swap, andb, orb,
                 load, stor, hhalt, wri, wrc, wrl, rdi, rdc, rdl, eol:
                        ();
                 ldc, ldla, ldl, ldg, stl, stg, move, copy, addc, mulc,
                 jump, jumpz, call, adjs, sets, exit:
                        (a: integer)
               end;

var code: array [0..cxmax] of instr;
    m   : array [0..amax] of integer;
    infile: text;

procedure compile;

const imax   = 100;     { length of identifier table }
      tmax   = 100;     { length of type table }
      lmax   = 10;      { maximum level }
      al     = 10;      { length of identifiers }
      fabs   = 0;       { standard functions }
      fsqr   = 1; fodd   = 2; fchr    = 3;
      ford   = 4; fwrite = 5; fwriteln= 6;
      fread  = 7; freadln= 8; feoln   = 9;
      { standard types }
      intip  = 1; booltip= 2; chartip = 3;

type symbol = (ident, number, sstring, plus, minus, star, lbrack, rbrack,
               colon, eql, neq, lss, leq, gtr, geq, lparen, rparen, comma,
               semicolon, period, becomes,
               beginsym, endsym, ifsym, thensym, elsesym, whilesym, dosym,
               casesym, repeatsym, untilsym, forsym, tosym, downtosym,
               notsym, divsym, modsym, andsym, orsym, constsym, varsym,
               typesym, arraysym, ofsym, recordsym, progsym, funcsym,
               procsym);
     idkind = (konst, varbl, field, tipe, funkt);
     tpkind = (simple, arrays, records);
     alfa   = packed array [1..al] of char;

var ch: char;                   { last character read }
    cc: integer;                { character count }
    ll: integer;                { line length }
    line: array [1..81] of char;{ present input line }
    sym: symbol;                { last symbol read }
    id: alfa;                   { last identifier read }
    num: integer;               { last number read }
    str: array [1..80] of char; { last string read }
    slen: integer;              { length of last string }
    word: array [beginsym..procsym] of alfa;
    cx: integer;                { code index }
    lev: integer;               { procedure nesting level }
    dx: integer;                { offset in stack }
    labeled: boolean;           { next instruction has label }
    namelist: array [-1..lmax] of integer;
    ix, tx: integer;            { indices in tables }
    itab: array [0..imax] of    { identifier table }
            record name: alfa; link: integer; tip: integer;
              case kind: idkind of
                konst: (val: integer);
                varbl: (vlevel, vadr: integer; refpar: boolean);
                field: (offset: integer);
                tipe : ();
                funkt: (flevel, fadr, lastpar, resultadr: integer;
                           inside: boolean)
            end;
    ttab: array [1..tmax] of    { type table }
            record size: integer;
              case kind: tpkind of
                simple : ();
                arrays : (low, high, elemtip: integer);
                records: (fields: integer)
            end;

procedure error(n: integer);
  var i: integer;
begin for i:= 1 to ll do write(line[i]); writeln;
      for i:= 1 to cc-2 do write(' '); writeln('^');
      writeln('error ', n:1, ' detected');
      halt; { Turbo Pascal exit routine }
end;

procedure getch;
begin if cc=ll then begin
          if eof(infile) then error(100); ll:= 0; cc:= 0;
          while not eoln(infile) do begin ll:= ll+1; read(infile, line[ll]) end;
          ll:= ll+1; read(infile, line[ll])
      end;
      cc:= cc+1; ch:= line[cc]
end;

procedure getsym;
  var k: integer; s: symbol; strend: boolean;
begin while ch in [' ', chr(9), chr(13), chr(10)] do getch;
      if ch in ['a'..'z', 'A'..'Z'] then begin
          k:= 0;
          repeat if k<>al then begin k:= k+1; id[k]:= ch end;
                 getch
          until not (ch in ['a'..'z', 'A'..'Z', '0'..'9']);
          while k<>al do begin k:= k+1; id[k]:= ' ' end;
          sym:= ident;
          for s:= beginsym to procsym do if word[s]=id then sym:= s
      end else if ch in ['0'..'9'] then begin
          num:= 0; sym:= number;
          repeat num:= num*10 + (ord(ch)-ord('0'));
                 getch
          until not (ch in ['0'..'9'])
      end else if ch=':' then begin
          getch;
          if ch='=' then begin getch; sym:= becomes end
                    else sym:= colon
      end else if ch='>' then begin
          getch;
          if ch='=' then begin getch; sym:= geq end
                    else sym:= gtr
      end else if ch='<' then begin
          getch;
          if ch='=' then begin getch; sym:= leq end else
          if ch='>' then begin getch; sym:= neq end
                    else sym:= lss
      end else if ch='.' then begin
          getch;
          if ch='.' then begin getch; sym:= colon end
                    else sym:= period
      end else if ch='''' then begin
          slen:= 0; strend:= false; sym:= sstring;
          repeat if cc=ll then error(101); getch;
                 if ch='''' then begin
                     getch;
                     if ch='''' then begin
                         slen:= slen+1; str[slen]:= ch
                     end else
                         strend:= true
                 end else begin
                     slen:= slen+1; str[slen]:= ch
                 end
          until strend;
          if slen=0 then error(102)
      end
      else if ch='+' then begin getch; sym:= plus end
      else if ch='-' then begin getch; sym:= minus end
      else if ch='*' then begin getch; sym:= star end
      else if ch='(' then begin getch; sym:= lparen end
      else if ch=')' then begin getch; sym:= rparen end
      else if ch='[' then begin getch; sym:= lbrack end
      else if ch=']' then begin getch; sym:= rbrack end
      else if ch='=' then begin getch; sym:= eql end
      else if ch=',' then begin getch; sym:= comma end
      else if ch=';' then begin getch; sym:= semicolon end
      else if ch='{'
      then begin repeat getch until ch='}';
                 getch; getsym
           end
      else error(103)
end;

procedure check(s: symbol);
begin if sym<>s then error(ord(s)) end;

procedure skip(s: symbol);
begin check(s); getsym end;

procedure enter(id: alfa; k: idkind; t: integer);
  var j: integer;
begin if ix=imax then error(104); ix:= ix+1;
      itab[0].name:= id; j:= namelist[lev];
      while itab[j].name<>id do j:= itab[j].link;
      if j<>0 then error(105);
      with itab[ix] do begin
        name:= id; link:= namelist[lev]; tip:= t; kind:= k
      end;
      namelist[lev]:= ix
end;

function position: integer;
  var i, j: integer;
begin itab[0].name:= id; i:= lev;
      repeat j:= namelist[i];
             while itab[j].name<>id do j:= itab[j].link;
             i:= i-1
      until (i<-1) or (j<>0);
      if j=0 then error(106); position:= j
end;

procedure gen(i: instr);
begin case i.op of
        dupl, eol, ldc, ldla, ldl, ldg:
          dx:= dx-1;
        neg, div2, rem2, swap, load, hhalt, wrl, rdl,
        addc, mulc, jump, call, sets, exit:
          ;
        add, mul, divd, remd, eqli, neqi, lssi, leqi, gtri,
        geqi, andb, orb, wrc, rdi, rdc, stl, stg, jumpz:
          dx:= dx+1;
        stor, wri, move:
          dx:= dx+2;
        copy:
          dx:= dx-i.a+1;
        adjs:
          dx:= dx+i.a
      end;
      if not (((i.op in [addc, adjs]) and (i.a=0)) or
              ((i.op=mulc) and (i.a=1))) then
      if labeled then begin
          code[cx]:= i; cx:= cx+1; labeled:= false
      end else if (code[cx-1].op=ldc) and (i.op=add) then
          code[cx-1].op:= addc
      else if (code[cx-1].op=ldc) and (i.op=mul) then
          code[cx-1].op:= mulc
      else if (code[cx-1].op=ldc) and (i.op=neg) then
          code[cx-1].a:= -code[cx-1].a
      else if (code[cx-1].op=ldc) and (code[cx-1].a=2) and (i.op=divd) then
          code[cx-1].op:= div2
      else if (code[cx-1].op=ldc) and (code[cx-1].a=2) and (i.op=remd) then
          code[cx-1].op:= rem2
      else if (code[cx-1].op=ldc) and (i.op=stor) then
          code[cx-1].op:= stg
      else if (code[cx-1].op=ldc) and (i.op=load) then
          code[cx-1].op:= ldg
      else if (code[cx-1].op=ldla) and (i.op=stor) then
          code[cx-1].op:= stl
      else if (code[cx-1].op=ldla) and (i.op=load) then
          code[cx-1].op:= ldl
      else begin
          code[cx]:= i; cx:= cx+1
end   end;

procedure gen0(op: opcode);
  var i: instr;
begin i.op:= op; gen(i) end;

procedure gen1(op: opcode; a: integer);
  var i: instr;
begin i.op:= op; i.a:= a; gen(i) end;

function codelabel: integer;
begin codelabel:= cx; labeled:= true end;

procedure address(lv, ad: integer);
begin if lv=0 then
          gen1(ldc, ad)
      else if lv=lev then
          gen1(ldla, ad-dx)
      else begin
          gen1(ldl, -dx);
          while lv+1<>lev do begin gen0(load); lv:= lv+1 end;
          gen1(addc, ad)
end   end;

procedure addressvar(ref: integer);
begin with itab[ref] do
      begin address(vlevel, vadr); if refpar then gen0(load) end
end;

procedure mustbe(x, y: integer);
begin if x<>y then
      if (ttab[x].kind=arrays) and (ttab[y].kind=arrays) and
         (ttab[x].low=ttab[y].low) and (ttab[x].high=ttab[y].high)
      then mustbe(ttab[x].elemtip, ttab[y].elemtip)
      else error(107)
end;

procedure expression(var x: integer);
  forward;

procedure selector(var t: integer; var ref: integer);
  var j, x: integer;
begin t:= itab[ref].tip; getsym;
      if sym in [period, lbrack] then begin
          addressvar(ref); ref:= 0;
          while sym in [period, lbrack] do
          case sym of
            period : begin if ttab[t].kind<>records then error(108);
                           getsym; check(ident);
                           j:= ttab[t].fields; itab[0].name:= id;
                           while itab[j].name<>id do j:= itab[j].link;
                           if j=0 then error(109);
                           gen1(addc, itab[j].offset);
                           t:= itab[j].tip; getsym
                     end;
            lbrack : begin repeat if ttab[t].kind<>arrays then error(110);
                                  getsym; expression(x); mustbe(intip, x);
                                  gen1(addc, -ttab[t].low);
                                  t:= ttab[t].elemtip;
                                  gen1(mulc, ttab[t].size); gen0(add)
                           until sym<>comma;
                           skip(rbrack)
end   end end        end;

procedure varpar(var t: integer);
  var j: integer;
begin check(ident); j:= position; selector(t, j);
      if j<>0 then addressvar(j)
end;

procedure standfct(n: integer);
  var x, l: integer;
begin case n of
        fabs: begin skip(lparen); expression(x); mustbe(intip, x);
                  gen0(dupl); gen1(ldc, 0); gen0(lssi);
                  l:= codelabel; gen1(jumpz, 0); gen0(neg);
                  code[l].a:= codelabel;
                  skip(rparen)
              end;
        fsqr: begin skip(lparen); expression(x); mustbe(intip, x);
                  gen0(dupl); gen0(mul); skip(rparen)
              end;
        fodd: begin skip(lparen); expression(x); mustbe(intip, x);
                  gen0(rem2); skip(rparen)
              end;
        fchr: begin skip(lparen); expression(x); mustbe(intip, x);
                  skip(rparen)
              end;
        ford: begin skip(lparen); expression(x); mustbe(chartip, x);
                  skip(rparen)
              end;
        fwrite, fwriteln:
              begin if n=fwrite then check(lparen);
                  if sym=lparen then begin
                      repeat getsym;
                          if sym=sstring then begin
                              for x:= 1 to slen do begin
                                  gen1(ldc, ord(str[x]));
                                  gen0(wrc)
                              end;
                              getsym
                          end else begin
                              expression(x);
                              if sym=colon then begin
                                  mustbe(intip, x); getsym;
                                  expression(x); mustbe(intip,x);
                                  gen0(wri)
                              end else if x=intip then begin
                                  gen1(ldc, 8); gen0(wri)
                              end else if x=chartip then
                                  gen0(wrc)
                              else
                                  error(111)
                          end
                      until sym<>comma;
                      skip(rparen)
                  end;
                  if n=fwriteln then gen0(wrl)
              end;
     fread, freadln:
              begin if n=fread then check(lparen);
                  if sym=lparen then begin
                      repeat getsym; varpar(x);
                             if x=intip   then gen0(rdi) else
                             if x=chartip then gen0(rdc)
                                          else error(112)
                      until sym<>comma;
                      skip(rparen)
                  end;
                  if n=freadln then gen0(rdl)
              end;
        feoln: gen0(eol)
end   end;

procedure funcall(i: integer);
  var d, p, x: integer;
begin getsym;
      with itab[i] do
      if flevel<0 then
          standfct(fadr)
      else begin
          if tip<>0 then gen1(ldc, 0); p:= i; d:= dx;
          if sym=lparen then begin
              repeat getsym;
                  if p=lastpar then error(113); p:= p+1;
                  if itab[p].refpar then
                      varpar(x)
                  else begin
                      expression(x);
                      if ttab[x].kind<>simple then gen1(copy, ttab[x].size)
                  end;
                  mustbe(itab[p].tip, x)
              until sym<>comma;
              skip(rparen)
          end;
          if p<>lastpar then error(114);
          if flevel<>0 then address(flevel, 0);
          gen1(call, fadr); dx:= d
end   end;

procedure factor(var t: integer);
   var i: integer;
begin if sym=ident then begin
          i:= position; t:= itab[i].tip;
          case itab[i].kind of
            konst: begin getsym; gen1(ldc, itab[i].val) end;
            varbl: begin selector(t, i);
                         if i<>0 then addressvar(i);
                         if ttab[t].kind=simple then gen0(load)
                   end;
            funkt: if t=0 then error(115) else funcall(i);
            tipe : error(116)
          end
      end else if sym=number then begin
          gen1(ldc, num); t:= intip; getsym
      end else if (sym=sstring) and (slen=1) then begin
          gen1(ldc, ord(str[1])); t:= chartip; getsym
      end else if sym=lparen then begin
          getsym; expression(t); skip(rparen)
      end else if sym=notsym then begin
          getsym; factor(t); mustbe(booltip, t); gen0(neg); gen1(addc, 1)
      end else
          error(117)
end;

procedure term(var x: integer);
  var y: integer;
begin factor(x);
      while sym in [andsym, star, divsym, modsym] do begin
          if sym=andsym then mustbe(booltip, x) else mustbe(intip, x);
          case sym of
            star  : begin getsym; factor(y); gen0(mul) end;
            divsym: begin getsym; factor(y); gen0(divd) end;
            modsym: begin getsym; factor(y); gen0(remd) end;
            andsym: begin getsym; factor(y); gen0(andb) end
          end;
          mustbe(x, y)
end   end;

procedure simplexpression(var x: integer);
  var y: integer;
begin if sym=plus then begin
          getsym; term(x); mustbe(intip, x)
      end else if sym=minus then begin
          getsym; term(x); mustbe(intip, x); gen0(neg)
      end else
          term(x);
      while sym in [orsym, plus, minus] do begin
          if sym=orsym then mustbe(booltip, x) else mustbe(intip, x);
          case sym of
            plus : begin getsym; term(y); gen0(add) end;
            minus: begin getsym; term(y); gen0(neg); gen0(add) end;
            orsym: begin getsym; term(y); gen0(orb) end
          end;
          mustbe(x, y)
end   end;

procedure expression{var x: integer};
  var op: symbol; y: integer;
begin simplexpression(x);
      if sym in [eql, neq, lss, leq, gtr, geq] then begin
          if ttab[x].kind<>simple then error(118);
          op:= sym; getsym; simplexpression(y); mustbe(x, y);
          case op of
            eql: gen0(eqli);
            neq: gen0(neqi);
            lss: gen0(lssi);
            leq: gen0(leqi);
            gtr: gen0(gtri);
            geq: gen0(geqi)
          end;
          x:= booltip
end   end;

procedure statement;
  var i, j, t, x: integer;
begin if sym=ident then begin
          i:= position;
          with itab[i] do
          case kind of
            varbl: begin selector(t, i); skip(becomes);
                         expression(x); mustbe(t, x);
                         if i=0 then gen0(swap)
                                else addressvar(i);
                         if ttab[t].kind=simple
                         then gen0(stor)
                         else gen1(move, ttab[t].size)
                   end;
            funkt: if tip=0 then
                       funcall(i)
                   else begin
                       if not inside then error(119);
                       getsym; skip(becomes);
                       expression(x); mustbe(tip, x);
                       address(flevel+1, resultadr);
                       gen0(stor)
                   end;
            konst, field, tipe: error(120)
          end
      end else if sym=ifsym then begin
          getsym; expression(t); mustbe(booltip, t); skip(thensym);
          i:= codelabel; gen1(jumpz, 0); statement;
          if sym=elsesym then begin
              getsym; j:= codelabel; gen1(jump, 0);
              code[i].a:= codelabel; i:= j; statement
          end;
          code[i].a:= codelabel
      end else if sym=whilesym then begin
          getsym; i:= codelabel; expression(t); mustbe(booltip, t);
          skip(dosym); j:= codelabel; gen1(jumpz, 0);
          statement; gen1(jump, i);
          code[j].a:= codelabel
      end else if sym=repeatsym then begin
          i:= codelabel;
          repeat getsym; statement until sym<>semicolon;
          skip(untilsym); expression(t); mustbe(booltip, t);
          gen1(jumpz, i)
      end else if sym=beginsym then begin
          repeat getsym; statement until sym<>semicolon;
          skip(endsym)
end   end;

procedure block(l: integer);
  forward;

procedure constant(var c, t: integer);
  var i, s: integer;
begin if (sym=sstring) and (slen=1) then begin
          c:= ord(str[1]); t:= chartip
      end else begin
          if sym=plus  then begin getsym; s:= +1 end else
          if sym=minus then begin getsym; s:= -1 end
                       else s:= 0;
          if sym=ident then begin
              i:= position;
              if itab[i].kind<>konst then error(121);
              c:= itab[i].val; t:= itab[i].tip
          end else if sym=number then begin
              c:= num; t:= intip
          end else
              error(122);
          if s<>0 then begin mustbe(t, intip); c:= c*s end
      end;
      getsym
end;

procedure constdeclaration;
  var a: alfa; t, c: integer;
begin a:= id; getsym; skip(eql); constant(c, t);
      skip(semicolon); enter(a, konst, t); itab[ix].val:= c
end;

procedure typ(var t: integer);
  var i, j, sz, ft: integer;
  procedure arraytyp(var t: integer);
    var x: integer;
  begin with ttab[t] do begin
            kind:= arrays; getsym; constant(low, x); mustbe(intip, x);
            skip(colon); constant(high, x); mustbe(intip, x);
            if low>high then error(123);
            if sym=comma then
                 arraytyp(elemtip)
            else begin
                 skip(rbrack); skip(ofsym); typ(elemtip)
                 end;
            size:= (high-low+1)*ttab[elemtip].size
  end   end;
begin if sym=ident then begin
          i:= position; if itab[i].kind<>tipe then error(124);
          t:= itab[i].tip; getsym
      end else begin
          if tx=tmax then error(125); tx:= tx+1; t:= tx;
          if sym=arraysym then begin
              getsym; check(lbrack); arraytyp(t)
          end else begin
              skip(recordsym);
              if lev=lmax then error(126); lev:= lev+1;
              namelist[lev]:= 0; check(ident); sz:= 0;
              repeat enter(id, field, 0); i:= ix; getsym;
                     while sym=comma do begin
                         getsym; check(ident); enter(id, field, 0);
                         getsym
                     end;
                     j:= ix; skip(colon); typ(ft);
                     repeat itab[i].tip:= ft; itab[i].offset:= sz;
                            sz:= sz+ttab[ft].size; i:= i+1
                     until i>j;
                    if sym=semicolon then getsym else check(endsym)
              until sym<>ident;
              ttab[t].size:= sz; ttab[t].kind:= records;
              ttab[t].fields:= namelist[lev]; lev:= lev-1;
              skip(endsym)
end   end end;

procedure typedeclaration;
  var a: alfa; t: integer;
begin a:= id; getsym; skip(eql); typ(t); skip(semicolon);
      enter(a, tipe, t)
end;

procedure vardeclaration;
  var p, q, t: integer;
begin enter(id, varbl, 0); p:= ix; getsym;
      while sym=comma do begin
          getsym; check(ident); enter(id, varbl, 0); getsym
      end;
      q:= ix; skip(colon); typ(t); skip(semicolon);
      repeat with itab[p] do begin
          vlevel:= lev; dx:= dx-ttab[t].size; tip:= t;
          vadr:= dx; refpar:= false
        end;
        p:= p+1
      until p>q
end;

procedure funcdeclaration(isf: boolean);
  var f, p, ps, odx: integer;
  procedure paramlist;
    var r: boolean; t: integer;
  begin if sym=varsym then begin r:= true; getsym end else r:= false;
        check(ident); p:= ix; enter(id, varbl, 0); getsym;
        while sym=comma do begin
            getsym; check(ident); enter(id, varbl, 0); getsym
        end;
        skip(colon); check(ident); typ(t);
        while p<ix do begin
            p:= p+1; itab[p].tip:= t; itab[p].refpar:= r;
            if r then ps:= ps+1 else ps:= ps+ttab[t].size
  end   end;
begin getsym; check(ident); enter(id, funkt, 0); getsym; f:= ix;
      itab[f].flevel:= lev; itab[f].fadr:= codelabel; gen1(jump, 0);
      if lev=lmax then error(127); lev:= lev+1; namelist[lev]:= 0;
      ps:= 1; odx:= dx;
      if sym=lparen then begin
          repeat getsym; paramlist until sym<>semicolon;
          skip(rparen)
      end;
      if lev>1 then dx:= -1
               else dx:= 0;
      itab[f].resultadr:= ps; p:= f;
      while p<ix do begin
          p:= p+1;
          with itab[p] do begin
              if refpar then ps:= ps-1 else ps:= ps-ttab[tip].size;
              vlevel:= lev; vadr:= ps
      end end;
      if isf then begin
          skip(colon); check(ident); typ(itab[f].tip);
          if ttab[itab[f].tip].kind<>simple then error(128)
      end;
      skip(semicolon);
      itab[f].lastpar:= ix; itab[f].inside:= true;
      block(itab[f].fadr);
      itab[f].inside:= false;
      gen1(exit, itab[f].resultadr-dx);
      lev:= lev-1; dx:= odx;
      skip(semicolon)
end;

procedure block{l: integer};
  var d, odx, oix: integer;
begin odx:= dx; oix:= ix;
      if sym=constsym then begin
          getsym; check(ident);
          repeat constdeclaration until sym<>ident
      end;
      if sym=typesym then begin
          getsym; check(ident);
          repeat typedeclaration until sym<>ident
      end;
      if sym=varsym then begin
          getsym; check(ident);
          repeat vardeclaration until sym<>ident
      end;
      while sym in [funcsym, procsym] do funcdeclaration(sym=funcsym);
      if l+1=codelabel then cx:= cx-1 else code[l].a:= codelabel;
      if lev=0 then
          gen1(sets, dx)
      else begin
          d:= dx-odx; dx:= odx;  gen1(adjs, d)
      end;
      statement;
      if lev<>0 then gen1(adjs, odx-dx); ix:= oix
end;

procedure listcode;
  var i: integer;
begin for i:= 0 to cx-1 do begin
          write(i, ' :    ');
          case code[i].op of
            add  : writeln('add');
            neg  : writeln('neg');
            mul  : writeln('mul');
            divd : writeln('divd');
            remd : writeln('remd');
            div2 : writeln('div2');
            rem2 : writeln('rem2');
            eqli : writeln('eqli');
            neqi : writeln('neqi');
            lssi : writeln('lssi');
            leqi : writeln('leqi');
            gtri : writeln('gtri');
            geqi : writeln('geqi');
            dupl : writeln('dupl');
            swap : writeln('swap');
            andb : writeln('andb');
            orb  : writeln('orb');
            load : writeln('load');
            stor : writeln('stor');
            hhalt : writeln('hhalt');
            wri  : writeln('wri');
            wrc  : writeln('wrc');
            wrl  : writeln('wrl');
            rdi  : writeln('rdi');
            rdc  : writeln('rdc');
            rdl  : writeln('rdl');
            eol  : writeln('eol');
            ldc  : writeln('ldc   ', code[i].a);
            ldla : writeln('ldla  ', code[i].a);
            ldl  : writeln('ldl   ', code[i].a);
            ldg  : writeln('ldg   ', code[i].a);
            stl  : writeln('stl   ', code[i].a);
            stg  : writeln('stg   ', code[i].a);
            move : writeln('move  ', code[i].a);
            copy : writeln('copy  ', code[i].a);
            addc : writeln('addc  ', code[i].a);
            mulc : writeln('mulc  ', code[i].a);
            jump : writeln('jump  ', code[i].a);
            jumpz: writeln('jumpz ', code[i].a);
            call : writeln('call  ', code[i].a);
            adjs : writeln('adjs  ', code[i].a);
            sets : writeln('sets  ', code[i].a);
            exit : writeln('exit  ', code[i].a)
end end   end;

begin { compile }
      word[beginsym ]:= 'begin     '; word[endsym   ]:= 'end       ';
      word[ifsym    ]:= 'if        '; word[thensym  ]:= 'then      ';
      word[elsesym  ]:= 'else      '; word[whilesym ]:= 'while     ';
      word[dosym    ]:= 'do        '; word[casesym  ]:= 'case      ';
      word[repeatsym]:= 'repeat    '; word[untilsym ]:= 'until     ';
      word[forsym   ]:= 'for       '; word[tosym    ]:= 'to        ';
      word[downtosym]:= 'downto    '; word[notsym   ]:= 'not       ';
      word[divsym   ]:= 'div       '; word[modsym   ]:= 'mod       ';
      word[andsym   ]:= 'and       '; word[orsym    ]:= 'or        ';
      word[constsym ]:= 'const     '; word[varsym   ]:= 'var       ';
      word[typesym  ]:= 'type      '; word[arraysym ]:= 'array     ';
      word[ofsym    ]:= 'of        '; word[recordsym]:= 'record    ';
      word[progsym  ]:= 'program   '; word[funcsym  ]:= 'function  ';
      word[procsym  ]:= 'procedure ';
      ttab[intip].size:= 1; ttab[intip].kind:= simple;
      ttab[chartip].size:= 1; ttab[chartip].kind:= simple;
      ttab[booltip].size:= 1; ttab[booltip].kind:= simple;
      tx:= 3; namelist[-1]:= 0; lev:= -1; ix:= 0;
      enter('false     ', konst, booltip); itab[ix].val:= ord(false);
      enter('true      ', konst, booltip); itab[ix].val:= ord(true);
      enter('maxint    ', konst, intip);   itab[ix].val:= 32767;
      enter('integer   ', tipe, intip);
      enter('char      ', tipe, chartip);
      enter('boolean   ', tipe, booltip);
      enter('abs       ', funkt, intip);
      itab[ix].flevel:= -1; itab[ix].fadr:= fabs; itab[ix].inside:= false;
      enter('sqr       ', funkt, intip);
      itab[ix].flevel:= -1; itab[ix].fadr:= fsqr; itab[ix].inside:= false;
      enter('odd       ', funkt, booltip);
      itab[ix].flevel:= -1; itab[ix].fadr:= fodd; itab[ix].inside:= false;
      enter('chr       ', funkt, chartip);
      itab[ix].flevel:= -1; itab[ix].fadr:= fchr; itab[ix].inside:= false;
      enter('ord       ', funkt, intip);
      itab[ix].flevel:= -1; itab[ix].fadr:= ford; itab[ix].inside:= false;
      enter('write     ', funkt, 0);
      itab[ix].flevel:= -1; itab[ix].fadr:= fwrite;
      enter('writeln   ', funkt, 0);
      itab[ix].flevel:= -1; itab[ix].fadr:= fwriteln;
      enter('read      ', funkt, 0);
      itab[ix].flevel:= -1; itab[ix].fadr:= fread;
      enter('readln    ', funkt, 0);
      itab[ix].flevel:= -1; itab[ix].fadr:= freadln;
      enter('eoln      ', funkt, booltip);
      itab[ix].flevel:= -1; itab[ix].fadr:= feoln; itab[ix].inside:= false;
      namelist[0]:= 0; lev:= 0; cc:= 0; ll:= 0; getch; getsym;
      labeled:= true; cx:= 0; dx:= amax+1;
      skip(progsym); skip(ident); check(lparen);
      repeat getsym; check(ident);
             if (id<>'input     ') and (id<>'output    ') then error(129);
             getsym
      until sym<>comma;
      skip(rparen); skip(semicolon); gen1(jump, 0); block(0); gen0(hhalt);
      check(period);
      listcode
end;

procedure interpret;
  var pc, sp, j, k, n: integer; i: instr; c: char; h: boolean;
begin pc:= 0; h:= false;
      repeat i:= code[pc]; pc:= pc+1;
        case i.op of
          add  : begin m[sp+1]:= m[sp+1]+m[sp]; sp:= sp+1 end;
          neg  : m[sp]:= -m[sp];
          mul  : begin m[sp+1]:= m[sp+1]*m[sp]; sp:= sp+1 end;
          divd : begin m[sp+1]:= m[sp+1] div m[sp]; sp:= sp+1 end;
          remd : begin m[sp+1]:= m[sp+1] mod m[sp]; sp:= sp+1 end;
          div2 : m[sp]:= m[sp] div 2;
          rem2 : m[sp]:= m[sp] mod 2;
          eqli : begin m[sp+1]:= ord(m[sp+1]=m[sp]); sp:= sp+1 end;
          neqi : begin m[sp+1]:= ord(m[sp+1]<>m[sp]); sp:= sp+1 end;
          lssi : begin m[sp+1]:= ord(m[sp+1]<m[sp]); sp:= sp+1 end;
          leqi : begin m[sp+1]:= ord(m[sp+1]<=m[sp]); sp:= sp+1 end;
          gtri : begin m[sp+1]:= ord(m[sp+1]>m[sp]); sp:= sp+1 end;
          geqi : begin m[sp+1]:= ord(m[sp+1]>=m[sp]); sp:= sp+1 end;
          dupl : begin sp:= sp-1; m[sp]:= m[sp+1] end;
          swap : begin k:= m[sp]; m[sp]:= m[sp+1]; m[sp+1]:= k end;
          andb : begin if m[sp]=0 then m[sp+1]:= 0; sp:= sp+1 end;
          orb  : begin if m[sp]=1 then m[sp+1]:= 1; sp:= sp+1 end;
          load : m[sp]:= m[m[sp]];
          stor : begin m[m[sp]]:= m[sp+1]; sp:= sp+2 end;
          hhalt: h:= true;
          wri  : begin write(output, m[sp+1]: m[sp]); sp:= sp+2 end;
          wrc  : begin write(output, chr(m[sp])); sp:= sp+1 end;
          wrl  : writeln(output);
          rdi  : begin read(input, m[m[sp]]); sp:= sp+1 end;
          rdc  : begin read(input, c); m[m[sp]]:= ord(c); sp:= sp+1 end;
          rdl  : readln(input);
          eol  : begin sp:= sp-1; m[sp]:= ord(eoln(input)) end;
          ldc  : begin sp:= sp-1; m[sp]:= i.a end;
          ldla : begin sp:= sp-1; m[sp]:= sp+1+i.a end;
          ldl  : begin sp:= sp-1; m[sp]:= m[sp+1+i.a] end;
          ldg  : begin sp:= sp-1; m[sp]:= m[i.a] end;
          stl  : begin m[sp+i.a]:= m[sp]; sp:= sp+1 end;
          stg  : begin m[i.a]:= m[sp]; sp:= sp+1 end;
          move : begin k:= m[sp]; j:= m[sp+1]; sp:= sp+2; n:= i.a;
                       repeat n:= n-1; m[k+n]:= m[j+n] until n=0
                 end;
          copy : begin j:= m[sp]; n:= i.a; sp:= sp-n+1;
                       repeat n:= n-1; m[sp+n]:= m[j+n] until n=0
                 end;
          addc : m[sp]:= m[sp]+i.a;
          mulc : m[sp]:= m[sp]*i.a;
          jump : pc:= i.a;
          jumpz: begin if m[sp]=0 then pc:= i.a; sp:= sp+1 end;
          call : begin sp:= sp-1; m[sp]:= pc; pc:= i.a end;
          adjs : sp:= sp+i.a;
          sets : sp:= i.a;
          exit : begin pc:= m[sp]; sp:= sp+i.a end;
        end
      until h
end;

begin
{      assign(infile, 'test.pas'); }
      assign(infile,ParamStr(1));
      reset(infile);
      compile;
    {  interpret;}
end.
