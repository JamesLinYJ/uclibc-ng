/*
 * ESP32-S3 Harvard aliases for the Xtensa FDPIC dynamic loader.
 *
 * Copyright (C) 2026 JamesLinYJ
 *
 * Licensed under the LGPL v2.1 or later, see COPYING.LIB in this tree.
 */

#ifndef _XTENSA_DL_ESP32S3_FDPIC_H
#define _XTENSA_DL_ESP32S3_FDPIC_H

#define ESP32S3_FDPIC_DBUS_BASE		0x3c000000UL
#define ESP32S3_FDPIC_IBUS_BASE		0x42000000UL
#define ESP32S3_FDPIC_WINDOW_SIZE	0x02000000UL
#define ESP32S3_FDPIC_ALIAS_DELTA	\
	(ESP32S3_FDPIC_IBUS_BASE - ESP32S3_FDPIC_DBUS_BASE)
#include <fdpic-loadmap.h>

/*
 * These are virtual aperture bounds, not installed RAM sizes.  Equal
 * offsets in the DBus and IBus windows select the same external-cache MMU
 * entry.  Data accesses use DBus; instruction fetches use IBus.
 */
static __always_inline Elf32_Addr
_dl_xtensa_fdpic_exec_alias(Elf32_Addr address)
{
	if (address - ESP32S3_FDPIC_DBUS_BASE < ESP32S3_FDPIC_WINDOW_SIZE)
		return address + ESP32S3_FDPIC_ALIAS_DELTA;
	return address;
}

static __always_inline Elf32_Addr
_dl_xtensa_fdpic_data_alias(Elf32_Addr address)
{
	if (address - ESP32S3_FDPIC_IBUS_BASE < ESP32S3_FDPIC_WINDOW_SIZE)
		return address - ESP32S3_FDPIC_ALIAS_DELTA;
	return address;
}

static __always_inline void
_dl_xtensa_fdpic_validate_loadmap(const struct elf32_fdpic_loadmap *map)
{
	if (!map || map->version != 0 || map->nsegs == 0 ||
	    map->nsegs > XTENSA_FDPIC_MAX_LOADSEGS)
		_dl_exit(-1);
}

static __always_inline void
_dl_xtensa_init_loadaddr_map(struct elf32_fdpic_loadaddr *loadaddr,
			     Elf32_Addr got,
			     struct elf32_fdpic_loadmap *map)
{
	_dl_xtensa_fdpic_validate_loadmap(map);
	__dl_init_loadaddr_map(loadaddr, got, map);
}

static __always_inline int
_dl_xtensa_init_loadaddr(struct elf32_fdpic_loadaddr *loadaddr,
			 Elf32_Phdr *phdr, int phnum)
{
	int count = __dl_init_loadaddr(loadaddr, phdr, phnum);

	if (count <= 0 || count > XTENSA_FDPIC_MAX_LOADSEGS)
		_dl_exit(-1);
	return count;
}

static __always_inline void
_dl_xtensa_init_loadaddr_hdr(struct elf32_fdpic_loadaddr loadaddr,
			     void *addr, Elf32_Phdr *phdr, int maxsegs)
{
	struct elf32_fdpic_loadseg *segment;

	if ((phdr->p_flags & (PF_X | PF_W)) == (PF_X | PF_W))
		_dl_exit(-1);

	__dl_init_loadaddr_hdr(loadaddr, addr, phdr, maxsegs);
	segment = &loadaddr.map->segs[loadaddr.map->nsegs - 1];
	if (phdr->p_flags & PF_X)
		segment->addr = _dl_xtensa_fdpic_exec_alias(segment->addr);
}

/* Convert an executable loadmap entry back to its mmap/DBus address. */
static __always_inline void
_dl_xtensa_unmap_loadseg(const struct elf32_fdpic_loadseg *segment)
{
	Elf32_Addr address = _dl_xtensa_fdpic_data_alias(segment->addr);
	Elf32_Addr offset = segment->p_vaddr & ADDR_ALIGN;
	Elf32_Word length;

	if (address < offset || segment->p_memsz > (Elf32_Word)-1 - offset)
		_dl_exit(-1);
	length = segment->p_memsz + offset;
	if (length)
		_dl_munmap((void *)(address - offset), length);
}

static __always_inline void
_dl_xtensa_update_loadaddr_hdr(struct elf32_fdpic_loadaddr loadaddr,
			       void *addr, Elf32_Phdr *phdr)
{
	struct elf32_fdpic_loadseg *segment;
	int i;

	if ((phdr->p_flags & (PF_X | PF_W)) == (PF_X | PF_W))
		_dl_exit(-1);

	_dl_xtensa_fdpic_validate_loadmap(loadaddr.map);
	for (i = 0; i < loadaddr.map->nsegs; i++)
		if (loadaddr.map->segs[i].p_vaddr == phdr->p_vaddr &&
		    loadaddr.map->segs[i].p_memsz == phdr->p_memsz)
			break;
	if (i == loadaddr.map->nsegs)
		_dl_exit(-1);

	segment = &loadaddr.map->segs[i];
	_dl_xtensa_unmap_loadseg(segment);
	segment->addr = (Elf32_Addr)addr;
	if (phdr->p_flags & PF_X)
		segment->addr = _dl_xtensa_fdpic_exec_alias(segment->addr);

#if defined(__SUPPORT_LD_DEBUG__)
	if (_dl_debug)
		_dl_dprintf(_dl_debug_file,
			    "%i: changed mapping %x at %x, size %x\n",
			    i, segment->p_vaddr, segment->addr,
			    segment->p_memsz);
#endif
}

static __always_inline void
__dl_loadaddr_unmap(struct elf32_fdpic_loadaddr loadaddr,
		    struct funcdesc_ht *funcdesc_ht)
{
	int i;

	_dl_xtensa_fdpic_validate_loadmap(loadaddr.map);
	for (i = 0; i < loadaddr.map->nsegs; i++)
		_dl_xtensa_unmap_loadseg(&loadaddr.map->segs[i]);

	_dl_free(loadaddr.map);
	if (funcdesc_ht)
		htab_delete(funcdesc_ht);
}

#endif /* _XTENSA_DL_ESP32S3_FDPIC_H */
