#ifndef SND_H_
#define SND_H_

#include <stdint.h>

#define REG_SOUND1CNT_L *(volatile uint16_t*)0x4000060
#define REG_SOUND1CNT_H *(volatile uint16_t*)0x4000062
#define REG_SOUND1CNT_X *(volatile uint32_t*)0x4000064

#define REG_SOUND2CNT_L *(volatile uint16_t*)0x4000068
#define REG_SOUND2CNT_H *(volatile uint32_t*)0x400006C

#define REG_SOUND3CNT_L *(volatile uint16_t*)0x4000070
#define REG_SOUND3CNT_H *(volatile uint16_t*)0x4000072
#define REG_SOUND3CNT_X *(volatile uint32_t*)0x4000074

#define REG_SOUND4CNT_L *(volatile uint16_t*)0x4000078
#define REG_SOUND4CNT_H *(volatile uint32_t*)0x400007C

#define REG_SOUNDCNT_L  *(volatile uint16_t*)0x4000080
#define REG_SOUNDCNT_H  *(volatile uint16_t*)0x4000082
#define REG_SOUNDCNT_X  *(volatile uint32_t*)0x4000084

#define REG_SOUNDBIAS   *(volatile uint16_t*)0x4000088

#define WAVE_RAM0_L     *(volatile uint16_t*)0x4000090
#define WAVE_RAM0_H     *(volatile uint16_t*)0x4000092
#define WAVE_RAM1_L     *(volatile uint16_t*)0x4000094
#define WAVE_RAM1_H     *(volatile uint16_t*)0x4000096
#define WAVE_RAM2_L     *(volatile uint16_t*)0x4000098
#define WAVE_RAM2_H     *(volatile uint16_t*)0x400009A
#define WAVE_RAM3_L     *(volatile uint16_t*)0x400009C
#define WAVE_RAM3_H     *(volatile uint16_t*)0x400009E

#define TWO_BANKS       (1 << 5);

void snd_init();
void trigger_sound();
void set_step(uint32_t set_step);

#endif // SND_H_
