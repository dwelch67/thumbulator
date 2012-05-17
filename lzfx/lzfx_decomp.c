/*
 * Copyright (c) 2009 Andrew Collette <andrew.collette at gmail.com>
 * http://lzfx.googlecode.com
 *
 * Implements an LZF-compatible compressor/decompressor based on the liblzf
 * codebase written by Marc Lehmann.  This code is released under the BSD
 * license.  License and original copyright statement follow.
 *
 *
 * Copyright (c) 2000-2008 Marc Alexander Lehmann <schmorp@schmorp.de>
 *
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 *
 *   1.  Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *   2.  Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MER-
 * CHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPE-
 * CIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTH-
 * ERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "lzfx.h"

/* Decompressor */
int lzfx_decompress_tiny
(
    const unsigned char *ibuf,
    const unsigned int ilen,
    unsigned char *obuf,
    unsigned int *olen
)
{
    unsigned int ipoff;
    unsigned int opoff;

    if(olen == 0) return LZFX_EARGS;
    if(ibuf == 0) return LZFX_EARGS;
    if(obuf == 0) return LZFX_EARGS;

    opoff=0;
    for(ipoff=0;ipoff<ilen;)
    {
        unsigned int ctrl;

        ctrl = ibuf[ipoff++];

        /* Format 000LLLLL: a literal byte string follows, of length L+1 */
        if(ctrl < (1 << 5))
        {

            ctrl++;

            if((opoff + ctrl) > *olen) return LZFX_OVERFLOW;
            if((ipoff + ctrl) > ilen) return LZFX_ECORRUPT;


            while(ctrl--)
            {
                obuf[opoff++]=ibuf[ipoff++];
            }

        /*  Format #1 [LLLooooo oooooooo]: backref of length L+1+2
                          ^^^^^ ^^^^^^^^
                            A      B
                   #2 [111ooooo LLLLLLLL oooooooo] backref of length L+7+2
                          ^^^^^          ^^^^^^^^
                            A               B
            In both cases the location of the backref is computed from the
            remaining part of the data as follows:

                location = op - A*256 - B - 1
        */
        }
        else
        {
            unsigned int len;
            unsigned int refoff;

            len = (ctrl >> 5);
            refoff = opoff - ((ctrl & 0x1f) << 8) -1;

            if(len==7) len += ibuf[ipoff++];    /* i.e. format #2 */

            len += 2;    /* len is now #octets */

            if((opoff + len) > *olen) return LZFX_OVERFLOW;
            if((ipoff >= ilen)) return LZFX_ECORRUPT;

            refoff -= ibuf[ipoff++];
            if(refoff > opoff) return LZFX_ECORRUPT;

            while(len--)
            {
                obuf[opoff++]=obuf[refoff++];
            }
        }
    }

    *olen = opoff;

    return 0;
}


