//* ---------- ADDRESSES ---------- */

#define MEM_IO 0x04000000
#define BG0CNT (*((volatile short int *)(MEM_IO + 0x8)))
#define BG1CNT (*((volatile short int *)(MEM_IO + 0xA)))
#define BG2CNT (*((volatile short int *)(MEM_IO + 0xC)))
#define BG3CNT (*((volatile short int *)(MEM_IO + 0xE)))

#define BG0HOFS (*((volatile unsigned char *)(MEM_IO + 0x10)))
#define BG0VOFS (*((volatile unsigned char *)(MEM_IO + 0x12)))

#define REG_DISPLAY (*((volatile unsigned int *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile unsigned int *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT (*((volatile unsigned int *)(MEM_IO + 0x0130)))
#define REG_KEY_CNT (*((volatile unsigned int *)(MEM_IO + 0x0132)))

/* ---------- CONSTANTS ---------- */

#define KEY_A 1
#define KEY_B 2
#define KEY_SELECT 4
#define KEY_START 8
#define KEY_RIGHT 16
#define KEY_LEFT 32
#define KEY_UP 64
#define KEY_DOWN 128
#define KEY_R 256
#define KEY_L 512

#define BG_MODE_0 0
#define BG_MODE_1 1
#define BG_MODE_2 2
#define BG_MODE_3 3
#define BG_MODE_4 4
#define BG_MODE_5 5

#define SCREEN_DISPLAY_BG0 1 << 8
#define SCREEN_DISPLAY_BG1 1 << 9
#define SCREEN_DISPLAY_BG2 1 << 10
#define SCREEN_DISPLAY_BG3 1 << 11
#define SCREEN_DISPLAY_OBJ 1 << 12

#define ONE_DIMENSIONAL_OBJ_CHARACTER_VRAM_MAPPING 1 << 6

#define BGCNT_SCREEN_SIZE_0 0 << 14
#define BGCNT_SCREEN_SIZE_1 1 << 14
#define BGCNT_SCREEN_SIZE_2 2 << 14
#define BGCNT_SCREEN_SIZE_3 3 << 14

#define BGCNT_256_COLORS 1 << 7
#define BGCNT_MOSAIC_ENABLE 1 << 6

#define BGCNT_PRIORITY_0 0
#define BGCNT_PRIORITY_1 1
#define BGCNT_PRIORITY_2 2
#define BGCNT_PRIORITY_3 3

#define KEY_PRESSED_MASK ~REG_KEY_INPUT & 0x3ff
