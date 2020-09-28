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
extern void write32 ( uint32_t addr, uint32_t data );


void svc_init() { 
  fprintf(stderr,"SVC Layer Initialization\n");
  }

void svc_handler(uint32_t addr) { 

  // Go get the system call number, using the official mechanism.
  uint32_t pc = fetch32(addr + 0x18);
  pc -= 2; // Correct for instuction address. 
  int syscall_no = fetch16(pc) & 0xff;


  // printf("SVC addr %x pc %0x\n", addr, pc);
  // fprintf(stderr,"SVC %d\n", syscall_no);
  
  // There are 8 items on the stack.   Display them.

  // fprintf(stderr,"%08x: ", addr);

  // for ( int i = addr; i < (addr + 0x20); i = i + 4) {
  //  fprintf(stderr,"%08x ", fetch32(i));
  //}
  //fprintf(stderr,"\n");


  switch(syscall_no) {
    case 2: // The chars avail call.  Arg: Stream Number
      {
        int ret = getc(stdin);
        if ( ret == EOF ) {
          write32(addr, 0);
          fprintf(stderr, "scv/swi GetC EOF\n");
          return;
          }
        // Otherwise, push it back.
        ungetc( ret, stdin);
        write32(addr, 1);
        return;
      }
      break; 
      
    case 3: // Getchar.   Arg: Stream Number
      {
        int ret = getchar();
        if ( ret == EOF ) {
          write32(addr, -1);
          return;  
          } 
        // Write the data into the stack.
        fprintf(stderr, "scv/swi Rx Character: 0x%x / %c \n", ret, ret);
        write32(addr, ret);
        return;
      }
      
    case 5: 
      // Putchar: Arg: Data, Stream Number
      {
        uint8_t c = fetch16(addr) & 0xFF; 
        putchar(c);
        fprintf(stderr, "scv/swi Tx Character: 0x%02x / %c\n", c,c);

        // Set the return code as zero.
        write32(addr, 0);
        return;        
      }
    default:
      fprintf(stderr, "---------- Other svc/swi %d ---------------\n", syscall_no);
        // Everything else does nothing...
        break;
  }

  return;

} 

