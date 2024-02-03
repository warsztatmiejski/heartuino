#pragma once
#include <stdbool.h>
#include <stdint.h>

#define STATE_SIGNL 0b10000000
#define STATE_PAUSE 0b01000000
#define CNT_MASK 0x0F

bool decode_morse_char(char c, uint8_t * morse_signals);
