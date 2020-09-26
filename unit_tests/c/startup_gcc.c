//*****************************************************************************

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
extern void ResetISR(void);

//*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);
void IntDefaultHandler(void);

extern void *_stack_top;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
const void *stack_top __attribute__ ((section(".boot_stack_pointer"))) = &_stack_top ; // The initial stack pointer

void (* const g_pfnVectors[])(void) __attribute__ ((section(".isr_vector"))) =
{
  /* 01 */ ResetISR,           // rese
  /* 02 */ IntDefaultHandler,  // NMI handler
  /* 03 */ IntDefaultHandler,  // Hard fault handler
  /* 04 */ IntDefaultHandler,  // MPU fault handler
  /* 05 */ IntDefaultHandler,  // Bus fault handler
  /* 06 */ IntDefaultHandler,  // Usage fault handler
  /* 07 */ 0, // Reserved
  /* 08 */ 0, // Reserved 
  /* 09 */ 0, // Reserved 
  /* 10 */ 0, // Reserved 
  /* 11 */ IntDefaultHandler, // SVCall 
  /* 12 */ IntDefaultHandler, // Debug monitor handler
  /* 13 */ 0,                 // Reserved
  /* 14 */ IntDefaultHandler, // PendSV handler
  /* 15 */ IntDefaultHandler, // SysTick handler

};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;


//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment, the sharedbss segment, and the topmem
    //
    //__asm(
    //	  "    ldr     r0, =_bss\n"
     //     "    ldr     r1, =_ebss\n"
      //    "    mov     r2, #0\n"
       //   "    .thumb_func\n"
     //     "zero_loop:\n"
      //    "        cmp     r0, r1\n"
      //    "        it      lt\n"
       //   "        strlt   r2, [r0], #4\n"
      //    "        blt     zero_loop\n"
	  
//	  );

//
    // Call the application's entry point.
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
