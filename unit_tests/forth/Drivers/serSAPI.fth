\ serSAPI.fth 
\ Just the basics 


only forth definitions
decimal

\ **************
\ *S Shared code
\ **************
-SaveR9 \ We don't need to preserve R9 on this system.

\ SVC( #2 ) int _serKey_q(int x);    ( stream -- t/f )

code _serKey_q 
  mov r0, r7
  svc # $02
  mov r7, r0
  next,
end-code 


\ : (serKey?)	\ base -- flag
\  charsavail ;

: (serkey)	\ base -- char
[ tasking? ] [if]
  begin
    dup _serKey_q false = 
   while
    pause
  repeat
[else]
  begin
    dup _serKey_q true =  
  until
[then]

  getc
;

\ Keep the spin loop here, but use the system call 
\ for the actual data write

\ SVC(  #5 ) void _seremit(int c, int base);        ( stream c  -- )

code _SEREMIT
  mov r1, r7
  ldr r0, [ r6, # $00 ]
  svc # $05
  ldr r7, [ r6, # $04 ]
  add .s r6, r6, # $08
  next,
end-code 

\ : (seremit)	\ char base --
\   putc
\ ;

: (sertype)	\ caddr len base --
  -rot bounds
  ?do  i c@ over _seremit  loop
  drop
;

: (sercr)	\ base --
  #13 over _seremit  #10 swap _seremit  ;

: +FaultConsole	\ --
\ *G For use in fault exception handlers, the multi-tasker must
\ ** be turned off and *\fo{EMIT) and friends must run in polled
\ ** mode.
[ tasking? ] [if]  single  [then]
;


\ *********
\ *S USART2
\ *********
\ *P USART2 is on the APB bus, so the baud rate is calculated
\ ** using that bus clock.

: serkey?0	\ -- flag
  0 _serKey_q  ;

: serkey0	\ -- char
  0 (serkey)  ;

: seremit0	\ char --
  0 _seremit  ;

: serType0	\ c-addr len --
  0 (sertype)  ;

: sercr0	\ --
  0 (sercr)  ;

create Console0	\ -- addr ; OUT managed by upper driver
  ' serkey0 ,		\ -- char ; receive char
  ' serkey?0 ,		\ -- flag ; check receive char
  ' seremit0 ,		\ -- char ; display char
  ' sertype0 ,		\ caddr len -- ; display string
  ' sercr0 ,		\ -- ; display new line

console-port 0 = [if]
  console0 constant console
[then]

\ *****************
\ *S Initialisation
\ *****************

: init-ser ;


\ ======
\ *> ###
\ ======

decimal
