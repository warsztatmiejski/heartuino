// CONFIG

#define STEP 250 /* ms */
#define NAME "sos"
// from 0x00 to 0x60
#define SOUNDS {0x01, 0x05, 0x20, 0x02, 0x03}

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "morse.h"

#define TICK 16
#define STEP_IN_TICKS (STEP / TICK)

static volatile bool _global_clicked;
static volatile uint8_t _global_sunny;
static volatile uint8_t _global_time;

static inline void setup() {
  // Setup globals
  _global_time = 0;
  _global_sunny = 0;
  _global_clicked = false;

  // Output: 0, 3, 4; each initially as high.
  // Input: 1, 2; 1 with pull-up.
  DDRB = (1 << DDB0) | (1 << DDB3) | (1 << DDB4);
  PORTB = (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4);

  // Timer with sending signal to PB0.
  TCCR0A = (1 << COM0A0) | (1 << WGM01);
  TCCR0B = (1 << FOC0A) | (1 << CS01);
  TIMSK0 = 0;

  // ADC1 on the port PB2 with interrupt.
  ADMUX = (1 << ADLAR) | (1 << MUX0);
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) |
           (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  ADCSRB = 0;
  DIDR0 = (1 << ADC1D);

  // Pin PB1 interrupt.
  MCUCR = (1 << ISC01); // 1 -> 0 triggers.
  GIMSK = (1 << INT0); // Interrupt on.

  // Watchdog (used as timer)
  WDTCR = (1 << WDTIE);
}

ISR(INT0_vect) {
  _delay_ms(50);
  _global_clicked = !_global_clicked;
}

ISR(ADC_vect) {
  _global_sunny = ADCH;
}

ISR(WDT_vect) {
  // 1 tick ~= 16ms
  _global_time++;
}

static inline void sound_off() {
  TCCR0B = (1 << FOC0A);
}

static inline void sound_on(uint8_t sound) {
  TCCR0B = (1 << FOC0A) | (1 << CS01);
  OCR0A = sound;
}

static void light() {
  if (_global_sunny < 200) {
    PORTB = (1 << PB1);
  } else if (_global_clicked) {
    PORTB = (1 << PB3) | (1 << PB1);
  } else {
    PORTB = (1 << PB4) | (1 << PB1);
  }
}

static void dark() {
  PORTB = (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4);
}

static const char name[] PROGMEM = NAME;
static const uint8_t sounds[] PROGMEM = SOUNDS;

static inline void on_step() {
  static uint8_t name_index = 0;
  static uint8_t signal_counter = 0;
  static uint8_t signal_index = 0;
  static uint8_t sound_index = 0;
  static uint8_t morse_signals[12] = {0};

  uint8_t signal;
  while (!(signal = morse_signals[signal_index])) {
    uint8_t character = pgm_read_byte(&name[name_index]);
    decode_morse_char(character, morse_signals);

    signal_index = 0;
    signal_counter = 0;
    if (++name_index >= sizeof(name)) {
      name_index = 0;
    }
  }

  if (signal & STATE_SIGNL) {
    light();
    uint8_t sound = pgm_read_byte(&sounds[sound_index]);
    sound_on(sound);
  } else if (signal & STATE_PAUSE) {
    dark();
    sound_off();
  }

  uint8_t cnt = signal & CNT_MASK;
  if (signal_counter++ >= cnt) {
    signal_counter = 0;
    signal_index++;
    if (signal & STATE_SIGNL) {
      if (++sound_index >= sizeof(sounds)) {
        sound_index = 0;
      }
    }
  }
}

int main() {
  setup();
  sleep_enable();

  for(;;) {
    cli();
    if (_global_time > STEP_IN_TICKS) {
      _global_time = 0;
      on_step();
    }

    sei();
    sleep_cpu();
  }
  return 0;
}
