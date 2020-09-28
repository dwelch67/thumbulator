# Thumbulator Updates

# Notes

The code is configured for two 1Mbyte blocks:
- ROM at 0x0000:0000 
- RAM at 0x2000:0000 

# Limitations 

Thumbulator does not support interrupts or multiple stacks 

# SockPuppet

This fork includes support for the SockPuppet ABI.   System calls are supported
via the SVC/SWI instruction.  
