

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

unsigned char data[16384];


int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    if(argc<3)
    {
        printf("bintoh infile outfile\n");
        return(1);
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    fp=fopen(argv[2],"wt");
    if(fp==NULL)
    {
        printf("Error creating [%s]\n",argv[2]);
        return(1);
    }
    fprintf(fp,"\n");
    fprintf(fp,"#define TEST_DATA_LEN %u\n",rb);
    fprintf(fp,"const unsigned char test_data[TEST_DATA_LEN]=\n");
    fprintf(fp,"{");
    for(ra=0;ra<rb;ra++)
    {
        if((ra&7)==0) fprintf(fp,"\n  ");
        fprintf(fp,"0x%02X,",data[ra]);
    }
    fprintf(fp,"\n");
    fprintf(fp,"};\n");
    fprintf(fp,"\n");
    fclose(fp);
    return(0);
}

