#include <string.h>
#include <ctype.h>
#include "roman.h"

struct RomanDigit {
    int val;
    const char *sym;
};

static const struct RomanDigit table[] = {
    {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
    {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
};

int to_roman(int num, char *buf, size_t buf_size) {
    if (num < 1 || num > 100) return -1; // 作业要求1-100，若做扩展可改上限
    buf[0] = '\0';
    
    for (int i = 0; i < 9; i++) {
        while (num >= table[i].val) {
            if (strlen(buf) + strlen(table[i].sym) + 1 > buf_size) return -1;
            strcat(buf, table[i].sym);
            num -= table[i].val;
        }
    }
    return 0;
}

static int roman_char_val(char c) {
    switch (toupper(c)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        default: return 0;
    }
}

int from_roman(const char *str) {
    int total = 0;
    int prev_val = 0;
    
    // 从后往前遍历，逻辑更简单：IV -> V=5, I=1 < 5 -> 5-1=4
    for (int i = strlen(str) - 1; i >= 0; i--) {
        int val = roman_char_val(str[i]);
        if (val == 0) return -1; // 非法字符
        
        if (val < prev_val) total -= val;
        else total += val;
        
        prev_val = val;
    }
    return total;
}
