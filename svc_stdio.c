// System Calls on thumbulator.
// Implement the minimal stdio calls defined by SockPuppet
//
// Cortex-M system calls pass parameters via the stack.

#include <stdint.h>
#include <stdio.h>

#include "thumbulator_svc.h"

// Access ram via the official paths.
extern uint32_t fetch32 ( uint32_t addr );
extern uint32_t fetch16 ( uint32_t addr );

void svc_init() { 
  fprintf(stderr,"SVC Layer Initialization\n");
  }

void svc_handler(uint32_t addr) { 

  // Go get the system call number, using the official mechanism.
  uint32_t pc = fetch32(addr + 0x18);
  pc -= 2; // Correct for instuction address. 
  int syscall_no = fetch16(pc) & 0xff;


  // printf("SVC addr %x pc %0x\n", addr, pc);
  fprintf(stderr,"SVC %d\n", syscall_no);
  
  // There are 8 items on the stack.   Display them.

  fprintf(stderr,"%08x: ", addr);

  for ( int i = addr; i < (addr + 0x20); i = i + 4) {
    fprintf(stderr,"%08x ", fetch32(i));
  }
  fprintf(stderr,"\n");


  switch(syscall_no) {
    case 2:
    default:
      
      putchar(fetch16(addr+4) & 0xFF );
  }

  return;

} 

