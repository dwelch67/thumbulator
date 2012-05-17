
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uart_init ( void )
{
}
void hexstring ( unsigned short d )
{
    printf("0x%04X\n",d);
}

extern int notmain ( void );

int main ( void )
{
    notmain();
}
