#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define PROFILE_COUNT 1000000

/**
 * Returns the modulo such that a = b (mod n)
 * @param a The dividend
 * @param n The divisor
 */
int mod1(int a, int n) {
    return ((a % n)+n) % n;
}

/**
 * Returns the modulo such that a = b (mod n)
 * @param a The dividend
 * @param n The divisor
 */
int mod2(int a, int n) {
    return (int) (a - n * floor((double) a / n));
}

/**
 * Sets `seconds` to the number of seconds since Unix epoch including the
 * fractional value. Returns 0 if successful else returns the error value from
 * clock_gettime and errno is set appropriately.
 * @param seconds Pointer to value to be set
 * @return 0 on success
 */
int get_time(double *seconds) {
    double value;
    struct timespec ts;
    int err = clock_gettime(CLOCK_REALTIME, &ts);
    if (err) {
        return err;
    }
    value = (double) ts.tv_sec;
    value += ts.tv_nsec * 1e-9;
    if (seconds != NULL) {
        *seconds = value;
    }
    return 0;
}


/**
 * Returns the number of seconds (including fractional part) elapsed after
 * running `f` `count` times with arguments `a` and `n`. On error the errno
 * statement will be printed to stderr and the application will terminate with
 * an exit code of 1.
 * @param count Number of times to run `f`
 * @param f Pointer to function to run
 * @param a First argument to pass into the function
 * @param n Second argument to pass into the function
 * @return Number of elapsed seconds.
 */
double profile(uint32_t count, int (*f)(int a, int n), int a, int n) {
    double t0;
    double t1;
    if (get_time(&t0)) {
        perror("Failed to get time");
        exit(1);
    }
    if (f != NULL) {
        for (uint32_t i = 0; i < count; i++) {
            f(a, n);
        }
    }
    if (get_time(&t1)) {
        perror("Failed to get time");
        exit(1);
    }
    return t1 - t0;
}

/**
 * Profiles the different ways to calculate modulo
 */
int main(int argc, char *argv[]) {
    int a = 1;
    int n = 3;
    if (argc > 1) {
        sscanf(argv[1], "%d", &a);
        sscanf(argv[2], "%d", &n);
    }
    printf("(mod1) %d %% %d = %d\n", a, n, mod1(a, n));
    printf("(mod2) %d %% %d = %d\n", a, n, mod2(a, n));
    printf("--- Profile ---\n");
    double val1 = profile(PROFILE_COUNT, &mod1, a, n);
    printf("%f\n", val1);
    double val2 = profile(PROFILE_COUNT, &mod2, a, n);
    printf("%f\n", val2);
    return 0;
}
