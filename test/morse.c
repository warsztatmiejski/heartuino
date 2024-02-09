#include <check.h>
#include <string.h>
#include "morse.h"

struct Fixture {
  char character;
  char* signal;
};

struct Fixture FIXTURES[] = {
  {'a', ". -$"},
  {'b', "- . . .$"},
  {'c', "- . - .$"},
  {'d', "- . .$"},
  {'e', ".$"},
  {'f', ". . - .$"},
  {'g', "- - .$"},
  {'h', ". . . .$"},
  {'i', ". .$"},
  {'j', ". - - -$"},
  {'k', "- . -$"},
  {'l', ". - . .$"},
  {'m', "- -$"},
  {'n', "- .$"},
  {'o', "- - -$"},
  {'p', ". - - .$"},
  {'q', "- - . -$"},
  {'r', ". - .$"},
  {'s', ". . .$"},
  {'t', "-$"},
  {'u', ". . -$"},
  {'v', ". . . -$"},
  {'w', ". - -$"},
  {'x', "- . . -$"},
  {'y', "- . - -$"},
  {'z', "- - . .$"},
  {' ', "!"},
  {'\0', "!"},
};


START_TEST(test_decode_morse_char) {
  uint8_t morse_signals[12] = {0};
  struct Fixture *fixture = &FIXTURES[_i];

  bool result = decode_morse_char(fixture->character, morse_signals);
  ck_assert(result);

  uint8_t i;
  uint8_t size = strlen(fixture->signal);
  for(i = 0; i <= size; i++) {
    uint8_t s = morse_signals[i];

#define _assert(X) \
    ck_assert_msg( \
        s == X, \
        "char: %c; pos: %2d; exp: " #X "; get %2x", fixture->character, i, s);

    switch(fixture->signal[i]) {
      case '.': _assert(DOT); break;
      case '-': _assert(DASH); break;
      case ' ': _assert(GAP); break;
      case '$': _assert(SHORT_GAP); break;
      case '!': _assert(MEDIUM_GAP); break;
      case '\0': ck_assert_msg(s == 0); break;
      default: ck_abort(); break;
#undef _assert
    }
  }
}
END_TEST

START_TEST(test_decode_morse_char_fail) {
  uint8_t morse_signals[12] = {DOT};
  char c = _i;

  bool result = decode_morse_char(c, morse_signals);

  ck_assert(!result);
  ck_assert_uint_eq(morse_signals[0], 0);
}
END_TEST

Suite * morse_suite(void) {
  Suite *s = suite_create("Morse");
  TCase *tc = tcase_create("Morse");

  int fixtures_size = sizeof(FIXTURES) / sizeof(struct Fixture);
  tcase_add_loop_test(tc, test_decode_morse_char, 0, fixtures_size);
  tcase_add_loop_test(tc, test_decode_morse_char_fail, '\x01', '\x1F');
  tcase_add_loop_test(tc, test_decode_morse_char_fail, '\x21', '\x60');
  tcase_add_loop_test(tc, test_decode_morse_char_fail, '\x7B', '\xFF');
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  Suite *s = morse_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : -1;
}
