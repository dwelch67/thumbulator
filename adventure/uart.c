
void PUT32 ( unsigned int , unsigned int  );
unsigned int GET32 ( unsigned int );

#define THUL_UART_BASE 0xE0000000

//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    PUT32(THUL_UART_BASE+0x0,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    return(0x20);
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d, unsigned int cr )
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
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}
//-------------------------------------------------------------------
void uart_init(void)
{
    uart_putc('H');
    uart_putc('I');
    hexstring(0x12345678,1);
}
//-------------------------------------------------------------------
void uart_fifo_flush ( void )
{
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
