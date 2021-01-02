
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
unsigned int TEST ( unsigned int );
unsigned int TEST2 ( unsigned int  );

static void hexstring ( unsigned int d )
{
	PUT32(0xD0000020,d);
}

int notmain ( void )
{
	hexstring(TEST(0));
	hexstring(TEST2(0));
    return(0);
}


