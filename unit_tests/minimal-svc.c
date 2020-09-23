
//-------------------------------------------------------------------
// Copyright (C) 2020 Robert Sexton
// A minimal executable that exercises the SVC Interface.
//-------------------------------------------------------------------

//*****************************************************************************
// The simplest possible program.
//*****************************************************************************
#include <stdbool.h> 
#include <stdint.h> 

volatile uint32_t counter;

extern int PutChar(int stream, uint8_t c);

const char message[] = "Hello!\n";

int main() {
  
  // Emit the message via the system call, stream 1.
  char *p = message;
  while(*p) {    
    PutChar(1,*p);
    p++;
  }
  
  __asm("bkpt 0\r");
  
}

