
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//void PUT32 ( unsigned int, unsigned int);

unsigned int ROR ( unsigned int rd, unsigned int rs );
unsigned int ROR_FLAG ( unsigned int rd, unsigned int rs );
unsigned int LSR ( unsigned int rd, unsigned int rs );
unsigned int LSR_FLAG ( unsigned int rd, unsigned int rs );
unsigned int LSL ( unsigned int rd, unsigned int rs );
unsigned int LSL_FLAG ( unsigned int rd, unsigned int rs );

void hexstring ( unsigned int d );

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
int regression ( void )
{
    unsigned int rb;
    unsigned int rc;
    unsigned int prand;
    unsigned int sum;

    prand=0x12345678;
    //for(rb=0;rb<5;rb++)
    //{
        //prand=prand32(prand);
        //hexstring(prand);
    //}
    sum=0;
    for(rb=0;rb<5000;rb++)
    {
        prand=prand32(prand);
        rc=ROR(prand,rb);
        //PUT32(0xD0000000,rc);
        //hexstring(rc);
        sum+=rc;
        rc=ROR_FLAG(prand,rb);
        sum+=rc;
    }
    for(rb=0;rb<5000;rb++)
    {
        prand=prand32(prand);
        rc=LSR(prand,rb);
        sum+=rc;
        rc=LSR_FLAG(prand,rb);
        sum+=rc;
    }
    for(rb=0;rb<5000;rb++)
    {
        prand=prand32(prand);
        rc=LSL(prand,rb);
        sum+=rc;
        rc=LSL_FLAG(prand,rb);
        sum+=rc;
    }

    hexstring(sum);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

