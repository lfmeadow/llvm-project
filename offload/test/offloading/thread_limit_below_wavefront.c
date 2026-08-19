// A thread_limit smaller than the device's wavefront size silently skips the
// parallel region in generic mode. The kernel is launched with exactly
// thread_limit threads, but generic mode reserves a whole wavefront for the
// main thread and runs the parallel region on the threads left over, so below
// one wavefront there are none left and the region never executes. The program
// gets a wrong answer rather than a diagnostic.
//
// Only generic mode is affected, which is why this compiles without
// optimization: at -O1 and above the kernel becomes Generic-SPMD and the same
// source gives the right answer.

// RUN: %libomptarget-compile-generic
// RUN: %libomptarget-run-generic | %fcheck-generic

// REQUIRES: gpu


#include <stdio.h>

int main(void) {
  long count = 0;

#pragma omp target teams distribute num_teams(5) thread_limit(4)                \
    map(tofrom : count)
  for (int team = 0; team < 5; team++) {
#pragma omp parallel for
    for (int i = 0; i < 6; i++) {
#pragma omp atomic
      count += 1;
    }
  }

  printf("count = %ld\n", count);
  return count != 30;
}

// CHECK: count = 30
