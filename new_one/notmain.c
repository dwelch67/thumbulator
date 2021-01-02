void PUT32 ( unsigned int, unsigned int );
void dummy ( unsigned int );

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

int notmain ( void )
{
	unsigned int ra;

	//for(ra=0;ra<50;ra++) dummy(ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,100-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,7);

	for(ra=0;;ra++) dummy(ra);

	return(0);
}
