/* Private macros for accessing __jmp_buf contents.  LoongArch64 version.
   Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

#include <jmpbuf-unwind.h>

/* Helper for generic ____longjmp_chk().  */
#define JB_FRAME_ADDRESS(buf) \
  ((void *) _jmpbuf_sp (buf))