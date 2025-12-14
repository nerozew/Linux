#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", ".");
    textdomain("guess");
    printf(_("Think of a number between 1 and 100.\n"));
    int low = 1, high = 100;
    char answer[100];
    while (low < high) {
        int mid = (low + high) / 2;
        printf(_("Is the number greater than %d? (yes/no)\n"), mid);
        if (scanf("%99s", answer) != 1) return 1;
        if (strcmp(answer, _("yes")) == 0) low = mid + 1;
        else if (strcmp(answer, _("no")) == 0) high = mid;
        else printf(_("Invalid input. Please answer 'yes' or 'no'.\n"));
    }
    printf(_("Your number is %d!\n"), low);
    return 0;
}
