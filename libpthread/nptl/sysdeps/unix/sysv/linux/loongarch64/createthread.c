/* Copyright (C) 2024 uClibc-ng contributors.

   Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
*/

/* Value passed to 'clone' for initialization of the thread register.  */
#define TLS_VALUE ((void *) (pd) \
		   + TLS_TCB_OFFSET + TLS_PRE_TCB_SIZE)

/* Get the real implementation.	 */
#include <sysdeps/pthread/createthread.c>