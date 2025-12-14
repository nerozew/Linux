#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>

#define MAX_GROUPS 10

void print_regerror(int errcode, regex_t *preg) {
    size_t length = regerror(errcode, preg, NULL, 0);
    char *buffer = malloc(length);
    if (buffer) {
        regerror(errcode, preg, buffer, length);
        fprintf(stderr, "Regex error: %s\n", buffer);
        free(buffer);
    } else {
        fprintf(stderr, "Regex error code: %d\n", errcode);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regexp> <substitution> <string>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *subst = argv[2];
    const char *input_str = argv[3];

    regex_t regex;
    regmatch_t pmatch[MAX_GROUPS];
    int reti;

    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        print_regerror(reti, &regex);
        regfree(&regex);
        return 1;
    }

    reti = regexec(&regex, input_str, MAX_GROUPS, pmatch, 0);
    if (reti == REG_NOMATCH) {
        printf("%s\n", input_str);
        regfree(&regex);
        return 0;
    } else if (reti) {
        print_regerror(reti, &regex);
        regfree(&regex);
        return 1;
    }

    printf("%.*s", (int)pmatch[0].rm_so, input_str);

    const char *p = subst;
    while (*p) {
        if (*p == '\\') {
            p++;
            if (*p == '\\') {
                putchar('\\');
            } else if (isdigit(*p)) {
                int group_idx = *p - '0';
                if (group_idx >= MAX_GROUPS) {
                     fprintf(stderr, "Error: Reference to \\%d out of bounds\n", group_idx);
                     regfree(&regex);
                     return 1;
                }
                if (group_idx > (int)regex.re_nsub) {
                    fprintf(stderr, "Error: Invalid backreference \\%d\n", group_idx);
                    regfree(&regex);
                    return 1;
                }
                if (pmatch[group_idx].rm_so != -1) {
                    int start = pmatch[group_idx].rm_so;
                    int end = pmatch[group_idx].rm_eo;
                    printf("%.*s", end - start, input_str + start);
                }
            } else {
                putchar(*p); 
            }
        } else {
            putchar(*p);
        }
        p++;
    }

    printf("%s\n", input_str + pmatch[0].rm_eo);

    regfree(&regex);
    return 0;
}
