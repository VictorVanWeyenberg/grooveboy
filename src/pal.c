#include "pal.h"
#include "dma.h"
#include "string.h"

extern short background_palette;
extern short background_palette_length;
extern short object_palette;
extern short object_palette_length;

void pal_init() {
  memcpy(MEM_BG_PAL, &background_palette, background_palette_length);
  memcpy(MEM_OBJ_PAL, &object_palette, object_palette_length);
  // dma_push(1, &background_palette, background_palette_length, MEM_BG_PAL);
  // dma_push(1, &object_palette, object_palette_length, MEM_OBJ_PAL);
}
