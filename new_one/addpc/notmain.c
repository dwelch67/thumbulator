
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
unsigned int TEST ( void );

static void hexstring ( unsigned int d )
{
	PUT32(0xD0000020,d);
}

int notmain ( void )
{
	hexstring(TEST());
    return(0);
}


