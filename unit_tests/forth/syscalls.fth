\ System calls

-SaveR9 \ We don't need to preserve R9 on this system.

\ SVC(  #0 ) int APIVER( );        ( -- ver )
code APIVER
  svc # $00
  sub .s r6, r6, # $04
  str r7, [ r6, # $00 ]
  mov r7, r0
  next,
end-code 

\ SVC(  #1 ) int SHARED_P( );      ( -- addr )
code SHARED_P
  svc # $01
  sub .s r6, r6, # $04
  str r7, [ r6, # $00 ]
  mov r7, r0
  next,
end-code 

\ Two are embedded right into the driver:
\ SVC(  #2 ) int  CHARSAVAIL(int x);    ( stream -- t/f )
\ SVC(  #5 ) void PUTC(int x, int c);        ( stream c  -- )

\ SVC(  #3 ) int  GETC(int x);       ( stream -- char )
code GETC 
  mov r0, r7
  svc # $03
  mov r7, r0
  next,
end-code 


\ SVC( #14 ) int SVCTEST( int x, int y, int r );

\ SVC( #15 ) int TICKS();          ( -- ms )
code TICKS
  svc # $0f
  sub .s r6, r6, # $04
  str r7, [ r6, # $00 ]
  mov r7, r0
  next,
end-code 

