/*
* Filename "thumbulator.c "
* 
* project Thumbulator_x86
* 
* version 2.1
**/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define DBUGFETCH   0 
#define DBUGRAM     0
#define DBUGRAMW    0
#define DBUGREG     0
#define DBUG        0
#define DISS        1

FILE *fpvcd;

/*
* Mask for select instuction
*/
#define INST_GROUP_MAX 19

/* -----------------------------------------------
*     
*  -----------------------------------------------
*/
#define CPSR_N (1UL<<31)
#define CPSR_Z (1UL<<30)
#define CPSR_C (1UL<<29)
#define CPSR_V (1UL<<28)
#define CPSR_Q (1UL<<27)

/* ------------------------------------------------------------------------------
*  Macros operation fields
*  ------------------------------------------------------------------------------
*/
/* bit manipulation macros */
#define BIT(x, bit) ((x) & (1 << (bit)))
#define BIT_SHIFT(x, bit) (((x) >> (bit)) & 1)
#define BITS(x, high, low) ((x) & (((1<<((high)-(low)+1))-1) << (low)))
#define BITS_SHIFT(x, high, low) (((x) >> (low)) & ((1<<((high)-(low)+1))-1))
#define BIT_SET(x, bit) (((x) & (1 << (bit))) ? 1 : 0)
#define ISNEG(x) BIT((x), 31)
#define ISPOS(x) (!(BIT(x, 31)))

/* ------------------------------------------------------------------------------
*  Prototype function
*  ------------------------------------------------------------------------------
*/
void dump_counters ( void );
void dump_registers(void);

uint32_t read32 ( uint32_t a);
uint32_t fetch16 ( uint32_t addr );
uint32_t fetch32 ( uint32_t addr );
uint32_t read_register ( uint8_t a);

/* */
void EncodeIC1 (void);
void EncodeIC2 (void);
void EncodeIC3 (void);
void EncodeIC4 (void);
void EncodeIC5 (void);
void EncodeIC6 (void);
void EncodeIC7 (void);
void EncodeIC8 (void);
void EncodeIC9 (void);
void EncodeIC10 (void);
void EncodeIC11 (void);
void EncodeIC12 (void);
void EncodeIC13 (void);
void EncodeIC14 (void);
void EncodeIC15 (void);
void EncodeIC16 (void);
void EncodeIC17 (void);
void EncodeIC18 (void);
void EncodeIC19 (void);

/* */
int execute ( void );
int reset ( void );
int run ( void );

/*
* Constants/macros/typdefs
*/
typedef void (*FuncPtrType)(void);

typedef struct setgroup_st  {
  uint16_t mask_or;
  uint16_t mask_xor;
  FuncPtrType selgroup;  
} setgroup_st_t;

/*
* Ïàìÿòü ìèêðîêîíòðîëëåðà
*/
#define ROMADDMASK 0xFFFFFUL
#define RAMADDMASK 0xFFFFFUL

#define ROMSIZE (ROMADDMASK+1)
#define RAMSIZE (RAMADDMASK+1)

/*
* Ïàìÿòü ìèêðîêîíòðîåðà cpu
*/
uint16_t rom[ROMSIZE>>1]; 
uint16_t ram[RAMSIZE>>1];

/*
* Ðåãèñòðû ìèêðîêîíòðîëëåðà cpu
*/
uint32_t reg_norm[16]; /* normal execution mode, do not have a thread mode */

uint32_t cpsr;
uint32_t systick_ints;
uint32_t systick_ctrl;
uint32_t systick_reload;
uint32_t systick_count;
uint32_t systick_calibrate;

uint32_t vcdcount;
uint32_t output_vcd;


uint32_t halfadd;
uint32_t handler_mode;

/*
* Ìåòðèêà ðàáîòû êîäà ïðîãðàììû 
*/
uint32_t instructions;
uint32_t fetches;
uint32_t reads;
uint32_t writes;


/*
* Ñåëåêò ãðóïï èíñòðóêöèé 
*/
static const setgroup_st_t instformat[INST_GROUP_MAX] = {
  { .mask_or = 0x00FFU, .mask_xor = 0xB0FFU, .selgroup = EncodeIC13},
  { .mask_or = 0x00FFU, .mask_xor = 0xDFFFU, .selgroup = EncodeIC17},
  { .mask_or = 0x03FFU, .mask_xor = 0x43FFU, .selgroup = EncodeIC4},
  { .mask_or = 0x03FFU, .mask_xor = 0x47FFU, .selgroup = EncodeIC5},
  { .mask_or = 0x09FFU, .mask_xor = 0xBDFFU, .selgroup = EncodeIC14},
  { .mask_or = 0x07FFU, .mask_xor = 0x1FFFU, .selgroup = EncodeIC2},
  { .mask_or = 0x07FFU, .mask_xor = 0x4FFFU, .selgroup = EncodeIC6},
  { .mask_or = 0x0DFFU, .mask_xor = 0x5DFFU, .selgroup = EncodeIC7},
  { .mask_or = 0x0FFFU, .mask_xor = 0x5FFFU, .selgroup = EncodeIC8},
  { .mask_or = 0x07FFU, .mask_xor = 0xE7FFU, .selgroup = EncodeIC18},
  { .mask_or = 0x0FFFU, .mask_xor = 0x8FFFU, .selgroup = EncodeIC10},
  { .mask_or = 0x0FFFU, .mask_xor = 0x9FFFU, .selgroup = EncodeIC11},
  { .mask_or = 0x0FFFU, .mask_xor = 0xAFFFU, .selgroup = EncodeIC12},
  { .mask_or = 0x0FFFU, .mask_xor = 0xCFFFU, .selgroup = EncodeIC15},
  { .mask_or = 0x0FFFU, .mask_xor = 0xDFFFU, .selgroup = EncodeIC16},
  { .mask_or = 0x0FFFU, .mask_xor = 0xFFFFU, .selgroup = EncodeIC19},
  { .mask_or = 0x1FFFU, .mask_xor = 0x1FFFU, .selgroup = EncodeIC1},
  { .mask_or = 0x1FFFU, .mask_xor = 0x3FFFU, .selgroup = EncodeIC3},
  { .mask_or = 0x1FFFU, .mask_xor = 0x7FFFU, .selgroup = EncodeIC9}  
}; 

/*
*  Òèï ðåàëèçàöèè ýìóëÿöèè èíñòðóêöèé
*/
uint8_t ic_soft_hard = 0; /* =0 ïðîãðàììíàÿ
                          *  =1 àïïàðàòíàÿ
                          */

/*
* Òåêóùåå ñîñòîÿíèå ìèêðîêíòðîëëåðà cpu
*/
uint8_t cpu_status = 0;   /* ñòàòóñ cpu */
uint16_t ic;  /* ntreofz bycnherwbz */ 
uint16_t err = 0;  /* ôëàãè îøèáêè */
uint32_t pc = 0;
uint32_t sp = 0;

//-------------------------------------------------------------------
void dump_counters ( void )
{
    fprintf(stdout,"\n\n");
    fprintf(stdout,"instructions %lu\n",instructions);
    fprintf(stdout,"fetches      %lu\n",fetches);
    fprintf(stdout,"reads        %lu\n",reads);
    fprintf(stdout,"writes       %lu\n",writes);
    fprintf(stdout,"memcycles    %lu\n",fetches+reads+writes);
    fprintf(stdout,"systick_ints %lu\n",systick_ints);
}
 
/*
* Îïåðàöèè ñ ïàìÿòüþ è ðåãèñòðàìè
*/
/*-------------------------------------------------------------------*/
uint32_t fetch16 ( uint32_t addr )
{
    uint32_t data;

    fetches++;

if(DBUGFETCH>0) fprintf(stdout,"fetch16(0x%08X)=",addr);
if(DBUG>0) fprintf(stdout,"fetch16(0x%08X)=",addr);

    switch( addr & 0xF0000000UL )
    {
        case 0x00000000UL: /* ROM */
            addr &= (uint32_t)ROMADDMASK;
            
//if(addr<0x50)
//{
//    fprintf(stdout,"fetch16(0x%08X), abort\n",addr);
//    exit(1);
//}
            addr >>= 1U;
            data = rom[addr];

if(DBUGFETCH>0) fprintf(stdout,"0x%04X\n",data);
if(DBUG>0) fprintf(stdout,"0x%04X\n",data);

            break;           
        case 0x40000000UL: /* RAM */
            addr &= (uint32_t)(RAMADDMASK);
            addr >>= 1U;
            data = ram[addr];

if(DBUGFETCH>0) fprintf(stdout,"0x%04X\n",data);
if(DBUG>0) fprintf(stdout,"0x%04X\n",data);

        break;    
    default:
	  err |= 1U<<0; /* ERR */
      fprintf(stdout,"fetch16(0x%08X), abort pc = 0x%04X\n",addr,read_register(15));
	  exit(1);
      break;
    }
    return(data);    
}
/*-------------------------------------------------------------------*/
uint32_t fetch32 ( uint32_t addr )
{
    uint32_t data;

if(DBUGFETCH>0) fprintf(stdout,"fetch32(0x%08X)=",addr);
if(DBUG>0) fprintf(stdout,"fetch32(0x%08X)=",addr);

    switch( addr & 0xF0000000UL )
    {
        case 0x00000000UL: /*ROM*/
            if(addr < 0x50UL)
            {
                data = read32(addr);

if(DBUGFETCH>0) fprintf(stdout,"0x%08X\n",data);
if(DBUG>0) fprintf(stdout,"0x%08X\n",data);

                if( addr == 0x00000000UL ) return(data);
                if( addr == 0x00000004UL ) return(data);
                if( addr == 0x0000003CUL ) return(data);
                
               err |= (1U<<1); /* ERR */
               fprintf(stdout,"fetch32(0x%08X), abort pc = 0x%04X\n",addr,read_register(15));
               exit(1);
            }
            break;
        case 0x40000000UL: /*RAM*/
            //data=fetch16(addr+0);
            //data|=((unsigned int)fetch16(addr+2))<<16;
            data = read32(addr);

if(DBUGFETCH>0) fprintf(stdout,"0x%08X\n",data);
if(DBUG>0) fprintf(stdout,"0x%08X\n",data);
            
            break;
          default:
            err |= (1U<<2); /* ERR */
    		fprintf(stdout,"fetch32(0x%08X), abort pc 0x%04X\n",addr,read_register(15));
    		exit(1);          
          break;
    }
    return(data);
}
/*-------------------------------------------------------------------*/
void write16 ( uint32_t addr, uint32_t data )
{
   writes++;

if(DBUG>0) fprintf(stdout,"write16(0x%08X,0x%04X)\n",addr,data);

   switch( addr & 0xF0000000UL )
    {
        case 0x40000000UL: /*RAM*/
if(DBUGRAM>0) fprintf(stdout,"write16(0x%08X,0x%04X)\n",addr,data);
			addr &= RAMADDMASK;
            addr >>= 1U;
            ram[addr] = data & 0xFFFFUL;
            break;
        default:
          err |= (1U<<3); /* ERR */
    	  fprintf(stdout,"write16(0x%08X,0x%04X), abort pc 0x%04X\n",addr,data,read_register(15));
    	  exit(1);               
        break;        
    }
    return;
}
/*-------------------------------------------------------------------*/
void write32 ( uint32_t addr, uint32_t data )
{

if(DBUG>0) fprintf(stdout,"write32(0x%08X,0x%08X)\n",addr,data);

    switch( addr & 0xF0000000UL )
    {
        case 0xF0000000UL: /*halt*/
            dump_counters();
            cpu_status = 2; /* STOP */
            exit(0);
        break;    
        case 0xE0000000UL: /*periph*/
            switch( addr )
            {
                case 0xE0000000UL:
if(DISS>0) printf("uart: [");
					printf("%c",data&0xFF);
if(DISS>0) printf("]\n");
fflush(stdout);                   
                    break;

                case 0xE000E010UL:
                {
                    uint32_t old;

                    old = systick_ctrl;
                    systick_ctrl = data & 0x00010007UL;
                    if( ((old & 1)==0U) && ((systick_ctrl & 1U)>0U) )
                    {
                        /*timer started, load count*/
                        systick_count = systick_reload;
                    }
                    break;
                }
                case 0xE000E014UL:
                {
                    systick_reload = (uint32_t)(data & 0x00FFFFFFUL);
                    break;
                }
                case 0xE000E018UL:
                {
                    systick_count = (uint32_t)(data & 0x00FFFFFFUL);
                    break;
                }
                case 0xE000E01CUL:
                {
                    systick_calibrate = (uint32_t)(data & 0x00FFFFFFUL);
                    break;
                }
            }
            break;
        case 0xD0000000UL: /*debug*/
            switch(addr & 0xFFUL)
            {
                case 0x00UL:
                {
                    fprintf(stdout,"[0x%08X][0x%08X] 0x%08X\n",read_register(14),addr,data);
                    return;
                }
                case 0x10UL:
                {
                    printf("0x%08X ",data);                	
                    return;
                }
                case 0x20UL:
                {
                    printf("0x%08X\n",data);
                    return;
                }
            }
            break;
        case 0x40000000UL: /*RAM*/

if(DBUGRAMW>0) fprintf(stdout,"write32(0x%08X,0x%08X)\n",addr,data);

            write16(addr + 0UL,(data>>0) & 0xFFFFUL);
            write16(addr + 2UL,(data>>16) & 0xFFFFUL);
            return;
        default:
          err |= (1U<<4); /* ERR */
    	  fprintf(stdout,"write32(0x%08X,0x%08X), abort pc 0x%04X\n",addr,data,read_register(15));
          exit(1);           
        break;
    }
    return;
}
/*-----------------------------------------------------------------*/
uint32_t read16 ( uint32_t addr )
{
    uint32_t data;

    reads++;

if(DBUG>0) fprintf(stdout,"read16(0x%08X)=",addr);

    switch( addr & 0xF0000000UL )
    {
        case 0x00000000UL: /*ROM*/
            addr &= ROMADDMASK;
            addr >>= 1U;
            data = rom[addr];
if(DBUG>0) fprintf(stdout,"0x%04X\n",data);            
            break;
        case 0x40000000UL: /*RAM*/
if(DBUGRAM>0) fprintf(stdout,"read16(0x%08X)=",addr);
            addr &= RAMADDMASK;
            addr >>= 1U;
            data = ram[addr];
if(DBUG>0) fprintf(stdout,"0x%04X\n",data);
if(DBUGRAM>0) fprintf(stdout,"0x%04X\n",data);            
            break;
        default:
          err |= (1U<<5); /* ERR */
          fprintf(stdout,"read16(0x%08X), abort pc 0x%04X\n",addr,read_register(15));
          exit(1);
        break;
    }
    return(data);    
}
/*-------------------------------------------------------------------*/
uint32_t read32 ( uint32_t addr )
{
    uint32_t data;

if(DBUG>0) fprintf(stdout,"read32(0x%08X)=",addr);

    switch( addr & 0xF0000000UL )
    {
        case 0x00000000UL: /*ROM*/
        case 0x40000000UL: /*RAM*/ 
        {

if(DBUGRAMW>0) fprintf(stdout,"read32(0x%08X)=",addr);
            
            data = read16( addr + 0U );
            data |= ((uint32_t)read16(addr + 2U))<<16U;

if(DBUG>0) fprintf(stdout,"0x%08X\n",data);
if(DBUGRAMW>0) fprintf(stdout,"0x%08X\n",data);

        }    
        break;   
        case 0xE0000000UL:
        {
            switch(addr)
            {
                case 0xE000E010UL:
                {
                    data = systick_ctrl;
                    systick_ctrl &= (~0x00010000UL);
                }
                break;
                case 0xE000E014UL:
                {
                    data = systick_reload;
                }
                break;
                case 0xE000E018UL:
                {
                    data = systick_count;
                }
                break;
                case 0xE000E01CUL:
                {
                    data = systick_calibrate;                    
                }
                break;
                default:
                  err |= (1U<<6); /* ERR */
    			  fprintf(stdout,"read32(0x%08X), abort pc 0x%04X\n",addr,read_register(15));
                  exit(1);                   
                break;
            }
        break;    
        }
        default:
          err |= (1U<<7); /* ERR */
          fprintf(stdout,"read32(0x%08X), abort pc 0x%04X\n",addr,read_register(15));
          exit(1);
        break;  
    }
    return(data);
}
/*-------------------------------------------------------------------*/
uint32_t read_register ( uint8_t reg )
{
    uint32_t data;

    reg &= 0xFU;

if(DBUG>0) fprintf(stdout,"read_register(%u)=",reg);
if(DBUGREG>0) fprintf(stdout,"read_register(%u)=",reg);

    data = reg_norm[reg];
    if(reg == 15U)
    {
        if( (data & 1U)>0 )
        {
            fprintf(stdout,"pc has lsbit set 0x%08X\n",data);
        }
        data &= ~1UL;
    }

if(DBUG>0) fprintf(stdout,"0x%08X\n",data);
if(DBUGREG>0) fprintf(stdout,"0x%08X\n",data);

    return(data);
}
/*-------------------------------------------------------------------*/
void write_register ( uint8_t reg, uint32_t data )
{
    reg &= 0xFU;

if(DBUG>0) fprintf(stdout,"write_register(%u,0x%08X)\n",reg,data);
if(DBUGREG>0) fprintf(stdout,"write_register(%u,0x%08X)\n",reg,data);

    if(reg == 15U) data &= ~1U;
    reg_norm[reg] = data;

    if(output_vcd>0)
    {
        uint32_t vv;
    	fprintf(fpvcd,"b");        
        for(vv=0x80000000UL;vv;vv>>=1)
        {
	        if(vv & data) fprintf(fpvcd,"1"); else fprintf(fpvcd,"0");
        }
        fprintf(fpvcd," r%u\n",reg); 
    }
}
/*-------------------------------------------------------------------*/
void do_zflag ( uint32_t x )
{
    if( x == 0U ) cpsr |= CPSR_Z; else cpsr &= ~CPSR_Z;
}
/*-------------------------------------------------------------------*/
void do_nflag ( uint32_t x ) 
{
    if( (x & 0x80000000UL)>0 ) cpsr |= CPSR_N; else cpsr &= ~CPSR_N;
}
/*-------------------------------------------------------------------*/
void do_cflag ( uint32_t a, uint32_t b, uint32_t c )
{
    uint32_t rc;

    cpsr &= ~CPSR_C;
    rc = (a & 0x7FFFFFFFUL) + (b & 0x7FFFFFFFUL) + c; /*carry in*/
    rc = (rc>>31) + (a>>31) + (b>>31);  /*carry out*/
    if( (rc & 2UL)>0 ) cpsr |= CPSR_C;
}
/*-------------------------------------------------------------------*/
void do_vflag ( uint32_t a, uint32_t b, uint32_t c )
{
    uint32_t rc;
    uint32_t rd;

    cpsr &= ~CPSR_V;
    rc = (a & 0x7FFFFFFFUL) + (b & 0x7FFFFFFFUL) + c; /*carry in*/
    rc >>= 31; /*carry in in lsbit*/
    rd = (rc & 1U) + ((a>>31) & 1U) + ((b>>31) & 1U); /*carry out*/
    rd >>= 1; /*carry out in lsbit*/
    rc = (rc ^ rd) & 1U; /*if carry in != carry out then signed overflow*/
    if( rc > 0 ) cpsr|=CPSR_V;
}
/*-------------------------------------------------------------------*/
void do_cflag_bit ( uint32_t x )
{
   if( x > 0 ) cpsr |= CPSR_C; else cpsr &= ~CPSR_C;
}
/*-------------------------------------------------------------------*/
void do_vflag_bit ( uint32_t x )
{
   if( x > 0 ) cpsr |= CPSR_V; else cpsr &= ~CPSR_V;
}
/*-------------------------------------------------------------------*/
int reset ( void )
{
  /*
  *
  */
    memset(ram,0xFFU,sizeof(ram));

    systick_ctrl = 0x00000004UL;
    systick_reload = 0x00000000UL;
    systick_count = 0x00000000UL;
    systick_calibrate = 0x00ABCDEFUL;
    handler_mode = 0;
    cpsr = 0UL;

    reg_norm[13] = fetch32(0x00000000UL); //cortex-m
    reg_norm[14] = 0xFFFFFFFFul;
    reg_norm[15] = fetch32(0x00000004ul); //cortex-m
    if( (reg_norm[15] & 1UL) == 0U ) 
    {
        fprintf(stdout,"reset vector with an ARM address 0x%08X\n",reg_norm[15]);
        return(1); /* ERR */
    }
    reg_norm[15] &= ~1UL;
    reg_norm[15] += 2UL;

    /* Ñòàòèñòèêà */
    instructions = 0;
    fetches = 0;
    reads = 0;
    writes = 0;

    return(0);
}
/*-------------------------------------------------------------------*/
int run ( void )
{   
    err = 0;
    if( reset() > 0)
    {
      //viv- dbg совместимость с David return 1; /* ERR */
    }  
    
    while(1)
    {
        if(output_vcd > 0U)
        {
            fprintf(fpvcd,"#%u\n",vcdcount++);
        }
        if( execute()>0 ) break;
    }
    dump_counters();
    
    return(0);
}
/*-------------------------------------------------------------------*/
int execute ( void )
{
    uint8_t i;

//if(fetches>400000) return(1);

    pc = read_register(15U);

    if( handler_mode > 0 )
    {
        if( (pc & 0xF0000000ul) == 0xF0000000UL)
        {
            uint32_t sp;

            handler_mode = 0;
//fprintf(stdout,"--leaving handler\n");
            sp = read_register(13U);
            write_register(0,read32(sp)); sp += 4UL;
            write_register(1,read32(sp)); sp += 4UL;
            write_register(2,read32(sp)); sp += 4UL;
            write_register(3,read32(sp)); sp += 4UL;
            write_register(12,read32(sp)); sp += 4UL;
            write_register(14,read32(sp)); sp += 4UL;
            pc = read32(sp); sp += 4UL;
            cpsr = read32(sp); sp += 4UL;
            write_register(13,sp);
        }
    }
    if( (systick_ctrl & 1UL) > 0 )
    {
        if(systick_count > 0)
        {
            systick_count--;
        }
        else
        {
            systick_count = systick_reload;
            systick_ctrl |= 0x00010000UL;
        }
    }

    if( (systick_ctrl & 3UL) == 3UL )
    {
        if( (systick_ctrl & 0x00010000UL) > 0)
        {
            if(handler_mode == 0)
            {
                uint32_t sp;

                systick_ints++;
//fprintf(stdout,"--- enter systick handler\n");
                sp = read_register(13);
                sp -= 4UL; write32(sp,cpsr);
                sp -= 4UL; write32(sp,pc);
                sp -= 4UL; write32(sp,read_register(14));
                sp -= 4UL; write32(sp,read_register(12));
                sp -= 4UL; write32(sp,read_register(3));
                sp -= 4UL; write32(sp,read_register(2));
                sp -= 4UL; write32(sp,read_register(1));
                sp -= 4UL; write32(sp,read_register(0));
                write_register(13U,sp);
                pc = fetch32(0x0000003CUL); //systick vector
                pc += 2UL;
                //write_register(14,0xFFFFFF00UL);
                write_register(14,0xFFFFFFF9UL);

                handler_mode = 1U;
            }
        }
    }

    ic = fetch16(pc - 2U);
    pc += 2UL;
    write_register(15,pc);

if(DISS > 0) fprintf(stdout,"--- 0x%08X: 0x%04X ",(pc-4),ic);

  if( output_vcd > 0 )
  {
      uint32_t vv;
      fprintf(fpvcd,"b");
      for(vv=0x8000;vv;vv>>=1)
      {
          if( (vv & ic)>0 ) fprintf(fpvcd,"1"); else fprintf(fpvcd,"0");
      }
      fprintf(fpvcd," inst\n");
  }
  
  instructions++;

  i=0;
  do{
       if( ((ic | instformat[i].mask_or) ^ instformat[i].mask_xor ) == 0 ) {
            instformat[i].selgroup();
            break;
       }            
  } while(i++ < INST_GROUP_MAX) ;  

  /* */
  if(i == INST_GROUP_MAX) {
    return 1;  
  } 

  /* */
  if(err > 0) {
    return 1;     
  }    
  return 0;  
}

/* ------------------------------------------------------------------
*  Ðåàëèçàöèÿ èíñòðóêöèé
*  ------------------------------------------------------------------
*/
void EncodeIC1 (void) {
  /* 1. Move shifted unsigned long */  
  
  uint8_t Rd = BITS_SHIFT(ic,2,0);  
  uint8_t Rs = BITS_SHIFT(ic,5,3);  
  uint8_t Offset5 = BITS_SHIFT(ic,10,6);
  uint8_t Op = BITS_SHIFT(ic,12,11);
  uint32_t rs = 0;  
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  switch (Op){
    case 0: /* LSL Rd,Rs,#Offset5 */
      {
        if(DISS>0) fprintf(stdout,"lsls r%u,r%u,#0x%X\n",Rd,Rs,Offset5);
        
        rs = read_register(Rs);        
        if( Offset5 == 0U ) {
           /* C unnaffected
            * result not shifted
            */
        }
        else {
            /*else Offset5 > 0*/
            do_cflag_bit( rs & (1UL<<(32U-Offset5)) );
            rs <<= Offset5;
        }
        write_register(Rd,rs);
        do_nflag(rs);
        do_zflag(rs);        
      }
    break;
    case 1: // LSR Rd,Rs,#Offset5
      {
        if(DISS>0) fprintf(stdout,"lsrs r%u,r%u,#0x%X\n",Rd,Rs,Offset5);

        rs=read_register(Rs);
        if(Offset5 == 0) {
            do_cflag_bit(rs & 0x80000000UL);
            rs = 0;
        }
        else {
            do_cflag_bit(rs & (1UL<<(Offset5-1U)));
            rs >>= Offset5;
        }
        write_register(Rd,rs);
        do_nflag(rs);
        do_zflag(rs);
      }      
    break;
    case 2: // ASR Rd,Rs,#Offset5
      {
        if(DISS>0) fprintf(stdout,"asrs r%u,r%u,#0x%X\n",Rd,Rs,Offset5);        

        rs=read_register(Rs);
        if(Offset5 == 0U) {
            if( (rs & 0x80000000UL) > 0) {
                do_cflag_bit(1U);
                rs = (uint32_t)(~0UL);
            }
            else {
                do_cflag_bit(0U);
                rs = 0UL;
            }
        }
        else {
            do_cflag_bit(rs & (1UL<<(Offset5-1)));
            temp = rs & 0x80000000UL;
            rs >>= Offset5;
            if(temp > 0) {
              /*asr, sign is shifted in*/            
                rs |= (~0UL)<<(32U-Offset5);
            }
        }
        write_register(Rd,rs);
        do_nflag(rs);
        do_zflag(rs);        
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }
}
void EncodeIC2 (void) {
  /* 2. Add/subtract */  

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint8_t Rd = BITS_SHIFT(ic,2,0);
  uint8_t Rs = BITS_SHIFT(ic,5,3);
  uint8_t RnOffset3 = BITS_SHIFT(ic,8,6);  
  uint8_t I = BITS_SHIFT(ic,10,10);
  uint8_t Op = BITS_SHIFT(ic,9,9);
  uint32_t rs = 0;
  uint32_t rn = 0;
  uint32_t temp = 0;
  
  uint8_t s = (Op<<1) | I;  	
  
  switch (s){
    case 0: // ADD Rd, Rs, Rn
		{
        if(DISS>0) fprintf(stdout,"adds r%u,r%u,r%u\n",Rd,Rs,RnOffset3);
        
        rs = read_register(Rs);
        rn = read_register(RnOffset3);
        temp = rs + rn;
        write_register(Rd,temp);
        do_nflag(temp);
        do_zflag(temp);
        do_cflag(rs,rn,0U);
        do_vflag(rs,rn,0U);        
        }        
    break;
    case 1: // ADD Rd, Rs, #Offset3
      {
        if(DISS>0) fprintf(stdout,"adds r%u,r%u,#0x%X\n",Rd,Rs,RnOffset3);

        rs = read_register(Rs);
        temp = rs + RnOffset3;
        write_register(Rd,temp);
        do_nflag(temp);
        do_zflag(temp);
        do_cflag(rs,RnOffset3,0U);
        do_vflag(rs,RnOffset3,0U);            
      }
    break;
    case 2: // SUB Rd, Rs, Rn
        {
         if(DISS>0) fprintf(stdout,"subs r%u,r%u,r%u\n",Rd,Rs,RnOffset3);            

          rs = read_register(Rs);
          rn = read_register(RnOffset3);
          temp = rs - rn;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(temp,~rn,1U);
          do_vflag(temp,~rn,1U);          
        }        
    break;
    case 3: // SUB Rd, Rs, #RnOffset3
        {
         if(DISS>0) fprintf(stdout,"subs r%u,r%u,#0x%X\n",Rd,Rs,RnOffset3);
            
          rs = read_register(Rs);
          rn = RnOffset3;
          temp = rs - rn;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(temp,~rn,1U);
          do_vflag(temp,~rn,1U);          
        }        
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }
}
void EncodeIC3 (void) {  
  /* 3. Move/compare/add/subtract immediate */  

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint8_t Op = BITS_SHIFT(ic,12,11);
  uint8_t Rd = BITS_SHIFT(ic,10,8);  
  uint8_t Offset8 = BITS_SHIFT(ic,7,0);  
  uint32_t rd = 0;
  uint32_t temp = 0;
    
  switch (Op){
    case 0: // MOV Rd, #Offset8
        {
          if(DISS>0) fprintf(stdout,"movs r%u,#0x%02X\n",Rd,Offset8);  
          write_register(Rd,Offset8);
          do_nflag(Offset8);
          do_zflag(Offset8);
        }
    break;
    case 1: // CMP Rd, #Offset8
        {
          if(DISS>0) fprintf(stdout,"cmp r%u,#0x%02X\n",Rd,Offset8);

          rd = read_register(Rd);
          temp = rd - Offset8;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~Offset8,1U);
          do_vflag(rd,~Offset8,1U);
        }        
    break;
    case 2: // ADD Rd, #Offset8
        {
          if(DISS>0) fprintf(stdout,"adds r%u,#0x%02X\n",Rd,Offset8);

          rd = read_register(Rd);
          temp = rd + Offset8;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,Offset8,0U);
          do_vflag(rd,Offset8,0U);
        }
    break;
    case 3: // SUB Rd, #Offset8
        {
          if(DISS>0) fprintf(stdout,"subs r%u,#0x%02X\n",Rd,Offset8);  
          
          rd = read_register(Rd);
          temp = rd - Offset8;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~Offset8,1U);
          do_vflag(rd,~Offset8,1U);          
        }        
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC4 (void) {  
  /* 4. ALU operations */

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint8_t Rd = BITS_SHIFT(ic,2,0);  
  uint8_t Rs = BITS_SHIFT(ic,5,3);    
  uint8_t Op = BITS_SHIFT(ic,9,6);
  uint32_t rd = 0;  
  uint32_t rs = 0;  
  uint32_t temp = 0;

  switch (Op){
    case 0: // AND Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"ands r%u,r%u\n",Rd,Rs);
            
          rd = read_register(Rd);
          rs = read_register(Rs);
          temp = rd & rs;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
        }
    break;
    case 1: // EOR Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"eors r%u,r%u\n",Rd,Rs);
            
          rd = read_register(Rd);
          rs = read_register(Rs);
          temp = rd ^ rs;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);          
        }
    break;
    case 2: // LSL Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"lsls r%u,r%u\n",Rd,Rs);

          rd = read_register(Rd);
          rs = read_register(Rs);
          rs &= 0xFFU;
          if(rs == 0)
          {
          }
          else if(rs < 32U)
          {
              do_cflag_bit(Rd & (1UL<<(32U-rs)));
              rd <<= rs;
          }
          else if(rs == 32U)
          {
              do_cflag_bit(rd & 1UL);
              rd = 0U;
          }
          else
          {
              do_cflag_bit(0U);
              rd = 0U;
          }
          write_register(Rd,rd);
          do_nflag(rd);
          do_zflag(rd);
        }        
    break;
    case 3: // LSR Rd, Rs
        {
          if(DISS) fprintf(stdout,"lsrs r%u,r%u\n",Rd,Rs);
            
          rd = read_register(Rd);
          rs = read_register(Rs);
          rs &= 0xFFU;
          if(rs == 0UL)
          {
          }
          else if(rs < 32UL)
          {
              do_cflag_bit(rd & (1UL<<(rs-1U)));
              rd >>= rs;
          }
          else if(rs == 32UL)
          {
              do_cflag_bit(rd & 0x80000000UL);
              rd = 0UL;
          }
          else
          {
              do_cflag_bit(0U);
              rd = 0UL;
          }
          write_register(Rd,rd);
          do_nflag(rd);
          do_zflag(rd);            
        }
    break;
    case 4: // ASR Rd, Rs 
        {
          if(DISS>0) fprintf(stdout,"asrs r%u,r%u\n",Rd,Rs);            

          rd = read_register(Rd);
          rs = read_register(Rs);
          rs &= 0xFFUL;
          if(rs == 0UL)
          {
          }
          else if(rs < 32UL)
          {
              do_cflag_bit(rd & (1UL<<(rs-1U)));
              temp = rd & 0x80000000UL;
              rd >>= rs;
              if(temp>0) /*asr, sign is shifted in*/
              {
                  rd |= (~0UL)<<(32UL-rs);
              }
          }
          else
          {
              if( (rd & 0x80000000UL) > 0)
              {
                  do_cflag_bit(1U);
                  rd = (uint32_t)(~0UL);
              }
              else
              {
                  do_cflag_bit(0U);
                  rd = 0UL;
              }
          }
          write_register(Rd,rd);
          do_nflag(rd);
          do_zflag(rd);
        }       
    break;
    case 5: // ADC Rd, Rs
        {
  		  if(DISS>0) fprintf(stdout,"adc r%u,r%u\n",Rd,Rs);

          rd=read_register(Rd);
          rs=read_register(Rs);
          temp = rd + rs;
          if((cpsr & CPSR_C) > 0UL) temp++;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          if(cpsr & CPSR_C) { do_cflag(rd,rs,1U); do_vflag(rd,rs,1U); }
          else            { do_cflag(rd,rs,0U); do_vflag(rd,rs,0U); }
            
        }
    break;
    case 6: // SBC Rd, Rs 
        {
          if(DISS>0) fprintf(stdout,"sbc r%u,r%u\n",Rd,Rs);            

          rd = read_register(Rd);
          rs = read_register(Rs);
          temp = rd - rs;
          if(!(cpsr & CPSR_C)) temp--;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          if( (cpsr & CPSR_C) > 0)
          {
              do_cflag(rd,~rs,1U);
              do_vflag(rd,~rs,1U);
          }
          else
          {
              do_cflag(rd,~rs,0U);
              do_vflag(rd,~rs,0U);
          }
        }
    break;
    case 7: // ROR Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"rors r%u,r%u\n",Rd,Rs);
            
          rd=read_register(Rd);
          rs=read_register(Rs);
          rs &= 0xFFUL;
          if(rs == 0UL)
          {
          }
          else
          {
              rs &= 0x1FUL;
              if(rs == 0UL)
              {
                  do_cflag_bit(rd & 0x80000000UL);
              }
              else
              {
                  do_cflag_bit(rd & (1<<(rs-1)));
                  temp=rd<<(32U - rs);
                  rd >>= rs;
                  rd |= temp;
              }
          }
          write_register(Rd,rd);
          do_nflag(rd);
          do_zflag(rd);
        }
    break;
    case 8: // TST Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"tst r%u,r%u\n",Rd,Rs);
            
          rd = read_register(Rd);
          rs = read_register(Rs);
          temp = rd & rs;
          do_nflag(temp);
          do_zflag(temp);
        }  
    break;
    case 9: // NEG Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"negs r%u,r%u\n",Rd,Rs);
            
          rs = read_register(Rs);
          temp = 0 - rs;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(0U,~rs,1U);
          do_vflag(0U,~rs,1U);
        }        
    break;
    case 10: // CMP Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"cmps r%u,r%u\n",Rd,Rs);
          
          rd = read_register(Rd);  
          rs = read_register(Rs);
          temp = rd - rs;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~rs,1U);
          do_vflag(rd,~rs,1U);
        }        
    break;
    case 11: // CMN Rd, Rs
        {
		  if(DISS>0) fprintf(stdout,"cmns r%u,r%u\n",Rd,Rs);
            
          rd = read_register(Rd);
          rs = read_register(Rs);
          temp = rd + rs;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,rs,0U);
          do_vflag(rd,rs,0U);
        }        
    break;
    case 12: // ORR Rd, Rs
        {
		  if(DISS>0) fprintf(stdout,"orrs r%u,r%u\n",Rd,Rs);
          
          rd = read_register(Rd);  
          rs = read_register(Rs);
          temp = rd | rs;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
        }        
    break;
    case 13: // MUL Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"muls r%u,r%u\n",Rd,Rs);
          
          rd = read_register(Rd);  
          rs = read_register(Rs);
          temp = rd * rs;
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
        }        
    break;
    case 14: // BIC Rd, Rs
        {
		  if(DISS>0) fprintf(stdout,"bics r%u,r%u\n",Rd,Rs);
          
          rd = read_register(Rd);  
          rs = read_register(Rs);
          temp = rd & (~rs);
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
        }        
    break;
    case 15: // MVN Rd, Rs
        {
          if(DISS>0) fprintf(stdout,"mvns r%u,r%u\n",Rd,Rs);
            
          rs = read_register(Rs);
          temp = (~rs);
          write_register(Rd,temp);
          do_nflag(temp);
          do_zflag(temp);
        }        
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }
}
void EncodeIC5 (void) {  
  /* 5. Hi unsigned long operations/branch exchange */

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint8_t RdHd = BITS_SHIFT(ic,2,0);
  uint8_t RsHs = BITS_SHIFT(ic,5,3);  
  uint8_t  H2 = BITS_SHIFT(ic,6,6);  
  uint8_t  H1 = BITS_SHIFT(ic,7,7);  
  uint8_t  Op = BITS_SHIFT(ic,9,8);  
  uint8_t s = (Op<<2) | (H1<<1) | H2;
  uint32_t rd = 0;
  uint32_t rs = 0;
  uint32_t temp = 0;
  
  switch (s){
    case 1: // ADD Rd, Hs
        {
          if(DISS>0) fprintf(stdout,"add r%u,r%u\n",RdHd,RsHs+8);

          rd = read_register(RdHd);
          rs = read_register(RsHs+8);
          temp = rd + rs;
          if(RdHd == 15U)
          {
              if((temp & 1U) == 0U)
              {
                  //fprintf(stdout,"add pc,... produced an arm address 0x%08X 0x%08X\n",pc,rc); 
                  err |= (1U<<8); /* ERR */ 
              }
              else {
                temp &= ~1U; /*write_register may do this as well*/
                temp += 2U; /*The program counter is special*/
                write_register(RdHd,temp);  
              }
          }
          else {
            write_register(RdHd,temp);  
          }
        }
        break;
    case 2: // ADD Hd, Rs
        {
          if(DISS>0) fprintf(stdout,"add r%u,r%u\n",RdHd+8,RsHs);

          rd = read_register(RdHd+8);
          rs = read_register(RsHs);
          temp = rd + rs;
          if((RdHd+8) == 15U)
          {
              if((temp & 1U) == 0U)
              {
                  //viv- fprintf(stdout,"add pc,... produced an arm address 0x%08X 0x%08X\n",pc,rc);
                  err |= (1U<<8); /* ERR */ 
              }
              else {
                temp &= ~1U; /*write_register may do this as well*/
                temp += 2U; /*The program counter is special*/
                write_register(RdHd+8,temp);  
              }
          }
          else {
            write_register(RdHd+8,temp);  
          }
        }
        break;
    case 3: // ADD Hd, Hs
        {
          if(DISS>0) fprintf(stdout,"add r%u,r%u\n",RdHd+8,RsHs+8);

          rd = read_register(RdHd+8);
          rs = read_register(RsHs+8);
          temp = rd + rs;
          if((RdHd+8) == 15U)
          {
              if((temp & 1U) == 0U)
              {
                  /*viv- fprintf(stdout,"add pc,... produced an arm address 0x%08X 0x%08X\n",pc,rc); */
                  err |= (1U<<8); /* ERR */ 
              }
              else {
                temp &= ~1U; /*write_register may do this as well*/
                temp += 2U; /*The program counter is special*/
                write_register(RdHd+8,temp);  
              }
          }
          else {
            write_register(RdHd+8,temp);  
          }
        }
    break;
    case 5: // CMP Rd, Hs
        {
          if(DISS>0) fprintf(stdout,"cmps r%u,r%u\n",RdHd,RsHs+8);            

          rd = read_register(RdHd);
          rs = read_register(RsHs+8);
          temp = rd - rs;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~rs,1U);
          do_vflag(rd,~rs,1U);          
        }         
    break;
    case 6: // CMP Hd, Rs
        {
          if(DISS>0) fprintf(stdout,"cmps r%u,r%u\n",RdHd+8,RsHs);            

          rd=read_register(RdHd+8);
          rs=read_register(RsHs);
          temp = rd - rs;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~rs,1U);
          do_vflag(rd,~rs,1U);          
        }         
    break;
    case 7: // CMP Hd, Hs
        {
          if(DISS>0) fprintf(stdout,"cmps r%u,r%u\n",RdHd+8,RsHs+8);            

          rd=read_register(RdHd+8);
          rs=read_register(RsHs+8);
          temp = rd - rs;
          do_nflag(temp);
          do_zflag(temp);
          do_cflag(rd,~rs,1U);
          do_vflag(rd,~rs,1U);          
        }         
    break;
    case 9: // MOV Rd, Hs
        {
          if(DISS>0) fprintf(stdout,"movs r%u,r%u\n",RdHd,RsHs+8);            

          rd = read_register(RsHs+8);

          write_register(RdHd,rd);
          do_nflag(rd);
          do_zflag(rd);
          do_cflag_bit(0U);
          do_vflag_bit(0U);          
        }
    break;
    case 10: // MOV Hd, Rs
        {
          if(DISS>0) fprintf(stdout,"movs r%u,r%u\n",RdHd+8,RsHs);            

          rd=read_register(RsHs);

          write_register(RdHd+8,rd);
          do_nflag(rd);
          do_zflag(rd);
          do_cflag_bit(0U);
          do_vflag_bit(0U);

        rs=read_register(RsHs);
        if( ((RdHd+8)==14))
        {
            //printf("mov lr,pc warning 0x%08X\n",pc-2);
            //rc|=1;
        }
        if((RdHd+8)==15)
        {
            //if((rc&1)==0)
            //{
                //fprintf(stderr,"cpy or mov pc,... produced an ARM address 0x%08X 0x%08X\n",pc,rc);
                //exit(1);
            //}
            rs &= ~1; //write_register may do this as well
            rs += 2; //The program counter is special
        }
        write_register(RdHd+8,rs);
        do_nflag(rs);
        do_zflag(rs);
        do_cflag_bit(0U);
        do_vflag_bit(0U);

        }        
    break;
    case 11: // MOV Hd, Hs
        {
          if(DISS>0) fprintf(stdout,"movs r%u,r%u\n",RdHd+8,RsHs+8);            

        rs=read_register(RsHs+8);

        if(((RsHs+8)==14)&&((RdHd+8)==15))
        {
            //printf("mov lr,pc warning 0x%08X\n",pc-2);
            //rc|=1;
        }
        if((RsHs+8)==15)
        {
            //if((rc&1)==0)
            //{
                //fprintf(stderr,"cpy or mov pc,... produced an ARM address 0x%08X 0x%08X\n",pc,rc);
                //exit(1);
            //}
            rs &= ~1; //write_register may do this as well
            rs +=  2; //The program counter is special
        }
        write_register(RdHd+8,rs);
        do_nflag(rs);
        do_zflag(rs);
        do_cflag_bit(0U);
        do_vflag_bit(0U);          
        }
        break;
    case 12: // BX Rs
        {
          if(DISS>0) fprintf(stdout,"bx r%u\n",RsHs);            

          rs = read_register(RsHs);
          rs += 2;

          if( (rs & 1) > 0 )
          {
              rs &= ~1UL;
              write_register(15U,rs);              
          }
          else
          {
              /*viv- fprintf(stdout,"cannot branch to arm 0x%08X 0x%04X\n",pc,inst);*/
              err |= (1U<<9); /* ERR */
          }
        }
    break;
    case 13: // BX Hs
        {
          if(DISS>0) fprintf(stdout,"bx r%u\n",RsHs+8);            
          
          rs = read_register(RsHs+8);
          rs += 2;

          if( (rs & 1) > 0 )
          {
              rs &= ~1UL;
              write_register(15U,rs);              
          }
          else
          {
              /*viv- fprintf(stdout,"cannot branch to arm 0x%08X 0x%04X\n",pc,inst);*/
              err |= (1U<<9); /* ERR */
          }
        }        
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }
}
void EncodeIC6 (void) {
  /* 6. PC-relative load */  

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint16_t Word8 = BITS_SHIFT(ic,7,0);
  uint32_t Rd = BITS_SHIFT(ic,10,8);
  uint32_t val = Word8<<2U;
  uint32_t r15 = 0;
  uint32_t temp = 0;
  
  /* LDR Rd,[PC,#imm] */
  if(DISS>0) fprintf(stdout,"ldr r%u,[PC+#0x%X]\n",Rd,val);

  r15 = pc;
  r15 &= ~3UL;
  val += r15;

  temp = read32(val);
  write_register(Rd,temp);
}
void EncodeIC7 (void) {  
  /* 7. Load/store with unsigned long offset */

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  uint8_t Rd = BITS_SHIFT(ic,2,0);;  
  uint8_t Rb = BITS_SHIFT(ic,5,3);
  uint8_t Ro = BITS_SHIFT(ic,8,6);  
  uint8_t B  = BITS_SHIFT(ic,10,10);
  uint8_t L  = BITS_SHIFT(ic,11,11);
  uint8_t s  = (L<<1) | B;
  uint32_t rd = 0;
  uint32_t ro = 0;
  uint32_t val = 0;
  uint32_t temp = 0;

  switch (s){
    case 0: // STR Rd, [Rb, Ro]
      {
        if(DISS>0) fprintf(stdout,"str r%u,[r%u,r%u]\n",Rd,Rb,Ro);
        
        ro = read_register(Rb) + read_register(Ro);
        temp = read_register(Rd);
        write32(ro,temp);
      }      
    break;
    case 1: // STRB Rd, [Rb, Ro]
      {
        if(DISS>0) fprintf(stdout,"strb r%u,[r%u,r%u]\n",Rd,Rb,Ro);

        ro = read_register(Rb) + read_register(Ro);
        rd = read_register(Rd);
        temp = read16(ro & (~1UL));
        if((ro & 1U) > 0U)
        {
            temp &= 0x00FFUL;
            temp |= rd<<8;
        }
        else
        {
            temp &= 0xFF00UL;
            temp |= rd & 0x00FFUL;
        }
        write16(ro & (~1UL),temp & 0xFFFFU);
        
      }
    break;
    case 2: // LDR Rd, [Rb, Ro]
        {
          if(DISS>0) fprintf(stdout,"ldr r%u,[r%u,r%u]\n",Rd,Rb,Ro);

          val = read_register(Rb) + read_register(Ro);
          temp = read32(val);
          write_register(Rd,temp);
        }
    break;
    case 3: // LDRB Rd, [Rb, Ro]
        {
          if(DISS>0) fprintf(stdout,"ldrb r%u,[r%u,r%u]\n",Rd,Rb,Ro);            

          val = read_register(Rb) + read_register(Ro);
          temp = read16(Rd & (~1U));
          if( (val & 1UL) > 0U)
          {
              temp >>= 8U;
          }
          else
          {
          }
          write_register(Rd,temp & 0xFFU);
        }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }
}
void EncodeIC8 (void) {  
  /* 8. Load/store sign-extended byte/halfword */

  uint8_t Rd = BITS_SHIFT(ic,2,0);
  uint8_t Rb = BITS_SHIFT(ic,5,3);
  uint8_t Ro = BITS_SHIFT(ic,8,6);  
  uint8_t S = BITS_SHIFT(ic,10,10);  
  uint8_t H = BITS_SHIFT(ic,11,11);  
  uint32_t val = 0;
  uint32_t temp = 0;
    
  uint8_t s = (S<<1) | H;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
    
    switch (s){
    case 0: // STRH Rd, [Rb, Ro]
            {
            if(DISS>0) fprintf(stdout,"strh r%u,[r%u,r%u]\n",Rd,Rb,Ro);                

              val = read_register(Rb) + read_register(Ro);
              temp = read_register(Rd);
              write16(val,temp & 0xFFFFU);
            }                
    break;
    case 1: // LDRH Rd, [Rb, Ro]
      {
        if(DISS>0) fprintf(stdout,"ldrh r%u,[r%u,r%u]\n",Rd,Rb,Ro);

        val = read_register(Rb) + read_register(Ro);
        temp = read16(val);
        write_register(Rd,temp & 0xFFFFU);
      }      
    break;
    case 2: // LDSB Rd, [Rb, Ro]
      {
        if(DISS>0) fprintf(stdout,"ldsb r%u,[r%u,r%u]\n",Rd,Rb,Ro);        
        
        val = read_register(Rb) + read_register(Ro);
        temp = read16(val & (~1UL));
        if( (val & 1) > 0)
        {
            temp >>= 8;
        }
        else
        {
        }
        temp &= 0xFFU;
        if( (temp & 0x80U) > 0) temp |= (uint32_t)((~0UL)<<8);
        write_register(Rd,temp);        
      }      
    break;
    case 3: // LDSH Rd, [Rb, Ro]
      {
        if(DISS>0) fprintf(stdout,"ldsh r%u,[r%u,r%u]\n",Rd,Rb,Ro);        

        val = read_register(Rb) + read_register(Ro);
        temp = read16(val);
        temp &= 0xFFFFU;
        if( (temp & 0x8000U) > 0) temp |= (uint32_t)((~0UL)<<16);
        write_register(Rd,temp);        
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */ 
    break;
  }  
}
void EncodeIC9 (void) {  
  /* 9. Load/store with immediate offset */

  uint8_t Rd = BITS_SHIFT(ic,2,0);
  uint8_t Rb = BITS_SHIFT(ic,5,3);
  uint16_t Offset5 = BITS_SHIFT(ic,10,6);  
  uint8_t L = BITS_SHIFT(ic,11,11);  
  uint8_t B = BITS_SHIFT(ic,12,12);  
  uint32_t rd = 0;
  uint32_t val = 0;
  uint32_t temp = 0;

  uint8_t s = (L<<1) | B;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
 
  switch (s){
    case 0: // STR Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"str r%u,[r%u,#0x%X]\n",Rd,Rb,Offset5<<2);        

        val = read_register(Rb) + (Offset5<<2);
        temp = read_register(Rd);
        write32(val,temp);
      }      
    break;
    case 2: // LDR Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"ldr r%u,[r%u,#0x%X]\n",Rd,Rb, Offset5<<2);        

        val = read_register(Rb) + (Offset5<<2);
        temp = read32(val);
        write_register(Rd,temp);
      }      
    break;
    case 1: // STRB Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"strb r%u,[r%u,#0x%X]\n",Rd,Rb,Offset5);

        val = read_register(Rb) + Offset5;
        rd = read_register(Rd);
        temp = read16(val & (~1UL));
        if((val & 1U) > 0)
        {
            temp &= 0x00FFul;
            temp |= rd<<8;
        }
        else
        {
            temp &= 0xFF00ul;
            temp |= rd & 0x00FFU;
        }
        write16(val & (~1UL),temp & 0xFFFFU);
      }      
    break;
    case 3: // LDRB Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"ldrb r%u,[r%u,#0x%X]\n",Rd,Rb,Offset5);
        
        val = read_register(Rb) + Offset5;
        temp = read16(val & (~1UL));
        if((val & 1UL) > 0) 
        {
            temp >>= 8U;
        }
        else
        {
        }
        write_register(Rd,temp & 0xFFU);
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC10 (void) {  
  /* 10. Load/store halfword */

  uint8_t Rd = BITS_SHIFT(ic,2,0);
  uint8_t Rb = BITS_SHIFT(ic,5,3);
  uint16_t Offset5 = BITS_SHIFT(ic,10,6);  
  uint8_t L = BITS_SHIFT(ic,11,11);  
  uint32_t val = 0;
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  switch (L){
    case 0: // STRH Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"strh r%u,[r%u,#0x%X]\n",Rd,Rb,Offset5<<1);        

        val = read_register(Rb) + (Offset5<<1);
        temp = read_register(Rd);
        write16(val,temp & 0xFFFFU);
      }      
    break;
    case 1: // LDRH Rd, [Rb, #Imm]
      {
        if(DISS>0) fprintf(stdout,"ldrh r%u,[r%u,#0x%X]\n",Rd,Rb,Offset5<<1);        

        val = read_register(Rb) + (Offset5<<1);
        temp = read16(val);
        write_register(Rd,temp & 0xFFFFU);
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC11 (void) {
   /* 11. SP-relative load/store */
  
  uint16_t Word8 = BITS_SHIFT(ic,7,0);
  uint8_t Rd = BITS_SHIFT(ic,10,8);
  uint8_t L = BITS_SHIFT(ic,11,11);  
  uint32_t rd = 0;
  uint32_t r13 = 0;
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  switch (L){
    case 0: // STR Rd, [SP, #Imm]
      {
        if(DISS>0) fprintf(stdout,"str r%u,[SP,#0x%X]\n",Rd,Word8<<2);

        temp = read_register(13U) + (Word8<<2);
        rd = read_register(Rd);        
        write32(temp,rd);
      }      
    break;
    case 1: // LDR Rd, [SP, #Imm]
      {
        if(DISS>0) fprintf(stdout,"ldr r%u,[SP,#0x%X]\n",Rd,Word8<<2);

        temp = Word8<<2;
        r13 = read_register(13);
        temp += r13;
        temp = read32(temp);
        write_register(Rd,temp);        
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC12 (void) { 
  /* 12. Load address */

  uint16_t Word8 = BITS_SHIFT(ic,7,0);
  uint8_t Rd = BITS_SHIFT(ic,10,8);
  uint8_t SP = BITS_SHIFT(ic,11,11);  
  uint32_t r13 = 0;
  uint32_t r15 = 0;
  uint32_t val = 0;
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  switch (SP){
    case 0: // ADD Rd, PC, #Imm
      {
        if(DISS>0) fprintf(stdout,"add r%u,PC,#0x%02X\n",Rd,Word8<<2);

        val =  Word8<<2;
        r15 = pc;
        temp = (r15 & (~3UL)) + val;
        write_register(Rd,temp);        
      }      
    break;
    case 1: // ADD Rd, SP, #Imm
      {
        if(DISS>0) fprintf(stdout,"add r%u,SP,#0x%02X\n",Rd,Word8<<2);

        val = Word8<<2;
        r13 = read_register(13U);
        temp = r13 + val;
        write_register(Rd,temp);
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC13 (void) {
  /* 13. Add offset to stack pointer */  

  uint16_t SWord7 = BITS_SHIFT(ic,6,0);
  uint8_t S = BITS_SHIFT(ic,7,7);    
  uint32_t val = 0;
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  switch (S){
    case 0: // ADD SP, #Imm
      {
        if(DISS>0) fprintf(stdout,"add SP,#0x%02X\n",SWord7<<2);

        val = SWord7<<2;
        temp = read_register(13U);
        temp = temp + val;
        write_register(13U,temp);
      }
    break;
    case 1: // ADD SP, #-Imm
      {
        if(DISS>0) fprintf(stdout,"add SP,#-0x%02X\n",SWord7<<2);

        val = SWord7<<2;
        temp = read_register(13U);
        temp -= val;
        write_register(13U,temp);
      }
    break;
    default: // ERR
     err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC14 (void) {
  /* 14. Push/pop unsigned longs */

  uint16_t Rlist = BITS_SHIFT(ic,7,0);
  uint8_t R = BITS_SHIFT(ic,8,8);  
  uint8_t L = BITS_SHIFT(ic,11,11);    
  uint32_t sp = 0;
  uint32_t ra = 0;
  uint32_t rb = 0;
  uint32_t rc = 0;
  uint32_t rd = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  char s = (L<<1) | R; 	
  switch (s){
    case 0: // PUSH { Rlist }
      {
        if(DISS>0)
        {
            fprintf(stdout,"push {");
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1)&0xFF,ra++)
            {
                if( (ic & rb)>0 ) 
                {
                    if(rc > 0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            fprintf(stdout,"}\n");
        }

        sp = read_register(13U);
        for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                rc++;
            }
        }        
        rc <<= 2U;
        sp -= rc;
        rd = sp;
        for(ra=0,rb=0x01;rb;rb=(rb<<1)&0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write32(rd,read_register(ra));
                rd += 4;
            }
        }
        write_register(13U,sp);
      }
    break;
    case 1: // PUSH { Rlist, LR }
      {
        if(DISS>0)
        {
            fprintf(stdout,"push {");
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFF,ra++)
            {
                if( (ic & rb)>0 ) 
                {
                    if(rc>0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            if(rc>0) fprintf(stdout,",");
            fprintf(stdout,"lr");            
            fprintf(stdout,"}\n");
        }
        sp = read_register(13U);
        for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                rc++;
            }
        }
        rc++;
        rc <<= 2U;
        sp -= rc;
        rd = sp;
        for(ra=0,rb=0x01;rb;rb=(rb<<1) & 0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write32(rd,read_register(ra));
                rd += 4U;
            }
        }
        rc = read_register(14U);
        write32(rd,rc); //read_register(14));

        if( (rc & 1UL) == 0 )
        {
            //viv- fprintf(stdout,"push {lr} with an ARM address pc 0x%08X popped 0x%08X\n",pc,rc);
            //TODO ?  exit(1);
        }
        
        write_register(13U,sp);
    }
    break;
    case 2: // POP { Rlist }
      {
        if(DISS>0)
        {
            fprintf(stdout,"pop {");
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFF,ra++)
            {
                if( (ic & rb)>0 )
                {
                    if(rc>0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            fprintf(stdout,"}\n");
        }

        sp=read_register(13U);
        for(ra=0,rb=0x01;rb;rb=(rb<<1) & 0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write_register(ra,read32(sp));
                sp += 4;
            }
        }
        write_register(13U,sp);        
      }      
    break;
    case 3: // POP { Rlist, PC }
      {
        if(DISS>0)
        {
            fprintf(stdout,"pop {");
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFF,ra++)
            {
                if( (ic & rb)>0 )
                {
                    if(rc>0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            if(rc>0) fprintf(stdout,",");
            fprintf(stdout,"pc");
            fprintf(stdout,"}\n");
        }

        sp = read_register(13U);
        for(ra=0,rb=0x01;rb;rb=(rb<<1) & 0xFFU,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write_register(ra,read32(sp));
                sp += 4;
            }
        }
        rc = read32(sp);
        if( (rc & 1UL) == 0 )
        {
            fprintf(stdout,"pop {rc} with an ARM address pc 0x%08X popped 0x%08X\n",pc,rc);
            //TODO ?? exit(1);
            rc &= ~1UL;
        }
        rc += 2;
        write_register(15U,rc);
        sp += 4;
        write_register(13U,sp);
      }
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC15 (void) {
  /* 15. Multiple load/store */
  
  uint16_t Rlist = BITS_SHIFT(ic,7,0);
  uint8_t Rb = BITS_SHIFT(ic,10,8);  
  uint8_t L = BITS_SHIFT(ic,11,11);   
  uint32_t sp = 0;
  uint32_t ra = 0;
  uint32_t rb = 0;
  uint32_t rc = 0;


  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  switch (L){
    case 0: // STMIA Rb!, { Rlist }
      {
        if(DISS>0)
        {
            fprintf(stdout,"stmia r%u!,{",Rb);
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFF,ra++)
            {
                if( (ic & rb)>0 )
                {
                    if(rc>0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            fprintf(stdout,"}\n");
        }

        sp = read_register(Rb);
        for(ra=0,rb=0x01;rb;rb=(rb<<1) & 0xFF,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write32(sp,read_register(ra));
                sp += 4;
            }
        }
        write_register(Rb,sp);        
      }      
    break;
    case 1: // LDMIA Rb!, { Rlist }
      {
        if(DISS>0)
        {
            fprintf(stdout,"ldmia r%u!,{",Rb);
            for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1) & 0xFF,ra++)
            {
                if( (ic & rb)>0 )
                {
                    if(rc>0) fprintf(stdout,",");
                    fprintf(stdout,"r%u",ra);
                    rc++;
                }
            }
            fprintf(stdout,"}\n");
        }

        sp = read_register(Rb);
        for(ra=0,rb=0x01;rb;rb=(rb<<1) & 0xFF,ra++)
        {
            if( (Rlist & rb) > 0 )
            {
                write_register(ra,read32(sp));
                sp += 4;
            }
        }
        //there is a write back exception.
        if( (Rlist & (1U<<Rb))==0) write_register(Rb,sp);
      }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC16 (void) {
  /* 16. Conditional branch */  

  uint16_t SOffset8 = BITS_SHIFT(ic,7,0);
  uint8_t Cond = BITS_SHIFT(ic,11,8);
  uint32_t val = 0;
  uint32_t ra = 0;  

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }
  
  val = SOffset8;
  if( (SOffset8 & 0x80) > 0 ) val |= (uint32_t)(~0UL)<<8;
  
  val <<= 1;
  val += pc;
  val += 2;
  
  switch (Cond){
    case 0: // BEQ label
    {
      if(DISS>0) fprintf(stdout,"beq 0x%08X\n",val-2);        

      if( (cpsr & CPSR_Z) > 0 )
      {
          write_register(15U,val);
      }    
    }      
    break;
    case 1: // BNE label
    {
      if(DISS>0) fprintf(stdout,"bne 0x%08X\n",val-2);    

      if( !( (cpsr & CPSR_Z) > 0 ) )
      {
          write_register(15U,val);
      }      
    }      
    break;
    case 2: // BCS label
    {
      if(DISS>0) fprintf(stdout,"bcs 0x%08X\n",val-2);    

      if( (cpsr & CPSR_C) > 0 )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 3: // BCC label
    {
      if(DISS>0) fprintf(stdout,"bcc 0x%08X\n",val-2);    

      if( !( (cpsr & CPSR_C) > 0 ) )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 4: // BMI label
    {
      if(DISS>0) fprintf(stdout,"bmi 0x%08X\n",val-2);    

      if( (cpsr & CPSR_N) > 0 )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 5: // BPL label
    {
      if(DISS>0) fprintf(stdout,"bpl 0x%08X\n",val-2);    

      if( !( (cpsr & CPSR_N) > 0) )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 6: // BVS label
    {
      if(DISS>0) fprintf(stdout,"bvs 0x%08X\n",val-2);    

      if( (cpsr & CPSR_V) > 0 )
      {
          write_register(15U,val);
      }
      
    }      
    break;
    case 7: // BVC label
    {
      if(DISS>0) fprintf(stdout,"bvc 0x%08X\n",val-2);    

      if( !( (cpsr & CPSR_V) > 0 ) )
      {
          write_register(15U,val);
      }      
    }      
    break;
    case 8: // BHI label
    {
      if(DISS>0) fprintf(stdout,"bhi 0x%08X\n",val-2);    

      if( ((cpsr & CPSR_C)>0)  && (!((cpsr & CPSR_Z)>0)) )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 9: // BLS label
    {
      if(DISS>0) fprintf(stdout,"bls 0x%08X\n",val-2);    

      if( ((cpsr& CPSR_Z)>0) || (!((cpsr & CPSR_C)>0)) )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 10: // BGE label
    {
      if(DISS>0) fprintf(stdout,"bge 0x%08X\n",val-2);    

      ra = 0;
      if(  ((cpsr & CPSR_N)>0) && ((cpsr & CPSR_V)>0) ) ra++;
      if(  (!((cpsr & CPSR_N)>0)) && (!((cpsr & CPSR_V)>0)) ) ra++;
      if( ra > 0 )
      {
          write_register(15U,val);
      }
      
    }      
    break;
    case 11: // BLT label
    {
      if(DISS>0) fprintf(stdout,"blt 0x%08X\n",val-2);    

      ra = 0;
      if( (!((cpsr & CPSR_N)>0)) && ((cpsr & CPSR_V)>0) ) ra++;
      if( (!((cpsr & CPSR_V)>0)) && ((cpsr & CPSR_N)>0) ) ra++;
      if( ra > 0 )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 12: // BGT label
    {
      if(DISS>0) fprintf(stdout,"bgt 0x%08X\n",val-2);    

      ra = 0;
      if( ((cpsr & CPSR_N)>0) && ((cpsr & CPSR_V)>0) ) ra++;
      if( (!((cpsr & CPSR_N)>0)) && (!((cpsr & CPSR_V)>0)) ) ra++;
      if( (cpsr & CPSR_Z)>0 ) ra=0;
      if( ra > 0 )
      {
          write_register(15U,val);
      }
    }      
    break;
    case 13: // BLE label
    {
      if(DISS>0) fprintf(stdout,"ble 0x%08X\n",val-2);    

      ra = 0;
      if( (!((cpsr & CPSR_N)>0)) && ((cpsr & CPSR_V)>0) ) ra++;
      if( (!((cpsr & CPSR_V)>0)) && ((cpsr & CPSR_N)>0) ) ra++;
      if( (cpsr & CPSR_Z) > 0 ) ra++;
      if( ra > 0 )
      {
          write_register(15U,val);
      }     
    }      
    break;
    case 14: // //undefined instruction
    {
     err |= (1U<<8); /* ERR */
    }      
    break;
    case 15: // create instrucion SWI
    {
      //
    }      
    break;
    default: // ERR
      err |= (1U<<8); /* ERR */
    break;
  }
}
void EncodeIC17 (void) {
  /* 17. Software Interrupt */  

  uint16_t Value8 = BITS_SHIFT(ic,7,0);  

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

    // SWI Value8
    if(DISS>0) fprintf(stdout,"swi 0x%02X\n",Value8 & 0xFF);

    if( (Value8 & 0xFF) == 0xCC ) //TODO ??
    {
        write_register(0U,cpsr);
    }
    else
    {
        fprintf(stdout,"\n\nswi 0x%02X\n",Value8);
        // exit(1);

       err |= (1U<<10); /* ERR */; //TODO ??
    }  
}
void EncodeIC18 (void) {
  /* 18. Unconditional branch */  

  uint16_t Offset11 = BITS_SHIFT(ic,10,0);
  uint32_t val = 0;
  
  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  // B label    
    if( (Offset11 & (1U<<10))>0 ) val |= (uint32_t)(~0UL)<<11;
    val <<= 1;
    val += pc;
    val += 2;

    if(DISS>0) fprintf(stdout,"b 0x%08X\n",val-2);

    write_register(15U,val);
}
void EncodeIC19 (void) {
  /* 19. Long branch with link */

  uint32_t Offset11 = BITS_SHIFT(ic,10,0);  
  uint8_t  H = BITS_SHIFT(ic,11,11);  
  uint32_t rb = 0;  
  uint32_t temp = 0;

  if (ic_soft_hard > 0)
  { /* Аппаратная реализация инструкций Thumb-2 */
		
	return;
  }

  switch(H) {
  	case 0:
		if(DISS>0) fprintf(stdout,"\n");
        rb = Offset11;
        if( rb & (1UL<<10U)) rb |= (~((1UL<<11U) - 1UL)); //sign extend
        rb <<= 12U;
        rb += pc;
        write_register(14U,rb);
    break;
  	case 1:
            //branch to thumb
            printf(" {Offset11=0x%x} ",Offset11);            
            printf(" {PC=0x%x} ",pc);
            printf(" {LR=0x%x} ",read_register(14U));            
            
            rb = read_register(14U) + (Offset11<<1);            
			rb += 2U;

 if(DISS) fprintf(stdout,"bl 0x%08X\n",rb-2);
            write_register(14,(pc-2)|1);
            write_register(15,rb);
  	break; 
  	default:

  	break;
  }
}

/* ---------------------------------------------------------
*  Main 
*  ---------------------------------------------------------
*/
int main ( int argc, char *argv[] )
{
    FILE *fp;

    uint32_t ra;

    if(argc < 2)
    {
        fprintf(stdout,"bin file not specified\n");
        return(1);
    }

    output_vcd=0;
    for(ra=2;ra<argc;ra++)
    {
        if(strcmp(argv[ra],"--vcd")==0) output_vcd=1;
    }
    fp = fopen(argv[1],"rb");
    if(fp == NULL)
    {
        fprintf(stdout,"Error opening file [%s]\n",argv[1]);
        return(1);
    }
    memset(rom,0xFF,sizeof(rom));
    ra = fread(rom,1,sizeof(rom),fp);
    fclose(fp);
    
    int i=0;
    for(i=0;i<10;i++) {
    printf("%i - 0x%08x\n",i,rom[i]);
    }

    if( output_vcd > 0 )
    {
        fprintf(stdout,"output vcd enabled\n");
        fpvcd = fopen("output.vcd","wt");
        if(fpvcd == NULL)
        {
            fprintf(stdout,"Error creating file output.vcd\n");
            output_vcd = 0;
            return(1);
        }
        fprintf(fpvcd,"$version Generated by thumbulator $end\n");
        fprintf(fpvcd,"$timescale 1ns $end\n");
        fprintf(fpvcd,"$scope module thumb $end\n");
        for(ra=0;ra<16;ra++)
        {
            fprintf(fpvcd,"$var wire 32 r%u r%u $end\n",ra,ra);
        }
        fprintf(fpvcd,"$var wire 16 inst inst $end\n");
        fprintf(fpvcd,"$upscope $end\n");
        fprintf(fpvcd,"$enddefinitions $end\n");
        vcdcount = 0;
        fprintf(fpvcd,"#%u\n",vcdcount++);
        for(ra=0;ra<16;ra++)
        {
            fprintf(fpvcd,"b0 r%u\n",ra);
        }
        fprintf(fpvcd,"b0 inst\n");
    }

    memset(ram,0x00,sizeof(ram));
    run();
    if(output_vcd > 0)
    {
        fclose(fpvcd);
    }
    return(0);
}

/* thumbulator.c */