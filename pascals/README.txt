
I have started to dabble with Pascal, specifically a reduced pascal
called pascal-s.  Wirth came up with it but this one in particular is
Jan van de Snepscheut/Johnstone's, or so the history indicates.

Long story short I have an interest in compilers, LLVM is the primary
interest but too big and complicated to include in a small package like
this one.  Also a moving target constantly under development, takes
forever to build even on a fast machine.  Wanted something more
digestible even if not good at optimization, have been and am still
considering including something small-c based.  I am from the pascal
generation but never learned it formally so this will be an education
for me.  I love that the syntax can be defined in terms of state
machines.  Probably why it compiled so fast even on a original IBM PC.
I didnt learn it fully back then because I had issues with the language
compared to C, but that is here nor there.  This version of pascal-s,
like P4 and P5 generates p-code, this one much simpler and cleaner
than what P5 produces.  The single file source for this pascal-s compiler
is less than 900 lines.

It may take work to get freepascal to compile pascals.pas, bug gnu
pascal (gpc) had no problems at all, so that is what I used.  Eventually
I may go so far as to rewrite it in C (interesting concept isn't it a
pascal compiler written in C), will see.

pascals.pas is the source code for the pascals compiler.  I modified
it to use a non-hardcoded filename but did not go so far as to put
in any checking to see if there was a file specified.  The output is
stdout and captured with a redirect.

pthumb.c this takes the pcode output from pascals and produces thumb
assembler.  This is incomplete at this time.

test*.pas are very simple test programs used to get started with
developing and testing pthumb.

vectors.s and memmap are there to generate thumb binaries to run
with the thumbulator.  DBUGRAM and DBUGRAMW were added to thumbulator.c
to help debug this stack based p-code execution.



