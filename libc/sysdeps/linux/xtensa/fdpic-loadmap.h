/* SPDX-License-Identifier: LGPL-2.1-or-later */
/* Shared C/assembly limits for the Linux Xtensa FDPIC loadmap. */
#ifndef _XTENSA_FDPIC_LOADMAP_H
#define _XTENSA_FDPIC_LOADMAP_H

#define XTENSA_FDPIC_LINUX_MAX_LOADSEGS	2048
#ifndef XTENSA_FDPIC_MAX_LOADSEGS
#define XTENSA_FDPIC_MAX_LOADSEGS XTENSA_FDPIC_LINUX_MAX_LOADSEGS
#endif
#if XTENSA_FDPIC_MAX_LOADSEGS <= 0 || \
    XTENSA_FDPIC_MAX_LOADSEGS > XTENSA_FDPIC_LINUX_MAX_LOADSEGS
#error Invalid Xtensa FDPIC loadmap bound
#endif

#define __XTENSA_FDPIC_STRINGIFY_1(value) #value
#define __XTENSA_FDPIC_STRINGIFY(value) __XTENSA_FDPIC_STRINGIFY_1(value)

#endif
