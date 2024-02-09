#pragma once
#include <stdbool.h>
#include <stdint.h>

#define STATE_SIGNL 0b10000000
#define STATE_PAUSE 0b01000000
#define CNT_MASK 0x0F

#define DOT        (STATE_SIGNL | 1)
#define DASH       (STATE_SIGNL | 3)
#define GAP        (STATE_PAUSE | 1)
#define SHORT_GAP  (STATE_PAUSE | 3)
#define MEDIUM_GAP (STATE_PAUSE | 7)

bool decode_morse_char(char c, uint8_t * morse_signals);
