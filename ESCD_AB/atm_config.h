#ifndef _ATM_CONFIG_H_
#define _ATM_CONFIG_H_

// Waveforms are fixed at compile time. Change these, then recompile.
// Do not use ATM_NOISE here — that name is a song-data macro.
// 0 = pulse, 1 = square, 2 = noise (keep noise on channel 3)

#ifndef ATM_WAVE_PULSE
#define ATM_WAVE_PULSE  0
#define ATM_WAVE_SQUARE 1
#define ATM_WAVE_NOISE  2
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

#endif
