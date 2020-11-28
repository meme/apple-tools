#pragma once

#include <stdint.h>

typedef struct {
    char magic[16];

    uint32_t mapping_offset;
    uint32_t mapping_count;

    uint32_t images_offset;
    uint32_t images_count;

    uint64_t base_address;

    uint64_t code_sig_offset;
    uint64_t code_sig_size;

    uint64_t slide_info_offset;
    uint64_t slide_info_size;
} cache_header_t;

typedef struct {
    uint64_t address;
    uint64_t size;
    uint64_t file_offset;
    uint32_t max_prot;
    uint32_t init_prot;
} cache_mapping_t;

typedef struct {
    uint64_t address;
    uint64_t mod_time;
    uint64_t inode;
    uint32_t path_offset;
    uint32_t unused;
} cache_image_t;

