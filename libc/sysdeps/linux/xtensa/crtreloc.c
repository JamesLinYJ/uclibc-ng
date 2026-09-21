/* Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   written by Alexandre Oliva <aoliva@redhat.com>
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

In addition to the permissions in the GNU Lesser General Public
License, the Free Software Foundation gives you unlimited
permission to link the compiled version of this file with other
programs, and to distribute those programs without any restriction
coming from the use of this file.  (The GNU Lesser General Public
License restrictions do apply in other respects; for example, they
cover modification of the file, and distribution when not linked
into another program.)

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, see <http://www.gnu.org/licenses/>.  */

#ifdef __FDPIC__

#include <sys/types.h>
#include <link.h>
#include <fdpic-loadmap.h>

/* Require an entire interval in one mapped segment, with no pointer wrapping. */
static __always_inline int
reloc_range_valid (const struct elf32_fdpic_loadmap *map,
                   const void *p, const void *e)
{
  unsigned long start = (unsigned long)p, end = (unsigned long)e;
  unsigned int i;

  if (!map || map->version || !map->nsegs ||
      map->nsegs > XTENSA_FDPIC_MAX_LOADSEGS || end <= start)
    return 0;
  for (i = 0; i < map->nsegs; i++)
    {
      unsigned long base = map->segs[i].addr, size = map->segs[i].p_memsz;
      if (start >= base && start - base < size && end - start <= size - (start - base))
        return 1;
    }
  return 0;
}

static __always_inline int
reloc_target_valid (const struct elf32_fdpic_loadmap *map, void *p,
                    unsigned long size)
{
  unsigned long address = (unsigned long)p;

  if (!size || address + size < address)
    return 0;
#ifdef ESP32S3_FDPIC_HARVARD_ALIAS
  /* The loader contract reserves this DBus bank for writable PSRAM.
     Reject both Flash data and every instruction-only alias. */
  if ((address >> 22) - 0xf1UL >= 2 ||
      ((address + size - 1) >> 22) - 0xf1UL >= 2)
    return 0;
#endif
  return reloc_range_valid (map, p, (void *)(address + size));
}

/* Relocate exactly four bytes, including unaligned targets, without touching
   neighbouring objects. This runs before libc/GOT initialization. */
static __always_inline int
reloc_range_indirect (void ***p, void ***e,
                      const struct elf32_fdpic_loadmap *map)
{
  for (; p < e; p++)
    {
      unsigned char *target;
      unsigned long value = 0;
      void *translated;
      unsigned int i;

      if (*p == (void **)-1)
        continue;
      target = __reloc_pointer (*p, map);
      if (!reloc_target_valid (map, target, sizeof (unsigned long)))
        return 0;
      for (i = 0; i < sizeof (unsigned long); i++)
        {
#ifdef __XTENSA_EB__
          value = (value << 8) | target[i];
#else
          value |= (unsigned long)target[i] << (i * 8);
#endif
        }
      translated = __reloc_pointer ((void *)value, map);
      if (translated == (void *)-1)
        return 0;
      value = (unsigned long)translated;
      for (i = 0; i < sizeof (unsigned long); i++)
        {
#ifdef __XTENSA_EB__
          target[i] = value >> ((sizeof (unsigned long) - 1 - i) * 8);
#else
          target[i] = value >> (i * 8);
#endif
        }
    }
  return 1;
}

attribute_hidden void *
__self_reloc (const struct elf32_fdpic_loadmap *map, void ***p, void ***e)
{
  void *got;

  if (!reloc_range_valid (map, p, e) ||
      ((unsigned long)p & (sizeof (void *) - 1)) ||
      ((unsigned long)e - (unsigned long)p) % sizeof (void *))
    return (void *)-1;
#ifdef ESP32S3_FDPIC_HARVARD_ALIAS
  if ((unsigned long)p >> 25 != 0x1e ||
      ((unsigned long)e - 1) >> 25 != 0x1e)
    return (void *)-1;
#endif
  got = __reloc_pointer (e[-1], map);
  if (!reloc_target_valid (map, got, 3 * sizeof (void *)) ||
      !reloc_range_indirect (p, e - 1, map))
    return (void *)-1;
  return got;
}

#endif /* __FDPIC__ */
