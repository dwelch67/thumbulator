
//-------------------------------------------------------------------
// Copyright (C) 2020 Robert Sexton
// A minimal executable that exercises the SVC Interface.
//-------------------------------------------------------------------

//*****************************************************************************
// The simplest possible program.
//*****************************************************************************
#include <stdbool.h> 
#include <stdint.h> 

#include "syscall-testshims.h"

volatile uint32_t counter;

const char message[] = "Hello!\n";

int main() {
  
  // Emit the message via the system call, stream 1.
  char *p = (char *) message;
  while(*p) {    
    PutChar(*p,0);
    p++;
  }
  
  // Echo Test. 
  while(GetCharAvail(0) ) {
    PutChar(0x41,0);
    PutChar(GetChar(0),0);    
  }
  
  __asm("bkpt 0\r");
  
}

