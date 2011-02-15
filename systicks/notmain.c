
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int);

unsigned int systicks;

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
unsigned int data[4096];
//------------------------------------------------------------------------
void systick_handler ( void )
{
    systicks++;
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb,rc;
    unsigned int prand;
    unsigned int prand_start;
    unsigned int errors;

    systicks=0;

    PUT32(0xE0010000,0x100);

    errors=0;
    prand=0x1234;
    prand=prand32(prand);
//    while(1)
    {
        prand_start=prand;
        for(ra=0;ra<4096;ra++)
        {
            prand=prand32(prand);
            data[ra]=prand;
        }
        prand=prand_start;
        for(ra=0;ra<4096;ra++)
        {
            prand=prand32(prand);
            rb=prand;
            rc=data[ra];
            if(rc!=rb)
            {
                errors++;
                hexstring(errors);
                hexstring(rc);
                hexstring(rb);
                PUT32(0xF0000000,0);
            }
        }
    }
    PUT32(0xE0010000,0x00);
    hexstring(systicks);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

