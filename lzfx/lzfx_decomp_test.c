
#include "def_data.h"


extern void uart_init ( void );
extern void hexstring ( unsigned short d );


int lzfx_decompress_tiny
(
    const unsigned char *ibuf,
    const unsigned int ilen,
    unsigned char *obuf,
    unsigned int *olen
);

unsigned int inf_data_len;
unsigned char inf_data[0x4000];

int lzfx_decompress_test ( void )
{
    int ret;
    unsigned int ra,rb;

    inf_data_len=sizeof(inf_data);
    ret=lzfx_decompress_tiny(def_data,DEF_DATA_LEN,inf_data,&inf_data_len);
    if(ret) return(ret);
    hexstring(inf_data_len);
    if(inf_data_len!=INF_DATA_LEN) return(1);
    rb=0; for(ra=0;ra<inf_data_len;ra++) rb+=inf_data[ra]; rb&=0xFFFF;
    hexstring(rb);
    if(rb!=INF_DATA_CHECKSUM) return(1);
    return(0);
}

int notmain ( void )
{
    uart_init();
    hexstring(0x1234);
    if(lzfx_decompress_test())
    {
        hexstring(0x0BAD);
    }
    hexstring(0x1234);
    return(0);
}

