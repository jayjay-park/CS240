#include "hw1.h"
#include <stdio.h>

/*
 * encode(int n, int l, int r)
 * This function encodes the subarray
 * from 1 till r in g_cip using ROT-n
 */

int encode(int n, int l, int r) {

  /* if l or r not valid indices of g_msg */

  if ((l > ARRAY_SIZE) || (r > ARRAY_SIZE) ||
      (r < 0) || (l < 0)) {
    return OUT_OF_BOUNDS;
  }

  /* if n is negative or l>r */

  if ((n < 0) || (l > r)) {
    return UNEXPECTED_INPUT;
  }

  /* copy the content of g_msg to g_cip */

  int index = 0;

  do {
    g_cip[index] = g_msg[index];
    index = index + 1;
  } while (index < ARRAY_SIZE);

  /* ROT-n */

  for (int i = l; i < r; i++) {

    int rem = 0;
    int rem_of_div = 0;
    char c = g_cip[i];

    /* first deal with multiples of 26
     * (number of alphabet) */

    if (n > 26) {
      rem_of_div = n % 26;
      if ((c >= 'a') && (c <= 'z')) {
        if (c + rem_of_div > 122) {
          rem = c + rem_of_div;
          rem = rem - 122;
          g_cip[i] = 97 + (rem - 1);
        }
        else {
          g_cip[i] = c + rem_of_div;
        }
      }
      else if ((c >= 'A') && (c <= 'Z')) {
        if (c + rem_of_div > 90) {
          rem = c + rem_of_div;
          rem = rem - 90;
          g_cip[i] = 65 + (rem - 1);
        }
        else {
          g_cip[i] = c + rem_of_div;
        }
      }
      else {
        g_cip[i] = c;
      }
    }
    else {
      if ((c >= 'a') && (c <= 'z')) {
        if (c + n > 122) {
          rem = c + n;
          rem = rem - 122;
          g_cip[i] = 97 + (rem - 1);
        }
        else {
          g_cip[i] = c + n;
        }
      }
      else if ((c >= 'A') && (c <= 'Z')) {
        if (c + n > 90) {
          rem = c + n;
          rem = rem - 90;
          g_cip[i] = 65 + (rem - 1);
        }
        else {
          g_cip[i] = c + n;
        }
      }
      else {
        g_cip[i] = c;
      }
    }
  }
  return SUCCESS;
} /* encode() */

/*
 * checksum()
 * This function performs a bitwise XOR operation
 * on all characters in g_msg
 */

int checksum() {

  int temp = 0;

  if (ARRAY_SIZE == 0) {
    return 0;
  }
  else {
    for (int j = 0; j < ARRAY_SIZE; j++) {
      printf("%d\n", g_msg[j]);
      temp ^= g_msg[j];
      printf("%d\n", temp);
    }
    return temp;
  }
} /* checksum() */
