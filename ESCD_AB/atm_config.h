#ifndef _ATM_CONFIG_H_
#define _ATM_CONFIG_H_

// Waveforms are fixed at compile time. Change these, then recompile.
// ATM_WAVE_PULSE  0
// ATM_WAVE_SQUARE 1
// ATM_WAVE_NOISE  2   (use on channel 3; ATM_NOISE() is a song macro)
// ATM_WAVE_SAW    3
// ATM_WAVE_TRI    4

#ifndef ATM_WAVE_PULSE
#define ATM_WAVE_PULSE  0
#define ATM_WAVE_SQUARE 1
#define ATM_WAVE_NOISE  2
#define ATM_WAVE_SAW    3
#define ATM_WAVE_TRI    4
#endif

#ifndef ATM_WAVE_CH0
#define ATM_WAVE_CH0 ATM_WAVE_PULSE
#endif
#ifndef ATM_WAVE_CH1
#define ATM_WAVE_CH1 ATM_WAVE_SQUARE
#endif
#ifndef ATM_WAVE_CH2
#define ATM_WAVE_CH2 ATM_WAVE_PULSE
#endif
#ifndef ATM_WAVE_CH3
#define ATM_WAVE_CH3 ATM_WAVE_NOISE
#endif

// 0 = stock Arduboy speaker on OCR4A only
// 1 = also drive OCR4D (homemade / dual-pin wiring, same as old AB_ALTERNATE_WIRING)
#ifndef ATM_ALT_WIRING
#define ATM_ALT_WIRING 0
#endif

#endif
