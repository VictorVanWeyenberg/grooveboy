#ifndef LNK_H_
#define LNK_H_

#include <stdint.h>

#define SIOCNT    *(volatile uint16_t*)0x4000128
#define SIODATA32 *(volatile uint32_t*)0x4000120

struct lnk_process {
    void *data;
    uint32_t length;
    uint32_t index;
};

struct lnk_dq {
	uint8_t max_size;
	uint8_t size;
	uint8_t first;
	struct lnk_process **head;
};

void lnk_init();
void lnk_interrupt();
void send(void *data, uint32_t length);
uint32_t recv();

#endif // LNK_H_
