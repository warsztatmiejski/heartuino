#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "morse.h"

static volatile bool _global_clicked;
static volatile bool _global_lighted;
static volatile uint16_t _global_time;

static inline void setup() {
  // Setup globals
  _global_time = 0;
  _global_clicked = false;
  _global_lighted = false;

  // Output: 0, 3, 4; each initially as high.
  // Input: 1, 2; 1 with pull-up.
  DDRB = (1 << DDB0) | (1 << DDB3) | (1 << DDB4);
  PORTB = (1 << PB0) | (1 << PB1) | (1 << PB3) | (1 << PB4);

  // Timer with sending signal to PB0.
  TCCR0A = (1 << COM0A0) | (1 << WGM01);
  TCCR0B = (1 << FOC0A) | (1 << CS02); // clk / 1024.
  TIMSK0 = (1 << OCIE0A);
  OCR0A = 127;

  // ADC1 on the port PB2 with interrupt.
  ADMUX = (1 << MUX0);
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) |
           (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
  ADCSRB = 0;
  DIDR0 = (1 << ADC1D);

  // Pin PB1 interrupt.
  MCUCR = (1 << ISC01); // 1 -> 0 triggers.
  GIMSK = (1 << INT0); // Interrupt on.
}

ISR(INT0_vect) {
  _delay_ms(50);
  _global_clicked = !_global_clicked;
}

ISR(ADC_vect) {
  uint16_t level = (ADCH << 8) | ADCL;
  _global_lighted = level > 800;
}

ISR(TIM0_COMPA_vect) {
  _global_time += OCR0A;
}

static void light() {
  if (_global_lighted) {
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

const char * name = "marek";

static uint8_t name_index = 0;
static uint8_t morse_index = 0;
static uint8_t signal_index = 0;
static uint8_t morse_signals[12] = {0};
static uint8_t sound = 0;

static inline void on_tick() {
  sound += 8;

  uint8_t signal = morse_signals[morse_index];

  if (!signal) {
    morse_index = 0;
    signal_index = 0;
    if (++name_index >= sizeof(name)) {
      name_index = 0;
    }
    decode_morse_char(name[name_index], morse_signals);
    return;
  }

  uint8_t cnt = signal & CNT_MASK;
  if (signal & STATE_SIGNL) {
    light();
  } else if (signal & STATE_PAUSE) {
    dark();
  }

  if (++morse_index >= cnt) {
    morse_index = 0;
    signal_index++;
  }
}

int main() {
  setup();
  sleep_enable();

  for(;;) {
    cli();
    if (_global_time > F_CPU / (1024 * 4)) {
      _global_time = 0;
      on_tick();
    }

    sei();
    sleep_cpu();
  }
  return 0;
}
