
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "testdata.h"

unsigned int edata[TESTDATALEN];

void hexstring ( unsigned int d ); //printf("%08X\n");
void hexstrings ( unsigned int d ); //printf("%08X ");
void hexstringcr ( unsigned int d ); //printf("%08X\r");


/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */

void encipher(unsigned int num_rounds, unsigned int v[2], unsigned int const key[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void decipher(unsigned int num_rounds, unsigned int v[2], unsigned int const key[4]) {
    unsigned int i;
    unsigned int v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}







int run_tea_test ( void )
{
    unsigned int ra;
    unsigned int errors;
    unsigned int data[2];
    unsigned int key[4];

    errors=0;

    key[0]=0x11111111;
    key[1]=0x22222222;
    key[2]=0x33333333;
    key[3]=0x44444444;

    hexstring(0x12345678);

    if((TESTDATALEN&1))
    {
        hexstring(0xBADBAD00);
        return(1);
    }

    for(ra=0;ra<TESTDATALEN;ra+=2)
    {
        data[0]=testdata[ra+0];
        data[1]=testdata[ra+1];
        encipher(32,data,key);
        edata[ra+0]=data[0];
        edata[ra+1]=data[1];
    }

    for(ra=0;ra<TESTDATALEN;ra+=2)
    {
        data[0]=edata[ra+0];
        data[1]=edata[ra+1];
        decipher(32,data,key);
        if(data[0]!=testdata[ra+0])
        {
            errors++;
            hexstrings(ra); hexstrings(data[0]); hexstring(testdata[ra+0]);
        }
        if(data[1]!=testdata[ra+1])
        {
            errors++;
            hexstrings(ra); hexstrings(data[1]); hexstring(testdata[ra+1]);
        }
        if(errors>20) break;
    }
    hexstring(errors);
    if(errors)
    {
        hexstring(0xBADBAD99);
        return(1);
    }
    hexstring(0x12345678);
    return(0);
}

