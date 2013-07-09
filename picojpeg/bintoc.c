
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fpin,*fpout;

unsigned int dlen;
unsigned char data[100000];

int main ( void )
{
    unsigned int ra;

    fpin=fopen("small2.jpg","rb");
    if(fpin==NULL) return(1);
    dlen=fread(data,1,sizeof(data),fpin);
    fclose(fpin);
    printf("%u bytes read\n",dlen);
    fpout=fopen("jpegdata.h","wt");
    if(fpout==NULL) return(1);
    fprintf(fpout,"\n");
    fprintf(fpout,"\n");
    fprintf(fpout,"const unsigned char jpegdata[]=\n");
    fprintf(fpout,"{\n");
    for(ra=0;ra<dlen;ra++) fprintf(fpout,"0x%02X,\n",data[ra]);
    fprintf(fpout,"};\n");
    fprintf(fpout,"\n");
    printf("Done.\n");
    return(0);
}

