#pragma once

#include <stdint.h>

/* mach/machine.h */
typedef int cpu_type_t;
typedef int cpu_subtype_t;

#define CPU_ARCH_ABI64	0x01000000
#define CPU_TYPE_X86		((cpu_type_t) 7)
#define	CPU_TYPE_X86_64		(CPU_TYPE_X86 | CPU_ARCH_ABI64)
#define CPU_SUBTYPE_X86_64_ALL		((cpu_subtype_t)3)


/* mach/vm_prot.h */

/*
 *	Types defined:
 *
 *	vm_prot_t		VM protection values.
 */

typedef int		vm_prot_t;

/*
 *	Protection values, defined as bits within the vm_prot_t type
 */

#define	VM_PROT_NONE	((vm_prot_t) 0x00)

#define VM_PROT_READ	((vm_prot_t) 0x01)	/* read permission */
#define VM_PROT_WRITE	((vm_prot_t) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	((vm_prot_t) 0x04)	/* execute permission */

/*
 *	The default protection for newly-created virtual memory
 */

#define VM_PROT_DEFAULT	(VM_PROT_READ|VM_PROT_WRITE)

/*
 *	The maximum privileges possible, for parameter checking.
 */

#define VM_PROT_ALL	(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)

/*
 *	An invalid protection value.
 *	Used only by memory_object_lock_request to indicate no change
 *	to page locks.  Using -1 here is a bad idea because it
 *	looks like VM_PROT_ALL and then some.
 */

#define VM_PROT_NO_CHANGE	((vm_prot_t) 0x08)

/*
 *      When a caller finds that he cannot obtain write permission on a
 *      mapped entry, the following flag can be used.  The entry will
 *      be made "needs copy" effectively copying the object (using COW),
 *      and write permission will be added to the maximum protections
 *      for the associated entry.
 */

#define VM_PROT_COPY            ((vm_prot_t) 0x10)


/*
 *	Another invalid protection value.
 *	Used only by memory_object_data_request upon an object
 *	which has specified a copy_call copy strategy. It is used
 *	when the kernel wants a page belonging to a copy of the
 *	object, and is only asking the object as a result of
 *	following a shadow chain. This solves the race between pages
 *	being pushed up by the memory manager and the kernel
 *	walking down the shadow chain.
 */

#define VM_PROT_WANTS_COPY	((vm_prot_t) 0x10)


/* mach-o/loader.h */

#define MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 0xcffaedfe

struct mach_header_64 {
    uint32_t	magic;		/* mach magic number identifier */
    cpu_type_t	cputype;	/* cpu specifier */
    cpu_subtype_t	cpusubtype;	/* machine specifier */
    uint32_t	filetype;	/* type of file */
    uint32_t	ncmds;		/* number of load commands */
    uint32_t	sizeofcmds;	/* the size of all the load commands */
    uint32_t	flags;		/* flags */
    uint32_t	reserved;	/* reserved */
};

struct load_command {
    uint32_t cmd;		/* type of load command */
    uint32_t cmdsize;	/* total size of command in bytes */
};

#define	MH_EXECUTE	0x2		/* demand paged executable file */
#define	MH_DYLINKER	0x7

#define LC_REQ_DYLD 0x80000000

/* Constants for the cmd field of all load commands, the type */
#define	LC_SEGMENT	0x1	/* segment of this file to be mapped */
#define	LC_SYMTAB	0x2	/* link-edit stab symbol table info */
#define	LC_SYMSEG	0x3	/* link-edit gdb symbol table info (obsolete) */
#define	LC_THREAD	0x4	/* thread */
#define	LC_UNIXTHREAD	0x5	/* unix thread (includes a stack) */
#define	LC_LOADFVMLIB	0x6	/* load a specified fixed VM shared library */
#define	LC_IDFVMLIB	0x7	/* fixed VM shared library identification */
#define	LC_IDENT	0x8	/* object identification info (obsolete) */
#define LC_FVMFILE	0x9	/* fixed VM file inclusion (internal use) */
#define LC_PREPAGE      0xa     /* prepage command (internal use) */
#define	LC_DYSYMTAB	0xb	/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB	0xc	/* load a dynamically linked shared library */
#define	LC_ID_DYLIB	0xd	/* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER 0xe	/* load a dynamic linker */
#define LC_ID_DYLINKER	0xf	/* dynamic linker identification */
#define	LC_PREBOUND_DYLIB 0x10	/* modules prebound for a dynamically */
/*  linked shared library */
#define	LC_ROUTINES	0x11	/* image routines */
#define	LC_SUB_FRAMEWORK 0x12	/* sub framework */
#define	LC_SUB_UMBRELLA 0x13	/* sub umbrella */
#define	LC_SUB_CLIENT	0x14	/* sub client */
#define	LC_SUB_LIBRARY  0x15	/* sub library */
#define	LC_TWOLEVEL_HINTS 0x16	/* two-level namespace lookup hints */
#define	LC_PREBIND_CKSUM  0x17	/* prebind checksum */

/*
 * load a dynamically linked shared library that is allowed to be missing
 * (all symbols are weak imported).
 */
#define	LC_LOAD_WEAK_DYLIB (0x18 | LC_REQ_DYLD)

#define	LC_SEGMENT_64	0x19	/* 64-bit segment of this file to be
				   mapped */
#define	LC_ROUTINES_64	0x1a	/* 64-bit image routines */
#define LC_UUID		0x1b	/* the uuid */
#define LC_RPATH       (0x1c | LC_REQ_DYLD)    /* runpath additions */
#define LC_CODE_SIGNATURE 0x1d	/* local of code signature */
#define LC_SEGMENT_SPLIT_INFO 0x1e /* local of info to split segments */
#define LC_REEXPORT_DYLIB (0x1f | LC_REQ_DYLD) /* load and re-export dylib */
#define	LC_LAZY_LOAD_DYLIB 0x20	/* delay load of dylib until first use */
#define	LC_ENCRYPTION_INFO 0x21	/* encrypted segment information */
#define	LC_DYLD_INFO 	0x22	/* compressed dyld information */
#define	LC_DYLD_INFO_ONLY (0x22|LC_REQ_DYLD)	/* compressed dyld information only */

/*
 * The 64-bit segment load command indicates that a part of this file is to be
 * mapped into a 64-bit task's address space.  If the 64-bit segment has
 * sections then section_64 structures directly follow the 64-bit segment
 * command and their size is reflected in cmdsize.
 */
struct segment_command_64 { /* for 64-bit architectures */
    uint32_t	cmd;		/* LC_SEGMENT_64 */
    uint32_t	cmdsize;	/* includes sizeof section_64 structs */
    char		segname[16];	/* segment name */
    uint64_t	vmaddr;		/* memory address of this segment */
    uint64_t	vmsize;		/* memory size of this segment */
    uint64_t	fileoff;	/* file offset of this segment */
    uint64_t	filesize;	/* amount to map from the file */
    vm_prot_t	maxprot;	/* maximum VM protection */
    vm_prot_t	initprot;	/* initial VM protection */
    uint32_t	nsects;		/* number of sections in segment */
    uint32_t	flags;		/* flags */
};

struct section_64 { /* for 64-bit architectures */
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	uint64_t	addr;		/* memory address of this section */
	uint64_t	size;		/* size in bytes of this section */
	uint32_t	offset;		/* file offset of this section */
	uint32_t	align;		/* section alignment (power of 2) */
	uint32_t	reloff;		/* file offset of relocation entries */
	uint32_t	nreloc;		/* number of relocation entries */
	uint32_t	flags;		/* flags (section type and attributes)*/
	uint32_t	reserved1;	/* reserved (for offset or index) */
	uint32_t	reserved2;	/* reserved (for count or sizeof) */
	uint32_t	reserved3;	/* reserved */
};

/*
 * A program that uses a dynamic linker contains a dylinker_command to identify
 * the name of the dynamic linker (LC_LOAD_DYLINKER).  And a dynamic linker
 * contains a dylinker_command to identify the dynamic linker (LC_ID_DYLINKER).
 * A file can have at most one of these.
 * This struct is also used for the LC_DYLD_ENVIRONMENT load command and
 * contains string for dyld to treat like environment variable.
 */
struct dylinker_command {
    uint32_t	cmd;		/* LC_ID_DYLINKER, LC_LOAD_DYLINKER or
					   LC_DYLD_ENVIRONMENT */
    uint32_t	cmdsize;	/* includes pathname string */
    uint32_t    name;		/* dynamic linker's path name */
};

/* Constants for the flags field of the mach_header */
#define	MH_NOUNDEFS	0x1		/* the object file has no undefined
					   references */
#define	MH_INCRLINK	0x2		/* the object file is the output of an
					   incremental link against a base file
					   and can't be link edited again */
#define MH_DYLDLINK	0x4		/* the object file is input for the
					   dynamic linker and can't be staticly
					   link edited again */
#define MH_BINDATLOAD	0x8		/* the object file's undefined
					   references are bound by the dynamic
					   linker when loaded. */
#define MH_PREBOUND	0x10		/* the file has its dynamic undefined
					   references prebound. */
#define MH_SPLIT_SEGS	0x20		/* the file has its read-only and
					   read-write segments split */
#define MH_LAZY_INIT	0x40		/* the shared library init routine is
					   to be run lazily via catching memory
					   faults to its writeable segments
					   (obsolete) */
#define MH_TWOLEVEL	0x80		/* the image is using two-level name
					   space bindings */
#define MH_FORCE_FLAT	0x100		/* the executable is forcing all images
					   to use flat name space bindings */
#define MH_NOMULTIDEFS	0x200		/* this umbrella guarantees no multiple
					   defintions of symbols in its
					   sub-images so the two-level namespace
					   hints can always be used. */
#define MH_NOFIXPREBINDING 0x400	/* do not have dyld notify the
					   prebinding agent about this
					   executable */
#define MH_PREBINDABLE  0x800           /* the binary is not prebound but can
					   have its prebinding redone. only used
                                           when MH_PREBOUND is not set. */
#define MH_ALLMODSBOUND 0x1000		/* indicates that this binary binds to
                                           all two-level namespace modules of
					   its dependent libraries. only used
					   when MH_PREBINDABLE and MH_TWOLEVEL
					   are both set. */
#define MH_SUBSECTIONS_VIA_SYMBOLS 0x2000/* safe to divide up the sections into
					    sub-sections via symbols for dead
					    code stripping */
#define MH_CANONICAL    0x4000		/* the binary has been canonicalized
					   via the unprebind operation */
#define MH_WEAK_DEFINES	0x8000		/* the final linked image contains
					   external weak symbols */
#define MH_BINDS_TO_WEAK 0x10000	/* the final linked image uses
					   weak symbols */

#define MH_ALLOW_STACK_EXECUTION 0x20000/* When this bit is set, all stacks
					   in the task will be given stack
					   execution privilege.  Only used in
					   MH_EXECUTE filetypes. */
#define	MH_DEAD_STRIPPABLE_DYLIB 0x400000 /* Only for use on dylibs.  When
					     linking against a dylib that
					     has this bit set, the static linker
					     will automatically not create a
					     LC_LOAD_DYLIB load command to the
					     dylib if no symbols are being
					     referenced from the dylib. */
#define MH_ROOT_SAFE 0x40000           /* When this bit is set, the binary
					  declares it is safe for use in
					  processes with uid zero */

#define MH_SETUID_SAFE 0x80000         /* When this bit is set, the binary
					  declares it is safe for use in
					  processes when issetugid() is true */

#define MH_NO_REEXPORTED_DYLIBS 0x100000 /* When this bit is set on a dylib,
					  the static linker does not need to
					  examine dependent dylibs to see
					  if any are re-exported */
#define	MH_PIE 0x200000			/* When this bit is set, the OS will
					   load the main executable at a
					   random address.  Only used in
					   MH_EXECUTE filetypes. */
#define SG_PROTECTED_VERSION_1 0x08

/* mach-o/fat.h */

#define FAT_MAGIC	0xcafebabe
#define FAT_CIGAM	0xbebafeca

struct fat_header {
    int32_t magic;		/* FAT_MAGIC */
    int32_t nfat_arch;	/* number of structs that follow */
};

struct fat_arch {
    cpu_type_t cputype;	/* cpu specifier (int) */
    cpu_subtype_t cpusubtype;	/* machine specifier (int) */
    unsigned long	offset;		/* file offset to this object file */
    unsigned long	size;		/* size of this object file */
    unsigned long	align;		/* alignment as a power of 2 */
};

/* osfmk/mach/i386/thread_status.h */
#define x86_THREAD_STATE64		4

/* osfmk/mach/i386/_structs.h */
struct __darwin_x86_thread_state64 {
    uint64_t	rax;
    uint64_t	rbx;
    uint64_t	rcx;
    uint64_t	rdx;
    uint64_t	rdi;
    uint64_t	rsi;
    uint64_t	rbp;
    uint64_t	rsp;
    uint64_t	r8;
    uint64_t	r9;
    uint64_t	r10;
    uint64_t	r11;
    uint64_t	r12;
    uint64_t	r13;
    uint64_t	r14;
    uint64_t	r15;
    uint64_t	rip;
    uint64_t	rflags;
    uint64_t	cs;
    uint64_t	fs;
    uint64_t	gs;
};

/* osfmk/mach/i386/vm_param.h */
#define VM_USRSTACK64		0x00007FFEEFC00000UL


/* osfmk/i386/cpu_capabilities.h */
#define	_COMM_PAGE64_AREA_LENGTH	( 1 * 4096 )			/* reserved length of entire comm area (2MB) */
#ifdef __ASSEMBLER__
#define _COMM_PAGE64_BASE_ADDRESS	( 0x00007fffffe00000 )   /* base address of allocated memory */
#else /* __ASSEMBLER__ */
#define _COMM_PAGE64_BASE_ADDRESS	( 0x00007fffffe00000ULL )   /* base address of allocated memory */
#endif /* __ASSEMBLER__ */
#define _COMM_PAGE64_START_ADDRESS	( _COMM_PAGE64_BASE_ADDRESS )	/* address traditional commpage code starts on */
#define _COMM_PAGE64_AREA_USED		( 1 * 4096 )				/* this is the amt actually populated */
