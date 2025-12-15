#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#include "config.h"
#include "libgreeting.h"
#include <string.h>

#define _(STRING) gettext(STRING)

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    if (argc > 1 && (strcmp(argv[1], "--help") == 0)) {
        printf(_("Usage: greeting [NAME]\n"));
        return 0;
    }

    printf("%s\n", get_greeting(argc > 1 ? argv[1] : NULL));
    return 0;
}
