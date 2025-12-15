#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/libgreeting.h"

int main() {
    // 测试默认情况
    const char *msg = get_greeting(NULL);
    assert(strstr(msg, "World") != NULL || strstr(msg, "Мир") != NULL);
    
    // 测试带参数情况
    msg = get_greeting("Tester");
    assert(strstr(msg, "Tester") != NULL);
    
    printf("All tests passed!\n");
    return 0;
}
