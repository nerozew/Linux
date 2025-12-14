#include <stdio.h>
#include <stdlib.h>

void print_help() {
    printf("Help: ./range N       -> [0, 1, ... N-1]\n");
    printf("      ./range M N     -> [M, M+1, ... N-1]\n");
    printf("      ./range M N S   -> [M, M+S, ... N-1]\n");
}

int main(int argc, char *argv[]) {
    // 定义变量，方便 GDB 调试时观察
    int start = 0;
    int end = 0;
    int step = 1;
    int current; 
    int i; // 用于计数的索引，方便脚本2使用

    if (argc < 2 || argc > 4) {
        print_help();
        return 0;
    }

    if (argc == 2) {
        // Case 1: ./range N
        start = 0;
        end = atoi(argv[1]);
        step = 1;
    } else if (argc == 3) {
        // Case 2: ./range M N
        start = atoi(argv[1]);
        end = atoi(argv[2]);
        step = 1;
    } else if (argc == 4) {
        // Case 3: ./range M N S
        start = atoi(argv[1]);
        end = atoi(argv[2]);
        step = atoi(argv[3]);
    }

    // 核心循环逻辑
    // 我们增加一个 i 计数器，专门为了满足作业中"第N个元素"的调试需求
    i = 0;
    for (current = start; (step > 0) ? (current < end) : (current > end); current += step) {
        printf("%d\n", current);
        i++; 
    }

    return 0;
}
