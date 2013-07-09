
void hexstring ( unsigned int d ); //printf("%08X\n");
void hexstrings ( unsigned int d ); //printf("%08X ");
void hexstringcr ( unsigned int d ); //printf("%08X\r");

#include "picojpeg.h"
#include "jpegdata.h"

unsigned int doff;

unsigned char pjpeg_need_bytes_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data)
{
    unsigned int ra;
//   uint n;
  // pCallback_data;

//   n = min(g_nInFileSize - g_nInFileOfs, buf_size);
   //if (n && (fread(pBuf, 1, n, g_pInFile) != n))
      //return PJPG_STREAM_READ_ERROR;
   //*pBytes_actually_read = (unsigned char)(n);
   //g_nInFileOfs += n;

    //memcpy(pBuf,&jpegdata[doff],buf_size);
    for(ra=0;ra<buf_size;ra++) pBuf[ra]=jpegdata[doff+ra];
    doff+=buf_size;
    *pBytes_actually_read = buf_size;
    return 0;
}


int run_test ( void )
{
    unsigned char status;
    pjpeg_image_info_t pInfo;
    unsigned int sum;
    unsigned int rb;



    doff=0;

    status = pjpeg_decode_init(&pInfo, pjpeg_need_bytes_callback,(void *)0,0);

    if (status)
    {
        return(1);
    }
    sum=0;
    while(status==0)
    {
        status=pjpeg_decode_mcu();
        //printf("decode %u\n",ra);
        if(status==0)
        {
            switch(pInfo.m_scanType)
            {
                case PJPG_GRAYSCALE: break;
                case PJPG_YH1V1:
                {
                    for(rb=0;rb<64;rb++)
                    {
                        sum+=pInfo.m_pMCUBufB[rb];
                        sum+=pInfo.m_pMCUBufG[rb];
                        sum+=pInfo.m_pMCUBufR[rb];
                    }
                    //// PJPG_H1V1: Each MCU contains is decoded to a single block of 8x8 RGB pixels.
////   unsigned char *m_pMCUBufR;
                    //for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufR[rb];
                    //fwrite(pInfo.m_pMCUBufR,1,64,fpout);
                    //fwrite(pInfo.m_pMCUBufR,1,64,fp);
   ////unsigned char *m_pMCUBufG;
                    //for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufG[rb];
                    //fwrite(pInfo.m_pMCUBufG,1,64,fpout);
                    //fwrite(pInfo.m_pMCUBufG,1,64,fp);
   ////unsigned char *m_pMCUBufB;
                    //for(rb=0;rb<64;rb++) sum+=pInfo.m_pMCUBufB[rb];
                    //fwrite(pInfo.m_pMCUBufB,1,64,fpout);
                    //fwrite(pInfo.m_pMCUBufB,1,64,fp);
                    break;
                }
                case PJPG_YH2V1: break;
                case PJPG_YH1V2: break;
                case PJPG_YH2V2: break;
            }
        }
    }
    if(status!=PJPG_NO_MORE_BLOCKS)
    {
        return(1);
    }
    //printf("---- sum 0x%08X %u\n",sum,sum);
    hexstring(sum);
    return(0);
}

