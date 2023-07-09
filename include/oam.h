#ifndef OAM_H_
#define OAM_H_

#include <stdint.h>

struct obj_attrs {
    volatile uint16_t
    y: 8,
        rotation_scaling: 1,
        disable: 1,
        mode: 2,
        mosaic: 1,
        palettes: 1,
        shape: 2;
    volatile uint16_t
    x: 9,
        not_used: 3,
        hflip: 1,
        vflip: 1,
        size: 2;
    volatile uint16_t
    tile_number: 10,
        priority: 2,
        palette_number: 4;
} __attribute__((packed, aligned(4)));

#define MEM_OAM ((struct obj_attrs *) 0x07000000)

struct obj_attrs *object_create(uint8_t x, uint8_t y, uint8_t hflip, uint8_t vflip, uint16_t number_tile);

#endif // OAM_H_
