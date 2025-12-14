#include <stdio.h>
#include <stdlib.h>

void print_help(void) {
    printf("Usage:\n");
    printf("  range N\n");
    printf("  range M N\n");
    printf("  range M N S\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 0;
    }

    long start, stop, step = 1;
    int count = 0;

    if (argc == 2) {
        start = 0;
        stop = atol(argv[1]);
    } else if (argc == 3) {
        start = atol(argv[1]);
        stop = atol(argv[2]);
    } else if (argc == 4) {
        start = atol(argv[1]);
        stop = atol(argv[2]);
        step = atol(argv[3]);
        if (step == 0) {
            fprintf(stderr, "Error: step cannot be zero.\n");
            return 1;
        }
    } else {
        print_help();
        return 1;
    }

    long current = start;
    while ((step > 0 && current < stop) || (step < 0 && current > stop)) {
        count++;
        printf("%ld\n", current);
        current += step;
    }

    return 0;
}
