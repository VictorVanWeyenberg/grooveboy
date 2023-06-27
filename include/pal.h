#ifndef __PAL_H
#define __PAL_H

#define MEM_PAL 0x05000000

#define MEM_BG_PAL ((short *)(MEM_PAL))
#define MEM_OBJ_PAL ((short *)(MEM_PAL + 0x200))

void pal_init();

#endif
