/*
 * Architecture specific code used by dl-startup.c
 * Copyright (C) 2024 uClibc-ng contributors.
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <features.h>
#include <sys/asm.h>

#ifndef _RTLD_PROLOGUE
# define _RTLD_PROLOGUE(entry)                                          \
        ".globl\t" __STRING (entry) "\n\t"                              \
        ".type\t" __STRING (entry) ", @function\n"                      \
        __STRING (entry) ":\n\t"
#endif

#ifndef _RTLD_EPILOGUE
# define _RTLD_EPILOGUE(entry)                                          \
        ".size\t" __STRING (entry) ", . - " __STRING (entry) "\n\t"
#endif

__asm__(\
	".text\n\
        " _RTLD_PROLOGUE (_start) "\
        move $a0, $sp\n\
        bl _dl_start\n\
        # Stash user entry point in s0.\n\
        move $s0, $a0\n\
        # See if we were run as a command with the executable file\n\
        # name as an extra leading argument.\n\
        la.pcrel $t0, _dl_skip_args\n\
        ld.w $a0, $t0, 0\n\
        # Load the original argument count.\n\
        ld.d $a1, $sp, 0\n\
        # Subtract _dl_skip_args from it.\n\
        sub.d $a1, $a1, $a0\n\
        # Adjust the stack pointer to skip _dl_skip_args words.\n\
        slli.d $a0, $a0, 3\n\
        add.d $sp, $sp, $a0\n\
        # Save back the modified argument count.\n\
        st.d $a1, $sp, 0\n\
        # Pass our finalizer function to _start.\n\
        la.pcrel $a0, _dl_fini\n\
        # Jump to the user entry point.\n\
        jr $s0\n\
        " _RTLD_EPILOGUE (_start) "\
        .previous" \
);

/* Get a pointer to the argv array.  On many platforms this can be just
 * the address of the first argument, on other platforms we need to
 * do something a little more subtle here.  */
#define GET_ARGV(ARGVP, ARGS) ARGVP = (((unsigned long*)ARGS)+1)

/* Function calls are not safe until the GOT relocations have been done.  */
#define NO_FUNCS_BEFORE_BOOTSTRAP

/* Handle relocation of the symbols in the dynamic loader. */
static __always_inline
void PERFORM_BOOTSTRAP_RELOC(ELF_RELOC *rpnt, ElfW(Addr) *reloc_addr,
	ElfW(Addr) symbol_addr, ElfW(Addr) load_addr, ElfW(Addr) *sym)
{
	switch (ELF_R_TYPE(rpnt->r_info)) {
		case R_LARCH_NONE:
			break;
		case R_LARCH_JUMP_SLOT:
			*reloc_addr = symbol_addr + rpnt->r_addend;
			break;
		case R_LARCH_RELATIVE:
			*reloc_addr = load_addr + rpnt->r_addend;
			break;
		default:
			_dl_exit(1);
	}
}

#define DL_UPDATE_LOADADDR_HDR(LOADADDR, ADDR, PHDR)