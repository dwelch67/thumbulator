// System Calls on thumbulator.
// Mininial Version that only reports on whats going on.
//
// Cortex-M system calls pass parameters via the stack.

#include <stdint.h>
#include <stdio.h>

#include "thumbulator_svc.h"

// Access ram via the official paths.
extern uint32_t fetch32 ( uint32_t addr );

void svc_init() { 
  printf("SVC Layer Initialization\n");
  }

void svc_handler(uint32_t addr) { 

  printf("SVC %d\n", -1);
  
  // There are 8 items on the stack.   Display them.

  fprintf(stderr,"%08x: ", addr);


  for ( int i = 0; i < 7; i++) {
    fprintf(stderr,"%08x ", fetch32(addr));
    addr += 4; 
    
  }
  fprintf(stderr,"\n");

  return;

} 

