

unsigned int atest ( unsigned int, unsigned int );
unsigned int stest ( unsigned int, unsigned int );




//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int);
#define THUL_UART_BASE 0xE0000000
//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    PUT32(THUL_UART_BASE+0x0,c);
}
//------------------------------------------------------------------------


void show_op ( unsigned int x )
{
    if(x&2) uart_send(0x31); else uart_send(0x30);
    if(x&1) uart_send(0x31); else uart_send(0x30);
    uart_send(0x20);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int re;


    uart_send(0x0D);
    uart_send(0x0A);
    uart_send(0x0A);

    rd=0x0432;
    re=0x0137;

    for(ra=0;ra<4;ra++)
    {
        for(rb=0;rb<4;rb++)
        {
            rc=atest(ra<<30,rb<<30);
            show_op(ra);
            show_op(rb);
            if(rc&1) uart_send(0x31); else uart_send(0x30);
            if(rd&0x8000) uart_send(0x31); else uart_send(0x30);
            rd<<=1;
            uart_send(0x20);
            if(rc&2) uart_send(0x31); else uart_send(0x30);
            if(re&0x8000) uart_send(0x31); else uart_send(0x30);
            re<<=1;
            uart_send(0x0D);
            uart_send(0x0A);
        }
    }

    uart_send(0x0D);
    uart_send(0x0A);
    uart_send(0x0A);

    rd=0x2340;
    re=0x8CEF;

    for(ra=0;ra<4;ra++)
    {
        for(rb=0;rb<4;rb++)
        {
            rc=stest(ra<<30,rb<<30);
            show_op(ra);
            show_op(rb);
            if(rc&1) uart_send(0x31); else uart_send(0x30);
            if(rd&0x8000) uart_send(0x31); else uart_send(0x30);
            rd<<=1;
            uart_send(0x20);
            if(rc&2) uart_send(0x31); else uart_send(0x30);
            if(re&0x8000) uart_send(0x31); else uart_send(0x30);
            re<<=1;
            rc=ra-rb;
            if(rc&4) uart_send(0x30); else uart_send(0x31);
            uart_send(0x0D);
            uart_send(0x0A);
        }
    }

    uart_send(0x0D);
    uart_send(0x0A);
    uart_send(0x0A);










    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//0x0432
//0x0137


//00 00 0 0
//00 01 0 0
//00 10 0 0
//00 11 0 0

//01 00 0 0
//01 01 1 0
//01 10 0 0
//01 11 0 1

//10 00 0 0
//10 01 0 0
//10 10 1 1
//10 11 1 1

//11 00 0 0
//11 01 0 1
//11 10 1 1
//11 11 0 1


//0x2340
//0x8CEF

//00 00 0 1
//00 01 0 0
//00 10 1 0
//00 11 0 0

//01 00 0 1
//01 01 0 1
//01 10 1 0
//01 11 1 0

//10 00 0 1
//10 01 1 1
//10 10 0 1
//10 11 0 0

//11 00 0 1
//11 01 0 1
//11 10 0 1
//11 11 0 1

//cmn
//00 00 0 0
//00 01 0 0
//00 10 0 0
//00 11 0 0

//01 00 0 0
//01 01 1 0
//01 10 0 0
//01 11 0 1

//10 00 0 0
//10 01 0 0
//10 10 1 1
//10 11 1 1

//11 00 0 0
//11 01 0 1
//11 10 1 1
//11 11 0 1

