
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int);
unsigned int GET32 ( unsigned int );

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018

#define DEBUG_ADDR 0xD0000020

//*********************************************************
//  THIS IS AN INTERRUPT HANDLER, DONT MESS AROUND
unsigned int twoled;
void systick_handler ( void )
{
    GET32(STCTRL); //clear pending interrupt
    if(twoled&1)
    {
        PUT32(DEBUG_ADDR,0x11111111);
        PUT32(DEBUG_ADDR,GET32(STCURR));
    }
    else
    {
        PUT32(DEBUG_ADDR,0x22222222);
    }
    twoled++;
}
//*********************************************************

//------------------------------------------------------------------------
int notmain ( void )
{

if(1)
{
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,1000-1);
    PUT32(STCTRL,0x00000007); //interrupt enabled
    while(1) continue;
}
else
{
    unsigned int ra;

    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,1000-1);
    PUT32(STCTRL,0x00000005); //interrupt not enabled
    while(1)
    {
        if(GET32(STCTRL)&0x00010000)
        {
            ra++;
            PUT32(DEBUG_ADDR,ra);
            PUT32(DEBUG_ADDR,GET32(STCTRL));
        }

    }
}

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

