
//------------------------------------------------------------------------
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
unsigned int prand32 ( unsigned int x )
{
    if(x&1)
    {
        x=x>>1;
        x=x^0xBF9EC099;
    }
    else
    {
        x=x>>1;
    }
    return(x);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    volatile unsigned int ra;
    unsigned int rb;
    unsigned int prand;

    for(rb=0;rb<5;rb++)
    {
        uart_putc((rb&7)|0x30);
        for(ra=0;ra<0x321;ra++) continue ;
    }

    prand=0x12345678;
    for(rb=0;rb<5;rb++)
    {
        prand=prand32(prand);
        hexstring(prand);
    }

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

