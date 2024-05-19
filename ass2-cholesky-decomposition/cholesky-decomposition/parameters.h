/*
In the test driver, there is a loop `for (p = PFIRST; p <= PLAST; p += PINC)`
The below parameters set this range of values that p takes on
*/
#define PFIRST 25
#define PLAST  1500
#define PINC   25

/*
In the test driver, the n dimensions are set to the below
values. If the value equals "-1" then that dimension is bound to the
index p, given above.
*/
#define N -1

/*
In the test driver, each experiment is repeated NREPEATS times and
the best time from these repeats is used to compute the performance
*/
#define NREPEATS 4
