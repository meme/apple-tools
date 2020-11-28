#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "cache_format.h"
#include "macho.h"

static int
segment_range(const char *path, uint64_t address, uintptr_t result)
{
    char *base = (char*) address;
    struct mach_header_64 *header = (struct mach_header_64*) base;
    assert(header->magic == MH_MAGIC_64);

    uint32_t offset = sizeof(struct mach_header_64);

    for (uint32_t i = 0; i < header->ncmds; i++) {
        struct load_command *command = (struct load_command*) (base + offset);

        if (command->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *segment = (struct segment_command_64*)
                command;

            uintptr_t start = segment->vmaddr;
            uintptr_t end = start + segment->vmsize;
            if (result >= start && result <= end) {
                printf("%s:%.*s (0x%08lx-0x%08lx)\n", path, (int) sizeof(segment->segname),
                        segment->segname, segment->vmaddr, segment->vmaddr + segment->vmsize);
                return 1;
            }
        }

        offset += command->cmdsize;
    }

    return 0;
}

static void
search(cache_header_t *header, cache_mapping_t *mapping, const char *needle)
{
    char *base = (char*) mapping->address;
    uint32_t needle_size = strlen(needle);

    char *result = base;
    while ((result = memmem(result, mapping->size - ((uintptr_t) result - mapping->address), needle, needle_size)) != NULL) {
        cache_image_t *image = (cache_image_t*) ((char*) header + header->images_offset);
        for (uint32_t i = 0; i < header->images_count; i++) {
            const char *path = (char*) header + image->path_offset;
            // If a result was found, stop searching.
            //
            if (segment_range(path, image->address, (uintptr_t) result)) {
                break;
            }
            image++;
        }

        printf("\t%p:%.*s\n", result, needle_size, result);
        result += needle_size;
    }
}

int
main(int argc, char *argv[])
{
    int f = 0;
    struct stat s;
    uint8_t *base = NULL;

    if (argc < 3) {
        return 1;
    }

    f = open(argv[2], O_RDONLY);
    if (f < 0) {
        perror("open");
        return 1;
    }

    if (fstat(f, &s) < 0) {
        perror("fstat");
        goto error;
    }

    base = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, f, 0);
    if (base == MAP_FAILED) {
        perror("mmap");
        goto error;
    }

    cache_header_t *header = (cache_header_t*) base;
    assert(memcmp(header->magic, "dyld_v1  arm64e", sizeof(header->magic)) == 0);

    cache_mapping_t *mapping = (cache_mapping_t*) (base + header->mapping_offset);

    for (uint32_t i = 0; i < header->mapping_count; i++) {
        // Map each mapping into the specified address. This IS NOT SECURE.
        //
        void *cache_map = mmap((void*) mapping->address, mapping->size, PROT_READ,
                MAP_PRIVATE | MAP_FIXED, f, mapping->file_offset);
        if (cache_map == MAP_FAILED) {
            perror("mmap");
            goto error;
        }

        search(header, mapping, argv[1]);

        mapping++;
    }

    // Remove all mappings.
    //
    mapping = (cache_mapping_t*) (base + header->mapping_offset);

    for (uint32_t i = 0; i < header->mapping_count; i++) {
        munmap((void*) mapping->address, mapping->size);
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
