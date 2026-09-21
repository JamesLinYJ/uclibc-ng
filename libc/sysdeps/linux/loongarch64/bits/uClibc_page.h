/*  Copyright (C) 2024 uClibc-ng contributors.
 *
 *  Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

/* Supply an architecture specific value for PAGE_SIZE and friends.  */

#ifndef _UCLIBC_PAGE_H
#define _UCLIBC_PAGE_H

/* LoongArch supports 4K, 16K, and 64K pages; default is 16K */
#define PAGE_SHIFT	14
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#define MMAP2_PAGE_SHIFT PAGE_SHIFT

#endif /* _UCLIBC_PAGE_H */