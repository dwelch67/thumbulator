

void PUT32 ( unsigned int, unsigned int);

#define THUL_UART_BASE 0xE0000000
//------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    PUT32(THUL_UART_BASE+0x0,c);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x0D);
    uart_putc(0x0A);
}

extern unsigned short ult ( unsigned short a, unsigned short b);
extern unsigned short slt ( short a, short b);

extern unsigned short ulte ( unsigned short a, unsigned short b);
extern unsigned short slte ( short a, short b);

extern unsigned short ugt ( unsigned short a, unsigned short b);
extern unsigned short sgt ( short a, short b);

extern unsigned short ugte ( unsigned short a, unsigned short b);
extern unsigned short sgte ( short a, short b);


unsigned short limit_test ( void )
{
    unsigned short ra;
    short sa;

    for(ra=0;;ra++) if(ult(ra,7)==0) break;
    hexstring(ra);
    if(ra!=7) return(1);

    for(ra=0xF000;;ra++) if(ult(ra,0xF007)==0) break;
    hexstring(ra);
    if(ra!=0xF007) return(1);

    for(sa=-7;;sa++) if(slt(sa,7)==0) break;
    hexstring(sa);
    if(sa!=7) return(1);

    for(sa=-17;;sa++) if(slt(sa,-7)==0) break;
    hexstring(sa);
    if(sa!=-7) return(1);

    for(ra=0;;ra++) if(ulte(ra,7)==0) break;
    hexstring(ra);
    if(ra!=8) return(1);

    for(ra=0xF000;;ra++) if(ulte(ra,0xF007)==0) break;
    hexstring(ra);
    if(ra!=0xF008) return(1);

    for(ra=0xF000;;ra++) if(ulte(ra,7)==0) break;
    hexstring(ra);
    if(ra!=0xF000) return(1);

    for(sa=-7;;sa++) if(slte(sa,7)==0) break;
    hexstring(sa);
    if(sa!=8) return(1);

    for(sa=-17;;sa++) if(slte(sa,-7)==0) break;
    hexstring(sa);
    if(sa!=-6) return(1);

    for(ra=0;;ra++) if(ugt(ra,7)) break;
    hexstring(ra);
    if(ra!=8) return(1);

    for(ra=0xF000;;ra++) if(ugt(ra,0xF007)) break;
    hexstring(ra);
    if(ra!=0xF008) return(1);

    for(ra=0;;ra++) if(ugte(ra,7)) break;
    hexstring(ra);
    if(ra!=7) return(1);

    for(ra=0xF000;;ra++) if(ugte(ra,0xF007)) break;
    hexstring(ra);
    if(ra!=0xF007) return(1);

    return(0);

}
unsigned short notmain ( void )
{
    hexstring(0x1234);

    if(limit_test()) hexstring(0xBAD);

    hexstring(0x1234);
    return(0);
}


