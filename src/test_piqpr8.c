#include <stdio.h>

#include "pifs_backend.h"

static int check_bytes(const char *name,
                       unsigned char (*get_byte)(int),
                       const unsigned char *expected,
                       int count)
{
  for (int i = 0; i < count; i++) {
    unsigned char actual = get_byte(i);
    if (actual != expected[i]) {
      fprintf(stderr,
              "%s byte %d: expected 0x%02x, got 0x%02x\n",
              name, i, expected[i], actual);
      return 1;
    }
  }

  return 0;
}

static int check_backend(const char *name,
                         const struct pifs_backend *backend,
                         unsigned char (*expected_get_byte)(int))
{
  if (!backend) {
    fprintf(stderr, "%s backend: expected match, got NULL\n", name);
    return 1;
  }

  if (backend->get_byte != expected_get_byte) {
    fprintf(stderr, "%s backend: unexpected byte generator\n", name);
    return 1;
  }

  return 0;
}

int main(void)
{
  const struct pifs_backend *default_backend = pifs_default_backend();
  const struct pifs_backend *pi_backend = pifs_find_backend("pi");
  const struct pifs_backend *pi_symbol_backend = pifs_find_backend("π");
  const struct pifs_backend *e_backend = pifs_find_backend("e");

  static const unsigned char expected_pi[] = {
    0x24, 0x43, 0x3f, 0xf6, 0x6a, 0xa8, 0x88, 0x88,
    0x85, 0x5a, 0xa3, 0x30, 0x08, 0x8d, 0xd3, 0x31,
  };
  static const unsigned char expected_e[] = {
    0xb7, 0x7e, 0xe1, 0x15, 0x51, 0x16, 0x62, 0x28,
    0x8a, 0xae, 0xed, 0xd2, 0x2a, 0xa6, 0x6a, 0xab,
  };

  if (pifs_find_backend(NULL) != default_backend ||
      pifs_find_backend("") != default_backend) {
    fprintf(stderr, "default backend lookup failed\n");
    return 1;
  }

  if (pi_backend != default_backend || pi_symbol_backend != default_backend) {
    fprintf(stderr, "pi aliases did not resolve to the default backend\n");
    return 1;
  }

  if (pifs_find_backend("PI") != default_backend ||
      pifs_find_backend("E") != e_backend) {
    fprintf(stderr, "case-insensitive backend lookup failed\n");
    return 1;
  }

  if (pifs_find_backend("nope")) {
    fprintf(stderr, "unknown backend unexpectedly resolved\n");
    return 1;
  }

  if (check_backend("pi", pi_backend, get_pi_byte) ||
      check_backend("e", e_backend, get_e_byte)) {
    return 1;
  }

  if (check_bytes("pi", get_pi_byte, expected_pi,
                  sizeof expected_pi / sizeof expected_pi[0])) {
    return 1;
  }

  if (check_bytes("e", get_e_byte, expected_e,
                  sizeof expected_e / sizeof expected_e[0])) {
    return 1;
  }

  return 0;
}
