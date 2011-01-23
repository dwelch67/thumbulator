
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this must match the amax in pascals.pas
#define amax  131071


char newline[1024];
char xstring[1024];
char ostring[1024];
char opcode[32];

FILE *fpin;
FILE *fpout;

unsigned int line;
unsigned int errors;

void nice_print ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=0;
    for(rb=0;ostring[rb];rb++) xstring[ra++]=ostring[rb];
    xstring[ra++]=0x20;
    for(;ra<30;) xstring[ra++]=0x20;
    xstring[ra++]=';';
    xstring[ra++]='@';
    xstring[ra++]=0x20;
    for(rb=0;newline[rb];rb++) xstring[ra++]=newline[rb];
    xstring[ra++]=0x0A;
    xstring[ra]=0;
    fwrite(xstring,1,ra,fpout);
}

unsigned int opconstant ( unsigned int off )
{
    unsigned int ra;

    for(;newline[off];off++) if(newline[off]!=0x20) break; //blanks
    if(newline[off]==0)
    {
        fprintf(stderr,"<%u>Error, syntax\n",line);
        errors++;
        sprintf(ostring,"syntax error");
        nice_print();
    }
    ra=strtoul(&newline[off],NULL,10);
    return(ra);
}

void r0constant ( unsigned int ra )
{
    if(ra>0xFF)
    {
        if((-ra)<0x100)
        {
            sprintf(ostring,"  mov r0,#0x%02X",(-ra)); nice_print();
            sprintf(ostring,"  neg r0,r0"); nice_print();
        }
        else
        {
fprintf(fpout,"@ r0=0x%08X\n",ra);
            sprintf(ostring,"  mov r0,#0");  nice_print();
            if(ra&0xFF000000)
            {
                sprintf(ostring,"  mov r1,#0x%02X",(ra>>24)&0xFF);  nice_print();
                sprintf(ostring,"  lsl r1,r1,#24"); nice_print();
                sprintf(ostring,"  orr r0,r1");                    nice_print();
            }
            if(ra&0x00FF0000)
            {
                sprintf(ostring,"  mov r1,#0x%02X",(ra>>16)&0xFF);  nice_print();
                sprintf(ostring,"  lsl r1,r1,#16"); nice_print();
                sprintf(ostring,"  orr r0,r1");                    nice_print();
            }
            if(ra&0x0000FF00)
            {
                sprintf(ostring,"  mov r1,#0x%02X",(ra>> 8)&0xFF);  nice_print();
                sprintf(ostring,"  lsl r1,r1,#8"); nice_print();
                sprintf(ostring,"  orr r0,r1");                    nice_print();
            }
            if(ra&0x000000FF)
            {
                sprintf(ostring,"  mov r1,#0x%02X",(ra>> 0)&0xFF);  nice_print();
                //sprintf(ostring,"  lsl r1,r1,#0"); nice_print();
                sprintf(ostring,"  orr r0,r1");                    nice_print();
            }
        }
    }
    else
    {
        sprintf(ostring,"  mov r0,#0x%02X",ra);            nice_print();
    }
}

int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int off;
    unsigned int tracksp;
    unsigned int pline;

    if(argc<2)
    {
        fprintf(stderr,"file not specified\n");
        return(1);
    }

    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        fprintf(stderr,"Error opening file [%s]\n",argv[1]);
        return(1);
    }

    sprintf(newline,"%s.s",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        fprintf(stderr,"Error creating file [%s]\n",newline);
        return(1);
    }

    fprintf(fpout,"\n");
    fprintf(fpout,"\n");
    fprintf(fpout,".thumb\n");
    fprintf(fpout,".thumb_func\n");
    fprintf(fpout,".globl notmain\n");
    fprintf(fpout,"notmain:\n");
    fprintf(fpout,"  mov r2,sp\n");

    line=0;
    errors=0;
    tracksp=1000;

    while(fgets(newline,sizeof(newline)-1,fpin))
    {
        line++;

        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]==0x0D) newline[ra]=0;
            if(newline[ra]==0x0A) newline[ra]=0;
        }

        if(newline[0]==0)
        {
            fprintf(stderr,"<%u> Warning, blank line\n",line);
            continue;
        }

        //--- line number
        rb=0;
        for(off=0;newline[off];off++)
        {
            xstring[rb]=newline[off];
            if(xstring[rb]==':')
            {
                xstring[rb]=0;
                break;
            }
            rb++;
        }
        if(newline[off]!=':')
        {
            fprintf(stderr,"<%u> Error, missing :\n",line);
            errors++;
            sprintf(ostring,"error");
            nice_print();
            continue;
        }
        if(newline[off]) off++;
        pline=strtoul(xstring,NULL,10);
        fprintf(fpout,"LL%05u:\n",pline);

        //blanks
        for(;newline[off];off++) if(newline[off]!=0x20) break;
        //opcode
        rb=0;
        for(;newline[off];off++)
        {
            opcode[rb]=newline[off];
            if(opcode[rb]==0x20) break;
            rb++;
        }
        if(newline[off]) off++;
        opcode[rb]=0;
        if(opcode[0]==0)
        {
            fprintf(stderr,"<%u> Error, no opcode\n",line);
            errors++;
            sprintf(ostring,"error");
            nice_print();
            continue;
        }
fprintf(fpout,"@ %u\n",tracksp>>2);
        //case i.op of
          //add  : begin m[sp+1]:= m[sp+1]+m[sp]; sp:= sp+1 end;
          //neg  : m[sp]:= -m[sp];
          //mul  : begin m[sp+1]:= m[sp+1]*m[sp]; sp:= sp+1 end;
          //divd : begin m[sp+1]:= m[sp+1] div m[sp]; sp:= sp+1 end;
          //remd : begin m[sp+1]:= m[sp+1] mod m[sp]; sp:= sp+1 end;
          //div2 : m[sp]:= m[sp] div 2;
          //rem2 : m[sp]:= m[sp] mod 2;
          //eqli : begin m[sp+1]:= ord(m[sp+1]=m[sp]); sp:= sp+1 end;
        if(strcmp(opcode,"eqli")==0)
        {
            sprintf(ostring,"  pop {r0}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  pop {r1}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  cmp r0,r1"); nice_print();
            sprintf(ostring,"  beq LL%05ueq",pline); nice_print();
            sprintf(ostring,"  mov r0,#0"); nice_print();
            sprintf(ostring,"  b LL%05une",pline); nice_print();
            sprintf(ostring,"LL%05ueq:",pline); nice_print();
            sprintf(ostring,"  mov r0,#1"); nice_print();
            sprintf(ostring,"LL%05une:",pline); nice_print();
            sprintf(ostring,"  push {r0}"); nice_print();
            tracksp-=4;
            continue;
        }
          //neqi : begin m[sp+1]:= ord(m[sp+1]<>m[sp]); sp:= sp+1 end;
        if(strcmp(opcode,"neqi")==0)
        {
            sprintf(ostring,"  pop {r0}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  pop {r1}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  cmp r0,r1"); nice_print();
            sprintf(ostring,"  bne LL%05une",pline); nice_print();
            sprintf(ostring,"  mov r0,#0"); nice_print();
            sprintf(ostring,"  b LL%05ueq",pline); nice_print();
            sprintf(ostring,"LL%05une:",pline); nice_print();
            sprintf(ostring,"  mov r0,#1"); nice_print();
            sprintf(ostring,"LL%05ueq:",pline); nice_print();
            sprintf(ostring,"  push {r0}"); nice_print();
            tracksp-=4;
            continue;
        }
          //lssi : begin m[sp+1]:= ord(m[sp+1]<m[sp]); sp:= sp+1 end;
          //leqi : begin m[sp+1]:= ord(m[sp+1]<=m[sp]); sp:= sp+1 end;
          //gtri : begin m[sp+1]:= ord(m[sp+1]>m[sp]); sp:= sp+1 end;
          //geqi : begin m[sp+1]:= ord(m[sp+1]>=m[sp]); sp:= sp+1 end;
          //dupl : begin sp:= sp-1; m[sp]:= m[sp+1] end;
          //swap : begin k:= m[sp]; m[sp]:= m[sp+1]; m[sp+1]:= k end;
          //andb : begin if m[sp]=0 then m[sp+1]:= 0; sp:= sp+1 end;
          //orb  : begin if m[sp]=1 then m[sp+1]:= 1; sp:= sp+1 end;
          //load : m[sp]:= m[m[sp]];
        if(strcmp(opcode,"load")==0)
        {
            sprintf(ostring,"  pop {r0}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  ldr r1,[r0]"); nice_print();
            sprintf(ostring,"  push {r1}"); nice_print();
            tracksp-=4;
            continue;
        }
          //stor : begin m[m[sp]]:= m[sp+1]; sp:= sp+2 end;
        if(strcmp(opcode,"stor")==0)
        {
            sprintf(ostring,"  pop {r0}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  pop {r1}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  str r1,[r0]"); nice_print();
            continue;
        }
          //hhalt: h:= true;
        if(strcmp(opcode,"hhalt")==0)
        {
            sprintf(ostring,"  swi 0"); nice_print();
            sprintf(ostring,"  b ."); nice_print();
            continue;
        }

          //wri  : begin write(output, m[sp+1]: m[sp]); sp:= sp+2 end;
          //wrc  : begin write(output, chr(m[sp])); sp:= sp+1 end;
          //wrl  : writeln(output);
          //rdi  : begin read(input, m[m[sp]]); sp:= sp+1 end;
          //rdc  : begin read(input, c); m[m[sp]]:= ord(c); sp:= sp+1 end;
          //rdl  : readln(input);
          //eol  : begin sp:= sp-1; m[sp]:= ord(eoln(input)) end;
          //ldc  : begin sp:= sp-1; m[sp]:= i.a end;
        if(strcmp(opcode,"ldc")==0)
        {
            ra=opconstant(off);
            if(ra&(~0xFFFF))
            {
                //this is actually a stack based address
                rb=(amax+1)-ra;
                rb<<=2;
                if(rb&0x80000000)
                {
                    rb=-rb;
                    if(rb>0xFF)
                    {
                        r0constant(rb);
                        sprintf(ostring,"  add r0,r2,r0");         nice_print();
                        sprintf(ostring,"  push {r0}");         nice_print();
                        tracksp-=4;
                    }
                    else
                    {
                        sprintf(ostring,"  mov r0,r2");         nice_print();
                        if(rb)
                        {
                            sprintf(ostring,"  add r0,#0x%02X",rb); nice_print();
                        }
                        sprintf(ostring,"  push {r0}");         nice_print();
                        tracksp-=4;
                    }

                }
                else
                {
                    if(rb>0xFF)
                    {
                        r0constant(rb);
                        sprintf(ostring,"  sub r0,r2,r0");         nice_print();
                        sprintf(ostring,"  push {r0}");         nice_print();
                        tracksp-=4;
                    }
                    else
                    {
                        sprintf(ostring,"  mov r0,r2");         nice_print();
                        if(rb)
                        {
                            sprintf(ostring,"  sub r0,#0x%02X",rb); nice_print();
                        }
                        sprintf(ostring,"  push {r0}");         nice_print();
                        tracksp-=4;
                    }
                }
            }
            else
            {
                r0constant(ra);
                sprintf(ostring,"  push {r0}"); nice_print();
                tracksp-=4;
            }
            continue;
        }
          //ldla : begin sp:= sp-1; m[sp]:= sp+1+i.a end;
          //ldl  : begin sp:= sp-1; m[sp]:= m[sp+1+i.a] end;
        if(strcmp(opcode,"ldl")==0)
        {
            ra=opconstant(off);
            rb=ra<<2;
            if(rb>0xFF)
            {
                //TODO FIXME
                sprintf(ostring,"  ldr r1,[sp,#0x%02X]",rb); nice_print();
            }
            else
            {
                sprintf(ostring,"  ldr r1,[sp,#0x%02X]",rb); nice_print();
            }
            sprintf(ostring,"  push {r1}"); nice_print();
            tracksp-=4;
            continue;
        }
          //ldg  : begin sp:= sp-1; m[sp]:= m[i.a] end;
        if(strcmp(opcode,"ldg")==0)
        {
            ra=opconstant(off);
            rb=(amax+1)-ra;
            rb<<=2;
            r0constant(rb);
            sprintf(ostring,"  sub r0,r2,r0"); nice_print();
            sprintf(ostring,"  ldr r1,[r0]"); nice_print();
            sprintf(ostring,"  push {r1}"); nice_print();
            tracksp-=4;
            continue;
        }
          //stl  : begin m[sp+i.a]:= m[sp]; sp:= sp+1 end;
          //stg  : begin m[i.a]:= m[sp]; sp:= sp+1 end;
        if(strcmp(opcode,"stg")==0)
        {
            ra=opconstant(off);
            rb=(amax+1)-ra;
            rb<<=2;
            sprintf(ostring,"  pop {r1}"); nice_print();
            tracksp+=4;
            r0constant(rb);
            sprintf(ostring,"  sub r0,r2,r0"); nice_print();
            sprintf(ostring,"  str r1,[r0]"); nice_print();
            continue;
        }
          //move : begin k:= m[sp]; j:= m[sp+1]; sp:= sp+2; n:= i.a;
                       //repeat n:= n-1; m[k+n]:= m[j+n] until n=0
                 //end;
          //copy : begin j:= m[sp]; n:= i.a; sp:= sp-n+1;
                       //repeat n:= n-1; m[sp+n]:= m[j+n] until n=0
                 //end;
          //addc : m[sp]:= m[sp]+i.a;
        if(strcmp(opcode,"addc")==0)
        {
            ra=opconstant(off);
            if(ra&0x80000000)
            {
                ra=-ra;
                if(ra>0xFF)
                {
                    r0constant(ra);
                    sprintf(ostring,"  pop {r1}"); nice_print();
                    tracksp+=4;
                    sprintf(ostring,"  sub r1,r0"); nice_print();
                    sprintf(ostring,"  push {r1}"); nice_print();
                    tracksp-=4;
                }
                else
                {
                    sprintf(ostring,"  pop {r1}"); nice_print();
                    tracksp+=4;
                    sprintf(ostring,"  sub r1,#0x%02X",ra); nice_print();
                    sprintf(ostring,"  push {r1}"); nice_print();
                    tracksp-=4;
                }
            }
            else
            {
                ra=opconstant(off);
                if(ra>0xFF)
                {
                    r0constant(ra);
                    sprintf(ostring,"  pop {r1}"); nice_print();
                    tracksp+=4;
                    sprintf(ostring,"  add r1,r0"); nice_print();
                    sprintf(ostring,"  push {r1}"); nice_print();
                    tracksp-=4;
                }
                else
                {
                    sprintf(ostring,"  pop {r1}"); nice_print();
                    tracksp+=4;
                    sprintf(ostring,"  add r1,#0x%02X",ra); nice_print();
                    sprintf(ostring,"  push {r1}"); nice_print();
                    tracksp-=4;
                }
            }
            continue;
        }
          //mulc : m[sp]:= m[sp]*i.a;
          //jump : pc:= i.a;
        if(strcmp(opcode,"jump")==0)
        {
            ra=opconstant(off);
            sprintf(ostring,"  b LL%05u",ra); nice_print();
            continue;
        }
          //jumpz: begin if m[sp]=0 then pc:= i.a; sp:= sp+1 end;
        if(strcmp(opcode,"jumpz")==0)
        {
            ra=opconstant(off);
            sprintf(ostring,"  pop {r0}"); nice_print();
            tracksp+=4;
            sprintf(ostring,"  cmp r0,#0"); nice_print();
            sprintf(ostring,"  beq LL%05u",ra); nice_print();
            continue;
        }
          //call : begin sp:= sp-1; m[sp]:= pc; pc:= i.a end;
        if(strcmp(opcode,"call")==0)
        {
            ra=opconstant(off);
            //5:  mov r0,pc   pc has address for 7
            //6:  add r0,#4   add two instructions, address for 9
            //7:  push {r0}   address for 9
            //8:  b label
            //9:  return here
            sprintf(ostring,"  mov r0,pc");    nice_print();
            sprintf(ostring,"  add r0,#4");    nice_print();
fprintf(fpout,"@ %u\n",tracksp>>2);
            sprintf(ostring,"  push {r0}");    nice_print();
            tracksp-=4;
fprintf(fpout,"@ %u\n",tracksp>>2);
            sprintf(ostring,"  b LL%05u",ra);  nice_print();
            tracksp+=4;
            continue;
        }
          //adjs : sp:= sp+i.a;
          //sets : sp:= i.a;
        if(strcmp(opcode,"sets")==0)
        {
            ra=opconstant(off);
            rb=(amax+1)-ra;
            rb<<=2;
            if(rb>0xFF)
            {
                r0constant(rb);
                sprintf(ostring,"  mov r1,r2");         nice_print();
                sprintf(ostring,"  sub r1,r0");         nice_print();
                sprintf(ostring,"  mov sp,r1");         nice_print();
            }
            else
            {
                if(rb==0)
                {
                    sprintf(ostring,"  ;@ sub sp,#0");      nice_print();
                }
                else
                {
                    sprintf(ostring,"  mov r0,r2");         nice_print();
                    sprintf(ostring,"  sub r0,#0x%02X",rb); nice_print();
                    sprintf(ostring,"  mov sp,r0");         nice_print();
                }
            }
            tracksp=ra;
            continue;
        }
          //exit : begin pc:= m[sp]; sp:= sp+i.a end;
        if(strcmp(opcode,"exit")==0)
        {
            ra=opconstant(off);
            sprintf(ostring,"  pop {r1}"); nice_print();
            tracksp+=4;
            if(ra>1)
            {
                ra--; //account for the pop {r1} above
                ra--; //what for?
                for(rb=0;rb<ra;rb++) //TODO FIXME very lazy
                {
                    sprintf(ostring,"  pop {r0}"); nice_print();
                    tracksp+=4;
                }
            }
fprintf(fpout,"@ %u\n",tracksp>>2);
            sprintf(ostring,"  bx r1"); nice_print();
            continue;
        }



        fprintf(stderr,"<%u> Error, unknown opcode [%s]\n",line,opcode);
        sprintf(ostring,"unknown opcode");
        nice_print();
        errors++;
    }

    fclose(fpout);
    fclose(fpin);
    return(errors);
}

