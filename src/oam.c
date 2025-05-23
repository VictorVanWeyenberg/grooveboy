#include "oam.h"

uint8_t object_memory_index = 0;

struct obj_attrs *object_create(uint8_t x, uint8_t y, uint8_t hflip, uint8_t vflip, uint16_t tile_number) {
    struct obj_attrs *oa = &MEM_OAM[object_memory_index++];
    oa->x = x & 0x1ff;
    oa->y = y & 0xff;
    oa->hflip = hflip & 1;
    oa->vflip = vflip & 1;
    oa->tile_number = tile_number & 0x3ff;
    oa->disable = 0;
    return oa;
}
