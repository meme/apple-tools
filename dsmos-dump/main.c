#include <assert.h>
#include <fcntl.h>
#include <openssl/blowfish.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "macho.h"
#include "schedule_data.h"

#define PAGE_SIZE 0x1000

static void
unprotect_segment(uint8_t* base, struct segment_command_64* segment)
{
    uint8_t* scratch = malloc(PAGE_SIZE);

    // First three pages are NOT protected.
    //
    uint8_t* src = base + PAGE_SIZE * 3;
    uint32_t count = (segment->vmsize / PAGE_SIZE) - 3;

    for (uint32_t i = 0; i < count; i++) {
        // Zero-ed IV each run.
        //
        uint8_t iv[8] = {};
        // Unprotect the page into the scratch space and write the unprotected
        // page back to the original page.
        //
        BF_cbc_encrypt(src, scratch, PAGE_SIZE, (BF_KEY*) &schedule_data, iv, BF_DECRYPT);
        memcpy(src, scratch, PAGE_SIZE);

        src += PAGE_SIZE;
    }

    free(scratch);
}

int
main(int argc, char* argv[])
{
    int f = 0;
    struct stat s;
    uint8_t* base = NULL;

    if (argc < 2) {
        return 1;
    }

    f = open(argv[1], O_RDWR);
    if (f < 0) {
        perror("open");
        return 1;
    }

    if (fstat(f, &s) < 0) {
        perror("fstat");
        goto error;
    }

    base = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    if (base == MAP_FAILED) {
        perror("mmap");
        goto error;
    }

    struct mach_header_64* header = (struct mach_header_64*) base;
    assert(header->magic == MH_MAGIC_64);
    assert(header->cputype == CPU_TYPE_X86_64);
    assert(header->cpusubtype == CPU_SUBTYPE_X86_64_ALL);

    uint32_t offset = sizeof(struct mach_header_64);

    for (uint32_t i = 0; i < header->ncmds; i++) {
        struct load_command* command = (struct load_command*) (base + offset);

        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64* segment = (struct segment_command_64*) command;
            if (segment->flags & SG_PROTECTED_VERSION_1) {
                assert((segment->vmsize % PAGE_SIZE) == 0);
                unprotect_segment(base, segment);
                segment->flags &= ~SG_PROTECTED_VERSION_1;
            }
        }

        offset += command->cmdsize;
    }

    munmap(base, s.st_size);
    close(f);
    return 0;
error:
    if (base)
        munmap(base, s.st_size);
    if (f)
        close(f);
    return 1;
}
