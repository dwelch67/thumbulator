(( App Startup ))

\ *> %DocGenDocsDir%/ReadMe

\ -------------------------------------------------------
\ -------------------------------------------------------
\ Most of the code is in here
\ -------------------------------------------------------
\ -------------------------------------------------------

: WFI [asm wfe asm] ;
: MARKER [asm svc #0 asm] ;
: BREAK [asm bkpt #0 asm] ;

: PURGE 
  begin
   key? dup . if key .  then 
  key? 0= until 
; 

\ -------------------------------------------
\ The word that sets everything up
\ -------------------------------------------
: StartApp
 hex 
 \ .banner cr .cpu cr
 
 \ ." key? " key? . 

 \ 0 20 ldump 
 
 \ ." Purge: "  purge ." Done"  cr

 \ words 

 \ break
 
 marker 
;


