
//-------------------------------------------------------------------
// Copyright (C) 2012 David Welch
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *fpin;
FILE *fpout;

unsigned int rd,rm,rn,rx;

#define ADDMASK 0xFFFF
unsigned short mem[ADDMASK+1];
unsigned short mark[ADDMASK+1];

unsigned int curradd;
unsigned int line;

char cstring[1024];
char newline[1024];


#define LABLEN 64

#define MAX_LABS 65536
struct
{
    char name[LABLEN];
    unsigned int addr;
    unsigned int line;
    unsigned int type;
} lab_struct[MAX_LABS];
unsigned int nlabs;
unsigned int lab;


#define NREGNAMES (19)
struct
{
    char name[4];
    unsigned char num;
} reg_names[NREGNAMES]=
{
    {"r0",0},{"r1",1},{"r2",2},{"r3",3},{"r4",4},{"r5",5},{"r6",6},{"r7",7},
    {"r8",8},{"r9",9},{"r10",10},{"r11",11},{"r12",12},{"r13",13},{"r14",14},{"r15",15},
    {"sp",13},{"lr",14},{"pc",15}
};


unsigned char hexchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned char numchar[256]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int rest_of_line ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra])
    {
        printf("<%u> Error: garbage at end of line\n",line);
        return(1);
    }
    return(0);
}
//-------------------------------------------------------------------
int get_reg_number ( char *s, unsigned int *rx )
{
    unsigned int ra;

    for(ra=0;ra<NREGNAMES;ra++)
    {
        if(strcmp(s,reg_names[ra].name)==0)
        {
            *rx=reg_names[ra].num;
            return(0);
        }
    }
    return(1);
}
//-------------------------------------------------------------------
unsigned int parse_immed ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if((newline[ra]==0x30)&&((newline[ra+1]=='x')||(newline[ra+1]=='X')))
    {
        ra+=2;
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==0x20) break;
            if(!hexchar[newline[ra]])
            {
                printf("<%u> Error: invalid number\n",line);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%u> Error: invalid number\n",line);
            return(0);
        }
        rx=strtoul(cstring,NULL,16);
    }
    else
    {
        rb=0;
        for(;newline[ra];ra++)
        {
            if(newline[ra]==0x20) break;
            if(!numchar[newline[ra]])
            {
                printf("<%u> Error: invalid number\n",line);
                return(0);
            }
            cstring[rb++]=newline[ra];
        }
        cstring[rb]=0;
        if(rb==0)
        {
            printf("<%u> Error: invalid number\n",line);
            return(0);
        }
        rx=strtoul(cstring,NULL,10);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_reg ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    rb=0;
    for(;newline[ra];ra++)
    {
        if(newline[ra]==',') break;
        if(newline[ra]==']') break;
        if(newline[ra]==0x20) break;
        cstring[rb++]=newline[ra];
    }
    cstring[rb]=0;
    if(get_reg_number(cstring,&rx))
    {
        printf("<%u> Error: not a register\n",line);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_low_reg ( unsigned int ra )
{
    unsigned int rb;

    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    rb=0;
    for(;newline[ra];ra++)
    {
        if(newline[ra]==',') break;
        if(newline[ra]=='!') break;
        if(newline[ra]=='}') break;
        if(newline[ra]==0x20) break;
        cstring[rb++]=newline[ra];
    }
    cstring[rb]=0;
    if(get_reg_number(cstring,&rx))
    {
        printf("<%u> Error: not a register\n",line);
        return(0);
    }
    if(rx>7)
    {
        printf("<%u> Error: invalid (high) register\n",line);
        return(0);
    }
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_comma ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]!=',')
    {
        printf("<%u> Error: syntax error\n",line);
        return(0);
    }
    ra++;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_character ( unsigned int ra, unsigned char ch )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    if(newline[ra]!=ch)
    {
        printf("<%u> Error: syntax error\n",line);
        return(0);
    }
    ra++;
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int no_spaces ( unsigned int ra )
{
    for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
    return(ra);
}
//-------------------------------------------------------------------
unsigned int parse_two_regs ( unsigned int ra )
{
    ra=parse_low_reg(ra); if(ra==0) return(0);
    rd=rx;
    ra=parse_comma(ra); if(ra==0) return(0);
    ra=parse_low_reg(ra); if(ra==0) return(0);
    rm=rx;
    return(ra);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int assemble ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;


    curradd=0;
    nlabs=0;
    memset(mem,0x00,sizeof(mem));
    memset(mark,0x00,sizeof(mark));

    line=0;
    while(fgets(newline,sizeof(newline)-1,fpin))
    {
        line++;
        //tabs to spaces and other things
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]<0x20)  newline[ra]=0x20;
            if(newline[ra]>=0x7F) newline[ra]=0;
        }

        //various ways to comment lines
        for(ra=0;newline[ra];ra++)
        {
            if(newline[ra]==';') newline[ra]=0;
            if(newline[ra]=='@') newline[ra]=0;
            if((newline[ra]=='/')&&(newline[ra+1]=='/')) newline[ra]=0;
            if(newline[ra]==0) break;
        }

        //skip spaces
        for(ra=0;newline[ra];ra++) if(newline[ra]!=0x20) break;
        if(newline[ra]==0) continue;

        //look for a label?
        for(rb=ra;newline[rb];rb++)
        {
            if(newline[rb]==0x20) break; //no spaces in labels
            if(newline[rb]==':') break;
        }
        if(newline[rb]==':')
        {
            //got a label
            rc=rb-ra;
            if(rc==0)
            {
                printf("<%u> Error: Invalid label\n",line);
                return(1);
            }
            rc--;
            if(rc>=LABLEN)
            {
                printf("<%u> Error: Label too long\n",line);
                return(1);
            }
            for(rb=0;rb<=rc;rb++)
            {
                lab_struct[nlabs].name[rb]=newline[ra++];
            }
            lab_struct[nlabs].name[rb]=0;
            lab_struct[nlabs].addr=curradd<<1;
            lab_struct[nlabs].line=line;
            lab_struct[nlabs].type=0;
            ra++;
            for(lab=0;lab<nlabs;lab++)
            {
                if(lab_struct[lab].type) continue;
                if(strcmp(lab_struct[lab].name,lab_struct[nlabs].name)==0) break;
            }
            if(lab<nlabs)
            {
                printf("<%u> Error: label [%s] already defined on line %u\n",line,lab_struct[lab].name,lab_struct[lab].line);
                return(1);
            }
            nlabs++;
            //skip spaces
            for(;newline[ra];ra++) if(newline[ra]!=0x20) break;
            if(newline[ra]==0) continue;
        }
//// .word -----------------------------------------------------------
        //if(strncmp(&newline[ra],".word ",6)==0)
        //{
            //ra+=6;
            //ra=parse_immed(ra); if(ra==0) return(1);
            //mem[curradd]=rx;
            //mark[curradd]|=0x8000;
            //curradd++;
            //if(rest_of_line(ra)) return(1);
            //continue;
        //}
// adc -----------------------------------------------------------
        if(strncmp(&newline[ra],"adc ",4)==0)
        {
            ra+=4;
            //adc rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4140|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// add -----------------------------------------------------------
        if(strncmp(&newline[ra],"add ",4)==0)
        {
            ra+=4;
            //add rd,rm,#immed_3
            //add rd,#immed_8
            //add rd,rn,rm
            //add rd,rm one or both high
            //add rd,pc,#immed_8*4
            //add rd,sp,#immed_8*4
            //add sp,#immed_7*4
            ra=parse_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(rd>7)
            {
                //add rdhi,
                if((rd==13)&&(newline[ra]=='#'))
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x1FC)!=rx)
                    {
                        printf("<%u> Error: Invalid immediate\n",line);
                        return(1);
                    }
                    //add sp,#immed_7*4
                    mem[curradd]=0xB000|(rx>>2);
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    ra=parse_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    //add rd,rm one or both high
                    mem[curradd]=0x4400|((rd&8)<<4)|(rm<<3)|(rd&7);
                    mark[curradd]=0x8000;
                    curradd++;
                }
            }
            else
            {
                //add rdlo,
                if(newline[ra]=='#')
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0xFF)!=rx)
                    {
                        printf("<%u> Error: Invalid immediate\n",line);
                        return(1);
                    }
                    //add rd,#immed_8
                    mem[curradd]=0x3000|(rd<<8)|rx;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //add rdlo,r?
                    ra=parse_reg(ra); if(ra==0) return(1);
                    rm=rx;
                    if(rm>7)
                    {
                        //add rdlo,rhi
                        if(newline[ra]==',')
                        {
                            //add rdlo,rhi,
                            ra=parse_comma(ra); if(ra==0) return(1);
                            if(newline[ra]!='#')
                            {
                                printf("<%u> Error: Invalid immediate\n",line);
                                return(1);
                            }
                            if(rm==15)
                            {
                                ra++;
                                ra=parse_immed(ra); if(ra==0) return(1);
                                if((rx&0x3FC)!=rx)
                                {
                                    printf("<%u> Error: Invalid immediate\n",line);
                                    return(1);
                                }
                                //add rd,pc,#immed_8*4
                                mem[curradd]=0xA000|(rd<<8)|(rx>>2);
                                mark[curradd]=0x8000;
                                curradd++;
                            }
                            else
                            if(rm==13)
                            {
                                ra++;
                                ra=parse_immed(ra); if(ra==0) return(1);
                                if((rx&0x3FC)!=rx)
                                {
                                    printf("<%u> Error: Invalid immediate\n",line);
                                    return(1);
                                }
                                //add rd,pc,#immed_8*4
                                mem[curradd]=0xA800|(rd<<8)|(rx>>2);
                                mark[curradd]=0x8000;
                                curradd++;
                            }
                            else
                            {
                                printf("<%u> Error: Invalid second register\n",line);
                                return(1);
                            }
                        }
                        else
                        {
                            //add rdlo,rdhi
                            //add rd,rm one or both high
                            mem[curradd]=0x4400|((rd&8)<<4)|(rm<<3)|(rd&7);
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                    else
                    {
                        //add rlo,rlo
                        ra=parse_comma(ra); if(ra==0) return(1);
                        if(newline[ra]=='#')
                        {
                            rn=rm;
                            ra++;
                            ra=parse_immed(ra); if(ra==0) return(1);
                            if((rx&0x7)!=rx)
                            {
                                printf("<%u> Error: Invalid immediate\n",line);
                                return(1);
                            }
                            //add rd,rm,#immed_3
                            mem[curradd]=0x1C00|(rx<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                        else
                        {
                            rn=rm;
                            ra=parse_low_reg(ra); if(ra==0) return(1);
                            rm=rx;
                            mem[curradd]=0x1800|(rm<<6)|(rn<<3)|rd;
                            mark[curradd]=0x8000;
                            curradd++;
                        }
                    }
                }
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// and -----------------------------------------------------------
        if(strncmp(&newline[ra],"and ",4)==0)
        {
            ra+=4;
            //and rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4000|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// asr -----------------------------------------------------------
        if(strncmp(&newline[ra],"asr ",4)==0)
        {
            ra+=4;
            //asr rd,rm,#immed_5
            //asr rd,rs
            ra=parse_two_regs(ra); if(ra==0) return(1);
            if(newline[ra]==',')
            {
                ra=parse_comma(ra); if(ra==0) return(1);
                if(newline[ra]=='#')
                {
                    ra++;
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx<1)||(rx>32))
                    {
                        printf("<%u> Error: Invalid immediate\n",line);
                        return(1);
                    }
                    rx&=0x1F;
                    //asr rd,rm,#immed_5
                    mem[curradd]=0x1000|(rx<<6)|(rm<<3)|rd;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    printf("<%u> Error: Invalid immediate\n",line);
                    return(1);
                }
            }
            else
            {
                //asr rd,rs
                mem[curradd]=0x4100|(rm<<3)|rd;
                mark[curradd]=0x8000;
                curradd++;
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// b -----------------------------------------------------------
//TODO
// bic -----------------------------------------------------------
        if(strncmp(&newline[ra],"bic ",4)==0)
        {
            ra+=4;
            //bic rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4380|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// bkpt -----------------------------------------------------------
        if(strncmp(&newline[ra],"bkpt ",5)==0)
        {
            ra+=5;
            //bkpt immed_8
            ra=parse_immed(ra); if(ra==0) return(1);
            if((rx&0xFF)!=rx)
            {
                printf("<%u> Error: Invalid immediate\n",line);
                return(1);
            }
            mem[curradd]=0xBE00|rx;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// bl -----------------------------------------------------------
//TODO
// bx -----------------------------------------------------------
        if(strncmp(&newline[ra],"bx ",3)==0)
        {
            ra+=3;
            //bx rm
            ra=parse_reg(ra); if(ra==0) return(1);
            rm=rx;
            mem[curradd]=0x4700|(rm<<3);
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// cmn -----------------------------------------------------------
        if(strncmp(&newline[ra],"cmn ",4)==0)
        {
            ra+=4;
            //cmn rn,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x42C0|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// cmp -----------------------------------------------------------
        if(strncmp(&newline[ra],"cmp ",4)==0)
        {
            ra+=4;
            //cmp rn,#immed_8
            //cmp rn,rm both low
            //cmp rn,rm one or the other high
            ra=parse_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            if(newline[ra]=='#')
            {
                ra++;
                ra=parse_immed(ra); if(ra==0) return(1);
                if((rx&0xFF)!=rx)
                {
                    printf("<%u> Error: Invalid immediate\n",line);
                    return(1);
                }
                //cmp rn,#immed_8
                mem[curradd]=0x2800|(rn<<8)|rx;
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                ra=parse_reg(ra); if(ra==0) return(1);
                rm=rx;
                if((rn>7)||(rm>7))
                {
                    if(rn==15)
                    {
                        printf("<%u> Error: Not wise to use r15 in this way\n",line);
                        return(1);
                    }
                    //cmp rn,rm one or the other high
                    mem[curradd]=0x4500|((rn&8)<<4)|(rm<<3)|rn;
                    mark[curradd]=0x8000;
                    curradd++;
                }
                else
                {
                    //cmp rn,rm both low
                    mem[curradd]=0x4280|(rm<<3)|rn;
                    mark[curradd]=0x8000;
                    curradd++;
                }

            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
// eor -----------------------------------------------------------
        if(strncmp(&newline[ra],"eor ",4)==0)
        {
            ra+=4;
            //eor rd,rm
            ra=parse_two_regs(ra); if(ra==0) return(1);
            mem[curradd]=0x4040|(rm<<3)|rd;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldmia -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldmia ",6)==0)
        {
            ra+=6;
            //ldmia rn!,{r0,r1,...,r7}
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rn=rx;
            ra=parse_character(ra,'!'); if(ra==0) return(1);
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'{'); if(ra==0) return(1);
            rm=0;
            while(1)
            {
                ra=parse_low_reg(ra); if(ra==0) return(1);
                if(rm&(1<<rx))
                {
                    printf("<%u> Warning: You already specified r%u\n",line,rx);
                }
                rm|=(1<<rx);
                if(newline[ra]=='}')
                {
                    ra++;
                    break;
                }
                ra=parse_comma(ra); if(ra==0) return(1);
            }
            mem[curradd]=0xC800|(rn<<8)|rm;
            mark[curradd]=0x8000;
            curradd++;
            if(rest_of_line(ra)) return(1);
            continue;
        }
// ldr -----------------------------------------------------------
        if(strncmp(&newline[ra],"ldr ",4)==0)
        {
            ra+=4;
            //ldr rd,[rn,#immed_5*4]
            //ldr rd,[rn,rm]
            //ldr rd,[pc,#immed_8*4]
            //ldr rd,[sp,#immed_8*4]
            ra=parse_low_reg(ra); if(ra==0) return(1);
            rd=rx;
            ra=parse_comma(ra); if(ra==0) return(1);
            ra=parse_character(ra,'['); if(ra==0) return(1);
            ra=parse_reg(ra); if(ra==0) return(1);
            rn=rx;
            if(rx>7)
            {
                if(rn==15)
                {
                }
                else
                if(rn==13)
                {
                }
                else
                {
                    printf("<%u> Error: Invalid base register\n",line);
                    return(1);
                }
                if(newline[ra]==']')
                {
                    rx=0;
                }
                else
                {
                    ra=parse_comma(ra); if(ra==0) return(1);
                    ra=parse_character(ra,'#'); if(ra==0) return(1);
                    ra=parse_immed(ra); if(ra==0) return(1);
                    if((rx&0x3FC)!=rx)
                    {
                        printf("<%u> Error: Invalid immediate\n",line);
                        return(1);
                    }
                }
                ra=parse_character(ra,']'); if(ra==0) return(1);
                //ldr rd,[pc,#immed_8*4]
                //ldr rd,[sp,#immed_8*4]
                mem[curradd]=0x0000|(rd<<8)|(rx>>2);
                if(rn==15) mem[curradd]|=0x4800;
                if(rn==13) mem[curradd]|=0x9800;
                mark[curradd]=0x8000;
                curradd++;
            }
            else
            {
                if(newline[ra]==',')
                {
                    ra=parse_comma(ra); if(ra==0) return(1);
                    if(newline[ra]=='#')
                    {
                        //ldr rd,[rn,#immed_5*4]
                        ra=parse_character(ra,'#'); if(ra==0) return(1);
                        ra=parse_immed(ra); if(ra==0) return(1);
                        if((rx&0x3C)!=rx)
                        {
                            printf("<%u> Error: Invalid immediate\n",line);
                            return(1);
                        }
                        ra=parse_character(ra,']'); if(ra==0) return(1);
                        mem[curradd]=0x6800|((rx>>2)<<6)|(rn<<3)|rd;
                        mark[curradd]=0x8000;
                        curradd++;
                    }
                    else
                    {
                        //ldr rd,[rn,rm]
                    }
                }
                else
                {
                    //ldr rd,[rn] immed_5 = 0
                }
                syntax error
            }
            if(rest_of_line(ra)) return(1);
            continue;
        }
//// nop -----------------------------------------------------------
        //if(strncmp(&newline[ra],"nop",3)==0)
        //{
            //ra+=3;
            ////nop
            ////printf("<%u> nop is a pseudo instruction for sll $0,$0,0\n",line);
            //mem[curradd]=0x00000000;
            //mark[curradd]|=0x8000;
            //curradd++;
            //if(rest_of_line(ra)) return(1);
            //continue;
        //}
// -----------------------------------------------------------
        printf("<%u> Error: syntax error\n",line);
        return(1);
    }
    return(0);
}
//-------------------------------------------------------------------
void dissassemble ( unsigned short inst )
{
    if((inst&0xFFC0)==0x4140)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        printf("adc r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFE00)==0x1C00)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rx=(inst>>6)&7;
        printf("add r%u,r%u,#%u",rd,rn,rx);
        return;
    }
    if((inst&0xF800)==0x3000)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        printf("add r%u,#0x%02X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFE00)==0x1800)
    {
        rd=inst&7;
        rn=(inst>>3)&7;
        rm=(inst>>6)&7;
        printf("add r%u,r%u,r%u",rd,rn,rm);
        return;
    }
    if((inst&0xFF00)==0x4400)
    {
        rd=inst&7;
        rd|=(inst>>4)&8;
        rm=(inst>>3)&0xF;
        printf("add r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0xA000)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        rx<<=2;
        printf("add r%u,pc,#0x%03X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xF800)==0xA800)
    {
        rd=(inst>>8)&7;
        rx=inst&0xFF;
        rx<<=2;
        printf("add r%u,sp,#0x%03X ; %u",rd,rx,rx);
        return;
    }
    if((inst&0xFF80)==0xB000)
    {
        rx=inst&0x7F;
        rx<<=2;
        printf("add sp,#0x%03X ; %u",rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x4000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        printf("and r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0x1000)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        rx=(inst>>6)&0x1F;
        if(rx==0) rx=32;
        printf("asr r%u,r%u,#%u",rd,rm,rx);
        return;
    }
    if((inst&0xFFC0)==0x4100)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        printf("asr r%u,r%u",rd,rm);
        return;
    }
// b -----------------------------------------------------------
    if((inst&0xFFC0)==0x4380)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        printf("bic r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xFF00)==0xBE00)
    {
        rx=inst&0xFF;
        printf("bkpt 0x%02X ; %u",rx,rx);
        return;
    }
// bl -----------------------------------------------------------
    if((inst&0xFF87)==0x4700)
    {
        rm=(inst>>3)&0xF;
        printf("bx r%u",rm);
        return;
    }
    if((inst&0xFFC0)==0x42C0)
    {
        rn=inst&7;
        rm=(inst>>3)&7;
        printf("cmn r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xF800)==0x2800)
    {
        rx=inst&0xFF;
        rn=(inst>>3)&7;
        printf("cmp r%u,#0x%02X ; %u",rn,rx,rx);
        return;
    }
    if((inst&0xFFC0)==0x4280)
    {
        rn=inst&7;
        rm=(inst>>3)&7;
        printf("cmp r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xFF00)==0x4500)
    {
        rn=inst&7;
        rn|=(inst>>4)&8;
        rm=(inst>>3)&0xF;
        printf("cmp r%u,r%u",rn,rm);
        return;
    }
    if((inst&0xFFC0)==0x4040)
    {
        rd=inst&7;
        rm=(inst>>3)&7;
        printf("eor r%u,r%u",rd,rm);
        return;
    }
    if((inst&0xF800)==0xC800)
    {
        rm=inst&0xFF;
        rn=(inst>>3)&7;
        printf("ldmia r%u!,{",rn);
        for(rx=0;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                printf("r%u",rx);
                break;
            }
        }
        for(rx++;rx<8;rx++)
        {
            if(rm&(1<<rx))
            {
                printf(",r%u",rx);
            }
        }
        printf("}");
        return;
    }


    printf("UNKNOWN");
}
//-------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    int ret;
    unsigned int ra;
    unsigned int rb;
    unsigned int inst;
    unsigned int inst2;

    if(argc!=2)
    {
        printf("mas filename\n");
        return(1);
    }

    fpin=fopen(argv[1],"rt");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    ret=assemble();
    fclose(fpin);
    if(ret)
    {
        printf("failed %u\n",line);
        return(1);
    }

    for(ra=0;ra<nlabs;ra++)
    {
        printf("label%04u: [0x%08X] [%s] %u\n",ra,lab_struct[ra].addr,lab_struct[ra].name,lab_struct[ra].type);
    }

    sprintf(newline,"%s.hex",argv[1]);
    fpout=fopen(newline,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",newline);
        return(1);
    }

    for(ra=0;ra<=ADDMASK;ra++)
    {
        curradd=(ra<<1);
        if(mark[ra]&0x8000)
        {
            printf("0x%08X: 0x%04X ",curradd,mem[ra]);
            dissassemble(mem[ra]);
            printf("\n");

            //rb=0x04;
            //rb+=(curradd>>8)&0xFF;
            //rb+=(curradd>>0)&0xFF;
            //rb+=0x00;
            //rb+=(mem[ra]>>24)&0xFF;
            //rb+=(mem[ra]>>16)&0xFF;
            //rb+=(mem[ra]>> 8)&0xFF;
            //rb+=(mem[ra]>> 0)&0xFF;
            //rb=(-rb)&0xFF;
            //fprintf(fpout,":%02X%04X%02X%08X%02X\n",0x04,curradd&0xFFFF,0x00,mem[ra],rb);
        }
    }

    fclose(fpout);


    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


