/*
--------------------------------------------------------------------------------
Direct Sound example source code
By Uze, 2001
http://www.BeLogic.com
You may freely distribute, modify or use this source code without any limitations.

-Created in Visual Studio C++ 6.0
-Compiles with GCC's build "DevKitAdvance"
-Uses Jeff's crt0.s and lnkscript (Crtls v1.26) from http://devrs.com/gba

Don't forget to update the makefile to point to your directories.
Give me at least credits if this code is used into another tutorial.
--------------------------------------------------------------------------------
*/

#include "gba.h"
#include "kick.h"

void InterruptProcess(void); // the interrupt handle from crt0.s
void ianfinlayson(void);

/* make defines for the bit positions of the control register */
#define TIMER_FREQ_1 0x0
#define TIMER_ENABLE 0x80

/* the GBA clock speed is fixed at this rate */
#define CLOCK 16777216 
#define CYCLES_PER_BLANK 280806

/* turn DMA on for different sizes */
#define DMA_ENABLE 0x80000000
#define DMA_32 0x04000000

/* this causes the DMA destination to be the same each time rather than increment */
#define DMA_DEST_FIXED 0x400000

/* this causes the DMA to repeat the transfer automatically on some interval */
#define DMA_REPEAT 0x2000000

/* this causes the DMA repeat interval to be synced with timer 0 */
#define DMA_SYNC_TO_TIMER 0x30000000

/* bit patterns for the sound control register */
#define SOUND_A_RIGHT_CHANNEL 0x100
#define SOUND_A_LEFT_CHANNEL 0x200
#define SOUND_A_FIFO_RESET 0x800

/* allows turning on and off sound for the GBA altogether */
#define SOUND_MASTER_ENABLE 0x80

volatile unsigned char* fifo_buffer_a  = (volatile unsigned char*) 0x40000A0;

void InterruptProcess(void)
{
	if (REG_IF & INT_TIMER1)
	{
		// clear the interrupt(s)
		REG_IF |= INT_TIMER1;
		// Sample finished! Stop DMA mode Direct sound
		REG_TM0CNT_H = 0;  // disable timer 0
		REG_DMA1CNT_H = 0; // stop DMA
		// ianfinlayson();
	}
	if (REG_IF & INT_TIMER2)
	{
		REG_IF |= INT_TIMER2;
		ianfinlayson();
	}
}

int main(void)
{
	REG_IME = 0x00;
	REG_INTERRUPT = &InterruptProcess;
	REG_IE = 0x30;	// enable irq for timer 1 overflow
	REG_IME = 0x01; // enable interrupt

	while (REG_VCOUNT >= 160)
		; // wait till VDraw
	while (REG_VCOUNT < 160)
		; // wait till VBlank

	REG_TM2D = 1;
	REG_TM2CNT = 0xC2; // enable timer2 + irq

	// ianfinlayson();

	//	play a sound using timer 0 as sampling source
	//	When timer 0 overflows, the interrupt handler loads the FIFO with the next sample
	while (1);
}

// https://ianfinlayson.net/class/cpsc305/samples/gba/sound/sound.c
void ianfinlayson(void) {
	const int sample_rate = 16000;
    /* start by disabling the timer and dma controller (to reset a previous sound) */
	REG_DMA1CNT = 0;
    REG_TM0CNT = 0;

    /* output to both sides and reset the FIFO */
	REG_SOUNDCNT_H |= SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET;

    /* enable all sound */
    REG_SOUNDCNT_X = SOUND_MASTER_ENABLE;

    /* set the dma channel to transfer from the sound array to the sound buffer */
	REG_DMA1SAD = (unsigned int) kick;
	REG_DMA1DAD = 0x40000A0;
	REG_DMA1CNT = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;

    /* set the timer so that it increments once each time a sample is due
     * we divide the clock (ticks/second) by the sample rate (samples/second)
     * to get the number of ticks/samples */
    unsigned short ticks_per_sample = CLOCK / sample_rate;

	// Timer 1 overflow.
	REG_TM1CNT_L = 0xffff - kick_size; // 0xffff-the number of samples to play
	REG_TM1CNT_H = 0xC4;				 // enable timer1 + irq and cascade from timer 0

    /* the timers all count up to 65536 and overflow at that point, so we count up to that
     * now the timer will trigger each time we need a sample, and cause DMA to give it one! */
    REG_TM0CNT = 65536 - ticks_per_sample;

    /* enable the timer */
    REG_TM0CNT_H = TIMER_ENABLE | TIMER_FREQ_1;
}