#pragma once
#include <stdbool.h>
#include <stdint.h>

#define STATE_SIGNL 0b10000000
#define STATE_PAUSE 0b01000000
#define CNT_MASK 0x0F
#define LEN_GAP 1
#define LEN_SHORT_GAP 3
#define LEN_MEDIUM_GAP 7
#define LEN_DASH (3 + LEN_GAP)
#define LEN_DOT (1 + LEN_GAP)
#define DOT        (STATE_SIGNL | LEN_DOT)
#define DASH       (STATE_SIGNL | LEN_DASH)
#define GAP        (STATE_PAUSE | LEN_GAP)
#define SHORT_GAP  (STATE_PAUSE | LEN_SHORT_GAP)
#define MEDIUM_GAP (STATE_PAUSE | LEN_MEDIUM_GAP)
#define EOT 4

bool decode_morse_char(char c, uint8_t * morse_signals);
