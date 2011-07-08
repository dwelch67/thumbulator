
//if compiled together with the test code, the compiler optimizer
//optimizes out the use of these funtions and simply puts the right
//answer in the result without doing the work of the test

unsigned short ult ( unsigned short a, unsigned short b)
{
    if(a<b) return(1);
    return(0);
}

unsigned short slt ( short a, short b)
{
    if(a<b) return(1);
    return(0);
}

unsigned short ulte ( unsigned short a, unsigned short b)
{
    if(a<=b) return(1);
    return(0);
}

unsigned short slte ( short a, short b)
{
    if(a<=b) return(1);
    return(0);
}

unsigned short ugt ( unsigned short a, unsigned short b)
{
    if(a>b) return(1);
    return(0);
}

unsigned short sgt ( short a, short b)
{
    if(a>b) return(1);
    return(0);
}

unsigned short ugte ( unsigned short a, unsigned short b)
{
    if(a>=b) return(1);
    return(0);
}

unsigned short sgte ( short a, short b)
{
    if(a>=b) return(1);
    return(0);
}

