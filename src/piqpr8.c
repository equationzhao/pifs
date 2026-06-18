/*  
    This program implements the BBP algorithm to generate a few hexadecimal
    digits beginning immediately after a given position id, or in other words
    beginning at position id + 1.  On most systems using IEEE 64-bit floating-
    point arithmetic, this code works correctly so long as d is less than
    approximately 1.18 x 10^7.  If 80-bit arithmetic can be employed, this limit
    is significantly higher.  Whatever arithmetic is used, results for a given
    position id can be checked by repeating with id-1 or id+1, and verifying 
    that the hex digits perfectly overlap with an offset of one, except possibly
    for a few trailing digits.  The resulting fractions are typically accurate 
    to at least 11 decimal digits, and to at least 9 hex digits.  
*/

/*  David H. Bailey     2006-09-08 */

#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>

#include "pifs_backend.h"

static double expm (double p, double ak)

/*  expm = 16^p mod ak.  This routine uses the left-to-right binary 
    exponentiation scheme. */

{
  int i, j;
  double p1, pt, r;
#define ntp 25
  static double tp[ntp];
  static int tp1 = 0;

/*  If this is the first call to expm, fill the power of two table tp. */

  if (tp1 == 0) {
    tp1 = 1;
    tp[0] = 1.;

    for (i = 1; i < ntp; i++) tp[i] = 2. * tp[i-1];
  }

  if (ak == 1.) return 0.;

/*  Find the greatest power of two less than or equal to p. */

  for (i = 0; i < ntp; i++) if (tp[i] > p) break;

  pt = tp[i-1];
  p1 = p;
  r = 1.;

/*  Perform binary exponentiation algorithm modulo ak. */

  for (j = 1; j <= i; j++){
    if (p1 >= pt){
      r = 16. * r;
      r = r - (int) (r / ak) * ak;
      p1 = p1 - pt;
    }
    pt = 0.5 * pt;
    if (pt >= 1.){
      r = r * r;
      r = r - (int) (r / ak) * ak;
    }
  }

  return r;
}

static double series (int m, int id)

/*  This routine evaluates the series  sum_k 16^(id-k)/(8*k+m) 
    using the modular exponentiation technique. */

{
  int k;
  double ak, p, s, t;
#define eps 1e-17

  s = 0.;

/*  Sum the series up to id. */

  for (k = 0; k < id; k++){
    ak = 8 * k + m;
    p = id - k;
    t = expm (p, ak);
    s = s + t / ak;
    s = s - (int) s;
  }

/*  Compute a few terms where k >= id. */

  for (k = id; k <= id + 100; k++){
    ak = 8 * k + m;
    t = pow (16., (double) (id - k)) / ak;
    if (t < eps) break;
    s = s + t;
    s = s - (int) s;
  }
  return s;
}

unsigned char get_pi_byte(int id)
{
  double s1 = series (1, id);
  double s2 = series (4, id);
  double s3 = series (5, id);
  double s4 = series (6, id);
  double pid = 4. * s1 - 2. * s2 - s3 - s4;
  pid = pid - (int) pid + 1.;

  double y = fabs(pid);
  y = 16. * (y - floor (y));
  unsigned char first = y;
  y = 16. * (y - floor (y));
  unsigned char second = y;
  return (first << 4) | second;
}

/*
 * Generate hexadecimal digits of the fractional part of e using
 * e - 2 = 1/2! + 1/3! + ... represented as mixed-radix remainders.
 */
#define E_HEX_DIGIT_CACHE_SIZE (SHRT_MAX + 2)
#define E_DIGIT_TERMS 11200

static pthread_mutex_t e_mutex = PTHREAD_MUTEX_INITIALIZER;
static int e_initialized = 0;
static int e_hex_digit_count = 0;
static int e_remainders[E_DIGIT_TERMS + 1];
static unsigned char e_hex_digits[E_HEX_DIGIT_CACHE_SIZE];

static void init_e_remainders(void)
{
  if (e_initialized) {
    return;
  }

  for (int i = 1; i <= E_DIGIT_TERMS; i++) {
    e_remainders[i] = 1;
  }

  e_initialized = 1;
}

static unsigned char next_e_hex_digit(void)
{
  int carry = 0;

  for (int i = E_DIGIT_TERMS; i > 0; i--) {
    int value = 16 * e_remainders[i] + carry;
    e_remainders[i] = value % (i + 1);
    carry = value / (i + 1);
  }

  return (unsigned char) carry;
}

unsigned char get_e_byte(int id)
{
  if (id < 0 || id + 1 >= E_HEX_DIGIT_CACHE_SIZE) {
    return 0;
  }

  pthread_mutex_lock(&e_mutex);
  init_e_remainders();

  while (e_hex_digit_count <= id + 1) {
    e_hex_digits[e_hex_digit_count++] = next_e_hex_digit();
  }

  unsigned char ret = (e_hex_digits[id] << 4) | e_hex_digits[id + 1];
  pthread_mutex_unlock(&e_mutex);
  return ret;
}
