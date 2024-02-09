#include "morse.h"
#ifdef __AVR__
  #include <avr/pgmspace.h>
  #define _MORSE_MEMORY PROGMEM
  #define _MORSE_SIZE(state) pgm_read_byte(&SIZE[state])
  #define _MORSE_LOAD(state, signal) { \
    signal = pgm_read_byte(&NEXT_SIGNAL[state]); \
    state = pgm_read_byte(&NEXT_STATE[state]); \
  }
#else
  #define _MORSE_MEMORY
  #define _MORSE_SIZE(state) SIZE[state]
  #define _MORSE_LOAD(state, signal) { \
    signal = NEXT_SIGNAL[state]; \
    state = NEXT_STATE[state]; \
  }
#endif

const uint8_t NEXT_STATE[26] _MORSE_MEMORY = {
  /*a*/ 'e' - 'a',
  /*b*/ 'd' - 'a',
  /*c*/ 'k' - 'a',
  /*d*/ 'n' - 'a',
  /*e*/ 0,
  /*f*/ 'u' - 'a',
  /*g*/ 'm' - 'a',
  /*h*/ 's' - 'a',
  /*i*/ 'e' - 'a',
  /*j*/ 'w' - 'a',
  /*k*/ 'n' - 'a',
  /*l*/ 'r' - 'a',
  /*m*/ 't' - 'a',
  /*n*/ 't' - 'a',
  /*o*/ 'm' - 'a',
  /*p*/ 'w' - 'a',
  /*q*/ 'g' - 'a',
  /*r*/ 'a' - 'a',
  /*s*/ 'i' - 'a',
  /*t*/ 0,
  /*u*/ 'i' - 'a',
  /*v*/ 's' - 'a',
  /*w*/ 'a' - 'a',
  /*x*/ 'd' - 'a',
  /*y*/ 'k' - 'a',
  /*z*/ 'g' - 'a',
};

const uint8_t NEXT_SIGNAL[26] _MORSE_MEMORY = {
  /*a*/ DASH,
  /*b*/ DOT,
  /*c*/ DOT,
  /*d*/ DOT,
  /*e*/ DOT,
  /*f*/ DOT,
  /*g*/ DOT,
  /*h*/ DOT,
  /*i*/ DOT,
  /*j*/ DASH,
  /*k*/ DASH,
  /*l*/ DOT,
  /*m*/ DASH,
  /*n*/ DOT,
  /*o*/ DASH,
  /*p*/ DOT,
  /*q*/ DASH,
  /*r*/ DOT,
  /*s*/ DOT,
  /*t*/ DASH,
  /*u*/ DASH,
  /*v*/ DASH,
  /*w*/ DASH,
  /*x*/ DASH,
  /*y*/ DASH,
  /*z*/ DOT,
};

const uint8_t SIZE[26] _MORSE_MEMORY = {
  /*a*/ 2 * 2 - 1,
  /*b*/ 4 * 2 - 1,
  /*c*/ 4 * 2 - 1,
  /*d*/ 3 * 2 - 1,
  /*e*/ 1 * 2 - 1,
  /*f*/ 4 * 2 - 1,
  /*g*/ 3 * 2 - 1,
  /*h*/ 4 * 2 - 1,
  /*i*/ 2 * 2 - 1,
  /*j*/ 4 * 2 - 1,
  /*k*/ 3 * 2 - 1,
  /*l*/ 4 * 2 - 1,
  /*m*/ 2 * 2 - 1,
  /*n*/ 2 * 2 - 1,
  /*o*/ 3 * 2 - 1,
  /*p*/ 4 * 2 - 1,
  /*q*/ 4 * 2 - 1,
  /*r*/ 3 * 2 - 1,
  /*s*/ 3 * 2 - 1,
  /*t*/ 1 * 2 - 1,
  /*u*/ 3 * 2 - 1,
  /*v*/ 4 * 2 - 1,
  /*w*/ 3 * 2 - 1,
  /*x*/ 4 * 2 - 1,
  /*y*/ 4 * 2 - 1,
  /*z*/ 4 * 2 - 1,
};

bool decode_morse_char(char c, uint8_t * morse_signals) {
  if (c == '\0' || c == ' ') {
    morse_signals[0] = MEDIUM_GAP;
    morse_signals[1] = 0;
    return true;
  }

  uint8_t state = c - 'a';
  if (state > 'z' - 'a') {
    morse_signals[0] = 0;
    return false;
  }
  int8_t size = _MORSE_SIZE(state);
  int8_t index = size;
  int8_t signal;

  while(index > 0) {
    _MORSE_LOAD(state, signal);
    morse_signals[--index] = signal;
    if (index > 1) {
      morse_signals[--index] = GAP;
    }
  }
  morse_signals[size] = SHORT_GAP;
  morse_signals[size + 1] = 0;
  return true;
}
