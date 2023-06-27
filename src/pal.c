#include "pal.h"
#include "dma.h"

extern short background_palette;
extern short background_palette_length;
extern short object_palette;
extern short object_palette_length;

void pal_init() {
  dma_push(1, &background_palette, background_palette_length, MEM_BG_PAL);
  dma_push(1, &object_palette, object_palette_length, MEM_OBJ_PAL);
  dma_on(1);
}

