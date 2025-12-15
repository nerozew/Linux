#include <stdio.h>
#include <libintl.h>
#include "config.h"
#include "libgreeting.h"

#define _(STRING) dgettext(PACKAGE, STRING)

const char* get_greeting(const char* name) {
    static char buffer[256];
    // 使用 dgettext 确保库也能正确翻译
    snprintf(buffer, sizeof(buffer), _("Hello, %s!"), name ? name : _("World"));
    return buffer;
}
