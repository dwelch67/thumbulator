
void uart_putc ( unsigned int );
unsigned int uart_getc ( void );

char get_onechar ( void );
void show_onechar ( char c );
void show_string ( char *s );
void show_int ( int i );
void *not_malloc ( unsigned int );
void *tbuf_malloc ( unsigned int );

void srandy ( unsigned int x );
unsigned int randy ( void );
void not_memcpy( char *d, char *s, int len);

int not_atoi ( char *s );

void err ( int i, char *s );
void errx(int i,char *s);
unsigned int randy ( void );
