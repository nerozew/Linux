#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>
#include "config.h" 
#include "roman.h"

#define _(STRING) gettext(STRING)

void print_help() {
    printf(_("Usage: guess [OPTIONS]\n"));
    printf(_("Guess the number game.\n\n"));
    printf(_("  -r         Use Roman numerals mode\n"));
    printf(_("  --help     Display this help and exit\n"));
    printf(_("  --version  Display version information and exit\n"));
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    int roman_mode = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            roman_mode = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            printf("guess %s\n", VERSION);
            return 0;
        }
    }

    printf(_("Think of a number between 1 and 100.\n"));
    /* 简化的游戏逻辑示例 */
    int left = 1, right = 100;
    char buf[100];
    
    while (left <= right) {
        int mid = (left + right) / 2;
        if (roman_mode) {
             char r_buf[32];
             to_roman(mid, r_buf, sizeof(r_buf));
             printf(_("Is your number greater than %s? (y/n): "), r_buf);
        } else {
             printf(_("Is your number greater than %d? (y/n): "), mid);
        }
        
        if (scanf("%99s", buf) != 1) break;
        if (strcasecmp(buf, "y") == 0) left = mid + 1;
        else if (strcasecmp(buf, "n") == 0) right = mid;
        else printf(_("Please answer y or n\n"));
        
        if (left == right) {
            if (roman_mode) {
                 char r_buf[32];
                 to_roman(left, r_buf, sizeof(r_buf));
                 printf(_("Your number is %s!\n"), r_buf);
            } else {
                 printf(_("Your number is %d!\n"), left);
            }
            break;
        }
    }
    return 0;
}

