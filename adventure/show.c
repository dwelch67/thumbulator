
#include "show.h"
#include "hdr.h"

void hexstring ( unsigned int d, unsigned int cr );

static unsigned int heap_off;
static unsigned int prand;

//static char heap_data[0x10000];

void show_init ( void )
{
    heap_off=0x40004000;
    prand=0x1234;
}

int not_atoi ( char *s )
{
    int x;

    x=0;
    while(*s)
    {
        x*=10;
        x+=(*s&0xF);
        s++;
    }
    return(x);
}

char  get_onechar ( void )
{
    char x;

    x=uart_getc();
    if(x==0x0D) x=0x0A;
    show_onechar(x);
    return(x);
}

void show_onechar ( char c )
{
    if(c==0x0A) uart_putc(0x0D);
    uart_putc(c);
}

void show_string ( char *s )
{
    while(*s)
    {
        show_onechar(*s);
        s++;
    }
}

void show_int ( int d )
{
    char s[16];
    int off;

    off=0;
    while(d)
    {
        s[off++]=d%10;
        d/=10;
    }
    if(off)
    {
        while(off)
        {
            off--;
            show_onechar(s[off]+0x30);
        }
    }
    else
    {
        show_onechar(0x30);
    }
}

void * not_malloc ( unsigned int len )
{
    void *ret;

//    ret=(void *)&heap_data[heap_off];
    ret=(void *)heap_off;
    heap_off+=len;

if(heap_off>=0x40007F00) hexstring(heap_off,1);
    return(ret);
}

void * tbuf_malloc ( unsigned int len )
{
    void *ret;

//    ret=(void *)&heap_data[heap_off];
    ret=(void *)heap_off;
if(heap_off>=0x40007F00) hexstring(heap_off,1);
    return(ret);
}




void copystr ( char *s, char *d )
{
    while(*s)
    {
        *d=*s;
        d++;
        s++;
    }
}

int weq ( char *a, char *b )
{
    int ra;

    for(ra=0;ra<5;ra++)
    {
        if(*a!=*b) return(0);
        if(*a==0) return(1);
        a++;
        b++;
    }
    return(1);
}

int length ( const char *s )
{
    int ra;

    ra=0;
    while(*s)
    {
        ra++;
        s++;
    }
    return(ra+1);
}


void not_memcpy( char *d, char *s, int len)
{
    while(len--)
    {
        *d=*s;
        d++;
        s++;
    }
}

void err ( int i, char *s )
{
    if(s)
    {
        show_string(s);
    }
    while(1) continue;
}

void errx(int i,char *s)
{
    err(i,s);
}

unsigned int randy ( void )
{
    if(prand&1)
    {
        prand=prand>>1;
        prand=prand^0xBF9EC099;
    }
    else
    {
        prand=prand>>1;
    }
    return(prand);
}

