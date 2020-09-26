/// @file syscall-testshim.h
/// @brief Function Prototypes for SAPI Calls via the SVC  layer
/// 
// Copyright (C) 2017, Robert Sexton.  All rights reserved.
// Covered by the terms of the supplied Licence.txt file

// Applies to SAPI ABI 2.05
// 

// ------------------------------------
// Standard IO Calls.
// ------------------------------------
int PutChar(int stream, uint8_t c);
int GetChar(int stream);
int GetCharAvail(int stream);
