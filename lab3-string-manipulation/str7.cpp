// requires additional changes to the code to make it work

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include <time.h>

#define REPEATS 50000

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock() {
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (gtod_ref_time_sec == 0.0)
    gtod_ref_time_sec = (double)tv.tv_sec;
  norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

void remove_ctrl(char *result, const char *s, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (s[i] >= 0x20) {
      *result++ = s[i];
    }
  }
  *result = '\0';
}

int main(int argc, const char *argv[]) {
  double dtime;

  std::cout << "call to remove\n";

  std::string s;

  std::string line;
  while (getline(std::cin, line)) {
    s += line + "\n";
  }

  char *result = (char *) malloc((s.length() + 1) * sizeof(char));

  dtime = dclock();
  for (int i = 0; i < REPEATS; i++) {
    remove_ctrl(result, s.data(), s.length());
  }
  dtime = dclock() - dtime;

  std::cout << result << "\n";
  std::cout << "Time: " << dtime << "\n";
  fflush(stdout);

  free(result);

  return EXIT_SUCCESS;
}
